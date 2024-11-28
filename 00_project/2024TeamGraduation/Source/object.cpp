//=============================================================================
// 
//  オブジェクト処理 [object.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object.h"
#include "manager.h"
#include "camera.h"
#include "MyEffekseer.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CObject::m_nNumAll = 0;	// 総数
std::map<CObject::LAYER, std::map<int, std::vector<CObject*>>> CObject::m_pObj = {};	// オブジェクト格納用

//==========================================================================
// コンストラクタ
//==========================================================================
CObject::CObject(int nPriority, const LAYER layer)
{
	if (nPriority < 0 || nPriority >= mylib_const::PRIORITY_NUM) {
		return;
	}

	// 値のクリア
	m_pos = 0.0f;		// 位置
	m_posOld = 0.0f;	// 前回の位置
	m_rot = 0.0f;		// 向き
	m_move = 0.0f;		// 移動量
	m_rotOrigin = 0.0f;	// 元の向き

	m_nPriority = nPriority;	// 優先順位
	m_Layer = layer;			// レイヤー名
	m_type = TYPE_NONE;			// 種類
	m_bDeath = false;			// 死亡フラグ
	m_bDisp = true;				// 描画フラグ
	m_bPosibleMove_WorldPause = false;		// 世界停止中に動けるフラグ
	m_nNumAll++;				// 総数加算

	// オブジェクト格納
	m_pObj[layer][nPriority].emplace_back();
	m_pObj[layer][nPriority].back() = this;

}

//==========================================================================
// デストラクタ
//==========================================================================
CObject::~CObject()
{

}

//==========================================================================
// 全てのオブジェクトの破棄
//==========================================================================
void CObject::ReleaseAll()
{
	// 削除する要素
	std::map<LAYER, std::map<int, std::vector<CObject*>>> objectsToRemove;
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			for (auto itr = priority.second.begin(); itr != priority.second.end();)
			{
				CObject* pObj = (*itr);

				if (pObj->m_type != TYPE_NONE)
				{// NONEじゃなければ

					// 終了処理
					pObj->Uninit();

					// 後で消すvectorに格納
					objectsToRemove[layer.first][priority.first].push_back(*itr);
					itr = priority.second.erase(itr);
				}
				else
				{
					if (pObj->IsDeath())
					{
						// 終了処理
						pObj->Uninit();

						// 後で消すvectorに格納
						objectsToRemove[layer.first][priority.first].push_back(*itr);
						itr = priority.second.erase(itr);
					}
					else
					{
						itr++;
					}

				}
			}
		}
	}

	//*******************************
	// 死亡処理
	//*******************************
	for (auto& layer : objectsToRemove)
	{
		for (auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 総数減算
				m_nNumAll--;
				delete obj;
			}
		}
	}
	objectsToRemove.clear();
}

//==========================================================================
// 全てのオブジェクトの更新処理
//==========================================================================
void CObject::UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// マネージャー取得
	CManager* pMgr = CManager::GetInstance();
	if (pMgr == nullptr) return;

	// 世界の停止状況取得
	bool bWorldPause = pMgr->IsWorldPaused();

	// 更新フラグ
	bool bUpdate = false;

	// 削除する要素
	std::map<LAYER, std::map<int, std::vector<CObject*>>> objectsToRemove;
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			auto& vec = priority.second; // イテレータを失わないために参照を取得

			for (int i = 0; i < (int)vec.size(); i++)
			{
				CObject* pObj = priority.second[i];

				// 更新フラグ更新
				bUpdate = !bWorldPause || (bWorldPause && pObj->IsPosibleMove_WorldPause());

				if (bUpdate &&
					!pObj->m_bDeath &&
					pObj->m_type != TYPE_NONE)
				{// タイプがNONE以外

					// 更新処理
					pObj->Update(fDeltaTime, fDeltaRate, fSlowRate);
				}

#if 1
				if (pObj->m_bDeath)
				{
					// 後で消すvectorに格納
					objectsToRemove[layer.first][priority.first].push_back(pObj);
					priority.second.erase(std::find(priority.second.begin(), priority.second.end(), pObj)); // erase要素とitrをインクリメント
				}
#endif
			}
		}
	}

#if 1
	//*******************************
	// 死亡処理
	//*******************************
	for (auto& layer : objectsToRemove)
	{
		for (auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 総数減算
				m_nNumAll--;
				obj->Uninit();
				delete obj;
			}
		}
	}
	objectsToRemove.clear();
#endif
}

//==========================================================================
// 全てのオブジェクトの描画処理
//==========================================================================
void CObject::DrawAll()
{
	bool bEfkDraw = false;	// エフェクシアの描画
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			if (priority.first == mylib_const::PRIORITY_ZSORT)
			{// Zソート描画
				DrawZSort(layer.first, priority.first);
			}
			else
			{// 通常描画
				DrawNone(layer.first, priority.first);
			}

			if (layer.first == LAYER::LAYER_DEFAULT &&
				(priority.first == 3 || (priority.first > 3 && !bEfkDraw)))
			{
				// エフェクシアの更新兼描画
				CMyEffekseer* pEffekseer = CMyEffekseer::GetInstance();
				if (pEffekseer != nullptr)
				{
					CManager* pManager = GET_MANAGER;
					pEffekseer->Update(pManager->GetDeltaTime(), pManager->GetDeltaRate(), pManager->GetSlowRate());
					bEfkDraw = true;
				}
			}
		}
	}

	if (!bEfkDraw)
	{
		// エフェクシアの更新兼描画
		CMyEffekseer* pEffekseer = CMyEffekseer::GetInstance();
		if (pEffekseer != nullptr)
		{
			CManager* pManager = GET_MANAGER;
			pEffekseer->Update(pManager->GetDeltaTime(), pManager->GetDeltaRate(), pManager->GetSlowRate());
		}
	}
}

//==========================================================================
// 通常描画
//==========================================================================
void CObject::DrawNone(const LAYER layer, int nPriority)
{
	for (const auto& obj : m_pObj[layer][nPriority])
	{
		if (obj->m_bDisp &&
			obj->m_bDeath == false &&
			obj->m_type != TYPE_NONE)
		{// NONEじゃなければ
			obj->Draw();
		}
	}
}

//==========================================================================
// Zソート描画
//==========================================================================
void CObject::DrawZSort(const LAYER layer, int nPriority)
{
	// 要素コピー
	std::vector<CObject*> tempVector;

	// サイズ分確保
	tempVector.reserve(m_pObj[layer][nPriority].size());
	std::move(m_pObj[layer][nPriority].begin(), m_pObj[layer][nPriority].end(), std::back_inserter(tempVector));

	// Zソート
	std::sort(tempVector.begin(), tempVector.end(), ZSort);

	for (const auto& obj : tempVector)
	{
		if (obj->m_bDisp &&
			obj->m_bDeath == false &&
			obj->m_type != TYPE_NONE)
		{// NONEじゃなければ
			obj->Draw();
		}
	}
}

//==========================================================================
// スクリーン座標取得(Z座標)
//==========================================================================
float CObject::ScreenZ() const 
{
	// カメラの座標変換行列を取得
	D3DXMATRIX mtxCamera = CManager::GetInstance()->GetCamera()->GetMtxView();

	// オブジェクト座標をカメラからの座標に変換
	MyLib::Vector3 screenPos;
	D3DXVec3TransformCoord(&screenPos, &m_pos, &mtxCamera);

	// カメラ視点から見たZ座標
	float screenZ = -screenPos.z;

	return screenZ;
}

//==========================================================================
// Zソートの比較関数
//==========================================================================
bool CObject::ZSort(const CObject *obj1, const CObject *obj2)
{
	return obj1->ScreenZ() < obj2->ScreenZ();
}

//==========================================================================
// 逆Zソートの比較関数
//==========================================================================
bool CObject::ZSortInverse(const CObject* obj1, const CObject* obj2)
{
	return obj1->ScreenZ() > obj2->ScreenZ();
}

//==========================================================================
// オブジェクトの死亡判定
//==========================================================================
void CObject::Release()
{
	// 死亡フラグを立てる
	m_bDeath = true;
}

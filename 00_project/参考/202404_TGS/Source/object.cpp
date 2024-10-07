//=============================================================================
// 
//  オブジェクト処理 [object.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object.h"
#include "manager.h"
#include "3D_effect.h"
#include "camera.h"
#include "MyEffekseer.h"
#include "fog.h"
#include "baggage.h"
#include "game.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CObject::m_nNumAll = 0;	// 総数
int CObject::m_nNumPriorityAll[mylib_const::PRIORITY_NUM] = {};
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
	memset(&m_pEffect[0], 0, sizeof(m_pEffect));	// エフェクトのポインタ
	m_nNumEffectParent = 0;		// エフェクトの親設定した数
	m_bHitstopMove = false;		// ヒットストップ時に動くかのフラグ
	m_nNumAll++;				// 総数加算
	m_nNumPriorityAll[nPriority]++;

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
}

//==========================================================================
// 全てのオブジェクトの更新処理
//==========================================================================
void CObject::UpdateAll()
{

#if _DEBUG
	// エディットの情報取得
	CEdit* pEdit = CManager::GetInstance()->GetEdit();
#endif

	// ヒットストップ中か
	bool bHitstop = CManager::GetInstance()->IsHitStop();

	// 削除する要素
	std::map<LAYER, std::map<int, std::vector<CObject*>>> objectsToRemove;
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			auto& vec = priority.second; // イテレータを失わないために参照を取得

			for (int i = 0; i < vec.size(); i++)
			{
				CObject* pObj = priority.second[i];

#if _DEBUG
				if (!pObj->m_bDeath &&
					pEdit != nullptr &&
					(pObj->m_type == TYPE_EDIT || pObj->m_type == TYPE_XFILE) &&
					(pObj->m_bHitstopMove || (!pObj->m_bHitstopMove && !bHitstop)))
				{// エディット状態だったらエディットのみ更新

					// 更新処理
					pObj->Update();
				}
				else if (
					!pObj->m_bDeath &&
					pEdit == nullptr &&
					pObj->m_type != TYPE_NONE &&
					(pObj->m_bHitstopMove || (!pObj->m_bHitstopMove && !bHitstop)))
				{// エディット状態じゃない && タイプがNONE以外

					// 更新処理
					pObj->Update();
				}
#else
				if (!pObj->m_bDeath &&
					pObj->m_type != TYPE_NONE &&
					(pObj->m_bHitstopMove || (!pObj->m_bHitstopMove && !bHitstop)))
				{// タイプがNONE以外

					// 更新処理
					pObj->Update();
				}
#endif

				if (pObj->m_bDeath)
				{
					// 後で消すvectorに格納
					objectsToRemove[layer.first][priority.first].push_back(pObj);
					priority.second.erase(std::find(priority.second.begin(), priority.second.end(), pObj)); // erase要素とitrをインクリメント
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
				obj->Uninit();
				delete obj;
			}
		}
	}
	objectsToRemove.clear();
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
					pEffekseer->Update();
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
			pEffekseer->Update();
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
// 描画設定
//==========================================================================
void CObject::SetEnableDisp(bool bDisp)
{
	m_bDisp = bDisp;
}

//==========================================================================
// エフェクト設定
//==========================================================================
int CObject::SetEffectParent(CEffect3D *pEffect3D)
{
	int nIdx = 0;
	for (int nCntEffect = 0; nCntEffect < mylib_const::MAX_OBJ; nCntEffect++)
	{
		if (m_pEffect[nCntEffect] != nullptr)
		{// nullptrじゃなかったら
			continue;
		}

		// エフェクトのポインタを渡す
		m_pEffect[nCntEffect] = pEffect3D;
		nIdx = nCntEffect;

		// エフェクトの親設定した数加算
		m_nNumEffectParent++;
		break;
	}

	return nIdx;
}

//==========================================================================
// 親設定したエフェクトの数取得
//==========================================================================
int CObject::GetEffectParentNum()
{
	return m_nNumEffectParent;
}

//==========================================================================
// 親設定したエフェクト取得
//==========================================================================
CEffect3D *CObject::GetEffectParent(int nIdx)
{
	return m_pEffect[nIdx];
}

//==========================================================================
// エフェクトの解放
//==========================================================================
void CObject::ReleaseEffect(int nIdx)
{
	// エフェクトnullptrにする
	m_pEffect[nIdx] = nullptr;

	// エフェクトの親設定した数減算
	m_nNumEffectParent--;
}

//==========================================================================
// エフェクトの解放
//==========================================================================
void CObject::UninitEffect()
{
	for (int nCntEffect = 0; nCntEffect < mylib_const::MAX_OBJ; nCntEffect++)
	{
		if (m_pEffect[nCntEffect] == nullptr)
		{// nullptrだったら
			continue;
		}
		m_pEffect[nCntEffect]->UninitParent();
		m_pEffect[nCntEffect] = nullptr;
	}
}

//==========================================================================
// オブジェクトの死亡判定
//==========================================================================
void CObject::Release()
{
	// エフェクトの解放
	UninitEffect();

	// 死亡フラグを立てる
	m_bDeath = true;
}
//
////==========================================================================
//// オブジェクトの破棄・死亡処理
////==========================================================================
//void CObject::Death()
//{
//	
//}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject::SetVtx()
{

}

//==========================================================================
// 位置設定
//==========================================================================
void CObject::SetPosition(const MyLib::Vector3& pos)
{
	m_pos = pos;
}

//==========================================================================
// 位置加算
//==========================================================================
void CObject::AddPosition(const MyLib::Vector3& pos)
{
	m_pos += pos;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CObject::GetPosition() const
{
	return m_pos;
}

//==========================================================================
// 位置設定
//==========================================================================
void CObject::SetOldPosition(const MyLib::Vector3& posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CObject::GetOldPosition() const
{
	return m_posOld;
}

//==========================================================================
// 元の位置設定
//==========================================================================
void CObject::SetOriginPosition(const MyLib::Vector3& pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 元の位置取得
//==========================================================================
MyLib::Vector3 CObject::GetOriginPosition() const
{
	return m_posOrigin;
}

//==========================================================================
// 移動量設定
//==========================================================================
void CObject::SetMove(const MyLib::Vector3& move)
{
	m_move = move;
}

//==========================================================================
// 移動量加算
//==========================================================================
void CObject::AddMove(const MyLib::Vector3& move)
{
	m_move += move;
}

//==========================================================================
// 移動量取得
//==========================================================================
MyLib::Vector3 CObject::GetMove() const
{
	return m_move;
}

//==========================================================================
// 向き設定
//==========================================================================
void CObject::SetRotation(const MyLib::Vector3& rot)
{
	m_rot = rot;
}

//==========================================================================
// 向き加算
//==========================================================================
void CObject::AddRotation(const MyLib::Vector3& rot)
{
	m_rot += rot;
}

//==========================================================================
// 向き取得
//==========================================================================
MyLib::Vector3 CObject::GetRotation() const
{
	return m_rot;
}

//==========================================================================
// 元の向き設定
//==========================================================================
void CObject::SetOriginRotation(const MyLib::Vector3& rot)
{
	m_rotOrigin = rot;
}

//==========================================================================
// 元の向き取得
//==========================================================================
MyLib::Vector3 CObject::GetOriginRotation() const
{
	return m_rotOrigin;
}

//==========================================================================
// 種類の設定
//==========================================================================
void CObject::SetType(const TYPE type)
{
	m_type = type;
}

//==========================================================================
// 種類の取得
//==========================================================================
CObject::TYPE CObject::GetType() const
{
	return m_type;
}

//==========================================================================
// オブジェクトの取得
//==========================================================================
CObject *CObject::GetObject()
{
	return this;
}

//==========================================================================
// 死亡の判定
//==========================================================================
bool CObject::IsDeath()
{
	return m_bDeath;
}

//==========================================================================
// オブジェクト2Dオブジェクトの取得
//==========================================================================
CObject2D *CObject::GetObject2D()
{
	return nullptr;
}

//==========================================================================
// オブジェクト3Dオブジェクトの取得
//==========================================================================
CObject3D *CObject::GetObject3D()
{
	return nullptr;
}

//==========================================================================
// オブジェクト3Dメッシュオブジェクトの取得
//==========================================================================
CObject3DMesh *CObject::GetObject3DMesh()
{
	return nullptr;
}

//==========================================================================
// オブジェクトXオブジェクトの取得
//==========================================================================
CObjectX *CObject::GetObjectX()
{
	return nullptr;
}

//==========================================================================
// オブジェクトキャラクターオブジェクトの取得
//==========================================================================
CObjectChara *CObject::GetObjectChara()
{
	return nullptr;
}
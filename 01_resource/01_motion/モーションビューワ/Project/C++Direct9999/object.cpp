//=============================================================================
// 
//  オブジェクト処理 [object.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object.h"
#include "manager.h"

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CObject::m_nNumAll = 0;	// 総数
int CObject::m_nNumPriorityAll[mylib_const::PRIORITY_NUM] = {};
CObject *CObject::m_apObject[mylib_const::PRIORITY_NUM][mylib_const::MAX_OBJ] = {};

//==========================================================================
// コンストラクタ
//==========================================================================
CObject::CObject(int nPriority)
{
	if (nPriority < 0 || nPriority >= mylib_const::PRIORITY_NUM)
	{// 範囲外だったら

		return;
	}

	// 値のクリア
	m_nID = -1;

	for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
	{
		if (m_apObject[nPriority][nCntObj] == NULL)
		{// NULLだったら

			m_apObject[nPriority][nCntObj] = this;	// 自分自身のポインタを受け取る
			m_nPriority = nPriority;	// 優先順位
			m_nID = nCntObj;			// 自分自身のID設定
			m_type = TYPE_NONE;			// 種類
			m_nNumAll++;				// 総数加算
			m_nNumPriorityAll[nPriority]++;

			return;
		}
	}

	if (m_nID == -1)
	{
		int n = 0;
	}

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
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != NULL)
			{// メモリの確保がされていたら

				if (m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// NONEじゃなかったら

					// 終了処理
					m_apObject[nCntPriority][nCntObj]->Uninit();
				}
			}
		}
	}
}

//==========================================================================
// 全てのオブジェクトの更新処理
//==========================================================================
void CObject::UpdateAll(void)
{
#if _DEBUG
	// エディットの情報取得
	CEdit *pEdit = CManager::GetEdit();
#endif

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != NULL)
			{// メモリの確保がされていたら

#if _DEBUG
				if (pEdit != NULL && (m_apObject[nCntPriority][nCntObj]->m_type == TYPE_EDIT || m_apObject[nCntPriority][nCntObj]->m_type == TYPE_XFILE || m_apObject[nCntPriority][nCntObj]->m_type == TYPE_ELEVATION))
				{// エディット状態だったらエディットのみ更新

					// 更新処理
					m_apObject[nCntPriority][nCntObj]->Update();
				}
				else if (pEdit == NULL && m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// エディット状態じゃない && タイプがNONE以外

					// 更新処理
					m_apObject[nCntPriority][nCntObj]->Update();
				}
#else
				if (m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// タイプがNONE以外

					// 更新処理
					m_apObject[nCntPriority][nCntObj]->Update();
				}
#endif
			}
		}
	}
}

//==========================================================================
// 全てのオブジェクトの描画処理
//==========================================================================
void CObject::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			if (m_apObject[nCntPriority][nCntObj] != NULL)
			{// メモリの確保がされていたら

				if (m_apObject[nCntPriority][nCntObj]->m_type != TYPE_NONE)
				{// NONEじゃなかったら

					// 描画処理
					m_apObject[nCntPriority][nCntObj]->Draw();
				}
			}
		}
	}
}

//==========================================================================
// オブジェクトの破棄
//==========================================================================
void CObject::Release(void)
{
	// 今回の番号保存
	int nIdx = m_nID;
	int nPriority = m_nPriority;

	if (m_apObject[nPriority][nIdx] != NULL)
	{// メモリの確保がされていたら

		// メモリの開放
		delete m_apObject[nPriority][nIdx];
		m_apObject[nPriority][nIdx] = NULL;

		// 総数を減らす
		m_nNumAll--;
		m_nNumPriorityAll[nPriority]--;
	}
	else
	{
		int nn = 0;
	}
}

//==========================================================================
// 嘆きのバーチャルヒット処理
//==========================================================================
bool CObject::Hit(const int nValue)
{
	// 死んでない
	return false;
}


//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject::SetVtx(void)
{

}

//==========================================================================
// 位置設定
//==========================================================================
void CObject::SetPosition(const D3DXVECTOR3 pos)
{
	
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObject::GetPosition(void) const
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// 移動量設定
void CObject::SetMove(const D3DXVECTOR3 rot)
{

}

// 移動量取得
D3DXVECTOR3 CObject::GetMove(void) const 
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================================
// 向き設定
//==========================================================================
void CObject::SetRotation(const D3DXVECTOR3 rot)
{

}

//==========================================================================
// 向き取得
//==========================================================================
D3DXVECTOR3 CObject::GetRotation(void) const
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
CObject::TYPE CObject::GetType(void) const
{
	return m_type;
}

//==========================================================================
// オブジェクトの取得
//==========================================================================
CObject *CObject::GetObject(int nPriority, int nIdx)
{
	return m_apObject[nPriority][nIdx];
}

//==========================================================================
// オブジェクトの取得
//==========================================================================
CObject *CObject::GetObject(void)
{
	return this;
}

//==========================================================================
// オブジェクト2Dオブジェクトの取得
//==========================================================================
CObject2D *CObject::GetObject2D(void)
{
	return NULL;
}

//==========================================================================
// オブジェクト3Dオブジェクトの取得
//==========================================================================
CObject3D *CObject::GetObject3D(void)
{
	return NULL;
}

//==========================================================================
// オブジェクト3Dメッシュオブジェクトの取得
//==========================================================================
CObject3DMesh *CObject::GetObject3DMesh(void)
{
	return NULL;
}

//==========================================================================
// オブジェクトXオブジェクトの取得
//==========================================================================
CObjectX *CObject::GetObjectX(void)
{
	return NULL;
}

//==========================================================================
// オブジェクトキャラクターオブジェクトの取得
//==========================================================================
CObjectChara *CObject::GetObjectChara(void)
{
	return NULL;
}
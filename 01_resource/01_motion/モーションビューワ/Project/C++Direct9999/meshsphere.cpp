//=============================================================================
// 
//  爆発球処理 [meshsphere.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "meshsphere.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "objectchara.h"
#include "calculation.h"
#include "particle.h"
#include "debugproc.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TEXTURE		"data\\TEXTURE\\glass.jpg"	// テクスチャ

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CMeshSphere::CMeshSphere(int nPriority) : CObject3DMesh(nPriority)
{
	m_nTexIdx = 0;		// テクスチャのインデックス番号
}

//==========================================================================
// デストラクタ
//==========================================================================
CMeshSphere::~CMeshSphere()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, float fSize, const char *pFileName, int nPriority)
{
	// 生成用のオブジェクト
	CMeshSphere *pObjMeshField = NULL;

	if (pObjMeshField == NULL)
	{// NULLだったら

		// メモリの確保
		pObjMeshField = DEBUG_NEW CMeshSphere(nPriority);

		if (pObjMeshField != NULL)
		{// メモリの確保が出来ていたら

			// テクスチャの割り当て
			pObjMeshField->m_nTexIdx = CManager::GetTexture()->Regist(pFileName);

			// テクスチャの割り当て
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// 位置・向き
			pObjMeshField->SetPosition(pos);
			pObjMeshField->SetWidthBlock(16);
			pObjMeshField->SetHeightBlock(16);
			pObjMeshField->SetWidthLen(fSize);
			pObjMeshField->SetHeightLen(fSize);

			// 初期化処理
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return NULL;
}

//==========================================================================
// 生成処理(オーバーロード)
//==========================================================================
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, float fSize, int nTexIdx, int nPriority,int nBlock)
{
	// 生成用のオブジェクト
	CMeshSphere *pObjMeshField = NULL;

	if (pObjMeshField == NULL)
	{// NULLだったら

		// メモリの確保
		pObjMeshField = DEBUG_NEW CMeshSphere(nPriority);

		if (pObjMeshField != NULL)
		{// メモリの確保が出来ていたら

			// テクスチャの割り当て
			pObjMeshField->m_nTexIdx = nTexIdx;

			// テクスチャの割り当て
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// 位置・向き
			pObjMeshField->SetPosition(pos);
			pObjMeshField->SetWidthBlock(nBlock);
			pObjMeshField->SetHeightBlock(nBlock);
			pObjMeshField->SetWidthLen(fSize);
			pObjMeshField->SetHeightLen(fSize);

			// 初期化処理
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMeshSphere::Init(void)
{
	HRESULT hr;

	// 種類設定
	SetType(TYPE_BALLAST);

	// オブジェクト3Dメッシュの初期化処理
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_SPHERE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMeshSphere::Uninit(void)
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMeshSphere::Update(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 色取得
	D3DXCOLOR col = GetColor();

	// 横長さ取得
	float fWidthLen = GetWidthLen();

	// 縦長さ取得
	float fHeightLen = GetHeightLen();

	// 長さ設定
	SetWidthLen(fWidthLen);
	SetHeightLen(fHeightLen);

	// 頂点座標更新
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CMeshSphere::Draw(void)
{
	if (CManager::GetEditType() != CManager::EDITTYPE_SETUP)
	{// エディット状態なら
		return;
	}

	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// 埋めるモード


	// 描画処理
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// 埋めるモード

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// 目標のサイズ設定
//==========================================================================
void CMeshSphere::SetSizeDest(float fSize)
{
	m_fSizeDest = fSize;
}

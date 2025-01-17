//=============================================================================
// 
//  メッシュフィールド処理 [meshfield.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CMeshField::m_apFilename[] =		//ファイル読み込み
{
	"data\\TEXTURE\\glass.jpg",
	"data\\TEXTURE\\FIELD\\senter.png",
	"data\\TEXTURE\\FIELD\\senterlong.png",
	"data\\TEXTURE\\FIELD\\sideL.png",
	"data\\TEXTURE\\FIELD\\sideR.png",
	"data\\TEXTURE\\FIELD\\odan.png",
	"data\\TEXTURE\\FIELD\\walkroad.jpg",
	"data\\TEXTURE\\FIELD\\walkroad_01.jpg",
	"data\\TEXTURE\\FIELD\\tile_04.jpg",
	"data\\TEXTURE\\FIELD\\tile_03.jpg",
	"data\\TEXTURE\\FIELD\\water_01.jpg",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CMeshField::CMeshField(int nPriority) : CObject3DMesh(nPriority)
{
	m_type = TYPE_CONCREAT;	// 種類
	m_nTexIdx = 0;			// テクスチャのインデックス番号
}

//==========================================================================
// デストラクタ
//==========================================================================
CMeshField::~CMeshField()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMeshField *CMeshField::Create()
{
	// 生成用のオブジェクト
	CMeshField *pObjMeshField = nullptr;

	if (pObjMeshField == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjMeshField = DEBUG_NEW CMeshField;

		if (pObjMeshField != nullptr)
		{// メモリの確保が出来ていたら

			// 位置・向き
			pObjMeshField->SetWidthBlock(1);
			pObjMeshField->SetHeightBlock(1);
			pObjMeshField->SetWidthLen(100.0f);
			pObjMeshField->SetHeightLen(100.0f);

			// テクスチャの割り当て
			pObjMeshField->m_nTexIdx = CTexture::GetInstance()->Regist(m_apFilename[pObjMeshField->m_type]);

			// テクスチャの割り当て
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// 初期化処理
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return nullptr;
}

//==========================================================================
// 生成処理(オーバーロード)
//==========================================================================
CMeshField *CMeshField::Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float fWidthLen, float fHeightLen, int nWidth, int nHeight, TYPE type, const char *aFileName, int nPriority)
{
	// 生成用のオブジェクト
	CMeshField *pObjMeshField = nullptr;

	if (pObjMeshField == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjMeshField = DEBUG_NEW CMeshField(nPriority);

		if (pObjMeshField != nullptr)
		{// メモリの確保が出来ていたら

			// 位置・向き
			pObjMeshField->SetPosition(pos);
			pObjMeshField->SetRotation(rot);
			pObjMeshField->SetWidthBlock(nWidth);
			pObjMeshField->SetHeightBlock(nHeight);
			pObjMeshField->SetWidthLen(fWidthLen);
			pObjMeshField->SetHeightLen(fHeightLen);
			pObjMeshField->m_type = type;	// 種類

			if (aFileName == nullptr)
			{// nullptrだったら

				// テクスチャの割り当て
				pObjMeshField->m_nTexIdx = CTexture::GetInstance()->Regist(m_apFilename[pObjMeshField->m_type]);
			}
			else
			{// ファイル名が入っていたら

				// テクスチャの割り当て
				pObjMeshField->m_nTexIdx = CTexture::GetInstance()->Regist(aFileName);
			}

			// テクスチャの割り当て
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// 初期化処理
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMeshField::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 種類設定
	SetType(TYPE_MESHFIELD);

	// オブジェクト3Dメッシュの初期化処理
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMeshField::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMeshField::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 頂点情報設定
	//SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CMeshField::Draw()
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// 埋めるモード

	// 描画処理
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// 埋めるモード
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CMeshField::SetVtx()
{
	// 頂点情報更新
	CObject3DMesh::SetVtx();
}

//==========================================================================
// メッシュフィールドオブジェクトの取得
//==========================================================================
CMeshField *CMeshField::GetMyObject()
{
	return this;
}

//=============================================================================
// 
//  メッシュウォール処理 [meshwall.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "meshwall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// 無名ネームスペース
//==========================================================================
namespace
{
	const char* TEXTURE_DEFAULT = "data\\TEXTURE\\glass.jpg";	// デフォルトテクスチャ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CMeshWall::CMeshWall(int nPriority) : CObject3DMesh(nPriority)
{
	m_nTexIdx = 0;	// テクスチャのインデックス番号
}

//==========================================================================
// デストラクタ
//==========================================================================
CMeshWall::~CMeshWall()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMeshWall *CMeshWall::Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float fWidthLen, float fHeightLen, int nWidth, int nHeight, int nPriority, const char *aFileName)
{
	// 生成用のオブジェクト
	CMeshWall *pObjMeshField = nullptr;

	if (pObjMeshField == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjMeshField = DEBUG_NEW CMeshWall;

		if (pObjMeshField != nullptr)
		{// メモリの確保が出来ていたら

			// 位置・向き
			pObjMeshField->SetPosition(pos);
			//pObjMeshField->SetRotation(rot);
			pObjMeshField->SetOriginRotation(rot);
			pObjMeshField->SetWidthBlock(nWidth);
			pObjMeshField->SetHeightBlock(nHeight);
			pObjMeshField->SetWidthLen(fWidthLen);
			pObjMeshField->SetHeightLen(fHeightLen);

			if (aFileName == nullptr)
			{// nullptrだったら

				// テクスチャの割り当て
				pObjMeshField->m_nTexIdx = CTexture::GetInstance()->Regist(TEXTURE_DEFAULT);
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
HRESULT CMeshWall::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 種類設定
	SetType(TYPE_MESHWALL);

	// オブジェクト3Dメッシュの初期化処理
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_WALL);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMeshWall::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMeshWall::Update()
{
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CMeshWall::Draw()
{
	// 描画処理
	CObject3DMesh::Draw();
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CMeshWall::SetVtx()
{
	// 頂点情報更新
	CObject3DMesh::SetVtx();
}

//==========================================================================
// メッシュフィールドオブジェクトの取得
//==========================================================================
CMeshWall *CMeshWall::GetMyObject()
{
	return this;
}

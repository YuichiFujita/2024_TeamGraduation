//=============================================================================
// 
//  目標地点処理 [ascensionCylinder.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "ascensionCylinder.h"
#include "input.h"
#include "calculation.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "elevation.h"
#include "game.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\GRADATION\\orange_02.jpg";
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CAscensionCylinder::CAscensionCylinder(int nPriority) : CMeshCylinder(nPriority)
{
	

}

//==========================================================================
// デストラクタ
//==========================================================================
CAscensionCylinder::~CAscensionCylinder()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CAscensionCylinder* CAscensionCylinder::Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen)
{
	// メモリの確保
	CAscensionCylinder* pObjMeshCylinder = DEBUG_NEW CAscensionCylinder;

	if (pObjMeshCylinder != nullptr)
	{
		// 位置・向き
		pObjMeshCylinder->SetPosition(pos);
		pObjMeshCylinder->SetWidthBlock(16);
		pObjMeshCylinder->SetHeightBlock(1);
		pObjMeshCylinder->SetWidthLen(fWidthLen);
		pObjMeshCylinder->SetHeightLen(fHeightLen);

		// 初期化処理
		pObjMeshCylinder->Init();
	}

	return pObjMeshCylinder;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAscensionCylinder::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 種類設定
	SetType(CObject::TYPE_OBJECT3D);

	// メッシュシリンダーの初期化処理
	CMeshCylinder::Init();

	// テクスチャの割り当て
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAscensionCylinder::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CAscensionCylinder::Update()
{

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CAscensionCylinder::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 背面のカリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject3DMesh::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングデフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

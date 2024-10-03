//=============================================================================
// 
//  ステンシルバッファ使用影[stencilshadow.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "stencilshadow.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// コンストラクタ
//==========================================================================
CStencilShadow::CStencilShadow(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CStencilShadow::~CStencilShadow()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStencilShadow* CStencilShadow::Create()
{
	// メモリの確保
	CStencilShadow* pObj = DEBUG_NEW CStencilShadow;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();

		// 情報設定
		pObj->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		pObj->SetSize(D3DXVECTOR2(640.0f, 360.0f));
		pObj->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));
		pObj->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f));
		pObj->SetVtx();

	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStencilShadow::Init()
{
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStencilShadow::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CStencilShadow::Kill()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CStencilShadow::Update()
{
	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CStencilShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ステンシルバッファ有効（ステンシル使用描画）
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// バッファへの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	//テスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	// Z+ステンシル成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	// Z+ステンシル失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	// Zテストのみ失敗

	// 描画
	CObject2D::Draw();

	// ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}


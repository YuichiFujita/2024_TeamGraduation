//============================================================
//
//	スクリーン処理 [screen.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "screen.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	子クラス [CScreen] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScreen::CScreen() : CObject2D(7)
{

}

//============================================================
//	デストラクタ
//============================================================
CScreen::~CScreen()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScreen::Init()
{
	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 種類の指定を無しにする
	SetType(CObject::TYPE::TYPE_NONE);

	// 位置をスクリーン中央にする
	SetPosition(VEC2_SCREEN_CENT);

	// 大きさをスクリーンサイズにする
	SetSize(VEC2_SCREEN_SIZE * 0.5f);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScreen::Uninit()
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CScreen::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// オブジェクト2Dの更新
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	描画処理
//============================================================
void CScreen::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// サンプラーステートを設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// U方向のラッピングを無効化
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// V方向のラッピングを無効化

	// オブジェクト2Dの描画
	CObject2D::Draw();

	// サンプラーステートを設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U方向のラッピングを有効化
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V方向のラッピングを有効化
}

//============================================================
//	生成処理
//============================================================
CScreen* CScreen::Create(const int nScreenTexIdx)
{
	// スクリーンの生成
	CScreen* pScreen = DEBUG_NEW CScreen;
	if (pScreen == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// スクリーンの初期化
		if (FAILED(pScreen->Init()))
		{ // 初期化に失敗した場合

			// スクリーンの破棄
			SAFE_DELETE(pScreen);
			return nullptr;
		}

		// テクスチャを登録・割当
		pScreen->BindTexture(nScreenTexIdx);

		// 確保したアドレスを返す
		return pScreen;
	}
}

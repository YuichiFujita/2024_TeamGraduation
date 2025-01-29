//=============================================================================
// 
//  チュートリアル処理 [tutorial.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "calculation.h"
#include "sound.h"
#include "MyEffekseer.h"
#include "camera.h"
#include "tutorialscreen.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CTutorial* CTutorial::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CTutorial::CTutorial() : 
	m_pTutorialScreen	(nullptr)	// チュートリアル画面
{
	// 値のクリア
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CTutorial::~CTutorial()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTutorial* CTutorial::Create()
{
	if (m_pThisPtr == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pThisPtr = DEBUG_NEW CTutorial;
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTutorial::Init()
{
	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// 画面生成
	m_pTutorialScreen = CTutorialScreen::Create();
	if (m_pTutorialScreen == nullptr) return E_FAIL;

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTutorial::Uninit()
{
	m_pThisPtr = nullptr;

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTutorial::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->GetState() != CFade::STATE_NONE) { return; }

	// 入力情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_A).bInput ||
		pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_B).bInput ||
		pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_START).bInput)
	{
		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_TITLE);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTutorial::Draw()
{

}

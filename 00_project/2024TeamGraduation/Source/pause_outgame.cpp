//==========================================================================
// 
//  アウトゲーム時ポーズ処理 [pause_outgame.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "pause_outgame.h"
#include "manager.h"
#include "fade.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPause_OutGame::CPause_OutGame()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPause_OutGame::~CPause_OutGame()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPause_OutGame::Init()
{
	// 初期化処理
	CPause::Init();

	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPause_OutGame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キーボード情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();

#if _DEBUG
	if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE)
	{
		if (pKey->GetTrigger(DIK_P) ||
			pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_START).bInput)
		{// ポーズ
			m_bPause = !m_bPause;

			// サウンドの再生
			PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
		}
	}
#endif // _DEBUG

	// 親の更新
	CPause::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// モード別決定処理
//==========================================================================
void CPause_OutGame::DecideByMode()
{
	// リトライする
	CManager::GetInstance()->GetFade()->SetFade(CManager::GetInstance()->GetMode());
}

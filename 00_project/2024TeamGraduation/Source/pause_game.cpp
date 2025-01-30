//==========================================================================
// 
//  ゲーム時ポーズ処理 [pause_game.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "pause_game.h"
#include "manager.h"
#include "fade.h"
#include "gamemanager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\pause\\retry.png";	// テクスチャファイル
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPause_Game::CPause_Game()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPause_Game::~CPause_Game()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPause_Game::Init()
{
	// 初期化処理
	CPause::Init();

	// テクスチャ切り替え
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	m_aObject2D[VTX::VTX_RETRY]->BindTexture(nTexIdx);

	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPause_Game::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キーボード情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE
	&&  CGameManager::GetInstance()->GetType() == CGameManager::ESceneType::SCENE_MAIN)
	{
		if (pKey->GetTrigger(DIK_P) ||
			pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_START).bInput)
		{// ポーズ
			m_bPause = !m_bPause;

			// サウンドの再生
			PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
		}
	}

	// 親の更新
	CPause::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// モード別決定処理
//==========================================================================
void CPause_Game::DecideByMode()
{
	// サウンド再生
	//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DONE);

	// ゲームをリトライする
	CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
}

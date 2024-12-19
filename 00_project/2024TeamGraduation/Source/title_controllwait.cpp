//=============================================================================
// 
// 操作待機処理 [title_controllwait.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "title_controllwait.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "titleLogo.h"
#include "titlestudent.h"
#include "fade.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int PLAYER_MIN = 10;	// プレイヤー最少人数
	const int PLAYER_MAX = 14;	// プレイヤー最大人数
}

namespace StateTime
{
	const float WAIT = 15.0f;	// 待機
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle_ControllWait::STATE_FUNC CTitle_ControllWait::m_StateFunc[] =	// 状態関数
{
	&CTitle_ControllWait::StateWait,		// 待機
	&CTitle_ControllWait::StateControll,	// 操作
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle_ControllWait::CTitle_ControllWait() : CTitleScene()
{
	// 状態
	m_state = EState::STATE_WAIT;		// 状態
	m_fStateTime = 0.0f;				// 状態時間

	// その他
	m_pLogo = nullptr;			// ロゴ
	m_vecTitleStudent.clear();	// タイトルの生徒
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitle_ControllWait::~CTitle_ControllWait()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitle_ControllWait::Init()
{
	// 初期化処理
	if (FAILED(CTitleScene::Init())) return E_FAIL;

	// ロゴの生成
	m_pLogo = CTitleLogo::Create();
	if (m_pLogo == nullptr) return E_FAIL;

	// プレイヤー生成
	int num = UtilFunc::Transformation::Random(PLAYER_MIN, PLAYER_MAX);
	for (int i = 0; i < num; i++)
	{
		// タイトルの生徒生成
		m_vecTitleStudent.push_back(CTitleStudent::Create());
		if (m_vecTitleStudent.back() == nullptr) return E_FAIL;
	}

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::ELabel::LABEL_BGM_TITLE);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle_ControllWait::Uninit()
{
	delete this;
}

//==========================================================================
// 削除処理
//==========================================================================
void CTitle_ControllWait::Kill()
{
	// ロゴ
	SAFE_KILL(m_pLogo);

	// 生徒
	for (int i = 0; i < static_cast<int>(m_vecTitleStudent.size()); i++)
	{
		SAFE_KILL(m_vecTitleStudent[i]);
	}

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle_ControllWait::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() != CInstantFade::EState::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// 親の更新
	CTitleScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
	}
}

//==========================================================================
// 状態更新
//==========================================================================
void CTitle_ControllWait::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 待機
//==========================================================================
void CTitle_ControllWait::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		// シーン切り替え
		//CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_SUSURU);
	}
}

//==========================================================================
// 操作
//==========================================================================
void CTitle_ControllWait::StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;
}

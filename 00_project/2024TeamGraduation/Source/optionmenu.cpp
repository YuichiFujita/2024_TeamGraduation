//=============================================================================
// 
//  オプションメニュー処理 [optionmenu.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "optionmenu.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "object2D.h"

// 派生先
#include "optionmenu_sound.h"
#include "optionmenu_keyconfig.h"
#include "optionmenu_other.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

namespace StateTime	// 状態別時間
{
	const float FADE = 0.3f;	// フェード
}

//==========================================================================
// 関数ポインタ
//==========================================================================
COptionMenu::STATE_FUNC COptionMenu::m_StateFunc[] =
{
	&COptionMenu::StateNone,	// なし
	&COptionMenu::StateEdit,	// 編集
	&COptionMenu::StateFadeIn,	// フェードイン
	&COptionMenu::StateFadeOut,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
COptionMenu::COptionMenu(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_fStateTimer = 0.0f;			// 状態タイマー

}

//==========================================================================
// デストラクタ
//==========================================================================
COptionMenu::~COptionMenu()
{

}

//==========================================================================
// 生成処理
//==========================================================================
COptionMenu* COptionMenu::Create(CTitle_OptionSelect::Select selectType)
{
	// メモリの確保
	COptionMenu* pObj = nullptr;

	switch (selectType)
	{
	case CTitle_OptionSelect::SELECT_KEYKONFIG:
		pObj = DEBUG_NEW COptionMenu_Keyconfig;
		break;

	case CTitle_OptionSelect::SELECT_SOUND:
		pObj = DEBUG_NEW COptionMenu_Sound;
		break;

	case CTitle_OptionSelect::SELECT_OTHER:
		pObj = DEBUG_NEW COptionMenu_Other;
		break;

	default:
		return pObj;
		break;
	}

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT COptionMenu::Init()
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 状態
	SetState(STATE::STATE_FADEIN);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void COptionMenu::Uninit()
{
	

	// 終了処理
	CObject::Release();
}

//==========================================================================
// 削除
//==========================================================================
void COptionMenu::Kill()
{
	
	// 終了処理
	CObject::Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void COptionMenu::Update()
{
	// 状態更新
	UpdateState();

	if (IsDeath()) return;
}

//==========================================================================
// 状態更新
//==========================================================================
void COptionMenu::UpdateState()
{
	// 状態タイマー
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// なにもなし
//==========================================================================
void COptionMenu::StateNone()
{
	
}

//==========================================================================
// 編集
//==========================================================================
void COptionMenu::StateEdit()
{

}

//==========================================================================
// フェードイン
//==========================================================================
void COptionMenu::StateFadeIn()
{
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	// 時間経過
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_NONE);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void COptionMenu::StateFadeOut()
{
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	// 時間経過
	if (m_fStateTimer >= StateTime::FADE)
	{
		Kill();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void COptionMenu::Draw()
{
	
}

//==========================================================================
// 状態設定
//==========================================================================
void COptionMenu::SetState(const STATE& state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}

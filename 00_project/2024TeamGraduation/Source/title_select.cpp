//=============================================================================
// 
// タイトルエンター処理 [title_select.cpp]
// Author : Ibuki Okusada
// 
//=============================================================================
#include "title_select.h"
#include "title.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "input.h"
#include "fade.h"
#include "keyconfig.h"
#include "title_pressenter.h"
#include "title_optionselect.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TIME_TUTORIAL_FADEOUT = 0.3f;	// チュートリアル確認のフェードアウト
	const MyLib::Vector3 SET_POS = MyLib::Vector3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.85f, 0.0f);
	const float HEIGHT = 70.0f;
	const D3DXCOLOR COL_NONESELECT = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	D3DXCOLOR GetColorNonSelect(float alpha) {
		return D3DXCOLOR(0.4f, 0.4f, 0.4f, alpha);
	}
}

namespace FILENAME
{
	const std::string TEXTURE[CTitle_Select::SELECT_MAX] =
	{
		"data\\TEXTURE\\title\\start.png",
		"data\\TEXTURE\\title\\option.png",
	};

	const std::string BG = "data\\TEXTURE\\ui_setting\\front.png";
}

namespace StateTime
{
	const float FADE = 0.3f;
	const float DRAW = 0.15f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle_Select::STATE_FUNC CTitle_Select::m_StateFunc[] =
{
	&CTitle_Select::StateNone,		// なし
	&CTitle_Select::StateFadeIn,	// フェードイン
	&CTitle_Select::StateFadeOut,	// フェードアウト
	&CTitle_Select::StateTutorial_FadeOut,		// チュートリアル確認のフェードアウト
	&CTitle_Select::StateNoActive,	// 反応しない
	&CTitle_Select::StateSetting,	// 設定中
	&CTitle_Select::StateBack,		// 戻る
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle_Select::CTitle_Select(float fadetime) : CObject() , m_fFadeOutTime(fadetime)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_fStateTime = 0.0f;			// 状態カウンター
	m_fSelectDrawTime = 0.0f;		// 選択肢の書く時間
	m_nSelect = 0;

	for (int i = 0; i < SELECT_MAX; i++)
	{
		m_ap2D[i] = nullptr;
	}
	m_pOptionSelect = nullptr;	// オプションの選択肢

	m_pSelect = nullptr;
	m_bPress = false;
}

//==========================================================================
// 生成処理
//==========================================================================
CTitle_Select* CTitle_Select::Create(float fadetime)
{
	// メモリの確保
	CTitle_Select* pScreen = DEBUG_NEW CTitle_Select(fadetime);

	if (pScreen != nullptr)
	{
		// 初期化処理
		pScreen->Init();
	}

	return pScreen;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitle_Select::Init()
{
	// 種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);


	CTexture* pTexture = CTexture::GetInstance();

	MyLib::Vector3 pos = SET_POS;

	//=============================
	// 背景を生成
	//=============================
	m_pSelect = CObject2D::Create(8);
	m_pSelect->SetType(CObject::TYPE_OBJECT2D);
	m_pSelect->SetPosition(pos);
	m_pSelect->SetAlpha(0.0f);
	m_pSelect->SetAnchorType(CObject2D::AnchorPoint::LEFT);

	// テクスチャ設定
	m_pSelect->BindTexture(pTexture->Regist(FILENAME::BG));
	D3DXVECTOR2 texture = pTexture->GetImageSize(m_pSelect->GetIdxTexture());
	D3DXVECTOR2 setsize = UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT * 1.5f);

	m_pSelect->SetSize(D3DXVECTOR2(0.0f, setsize.y));
	m_pSelect->SetSizeOrigin(setsize);

	//=============================
	// 選択肢を生成
	//=============================
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] == nullptr)
		{
			// 生成
			m_ap2D[i] = CObject2D::Create(8);
			CObject2D* pObj2D = m_ap2D[i];

			pObj2D->SetType(CObject::TYPE_OBJECT2D);
			pObj2D->SetPosition(pos);
			pObj2D->SetAlpha(0.0f);

			// テクスチャ設定
			pObj2D->BindTexture(pTexture->Regist(FILENAME::TEXTURE[i]));
			D3DXVECTOR2 texture = pTexture->GetImageSize(pObj2D->GetIdxTexture());
			pObj2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT));
			pObj2D->SetSizeOrigin(pObj2D->GetSize());
		}

		pos.x += SCREEN_WIDTH * 0.4f;
	}

	// 状態遷移
	SetState(STATE::STATE_FADEIN);
	m_pSelect->SetPosition(m_ap2D[m_nSelect]->GetPosition() - MyLib::Vector3(m_pSelect->GetSizeOrigin().x, 0.0f, 0.0f));

	// 色設定
	for (int i = 0; i < SELECT_MAX; i++)
	{
		D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ap2D[i]->GetAlpha());
		if (m_nSelect != i)
		{
			col = GetColorNonSelect(col.a);
		}
		m_ap2D[i]->SetColor(col);
	}

	// オプション選択肢の生成
	if (m_pOptionSelect == nullptr)
	{
		m_pOptionSelect = CTitle_OptionSelect::Create();
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle_Select::Uninit()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		m_ap2D[i] = nullptr;
	}
	m_pSelect = nullptr;

	Release();
}

//==========================================================================
// 削除
//==========================================================================
void CTitle_Select::Kill()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->Uninit();
			m_ap2D[i] = nullptr;
		}
	}

	if (m_pSelect != nullptr)
	{
		m_pSelect->Uninit();
		m_pSelect = nullptr;
	}

	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle_Select::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 選択肢を書く
	DrawSelect();

	// 状態別更新処理
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// 選択肢を書く
//==========================================================================
void CTitle_Select::DrawSelect()
{
	// 書く時間加算
	m_fSelectDrawTime += CManager::GetInstance()->GetDeltaTime();

	// 書くように拡縮
	D3DXVECTOR2 size = m_pSelect->GetSize(), sizeOrigin = m_pSelect->GetSizeOrigin();
	size.x = UtilFunc::Correction::EaseInExpo(0.0f, sizeOrigin.x, 0.0f, StateTime::DRAW, m_fSelectDrawTime);
	m_pSelect->SetSize(size);

	// UV座標設定
	D3DXVECTOR2* pTex = m_pSelect->GetTex();
	pTex[1].x = pTex[3].x = UtilFunc::Transformation::Clamp(size.x / sizeOrigin.x, 0.0f, 1.0f);
}


//==========================================================================
// なにもなし
//==========================================================================
void CTitle_Select::StateNone()
{
	// 入力情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (m_bPress)
	{
		if (!pKeyConfigPad->GetPress(INGAME::ACT_BACK))
		{
			m_bPress = false;
		}
		return;
	}

	//=============================
	// 方向入力
	//=============================
	
	bool bLeft = (pInputGamepad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pInputGamepad->GetStickMoveL(0).x < 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pInputKeyboard->GetTrigger(DIK_A);

	bool bRight = (pInputGamepad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pInputGamepad->GetStickMoveL(0).x > 0) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pInputKeyboard->GetTrigger(DIK_D);
	
	if (bLeft || bRight)
	{
		// 切り替え
		m_nSelect ^= 1;
		m_pSelect->SetPosition(m_ap2D[m_nSelect]->GetPosition() - MyLib::Vector3(m_pSelect->GetSizeOrigin().x, 0.0f, 0.0f));

		// 書く時間リセット
		m_fSelectDrawTime = 0.0f;
		m_pSelect->SetSize(D3DXVECTOR2(0.0f, m_pSelect->GetSize().y));

		// 色設定
		for (int i = 0; i < SELECT_MAX; i++)
		{
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_ap2D[i]->GetAlpha());
			if (m_nSelect != i)
			{
				col = GetColorNonSelect(col.a);
			}
			m_ap2D[i]->SetColor(col);
		}

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_CURSOR_MOVE);
		return;
	}

	//=============================
	// 決定
	//=============================
	if (pKeyConfigPad->GetTrigger(INGAME::ACT_OK) || pKeyConfigKey->GetTrigger(INGAME::ACT_OK))
	{
		switch (m_nSelect)
		{
		case SELECT::SELECT_START:	// ゲーム開始
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_GAME);
			break;

		case SELECT::SELECT_OPTION:	// オプション
			
			// オプション決定時の設定
			DecideOptionSetting();	


			// 状態遷移
			SetState(STATE::STATE_FADEOUT);
			break;
		}
	}

	//=============================
	// 戻る
	//=============================
	if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) || pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		SetState(STATE::STATE_BACK);

		// プレスエンター呼び戻し
		CTitle_PressEnter* pEnter = CTitle::GetInstance()->GetTitlePressEnter();
		if (pEnter != nullptr)
		{
			pEnter->SetState(CTitle_PressEnter::STATE::STATE_FADEIN);
		}

	}
}

//==========================================================================
// オプション決定時の設定
//==========================================================================
void CTitle_Select::DecideOptionSetting()
{
	// 状態遷移
	SetState(STATE::STATE_FADEOUT);

	// オプション選択肢の状態設定
	if (m_pOptionSelect != nullptr)
	{
		m_pOptionSelect->SetState(CTitle_OptionSelect::STATE::STATE_SCROLLWAIT);
	}

}

//==========================================================================
// フェードイン
//==========================================================================
void CTitle_Select::StateFadeIn()
{
	// 不透明度更新
	float alpha = m_fStateTime / StateTime::FADE;
	
	if (m_fStateTime >= StateTime::FADE)
	{
		// 状態遷移
		SetState(STATE::STATE_NONE);
		alpha = 1.0f;
	}


	// 背景の不透明度
	m_pSelect->SetAlpha(alpha);

	// 選択肢の不透明度
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CTitle_Select::StateFadeOut()
{
	// 不透明度更新
	float alpha = 1.0f - (m_fStateTime / StateTime::FADE);


	if (m_fStateTime >= StateTime::FADE)
	{
		// 状態遷移
		SetState(STATE::STATE_SETTING);
		alpha = 0.0f;
	}

	// 背景の不透明度
	m_pSelect->SetAlpha(alpha);

	// 選択肢の不透明度
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// チュートリアル確認のフェードアウト
//==========================================================================
void CTitle_Select::StateTutorial_FadeOut()
{
	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_TUTORIAL_FADEOUT, m_fStateTime);

	if (m_fStateTime >= TIME_TUTORIAL_FADEOUT)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_NONE;
	}
}


//==========================================================================
// 反応しない
//==========================================================================
void CTitle_Select::StateNoActive()
{
	
}

//==========================================================================
// 設定中
//==========================================================================
void CTitle_Select::StateSetting()
{
	m_bPress = true;
}

//==========================================================================
// 戻る
//==========================================================================
void CTitle_Select::StateBack()
{
	// 不透明度更新
	float alpha = 1.0f - (m_fStateTime / StateTime::FADE);
	alpha = UtilFunc::Transformation::Clamp(alpha, 0.0f, 1.0f);

	if (m_fStateTime >= StateTime::FADE)
	{
		SetState(STATE::STATE_NOACTIVE);
	}

	// 背景の不透明度
	m_pSelect->SetAlpha(alpha);

	// 選択肢の不透明度
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitle_Select::SetState(STATE state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画
//==========================================================================
void CTitle_Select::Draw()
{
	return;
}
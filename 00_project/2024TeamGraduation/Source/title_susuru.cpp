//=============================================================================
// 
// SUSURUシーン処理 [title_susuru.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "title_susuru.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "dressup.h"
#include "playerManager.h"
#include "titlestudent.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\title\\susuru.png";		// テクスチャ
	const MyLib::Vector2 TEXTSIZE = MyLib::Vector2((SCREEN_WIDTH / (CTitle_SUSURU::NUM_TEXT + 1)) * 0.75f, SCREEN_HEIGHT * 0.5f);	// サイズ
}

namespace StateTime
{
	const float WAIT = 0.2f;		// 待機
	float SCROLL[CTitle_SUSURU::NUM_TEXT] = // 送り
	{
		2.7f,	// 俺はただ
		3.4f,	// でも、気づいたんだよ
		2.1f,	// この学園でトップを目指すためには
		1.25f,	// 「モテ」なきゃいけない
		3.2f,	// スポーツも勉強も
		2.3f,	// モテなきゃ意味がない
		2.5f,	// 必ずこの学園の頂点
		2.85f,	// いや「モテ王」の座に
		1.5f,	// 待っていろよ
		4.0f,	// 俺の輝く魅力で
	};
	const float END = 2.0f;		// 終了
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle_SUSURU::STATE_FUNC CTitle_SUSURU::m_StateFunc[] =	// 状態関数
{
	&CTitle_SUSURU::StateWait,		// 待機
	&CTitle_SUSURU::StateScroll,	// 文字送り
	&CTitle_SUSURU::StateEnd,		// 終了
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle_SUSURU::CTitle_SUSURU() : CTitleScene()
{
	// 状態
	m_state = EState::STATE_WAIT;		// 状態
	m_fStateTime = 0.0f;				// 状態時間

	// その他
	m_nIdxScroll = 0;		// 送る文字インデックス
	memset(&m_apText, 0, sizeof(m_apText));	// 文字
	m_pSUSURU = nullptr;	// SUSURUさん
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitle_SUSURU::~CTitle_SUSURU()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitle_SUSURU::Init()
{
	// 初期化処理
	if(FAILED(CTitleScene::Init())) return E_FAIL;

	// テキスト生成
	if (FAILED(CreateText())) return E_FAIL;

	// SUSURUさん生成
	if (FAILED(CreateSUSURU())) return E_FAIL;

	// 送る文字インデックス
	m_nIdxScroll = -1;

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::ELabel::LABEL_BGM_TITLE_SUSURU);

	return S_OK;
}

//==========================================================================
// テキスト生成
//==========================================================================
HRESULT CTitle_SUSURU::CreateText()
{
	// 文字
	float u = (1.0f / NUM_TEXT);
	for (int i = 0; i < NUM_TEXT; i++)
	{
		// 生成
		m_apText[i] = CObject2D::Create(4);
		if (m_apText[i] == nullptr) return E_FAIL;

		// 種類設定
		m_apText[i]->SetType(CObject::TYPE::TYPE_OBJECT2D);
		m_apText[i]->SetAnchorType(CObject2D::AnchorPoint::TOP_CENTER);

		// テクスチャ設定
		CTexture* pTexture = CTexture::GetInstance();
		int texID = CTexture::GetInstance()->Regist(TEXTURE);
		m_apText[i]->BindTexture(texID);

		// サイズ設定
		m_apText[i]->SetSize(MyLib::Vector2(TEXTSIZE.x, 0.0f));
		m_apText[i]->SetSizeOrigin(TEXTSIZE);

		// 位置設定
		m_apText[i]->SetPosition(MyLib::Vector3(-TEXTSIZE.x * 0.5f, 0.0f, 0.0f) + MyLib::Vector3(SCREEN_WIDTH - TEXTSIZE.x * 1.5f * i, 0.0f, 0.0f));

		// UV
		D3DXVECTOR2* pTex = m_apText[i]->GetTex();
		pTex[0].x = pTex[2].x = 0.9f - u * i;
		pTex[1].x = pTex[3].x = 0.9f - (u * i) + u;
		int nnn = 0;
	}
	return S_OK;
}

//==========================================================================
// SUSURUさん生成
//==========================================================================
HRESULT CTitle_SUSURU::CreateSUSURU()
{

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle_SUSURU::Uninit()
{
	delete this;
}

//==========================================================================
// 削除処理
//==========================================================================
void CTitle_SUSURU::Kill()
{
	// テキスト
	for (int i = 0; i < CTitle_SUSURU::NUM_TEXT; i++)
	{
		SAFE_KILL(m_apText[i]);
	}

	// SUSURU
	SAFE_KILL(m_pSUSURU);

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle_SUSURU::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() == CInstantFade::EState::STATE_FADEIN)
	{// フェードINは抜ける
		return;
	}

	// 親の更新
	CTitleScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_A).bInput ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// シーン切り替え
		CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_CONTROLLWAIT);
	}

#if _DEBUG

	for(int i = 0; i < CTitle_SUSURU::NUM_TEXT; i++)
	{
		std::string str = "SCROLL[" + std::to_string(i) + "]";
		ImGui::DragFloat(str.c_str(), &StateTime::SCROLL[i], 0.01f);
	}

	if (ImGui::Button("Reset"))
	{
		// 送る文字インデックス
		m_nIdxScroll = -1;

		// 状態
		m_state = EState::STATE_WAIT;	// 状態
		m_fStateTime = 0.0f;			// 状態時間

		// BGM再生
		CSound::GetInstance()->StopSound(CSound::ELabel::LABEL_SE_AI);

		for (const auto& text : m_apText)
		{
			// サイズ設定
			MyLib::Vector2 size = text->GetSize();
			size.y = 0.0f;
			text->SetSize(size);
		}
	}

#endif // _DEBUG

}

//==========================================================================
// 状態更新
//==========================================================================
void CTitle_SUSURU::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 待機
//==========================================================================
void CTitle_SUSURU::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		m_fStateTime -= StateTime::WAIT;
		m_state = EState::STATE_SCROLL;

		if (m_nIdxScroll < 0)
		{// AI読み上げ開始
			PLAY_SOUND(CSound::ELabel::LABEL_SE_AI);
		}

		// 送る文字更新
		m_nIdxScroll++;

		if (m_nIdxScroll >= CTitle_SUSURU::NUM_TEXT)
		{// 全て更新

			// 終了状態
			m_state = EState::STATE_END;	// 状態
			m_fStateTime = 0.0f;			// 状態時間
		}
	}
}

//==========================================================================
// 文字送り
//==========================================================================
void CTitle_SUSURU::StateScroll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 文字送り
	MyLib::Vector2 size = m_apText[m_nIdxScroll]->GetSize();

	// サイズ設定
	size.y = UtilFunc::Correction::EasingLinear(0.0f, TEXTSIZE.y, 0.0f, StateTime::SCROLL[m_nIdxScroll], m_fStateTime);
	m_apText[m_nIdxScroll]->SetSize(size);


	// UVをサイズに合わせる
	D3DXVECTOR2* pTex = m_apText[m_nIdxScroll]->GetTex();
	pTex[2].y = pTex[3].y = m_apText[m_nIdxScroll]->GetSize().y / m_apText[m_nIdxScroll]->GetSizeOrigin().y;

	if (m_fStateTime >= StateTime::SCROLL[m_nIdxScroll])
	{
		m_fStateTime -= StateTime::SCROLL[m_nIdxScroll];
		m_state = EState::STATE_WAIT;
	}
}

//==========================================================================
// 終了
//==========================================================================
void CTitle_SUSURU::StateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= StateTime::END)
	{
		CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_CONTROLLWAIT);
	}
}

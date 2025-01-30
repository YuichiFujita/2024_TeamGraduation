//==========================================================================
// 
//  チュートリアル画面処理 [tutorialscreen.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
//==========================================================================
// インクルードファイル
//==========================================================================
#include "tutorialscreen.h"
#include "manager.h"
#include "fade.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	namespace Fade
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\bg_scroll.png";	// テクスチャ
		const float SCROLL_X = 0.00038f;
		const float SCROLL_Y = SCROLL_X * UtilFunc::Calculation::AspectRatio(D3DXVECTOR2(640.0f, 360.0f));
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);	// 初期化色
	}

	namespace BG
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\bg.png";	// テクスチャ
	}

	namespace Manual
	{
		const std::string TEXTURE = "data\\TEXTURE\\tutorial\\manual.png";	// テクスチャ
	}

	namespace ManualText
	{
		const std::string TEXTURE[CTutorialScreen::EManualType::TYPE_MAX] =	// テクスチャ
		{
			"data\\TEXTURE\\tutorial\\move.png",		// 移動
			"data\\TEXTURE\\tutorial\\blink.png",		// ブリンク
			"data\\TEXTURE\\tutorial\\jump.png",		// ジャンプ
			"data\\TEXTURE\\tutorial\\throw.png",		// 投げ
			"data\\TEXTURE\\tutorial\\pass.png",		// パス
			"data\\TEXTURE\\tutorial\\catch.png",		// キャッチ
			"data\\TEXTURE\\tutorial\\SP.png",			// スペシャル
			"data\\TEXTURE\\tutorial\\justcatch.png",	// ジャストキャッチ
		};
		const float START_X = -640.0f;	// スタート位置(X)
	}

	namespace StateTime
	{
		const float SPAWN = 0.3f;	// 登場
		const float FADEOUT = 0.3f;	// フェードアウト
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTutorialScreen::STATE_FUNC CTutorialScreen::m_StateFunc[] =
{
	&CTutorialScreen::StateNone,	// なにもなし
	&CTutorialScreen::StateSpawn,	// 登場
	&CTutorialScreen::StateFadeIn,	// フェードイン
	&CTutorialScreen::StateFadeOut,	// フェードアウト
};

//==========================================================================
//	コンストラクタ
//==========================================================================
CTutorialScreen::CTutorialScreen(int nPriority) : CObject2D(nPriority),
	m_state			(EState::STATE_NONE),		// 状態
	m_fStateTime	(0.0f),						// 状態タイマー
	m_ManualType	(EManualType::TYPE_MOVE),	// 説明の種類
	m_scrollDir		(EScrollDir::SCROLL_R),		// スクロール方向
	m_pFade			(nullptr),					// フェード
	m_pManual		(nullptr)					// 説明
{

}

//==========================================================================
//	デストラクタ
//==========================================================================
CTutorialScreen::~CTutorialScreen()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTutorialScreen* CTutorialScreen::Create()
{
	// メモリの確保
	CTutorialScreen* pObj = DEBUG_NEW CTutorialScreen();

	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
//	初期化処理
//==========================================================================
HRESULT CTutorialScreen::Init()
{
	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 背景生成
	if (FAILED(CreateBG()))
	{
		return E_FAIL;
	}

	// フェード生成
	if (FAILED(CreateFade()))
	{
		return E_FAIL;
	}

	// マニュアル生成
	if (FAILED(CreateManual()))
	{
		return E_FAIL;
	}

	// マニュアルテキスト生成
	if (FAILED(CreateManualText()))
	{
		return E_FAIL;
	}

	// 状態設定
	SetState(EState::STATE_FADEIN);
	StateFadeIn(0.0f, 0.0f, 1.0f);

	return S_OK;
}

//==========================================================================
// 背景生成
//==========================================================================
HRESULT CTutorialScreen::CreateBG()
{
	// 初期化処理
	CObject2D::Init();

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(BG::TEXTURE);
	BindTexture(nTexID);

	// サイズ設定
	SetSize(MyLib::Vector2(640.0f, 360.0f));

	// 位置設定
	SetPosition(VEC3_SCREEN_CENT);

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// フェード生成
//==========================================================================
HRESULT CTutorialScreen::CreateFade()
{
	// 生成処理
	m_pFade = CObject2D::Create(GetPriority() + 1);
	if (m_pFade == nullptr) { return E_FAIL; }

	// 位置を設定
	m_pFade->SetPosition(VEC3_SCREEN_CENT);

	// 大きさを設定
	m_pFade->SetSize(MyLib::Vector2(640.0f, 360.0f));

	// 色を設定
	m_pFade->SetColor(Fade::INIT_COL);

	// テクスチャを登録・割当
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(Fade::TEXTURE);
	m_pFade->BindTexture(texID);

	return S_OK;
}

//==========================================================================
// 説明生成
//==========================================================================
HRESULT CTutorialScreen::CreateManual()
{
	// 生成処理
	m_pManual = CObject2D::Create(GetPriority() + 1);
	if (m_pManual == nullptr) { return E_FAIL; }

	// 位置を設定
	m_pManual->SetPosition(VEC3_SCREEN_CENT);

	// 大きさを設定
	m_pManual->SetSize(MyLib::Vector2(640.0f, 360.0f));

	// テクスチャを登録・割当
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(Manual::TEXTURE);
	m_pManual->BindTexture(texID);

	return S_OK;
}

//==========================================================================
// 説明テキスト生成
//==========================================================================
HRESULT CTutorialScreen::CreateManualText()
{
	// 生成処理
	m_pManualText = CObject2D::Create(GetPriority() + 1);
	if (m_pManualText == nullptr) { return E_FAIL; }

	// 位置を設定
	m_pManualText->SetPosition(VEC3_SCREEN_CENT);

	// 大きさを設定
	m_pManualText->SetSize(MyLib::Vector2(640.0f, 360.0f));

	// テクスチャを登録・割当
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(ManualText::TEXTURE[m_ManualType]);
	m_pManualText->BindTexture(texID);

	return S_OK;
}

//==========================================================================
//	終了処理
//==========================================================================
void CTutorialScreen::Uninit()
{
	// オブジェクトを破棄
	Release();
}

//==========================================================================
//	削除処理
//==========================================================================
void CTutorialScreen::Kill()
{
	// フェード
	SAFE_KILL(m_pFade);

	// マニュアル
	SAFE_KILL(m_pManual);

	// 自身の終了
	CTutorialScreen::Uninit();
}

//==========================================================================
//	更新処理
//==========================================================================
void CTutorialScreen::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 更新処理
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 背景更新
	UpdateBG(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// 説明の変更処理
	ChangeManual();
}

//==========================================================================
// 状態更新
//==========================================================================
void CTutorialScreen::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 更新処理
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 何もなし
//==========================================================================
void CTutorialScreen::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// 登場
//==========================================================================
void CTutorialScreen::StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 位置更新
	MyLib::Vector3 pos = m_pManualText->GetPosition();
	float posDestX = SCREEN_WIDTH;	// 目標のX

	if (m_fStateTime <= StateTime::SPAWN * 0.5f)
	{
		posDestX = (m_scrollDir == EScrollDir::SCROLL_R) ? (-640.0f - SCREEN_WIDTH) : (SCREEN_WIDTH + 640.0f);	// 目標のX
		pos.x = UtilFunc::Correction::EasingEaseIn(m_pManualText->GetOriginPosition().x, posDestX, 0.0f, StateTime::SPAWN * 0.5f, m_fStateTime);
	}
	else
	{
		float startX = (m_scrollDir == EScrollDir::SCROLL_R) ? (SCREEN_WIDTH + 640.0f) : ManualText::START_X;	// 開始のX
		pos.x = UtilFunc::Correction::EaseOutBack(startX, 640.0f, StateTime::SPAWN * 0.5f, StateTime::SPAWN, m_fStateTime);

		// テクスチャを登録・割当
		CTexture* pTexture = CTexture::GetInstance();
		int texID = pTexture->Regist(ManualText::TEXTURE[m_ManualType]);
		m_pManualText->BindTexture(texID);
	}
	m_pManualText->SetPosition(pos);

	if (m_fStateTime >= StateTime::SPAWN)
	{
		// NONEに設定
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// フェードイン
//==========================================================================
void CTutorialScreen::StateFadeIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 不透明度割り出し
	float alpha = UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);

	// 不透明度設定
	SetAlpha(alpha);
	m_pFade->SetAlpha(alpha);		// フェードの情報
	m_pManual->SetAlpha(alpha);		// 説明の情報
	m_pManualText->SetAlpha(alpha);	// 説明テキストの情報

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CTutorialScreen::StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 不透明度割り出し
	float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);

	// 不透明度設定
	SetAlpha(alpha);
	m_pFade->SetAlpha(alpha);		// フェードの情報
	m_pManual->SetAlpha(alpha);		// 説明の情報
	m_pManualText->SetAlpha(alpha);	// 説明テキストの情報

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// 削除
		Kill();
	}
}

//==========================================================================
// 背景更新
//==========================================================================
void CTutorialScreen::UpdateBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// フェードスクロール
	D3DXVECTOR2* pTex = m_pFade->GetTex();
	for (int i = 0; i < 4; i++)
	{
		pTex[i].x += Fade::SCROLL_X;
		pTex[i].y -= Fade::SCROLL_Y;
	}
}

//==========================================================================
// 説明の変更処理
//==========================================================================
void CTutorialScreen::ChangeManual()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	bool bChange = false;	// 変更フラグ
	EManualType oldType = m_ManualType;	// 前回の種類

	if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT).bInput)
	{// 次ページへ

		EManualType old = m_ManualType;
		m_ManualType = (EManualType)UtilFunc::Transformation::Clamp(m_ManualType + 1, 0, EManualType::TYPE_MAX - 1);
		bChange = true;

		// 右スクロール
		m_scrollDir = EScrollDir::SCROLL_R;

		if (old != m_ManualType)
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_NEXT);
		}
		else
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
		}
	}
	else if (pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_LEFT).bInput)
	{// 前ページへ

		EManualType old = m_ManualType;
		m_ManualType = (EManualType)UtilFunc::Transformation::Clamp(m_ManualType - 1, 0, EManualType::TYPE_MAX - 1);
		bChange = true;

		// 左スクロール
		m_scrollDir = EScrollDir::SCROLL_L;

		if (old != m_ManualType)
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BACK);
		}
		else
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
		}
	}

	// 変更されていたら状態設定
	if (bChange &&
		m_ManualType != oldType)
	{
		// 登場
		SetState(EState::STATE_SPAWN);

		// 元の位置
		m_pManualText->SetOriginPosition(m_pManualText->GetPosition());
	}

	if (m_state != EState::STATE_FADEIN &&
		pPad->GetAllTrigger(CInputGamepad::BUTTON::BUTTON_A).bInput)
	{// ウィンドウ閉じる

		if (!GET_MANAGER->GetFade()->IsFade())
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
		}

		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CTutorialScreen::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 
//  チュートリアル確認画面処理 [tutorialcheck.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
//==========================================================================
// インクルードファイル
//==========================================================================
#include "tutorialcheck.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "title_controllwait.h"
#include "tutorialscreen.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	namespace BG
	{
		const std::string TEXTURE = "data\\TEXTURE\\tutorial\\window.png";	// テクスチャ
	}

	namespace Select
	{
		const std::string TEXTURE[CTutorialCheck::ESelect::SELECT_MAX] =	// テクスチャ
		{
			"data\\TEXTURE\\tutorial\\select_yes.png",	// チュートリアル見る
			"data\\TEXTURE\\tutorial\\select_no.png",	// チュートリアル見ない
		};

		const MyLib::Vector3 POSITION_SELECT[CTutorialCheck::ESelect::SELECT_MAX] =	// 選択肢の位置
		{
			MyLib::Vector3(640.0f - 200.0f, 550.0f, 0.0f),	// 試合開始
			MyLib::Vector3(640.0f + 200.0f, 550.0f, 0.0f),	// 色々
		};

		const float SIZEHEIGHT = 90.0f;	// 縦幅
	}

	namespace StateTime
	{
		const float FADE = 0.3f;	// フェード
		const float SPAWN = 0.3f;	// 登場
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTutorialCheck::STATE_FUNC CTutorialCheck::m_StateFunc[] =
{
	&CTutorialCheck::StateNone,		// なにもなし
	&CTutorialCheck::StateFadeIn,	// フェードイン
	&CTutorialCheck::StateFadeOut,	// フェードアウト
};

//==========================================================================
//	コンストラクタ
//==========================================================================
CTutorialCheck::CTutorialCheck(int controllIdx, int nPriority) : CObject2D(nPriority),
	m_nControllIdx		(controllIdx),			// 操作インデックス
	m_pControllWait		(nullptr),				// 操作待機情報
	m_state			(EState::STATE_NONE),	// 状態
	m_fStateTime	(0.0f),					// 状態タイマー
	m_select		(ESelect::SELECT_YES)	// 選択肢
{
}

//==========================================================================
//	デストラクタ
//==========================================================================
CTutorialCheck::~CTutorialCheck()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTutorialCheck* CTutorialCheck::Create(int controllIdx, CTitle_ControllWait* pControllWait)
{
	// メモリの確保
	CTutorialCheck* pObj = DEBUG_NEW CTutorialCheck(controllIdx);

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->m_pControllWait = pControllWait;

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
HRESULT CTutorialCheck::Init()
{
	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 状態設定
	SetState(EState::STATE_FADEIN);

	// 背景生成
	if (FAILED(CreateBG()))
	{
		return E_FAIL;
	}

	// 選択肢生成
	if (FAILED(CreateSelect()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 背景生成
//==========================================================================
HRESULT CTutorialCheck::CreateBG()
{
	// 初期化処理
	CObject2D::Init();

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(BG::TEXTURE);
	BindTexture(texID);

	// 画像サイズ取得
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ計算
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 300.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// 位置設定
	SetPosition(VEC3_SCREEN_CENT);

	// 色を設定
	SetColor(MyLib::color::White(0.0f));

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 選択肢生成
//==========================================================================
HRESULT CTutorialCheck::CreateSelect()
{
	// テクスチャ取得
	CTexture* pTexture = CTexture::GetInstance();

	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// 生成処理
		m_pSelect[i] = CObject2D::Create(GetPriority() + 1);
		if (m_pSelect[i] == nullptr) { return E_FAIL; }

		// テクスチャを登録・割当
		int texID = pTexture->Regist(Select::TEXTURE[i]);
		m_pSelect[i]->BindTexture(texID);

		// 位置を設定
		m_pSelect[i]->SetPosition(Select::POSITION_SELECT[i]);

		// 画像サイズ取得
		MyLib::Vector2 size = pTexture->GetImageSize(texID);

		// 縦幅を元にサイズ計算
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Select::SIZEHEIGHT);
		m_pSelect[i]->SetSize(size);
		m_pSelect[i]->SetSizeOrigin(m_pSelect[i]->GetSize());

		// 色を設定
		m_pSelect[i]->SetColor(MyLib::color::White(0.0f));
	}

	return S_OK;
}

//==========================================================================
//	終了処理
//==========================================================================
void CTutorialCheck::Uninit()
{
	// オブジェクトを破棄
	Release();
}

//==========================================================================
//	削除処理
//==========================================================================
void CTutorialCheck::Kill()
{
	// フェード
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pSelect[i]);
	}

	// 自身の終了
	CTutorialCheck::Uninit();
}

//==========================================================================
//	更新処理
//==========================================================================
void CTutorialCheck::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 更新処理
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 色の更新
	UpdateColor();

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新
//==========================================================================
void CTutorialCheck::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 更新処理
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 何もなし
//==========================================================================
void CTutorialCheck::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, m_nControllIdx) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, m_nControllIdx) ||
		pPad->GetLStickTrigger(m_nControllIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT) ||
		pPad->GetLStickTrigger(m_nControllIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
	{// 変更

		// 現在の選択肢の逆にする
		m_select = (m_select == ESelect::SELECT_YES) ? ESelect::SELECT_NO : ESelect::SELECT_YES;

		// サウンドの再生
		int soundLabel = UtilFunc::Transformation::Random(CSound::ELabel::LABEL_SE_WRITE01, CSound::ELabel::LABEL_SE_WRITE02);
		PLAY_SOUND((CSound::ELabel)soundLabel);
	}

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, m_nControllIdx))
	{// 決定

		if (!GET_MANAGER->GetFade()->IsFade())
		{
			// サウンドの再生
			PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
		}

		Decide();
	}
}

//==========================================================================
// フェードイン
//==========================================================================
void CTutorialCheck::StateFadeIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, 0.0f, StateTime::FADE, m_fStateTime);
	
	// 設定
	SetAlpha(alpha);
	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// 時間経過
	if (m_fStateTime >= StateTime::FADE)
	{
		// NONEに設定
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CTutorialCheck::StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, 0.0f, StateTime::FADE, m_fStateTime);

	// 設定
	SetAlpha(alpha);
	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// 時間経過
	if (m_fStateTime >= StateTime::FADE)
	{
		// 操作待機の状態設定
		if (m_pControllWait->GetState() == CTitle_ControllWait::EState::STATE_TUTORIALCHECK)
		{
			m_pControllWait->SetState(CTitle_ControllWait::EState::STATE_WAIT);
		}

		// 削除
		Kill();
	}
}

//==========================================================================
// 色の更新
//==========================================================================
void CTutorialCheck::UpdateColor()
{
	// 選択してるやつを目立たせる
	for (const auto& select : m_pSelect)
	{
		select->SetColor(MyLib::color::White(select->GetAlpha()));
		select->SetSize(select->GetSizeOrigin());
	}

	// 選択してるやつを目立たせる
	m_pSelect[m_select]->SetColor(MyLib::color::Red(m_pSelect[m_select]->GetAlpha()));
	m_pSelect[m_select]->SetSize(m_pSelect[m_select]->GetSizeOrigin() * 1.2f);
}

//==========================================================================
// 決定
//==========================================================================
void CTutorialCheck::Decide()
{
	switch (m_select)
	{
	case CTutorialCheck::SELECT_YES:	// チュートリアル見る

		// チュートリアル中状態設定
		m_pControllWait->SetState(CTitle_ControllWait::EState::STATE_TUTORIAL);
		break;

	case CTutorialCheck::SELECT_NO:	// チュートリアルみないで開始

		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
		break;

	default:
		break;
	}
}

//==========================================================================
// キャンセル
//==========================================================================
void CTutorialCheck::Cancel()
{
	// フェードアウト
	SetState(EState::STATE_FADEOUT);
}

//==========================================================================
// 状態設定
//==========================================================================
void CTutorialCheck::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//=============================================================================
// 
//  クリアランク処理(一番上) [clearrank.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "clearrank.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE[] =	// テクスチャのファイル
	{
		"data\\TEXTURE\\result\\rank_S.png",
		"data\\TEXTURE\\result\\rank_A.png",
		"data\\TEXTURE\\result\\rank_B.png",
		"data\\TEXTURE\\result\\rank_C.png",
		"data\\TEXTURE\\result\\rank_C.png",
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\clearrank.png";
	const float SIZE_HEIGHT = 50.0f;	// 縦幅のサイズ
	const float MOVEVALUE_TEXT = 3.0f;	// テキストの移動量
	const float MOVEVALUE_RANK = 6.0f;	// ランクの移動量
}

namespace StateTime
{
	const float WAIT = 0.5f;		// 待機
	const float EMPHASIZE = 1.0f;	// 強調
	const int SCROLLEND = 1.0f;	// スクロール終了後
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CClearRank::STATE_FUNC CClearRank::m_StateFunc[] =
{
	&CClearRank::StateScrollText,	// 文字送り
	&CClearRank::StateSrollVoid,	// 空間送り
	&CClearRank::StateScrollRank,	// ランク送り
	&CClearRank::StateFinish,		// 終了
	&CClearRank::StateEmphasize,	// 強調
	&CClearRank::StateNone,			// なにもなし

};

//==========================================================================
// コンストラクタ
//==========================================================================
CClearRank::CClearRank(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// ランク
	m_pText = nullptr;		// 文字


	m_fStateTime = 0.0f;		// 状態カウンター
	m_state = State::STATE_SCROLL_TEXT;			// 状態
	m_fMoveTextLen = 0.0f;	// テキストの移動距離
	m_fMoveRankLen = 0.0f;	// ランクの移動距離
	m_bFinish = false;		// 終了
}

//==========================================================================
// デストラクタ
//==========================================================================
CClearRank::~CClearRank()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CClearRank* CClearRank::Create(CJudge::JUDGE rank)
{
	// メモリの確保
	CClearRank* pClearRank = DEBUG_NEW CClearRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = rank;

		// 初期化処理
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CClearRank::Init()
{
	//=============================
	// 文字生成
	//=============================
	CreateText();

	//=============================
	// ランク生成
	//=============================
	CreateRank();


	// 状態遷移
	SetState(State::STATE_NONE);

	return S_OK;
}

//==========================================================================
// 文字生成
//==========================================================================
void CClearRank::CreateText()
{
	// 生成
	m_pText = CObject2D::Create(GetPriority());
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(D3DXVECTOR2(0.0f, size.y));
	m_pText->SetSizeOrigin(size);

	// 位置設定
	m_pText->SetPosition(MyLib::Vector3(200.0f, 200.0f, 0.0f));

	// アンカーポイントの設定
	m_pText->SetAnchorType(AnchorPoint::LEFT);
}

//==========================================================================
// ランク生成
//==========================================================================
void CClearRank::CreateRank()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);

	SetSize(D3DXVECTOR2(0.0f, size.y));
	SetSizeOrigin(size);
	SetPosition(m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f, 0.0f, 0.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_ENEMY);

	// アンカーポイントの設定
	SetAnchorType(AnchorPoint::LEFT);
}


//==========================================================================
// 終了処理
//==========================================================================
void CClearRank::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CClearRank::Update()
{
	// 状態更新
	UpdateState();

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CClearRank::SetVtx()
{
	CObject2D::SetVtx();
}

//==========================================================================
// 状態更新
//==========================================================================
void CClearRank::UpdateState()
{
	// 状態タイマー
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// スキップ
//==========================================================================
void CClearRank::Skip()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// モード設定
		SetState(CClearRank::State::STATE_FINISH);
	}
}

//==========================================================================
// 文字送り
//==========================================================================
void CClearRank::StateScrollText()
{
	// サイズ取得
	D3DXVECTOR2 size = m_pText->GetSize(), sizeOrigin = m_pText->GetSizeOrigin();

	// テキスト移動距離加算
	m_fMoveTextLen += MOVEVALUE_TEXT;
	m_fMoveTextLen = UtilFunc::Transformation::Clamp(m_fMoveTextLen, 0.0f, sizeOrigin.x);

	if (m_fMoveTextLen >= sizeOrigin.x)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_VOID);
	}

	// サイズ設定
	size.x = m_fMoveTextLen;
	m_pText->SetSize(size);

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = m_pText->GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);

	// スキップ
	Skip();
}

//==========================================================================
// 空間送り
//==========================================================================
void CClearRank::StateSrollVoid()
{

	if (m_fStateTime >= StateTime::WAIT)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_RANK);

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	// スキップ
	Skip();
}

//==========================================================================
// ランク送り
//==========================================================================
void CClearRank::StateScrollRank()
{
	// サイズ取得
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();

	// テキスト移動距離加算
	m_fMoveRankLen += MOVEVALUE_RANK;

	if (m_fMoveRankLen >= sizeOrigin.x/* + MOVEVALUE_RANK * (StateTime::SCROLLEND * 60.0f)*/)
	{
		// 状態遷移
		SetState(State::STATE_FINISH);
	}

	// サイズ設定
	size.x = UtilFunc::Transformation::Clamp(m_fMoveRankLen, 0.0f, sizeOrigin.x);
	SetSize(size);

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);

	// スキップ
	Skip();
}

//==========================================================================
// 終了
//==========================================================================
void CClearRank::StateFinish()
{
	// 終了フラグ
	m_bFinish = true;

	// サイズ設定
	SetSize(GetSizeOrigin());
	m_pText->SetSize(m_pText->GetSizeOrigin());

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = GetTex();
	D3DXVECTOR2* pTexText = m_pText->GetTex();
	pTex[1].x = pTex[3].x = pTexText[1].x = pTexText[3].x = 1.0f;

	// 状態遷移
	SetState(State::STATE_EMPHASIZE);
}

//==========================================================================
// 強調
//==========================================================================
void CClearRank::StateEmphasize()
{
	// サイズ取得
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	float ratio = m_fStateTime / StateTime::EMPHASIZE;
	size.x = sizeOrigin.x + sinf(D3DX_PI * ratio) * (sizeOrigin.x * 0.5f);
	size.y = sizeOrigin.y + sinf(D3DX_PI * ratio) * (sizeOrigin.y * 0.5f);
	SetSize(size);

#if 0	// イージング補正
	if (m_fStateTime <= StateTime::EMPHASIZE * 0.5f)
	{
		size.x = UtilFunc::Correction::EaseOutExpo(sizeOrigin.x, sizeOrigin.x * 1.5f, 0.0f, StateTime::EMPHASIZE * 0.5f, m_fStateTime);
		size.y = UtilFunc::Correction::EaseOutExpo(sizeOrigin.y, sizeOrigin.y * 1.5f, 0.0f, StateTime::EMPHASIZE * 0.5f, m_fStateTime);
	}
	else
	{
		size.x = UtilFunc::Correction::EaseInExpo(sizeOrigin.x * 1.5f, sizeOrigin.x, StateTime::EMPHASIZE * 0.5f, StateTime::EMPHASIZE, m_fStateTime);
		size.y = UtilFunc::Correction::EaseInExpo(sizeOrigin.y * 1.5f, sizeOrigin.y, StateTime::EMPHASIZE * 0.5f, StateTime::EMPHASIZE, m_fStateTime);
	}
	SetSize(size);
#endif

	if (m_fStateTime >= StateTime::EMPHASIZE)
	{
		// 状態遷移
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CClearRank::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画処理
//==========================================================================
void CClearRank::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

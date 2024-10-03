//=============================================================================
// 
//  トータルランク処理 [toatalrank.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "toatalrank.h"
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
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\toatalrank.png";

	const int RANKPOINT[] =	// ランクの基準
	{
		4,
		3,
		2,
		1
	};

	const float TIMEPOINT[] =	// タイマーの基準
	{
		80.0f,
		100.0f,
		120.0f,
		140.0f
	};

	const float SIZE_HEIGHT = 50.0f;	// 縦幅のサイズ
	const float MOVEVALUE_TEXT = 3.0f;	//テキストの移動量
}

namespace StateTime
{
	const float WAIT = 0.5f;	// 待機
	const float EMPHASIZE = 1.0f;	// 強調
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CToatalRank::STATE_FUNC CToatalRank::m_StateFunc[] =
{
	&CToatalRank::StateScrollText,	// 文字送り
	&CToatalRank::StateSrollVoid,	// 空間送り
	&CToatalRank::StateScrollRank,	// ランク送り
	&CToatalRank::StateFinish,		// 終了
	&CToatalRank::StateNone,		// なにもなし

};

//==========================================================================
// コンストラクタ
//==========================================================================
CToatalRank::CToatalRank(int nPriority) : CObject2D(nPriority)
{
	// 値のトータル
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// ランク

	m_fStateTime = 0.0f;		// 状態カウンター
	m_state = State::STATE_SCROLL_TEXT;			// 状態
	m_fMoveTextLen = 0.0f;	// テキストの移動距離
	m_fMoveRankLen = 0.0f;	// ランクの移動距離
	m_bFinish = false;		// 終了
	m_bSoundFinish = false;	// サウンド終了
}

//==========================================================================
// デストラクタ
//==========================================================================
CToatalRank::~CToatalRank()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CToatalRank* CToatalRank::Create(CJudge::JUDGE rank, float time)
{
	// メモリの確保
	CToatalRank* pClearRank = DEBUG_NEW CToatalRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = pClearRank->CalRank(rank, time);

		// 初期化処理
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CToatalRank::Init()
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
void CToatalRank::CreateText()
{
	// 生成
	m_pText = CObject2D::Create(GetPriority());
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(D3DXVECTOR2(0.0f, size.y));
	m_pText->SetSizeOrigin(size);

	// 位置設定
	m_pText->SetPosition(MyLib::Vector3(700.0f, 400.0f, 0.0f));

	// アンカーポイントの設定
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// ランク生成
//==========================================================================
void CToatalRank::CreateRank()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);
	SetSize(D3DXVECTOR2(0.0f, size.y));
	SetSizeOrigin(size);

	// 位置設定
	SetPosition(m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSize().x + 120.0f, 100.0f, 0.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// アンカーポイント設定
	SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// 終了処理
//==========================================================================
void CToatalRank::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CToatalRank::Update()
{
	// 状態更新
	UpdateState();

	// 更新処理
	CObject2D::Update();
}


//==========================================================================
// 状態更新
//==========================================================================
void CToatalRank::UpdateState()
{
	// 状態タイマー
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// スキップ
//==========================================================================
void CToatalRank::Skip()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// テキスト移動距離加算
		m_pText->SetSize(m_pText->GetSizeOrigin());
		D3DXVECTOR2* pTex = m_pText->GetTex();
		pTex[1].x = pTex[3].x = 1.0f;

		SetState(CToatalRank::State::STATE_SCROLL_RANK);
	}
}

//==========================================================================
// 文字送り
//==========================================================================
void CToatalRank::StateScrollText()
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
void CToatalRank::StateSrollVoid()
{

	if (m_fStateTime >= StateTime::WAIT)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_RANK);
	}

	// スキップ
	Skip();
}

//==========================================================================
// ランク送り
//==========================================================================
void CToatalRank::StateScrollRank()
{
	// サイズ取得
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();

	// テキスト移動距離加算
	/*m_fMoveRankLen += MOVEVALUE_TEXT;
	m_fMoveRankLen = UtilFunc::Transformation::Clamp(m_fMoveRankLen, 0.0f, sizeOrigin.x);*/
	m_fMoveRankLen = UtilFunc::Correction::EaseInExpo(0.0f, sizeOrigin.x, 0.0f, 0.9f, m_fStateTime);

	if (m_fStateTime >= 0.4f &&
		!m_bSoundFinish)
	{
		// サウンド再生終了
		m_bSoundFinish = true;

		// サウンド再生
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	if (m_fMoveRankLen >= sizeOrigin.x)
	{
		// 状態遷移
		SetState(State::STATE_FINISH);
	}

	// サイズ設定
	size.x = m_fMoveRankLen;
	SetSize(size);

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// 終了
//==========================================================================
void CToatalRank::StateFinish()
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
	SetState(State::STATE_NONE);
}

//==========================================================================
// 状態設定
//==========================================================================
void CToatalRank::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画処理
//==========================================================================
void CToatalRank::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// ランク計算
//==========================================================================
CJudge::JUDGE CToatalRank::CalRank(CJudge::JUDGE rank, float time)
{
	int point = 0, i = 0;

	// 時間のポイント計算
	while (1)
	{
		if (TIMEPOINT[i] >= time)
		{
			point += CJudge::JUDGE::JUDGE_DDD - (i);
			break;
		}
	}
	
	// ランクのポイント計算
	point += CJudge::JUDGE::JUDGE_DDD - rank;

	if (point >= CJudge::JUDGE::JUDGE_DDD * 2)
	{
		return CJudge::JUDGE::JUDGE_AAA;
	}
	else if (point >= CJudge::JUDGE::JUDGE_CCC * 2)
	{
		return CJudge::JUDGE::JUDGE_BBB;
	}
	else if (point >= CJudge::JUDGE::JUDGE_BBB * 2)
	{
		return CJudge::JUDGE::JUDGE_CCC;
	}
	else
	{
		return CJudge::JUDGE::JUDGE_DDD;
	}
}
//=============================================================================
// 
//  字幕処理 [subtitle.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "subtitle.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "player.h"
#include "baggage.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

// 状態別タイマー
namespace StateTime
{
	const float FADEIN = 0.15f;		// フェードイン
	const float FADEOUT = 0.15f;		// フェードアウト
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CSubTitle::STATE_FUNC CSubTitle::m_StateFuncList[] =
{
	&CSubTitle::StateNone,		// なし
	&CSubTitle::StateFadeIn,	// フェードイン
	&CSubTitle::StateFadeout,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CSubTitle::CSubTitle(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = State::STATE_NONE;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
	m_fLifeTimer = 0.0f;	// 生存時間
}

//==========================================================================
// デストラクタ
//==========================================================================
CSubTitle::~CSubTitle()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSubTitle* CSubTitle::Create(const MyLib::Vector3& pos, float life)
{
	// メモリの確保
	CSubTitle* pObj = DEBUG_NEW CSubTitle;

	if (pObj != nullptr)
	{
		pObj->m_fLifeTimer = life;
		pObj->SetPosition(pos);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSubTitle::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// 各種変数初期化
	m_state = State::STATE_FADEIN;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);



	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSubTitle::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSubTitle::Update()
{
	// 生存時間減算
	m_fLifeTimer -= CManager::GetInstance()->GetDeltaTime();

	if (m_fLifeTimer <= 0.0f &&
		m_state != State::STATE_FADEOUT)
	{
		m_state = State::STATE_FADEOUT;
		m_fStateTime = 0.0f;
	}

	// 状態更新
	UpdateState();

	if (CManager::GetInstance()->GetInstantFade()->GetState() != CInstantFade::STATE::STATE_NONE)
	{
		Uninit();
	}

	if (IsDeath())
	{
		return;
	}

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CSubTitle::UpdateState()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 実行
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CSubTitle::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードイン
//==========================================================================
void CSubTitle::StateFadeIn()
{
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		SetAlpha(1.0f);

		// 状態タイマーリセット
		m_fStateTime = 0.0f;
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CSubTitle::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// 状態タイマーリセット
		m_fStateTime = 0.0f;

		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CSubTitle::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CSubTitle::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 字幕割り当て
//==========================================================================
void CSubTitle::BindSubtitle(const std::string& texfile)
{
	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(texfile);
	BindTexture(texID);
}

//==========================================================================
// 高さを基準にサイズ設定
//==========================================================================
void CSubTitle::SetSizeByHeight(const float height)
{

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(GetIdxTexture());
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, height);

	// サイズ設定
	SetSize(size);
	SetSizeOrigin(size);
}

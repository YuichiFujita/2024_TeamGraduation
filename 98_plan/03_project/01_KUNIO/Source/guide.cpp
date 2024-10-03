//=============================================================================
// 
//  字幕処理 [guide.cpp]
//  Author : 日野澤匠泉
// 
//=============================================================================
#include "guide.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"
#include "player.h"
#include "baggage.h"
#include "controlkeydisp.h"
#include "keyconfig.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE[] =
	{
		"data\\TEXTURE\\guide\\guide_000.png",	// 開始
		"data\\TEXTURE\\battlewin\\guide.png",	// ゴール
	};
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
CGuide::STATE_FUNC CGuide::m_StateFuncList[] =
{
	&CGuide::StateNone,		// なし
	&CGuide::StateFadeIn,	// フェードイン
	&CGuide::StateFadeout,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CGuide::CGuide(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = State::STATE_NONE;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
	m_fLifeTimer = 0.0f;	// 生存時間
	m_pKeyDisp = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CGuide::~CGuide()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGuide* CGuide::Create(Type type)
{
	// メモリの確保
	CGuide* pObj = DEBUG_NEW CGuide;

	if (pObj != nullptr)
	{
		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TEXTURE[type]);
		pObj->BindTexture(texID);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGuide::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// 各種変数初期化
	m_state = State::STATE_FADEIN;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(GetIdxTexture());
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 100);

	// サイズ設定
	SetSize(size);
	SetSizeOrigin(size);

	// 開始ボタンも生成
	if(m_pKeyDisp == nullptr)
	{
		CKeyConfig* pkeyconfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
		MyLib::Vector3 keypos = GetPosition();
		keypos.x -= (size.x * 0.5f + size.y * 0.75f);
		float range = size.y * 0.65f;
		m_pKeyDisp = CControlKeyDisp::Create(keypos, 0.0f, range, range, pkeyconfig->GetKey(INGAME::ACT_AIR));
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGuide::Uninit()
{
	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp = nullptr;
	}

	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 削除処理
//==========================================================================
void CGuide::Kill()
{
	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp->Uninit();
		m_pKeyDisp = nullptr;
	}

	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGuide::Update()
{
	// 状態更新
	UpdateState();

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
void CGuide::UpdateState()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 実行
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CGuide::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードイン
//==========================================================================
void CGuide::StateFadeIn()
{
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp->SetAlpha(alpha);
	}

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		SetAlpha(1.0f);

		if (m_pKeyDisp != nullptr)
		{
			m_pKeyDisp->SetAlpha(1.0f);
		}

		// 状態タイマーリセット
		m_fStateTime = 0.0f;
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CGuide::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_pKeyDisp != nullptr)
	{
		m_pKeyDisp->SetAlpha(alpha);
	}

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
void CGuide::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CGuide::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}
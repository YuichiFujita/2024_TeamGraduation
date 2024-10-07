//=============================================================================
// 
//  水しぶき背景処理 [splashwater_bg.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "splashwater_bg.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SPLASH = "data\\TEXTURE\\splash\\BG.png";
	const float TIME_FADEOUT_RATIO = 0.3f;
	const float DEFAULT_LIFE = 1.0f;
}

namespace StateTime
{
	const float APPEARANCE = 0.3f;
}


//==========================================================================
// コンストラクタ
//==========================================================================
CSplashwater_BG::CSplashwater_BG(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_fLife = 0.0f;			// 寿命
	m_fOriginLife = 0.0f;	// 寿命
	m_state = State::STATE_NONE;				// 状態
	m_fStateTime = 0.0f;	// 状態タイマー
}

//==========================================================================
// デストラクタ
//==========================================================================
CSplashwater_BG::~CSplashwater_BG()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSplashwater_BG* CSplashwater_BG::Create()
{
	// メモリの確保
	CSplashwater_BG* pObj = DEBUG_NEW CSplashwater_BG;

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
HRESULT CSplashwater_BG::Init()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// 位置
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// 色設定
	SetAlpha(0.0f);
	float rg = UtilFunc::Transformation::Random(7, 10) * 0.1f;
	SetOriginColor(D3DXCOLOR(rg, rg, 1.0f, 0.5f));

	// 寿命
	m_fLife = DEFAULT_LIFE + UtilFunc::Transformation::Random(-2, 1) * 0.1f;
	m_fOriginLife = m_fLife;

	m_state = State::STATE_APPEARANCE;	// 状態
	m_fStateTime = 0.0f;	// 状態タイマー

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSplashwater_BG::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSplashwater_BG::Update()
{
	// 終了処理
	CObject2D::Update();

	if (m_state == State::STATE_APPEARANCE)
	{
		StateAppearance();
		return;
	}

	m_fLife -= CManager::GetInstance()->GetDeltaTime();

	float lifeRatio = m_fLife / m_fOriginLife;
	if (lifeRatio <= TIME_FADEOUT_RATIO)
	{
		SetAlpha(GetOriginColor().a * (lifeRatio / TIME_FADEOUT_RATIO));
	}

	if (m_fLife <= 0.0f)
	{
		Uninit();
	}
}


//==========================================================================
// 出現状態
//==========================================================================
void CSplashwater_BG::StateAppearance()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	float alpha = UtilFunc::Correction::EasingLinear(0.0f, GetOriginColor().a, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		SetAlpha(GetOriginColor().a);
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CSplashwater_BG::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

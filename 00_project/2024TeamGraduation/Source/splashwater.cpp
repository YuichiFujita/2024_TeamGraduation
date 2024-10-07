//=============================================================================
// 
//  サンプル_オブジェクト2D処理 [sample_obj2D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "splashwater.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SPLASH[] =
	{
		"data\\TEXTURE\\splash\\drop_pattern_000.png",
		"data\\TEXTURE\\splash\\splash_01.png",

	};	// テクスチャのファイル
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
CSplashwater::CSplashwater(int nPriority) : CObject2D(nPriority)
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
CSplashwater::~CSplashwater()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSplashwater* CSplashwater::Create()
{
	// メモリの確保
	CSplashwater* pObj = DEBUG_NEW CSplashwater;

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
HRESULT CSplashwater::Init()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH[UtilFunc::Transformation::Random(0, 1)]);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 25.0f + UtilFunc::Transformation::Random(-200, 100) * 0.1f);
#endif

	// 目標サイズ
	m_sizeDest = size;

	size *= 0.5f;
	SetSize(size);
	SetSizeOrigin(size);

	// 位置設定
	m_posDest = MyLib::Vector3(
		UtilFunc::Transformation::Random(-64, 64) * 10.0f + 640.0f,
		UtilFunc::Transformation::Random(-4, 66) * 10.0f + UtilFunc::Transformation::Random(-8, 8) * 10.0f,
		//UtilFunc::Transformation::Random(-40, 100),
		0.0f);

	SetPosition(MyLib::Vector3(
		UtilFunc::Transformation::Random(-50, 50) + 640.0f,
		UtilFunc::Transformation::Random(-20, 20) + 600.0f,
		0.0f));
	SetOriginPosition(GetPosition());

	// 色設定
	float rg = UtilFunc::Transformation::Random(7, 10) * 0.1f;
	SetColor(D3DXCOLOR(rg, rg, 1.0f, UtilFunc::Transformation::Random(2, 9) * 0.1f));
	SetOriginColor(GetColor());

	// 寿命
	m_fLife = DEFAULT_LIFE + UtilFunc::Transformation::Random(-2, 10) * 0.1f;
	m_fOriginLife = m_fLife;

	m_state = State::STATE_APPEARANCE;	// 状態
	m_fStateTime = 0.0f;	// 状態タイマー

	// 移動量
	SetMove(CalSetmove());

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSplashwater::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSplashwater::Update()
{
	// 終了処理
	CObject2D::Update();

	if (m_state == State::STATE_APPEARANCE)
	{
		StateAppearance();
		return;
	}

	m_fLife -= CManager::GetInstance()->GetDeltaTime();

	MyLib::Vector3 move = GetMove();

	if (UtilFunc::Transformation::Random(0, 8) == 0)
	{
		move.y = CalSetmove().y;
	}

	if (UtilFunc::Transformation::Random(0, 4) == 0)
	{
		move.x = CalSetmove().x;
	}

	AddPosition(move);
	SetMove(move);

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
void CSplashwater::StateAppearance()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 位置更新
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseInOut(GetOriginPosition(), m_posDest, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetPosition(pos);

	// サイズ更新
	D3DXVECTOR2 size;
	size.x = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().x, m_sizeDest.x, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().y, m_sizeDest.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetSize(size);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		SetPosition(m_posDest);
		SetSize(m_sizeDest);
		m_state = State::STATE_NONE;
	}
}

//==========================================================================
// 移動量計算
//==========================================================================
MyLib::Vector3 CSplashwater::CalSetmove()
{
	MyLib::Vector3 move;
	move.x = UtilFunc::Transformation::Random(-8, 8) * 0.1f;
	//move.x = UtilFunc::Transformation::Random(-40, 40) * 0.1f;
	move.y = UtilFunc::Transformation::Random(0, 400) * 0.01f;
	//move.y = UtilFunc::Transformation::Random(40, 1500) * 0.01f;
	return move;
}

//==========================================================================
// 描画処理
//==========================================================================
void CSplashwater::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	// 描画処理
	CObject2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

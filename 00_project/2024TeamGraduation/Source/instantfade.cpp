//=============================================================================
// 
//  遷移なしフェード処理 [instantfade.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "instantfade.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define ALPHAMOVE	(0.025f)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CInstantFade::CInstantFade()
{
	// 値のクリア
	m_fDuration = 0.0f;		// フェードまでの時間
	m_fTimerMove = 0.0f;	// フェード時間
	m_aObject2D = nullptr;	// オブジェクト2Dのオブジェクト
	m_state = STATE_NONE;	// 状態
}

//==========================================================================
// デストラクタ
//==========================================================================
CInstantFade::~CInstantFade()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CInstantFade *CInstantFade::Create()
{
	// 生成用のオブジェクト
	CInstantFade *pFade = nullptr;

	if (pFade == nullptr)
	{// nullptrだったら

		// メモリの確保
		pFade = DEBUG_NEW CInstantFade;

		if (pFade != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			if (FAILED(pFade->Init()))
			{// 失敗していたら
				return nullptr;
			}
		}
		else
		{
			delete pFade;
			pFade = nullptr;
		}

		return pFade;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CInstantFade::Init()
{
	// 生成処理
	m_aObject2D = CObject2D::Create(8);
	m_aObject2D->SetType(CObject::TYPE::TYPE_NONE);

	if (m_aObject2D == nullptr)
	{// 失敗していたら
		return E_FAIL;
	}

	m_state = STATE_NONE;			// 状態
	m_aObject2D->SetSize(D3DXVECTOR2(640.0f, 360.0f));	// サイズ
	m_aObject2D->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));	// 位置
	m_aObject2D->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// 色設定

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CInstantFade::Uninit()
{
	if (m_aObject2D != nullptr)
	{// nullptrじゃなかったら

		// 終了処理
		m_aObject2D->Uninit();
		delete m_aObject2D;
		m_aObject2D = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CInstantFade::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 色取得
	D3DXCOLOR col = m_aObject2D->GetColor();

	switch (m_state)
	{
	case STATE_NONE:
		break;

	case STATE_FADEOUT:

		// 経過時間を加算
		m_fTimerMove += fDeltaTime;

		// 不透明度増加
		col.a = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, m_fDuration, m_fTimerMove);

		if (m_fTimerMove >= m_fDuration)
		{// 目標まで行ったら
			col.a = 1.0f;
			m_fTimerMove = 0.0f;
			m_state = STATE_FADECOMPLETION;
		}
		break;

	case STATE_FADEIN:

		// 経過時間を加算
		m_fTimerMove += fDeltaTime;

		// 不透明度減少
		col.a = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, m_fDuration, m_fTimerMove);

		if (m_fTimerMove >= m_fDuration)
		{// 透明になったら
			col.a = 0.0f;
			m_fTimerMove = 0.0f;
			m_state = STATE_NONE;
		}
		break;

	case STATE_FADECOMPLETION:
		m_state = STATE_FADEIN;
		break;
	}

	// 色設定
	m_aObject2D->SetColor(col);

	// 頂点座標更新
	m_aObject2D->SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CInstantFade::Draw()
{
	// 描画処理
	m_aObject2D->Draw();
}

//==========================================================================
// フェード設定
//==========================================================================
void CInstantFade::SetFade(D3DXCOLOR FadeColor, float fDuration)
{
	if (m_state == STATE_NONE)
	{// 何もしていないとき

		m_fDuration = fDuration;	// フェードまでの時間
		m_fTimerMove = 0.0f;		// フェード時間
		m_state = STATE_FADEOUT;	// フェードアウト状態に設定

		// 色設定
		m_aObject2D->SetColor(D3DXCOLOR(FadeColor.r, FadeColor.g, FadeColor.b, 0.0f));
	}
}

//==========================================================================
// 状態取得
//==========================================================================
CInstantFade::STATE CInstantFade::GetState()
{
	return m_state;
}

//==========================================================================
// オブジェクト2Dオブジェクトの取得
//==========================================================================
CObject2D *CInstantFade::GetMyObject()
{
	return m_aObject2D;
}
//=============================================================================
// 
//  スキップUI処理 [skip_ui.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "skip_ui.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "object_circlegauge2D.h"
#include "game.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\key\\B.png";
	const float TIME_FADEOUT_RATIO = 0.3f;
	const float DEFAULT_LIFE = 1.0f;
	const float TIME_PRESS = 1.5f;
}

namespace StateTime
{
	const float FADEOUT = 0.3f;
}


//==========================================================================
// コンストラクタ
//==========================================================================
CSkip_UI::CSkip_UI(int nPriority, CObject::LAYER layer) : CObject(nPriority, layer)
{
	// 値のクリア
	m_state = State::STATE_NONE;				// 状態
	m_fStateTime = 0.0f;	// 状態タイマー

	m_fPressTimer = 0.0f;			// 押下時間
	m_bCompleteSkip = false;		// スキップ完了
	m_pButton = nullptr;			// ボタン
	m_pObjCircleGauge = nullptr;	// 円ゲージ
}

//==========================================================================
// デストラクタ
//==========================================================================
CSkip_UI::~CSkip_UI()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSkip_UI* CSkip_UI::Create()
{
	// メモリの確保
	CSkip_UI* pObj = DEBUG_NEW CSkip_UI;

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
HRESULT CSkip_UI::Init()
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置
	SetPosition(MyLib::Vector3(1100.0f, 670.0f, 0.0f));

	m_state = State::STATE_FADEOUT;	// 状態
	m_fStateTime = 0.0f;	// 状態タイマー

// UI生成
	CreateUI();

	return S_OK;
}

//==========================================================================
// UI生成
//==========================================================================
void CSkip_UI::CreateUI()
{
	CreateButton();
	CreateGauge();
}

//==========================================================================
// ボタン作成
//==========================================================================
void CSkip_UI::CreateButton()
{
	if (m_pButton != nullptr)
	{
		m_pButton->SetAlpha(1.0f);
		return;
	}

	// 作成
	m_pButton = CObject2D::Create(GetPriority());

	// 種類の設定
	m_pButton->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	m_pButton->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 32.0f);
	m_pButton->SetSize(size);
	m_pButton->SetSizeOrigin(size);

	// 位置
	m_pButton->SetPosition(GetPosition());
	m_pButton->SetVtx();
}

//==========================================================================
// ゲージ作成
//==========================================================================
void CSkip_UI::CreateGauge()
{
	if (m_pObjCircleGauge != nullptr) return;

	// 作成
	m_pObjCircleGauge = CObjectCircleGauge2D::Create(32, 40.0f);
	m_pObjCircleGauge->SetType(CObject::TYPE::TYPE_OBJECT2D);

	m_pObjCircleGauge->SetPosition(GetPosition());
}

//==========================================================================
// 終了処理
//==========================================================================
void CSkip_UI::Uninit()
{
	// 解放処理
	Release();
}

//==========================================================================
// 削除
//==========================================================================
void CSkip_UI::Kill()
{
	// UI削除
	KillUI();

	// 解放処理
	Release();
}

//==========================================================================
// UI削除
//==========================================================================
void CSkip_UI::KillUI()
{
	if (m_pButton != nullptr)
	{
		m_pButton->Uninit();
		m_pButton = nullptr;
	}

	if (m_pObjCircleGauge != nullptr)
	{
		m_pObjCircleGauge->Uninit();
		m_pObjCircleGauge = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CSkip_UI::Update()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pGamepad = CInputGamepad::GetInstance();
	
	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	if (pKey->GetPress(DIK_O) ||
		pGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_B, 0))
	{// 押し中

		// 状態設定
		m_state = State::STATE_NONE;
		m_fStateTime = 0.0f;


		// タイマー加算
		m_fPressTimer += deltaTime;
		if (m_fPressTimer >= TIME_PRESS)
		{
			m_bCompleteSkip = true;
			m_fPressTimer = TIME_PRESS;
		}

		// UI作成
		CreateButton();
		CreateGauge();

		if (m_pObjCircleGauge != nullptr)
		{
			m_pObjCircleGauge->SetRate(m_fPressTimer / TIME_PRESS);
			m_pObjCircleGauge->SetVtx();
		}

	}
	else
	{// 押してない

		// 状態設定
		m_state = State::STATE_FADEOUT;

		// タイマーリセット
		m_fPressTimer = 0.0f;

		// ゲージ即削除
		if (m_pObjCircleGauge != nullptr)
		{
			m_pObjCircleGauge->Uninit();
			m_pObjCircleGauge = nullptr;
		}
	}

	if (m_state == State::STATE_FADEOUT)
	{
		StateFadeout();
	}

	
}


//==========================================================================
// 出現状態
//==========================================================================
void CSkip_UI::StateFadeout()
{
	if (m_pButton == nullptr) return;

	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 不透明度設定
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	m_pButton->SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// UI削除
		KillUI();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CSkip_UI::Draw()
{
	
}

//==========================================================================
// 状態設定
//==========================================================================
void CSkip_UI::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

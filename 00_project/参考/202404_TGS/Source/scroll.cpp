//=============================================================================
// 
//  巻き物処理 [scroll.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "scroll.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_ROLL = "data\\TEXTURE\\scroll\\scroll_roll.png";		// ロール部分のテクスチャ
	const std::string TEXTURE_PAPER = "data\\TEXTURE\\scroll\\scroll_paper.png";	// 紙部分のテクスチャ
	const std::string TEXTURE_EDGE = "data\\TEXTURE\\scroll\\scroll_edge.png";		// 端部分のテクスチャ
}

namespace StateTime	// 状態別時間
{
	const float OPEN = 0.75f;	// オープン
	const float CLOSE = 0.75f;	// クローズ
	const float FADEOUT = 0.3f;	// フェードアウト
}


//==========================================================================
// 関数ポインタ
//==========================================================================
CScroll::STATE_FUNC CScroll::m_StateFunc[] =
{
	&CScroll::StateNone,	// なし
	&CScroll::StateOpen,	// オープン
	&CScroll::StateWait,	// 待機
	&CScroll::StateWaitPress,	// 押下待機
	&CScroll::StateClose,	// クローズ
	&CScroll::StateFadeout,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CScroll::CScroll(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_fStateTimer = 0.0f;			// 状態タイマー
	m_pPapaer = nullptr;			// 紙部分
	m_pEdge = nullptr;				// 端部分
	m_bFinishOpen = false;			// オープン終了判定
	m_bAutoWaitPress = false;		// 自動押下待機判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CScroll::~CScroll()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CScroll* CScroll::Create(const MyLib::Vector3& pos, const float toOpenTime, const float height, const float scrollLength, bool bAutoWaitPress, bool bFadeOut, int nPriority)
{
	// メモリの確保
	CScroll* pObj = DEBUG_NEW CScroll(nPriority + 1);

	if (pObj != nullptr)
	{
		// 引数情報設定
		pObj->SetPosition(pos);								// 位置
		pObj->SetOriginPosition(pos);						// 位置
		pObj->SetSizeOrigin(D3DXVECTOR2(height, height));	// サイズ
		pObj->m_fToOpenTimer = toOpenTime;					// オープンまでの時間
		pObj->m_fScrollLength = scrollLength;				// 巻き物の長さ
		pObj->m_bFadeOut = bFadeOut;						// フェードアウト判定
		pObj->m_bAutoWaitPress = bAutoWaitPress;			// 自動押下待機判定

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CScroll::Init()
{
	// 紙部分生成
	CreatePaper();

	// 端部分生成
	CreateEdge();


	// オブジェクト2Dの初期化
	CObject2D::Init();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(TEXTURE_ROLL);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = pTexture->GetImageSize(texID);

	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, GetSizeOrigin().y);
	SetSize(size);
	SetSizeOrigin(size);

	// 状態
	m_state = STATE::STATE_NONE;
	m_fStateTimer = 0.0f;

	return S_OK;
}

//==========================================================================
// 紙部分生成
//==========================================================================
void CScroll::CreatePaper()
{
	// 紙部分
	m_pPapaer = CObject2D::Create(GetPriority() - 1);
	if (m_pPapaer == nullptr) return;

	// 種類の設定
	m_pPapaer->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(TEXTURE_PAPER);
	m_pPapaer->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = pTexture->GetImageSize(texID);

	// ロール部分の縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, GetSizeOrigin().y);
	size.x = 0.0f;
	m_pPapaer->SetSize(size);
	m_pPapaer->SetSizeOrigin(size);

	// 位置設定
	MyLib::Vector3 rollpos = GetPosition();
	m_pPapaer->SetPosition(rollpos);
}

//==========================================================================
// 端部分生成
//==========================================================================
void CScroll::CreateEdge()
{
	// 端部分
	m_pEdge = CObject2D::Create(GetPriority());
	if (m_pEdge == nullptr) return;

	// 種類の設定
	m_pEdge->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = pTexture->Regist(TEXTURE_EDGE);
	m_pEdge->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = pTexture->GetImageSize(texID);

	// ロール部分の縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, GetSizeOrigin().y);
	m_pEdge->SetSize(size);
	m_pEdge->SetSizeOrigin(size);

	// 位置設定
	MyLib::Vector3 rollpos = GetPosition();
	m_pEdge->SetPosition(rollpos + MyLib::Vector3(GetSize().x, 0.0f, 0.0f));

}

//==========================================================================
// 終了処理
//==========================================================================
void CScroll::Uninit()
{
	m_pPapaer = nullptr;
	m_pEdge = nullptr;

	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CScroll::Kill()
{
	// 紙部分の終了
	if (m_pPapaer != nullptr)
	{
		m_pPapaer->Uninit();
		m_pPapaer = nullptr;
	}

	// 端部分の終了
	if (m_pEdge != nullptr)
	{
		m_pEdge->Uninit();
		m_pEdge = nullptr;
	}

	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CScroll::Update()
{
	// フラグリセット
	ResetFlag();

	// 状態更新
	UpdateState();

	if (IsDeath()) return;

	// ロール部分の調整
	AdjustRoll();

	// 端部分の調整
	AdjustEdge();

	// 終了処理
	CObject2D::Update();
}

//==========================================================================
// フラグリセット
//==========================================================================
void CScroll::ResetFlag()
{
	m_bFinishOpen = false;
}

//==========================================================================
// 状態更新
//==========================================================================
void CScroll::UpdateState()
{
	// 状態タイマー
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// なにもなし
//==========================================================================
void CScroll::StateNone()
{
	// オープンスキップ
	OpenSkip();

	if (m_fStateTimer >= m_fToOpenTimer)
	{// オープンまでの時間経過

		// 状態遷移
		SetState(STATE::STATE_OPEN);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_OPEN);
	}
}

//==========================================================================
// オープン
//==========================================================================
void CScroll::StateOpen()
{
	// オープンスキップ
	OpenSkip();

	// サイズ取得
	D3DXVECTOR2 size = m_pPapaer->GetSize();

	// 線形補間
	size.x = UtilFunc::Correction::EasingEaseOut(m_pPapaer->GetSizeOrigin().x, m_fScrollLength * 0.5f, 0.0f, StateTime::OPEN, m_fStateTimer);

	// サイズ設定
	m_pPapaer->SetSize(size);


	if (m_fStateTimer >= StateTime::OPEN)
	{// 時間経過

		// 状態遷移
		if (m_bAutoWaitPress)
		{
			SetState(STATE::STATE_WAITPRESS);
		}
		else
		{
			SetState(STATE::STATE_WAIT);
		}

		// オープン終了判定
		m_bFinishOpen = true;
	}
}

//==========================================================================
// 待機
//==========================================================================
void CScroll::StateWait()
{

}

//==========================================================================
// 押下待機
//==========================================================================
void CScroll::StateWaitPress()
{
	// インプット取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0))
	{
		// 状態遷移
		SetState(STATE::STATE_CLOSE);
	}
}

//==========================================================================
// クローズ
//==========================================================================
void CScroll::StateClose()
{
	// サイズ取得
	D3DXVECTOR2 size = m_pPapaer->GetSize();

	// 線形補間
	size.x = UtilFunc::Correction::EasingEaseIn(m_fScrollLength * 0.5f, 0.0f, 0.0f, StateTime::CLOSE, m_fStateTimer);

	// サイズ設定
	m_pPapaer->SetSize(size);

	if (m_fStateTimer >= StateTime::CLOSE)
	{// 時間経過

		if (m_bFadeOut)
		{
			SetState(STATE::STATE_FADEOUT);
		}
		else
		{
			Kill();
		}
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CScroll::StateFadeout()
{

	// 不透明度設定
	m_pPapaer->SetAlpha(UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTimer));

	if (m_fStateTimer >= StateTime::FADEOUT)
	{// 時間経過
		Kill();
	}
}

//==========================================================================
// ロール部分の調整
//==========================================================================
void CScroll::AdjustRoll()
{
	// 位置取得
	MyLib::Vector3 rollPos = GetPosition(), rollPosOrigin = GetOriginPosition();

	// 線形補間
	rollPos.x = rollPosOrigin.x - m_pPapaer->GetSize().x;

	// 位置設定
	SetPosition(rollPos);

	// 不透明度設定
	SetAlpha(m_pPapaer->GetAlpha());

	m_pPapaer->SetVtx();
}

//==========================================================================
// 端部分の調整
//==========================================================================
void CScroll::AdjustEdge()
{
	// 位置取得
	MyLib::Vector3 edgePos = m_pEdge->GetPosition(), rollPosOrigin = GetOriginPosition();

	// 線形補間
	edgePos.x = rollPosOrigin.x + m_pPapaer->GetSize().x;

	// 位置設定
	m_pEdge->SetPosition(edgePos);

	// 不透明度設定
	m_pEdge->SetAlpha(m_pPapaer->GetAlpha());

	m_pEdge->SetVtx();
}

//==========================================================================
// オープンスキップ
//==========================================================================
void CScroll::OpenSkip()
{
	// インプット取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// スキップ
	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0))
	{
		// 状態遷移
		SetState(STATE::STATE_OPEN);
		m_fStateTimer = StateTime::OPEN;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CScroll::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CScroll::SetState(const STATE& state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}

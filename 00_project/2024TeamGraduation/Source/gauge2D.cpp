//============================================================
//
//	ゲージ2D処理 [gauge2D.cpp]
//	Author：Kai Takada
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gauge2D.h"
#include "manager.h"
#include "texture.h"
#include "ball.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int MAX_VERTEX = 12;	// 頂点数
	const int PRIORITY	 = 5;	// ゲージ2Dの優先順位
	const char* PASS_BAR = "data\\TEXTURE\\gauge\\bar_01.jpg";		// パス(ゲージ)
	const char* PASS_FRAME = "data\\TEXTURE\\gauge\\frame.png";		// パス(枠)
	const char* PASS_ASSIST = "data\\TEXTURE\\gauge\\assist.png";	// アシスト(ボタン)
	const float FRAME_RAT = 1.1f;	// 枠の大きさ倍率(ゲージの〇倍)
}

// max時発光
namespace Bright
{
	const MyLib::PosGrid3 END = MyLib::PosGrid3(318, 80, 100);		// 終了色
	const MyLib::PosGrid3 START = MyLib::PosGrid3(350, 20, 100);	// 開始色
	const float END_TIME = 0.5f;									// 終了時間
}

// max時発光
namespace Assist
{
	const MyLib::PosGrid3 END = MyLib::PosGrid3(45, 80, 100);		// 終了色
	const MyLib::PosGrid3 START = MyLib::PosGrid3(60, 20, 100);		// 開始色
}

//************************************************************
//	静的関数
//************************************************************
float CGauge2D::m_fBrightTime = 0.0f;						// maxの時光る色
float CGauge2D::m_fBrightTimeEnd = Bright::END_TIME;		// maxの時光る色終了時間

//************************************************************
//	子クラス [CGauge2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGauge2D::CGauge2D(const float nFrame) : CObject(PRIORITY, CObject::LAYER::LAYER_2D),
	m_fFrame			(nFrame),									// 表示値の変動フレーム定数
	m_team				(CGameManager::ETeamSide::SIDE_NONE),		// チーム
	m_state				(STATE_NONE),								// 状態
	m_fNumGauge			(0),										// 表示値
	m_fChange			(0.0f),										// ゲージ変動量
	m_fStateTime		(0),										// 状態管理カウンター
	m_fMaxNumGauge		(0),										// 表示値の最大値
	m_fCurrentNumGauge	(0.0f),										// 現在表示値
	m_bDrawFrame		(false),									// 枠表示状況
	m_pBg				(nullptr),									// 背景
	m_pBar				(nullptr),									// ゲージ
	m_pFrame			(nullptr),									// フレーム
	m_pAssist			(nullptr)									// ボタンアシスト
{

}

//============================================================
//	デストラクタ
//============================================================
CGauge2D::~CGauge2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGauge2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// メンバ変数を初期化
	m_state				= STATE_NONE;				// 状態
	m_fNumGauge			= 0;						// 表示値
	m_fChange			= 0.0f;						// ゲージ変動量
	m_fStateTime		= 0;						// 状態管理カウンター
	m_fMaxNumGauge		= 0;						// 表示値の最大値
	m_fCurrentNumGauge	= 0.0f;						// 現在表示値
	m_bDrawFrame		= false;					// 枠表示状況

	m_pBg = CObject2D::Create(PRIORITY);					// 背景
	m_pBar = CObject2D::Create(PRIORITY);					// ゲージ
	m_pFrame = CObject2D::Create(PRIORITY);					// フレーム
	m_pAssist = CObject2D::Create(PRIORITY);				// ボタンアシスト

	SetType(CObject::TYPE::TYPE_UI);
	
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGauge2D::Uninit()
{
	// ポリゴンの破棄
	SAFE_UNINIT(m_pBg);
	SAFE_UNINIT(m_pBar);
	SAFE_UNINIT(m_pFrame);
	SAFE_UNINIT(m_pAssist);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CGauge2D::Kill()
{
	// 自身の終了
	CGauge2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGauge2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pBar == nullptr) return;
	MyLib::Vector2 size = m_pBar->GetSize();

	// ゲージの設定
	if (m_state == STATE_CHANGE)
	{ // 体力が変動中の場合

		// カウンターを減算
		m_fStateTime--;

		// 現在のゲージに変動量を加算
		m_fCurrentNumGauge += m_fChange;

		// 現在の表示値を補正
		UtilFunc::Transformation::ValueNormalize(m_fCurrentNumGauge, m_fMaxNumGauge, 0.0f);

		// 頂点情報の設定
		m_pBar->SetSize(size);

		if (m_fStateTime <= 0)
		{ // カウンターが 0以下になった場合

			// 通常状態にする
			m_state = STATE_NONE;
		}
	}

	if (m_fCurrentNumGauge == m_fMaxNumGauge)
	{// ゲージMAX時

		// バーを発光させる
		BrightBar();
	}
	else
	{// それ以外

		// 色設定
		m_pBar->SetColor(m_pBar->GetOriginColor());
	}
}

//============================================================
//	描画処理
//============================================================
void CGauge2D::Draw()
{


}

//============================================================
//	位置の設定処理
//============================================================
void CGauge2D::SetPosition(const MyLib::Vector3& rPos)
{
	if (m_pBg == nullptr ||
		m_pBar == nullptr ||
		m_pFrame == nullptr||
		m_pAssist == nullptr)
	{// １つでもなかったら
		MyAssert::CustomAssert(false, "Gauge2D: なんで無いん？");
	}

	// 引数の位置を設定
	CObject::SetPosition(rPos);

	// TODO: ずらせ
	m_pBg->SetPosition(rPos);
	m_pBar->SetPosition(rPos);
	m_pFrame->SetPosition(rPos);
	m_pAssist->SetPosition(rPos);
}

//============================================================
//	テクスチャ座標の設定
//============================================================
void CGauge2D::SetTexUV(const std::vector<D3DXVECTOR2>& uv)
{
	if (m_pBg == nullptr ||
		m_pBar == nullptr ||
		m_pFrame == nullptr)
	{// １つでもなかったら
		MyAssert::CustomAssert(false, "Gauge2D: なんで無いん？");
	}

	m_pBg->SetTexUV(uv);
	m_pBar->SetTexUV(uv);
	m_pFrame->SetTexUV(uv);
	m_pAssist->SetTexUV(uv);
}

//============================================================
//	生成処理
//============================================================
CGauge2D* CGauge2D::Create
(
	const float fMax,					// 最大表示値
	const float fFrame,					// 表示値変動フレーム
	const MyLib::Vector3& rPos,			// 位置
	const MyLib::Vector2& rSizeGauge,	// ゲージ大きさ
	const D3DXCOLOR& rColFront,			// 表ゲージ色
	const D3DXCOLOR& rColBack,			// 裏ゲージ色
	const bool bDrawFrame				// 枠描画状況
)
{
	// ゲージ2Dの生成
	CGauge2D* pGauge2D = new CGauge2D(fFrame);
	if (pGauge2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ゲージ2Dの初期化
		if (FAILED(pGauge2D->Init()))
		{ // 初期化に失敗した場合

			// ゲージ2Dの破棄
			SAFE_DELETE(pGauge2D);
			return nullptr;
		}

		// テクスチャを登録・割当
		pGauge2D->BindTexture();

		// ゲージ最大値を設定
		pGauge2D->SetMaxNum(fMax);

		// 位置を設定
		pGauge2D->SetPosition(rPos);

		// 大きさを設定
		pGauge2D->SetSize(rSizeGauge);		// 全部大きさ
		pGauge2D->InitSize();				// 初期大きさ設定

		// 色を設定
		pGauge2D->SetColorFront(rColFront);	// 表ゲージ色
		pGauge2D->SetColorBack(rColBack);	// 裏ゲージ色

		// 枠の表示状況を設定
		pGauge2D->SetEnableDrawFrame(bDrawFrame);

		// 確保したアドレスを返す
		return pGauge2D;
	}
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CGauge2D::BindTexture()
{
	CTexture* pTex = CTexture::GetInstance();

	// テクスチャ割り当て
	m_pBg->BindTexture(pTex->Regist(""));
	m_pBar->BindTexture(pTex->Regist(PASS_BAR));
	m_pFrame->BindTexture(pTex->Regist(PASS_FRAME));
	m_pAssist->BindTexture(pTex->Regist(PASS_ASSIST));
}

//============================================================
// 初期サイズ設定
//============================================================
void CGauge2D::InitSize()
{
	m_pBg->SetSizeOrigin(m_pBg->GetSize());
	m_pBar->SetSizeOrigin(m_pBar->GetSize());
	m_pFrame->SetSizeOrigin(m_pFrame->GetSize());
	m_pAssist->SetSizeOrigin(m_pAssist->GetSize());
}

//============================================================
// ゲージ発光
//============================================================
void CGauge2D::BrightBar()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	CPlayer* pPlayer = pBall->GetPlayer();

	MyLib::Vector3 end = Bright::END;
	MyLib::Vector3 start = Bright::START;
	MyLib::Vector3 easingBar = MyLib::Vector3();
	MyLib::Vector3 easingAssist = MyLib::Vector3();

	if (m_fBrightTime >= m_fBrightTimeEnd * 0.5f)
	{// 半分を超えたら

		easingBar = UtilFunc::Correction::EasingLinear(end, start, m_fBrightTimeEnd * 0.5f, m_fBrightTimeEnd, m_fBrightTime);

		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN ||
			pPlayer->GetTeam() == m_team)
		{// ボールを自陣内野が持っているとき

			easingAssist = UtilFunc::Correction::EasingLinear(Assist::END, Assist::START, m_fBrightTimeEnd * 0.5f, m_fBrightTimeEnd, m_fBrightTime);
		}
	}
	else
	{
		easingBar = UtilFunc::Correction::EasingLinear(start, end, 0.0f, m_fBrightTimeEnd * 0.5f, m_fBrightTime);
	
		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN ||
			pPlayer->GetTeam() == m_team)
		{// ボールを自陣内野が持っているとき

			easingAssist = UtilFunc::Correction::EasingLinear(Assist::START, Assist::END, 0.0f, m_fBrightTimeEnd * 0.5f, m_fBrightTime);
		}
	}

	// イージングした値をColor型に変換
	MyLib::Color colBar = UtilFunc::Transformation::HSVtoRGB(easingBar.x, easingBar.y, easingBar.z);
	MyLib::Color colAssist = UtilFunc::Transformation::HSVtoRGB(easingAssist.x, easingAssist.y, easingAssist.z);

	// 色設定
	m_pBar->SetColor(colBar);
	m_pAssist->SetColor(colAssist);
}

//============================================================
//	ゲージの加算処理
//============================================================
void CGauge2D::AddNum(const float fAdd)
{
	// 現在の表示値を設定
	m_fCurrentNumGauge = (float)m_fNumGauge;

	// 表示値の変動量を求める
	m_fChange = fAdd / m_fFrame;

	// 情報を設定
	m_state = STATE_CHANGE;		// ゲージ変動状態
	m_fStateTime = m_fFrame;	// 状態管理カウンター

	// 表示値を引数分加算
	m_fNumGauge += fAdd;

	// 表示値の制限
	UtilFunc::Transformation::ValueNormalize(m_fNumGauge, m_fMaxNumGauge, 0.0f);
}

//============================================================
//	ゲージの加算処理
//============================================================
void CGauge2D::SubNum(const float fSub)
{
	// 現在の表示値を設定
	m_fCurrentNumGauge = (float)m_fNumGauge;

	// 表示値の変動量を求める
	m_fChange = fSub / m_fFrame;

	// 情報を設定
	m_state = STATE_CHANGE;		// ゲージ変動状態
	m_fStateTime = m_fFrame;	// 状態管理カウンター

	// 表示値を引数分加算
	m_fNumGauge += fSub;

	// 表示値の制限
	UtilFunc::Transformation::ValueNormalize(m_fNumGauge, m_fMaxNumGauge, 0.0f);
}

//============================================================
//	ゲージの設定処理
//============================================================
void CGauge2D::SetNum(const float nNum)
{
	// 引数の表示値を設定
	m_fNumGauge = nNum;
	UtilFunc::Transformation::ValueNormalize(m_fNumGauge, m_fMaxNumGauge, 0.0f);	// 表示値の制限

	// 現在の表示値を設定
	m_fCurrentNumGauge = m_fNumGauge;
	UtilFunc::Transformation::ValueNormalize(m_fCurrentNumGauge, m_fMaxNumGauge, 0.0f);	// 現在の表示値の制限

	// 情報を設定
	m_state = STATE_NONE;	// ゲージ変動状態
	m_fStateTime = 0;	// 状態管理カウンター
}

//============================================================
//	枠オフセットの設定処理
//============================================================
void CGauge2D::SetAnchorType(const CObject2D::AnchorPoint& type)
{
	// アンカーポイント設定
	m_pBg->SetAnchorType(type);
	m_pBar->SetAnchorType(type);
	m_pFrame->SetAnchorType(type);
	m_pAssist->SetAnchorType(type);
}

//============================================================
//	ゲージ大きさの設定処理
//============================================================
void CGauge2D::SetSize(const MyLib::Vector2& rSize)
{
	MyLib::Vector2 sizeFrame = rSize * FRAME_RAT;
	MyLib::Vector2 sizeAssist = rSize;
	sizeAssist.x *= 0.2f;

	m_pBg->SetSize(rSize);
	m_pBar->SetSize(rSize);
	m_pFrame->SetSize(sizeFrame);
	m_pAssist->SetSize(sizeAssist);
}

//============================================================
//	ゲージ大きさの設定処理
//============================================================
void CGauge2D::SetSizeGauge(const MyLib::Vector2& rSize)
{
	// サイズ
	m_pBar->SetSize(rSize);
}

//============================================================
//	ゲージ大きさの設定処理
//============================================================
void CGauge2D::SetSizeGaugeRadius(const float fRadius)
{
	// サイズ
	MyLib::Vector2 size = m_pBar->GetSizeOrigin();
	size.x *= fRadius;

	m_pBar->SetSize(size);
}

//============================================================
//	枠大きさの設定処理
//============================================================
void CGauge2D::SetSizeFrame(const MyLib::Vector2& rSize)
{
	// サイズ
	m_pFrame->SetSize(rSize);
}

//============================================================
//	背景大きさの設定処理
//============================================================
void CGauge2D::SetSizeBg(const MyLib::Vector2& rSize)
{
	// サイズ
	m_pBg->SetSize(rSize);
}

//============================================================
//	表ゲージ色の設定処理
//============================================================
void CGauge2D::SetColorFront(const D3DXCOLOR& rCol)
{
	// 引数のゲージ色を代入
	m_pBar->SetColor(rCol);
	m_pBar->SetOriginColor(rCol);
}

//============================================================
//	裏ゲージ色の設定処理
//============================================================
void CGauge2D::SetColorBack(const D3DXCOLOR& rCol)
{
	// 引数の背景色を代入
	m_pBg->SetColor(rCol);
	m_pBg->SetOriginColor(rCol);
}

//============================================================
//	ゲージ最大値の設定処理
//============================================================
void CGauge2D::SetMaxNum(const float fMax)
{
	// 引数の表示最大値を設定
	m_fMaxNumGauge = fMax;

	// ゲージの設定
	SetNum(m_fMaxNumGauge);
}

//============================================================
//	枠表示状況設定処理
//============================================================
void CGauge2D::SetEnableDrawFrame(const bool bDraw)
{
	// 引数の枠の表示状況を設定
	m_bDrawFrame = bDraw;
}

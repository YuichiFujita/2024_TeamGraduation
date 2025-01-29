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
#include "string2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY	 = 5;	// ゲージ2Dの優先順位
	const char* PASS_BAR = "data\\TEXTURE\\gauge\\bar_01.jpg";		// パス(ゲージ)
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
	const MyLib::PosGrid3 END = MyLib::PosGrid3(175, 40, 100);		// 終了色
	const MyLib::PosGrid3 START = MyLib::PosGrid3(227, 0, 97);		// 開始色
	const float END_ALPHA = 1.0f;									// 終了色(透明度)
	const float START_ALPHA = 0.45f;								// 開始色(透明度)
	const float END_TIME = 2.2f;									// 終了時間
}

// チーム名
namespace Name
{
	const MyLib::Vector3 POS[] = { MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 95.0f, 0.0f), MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 95.0f, 0.0f) };	// 原点位置
	const char*	FONT	= "data\\FONT\\Kazesawa Bold.ttf";	// フォントパス
	const bool	ITALIC	= false;							// イタリック
	const float	HEIGHT	= 19.0f;							// 文字縦幅
	const D3DXCOLOR COL	= MyLib::color::Black();			// 色
}

//************************************************************
//	静的関数
//************************************************************
float CGauge2D::m_fBrightTime = 0.0f;					// maxの時光る色
float CGauge2D::m_fBrightTimeEnd = Bright::END_TIME;	// maxの時光る色終了時間
float CGauge2D::m_fAssistTime = 0.0f;					// アシストカウンター
float CGauge2D::m_fAssistTimeEnd = Assist::END_TIME;	// アシストカウンター

//************************************************************
//	子クラス [CGauge2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGauge2D::CGauge2D(const float nFrame) : CObject(PRIORITY, CObject::LAYER::LAYER_2D),
	m_fFrame			(nFrame),								// 表示値の変動フレーム定数
	m_team				(CGameManager::ETeamSide::SIDE_NONE),	// チーム
	m_state				(STATE_NONE),	// 状態
	m_fNumGauge			(0.0f),			// 表示値
	m_fChange			(0.0f),			// ゲージ変動量
	m_fStateTime		(0.0f),			// 状態管理カウンター
	m_fMaxNumGauge		(0.0f),			// 表示値の最大値
	m_fCurrentNumGauge	(0.0f),			// 現在表示値
	m_pBg				(nullptr),		// 背景
	m_pBar				(nullptr),		// ゲージ
	m_pFrame			(nullptr),		// フレーム
	m_pAssist			(nullptr),		// ボタンアシスト
	m_pTeamName			(nullptr),		// チーム名
	m_offsetFrame		(VEC3_ZERO),	// フレームオフセット
	m_offsetAssist		(VEC3_ZERO),	// アシストオフセット
	m_offsetName		(VEC3_ZERO)		// チーム名オフセット
{
	m_fSizeFrame = FRAME_RAT;
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
	// ポリゴンの生成
	m_pBg = CObject2D::Create(PRIORITY);		// 背景
	m_pBar = CObject2D::Create(PRIORITY);		// ゲージ
	m_pFrame = CObject2D::Create(PRIORITY);		// フレーム
	m_pAssist = CObject2D::Create(PRIORITY);	// ボタンアシスト

	// チーム名の生成
	m_pTeamName = CString2D::Create
	( // 引数
		Name::FONT,		// フォントパス
		Name::ITALIC,	// イタリック
		L"あいうえお",	// 指定文字列
		VEC3_ZERO,		// 原点位置
		Name::HEIGHT,	// 文字縦幅
		XALIGN_CENTER,	// 横配置
		VEC3_ZERO,		// 原点向き
		Name::COL		// 色
	);
	if (m_pTeamName == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 
	m_pTeamName->SetType(CObject::TYPE::TYPE_UI);

	// 世界停止中でも動く！
	SetEnablePosibleMove_WorldPause(true);
	m_pBg->SetEnablePosibleMove_WorldPause(true);
	m_pBar->SetEnablePosibleMove_WorldPause(true);
	m_pFrame->SetEnablePosibleMove_WorldPause(true);
	m_pAssist->SetEnablePosibleMove_WorldPause(true);
	m_pTeamName->SetEnablePosibleMove_WorldPause(true);

	// 種類をUIにする
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

	// チーム名の破棄
	SAFE_UNINIT(m_pTeamName);

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

	ImGui::Text("GaugeUpdate");

	// ゲージの設定
	if (m_state == STATE_CHANGE)
	{ // 体力が変動中の場合

		// カウンターを減算
		m_fStateTime -= fDeltaTime * fSlowRate;

		// 現在のゲージに変動量を加算
		m_fCurrentNumGauge += m_fChange;

		// 現在の表示値を補正
		UtilFunc::Transformation::ValueNormalize(m_fCurrentNumGauge, m_fMaxNumGauge, 0.0f);

		// 頂点情報の設定
		m_pBar->SetSize(size);

		if (m_fStateTime <= 0.0f)
		{
			// 通常状態にする
			m_state = STATE_NONE;
		}
	}

	if (m_fCurrentNumGauge == m_fMaxNumGauge)
	{ // ゲージMAX時

		// バーを発光させる
		BrightBar();

		// アシストを発光させる
		BrightAssist();
	}
	else
	{ // それ以外

		// 色設定
		m_pBar->SetColor(m_pBar->GetOriginColor());
		m_pAssist->SetAlpha(0.0f);
	}
}

//============================================================
//	描画処理
//============================================================
void CGauge2D::Draw()
{


}

//============================================================
//	描画状況の設定処理
//============================================================
void CGauge2D::SetEnableDisp(const bool bDisp)
{
	m_pBg->SetEnableDisp(bDisp);
	m_pBar->SetEnableDisp(bDisp);
	m_pAssist->SetEnableDisp(bDisp);
	m_pFrame->SetEnableDisp(bDisp);
	m_pTeamName->SetEnableDisp(bDisp);
}

//============================================================
//	位置の初期化処理
//============================================================
void CGauge2D::InitPosition()
{
	m_pBg->SetOriginPosition(m_pBg->GetPosition());
	m_pBar->SetOriginPosition(m_pBar->GetPosition());
	m_pAssist->SetOriginPosition(m_pAssist->GetPosition());
	m_pFrame->SetOriginPosition(m_pFrame->GetPosition());
	m_pTeamName->SetOriginPosition(m_pTeamName->GetPosition());
}

//============================================================
//	大きさの初期化処理
//============================================================
void CGauge2D::InitSize()
{
	m_pBg->SetSizeOrigin(m_pBg->GetSize());
	m_pBar->SetSizeOrigin(m_pBar->GetSize());
	m_pAssist->SetSizeOrigin(m_pAssist->GetSize());
	m_pFrame->SetSizeOrigin(m_pFrame->GetSize());
}

//============================================================
//	位置の設定処理
//============================================================
void CGauge2D::SetPosition(const MyLib::Vector3& rPos)
{
	if (m_pBg == nullptr
	||  m_pBar == nullptr
	||  m_pFrame == nullptr
	||  m_pAssist == nullptr
	||  m_pTeamName == nullptr)
	{ // １つでもなかったら

		MyAssert::CustomAssert(false, "Gauge2D: なんで無いん？");
	}

	// 引数の位置を設定
	CObject::SetPosition(rPos);

	// 相対位置を設定
	SetPositionRelative();
}

//============================================================
//	テクスチャ座標の設定
//============================================================
void CGauge2D::SetTexUV(const std::vector<D3DXVECTOR2>& uv)
{
	if (m_pBg == nullptr
	||  m_pBar == nullptr
	||  m_pFrame == nullptr
	||  m_pAssist == nullptr
	||  m_pTeamName == nullptr)
	{ // １つでもなかったら

		MyAssert::CustomAssert(false, "Gauge2D: なんで無いん？");
	}

	m_pBg->SetTexUV(uv);
	m_pBar->SetTexUV(uv);
	m_pFrame->SetTexUV(uv);
}

//============================================================
//	生成処理
//============================================================
CGauge2D* CGauge2D::Create
(
	const float fMax,						// 最大表示値
	const float fFrame,						// 表示値変動フレーム
	const std::string& rTeamName,			// チーム名
	const MyLib::Vector3& rPos,				// 位置
	const MyLib::Vector3& rOffsetFrame,		// フレームオフセット
	const MyLib::Vector3& rOffsetAssist,	// アシストオフセット
	const MyLib::Vector3& rOffsetName,		// チーム名オフセット
	const MyLib::Vector2& rSizeGauge,		// ゲージ大きさ
	const MyLib::Vector2& rSizeFrame,		// フレーム大きさ
	const MyLib::Vector2& rSizeAssist,		// アシスト大きさ
	const D3DXCOLOR& rColFront,				// 表ゲージ色
	const D3DXCOLOR& rColBack 				// 裏ゲージ色
)
{
	// ゲージ2Dの生成
	CGauge2D* pGauge2D = DEBUG_NEW CGauge2D(fFrame);
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

		// ゲージ最大値を設定
		pGauge2D->SetMaxNum(fMax);

		// チーム名を設定
		pGauge2D->m_pTeamName->SetString(rTeamName);

		// 位置を設定
		pGauge2D->SetPosition(rPos);

		// 位置を初期化
		pGauge2D->InitPosition();

		// オフセットを設定
		pGauge2D->SetOffsetFrame(rOffsetFrame);		// フレームオフセット
		pGauge2D->SetOffsetAssist(rOffsetAssist);	// アシストオフセット
		pGauge2D->SetOffsetTeamName(rOffsetAssist);	// チーム名オフセット

		// 大きさを設定
		pGauge2D->SetSizeGauge(rSizeGauge);		// ゲージ大きさ
		pGauge2D->SetSizeFrame(rSizeFrame);		// フレーム大きさ
		pGauge2D->SetSizeAssist(rSizeAssist);	// アシスト大きさ

		// 大きさを初期化
		pGauge2D->InitSize();

		// 色を設定
		pGauge2D->SetColorFront(rColFront);	// 表ゲージ色
		pGauge2D->SetColorBack(rColBack);	// 裏ゲージ色

		// 確保したアドレスを返す
		return pGauge2D;
	}
}

//============================================================
//	背景テクスチャ割当処理 (インデックス)
//============================================================
void CGauge2D::BindTextureBG(const int nTexID)
{
	// 背景テクスチャの割当
	m_pBg->BindTexture(nTexID);
}

//============================================================
//	ゲージテクスチャ割当処理 (インデックス)
//============================================================
void CGauge2D::BindTextureBar(const int nTexID)
{
	// ゲージテクスチャの割当
	m_pBar->BindTexture(nTexID);
}

//============================================================
//	フレームテクスチャ割当処理 (インデックス)
//============================================================
void CGauge2D::BindTextureFrame(const int nTexID)
{
	// フレームテクスチャの割当
	m_pFrame->BindTexture(nTexID);
}

//============================================================
//	アシストテクスチャ割当処理 (インデックス)
//============================================================
void CGauge2D::BindTextureAssist(const int nTexID)
{
	// アシストテクスチャの割当
	m_pAssist->BindTexture(nTexID);
}

//============================================================
//	ゲージ発光
//============================================================
void CGauge2D::BrightBar()
{
	MyLib::Vector3 start = Bright::START;	// 開始色
	MyLib::Vector3 end = Bright::END;		// 終了色
	MyLib::Vector3 easingBar =VEC3_ZERO;	// ゲージ色
	if (m_fBrightTime >= m_fBrightTimeEnd * 0.5f)
	{
		easingBar = UtilFunc::Correction::EasingLinear(end, start, m_fBrightTimeEnd * 0.5f, m_fBrightTimeEnd, m_fBrightTime);
	}
	else
	{
		easingBar = UtilFunc::Correction::EasingLinear(start, end, 0.0f, m_fBrightTimeEnd * 0.5f, m_fBrightTime);
	}

	// イージングした値をColor型に変換
	MyLib::Color colBar = UtilFunc::Transformation::HSVtoRGB(easingBar.x, easingBar.y, easingBar.z);

	// 色設定
	m_pBar->SetColor(colBar);
}

//============================================================
//	アシスト発光
//============================================================
void CGauge2D::BrightAssist()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// ボール情報
	CPlayer* pPlayer = pBall->GetPlayer();		// プレイヤー情報
	MyLib::Vector3 easingAssist = VEC3_ZERO;	// アシスト色
	float fAlphaAssist = 0.0f;					// アシスト透明度
	if (m_fAssistTime >= m_fAssistTimeEnd * 0.5f)
	{
		if (pPlayer != nullptr)
		{
			if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN
			&&  pPlayer->GetTeam() == m_team)
			{ // ボールを自陣内野が持っているとき

				easingAssist = UtilFunc::Correction::EasingLinear(Assist::END, Assist::START, m_fAssistTimeEnd * 0.5f, m_fAssistTimeEnd, m_fAssistTime);
				fAlphaAssist = UtilFunc::Correction::EasingLinear(Assist::END_ALPHA, Assist::START_ALPHA, m_fAssistTimeEnd * 0.5f, m_fAssistTimeEnd, m_fAssistTime);
			}
		}
	}
	else
	{
		if (pPlayer != nullptr)
		{
			if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN
			&&  pPlayer->GetTeam() == m_team)
			{ // ボールを自陣内野が持っているとき

				easingAssist = UtilFunc::Correction::EasingLinear(Assist::START, Assist::END, 0.0f, m_fAssistTimeEnd * 0.5f, m_fAssistTime);
				fAlphaAssist = UtilFunc::Correction::EasingLinear(Assist::START_ALPHA, Assist::END_ALPHA, 0.0f, m_fAssistTimeEnd * 0.5f, m_fAssistTime);
			}
		}
	}

	// イージングした値をColor型に変換
	MyLib::Color colAssist = UtilFunc::Transformation::HSVtoRGB(easingAssist.x, easingAssist.y, easingAssist.z);
	colAssist.a = fAlphaAssist;

	// 色設定
	m_pAssist->SetColor(colAssist);
}

//============================================================
//	相対位置の設定処理
//============================================================
void CGauge2D::SetPositionRelative()
{
	MyLib::Vector3 pos = GetPosition();

	m_pBg->SetPosition(pos);
	m_pBar->SetPosition(pos);
	m_pFrame->SetPosition(pos + m_offsetFrame);
	m_pAssist->SetPosition(pos + m_offsetAssist);
	m_pTeamName->SetPosition(pos + m_offsetName);
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
	m_fStateTime = 0.0f;	// 状態管理カウンター
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
void CGauge2D::SetSizeGaugeRadius(const float fRadius)
{
	// サイズ
	MyLib::Vector2 size = m_pBar->GetSizeOrigin();
	size.x *= fRadius;

	m_pBar->SetSize(size);
}

//============================================================
//	ゲージ大きさの設定処理
//============================================================
void CGauge2D::SetSizeGauge(const MyLib::Vector2& rSize)
{
	m_pBg->SetSize(rSize);
	m_pBar->SetSize(rSize);
}

//============================================================
//	フレーム大きさの設定処理
//============================================================
void CGauge2D::SetSizeFrame(const MyLib::Vector2& rSize)
{
	m_pFrame->SetSize(rSize);
}

//============================================================
//	アシスト大きさの設定処理
//============================================================
void CGauge2D::SetSizeAssist(const MyLib::Vector2& rSize)
{
	m_pAssist->SetSize(rSize);
}

//============================================================
//	フレームオフセットの設定処理
//============================================================
void CGauge2D::SetOffsetFrame(const MyLib::Vector3& rOffset)
{
	// 引数のフレームオフセットを代入
	m_offsetFrame = rOffset;

	// 相対位置を設定
	SetPositionRelative();
}

//============================================================
//	アシストオフセットの設定処理
//============================================================
void CGauge2D::SetOffsetAssist(const MyLib::Vector3& rOffset)
{
	// 引数のアシストオフセットを代入
	m_offsetAssist = rOffset;

	// 相対位置を設定
	SetPositionRelative();
}

//============================================================
//	チーム名オフセットの設定処理
//============================================================
void CGauge2D::SetOffsetTeamName(const MyLib::Vector3& rOffset)
{
	// 引数のアシストオフセットを代入
	m_offsetName = rOffset;

	// 相対位置を設定
	SetPositionRelative();
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
//	横配置の設定処理
//============================================================
void CGauge2D::SetAlignX(const EAlignX alignX)
{
	// 横配置の設定
	m_pTeamName->SetAlignX(alignX);
}

//============================================================
//	文字列の設定処理
//============================================================
void CGauge2D::SetString(const std::string& rStr)
{
	// 文字列の設定
	m_pTeamName->SetString(rStr);
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
//	デバッグ
//============================================================
void CGauge2D::Debug()
{
	if (ImGui::TreeNode("Gauge2D"))
	{
		ImGui::DragFloat("m_fSizeFrame", &m_fSizeFrame, 0.01f, 0.0f, 0.0f, "%.2f");

		SetSizeFrame(m_pFrame->GetSizeOrigin() * m_fSizeFrame);

		MyLib::Vector3 pos = MyLib::Vector3(30.0f, 670.0f, 0.0f);
		if (m_team == CGameManager::ETeamSide::SIDE_RIGHT) pos.x = 1250.0f;

		pos.x *= m_fSizeFrame;
		m_pFrame->SetPosition(pos);

		ImGui::TreePop();
	}
}

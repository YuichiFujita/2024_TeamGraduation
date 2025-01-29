//=============================================================================
// 
//  チームステータス処理 [teamStatus.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "teamStatus.h"
#include "player.h"
#include "playerStatus.h"
#include "gauge2D.h"
#include "gameEndManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace Charm
{
	const float VALUE_MAX = 100.0f;	// 最大値
}

namespace Special
{
	const float VALUE_MAX = 100.0f;			// 最大値
	const float VALUE_FRAMERATE = 100.0f;	// 値の伸びる速度
	const MyLib::Vector2 GAUGE_SIZE = MyLib::Vector2(250.0f, 14.0f);		// ゲージサイズ
	const MyLib::Vector3 GAUGE_POS = MyLib::Vector3(-2.0f, 34.0f, 0.0f);	// ゲージ位置
	const float FRAME_WIDTH = GAUGE_SIZE.x + 13.0f;	// フレーム横幅
	const float ASSIST_WIDTH = 48.0f;				// アシスト横幅
}

//==========================================================================
// コンストラクタ
//==========================================================================
CTeamStatus::CTeamStatus() :
	m_typeTeam(CGameManager::SIDE_NONE),	// チームサイド
	m_sCharmInfo(SCharmInfo()),				// モテ情報
	m_sSpecialInfo(SSpecialInfo())			// スペシャル情報
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CTeamStatus::~CTeamStatus()
{

}

//==========================================================================
// 生成
//==========================================================================
CTeamStatus* CTeamStatus::Create()
{
	// メモリの確保
	CTeamStatus* pStatus = DEBUG_NEW CTeamStatus;
	if (pStatus != nullptr)
	{
		// クラスの初期化
		if (FAILED(pStatus->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pStatus);
			return nullptr;
		}
	}

	return pStatus;
}

//==========================================================================
// 初期化
//==========================================================================
HRESULT CTeamStatus::Init()
{
	//モテ情報
	InitCharmInfo();

	//スペシャル情報
	InitSpecialInfo();

	return S_OK;
}

//==========================================================================
// 終了
//==========================================================================
void CTeamStatus::Uninit()
{
	m_sSpecialInfo.pGauge = nullptr;

	delete this;
}

//==========================================================================
// チーム設定による
//==========================================================================
void CTeamStatus::TeamSetting(const CGameManager::ETeamSide team)
{
	CObject2D::AnchorPoint anchor = CObject2D::AnchorPoint::CENTER;
	MyLib::Vector3 pos = MyLib::Vector3();
	MyLib::Vector3 dest = Special::GAUGE_POS;
	std::vector<D3DXVECTOR2> uv =
	{// テクスチャ座標
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f),
	};

	// チームサイド設定
	SetTeam(team);

	switch (team)
	{
	case CGameManager::ETeamSide::SIDE_LEFT:

		anchor = CObject2D::LEFT;
		pos += MyLib::Vector3(0.0f, SCREEN_HEIGHT, 0.0f);	// 右下
		pos += MyLib::Vector3(dest.x, -dest.y, 0.0f);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		anchor = CObject2D::RIGHT;
		pos += MyLib::Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);	// 左下
		pos += MyLib::Vector3(-dest.x, -dest.y, 0.0f);

		// テクスチャ反転
		UtilFunc::Calculation::SwapValue(uv[0], uv[1]);
		UtilFunc::Calculation::SwapValue(uv[2], uv[3]);
		break;

	default:
		break;
	}

	m_sSpecialInfo.pGauge->SetTeam(team);
	m_sSpecialInfo.pGauge->SetAnchorType(anchor);
	m_sSpecialInfo.pGauge->SetPosition(pos);
	m_sSpecialInfo.pGauge->SetTexUV(uv);

	const MyLib::Vector3 OFFSET_FRAME[] =
	{
		MyLib::Vector3(-13.0f, -13.0f, 0.0f),
		MyLib::Vector3(12.5f, -13.0f, 0.0f),
	};
	m_sSpecialInfo.pGauge->SetOffsetFrame(OFFSET_FRAME[team]);

	const MyLib::Vector3 OFFSET_ASSIST[] =
	{
		MyLib::Vector3(375.0f, -31.0f, 0.0f),
		MyLib::Vector3(-375.0f, -31.0f, 0.0f),
	};
	m_sSpecialInfo.pGauge->SetOffsetAssist(OFFSET_ASSIST[team]);

	const MyLib::Vector3 OFFSET_NAME[] =
	{
		MyLib::Vector3(12.0f, -32.0f, 0.0f),
		MyLib::Vector3(-12.0f, -32.0f, 0.0f),
	};
	m_sSpecialInfo.pGauge->SetOffsetTeamName(OFFSET_NAME[team]);

	const EAlignX ALIGN_X[] =
	{
		XALIGN_LEFT,
		XALIGN_RIGHT,
	};
	m_sSpecialInfo.pGauge->SetAlignX(ALIGN_X[team]);

	// 外部ファイルから設定されたチーム名を読込
	std::string sTeamName;	// チーム名
	CEntryRuleManager::LoadName(&sTeamName, m_typeTeam);	// チーム名読込
	m_sSpecialInfo.pGauge->SetString(sTeamName);
}

//==========================================================================
// 全滅で終了
//==========================================================================
bool CTeamStatus::CheckAllDead()
{
	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		CGameManager::ETeamSide team = pPlayer->GetTeam();
		CPlayer::EFieldArea area = pPlayer->GetAreaType();
		CPlayer::EState state = pPlayer->GetState();

		// 同じチームが生きている場合 = 全滅していない
		if (team == m_typeTeam &&
			area == CPlayer::EFieldArea::FIELD_IN &&	// 内野
			(state != CPlayer::EState::STATE_DEAD_AFTER &&
			state != CPlayer::EState::STATE_DEAD_CARRY))
		{
			return false;
		}
	}
	return true;
}

//==========================================================================
// モテ情報初期化
//==========================================================================
void CTeamStatus::InitCharmInfo()
{
	m_sCharmInfo.fValueMax = Charm::VALUE_MAX;
}

//==========================================================================
// モテゲージ値増加
//==========================================================================
void CTeamStatus::AddCharmValue(float fValue)
{
	m_sCharmInfo.fValue = UtilFunc::Transformation::Clamp(m_sCharmInfo.fValue + fValue, 0.0f, m_sCharmInfo.fValueMax);
}
//==========================================================================
// モテゲージ値増加
//==========================================================================
void CTeamStatus::SubCharmValue(float fValue)
{
	m_sCharmInfo.fValue = UtilFunc::Transformation::Clamp(m_sCharmInfo.fValue - fValue, 0.0f, m_sCharmInfo.fValueMax);
}

//==========================================================================
// スペシャル情報初期化
//==========================================================================
void CTeamStatus::InitSpecialInfo()
{
	if (m_sSpecialInfo.pGauge != nullptr)
	{
		m_sSpecialInfo.pGauge->Uninit();
		m_sSpecialInfo.pGauge = nullptr;
	}

	// ゲージの生成
	CTexture* pTexture = CTexture::GetInstance();
	m_sSpecialInfo.pGauge = CGauge2D::Create
	( // 引数
		Special::VALUE_MAX,		// 最大表示値
		10.1f,					// 表示値変動フレーム
		"",						// チーム名
		Special::GAUGE_POS,		// 位置
		VEC3_ZERO,				// フレームオフセット
		VEC3_ZERO,				// アシストオフセット
		VEC3_ZERO,				// チーム名オフセット
		Special::GAUGE_SIZE,	// ゲージ大きさ
		VEC2_ZERO,				// フレーム大きさ
		VEC2_ZERO,				// アシスト大きさ
		MyLib::color::Blue()	// ゲージ色
	);

	// フレームテクスチャ割当
	const int nTexFrameID = pTexture->Regist("data\\TEXTURE\\gaugeframe000.png");
	m_sSpecialInfo.pGauge->BindTextureFrame(nTexFrameID);

	// フレーム横幅を元にサイズを設定
	MyLib::Vector2 sizeFrame = pTexture->GetImageSize(nTexFrameID);
	sizeFrame = UtilFunc::Transformation::AdjustSizeByWidth(sizeFrame, Special::FRAME_WIDTH);
	m_sSpecialInfo.pGauge->SetSizeFrame(sizeFrame);

	// アシストテクスチャ割当
	const int nTexAssistID = pTexture->Regist("data\\TEXTURE\\xbox_lb.png");
	m_sSpecialInfo.pGauge->BindTextureAssist(nTexAssistID);

	// アシスト横幅を元にサイズを設定
	MyLib::Vector2 sizeAssist = pTexture->GetImageSize(nTexAssistID);
	sizeAssist = UtilFunc::Transformation::AdjustSizeByWidth(sizeAssist, Special::ASSIST_WIDTH);
	m_sSpecialInfo.pGauge->SetSizeAssist(sizeAssist);

	// 上限設定
	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;

	// 値の初期化
	ZeroSpecialValue();
}

//==========================================================================
// スペシャルゲージ増加
//==========================================================================
void CTeamStatus::SetSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue =	UtilFunc::Transformation::Clamp(fValue, 0.0f, m_sSpecialInfo.fValueMax);

	if (m_sSpecialInfo.pGauge != nullptr)
	{// ゲージに値設定
		m_sSpecialInfo.pGauge->SetNum(m_sSpecialInfo.fValue);
	}

	if (m_sSpecialInfo.pGauge != nullptr)
	{
		MyLib::Vector2 size = Special::GAUGE_SIZE;
		float fRad = m_sSpecialInfo.fValue / m_sSpecialInfo.fValueMax;	// TODO：float０割り

		// サイズ設定
		m_sSpecialInfo.pGauge->SetSizeGaugeRadius(fRad);
	}
}

//==========================================================================
// スペシャルゲージ増加
//==========================================================================
void CTeamStatus::AddSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue += fValue;
	SetSpecialValue(m_sSpecialInfo.fValue);
}

//==========================================================================
// スペシャルゲージ減少
//==========================================================================
void CTeamStatus::SubSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue -= fValue;
	SetSpecialValue(m_sSpecialInfo.fValue);
}

//==========================================================================
// ゲージ描画フラグ設定
//==========================================================================
void CTeamStatus::SetEnableGaugeDisp(const bool bDisp)
{
	m_sSpecialInfo.pGauge->SetEnableDisp(bDisp);
}

//==========================================================================
// デバッグ
//==========================================================================
void CTeamStatus::Debug()
{

}

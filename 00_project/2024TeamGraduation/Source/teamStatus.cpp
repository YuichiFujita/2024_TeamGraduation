//=============================================================================
// 
//  チームステータス処理 [teamStatus.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "teamStatus.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace Charm
{
	const float VALUE_MAX = 100.0f;	// 最大値
}

namespace Special
{
	const float VALUE_MAX = 100.0f;										// 最大値
	const MyLib::Vector2 GAUGE_SIZE = MyLib::Vector2(50.0f, 200.0f);	// ゲージサイズ
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
	return nullptr;
}

//==========================================================================
// 初期化
//==========================================================================
void CTeamStatus::Init()
{
	//モテ情報
	InitCharmInfo();

	//スペシャル情報
	InitSpecialInfo();
}

//==========================================================================
// 終了
//==========================================================================
void CTeamStatus::Uninit()
{
	if (m_sSpecialInfo.pGauge != nullptr)
	{
		m_sSpecialInfo.pGauge->Uninit();
		m_sSpecialInfo.pGauge = nullptr;
	}
}

//==========================================================================
// スペシャル情報初期化
//==========================================================================
void CTeamStatus::InitCharmInfo()
{
	m_sCharmInfo.fValueMax = Charm::VALUE_MAX;
}

//==========================================================================
// モテ情報初期化
//==========================================================================
void CTeamStatus::InitSpecialInfo()
{
	if (m_sSpecialInfo.pGauge != nullptr)
	{
		m_sSpecialInfo.pGauge->Uninit();
		m_sSpecialInfo.pGauge = nullptr;
	}

	m_sSpecialInfo.pGauge = CObject2D::Create();
	m_sSpecialInfo.pGauge->SetAnchorType(CObject2D::LEFT);
	m_sSpecialInfo.pGauge->SetSize(Special::GAUGE_SIZE);
	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;

}

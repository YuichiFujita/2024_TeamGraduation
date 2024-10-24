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
	const float VALUE_MAX = 100.0f;												// 最大値
	const MyLib::Vector2 GAUGE_SIZE = MyLib::Vector2(250.0f, 30.0f);			// ゲージサイズ
	const MyLib::Vector3 GAUGE_POS = MyLib::Vector3(30.0f, 50.0f, 0.0f);		// ゲージ位置
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
}

//==========================================================================
// チーム設定による
//==========================================================================
void CTeamStatus::TeamSetting(const CGameManager::TeamSide team)
{
	CObject2D::AnchorPoint anchor = CObject2D::AnchorPoint::CENTER;
	MyLib::Vector3 pos = MyLib::Vector3();
	MyLib::Vector3 dest = Special::GAUGE_POS;

	// チームサイド設定
	SetTeam(team);

	switch (team)
	{
	case CGameManager::SIDE_LEFT:

		anchor = CObject2D::RIGHT;
		pos += MyLib::Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);	// 左下
		pos += MyLib::Vector3(-dest.x, -dest.y, 0.0f);
		break;

	case CGameManager::SIDE_RIGHT:

		anchor = CObject2D::LEFT;
		pos += MyLib::Vector3(0.0f, SCREEN_HEIGHT, 0.0f);	// 右下
		pos += MyLib::Vector3(dest.x, -dest.y, 0.0f);
		break;

	default:
		break;
	}

	m_sSpecialInfo.pGauge->SetAnchorType(anchor);
	m_sSpecialInfo.pGauge->SetPosition(pos);
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
	m_sCharmInfo.fValue += fValue;

	UtilFunc::Transformation::Clamp(m_sCharmInfo.fValue, 0.0f, m_sCharmInfo.fValueMax);
}
//==========================================================================
// モテゲージ値増加
//==========================================================================
void CTeamStatus::SubCharmValue(float fValue)
{
	m_sCharmInfo.fValue -= fValue;

	UtilFunc::Transformation::Clamp(m_sCharmInfo.fValue, 0.0f, m_sCharmInfo.fValueMax);
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

	//ゲージ生成
	m_sSpecialInfo.pGauge = CObject2D::Create();
	m_sSpecialInfo.pGauge->SetSize(Special::GAUGE_SIZE);

	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;
}

//==========================================================================
// スペシャルゲージ増加
//==========================================================================
void CTeamStatus::AddSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue += fValue;

	UtilFunc::Transformation::Clamp(m_sSpecialInfo.fValue, 0.0f, m_sSpecialInfo.fValueMax);

	if (m_sSpecialInfo.pGauge != nullptr)
	{
		MyLib::Vector2 size = Special::GAUGE_SIZE;
		float fRad = m_sSpecialInfo.fValue / m_sSpecialInfo.fValueMax;
		
		size.x *= fRad;

		m_sSpecialInfo.pGauge->SetSize(size);
	}
}

//==========================================================================
// スペシャルゲージ減少
//==========================================================================
void CTeamStatus::SubSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue += fValue;

	UtilFunc::Transformation::Clamp(m_sSpecialInfo.fValue, 0.0f, m_sSpecialInfo.fValueMax);

	if (m_sSpecialInfo.pGauge != nullptr)
	{
		MyLib::Vector2 size = Special::GAUGE_SIZE;
		float fRad = m_sSpecialInfo.fValue / m_sSpecialInfo.fValueMax;
		
		size.x *= fRad;

		m_sSpecialInfo.pGauge->SetSize(size);
	}
}

//==========================================================================
// デバッグ
//==========================================================================
void CTeamStatus::Debug()
{
	//-----------------------------
// コート
//-----------------------------
	if (ImGui::TreeNode("Special"))
	{
		ImGui::DragFloat("GaugeSize", (float*)&m_sSpecialInfo.fValue, 1.0f, 0.0f, m_sSpecialInfo.fValueMax, "%.2f");

		// 位置設定
		ImGui::TreePop();
	
		if (m_sSpecialInfo.pGauge != nullptr)
		{
			MyLib::Vector2 size = Special::GAUGE_SIZE;
			float fRad = m_sSpecialInfo.fValue / m_sSpecialInfo.fValueMax;
		
			size.x *= fRad;

			m_sSpecialInfo.pGauge->SetSize(size);
		}
	}
}

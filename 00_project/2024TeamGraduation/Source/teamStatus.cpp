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
	const float VALUE_FRAMERATE = 100.0f;										// 値の伸びる速度
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
	//std::vector<D3DXVECTOR2> uv =
	//{// テクスチャ座標
	//	uv[0] = D3DXVECTOR2(0.0f, 0.0f),
	//	uv[1] = D3DXVECTOR2(1.0f, 0.0f),
	//	uv[2] = D3DXVECTOR2(0.0f, 1.0f),
	//	uv[3] = D3DXVECTOR2(1.0f, 1.0f),
	//};

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
		break;

	default:
		break;
	}

	m_sSpecialInfo.pGauge->SetAnchorType(anchor);
	m_sSpecialInfo.pGauge->SetPosition(pos);
	//m_sSpecialInfo.pGauge->SetTexUV(uv);
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

		// 同じチームが生きている場合 = 全滅していない
		if (pPlayer->GetTeam() == m_typeTeam &&
			pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN &&	// 内野
			pPlayer->GetState() != CPlayer::EState::STATE_DEAD_AFTER)
		{
			return false;
		}
	}

	// 試合終了
	CGameManager::GetInstance()->SetSceneType(CGameManager::ESceneType::SCENE_END);
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

	//ゲージ生成
	m_sSpecialInfo.pGauge = CGauge2D::Create(Special::VALUE_MAX, 0.1f, Special::GAUGE_POS, Special::GAUGE_SIZE);
	m_sSpecialInfo.pGauge->SetColorFront(MyLib::color::White());
	m_sSpecialInfo.pGauge->SetColorBack(MyLib::color::Black());

	// 値の初期化
	ZeroSpecialValue();

	// 上限設定
	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;
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
		float fRad = m_sSpecialInfo.fValue / m_sSpecialInfo.fValueMax;

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
// デバッグ
//==========================================================================
void CTeamStatus::Debug()
{
	if (ImGui::TreeNode("TeamStatus"))
	{
		if (m_sSpecialInfo.pGauge != nullptr)
		{// ゲージのmax時間

			D3DXCOLOR col = m_sSpecialInfo.pGauge->GetBar()->GetColor();

			ImGui::Text("BrightTime: [%.2f]", m_sSpecialInfo.pGauge->GetBrightTime());
			ImGui::Text("BrightTime: [r: %.2f] [g: %.2f] [b: %.2f] [a: %.2f]", col.r, col.g, col.b, col.a);
		}

		ImGui::TreePop();
	}
}

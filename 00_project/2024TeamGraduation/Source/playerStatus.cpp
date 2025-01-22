//=============================================================================
// 
//  プレイヤーステータス処理 [playerStatus.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerStatus.h"
#include "player.h"
#include "object_circlegauge2D.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int MIN_LIFE = 0;	// 最小体力
}

namespace LifeGauge
{
	const int DIVISION = 32;	// 分割数
	const float SIZE = 40.0f;	// サイズ
	const float DISTANCE = 100.0f;	// 間隔
	const MyLib::Vector3 DEFAULTPOS[CGameManager::ETeamSide::SIDE_MAX] =
	{
		MyLib::Vector3(60.0f, 80.0f, 0.0f),	// 左サイド
		MyLib::Vector3(SCREEN_WIDTH - 60.0f, 80.0f, 0.0f),	// 右サイド
	};
}

//==========================================================================
// 静的メンバ変数定義
//==========================================================================
CListManager<CObjectCircleGauge2D> CPlayerStatus::m_LifeGaugeListLeft = {};		// 左体力ゲージのリスト
CListManager<CObjectCircleGauge2D> CPlayerStatus::m_LifeGaugeListRight = {};	// 右体力ゲージのリスト

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerStatus::CPlayerStatus(CPlayer* pPlayer) :
	m_pPlayer		(pPlayer),	// プレイヤーのポインタ
	m_pLifeGauge	(nullptr)	// 体力ゲージ
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerStatus::~CPlayerStatus()
{
	m_LifeGaugeListLeft.Delete(m_pLifeGauge);
	m_LifeGaugeListRight.Delete(m_pLifeGauge);
}

//==========================================================================
// 初期化処理
//==========================================================================
void CPlayerStatus::Init()
{
	// 体力ゲージ生成
	if (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
	{// 内野のみ生成
		CreateLifeGuge();
	}
}

//==========================================================================
// 体力ゲージ生成
//==========================================================================
void CPlayerStatus::CreateLifeGuge()
{
	// 体力ゲージ生成
	m_pLifeGauge = CObjectCircleGauge2D::Create(LifeGauge::DIVISION, LifeGauge::SIZE);
	m_pLifeGauge->SetType(CObject::TYPE::TYPE_UI);

	// チーム取得
	CGameManager::ETeamSide teamSide = m_pPlayer->GetTeam();

	// リスト追加・位置設定
	int nNum = 0;
	switch (teamSide)
	{
	case CGameManager::ETeamSide::SIDE_NONE:
		return;

	case CGameManager::ETeamSide::SIDE_LEFT:

		// 既にある分考慮してずらす
		nNum = m_LifeGaugeListLeft.GetNumAll();

		// 位置設定
		m_pLifeGauge->SetPosition(LifeGauge::DEFAULTPOS[teamSide] + MyLib::Vector3(nNum * LifeGauge::DISTANCE, 0.0f, 0.0f));

		// リスト追加
		m_LifeGaugeListLeft.Regist(m_pLifeGauge);

		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// 既にある分考慮してずらす
		nNum = m_LifeGaugeListRight.GetNumAll();

		// 位置設定
		m_pLifeGauge->SetPosition(LifeGauge::DEFAULTPOS[teamSide] - MyLib::Vector3(nNum * LifeGauge::DISTANCE, 0.0f, 0.0f));

		// リスト追加
		m_LifeGaugeListRight.Regist(m_pLifeGauge);

		break;

	default:
		assert(false);
		break;
	}

	// 色設定
	m_pLifeGauge->SetColor(MyLib::color::Green());


	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist("data\\TEXTURE\\GRADATION\\black_04.jpg");
	m_pLifeGauge->BindTexture(nTexID);

	// ゲージフレーム生成
	CreateGaugeFrame();

}

//==========================================================================
// ゲージフレーム生成
//==========================================================================
void CPlayerStatus::CreateGaugeFrame()
{
	// ゲージフレーム
	m_pGaugeFrame = CObject2D::Create(m_pLifeGauge->GetPriority());

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist("data\\TEXTURE\\player\\gauge.png");
	m_pGaugeFrame->BindTexture(nTexID);

	// 位置設定
	m_pGaugeFrame->SetPosition(m_pLifeGauge->GetPosition());

	// サイズ設定
	m_pGaugeFrame->SetSize(MyLib::Vector2(LifeGauge::SIZE * 1.25f));
}

//==========================================================================
// 削除処理
//==========================================================================
void CPlayerStatus::Kill()
{
	// 削除
	if (m_pLifeGauge != nullptr)
	{
		m_LifeGaugeListLeft.Delete(m_pLifeGauge);
		m_LifeGaugeListRight.Delete(m_pLifeGauge);
		m_pLifeGauge->Kill();
		m_pLifeGauge = nullptr;
	}
}

//==========================================================================
//	体力減算
//==========================================================================
void CPlayerStatus::LifeDamage(const int nDmg)
{
	// 外野の場合抜ける
	if (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) { return; }

	int nLife = m_pPlayer->GetLife();	// 体力
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// 体力を減算
	nLife -= nDmg;

	// 体力を補正
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// 体力を反映
	m_pPlayer->SetLife(nLife);

	// ゲージの目標値設定
	float ratio = static_cast<float>(nLife) / static_cast<float>(nLifeOrigin);
	m_pLifeGauge->SetRateDest(ratio);
}

//==========================================================================
//	体力加算
//==========================================================================
void CPlayerStatus::LifeHeal(const int nHeal)
{
	// 外野の場合抜ける
	if (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) { return; }

	int nLife = m_pPlayer->GetLife();	// 体力
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// 体力を加算
	nLife += nHeal;

	// 体力を補正
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// 体力を反映
	m_pPlayer->SetLife(nLife);

	// ゲージの目標値設定
	float ratio = static_cast<float>(nLife) / static_cast<float>(nLifeOrigin);
	m_pLifeGauge->SetRateDest(ratio);
}

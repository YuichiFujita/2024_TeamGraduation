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
}

//==========================================================================
// 静的メンバ変数定義
//==========================================================================
//CListManager<CObjectCircleGauge2D> CPlayerStatus::m_LifeGaugeList = {};	// 体力ゲージのリスト

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

}

//==========================================================================
// 初期化処理
//==========================================================================
void CPlayerStatus::Init()
{
	// 体力ゲージ生成
	m_pLifeGauge = CObjectCircleGauge2D::Create(LifeGauge::DIVISION, LifeGauge::SIZE);
}

//==========================================================================
// 削除処理
//==========================================================================
void CPlayerStatus::Kill()
{
	// 削除
	if (m_pLifeGauge != nullptr)
	{
		m_pLifeGauge->Kill();
		m_pLifeGauge = nullptr;
	}
}

//==========================================================================
//	体力減算
//==========================================================================
void CPlayerStatus::LifeDamage(const int nDmg)
{
	int nLife = m_pPlayer->GetLife();	// 体力
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// 体力を減算
	nLife -= nDmg;

	// 体力を補正
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// 体力を反映
	m_pPlayer->SetLife(nLife);

	// ゲージの目標値設定
	m_pLifeGauge->SetRateDest(nLife / nLifeOrigin);
}

//==========================================================================
//	体力加算
//==========================================================================
void CPlayerStatus::LifeHeal(const int nHeal)
{
	int nLife = m_pPlayer->GetLife();	// 体力
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// 体力を加算
	nLife += nHeal;

	// 体力を補正
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// 体力を反映
	m_pPlayer->SetLife(nLife);

	// ゲージの目標値設定
	m_pLifeGauge->SetRateDest(nLife / nLifeOrigin);
}

//=============================================================================
// 
//  プレイヤーステータス処理 [playerStatus.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerStatus.h"
#include "player.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int MIN_LIFE = 0;	// 最小体力
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerStatus::CPlayerStatus(CPlayer* pPlayer) :
	m_pPlayer	(pPlayer),					// プレイヤーのポインタ
	m_typeTeam	(CGameManager::SIDE_NONE)	// チームサイド
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerStatus::~CPlayerStatus()
{

}

//==========================================================================
//	体力減算
//==========================================================================
void CPlayerStatus::LifeDamage(const int nDmg)
{
	int nLife = m_pPlayer->GetLife();	// 体力

	// 体力を減算
	nLife -= nDmg;

	// 体力を補正
	UtilFunc::Transformation::ValueNormalize(nLife, m_pPlayer->GetLifeOrigin(), MIN_LIFE);

	// 体力を反映
	m_pPlayer->SetLife(nLife);
}

//==========================================================================
//	体力加算
//==========================================================================
void CPlayerStatus::LifeHeal(const int nHeal)
{
	int nLife = m_pPlayer->GetLife();	// 体力

	// 体力を加算
	nLife += nHeal;

	// 体力を補正
	UtilFunc::Transformation::ValueNormalize(nLife, m_pPlayer->GetLifeOrigin(), MIN_LIFE);

	// 体力を反映
	m_pPlayer->SetLife(nLife);
}

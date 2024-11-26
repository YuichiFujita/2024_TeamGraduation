//==========================================================================
// 
//  指定なしプレイヤー処理 [playerNone.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================
#include "playerNone.h"
#include "player.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerNone::CPlayerNone(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_USER)
{
	ChangeMoveControl(nullptr);
	ChangeActionControl(nullptr);
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerNone::~CPlayerNone()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerNone::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 
//  �w��Ȃ��v���C���[���� [playerNone.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================
#include "playerNone.h"
#include "player.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerNone::CPlayerNone(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_USER)
{
	ChangeMoveControl(nullptr);
	ChangeActionControl(nullptr);
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerNone::~CPlayerNone()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerNone::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 
//  ����v���C���[���� [playerIn.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerIn.h"
#include "playerStatus.h"

// �g�p�N���X
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerIn::CPlayerIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerIn::~CPlayerIn()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �v���C���[�̎擾
	CPlayer* pPlayer = GetPlayer();

	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����̍X�V
	pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerIn::Hit(CBall* pBall)
{
	// ���N���X�̃q�b�g
	CPlayer::SHitInfo hitInfo = CPlayerBase::Hit(pBall);	// �q�b�g���

	// �_���[�W���󂯂Ă��Ȃ��ꍇ�͔�����
	if (!hitInfo.bHit) { return hitInfo; }

	CGameManager::ETeamSide sideBall = pBall->GetTypeTeam();	// �{�[���`�[���T�C�h
	CPlayer* pPlayer = GetPlayer();								// �v���C���[���
	CPlayerStatus* pStatus = pPlayer->GetStatus();				// �X�e�[�^�X���
	CGameManager* pGameManager = CGameManager::GetInstance();	// �Q�[���}�l�[�W���[���
	MyLib::HitResult_Character hitresult = {};					// �Փˏ��

	// ���G��ԂȂ甲����
	if (!pPlayer->GetDamageInfo().bReceived) { return hitInfo; }

	// �_���[�W��^����
	pStatus->LifeDamage(pBall->GetDamage());

	if (pPlayer->GetLife() <= 0)
	{
		// �I��
		pPlayer->DeadSetting(&hitresult, pBall);
	}
	else
	{
		// �_���[�W��Ԃɂ���
		pPlayer->DamageSetting(pBall);
	}

	// ���e���Z(�{�[����������)
	pGameManager->AddCharmValue(sideBall, CCharmManager::ETypeAdd::ADD_HIT);

	// �X�y�V�������Z
	pGameManager->AddSpecialValue(sideBall, CSpecialValueManager::ETypeAdd::ADD_HIT);

	return hitInfo;
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerIn::ChangeMoveControl(CPlayerControlMove* control)
{
	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	// ����N���X�̓���
	delete pControlMove;
	pControlMove = control;

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerIn::ChangeActionControl(CPlayerControlAction* control)
{
	// ����N���X�̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����N���X�̓���
	delete pControlAction;
	pControlAction = control;

	// ����N���X�̔��f
	SetPlayerControlAction(pControlAction);
}

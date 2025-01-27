//==========================================================================
// 
//  ����v���C���[���� [playerIn.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerIn.h"
#include "playerStatus.h"
#include "EffekseerObj.h"

// �g�p�N���X
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerIn::CPlayerIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase)
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

	if (!pPlayer->IsDeathState())
	{ // ����ł��Ȃ��ꍇ

		// ����̍X�V
		pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
		pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	}

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
	pStatus->LifeDamage(pBall->GetDamage());	// TODO�F�̗͏���

	// �q�b�g�G�t�F�N�g����
	switch (pBall->GetTypeAtk())
	{
	case CBall::ATK_NORMAL:
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_HIT,
			pBall->GetPosition(),
			MyLib::Vector3(0.0f, pBall->GetMove().AngleXZ(0.0f), 0.0f),
			MyLib::Vector3(),
			12.0f, true);
		break;

	case CBall::ATK_JUMP:
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_HIT,
			pBall->GetPosition(),
			MyLib::Vector3(0.0f, pBall->GetMove().AngleXZ(0.0f), 0.0f),
			MyLib::Vector3(),
			20.0f, true);
		break;

	case CBall::ATK_SPECIAL:
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_HIT,
			pBall->GetPosition(),
			MyLib::Vector3(0.0f, pBall->GetMove().AngleXZ(0.0f), 0.0f),
			MyLib::Vector3(),
			20.0f, true);
		break;

	default:
		break;
	}

	// ��
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_SWEAT,
		pPlayer->GetPosition() + MyLib::Vector3(0.0f, 100.0f, 0.0f),
		MyLib::Vector3(0.0f, -pBall->GetMove().AngleXZ(0.0f), 0.0f),
		MyLib::Vector3(),
		12.0f, true);

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
	pGameManager->AddCharmValue(sideBall, CCharmValueManager::ETypeAdd::ADD_HIT);

	// �X�y�V�������Z
	pGameManager->AddSpecialValue(sideBall, CSpecialValueManager::ETypeAdd::ADD_HIT);

	return hitInfo;
}

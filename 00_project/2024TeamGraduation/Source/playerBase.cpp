//==========================================================================
// 
//  �v���C���[�x�[�X���� [playerBase.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerBase.h"
#include "playerStatus.h"
#include "ball.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerBase::CPlayerBase(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) :
	m_pPlayer		 (pPlayer),	// �v���C���[���
	m_pControlMove	 (nullptr),	// �ړ�����
	m_pControlAction (nullptr)	// �A�N�V��������
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerBase::~CPlayerBase()
{
	// ����̔j��
	DeleteControl();
}

//==========================================================================
// ����̔j��
//==========================================================================
void CPlayerBase::DeleteControl()
{
	// �ړ�����̔j��
	SAFE_DELETE(m_pControlMove);

	// �A�N�V��������̔j��
	SAFE_DELETE(m_pControlAction);
}

//==========================================================================
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerBase::Hit(CBall* pBall)
{
	CGameManager::TeamSide sideBall = pBall->GetTypeTeam();	// �{�[���`�[���T�C�h
	CBall::EAttack atkBall	= pBall->GetTypeAtk();			// �{�[���U�����
	CBall::EState stateBall = pBall->GetState();			// �{�[�����
	MyLib::Vector3 posB = pBall->GetPosition();				// �{�[���ʒu
	MyLib::HitResult_Character hitresult = {};				// �Փˏ��
	CPlayerStatus* pStatus = m_pPlayer->GetStatus();		// �X�e�[�^�X���

	// �q�b�g���̏�����
	CPlayer::SHitInfo hitInfo;
	hitInfo.eHit = CPlayer::HIT_NONE;
	hitInfo.bHit = false;

	if (m_pPlayer->GetMotionFrag().bDead)
	{ // ���S��ԂȂ炷�蔲��

		return hitInfo;
	}

	if (stateBall == CBall::STATE_LAND
	||  stateBall == CBall::STATE_FREE && pBall->GetTypeTeam() != pStatus->GetTeam())
	{ // �{�[�������n���Ă���A�܂��̓t���[�{�[���������̃`�[���{�[���ł͂Ȃ��ꍇ

		// �{�[�����L���b�`
		pBall->CatchLand(m_pPlayer);

		if (pBall->IsLanding())
		{ // �{�[�������n���Ă���ꍇ

			// �����Ă�̃L���b�`
			m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_DROPCATCH_WALK);
		}

		// �L���b�`���
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	// �_���[�W���󂯕t���Ȃ��Ȃ炷�蔲����
	if (!m_pPlayer->GetDamageInfo().bReceived) { return hitInfo; }

	// ���o�E���h�{�[���̏ꍇ�L���b�`����
	if (stateBall == CBall::STATE_REBOUND)
	{
		// �L���b�`���
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;

		// �J�o�[�L���b�`���̐ݒ�
		m_pPlayer->CoverCatchSetting(pBall);

		return hitInfo;
	}

	// �U����ԈȊO�Ȃ炷�蔲����
	if (atkBall == CBall::EAttack::ATK_NONE) { return hitInfo; }
	
	// �����̃{�[���Ȃ炷�蔲����
	if (pStatus->GetTeam() == sideBall) { return hitInfo; }

	if (m_pPlayer->GetMotionFrag().bCatch
	&&  UtilFunc::Collision::CollisionViewRange3D(m_pPlayer->GetPosition(), posB, m_pPlayer->GetRotation().y, 160))	// TODO�F160�̓{�[���X�e�[�^�X�ɕύX
	{ // �L���b�`�A�N�V���������������ł��󂯕t�����̏ꍇ

		// �L���b�`������
		m_pPlayer->CatchSetting(pBall);

		// �L���b�`���
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;

		return hitInfo;
	}

	// �_���[�W���󂯂�ꍇ�̓t���O��ON�ɂ���
	hitInfo.bHit = true;

	// �_���[�W��^����
	//pStatus->LifeDamage(pBall->GetDamage());	// TODO : �ォ��Ball���̍U�����o���X�g���e�W�[�ɂ��āAGetDamage���쐬
	pStatus->LifeDamage(10);

	if (m_pPlayer->GetLife() <= 0)
	{
		// �I��
		m_pPlayer->DeadSetting(&hitresult, pBall);
	}
	else
	{
		// �_���[�W��Ԃɂ���
		m_pPlayer->DamageSetting(pBall);
	}

	// �Q�[���}�l�[�W���擾
	CGameManager* pGameMgr = CGameManager::GetInstance();
	if (pGameMgr == nullptr) return hitInfo;

	// ���e���Z(�{�[����������)
	pGameMgr->AddCharmValue(sideBall, CCharmManager::EType::TYPE_HIT);

	return hitInfo;
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CPlayerBase::Debug()
{

}

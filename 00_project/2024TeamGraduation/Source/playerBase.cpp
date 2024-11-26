//==========================================================================
// 
//  �v���C���[�x�[�X���� [playerBase.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerBase.h"
#include "playerStatus.h"
#include "playercontrol_action.h"
#include "playercontrol_move.h"
#include "playerAction.h"
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
CPlayerBase::CPlayerBase(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) :
	m_curTypeBase	 (typeBase),	// ���݂̃x�[�X�^�C�v
	m_newTypeBase	 (typeBase),	// �V�����x�[�X�^�C�v
	m_pPlayer		 (pPlayer),		// �v���C���[���
	m_pControlMove	 (nullptr),		// �ړ�����
	m_pControlAction (nullptr)		// �A�N�V��������
{
	// �ʒu�␳�̊���
	pPlayer->ChangePosAdjuster(typeTeam, typeArea);
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
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerBase::Hit(CBall* pBall)
{
	CGameManager::ETeamSide sideBall = pBall->GetTypeTeam();	// �{�[���`�[���T�C�h
	CBall::EAttack atkBall	= pBall->GetTypeAtk();		// �{�[���U�����
	CBall::EState stateBall = pBall->GetState();		// �{�[�����
	MyLib::Vector3 posB = pBall->GetPosition();			// �{�[���ʒu
	MyLib::HitResult_Character hitresult = {};			// �Փˏ��
	CPlayerStatus* pStatus = m_pPlayer->GetStatus();	// �X�e�[�^�X���

	// �q�b�g���̏�����
	CPlayer::SHitInfo hitInfo;
	hitInfo.eHit = CPlayer::HIT_NONE;
	hitInfo.bHit = false;

	if (m_pPlayer->GetMotionFrag().bDead)
	{ // ���S��ԂȂ炷�蔲��

		return hitInfo;
	}

	if (stateBall == CBall::STATE_LAND																			// �{�[�������n���Ă���
	||  stateBall == CBall::STATE_FREE && pBall->GetTypeTeam() != m_pPlayer->GetTeam()							// �t���[�{�[�������`�[���̃{�[������Ȃ�
	||  pBall->IsPass() && (pBall->GetTarget() == m_pPlayer || pBall->GetTypeTeam() != m_pPlayer->GetTeam()))	// �p�X��Ԋ��^�[�Q�b�g���������g���G�`�[��
	{ // ��L�̏����̏ꍇ

		// �{�[�����L���b�`
		pBall->CatchLand(m_pPlayer);

		if (pBall->IsLanding())
		{ // �{�[�������n���Ă���ꍇ

			// �����Ă�̃L���b�`
			m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_DROPCATCH_WALK);

			// �����̗P�\�ݒ�
			m_pPlayer->GetBase()->GetPlayerControlAction()->SetThrowDrop();
		}

		// �L���b�`���
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	// ���o�E���h�{�[���̏ꍇ�L���b�`����
	if (stateBall == CBall::STATE_REBOUND &&
		m_pPlayer->GetState() != CPlayer::EState::STATE_DMG)
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
	if (m_pPlayer->GetTeam() == sideBall) { return hitInfo; }

	// �_���[�W���󂯕t���Ȃ��Ȃ炷�蔲����
	if (!m_pPlayer->GetDamageInfo().bReceived)
	{
		hitInfo.bHit = true;
		return hitInfo;
	}

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

	if (m_pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN)
	{// �R�[�g����߂��Ă���Ƃ�
			
		// ���e����
		CGameManager* pGameMgr = CGameManager::GetInstance();
		pGameMgr->SubCharmValue(m_pPlayer->GetTeam(), CCharmValueManager::ETypeSub::SUB_INVADE_RUN);
	}

	return hitInfo;
}

//==========================================================================
// �ʒu�̏�����
//==========================================================================
void CPlayerBase::InitPosition(const MyLib::Vector3& rPos)
{
	// �v���C���[�ʒu�̐ݒ�
	m_pPlayer->SetPosition(rPos);
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CPlayerBase::Debug()
{

}

//==========================================================================
// �J�j��������
//==========================================================================
bool CPlayerBase::IsCrab()
{
	// �{�[���������Ă��Ȃ���
	if (m_pPlayer->GetBall() != nullptr) { return false; }

	// ���E�Ƀ{�[�������邩
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// �{�[�����
	if (pBall == nullptr) { return false; }

	// �{�[���̏�ԁF�G���ł��邩
	if (pBall->GetTypeTeam() == m_pPlayer->GetTeam())	{ return false; }

	// �G�`�[���ōU�����肪����ꍇ�̓J�j����
	if (pBall->IsAttack()) { return true; }

	if (pBall->GetTypeTeam() == CGameManager::ETeamSide::SIDE_NONE)	{ return false; }

	// �t���[�{�[���̓J�j�������Ȃ�
	if (pBall->GetState() == CBall::EState::STATE_FREE) { return false; }

	// ���g�̏�ԁF�u�����N������łȂ�
	CPlayer::EAction action = m_pPlayer->GetActionPattern()->GetAction();	// �A�N�V�����p�^�[��
	if (action == CPlayer::EAction::ACTION_BLINK) { return false; }
	if (m_pPlayer->IsDash()) { return false; }

	return true;
}

//==========================================================================
//	�x�[�X�ύX�̍X�V
//==========================================================================
void CPlayerBase::UpdateChangeBase()
{
	if (m_curTypeBase != m_newTypeBase)
	{ // ���݂̃x�[�X�^�C�v����ύX���������ꍇ

		// �x�[�X�̕ύX
		m_pPlayer->ChangeBase(m_newTypeBase);
	}
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
// ����̐ݒ� (�ړ�)
//==========================================================================
void CPlayerBase::SetPlayerControlMove(CPlayerControlMove* pControlMove)
{
	// ����N���X�̐ݒ�
	m_pControlMove = pControlMove;
}

//==========================================================================
// ����̐ݒ� (�A�N�V����)
//==========================================================================
void CPlayerBase::SetPlayerControlAction(CPlayerControlAction* pControlAction)
{
	// ����N���X�̐ݒ�
	m_pControlAction = pControlAction;
}

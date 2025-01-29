//==========================================================================
// 
//  �O��v���C���[���� [playerOut.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerOut.h"

// �g�p�N���X
#include "playercontrol_move.h"
#include "playercontrol_action.h"
#include "playerAction.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerOut::CPlayerOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase),
	m_posLeft	(VEC3_ZERO),	// �ړ��\���ʒu
	m_posRight	(VEC3_ZERO),	// �ړ��\�E�ʒu
	m_bClab(false)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerOut::~CPlayerOut()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerOut::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
CPlayer::SHitInfo CPlayerOut::Hit(CBall* pBall)
{
	CGameManager::ETeamSide sideBall = pBall->GetTypeTeam();	// �{�[���`�[���T�C�h
	CBall::EAttack atkBall	= pBall->GetTypeAtk();				// �{�[���U�����
	CBall::EState stateBall = pBall->GetState();				// �{�[�����
	MyLib::Vector3 posBall = pBall->GetPosition();				// �{�[���ʒu
	MyLib::HitResult_Character hitresult = {};					// �Փˏ��
	CPlayer* pPlayer = GetPlayer();								// �v���C���[���
	CPlayerStatus* pStatus = pPlayer->GetStatus();				// �X�e�[�^�X���
	CPlayer::EState state = pPlayer->GetState();				// �v���C���[���
	CPlayer::EAction action = pPlayer->GetActionPattern()->GetAction();	// �v���C���[�s�����
	CBallStatus::SBallParameter ballParameter = pPlayer->GetBallParameter();	// �{�[���X�e�[�^�X
	float fCatchRange = UtilFunc::Transformation::RadianChangeToDegree(ballParameter.fCatchRange);	// �L���b�`�͈�

	// �q�b�g���̏�����
	CPlayer::SHitInfo hitInfo;
	hitInfo.eHit = CPlayer::HIT_NONE;
	hitInfo.bHit = false;

	if (IsAutoBallCatch(pBall))
	{ // �����{�[���L���b�`���\�ȏꍇ

		// �{�[�����L���b�`
		pBall->CatchLand(pPlayer);

		if (pBall->IsLanding())
		{ // �{�[�������n���Ă���ꍇ

			// �����Ă�̃L���b�`
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_DROPCATCH_WALK);

			// �����̗P�\�ݒ�
			pPlayer->GetBase()->GetPlayerControlAction()->SetThrowDrop();
		}

		// �L���b�`���
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	// �U����ԈȊO�Ȃ炷�蔲����
	if (atkBall == CBall::EAttack::ATK_NONE) { return hitInfo; }

	if (pPlayer->GetMotionFrag().bCatch
	&&  UtilFunc::Collision::CollisionViewRange3D(pPlayer->GetPosition(), posBall, pPlayer->GetRotation().y, fCatchRange))
	{ // �L���b�`�A�N�V���������������ł��󂯕t�����̏ꍇ

		// �L���b�`������
		pPlayer->CatchSetting(pBall);

		// �L���b�`���
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	return hitInfo;
}

//==========================================================================
// �ʒu�̏�����
//==========================================================================
void CPlayerOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// ���E�ʒu�̒������W���v�Z
	MyLib::Vector3 posCenter;	// �������W
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// �v���C���[�ʒu�̐ݒ�
	GetPlayer()->SetPosition(posCenter);
}

//==========================================================================
// �J�j��������
//==========================================================================
bool CPlayerOut::IsCrab()
{
	// �v���C���[���̎擾
	CPlayer* pPlayer = GetPlayer();

	if (!pPlayer || !m_bClab) return false;

	// ���E�Ƀ{�[�������邩
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// �{�[�����
	if (pBall == nullptr) { return false; }

	// ���g�̏�ԁF�u�����N������łȂ�
	CPlayer::EAction action = pPlayer->GetActionPattern()->GetAction();	// �A�N�V�����p�^�[��
	if (action == CPlayer::EAction::ACTION_BLINK) { return false; }
	if (pPlayer->IsDash()) { return false; }

	return true;

}

//==========================================================================
// �J�j�������[�V��������
//==========================================================================
bool CPlayerOut::IsCrabMotion()
{
	// �S���J�j����
	return true;
}

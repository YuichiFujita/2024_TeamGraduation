//==========================================================================
// 
//  �v���C���[�ʒu�␳_����R�[�g���� [playerPosAdj_in.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerPosAdj_in.h"
#include "player.h"
#include "playerStatus.h"
#include "playerAction.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerPosAdjIn::CPlayerPosAdjIn()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerPosAdjIn::~CPlayerPosAdjIn()
{

}

//==========================================================================
// ����
//==========================================================================
void CPlayerPosAdjIn::UpdateAdjuster(CPlayer* pPlayer)
{
	CPlayer::SMotionFrag flagMotion = pPlayer->GetMotionFrag();	// ���[�V�����t���O
	MyLib::Vector3 pos = pPlayer->GetPosition();	// �ʒu
	CPlayer::EState state = pPlayer->GetState();	// ���
	bool bJump = pPlayer->IsJump();					// �W�����v�t���O

	if (state != CPlayer::EState::STATE_OUTCOURT
	&&  state != CPlayer::EState::STATE_OUTCOURT_RETURN)
	{ // �R�[�g�z����ԈȊO�̓R�[�g���ɕ␳

		// �R�[�g���ɕ␳
		CGameManager::GetInstance()->SetPosLimit(pos);
	}

	if (pos.y <= CGameManager::FIELD_LIMIT)
	{ // �n�ʂ�艺�̏ꍇ

		// �n�ʂɒ��n������
		pos.y = CGameManager::FIELD_LIMIT;

		if (bJump && !flagMotion.bDead)
		{ // �W�����v�����n

			// ���n���[�V�����̍Đ�
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND);
		}

		// �d�͂̏�����
		MyLib::Vector3 move = pPlayer->GetMove();
		move.y = 0.0f;
		pPlayer->SetMove(move);

		// �W�����v���Ă��Ȃ���Ԃɂ���
		pPlayer->SetEnableJump(false);
	}

	if (!pPlayer->IsJump() && !flagMotion.bDead)
	{ // ����R�[�g�ɐN�������Ƃ��̓R�[�g���ɕ␳

		// �`�[���R�[�g�ɖ߂�
		CheckReturn(pPlayer);

#if 1
		// �����Ƃ��Ƃ���
		CheckUnstable(pPlayer);
#endif
	}

	// �ʒu�𔽉f
	pPlayer->SetPosition(pos);
}

//==========================================================================
// �`�[���R�[�g���A
//==========================================================================
void CPlayerPosAdjIn::CheckReturn(CPlayer* pPlayer)
{
	CPlayer::EState state = pPlayer->GetState();	// ���

	// �����߂��Ă�ꍇ�͔�����
	if (state == CPlayer::EState::STATE_INVADE_RETURN
	||  state == CPlayer::EState::STATE_INVADE_TOSS) { return; }

	if (IsLineOut(pPlayer))
	{ // ���C���𒴂��Ă����ꍇ

		// ���A�ݒ�
		ReturnSetting(pPlayer);
	}
}

//==========================================================================
// �����Ƃ��Ƃ���
//==========================================================================
void CPlayerPosAdjIn::CheckUnstable(CPlayer* pPlayer)
{
	// �N������߂���
	if (pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN ||
		pPlayer->GetState() == CPlayer::EState::STATE_INVADE_TOSS)
		return;

	CMotion* motion = pPlayer->GetMotion();
	CPlayerAction* pAction = pPlayer->GetActionPattern();
	CPlayer::SMotionFrag motionFrag = pPlayer->GetMotionFrag();	// ���[�V�����t���O

	CPlayer::EAction action = CPlayer::EAction::ACTION_NONE;	// �A�N�V�������

	bool bBrake = pPlayer->IsBrake();							// �u���[�L�t���O
	MyLib::Vector3 move = pPlayer->GetMove();					// �ړ���

	// �ʃA�N�V������
	if (pAction->GetAction() != CPlayer::EAction::ACTION_NONE &&
		pAction->GetAction() != CPlayer::EAction::ACTION_UNSTABLE)
	{
		pPlayer->SetEnableAction(true);
		return;
	}

	if (IsUnstable(pPlayer))
	{ // �����Ƃ��ƃ��C���𒴂��Ă����ꍇ

		// �u���[�L�t���O
		if (!bBrake)
		{
			move.x = 0.0f;
			bBrake = true;
		}
		else
		{
			move.x = 0.0f;
			pPlayer->SetEnableAction(true);
			pPlayer->SetEnableMove(false);
		}

		if (motionFrag.bMove)
		{// �ړ����Ȃ�

			// �ݒ�
			pPlayer->SetMove(move);
			pPlayer->SetEnableBrake(bBrake);
			pAction->SetAction(action);
			return;
		}

		// ���͏�
		EInputUnstable inputUnstable = IsInputLine(pPlayer);
		if (inputUnstable == EInputUnstable::INPUT_ENEMY)
		{// �G���ɓ��͂��ꂽ

			// ���A�ݒ�
			ReturnSetting(pPlayer);
		}
		else if (inputUnstable == EInputUnstable::INPUT_FRIEND)
		{// �������ɓ��͂��ꂽ

			bBrake = false;
			pPlayer->SetEnableAction(true);
			pPlayer->SetEnableMove(true);
			return;
		}

		// �㏑���h�~
		if (motion->GetType() == CPlayer::EMotion::MOTION_UNSTABLE)	return;

		// �����Ƃ��ƃ��[�V�����̍Đ�
		if (motion->GetType() != CPlayer::EMotion::MOTION_TOSS)
		{
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_UNSTABLE);
		}

		// �A�N�V�����ݒ�
		action = CPlayer::EAction::ACTION_UNSTABLE;
	}
	else
	{
		bBrake = false;
	}

	// �ݒ�
	pPlayer->SetMove(move);
	pPlayer->SetEnableBrake(bBrake);
	pAction->SetAction(action);
}

//==========================================================================
// ���A�ݒ�
//==========================================================================
void CPlayerPosAdjIn::ReturnSetting(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();	// �ʒu
	CBall* pBall = pPlayer->GetBall();				// �{�[�����

	// �m�b�N�o�b�N�̊J�n�ʒu��ݒ�
	CPlayer::SKnockbackInfo infoKnock = pPlayer->GetKnockBackInfo();	// �擾
	infoKnock.posStart = pos;				// �ʒu�ݒ�
	pPlayer->SetKnockBackInfo(infoKnock);	// ���f

	CGameManager::ETeamSide team = pPlayer->GetTeam();		// �`�[��
	
	CGameManager* pGameMgr = CGameManager::GetInstance();

	if (pBall == nullptr)
	{ // �{�[���������Ă��Ȃ��ꍇ

		// �N������߂��Ԃɂ���
		pPlayer->SetState(CPlayer::EState::STATE_INVADE_RETURN);
	}
	else
	{ // �{�[���������Ă�ꍇ

		// �g�X���[�V�����̍Đ�
		pPlayer->SetMotion(CPlayer::EMotion::MOTION_TOSS);

		// �g�X��Ԃɂ���
		pPlayer->SetState(CPlayer::EState::STATE_INVADE_TOSS);

		// ���e����
		pGameMgr->SubCharmValue(team, CCharmManager::ETypeSub::SUB_INVADE_RUN);
	}
}

//==========================================================================
// �[�ɓ���������
//==========================================================================
bool CPlayerPosAdjIn::CheckEdgeEscape(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();
	CGameManager* pGmMgr = CGameManager::GetInstance();
	MyLib::Vector3 posCourt = MyLib::Vector3();											// ���w����
	MyLib::Vector3 sizeCourt = pGmMgr->GetCourtSize(pPlayer->GetTeam(), posCourt);		// ���w�T�C�Y

	// �{�[�����
	CBall* pBall = pGmMgr->GetBall();
	CPlayer* pBallPlayer = pBall->GetPlayer();
	MyLib::Vector3 posBP = pBallPlayer->GetPosition();

	// �{�[�������v���C���[�Ǝ��w�����̃x�N�g��
	MyLib::Vector3 vecDiff = (posBP - posCourt).Normal();

	// �[�G���A���x�N�g�������������
	// ���w4�_(�㉺���E)
	MyLib::Vector3 posEdge[4] =
	{
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z + sizeCourt.x),	// �E��
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z - sizeCourt.x),	// �E��O
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z - sizeCourt.x),	// ����O
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z + sizeCourt.x),	// ����
	};

	MyLib::Vector3 lineMid = MyLib::Vector3();	// ��_
	MyLib::Vector2 areaSize = MyLib::Vector2(sizeCourt.x, sizeCourt.z);		// �G���A�T�C�Y
	areaSize.x *= 0.2f;							//z��x�Z

	// ����
	for (int i = 0; i < 4; i++)
	{
		MyLib::Vector3 pos1 = posEdge[0];
		if (i < 3)
		{
			pos1 = posEdge[i + 1];
		}

		if (UtilFunc::Collision::IsVecToLine(posEdge[i], pos1, posCourt, vecDiff))
		{// ���C������

			lineMid = UtilFunc::Calculation::GetCenterPosition3D(posEdge[i], pos1);

			areaSize += lineMid;
			// �G���A�T�C�Y�ݒ�
			if (lineMid.z == posCourt.z)
			{
				float f = areaSize.x;
				areaSize.x = areaSize.y;
				areaSize.y = f;
			}
		
			break;
		}
		
	}

	if (UtilFunc::Collision::CollisionSquare(lineMid, areaSize, 0.0f, pos))
	{// �[�G���A���ɂ��邩

		return true;
	}

	return false;
}

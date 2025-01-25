//==========================================================================
// 
//  �v���C���[�R���g���[������ [playerUsercontrol_action.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "playerUsercontrol_action.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "ball.h"
#include "playerAction.h"
#include "teamStatus.h"
#include "playerStatus.h"
#include "playerManager.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TAPTIME = 0.125f;	// �^�b�v�̓��͎���
	const float TAPRATE_MIN = 0.6f;	// �^�b�v�̍ŏ�����
	const float TAPRATE_MAX = 1.0f;	// �^�b�v�̍ő劄��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUserControlAction::CPlayerUserControlAction() :
	 m_bThrowButtonHold	(false)	// �����{�^���̃z�[���h����
{

}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerUserControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		// �A�N�V�����p�^�[���ύX
		CPlayer::EMotion motion = CPlayer::EMotion::MOTION_CATCH_STANCE;
		if (player->IsJump())
		{
			motion = CPlayer::EMotion::MOTION_CATCH_STANCE_JUMP;
		}
		SetPattern(player, motion, CPlayer::EAction::ACTION_CATCH);

		// �{�^���z�[���h����
		m_bThrowButtonHold = true;
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerUserControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���Ȃ������甲����
	CBall* pBall = player->GetBall();
	if (pBall == nullptr) return;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �^�b�v�擾
	CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx(), TAPTIME);

	// �����̃{�^���z�[���h����
	if (m_bThrowButtonHold)
	{
		// ��������
		bool bHold = pPad->GetPress(CInputGamepad::BUTTON::BUTTON_B, player->GetMyPlayerIdx());

		if (pPad->GetRelease(CInputGamepad::BUTTON::BUTTON_B, player->GetMyPlayerIdx()))
		{// �����ꂽ�u�Ԃ͂Ȃɂ����Ȃ�
			m_bThrowButtonHold = bHold;
			return;
		}
		m_bThrowButtonHold = bHold;
	}

	// �����̃{�^���z�[���h����Ă����甲����
	if (m_bThrowButtonHold) return;

	if (pKey->GetTrigger(DIK_K) ||
		tapInfo.bInput)
	{// �^�b�v�͈͂̓p�X
		PassSetting(player);
	}

	if (pKey->GetTrigger(DIK_RETURN) ||
		tapInfo.bComplete)
	{// �^�b�v�͈͂𒴂��Ă����ꍇ�͓���
		ThrowSetting(player);
	}
}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerUserControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	
	if (!player->IsJump() &&
		(pKey->GetTrigger(DIK_SPACE) || 
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// �W�����v���ĂȂ� && �W�����v�{�^��

		// �W�����v�g���K�[ON
		SetEnableJumpTrigger(true);

		// �W�����v�ݒ�
		JumpSetting(player);
	}

	if (pKey->GetPress(DIK_SPACE) ||
		pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx()))
	{// �W�����v�{�^���z�[���h�ŏ㏸
		JumpFloat(player, fDeltaTime, fDeltaRate, fSlowRate);
	}

	if ((!pKey->GetPress(DIK_SPACE) && !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// �W�����v�{�^��������

		// �W�����v�g���K�[OFF
		SetEnableJumpTrigger(false);
	}
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerUserControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CGameManager::ETeamSide playerTeam = player->GetTeam();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(playerTeam);
	
	// �X�y�V�����Q�[�WMAX�{�{�[��������
	if (pBall == nullptr ||	!pTeamStatus->IsMaxSpecial()) return;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()))
	{
		//����
		SpecialSetting(player, pBall, pTeamStatus);
	}
}

//==========================================================================
// ���e�{�^��
//==========================================================================
void CPlayerUserControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	player->GetActionPattern()->SetEnableCharm(false);

	if (pKey->GetPress(DIK_C) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_RB, player->GetMyPlayerIdx()))
	{
		//���e�A�N�V���������������
		player->GetActionPattern()->SetEnableCharm(true);
	}
}

//==========================================================================
// ���[�U�[�ύX
//==========================================================================
void CPlayerUserControlAction::UserChange(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ŗǖڕW���
	struct SBestDest
	{
		CPlayer* pPlayer;	// ���݂̖ڕW�v���C���[
		float fMinValue;	// ���݂̍ŗǒl
	};

	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();	// �v���C���[�}�l�[�W���[
	CInputGamepad* pPad = CInputGamepad::GetInstance();				// �p�b�h���
	const int nPadIdx = player->GetMyPlayerIdx();					// �v���C���[���쌠�C���f�b�N�X

	// �E�X�e�B�b�N�̃g���K�[���͌��m�t���O
	const bool bInput = pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_X)
					 || pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_Y);

	if (bInput && !pPlayerManager->IsUserChange())
	{ // �E�X�e�B�b�N�����͂���Ă��銎�A�܂��N�����t���[���ł��̑�������Ă��Ȃ��ꍇ

		CListManager<CPlayer> list = pPlayerManager->GetInList(player->GetTeam());	// ����v���C���[���X�g
		std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
		MyLib::Vector3 posThis = player->GetPosition();		// ����v���C���[�ʒu
		float fRotStick = pPad->GetStickRotR(nPadIdx);		// �X�e�B�b�N����

		// ������̍ŗǖڕW���
		SBestDest bestRot;
		bestRot.pPlayer = nullptr;
		bestRot.fMinValue = D3DX_PI * 2.0f;

		// TODO�F��������K�v���ȁH
#if 0
		// ������̍ŗǖڕW���
		SBestDest bestDis;
		bestDis.pPlayer = nullptr;
		bestDis.fMinValue = CGameManager::GetInstance()->GetHalfCourtDiagonal();
#endif

		while (list.ListLoop(itr))
		{ // ���X�g���̗v�f�����J��Ԃ�

			CPlayer* pItrPlayer = (*itr);	// �v���C���[���

			// ���Ƀ��[�U�[�̏ꍇ����
			if (pItrPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) { continue; }

			// ����v���C���[���猩��AI�̌������v�Z
			float fRotSpace = posThis.AngleXZ(pItrPlayer->GetPosition());

			// �����̍������v�Z
			float fRotDiff = fRotStick - fRotSpace;				// �����̌v�Z
			UtilFunc::Transformation::RotNormalize(fRotDiff);	// ���K�� (3.14�`-3.14)
			fRotDiff = fabsf(fRotDiff);							// ��Βl�� (0.0f�`3.14)

			// ������̍ŗǖڕW���X�V
			if (bestRot.pPlayer == nullptr || bestRot.fMinValue < fRotDiff)
			{ // ���ŗǒl���������ꍇ

				// ����̍ŗǒl��ۑ�
				bestRot.fMinValue = fRotDiff;

				// �ڕW�v���C���[��ۑ�
				bestRot.pPlayer = pItrPlayer;
			}

			// TODO�F��������K�v���ȁH
#if 0
			// ������̍ŗǖڕW���X�V
			if (bestDis.pPlayer == nullptr || bestDis.fMinValue < )
			{ // ���ŗǒl���������ꍇ

				// ����̍ŗǒl��ۑ�
				bestDis.fMinValue = ;

				// �ڕW�v���C���[��ۑ�
				bestDis.pPlayer = pItrPlayer;
			}
#endif
		}

		// �ŗǂ̖ڕW�v���C���[�ɑ��쌠��n��
		if (bestRot.pPlayer != nullptr)
		{ // �ύX�Ώۂ����݂���ꍇ

			// �x�[�X�̓���ւ�
			pPlayerManager->SwapBase(player, bestRot.pPlayer);

			// ���[�U�[�ύX����ς݂ɂ���
			pPlayerManager->SetEnableUserChange(true);
		}
	}
}

//=============================================================================
// 
//  �v���C���[�R���g���[������ [playerUsercontrol_action.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
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

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TAPTIME = 0.125f;		// �^�b�v�̓��͎���
	const float TAPRATE_MIN = 0.6f;	// �^�b�v�̍ŏ�����
	const float TAPRATE_MAX = 1.0f;	// �^�b�v�̍ő劄��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUserControlAction::CPlayerUserControlAction()
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

	CMotion* pMotion = player->GetMotion();

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
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerUserControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr) return;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	// �^�b�v�擾
	CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx(), TAPTIME);

	if (pKey->GetTrigger(DIK_K) ||
		tapInfo.bInput)
	{// �^�b�v�͈͂̓p�X
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
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
		JumpFloat(player);
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

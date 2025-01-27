//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�A�N�V�������� [playerAIcontrol_action.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAIOut_controlAction.h"
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
//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TAPTIME = 0.15f;	// �^�b�v�̓��͎���
	const float TAPRATE_MIN = 0.7f;	// �^�b�v�̍ŏ�����
	const float TAPRATE_MAX = 1.0f;	// �^�b�v�̍ő劄��
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
bool CPlayerAIOutControlAction::m_bAutoThrow = true;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIOutControlAction::CPlayerAIOutControlAction()
{
	ZeroMemory(&m_sFlag, sizeof(m_sFlag));
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerAIOutControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;

	CMotion* pMotion = player->GetMotion();

	// TODO�F�S�����L���b�`�@�\
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall->IsAttack() || pBall->GetTypeTeam() == player->GetTeam()) return;

	if (UtilFunc::Collision::CircleRange3D(pBall->GetPosition(), player->GetPosition(), pBall->GetRadius(), 100.0f))
	{
		// �A�N�V�����p�^�[���ύX
		SetPattern(player, CPlayer::EMotion::MOTION_CATCH_STANCE, CPlayer::EAction::ACTION_CATCH);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAIOutControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if _DEBUG
	if (!m_bAutoThrow) { return; }
#endif

	if (!player->GetBall()) return;


	CBall* pBall = player->GetBall();

	if (m_sFlag.bThrow)
	{
		m_sFlag.bThrow = false;

		// ������
		ThrowSetting(player);
	}
	else if (m_sFlag.bPass)
	{
		m_sFlag.bPass = false;

		// �p�X
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
	}
}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerAIOutControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bJump = player->IsJump();

	if (bJump) return;

	//�W�����v����	//TODO: AI�ł̍s���t���O�Ƃ��g���H
	if (m_sFlag.bJump)
	{
		// �t���O���Z�b�g
		m_sFlag.bJump = false;

		// �W�����v�Z�b�g
		JumpSetting(player);
	}

	if (m_sFlag.bJumpFloat)
	{// �W�����v�{�^���z�[���h�ŏ㏸
		JumpFloat(player, fDeltaTime, fDeltaRate, fSlowRate);
	}
	if (!m_sFlag.bJumpFloat)
	{// �W�����v�{�^��������

		// �W�����v�g���K�[OFF
		SetEnableJumpTrigger(false);
	}
}

//==========================================================================
// �W�����v�㏸
//==========================================================================
void CPlayerAIOutControlAction::JumpFloat(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �W�����v����擾
	bool bJump = player->IsJump();

	if (bJump && IsJumpTrigger())
	{// �W�����v���͉����Ă�ԋ����L�тĂ���

		// �C���v�b�g���擾
		CInputGamepad* pPad = CInputGamepad::GetInstance();

		//�W�����v����
		CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx(), TAPTIME);

		if (tapInfo.fRatio < 1.0f && pPad->GetPress(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx()))
		{// �^�b�v�͈� && ���͌p��

			// �ړ��ʎ擾
			MyLib::Vector3 move = player->GetMove();

			float jumpRatio = TAPRATE_MIN + (TAPRATE_MAX - TAPRATE_MIN) * tapInfo.fRatio;
			move.y = player->GetParameter().fVelocityJump * jumpRatio;

			// �ړ��ʐݒ�
			player->SetMove(move);
		}
	}
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerAIOutControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// ���e�{�^��
//==========================================================================
void CPlayerAIOutControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	player->GetActionPattern()->SetEnableCharm(false);

	//TODO: �t���O�������ő���
	if (false)
	{
		//���e�A�N�V���������������
		player->GetActionPattern()->SetEnableCharm(true);
	}
}

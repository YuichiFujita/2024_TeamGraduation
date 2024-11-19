//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�A�N�V�������� [playerAIcontrol_action.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAIcontrol_action.h"
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
bool CPlayerAIControlAction::m_bAutoThrow = true;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlAction::CPlayerAIControlAction()
{
	ZeroMemory(&m_sFlag, sizeof(m_sFlag));
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerAIControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if _DEBUG
	if (!m_bAutoThrow) { return; }
#endif

	CBall* pBall = player->GetBall();

	if (m_sFlag.bThrow)
	{
		if (pBall == nullptr)
		{
			m_sFlag.bThrow = false;

			return;
		}

		// ������
		ThrowSetting(player);
	}
}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerAIControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bJump = player->IsJump();

	if (bJump) return;

	//�W�����v����	//TODO: AI�ł̍s���t���O�Ƃ��g���H
	if (m_sFlag.bJump)
	{
		// �t���O���Z�b�g
		m_sFlag.bJump = false;

		JumpSetting(player);
	}
}

//==========================================================================
// �W�����v�㏸
//==========================================================================
void CPlayerAIControlAction::JumpFloat(CPlayer* player)
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
void CPlayerAIControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(player->GetTeam());

	//�X�y�V�����Q�[�WMAX�{�{�[��������
	if (pBall == nullptr || !pTeamStatus->IsMaxSpecial()) return;

	//TODO: �t���O�������Ŕ�������
	if (m_sFlag.bSpecial)
	{
		// �t���O���Z�b�g
		m_sFlag.bSpecial = false;

		//����
		SpecialSetting(player, pBall, pTeamStatus);
	}
}

//==========================================================================
// ���e�{�^��
//==========================================================================
void CPlayerAIControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	player->GetActionPattern()->SetEnableCharm(false);

	//TODO: �t���O�������ő���
	if (false)
	{
		//���e�A�N�V���������������
		player->GetActionPattern()->SetEnableCharm(true);
	}
}

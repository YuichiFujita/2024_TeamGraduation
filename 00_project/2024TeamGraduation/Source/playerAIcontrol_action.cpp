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
	const float TAPTIME = 0.125f;	// �^�b�v�̓��͎���
	const float TAPRATE_MIN = 0.6f;	// �^�b�v�̍ŏ�����
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
	m_fJumpTimer = 0.0f;
	m_fJumpRate = 0.0f;
	m_fMaxJumpRate = 0.0f;
	m_fCatchRadius = 0.0f;
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerAIControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;
	//if (!m_sFlag.bCatch) return;

	// TODO�F�S�����L���b�`�@�\
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall->IsAttack() || pBall->GetTypeTeam() == player->GetTeam()) return;

	if (UtilFunc::Collision::CircleRange3D(pBall->GetPosition(), player->GetPosition(), pBall->GetRadius(), m_fCatchRadius))
	{
		// �A�N�V�����p�^�[���ύX
		SetPattern(player, CPlayer::EMotion::MOTION_CATCH_STANCE, CPlayer::EAction::ACTION_CATCH);
	}

	m_sFlag.bCatch = false;
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
void CPlayerAIControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//�W�����v����
	if (!player->IsJump() &&		// �W�����v���ĂȂ�
		m_sFlag.bJump)				// �W�����vON
	{
		// �W�����vOFF
		m_sFlag.bJump = false;

		// �W�����v�㏸ON
		m_sFlag.bJumpFloat = true;

		// �W�����v�g���K�[ON
		SetEnableJumpTrigger(true);

		// �W�����v�ݒ�
		JumpSetting(player);
	}

	if (m_sFlag.bJumpFloat)
	{// �W�����v�{�^���z�[���h�ŏ㏸
		JumpFloat(player, fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �W�����v�㏸
//==========================================================================
void CPlayerAIControlAction::JumpFloat(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �W�����v����擾
	bool bJump = player->IsJump();

	if (bJump && IsJumpTrigger())
	{// �W�����v���͉����Ă�ԋ����L�тĂ���

		//�W�����v����
		m_fJumpTimer += fDeltaTime * fSlowRate;
		float rate = m_fJumpTimer / TAPTIME;
		
		if (m_fMaxJumpRate > rate)
		{
			// �ړ��ʎ擾
			MyLib::Vector3 move = player->GetMove();

			float jumpRate = TAPRATE_MIN + (TAPRATE_MAX - TAPRATE_MIN) * rate;
			m_fJumpRate = jumpRate;

			move.y = player->GetParameter().fVelocityJump * jumpRate;
		
			// �ړ��ʐݒ�
			player->SetMove(move);
		}
		else
		{// �I��
			// �t���O���Z�b�g
			m_sFlag.bJumpFloat = false;
			m_fJumpTimer = 0.0f;
			m_fJumpRate = 0.0f;

			// �W�����v�g���K�[OFF
			SetEnableJumpTrigger(false);
		}

		return;
	}

	// �^�C�}�[���Z�b�g
	m_fJumpTimer = 0.0f;
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

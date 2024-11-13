//==========================================================================
// 
//  AI�v���C���[���� [playerAI.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "teamStatus.h"

#include "ball.h"

// �g�p�N���X
#include "playerStatus.h"
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAI::CPlayerAI(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeArea)
{
	// ��������̐ݒ�
	switch (typeArea)
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::EFieldArea::FIELD_IN:

		// ���쑀��̊���
		ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
		ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
		break;

	case CPlayer::EFieldArea::FIELD_OUT:

		// �O�쑀��̊���
		ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
		ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
		break;

	default:
		assert(false);
		break;
	}

	// �v���C���[AI�R���g���[���̐���
	m_pAIControl = nullptr;
	m_pAIControl = CPlayerAIControl::Create(pPlayer);
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �v���C���[�̎擾
	CPlayer* pPlayer = GetPlayer();

	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����̍X�V
	pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);

	// �R���g���[���X�V
	if (m_pAIControl)
	{
		m_pAIControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerAI::Hit(CBall* pBall)
{
	CPlayer::SHitInfo hitInfo = CPlayerBase::Hit(pBall);	// �q�b�g���

	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // �ʏ���
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // �L���b�`���
		// �������[�h
		m_pAIControl->SetMode(CPlayerAIControl::EMode::MODE_THROW);
	}

	return hitInfo;
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerControlMove* control)
{
	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	// ����N���X�̓���
	delete pControlMove;
	pControlMove = control;

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerControlAction* control)
{
	// ����N���X�̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����N���X�̓���
	delete pControlAction;
	pControlAction = control;

	// ����N���X�̔��f
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CPlayerAI::Debug()
{
#if _DEBUG
	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*>(GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// ���������t���O�ݒ�
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{ 
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}

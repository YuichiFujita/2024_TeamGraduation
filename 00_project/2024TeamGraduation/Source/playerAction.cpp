//=============================================================================
// 
//  �v���C���[�R���g���[������ [playerAction.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAction.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"

namespace
{
	const float TIME_BLINK = 0.2f;		// �u�����N����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAction::ACTION_FUNC CPlayerAction::m_ActionFunc[] =	// �s���֐�
{
	&CPlayerAction::ActionNone,			// �Ȃ�
	&CPlayerAction::ActionBlink,		// �u�����N
	&CPlayerAction::ActionRun,			// ����
	&CPlayerAction::ActionJump,			// �W�����v
	&CPlayerAction::ActionCatch,		// �L���b�`
	&CPlayerAction::ActionThrow,		// ����
	&CPlayerAction::ActionThrowJump,	// ����(�W�����v)
	&CPlayerAction::ActionSpecial,		// �X�y�V����
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAction::CPlayerAction(CPlayer* player)
{
	m_Action = CPlayer::Action::ACTION_NONE;	// �A�N�V����
	m_fActionTime = 0.0f;						// �A�N�V��������
	m_pPlayer = player;							// �v���C���[�̃|�C���^
}

//==========================================================================
// �X�V
//==========================================================================
void CPlayerAction::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �A�N�V��������
	m_fActionTime += fDeltaTime * fDeltaRate * fSlowRate;

	// �s���X�V
	(this->*(m_ActionFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CPlayerAction::ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �u�����N
//==========================================================================
void CPlayerAction::ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fActionTime >= TIME_BLINK)
	{// �u�����N�o��
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAction::ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerAction::ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerAction::ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAction::ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// �W�����v����
//==========================================================================
void CPlayerAction::ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerAction::ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// �A�N�V�����ݒ�
//==========================================================================
void CPlayerAction::SetAction(CPlayer::Action action)
{
	m_Action = action;
	m_fActionTime = 0.0f;		// �A�N�V��������
}

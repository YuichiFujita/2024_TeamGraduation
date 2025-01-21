//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "teamStatus.h"

// �f�o�b�O�p
#include "3D_effect.h"

// player�֘A
#include "playerBase.h"
#include "playerStatus.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_move.h"
#include "playerManager.h"

#include "playerAIcontrol_idle.h"
#include "playerAIcontrol_attack.h"
#include "playerAIcontrol_defense.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	
}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControl* CPlayerAIControl::Create(CPlayer* player)
{
	if (!player) assert(("�v���C���[��񂪖�����", false));

	// �������̊m��
	CPlayerAIControl* pObj = nullptr;

	if (pObj == nullptr)
	{
		pObj = DEBUG_NEW CPlayerAIControl;

		pObj->m_pAI = player;

		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerAIControl::Init()
{
	// ���[�h����
	m_pAIControlMode = CPlayerAIControlMode::Create(m_pAI, CPlayerAIControlMode::EMode::MODE_IDLE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControl::Uninit()
{
	if (!m_pAIControlMode) return;
	m_pAIControlMode->Uninit();
	m_pAIControlMode = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�h�Ǘ�
	ModeManager();

	if (m_pAIControlMode)
	{
		m_pAIControlMode->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControl::ModeManager()
{
	if (!m_pAIControlMode) return;

	CPlayerAIControlMode::EMode modeCurrent, mode;
	modeCurrent = m_pAIControlMode->GetMode();
	mode = modeCurrent;

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) {// �{�[�������E�ɑ��݂��Ȃ��ꍇ
		mode = CPlayerAIControlMode::EMode::MODE_IDLE;
		return;
	}

	if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		mode = CPlayerAIControlMode::EMode::MODE_ATTACK;	// ����
	}
	else if (pBall->GetPlayer() == nullptr || pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		mode = CPlayerAIControlMode::EMode::MODE_DEFENCE;	// �L���b�`
	}
	else if (pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() == m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[���������ꍇ
		mode = CPlayerAIControlMode::EMode::MODE_IDLE;	// �ҋ@
	}

	if (mode != modeCurrent)
	{// ���݂̃��[�h�ƈႤ�ꍇ
		// ���[�h�ύX
		ChangeMode(m_pAI, mode);
	}
}

//==========================================================================
// ���[�h�؂�ւ�
//==========================================================================
void CPlayerAIControl::ChangeMode(CPlayer* pPlayer, CPlayerAIControlMode::EMode mode)
{
	if (!m_pAIControlMode) return;

	m_pAIControlMode->Uninit();
	m_pAIControlMode = nullptr;

	if (!m_pAIControlMode)
	{
		m_pAIControlMode = CPlayerAIControlMode::Create(pPlayer, mode);
	}
}

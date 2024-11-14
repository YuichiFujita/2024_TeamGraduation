//==========================================================================
// 
//  AI�v���C���[���� [playerAIIn.cpp]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================
#include "playerAIIn.h"

// �g�p�N���X
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIIn::CPlayerAIIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerAI(pPlayer, typeTeam, typeArea)
{
	// ���쑀��̊���
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	// �v���C���[AI�R���g���[���̐���
	m_pAIControl = CPlayerAIControl::Create(pPlayer);
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIIn::~CPlayerAIIn()
{
	delete m_pAIControl;
	m_pAIControl = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���N���X�̍X�V
	CPlayerAI::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �R���g���[���X�V
	if (m_pAIControl)
	{
		m_pAIControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerAIIn::Hit(CBall* pBall)
{
	// ���N���X�̃q�b�g
	CPlayer::SHitInfo hitInfo = CPlayerAI::Hit(pBall);	// �q�b�g���

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
// �f�o�b�O
//==========================================================================
void CPlayerAIIn::Debug()
{
#if _DEBUG
	// ���N���X�̃f�o�b�O�\��
	CPlayerAI::Debug();

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

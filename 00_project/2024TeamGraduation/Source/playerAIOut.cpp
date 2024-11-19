//==========================================================================
// 
//  AI�v���C���[���� [playerAIOut.cpp]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================
#include "playerAIOut.h"

// �g�p�N���X
#include "playerAIOut_controlMove.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_AI)
{
	// �O�쑀��̊���
	ChangeMoveControl(DEBUG_NEW CPlayerAIOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{

}

//==========================================================================
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerAIOut::Hit(CBall* pBall)
{
	// ���N���X�̃q�b�g
	CPlayer::SHitInfo hitInfo = CPlayerOut::Hit(pBall);	// �q�b�g���

#if 0
	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // �ʏ���
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // �L���b�`���
		// �������[�h
		m_pAIControl->SetMode(CPlayerAIControl::EMode::MODE_THROW);
	}
#endif

	return hitInfo;
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CPlayerAIOut::Debug()
{
#if _DEBUG
	// ���N���X�̃f�o�b�O�\��
	CPlayerOut::Debug();

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

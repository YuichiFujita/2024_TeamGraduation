//=============================================================================
// 
//  AI�v���C���[���� [playerAI.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "teamStatus.h"


// �g�p�N���X
#include "playerStatus.h"
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAI::STATE_FUNC CPlayerAI::m_StateFunc[] =	// ��Ԋ֐�
{
	&CPlayerAI::StateNone,				// �Ȃ�
	//&CPlayerAI::StateThrowManager,		// �����}�l�[�W���[
	//&CPlayerAI::StateCatchManager,		// �L���b�`�}�l�[�W���[
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAI::CPlayerAI()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CPlayerAI *CPlayerAI::Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos)
{
	// �������̊m��
	CPlayerAI* pPlayer = DEBUG_NEW CPlayerAI;
	if (pPlayer != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pPlayer);
			return nullptr;
		}

		// �`�[���T�C�h��ݒ�
		pPlayer->GetStatus()->SetTeam(team);

		// �ʒu��ݒ�
		pPlayer->SetPosition(rPos);
	}

	return pPlayer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerAI::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// �e�N���X�̏�����
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// ����֘A
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAI::Uninit()
{
	// �e�N���X�̏I��
	CPlayer::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CPlayerAI::Kill()
{
	// �e�N���X�̏I��
	CPlayer::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayerAI::Draw()
{
	// �e�N���X�̕`��
	CPlayer::Draw();
}

//==========================================================================
// �q�b�g����
//==========================================================================
CPlayerAI::SHitInfo CPlayerAI::Hit(CBall* pBall)
{
	CPlayer::SHitInfo hitInfo = CPlayer::Hit(pBall);	// �q�b�g���

	if (hitInfo.eHit == EHit::HIT_NONE){
		return hitInfo;
	}

	if (hitInfo.eHit == EHit::HIT_CATCH)
	{// �L���b�`���
		StateThrowManager(pBall);
	}

	return hitInfo;
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAI::StateThrowManager(CBall* pBall)
{
	CGameManager* pGameManager = CGame::GetInstance()->GetGameManager();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(this->GetCharStatus()->GetTeam());

	// �{�[��������
	if (pBall == nullptr) return;

	// TODO: �t���O�������Ŕ�������
	if (false)
	{
		if (pTeamStatus->IsMaxSpecial())
		{
			SpecialThrow();
		}
		else
		{
			Throw();
		}
	}
}

//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAI::StateCatchManager()
{

}

//==========================================================================
// �ʏ퓊������
//==========================================================================
void CPlayerAI::Throw()
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction =pControlAction->GetAI();

	pControlAIAction->SetIsThrow(true);
}

//==========================================================================
// �W�����v��������
//==========================================================================
void CPlayerAI::JumpThrow()
{

}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CPlayerAI::SpecialThrow()
{
	
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CPlayerAI::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	(this->*(m_StateFunc[m_throwMode]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CPlayerAI::StateNone()
{

}

//==========================================================================
// ���쏈��
//==========================================================================
void CPlayerAI::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// �ړ�����	// TODO�FAI�ނ��`
	//m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	delete pControlMove;
	pControlMove = control;

	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	delete pControlAction;
	pControlAction = control;

	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayerAI::DeleteControl()
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	if (pControlMove != nullptr)
	{// �ړ�����
		delete pControlMove;
		pControlMove = nullptr;
	}

	if (pControlAction != nullptr)
	{// �A�N�V��������
		delete pControlAction;
		pControlAction = nullptr;
	}

	// ����֘A�폜
	CPlayer::DeleteControl();

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}


//==========================================================================
// �f�o�b�O����
//==========================================================================
void CPlayerAI::Debug()
{
#if _DEBUG
	// �f�o�b�O����
	CPlayer::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*> (GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// ���������t���O�ݒ�
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{ 
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}
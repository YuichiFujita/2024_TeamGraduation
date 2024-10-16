//=============================================================================
// 
//  AI�v���C���[���� [playerAI.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"

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
// �R���X�g���N�^
//==========================================================================
CPlayerAI::CPlayerAI()
{
	// �p�^�[���p�C���X�^���X
	m_pControlMove = nullptr;	// �ړ�����
	m_pControlAction = nullptr;	// �A�N�V��������
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
CPlayerAI *CPlayerAI::Create(const int nIdx, const CGameManager::TeamSide team)
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

		// �v���C���[�C���f�b�N�X��ݒ�
		pPlayer->SetMyPlayerIdx(nIdx);

		// �`�[���T�C�h��ݒ�
		pPlayer->GetStatus()->SetTeam(team);
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
	CPlayer::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);
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
// �ړ�����
//==========================================================================
void CPlayerAI::Move(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ړ�����
	//m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	//m_pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	delete m_pControlMove;
	m_pControlMove = control;
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	delete m_pControlAction;
	m_pControlAction = control;
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayerAI::DeleteControl()
{
	if (m_pControlMove != nullptr)
	{// �ړ�����
		delete m_pControlMove;
		m_pControlMove = nullptr;
	}

	if (m_pControlAction != nullptr)
	{// �A�N�V��������
		delete m_pControlAction;
		m_pControlAction = nullptr;
	}

	// ����֘A�폜
	CPlayer::DeleteControl();
}

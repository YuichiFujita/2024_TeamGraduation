//=============================================================================
// 
//  ���[�U�[�v���C���[���� [playerUser.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerUser.h"
#include "manager.h"
#include "calculation.h"

// �g�p�N���X
#include "playerStatus.h"
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUser::CPlayerUser()
{
	// �p�^�[���p�C���X�^���X
	m_pControlMove = nullptr;	// �ړ�����
	m_pControlAction = nullptr;	// �A�N�V��������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUser::~CPlayerUser()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CPlayerUser *CPlayerUser::Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos)
{
	// �������̊m��
	CPlayerUser* pPlayer = DEBUG_NEW CPlayerUser;
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
HRESULT CPlayerUser::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// �e�N���X�̏�����
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// ����֘A
	ChangeMoveControl(DEBUG_NEW CPlayerControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerControlAction());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerUser::Uninit()
{
	// �e�N���X�̏I��
	CPlayer::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CPlayerUser::Kill()
{
	// �e�N���X�̏I��
	CPlayer::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerUser::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayerUser::Draw()
{
	// �e�N���X�̕`��
	CPlayer::Draw();
}

//==========================================================================
// ���쏈��
//==========================================================================
void CPlayerUser::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ړ�����
	m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	m_pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeMoveControl(CPlayerControlMove* control)
{
	delete m_pControlMove;
	m_pControlMove = control;
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeActionControl(CPlayerControlAction* control)
{
	delete m_pControlAction;
	m_pControlAction = control;
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayerUser::DeleteControl()
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

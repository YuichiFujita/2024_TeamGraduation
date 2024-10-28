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
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"

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
	ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());

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
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// �ړ�����
	pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeMoveControl(CPlayerUserControlMove* control)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	delete pControlMove;
	pControlMove = control;

	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeActionControl(CPlayerUserControlAction* control)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	delete pControlAction;
	pControlAction = control;

	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayerUser::DeleteControl()
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

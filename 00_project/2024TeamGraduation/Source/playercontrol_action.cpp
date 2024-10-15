//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playercontrol_action.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerControlAction::CPlayerControlAction()
{

}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlAction::Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �ړ��ʎ擾
	float fMove = player->GetVelocity();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �ڕW�̌����擾
	float fRotDest = player->GetRotDest();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// ��Ԏ擾
	CPlayer::STATE state = player->GetState();

	// �A�N�V�����擾
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return;
	CPlayer::Action action = pPlayerAction->GetAction();


	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// �ړ��\���[�V�����̎�

		//--------------------------
		// �u�����N����
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK)
		{
			//Blink(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

		// �ړ����ɂ���
		motionFrag.bMove = true;

	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();



}

//==========================================================================
// ����
//==========================================================================
void CPlayerControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() == nullptr)
	{
		return;
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetPress(DIK_K))
	{

	}

}

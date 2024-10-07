//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playercontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "debugproc.h"
#include "collisionLine_Box.h"
#include "EffekseerObj.h"

namespace
{
	const float MULTIPLIY_DASH = 1.5f;		// �_�b�V���̔{��
}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player)
{
	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_GOAL) return;

	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// �_�b�V������
	bool bDash = false;
	if (pInputGamepad->GetPress(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()) &&
		pInputGamepad->IsTipStick())
	{// ���X�e�B�b�N���|��Ă�ꍇ
		//bDash = true;
	}
	player->SetEnableDash(bDash);


	// �ړ��ʎ擾
	float fMove = player->GetVelocity();
	if (bDash){
		// �_�b�V���{���|����
		fMove *= MULTIPLIY_DASH;
	}


	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();
	Camerarot.y -= pCamera->GetOriginRotation().y;

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

	// �ړ�����
	int angle = 0;

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_DEADWAIT)
	{// �ړ��\���[�V�����̎�

		

		// �ړ����ɂ���
		motionFrag.bMove = true;

		if (pInputGamepad->IsTipStick())
		{// ���X�e�B�b�N���|��Ă�ꍇ

		}

		// �W�����v�󋵎擾
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����I������
			pMotion->ToggleFinish(true);

			// �ړ����[�V����
			if (bDash)
			{
				//pMotion->Set(CPlayer::MOTION::MOTION_DASH);
			}
			else
			{
				pMotion->Set(CPlayer::MOTION::MOTION_WALK);
			}
		}


		if (!bJump &&
			(pInputKeyboard->GetTrigger(DIK_SPACE) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx())))
		{// �W�����v

			//bJump = true;
			//motionFrag.bJump = true;
			//move.y += 17.0f;

			//pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			//// �T�E���h�Đ�
			//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
		}

		// �W�����v����ݒ�
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// �ړ��\�ȃ��[�V�������擾
		state != CPlayer::STATE::STATE_DEAD)
	{
		if (pInputKeyboard->GetPress(DIK_A))
		{//���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//A+W,����ړ�
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//A+S,�����ړ�
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,���ړ�
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{//D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//D+W,�E��ړ�
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//D+S,�E���ړ�
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,�E�ړ�
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{//W�������ꂽ�A��ړ�
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{//S�������ꂽ�A���ړ�
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

}


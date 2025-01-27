//==========================================================================
// 
//  ���[�U�[�v���C���[���� [playerUserOut.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerUserOut.h"
#include "manager.h"
#include "camera.h"

// �g�p�N���X
#include "playerUserOut_controlMove.h"
#include "playerUserOut_controlAction.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUserOut::CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_USER)
{
	// �O�쑀��̊���
	ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserOutControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUserOut::~CPlayerUserOut()
{

}

//==========================================================================
// �J�j�������[�V�����ݒ�
//==========================================================================
void CPlayerUserOut::MotionCrab(int nStartKey)
{
	CPlayer::CRAB_DIRECTION playerDir = CPlayer::CRAB_DIRECTION::CRAB_NONE;
	CPlayer::CRAB_DIRECTION inputDir = CPlayer::CRAB_DIRECTION::CRAB_NONE;

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// ����
	MyLib::Vector3 rot = GetPlayer()->GetRotation();

	//--------------------------------
	// �v���C���[����
	//--------------------------------
	bool bRot = false;
	D3DXCOLOR col = D3DXCOLOR();
	float fRotY = D3DX_PI * 1.0f + rot.y;
	UtilFunc::Transformation::RotNormalize(fRotY);

	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);
	if (!UtilFunc::Collision::CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// ������
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// �����
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
		bRot = true;
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// ������
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
	{// �E����
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
		bRot = true;
		col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{// �����������
		MyAssert::CustomAssert(false, "�J�j�����F�ǂ����Ă�˂�");
	}

	//--------------------------------
	// ���͕���
	//--------------------------------
	CPlayer::EDashAngle angle = GetPlayerControlMove()->GetInputAngle();
	if (angle == CPlayer::EDashAngle::ANGLE_MAX) return;

	switch (angle)
	{
	case CPlayer::EDashAngle::ANGLE_UP:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
		break;

	case CPlayer::EDashAngle::ANGLE_DOWN:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
		break;

	case CPlayer::EDashAngle::ANGLE_RIGHT:
	case CPlayer::EDashAngle::ANGLE_RIGHTUP:
	case CPlayer::EDashAngle::ANGLE_RIGHTDW:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
		break;

	case CPlayer::EDashAngle::ANGLE_LEFT:
	case CPlayer::EDashAngle::ANGLE_LEFTUP:
	case CPlayer::EDashAngle::ANGLE_LEFTDW:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
		break;

	default:
		break;
	}

	if (playerDir == CPlayer::CRAB_DIRECTION::CRAB_NONE ||
		inputDir == CPlayer::CRAB_DIRECTION::CRAB_NONE)
	{// ����Ɉ����������Ă��Ȃ�
		return;
	}

	// ���[�V�����ݒ�
	GetPlayer()->SetMotion(Crab::MOTION_WALK[playerDir][inputDir], nStartKey);
}

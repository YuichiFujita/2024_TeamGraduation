//==========================================================================
// 
//  ���[�U�[�v���C���[���� [playerUser.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerUser.h"
#include "manager.h"
#include "camera.h"

// �g�p�N���X
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace Crab	// �J�j����
{
	const float RANGE = D3DX_PI * 0.25f;	// ����p�x�̔���

	const float RANGE_MIN_MAX[] =	// ����p�x��
	{
		D3DX_PI * 1.0f + RANGE,		// ��Max
		D3DX_PI * 1.0f - RANGE,		// ��Min
		D3DX_PI * 0.0f + RANGE,		// ��Max
		D3DX_PI * 0.0f - RANGE,		// ��Min
		D3DX_PI * -0.5f + RANGE,	// ��Max
		D3DX_PI * -0.5f - RANGE,	// ��Min
		D3DX_PI * 0.5f + RANGE,		// �EMax
		D3DX_PI * 0.5f - RANGE,		// �EMin
	};

	// �J�j�������[�V�����ݒ��
	const CPlayer::EMotion MOTION_WALK[CPlayer::CRAB_DIRECTION::CRAB_MAX][CPlayer::CRAB_DIRECTION::CRAB_MAX] =
	{
		// [ �v���C���[���� ][ �C���v�b�g���� ]

		{// UP(�v���C���[�������)
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// UP 
			CPlayer::EMotion::MOTION_CRAB_BACK,		// DOWN
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// LEFT
			CPlayer::EMotion::MOTION_CRAB_RIGHT		// RIGHT
		},
		{// DOWN(�v���C���[��������)
			CPlayer::EMotion::MOTION_CRAB_BACK,		// UP 
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// DOWN
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// LEFT
			CPlayer::EMotion::MOTION_CRAB_LEFT 		// RIGHT
		},
		{// LEFT(�v���C���[��������)
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// UP 
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// DOWN
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// LEFT
			CPlayer::EMotion::MOTION_CRAB_BACK 		// RIGHT
		},
		{// RIGHT(�v���C���[���E����)
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// UP 
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// DOWN
			CPlayer::EMotion::MOTION_CRAB_BACK,		// LEFT
			CPlayer::EMotion::MOTION_CRAB_FRONT 	// RIGHT
		},
	};
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUser::CPlayerUser(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUser::~CPlayerUser()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerUser::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �v���C���[�̎擾
	CPlayer* pPlayer = GetPlayer();

	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����̍X�V
	pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeMoveControl(CPlayerControlMove* control)
{
	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	// ����N���X�̓���
	delete pControlMove;
	pControlMove = control;

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeActionControl(CPlayerControlAction* control)
{
	// ����N���X�̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����N���X�̓���
	delete pControlAction;
	pControlAction = control;

	// ����N���X�̔��f
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �J�j�������[�V�����ݒ�
//==========================================================================
void CPlayerUser::MotionCrab(int nStartKey)
{
	CPlayer::CRAB_DIRECTION playerDir = CPlayer::CRAB_DIRECTION::CRAB_NONE;
	CPlayer::CRAB_DIRECTION inputDir = CPlayer::CRAB_DIRECTION::CRAB_NONE;

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// ����
	MyLib::Vector3 rot = GetPlayer()->GetRotation();

	// �����_(bool)
	auto CollisionRangeAngle = [](float angle, float maxAngle, float minAngle)
	{
		// ���K������
		UtilFunc::Transformation::RotUnNormalize(angle);
		UtilFunc::Transformation::RotUnNormalize(maxAngle);
		UtilFunc::Transformation::RotUnNormalize(minAngle);

		// �x���@�ɕϊ�
		int nAngle = static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(angle));
		int nMaxAngle = static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(maxAngle));
		int nMinAngle = static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(minAngle));

		if (nMaxAngle <= nMinAngle)
		{// �͈͂�360�����ׂ��ꍇ

			// nAngle��Min�ȏ�Max�ȉ��B
			bool bRange = (nMaxAngle <= nAngle && nAngle <= nMinAngle);
			return bRange;
		}
		else
		{// �͈͂��ʏ�̏����Ŏw�肳��Ă���ꍇ

			// nAngle��Min�ȏ�Max�ȉ��B
			bool bRange = (nMaxAngle >= nAngle && nAngle >= nMinAngle);
			return bRange;
		}

		return false;
	};

	//--------------------------------
	// �v���C���[����
	//--------------------------------
	bool bRot = false;
	D3DXCOLOR col = D3DXCOLOR();
	float fRotY = D3DX_PI * 1.0f + rot.y;
	UtilFunc::Transformation::RotNormalize(fRotY);
	
	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);
	if (!CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// ������
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// �����
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
		bRot = true;
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// ������
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
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
	CPlayer::EDashAngle* angle = GetPlayerControlMove()->GetInputAngle();
	if (angle == nullptr) return;

	switch (*angle)
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

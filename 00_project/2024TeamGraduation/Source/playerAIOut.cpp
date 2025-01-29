//==========================================================================
// 
//  AI�v���C���[���� [playerAIOut.cpp]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================
#include "playerAIOut.h"
#include "manager.h"
#include "camera.h"

// �g�p�N���X
#include "playerAIOut_controlMove.h"
#include "playerAIOut_controlAction.h"
#include "playerAction.h"
#include "playerAIOut_control.h"
#include "playerManager.h"

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
	ChangeActionControl(DEBUG_NEW CPlayerAIOutControlAction());

	// AI�R���g���[��(�O��)�̐���
	m_pAIOutControl = CPlayerAIOutControl::Create(pPlayer);
	m_eAIRange = RANGE_NONE;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{
	if (m_pAIOutControl)
	{
		delete m_pAIOutControl;
		m_pAIOutControl = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIOut::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���N���X�̍X�V
	CPlayerOut::Update(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pAIOutControl)
	{// AI�R���g���[��(�O��)�̍X�V
		m_pAIOutControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �J�j�������[�V�����ݒ�
//==========================================================================
void CPlayerAIOut::MotionCrab(int nStartKey)
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
	float fRotY = D3DX_PI * 1.0f + rot.y;
	UtilFunc::Transformation::RotNormalize(fRotY);

	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);

	// �O��|�W�V�����̎擾
	CPlayerManager::EOutPos eOutPos = CPlayerManager::GetInstance()->GetOutPosition(GetPlayer());

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �i�s�����̎擾
	float fDirection = pControlAIOutMove->GetClabDirection();

	// �p�x
	float range[] =
	{
		-1.57f,		// ��
		1.57f,		// �E
		D3DX_PI,	// ��
		0.0f,		// ��
	};

	if (eOutPos == CPlayerManager::EOutPos::OUT_LEFT ||
		eOutPos == CPlayerManager::EOutPos::OUT_RIGHT)
	{
		if (!UtilFunc::Collision::CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
		{// ������
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
		{// �����
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
		{// ������
			if (fDirection == range[2])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
			}
			else if (fDirection == range[3])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
			}
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
		{// �E����
			if (fDirection == range[2])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
			}
			else if (fDirection == range[3])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
			}
			bRot = true;
		}
		else
		{// �����������
			MyAssert::CustomAssert(false, "�J�j�����F�ǂ����Ă�˂�");
		}
	}
	else
	{
		if (!UtilFunc::Collision::CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
		{// ������
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
		{// �����
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
		{// ������
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
		{// �E����
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
			bRot = true;
		}
		else
		{// �����������
			MyAssert::CustomAssert(false, "�J�j�����F�ǂ����Ă�˂�");
		}
	}

	
	if (fDirection == range[0])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}
	else if (fDirection == range[1])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
	}
	else if (fDirection == range[2])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
	}
	else if (fDirection == range[3])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}

	if (playerDir == CPlayer::CRAB_DIRECTION::CRAB_NONE ||
		inputDir == CPlayer::CRAB_DIRECTION::CRAB_NONE)
	{// ����Ɉ����������Ă��Ȃ�
		return;
	}

	// ���[�V�����ݒ�
	GetPlayer()->SetMotion(Crab::MOTION_WALK[playerDir][inputDir], nStartKey);
}

//==========================================================================
// �J�j
//==========================================================================
bool CPlayerAIOut::IsCrab()
{
	// �v���C���[���̎擾
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return false;

	// ���E�Ƀ{�[�������邩
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// �{�[�����
	if (pBall == nullptr) { return false; }

	return true;
}

//==========================================================================
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerAIOut::Hit(CBall* pBall)
{
	// ���N���X�̃q�b�g
	CPlayer::SHitInfo hitInfo = CPlayerOut::Hit(pBall);	// �q�b�g���

#if 1
	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // �ʏ���
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // �L���b�`���
		// �������[�h
		m_pAIOutControl->SetMode(CPlayerAIOutControl::EMode::MODE_THROW);
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

	CPlayerAIOutControlAction* pControlAction = dynamic_cast<CPlayerAIOutControlAction*>(GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// ���������t���O�ݒ�
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}
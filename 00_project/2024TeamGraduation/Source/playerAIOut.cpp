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
#include "playerAIOut_control.h"

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
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
		bRot = true;
		col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{// �����������
		MyAssert::CustomAssert(false, "�J�j�����F�ǂ����Ă�˂�");
	}

	//--------------------------------
	// ����
	//--------------------------------
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �i�s�����̎擾
	float fDirection = pControlAIOutMove->GetClabDirection() + D3DX_PI;

	if (!UtilFunc::Collision::CollisionRangeAngle(fDirection, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// ������
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// �����
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// ������
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
	{// �E����
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
	}
	else
	{// �����������
		MyAssert::CustomAssert(false, "�J�j�����F�ǂ������Ă�˂�");
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

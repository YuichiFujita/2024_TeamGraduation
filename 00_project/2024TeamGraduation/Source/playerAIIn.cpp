//==========================================================================
// 
//  AI�v���C���[���� [playerAIIn.cpp]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================
#include "playerAIIn.h"
#include "manager.h"
#include "camera.h"

// �g�p�N���X
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIIn::CPlayerAIIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerIn(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_AI)
{
	// ���쑀��̊���
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	// AI�R���g���[��(����)�̐���
	m_pAIControl = CPlayerAIControl::Create(pPlayer);
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIIn::~CPlayerAIIn()
{
	if (m_pAIControl)
	{
		delete m_pAIControl;
		m_pAIControl = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���N���X�̍X�V
	CPlayerIn::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// AI�R���g���[��(����)�̍X�V
	if (m_pAIControl)
	{
		m_pAIControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �J�j�������[�V�����ݒ�
//==========================================================================
void CPlayerAIIn::MotionCrab(int nStartKey)
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
	// ����
	//--------------------------------
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �i�s�����̎擾
	float fDirection = pControlAIMove->GetClabDirection() + D3DX_PI;

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
CPlayer::SHitInfo CPlayerAIIn::Hit(CBall* pBall)
{
	// ���N���X�̃q�b�g
	CPlayer::SHitInfo hitInfo = CPlayerIn::Hit(pBall);	// �q�b�g���

	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // �ʏ���
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // �L���b�`���
		// �������[�h
		//m_pAIControl->SetMode(CPlayerAIControl::EMode::MODE_THROW);
	}

	return hitInfo;
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CPlayerAIIn::Debug()
{
#if _DEBUG
	// ���N���X�̃f�o�b�O�\��
	CPlayerIn::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*>(GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// ���������t���O�ݒ�
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}

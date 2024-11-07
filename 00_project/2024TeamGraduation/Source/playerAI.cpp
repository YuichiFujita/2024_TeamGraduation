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

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
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
	CPlayer::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ����
	MyLib::Vector3 rot = GetRotation();

	float fRange = D3DX_PI * 0.25f;

	float fRangeMinMax[] =
	{
		D3DX_PI * 1.0f + fRange,	// ��
		D3DX_PI * 1.0f - fRange,	// ��
		D3DX_PI * 0.0f + fRange,	// ��
		D3DX_PI * 0.0f - fRange,	// ��
		D3DX_PI * -0.5f + fRange,	// �E
		D3DX_PI * -0.5f - fRange,	// �E
		D3DX_PI * 0.5f + fRange,	// ��
		D3DX_PI * 0.5f - fRange,	// ��
	};

	for (int i = 0; i < sizeof(fRangeMinMax) / sizeof(fRangeMinMax[0]); i++)
	{
		UtilFunc::Transformation::RotNormalize(fRangeMinMax[i]);

		MyLib::Vector3 move(
			sinf(fRangeMinMax[i] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[i] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}

	{
		MyLib::Vector3 move(
			sinf(fRangeMinMax[0] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[0] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}

	{
		MyLib::Vector3 move(
			sinf(fRangeMinMax[1] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[1] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
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
// ���쏈��
//==========================================================================
void CPlayerAI::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// �ړ�����	// TODO�FAI�ނ��`
	//m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	delete pControlMove;
	pControlMove = control;

	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	delete pControlAction;
	pControlAction = control;

	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayerAI::DeleteControl()
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


//==========================================================================
// �f�o�b�O����
//==========================================================================
void CPlayerAI::Debug()
{
#if _DEBUG
	// �f�o�b�O����
	CPlayer::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*> (GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// ���������t���O�ݒ�
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{ 
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}
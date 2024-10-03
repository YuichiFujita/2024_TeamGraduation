//=============================================================================
// 
// �g�k�n���h������ [handle_Scale.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "handle_Scale.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"
#include "camera.h"
#include "objectLine.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHandle_Scale::CHandle_Scale(int nPriority) : CHandle(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHandle_Scale::~CHandle_Scale()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CHandle_Scale::Init()
{

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// ����������
	CHandle::Init();

	return S_OK;
}

//==========================================================================
// �͂ݒ�����
//==========================================================================
void CHandle_Scale::Grab()
{
	CInputMouse* pMouse = CInputMouse::GetInstance();

	// �h���b�O�擾
	m_DiffScale = 0.0f;
	m_DiffScale = pMouse->GetScreenDiffFactor();

	if (pMouse->GetDeltaX() < 0) {
		m_DiffScale *= -1;
	}



	// �Ĉړ���
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	// ���Z�b�g
	m_DiffMove = 0.0f;

	// �ړ��̍���
	m_DiffMove = diffpos;

}

//==========================================================================
// �ړ������擾
//==========================================================================
CHandle::SEditHandleInfo CHandle_Scale::GetDiff(HandleAngle angle)
{
	CHandle::SEditHandleInfo info;
	info.scale = m_DiffMove * 0.001f;

	switch (angle)
	{
	case CHandle::ANGLE_Z:
		info.scale.x = 0.0f;
		info.scale.y = 0.0f;
		break;

	case CHandle::ANGLE_Y:
		info.scale.x = 0.0f;
		info.scale.z = 0.0f;
		break;

	case CHandle::ANGLE_X:
		info.scale.y = 0.0f;
		info.scale.z = 0.0f;
		break;

	default:
		info.scale = m_DiffScale;
		break;
	}

	return info;
}

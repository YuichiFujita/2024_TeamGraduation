//=============================================================================
// 
// �ړ��n���h������ [handle_Move.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "handle_Move.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"
#include "camera.h"
#include "objectLine.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHandle_Move::CHandle_Move(int nPriority) : CHandle(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHandle_Move::~CHandle_Move()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CHandle_Move::Init()
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
void CHandle_Move::Grab()
{
	CInputMouse* pMouse = CInputMouse::GetInstance();

	// �Ĉړ���
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	// ���Z�b�g
	m_DiffMove = 0.0f;

	// �ړ��̍���
	m_DiffMove = diffpos;
	m_DiffMove.y *= m_scale;
}

//==========================================================================
// �ړ������擾
//==========================================================================
CHandle::SEditHandleInfo CHandle_Move::GetDiff(HandleAngle angle)
{
	CHandle::SEditHandleInfo info;
	info.pos = m_DiffMove;

	switch (angle)
	{
	case CHandle::ANGLE_Z:
		info.pos.x = 0.0f;
		info.pos.y = 0.0f;
		break;

	case CHandle::ANGLE_Y:
		info.pos.x = 0.0f;
		info.pos.z = 0.0f;
		break;

	case CHandle::ANGLE_X:
		info.pos.y = 0.0f;
		info.pos.z = 0.0f;
		break;

	default:
		info.pos.y = 0.0f;

		{
			CInputMouse* pMouse = CInputMouse::GetInstance();
			info.pos.x = pMouse->GetWorldPosition().x - GetPosition().x;
			info.pos.z = pMouse->GetWorldPosition().z - GetPosition().z;
		}
		break;
	}

	return info;
}

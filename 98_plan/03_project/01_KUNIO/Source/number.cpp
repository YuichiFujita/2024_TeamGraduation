//=============================================================================
// 
//  �������� [number.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "number.h"

// �h����
#include "number_2D.h"
#include "number_3D.h"
#include "number_Billboard.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CNumber::CNumber(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	// �l�̃N���A
	m_objType = OBJECTTYPE_2D;	// �I�u�W�F�N�g�̎��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CNumber::~CNumber()
{

}

//==========================================================================
// ��������
//==========================================================================
CNumber* CNumber::Create(EObjectType objtype, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CNumber* pNumber = nullptr;

	// �������̊m��
	switch (objtype)
	{
	case CNumber::OBJECTTYPE_2D:
		pNumber = DEBUG_NEW CNumber2D(nPriority);
		break;

	case CNumber::OBJECTTYPE_3D:
		pNumber = DEBUG_NEW CNumber3D(nPriority);
		break;

	case CNumber::OBJECTTYPE_BILLBOARD:
		pNumber = DEBUG_NEW CNumberBillboard(nPriority);
		break;
	}

	if (pNumber != nullptr)
	{
		// �I�u�W�F�N�g�̎��
		pNumber->m_objType = objtype;

		// ����������
		pNumber->Init();
	}

	return pNumber;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CNumber::Init()
{
	
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CNumber::Uninit()
{
	Release();
}

//==========================================================================
// �폜
//==========================================================================
void CNumber::Kill()
{
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CNumber::Update()
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CNumber::Draw()
{
	
}

//==========================================================================
// ��ސݒ�
//==========================================================================
void CNumber::SetType(const CObject::TYPE type)
{
	CObject::SetType(type);
}
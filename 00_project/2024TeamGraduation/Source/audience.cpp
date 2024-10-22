#if 0
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
void CNumber::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

//==========================================================================
// �����̐����ݒ�
//==========================================================================
void CNumber::SetNum(int num)
{
	// TODO:�ق�Ƃɂ���Ȃ��Ƃ��đ��v�H

	float fWidthRate	= 1.0f / 10;		// ���̕������̊���
	float fHeightRate	= 1.0f / 1;			// �c�̕������̊���
	int nWidthCurrent	= num % 10;			// ���݂̉��̃p�^�[��
	int nHeightCurrent	= (num / 10) % 1;	// ���݂̏c�̃p�^�[��

	// �e�N�X�`�����W�̐ݒ�
	D3DXVECTOR2 tex[4];
	tex[0] = D3DXVECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate *  nHeightCurrent);
	tex[1] = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate *  nHeightCurrent);
	tex[2] = D3DXVECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate * (nHeightCurrent + 1));
	tex[3] = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate * (nHeightCurrent + 1));

	m_nNum = num;
	SetTex(&tex[0]);
}
#endif

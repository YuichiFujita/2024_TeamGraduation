//=============================================================================
// 
//  �N������ [course_title.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "course_title.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "convert.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-04.jpg";

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCourse_Title::CCourse_Title(int nPriority, const LAYER layer) : CCourse(nPriority, layer)
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCourse_Title::~CCourse_Title()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCourse_Title::Init()
{
	HRESULT hr;

	// �g�̍���
	m_fWaveHeight = 20.0f;

	// ��_
	m_vecSegmentPosition.clear();
	m_vecSegmentPosition.push_back(MyLib::Vector3(-15000.0f, 0.0f, 0.0f));
	m_vecSegmentPosition.push_back(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	m_vecSegmentPosition.push_back(MyLib::Vector3(15000.0f, 0.0f, 0.0f));
	m_vecSegmentPosition.push_back(MyLib::Vector3(50000.0f, 0.0f, 0.0f));


	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �I�u�W�F�N�g3D���b�V���̏���������
	Reset();

	// ���_���ݒ�
	SetVtx();


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// �S�Ă̗v�f����������
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));



	// ���_���W�v�Z
	SetVtxPosition();

	SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCourse_Title::Uninit()
{
	// �I������
	CCourse::Uninit();
}

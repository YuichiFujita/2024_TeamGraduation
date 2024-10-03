//=============================================================================
// 
//  �T���v��_�I�u�W�F�N�g3D���� [sample_obj3D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "sample_obj3D.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// �e�N�X�`���̃t�@�C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSample_Obj3D::CSample_Obj3D(int nPriority) : CObject3D(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSample_Obj3D::~CSample_Obj3D()
{

}

//==========================================================================
// ��������
//==========================================================================
CSample_Obj3D* CSample_Obj3D::Create()
{
	// �������̊m��
	CSample_Obj3D* pObj = DEBUG_NEW CSample_Obj3D;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSample_Obj3D::Init()
{

	// �I�u�W�F�N�g3D�̏�����
	CObject3D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	SetSizeOrigin(setsize);


	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSample_Obj3D::Uninit()
{
	// �I������
	CObject3D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSample_Obj3D::Update()
{
	// �X�V����
	CObject3D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSample_Obj3D::Draw()
{
	// �`�揈��
	CObject3D::Draw();
}

//=============================================================================
// 
//  �K���X����鏈�� [sample_obj2D.cpp]
//  Author : �����V����l
// 
//=============================================================================
#include "glassclush.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "renderer.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\result\\grass_crush.png";	// �e�N�X�`���̃t�@�C��
}

CGlassclush *CGlassclush::m_ThisPtr = nullptr;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGlassclush::CGlassclush(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGlassclush::~CGlassclush()
{

}

//==========================================================================
// �폜
//==========================================================================
void CGlassclush::Kill()
{
	if (m_ThisPtr != nullptr)
	{
		m_ThisPtr->Uninit();
		m_ThisPtr = nullptr;
	}
}

//==========================================================================
// ��������
//==========================================================================
CGlassclush* CGlassclush::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CGlassclush;

		if (m_ThisPtr != nullptr)
		{
			// ����������
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGlassclush::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

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
	SetSize(D3DXVECTOR2(640.0f,360.0f));
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(640.0f,360.0f,0.0f));

	SetAlpha(0.0f);

	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �u���[�̍폜
	CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(0.0f, 0.0f, 60.0f);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGlassclush::Uninit()
{
	// �I������
	CObject2D::Uninit();

	m_ThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CGlassclush::Update()
{
	float alpha = GetAlpha();

	alpha += 0.1f;

	SetAlpha(alpha);

	if (alpha >= 1.0f)
	{
		SetAlpha(1.0f);
	}

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CGlassclush::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

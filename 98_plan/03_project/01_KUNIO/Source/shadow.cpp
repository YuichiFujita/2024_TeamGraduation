//=============================================================================
// 
//  �e���� [shadow.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "elevation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define POS_SHADOW	(50.0f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CShadow::m_nNumAll = 0;						// �e�̑���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{

	m_pObject = nullptr;	// �I�u�W�F�N�g�̃|�C���^
	m_nNumAll = 0;		// ����
	m_nTexIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CShadow::~CShadow()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CShadow *CShadow::Create()
{
	// �����p�̃I�u�W�F�N�g
	CShadow *pShadow = nullptr;

	if (pShadow == nullptr)
	{// nullptr��������

		// �������̊m��
		pShadow = DEBUG_NEW CShadow;

		//if (pShadow->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pShadow;
		//	return nullptr;
		//}

		if (pShadow != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pShadow->Init();
		}

		return pShadow;
	}

	return nullptr;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CShadow *CShadow::Create(MyLib::Vector3 pos, float size)
{
	// �����p�̃I�u�W�F�N�g
	CShadow *pShadow = nullptr;

	if (pShadow == nullptr)
	{// nullptr��������

		// �������̊m��
		pShadow = DEBUG_NEW CShadow;

		//if (pShadow->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pShadow;
		//	return nullptr;
		//}

		if (pShadow != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			pShadow->SetPosition(pos);
			pShadow->SetSize(MyLib::Vector3(size, 0.0f, size));	// �T�C�Y

			// ����������
			pShadow->Init();
		}
		return pShadow;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CShadow::Init()
{
	HRESULT hr;

	// �e��ϐ��̏�����
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));	// �F

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// ����������
	hr = CObject3D::Init();

	if (FAILED(hr))
	{// ���s�����Ƃ�

		return E_FAIL;
	}

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\effect\\effect000.jpg");

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CShadow::Uninit()
{
	// �I������
	CObject3D::Uninit();

	// �������Z
	m_nNumAll--;
}

//==========================================================================
// �X�V����
//==========================================================================
void CShadow::Update()
{
	// ���S�̔���
	if (IsDeath())
	{// ���S�t���O�������Ă�����
		return;
	}

	/*if (!CGame::GetInstance()->GetElevation()->IsChange())
	{
		return;
	}*/

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �����擾
	bool bLand = false;
	pos.y = CGame::GetInstance()->GetElevation()->GetHeight(pos, &bLand);

	// �ʒu�ݒ�
	SetPosition(pos);

	// ���_���ݒ�
	CObject3D::SetVtx();

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\effect\\effect000.jpg");

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CShadow::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0); 

	// �`�揈��
	CObject3D::Draw();

	// ���Z���������Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// �����擾
//==========================================================================
int CShadow::GetNumAll()
{
	return m_nNumAll;
}

//=============================================================================
// 
//  ���b�V���h�[������ [meshdome.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "meshdome.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE		"data\\TEXTURE\\field002.png"
#define POS_MESHDOME	(95000.0f)
//#define POS_MESHDOME	(85000.0f)
#define POS_MESHDOME_Y	(POS_MESHDOME)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMeshDome::CMeshDome(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_fMove = 0.0f;	// ��]�ړ���
	m_nTexIdx = 0;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMeshDome::~CMeshDome()
{

}

//==========================================================================
// ��������
//==========================================================================
CMeshDome *CMeshDome::Create()
{
	// �����p�̃I�u�W�F�N�g
	CMeshDome *pObjMeshCylinder = nullptr;

	if (pObjMeshCylinder == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjMeshCylinder = DEBUG_NEW CMeshDome;

		//if (pObjMeshCylinder->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pObjMeshCylinder;
		//	return nullptr;
		//}

		if (pObjMeshCylinder != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu�E����
			pObjMeshCylinder->SetWidthBlock(32);
			pObjMeshCylinder->SetHeightBlock(8);
			pObjMeshCylinder->SetWidthLen(POS_MESHDOME);
			pObjMeshCylinder->SetHeightLen(POS_MESHDOME_Y);

			// �e�N�X�`���̊��蓖��
			pObjMeshCylinder->m_nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);

			// �e�N�X�`���̊��蓖��
			pObjMeshCylinder->BindTexture(pObjMeshCylinder->m_nTexIdx);

			// ����������
			pObjMeshCylinder->Init();
		}

		return pObjMeshCylinder;
	}

	return nullptr;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CMeshDome *CMeshDome::Create(const float fMove, const char *aFileName, const int nWidth, const int nHeight, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CMeshDome *pObjMeshCylinder = nullptr;

	if (pObjMeshCylinder == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjMeshCylinder = DEBUG_NEW CMeshDome(nPriority);

		//if (pObjMeshCylinder->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pObjMeshCylinder;
		//	return nullptr;
		//}

		if (pObjMeshCylinder != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu�E����
			pObjMeshCylinder->m_fMove = fMove;
			pObjMeshCylinder->SetWidthBlock(nWidth);
			pObjMeshCylinder->SetHeightBlock(nHeight);
			pObjMeshCylinder->SetWidthLen(POS_MESHDOME);
			pObjMeshCylinder->SetHeightLen(POS_MESHDOME_Y);

			if (aFileName == nullptr)
			{// nullptr��������

				// �e�N�X�`���̊��蓖��
				pObjMeshCylinder->m_nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
			}
			else
			{// �t�@�C�����������Ă�����

				// �e�N�X�`���̊��蓖��
				pObjMeshCylinder->m_nTexIdx = CTexture::GetInstance()->Regist(aFileName);
			}

			// �e�N�X�`���̊��蓖��
			pObjMeshCylinder->BindTexture(pObjMeshCylinder->m_nTexIdx);

			// ����������
			pObjMeshCylinder->Init();
		}

		return pObjMeshCylinder;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMeshDome::Init()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ��ސݒ�
	SetType(TYPE_MESHDOME);

	// �I�u�W�F�N�g3D���b�V���̏���������
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_DOME);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	SetPosition(MyLib::Vector3(0.0f, -1500.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMeshDome::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMeshDome::Update()
{
	MyLib::Vector3 rot = GetRotation();	// ����

	// ��]
	rot.y += m_fMove;

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(rot.y);

	// �����ݒ�
	SetRotation(rot);

	// ���_���ݒ�
	SetVtx();

}

//==========================================================================
// �`�揈��
//==========================================================================
void CMeshDome::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �`�揈��
	CObject3DMesh::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CMeshDome::SetVtx()
{
	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ���b�V���h�[���I�u�W�F�N�g�̎擾
//==========================================================================
CMeshDome *CMeshDome::GetMyObject()
{
	return this;
}

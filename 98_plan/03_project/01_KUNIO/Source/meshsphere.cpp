//=============================================================================
// 
//  ���������� [meshsphere.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "meshsphere.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "objectchara.h"
#include "calculation.h"
#include "particle.h"
#include "debugproc.h"
#include "impactwave.h"
#include "elevation.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define TEXTURE		"data\\TEXTURE\\glass.jpg"	// �e�N�X�`��

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMeshSphere::CMeshSphere(int nPriority) : CObject3DMesh(nPriority)
{
	m_nTexIdx = 0;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMeshSphere::~CMeshSphere()
{

}

//==========================================================================
// ��������
//==========================================================================
CMeshSphere *CMeshSphere::Create(MyLib::Vector3 pos, float fSize, const char *pFileName, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CMeshSphere *pObjMeshField = nullptr;

	if (pObjMeshField == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjMeshField = DEBUG_NEW CMeshSphere(nPriority);

		if (pObjMeshField != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �e�N�X�`���̊��蓖��
			pObjMeshField->m_nTexIdx = CTexture::GetInstance()->Regist(pFileName);

			// �e�N�X�`���̊��蓖��
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// �ʒu�E����
			pObjMeshField->SetPosition(pos);
			pObjMeshField->SetWidthBlock(16);
			pObjMeshField->SetHeightBlock(16);
			pObjMeshField->SetWidthLen(fSize);
			pObjMeshField->SetHeightLen(fSize);

			// ����������
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return nullptr;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CMeshSphere *CMeshSphere::Create(MyLib::Vector3 pos, float fSize, int nTexIdx, int nPriority,int nBlock)
{
	// �����p�̃I�u�W�F�N�g
	CMeshSphere *pObjMeshField = nullptr;

	if (pObjMeshField == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjMeshField = DEBUG_NEW CMeshSphere(nPriority);

		if (pObjMeshField != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �e�N�X�`���̊��蓖��
			pObjMeshField->m_nTexIdx = nTexIdx;

			// �e�N�X�`���̊��蓖��
			pObjMeshField->BindTexture(pObjMeshField->m_nTexIdx);

			// �ʒu�E����
			pObjMeshField->SetPosition(pos);
			pObjMeshField->SetWidthBlock(nBlock);
			pObjMeshField->SetHeightBlock(nBlock);
			pObjMeshField->SetWidthLen(fSize);
			pObjMeshField->SetHeightLen(fSize);

			// ����������
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMeshSphere::Init()
{
	HRESULT hr;

	// ��ސݒ�
	SetType(TYPE_MESHSPHERE);

	// �I�u�W�F�N�g3D���b�V���̏���������
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_SPHERE);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMeshSphere::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMeshSphere::Update()
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �F�擾
	D3DXCOLOR col = GetColor();

	// �������擾
	float fWidthLen = GetWidthLen();

	// �c�����擾
	float fHeightLen = GetHeightLen();

	// �����␳
	UtilFunc::Correction::InertiaCorrection(fWidthLen, m_fSizeDest, 0.15f);
	UtilFunc::Correction::InertiaCorrection(fHeightLen, m_fSizeDest, 0.15f);

	// �����ݒ�
	SetWidthLen(fWidthLen);
	SetHeightLen(fHeightLen);

	// ���_���W�X�V
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMeshSphere::Draw()
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObject3DMesh::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// �ڕW�̃T�C�Y�ݒ�
//==========================================================================
void CMeshSphere::SetSizeDest(float fSize)
{
	m_fSizeDest = fSize;
}

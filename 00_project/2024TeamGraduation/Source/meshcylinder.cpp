//=============================================================================
// 
//  ���b�V���V�����_�[���� [meshcylinder.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define POS_MESHCYLINDER	(5000.0f)
#define POS_MESHCYLINDER_Y	(3500.0f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMeshCylinder::CMeshCylinder(int nPriority) : CObject3DMesh(nPriority)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//==========================================================================
// ��������
//==========================================================================
CMeshCylinder* CMeshCylinder::Create()
{
	// �������̊m��
	CMeshCylinder* pObjMeshCylinder = DEBUG_NEW CMeshCylinder;

	if (pObjMeshCylinder != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�E����
		pObjMeshCylinder->SetWidthBlock(16);
		pObjMeshCylinder->SetHeightBlock(1);
		pObjMeshCylinder->SetWidthLen(POS_MESHCYLINDER);
		pObjMeshCylinder->SetHeightLen(POS_MESHCYLINDER_Y);

		// ����������
		pObjMeshCylinder->Init();
	}

	return pObjMeshCylinder;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CMeshCylinder *CMeshCylinder::Create(const char *aFileName)
{
	// �����p�̃I�u�W�F�N�g
	CMeshCylinder *pObjMeshCylinder = nullptr;

	if (pObjMeshCylinder == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjMeshCylinder = DEBUG_NEW CMeshCylinder;

		//if (pObjMeshCylinder->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pObjMeshCylinder;
		//	return nullptr;
		//}

		if (pObjMeshCylinder != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu�E����
			pObjMeshCylinder->SetWidthBlock(32);
			pObjMeshCylinder->SetHeightBlock(1);
			pObjMeshCylinder->SetWidthLen(POS_MESHCYLINDER);
			pObjMeshCylinder->SetHeightLen(POS_MESHCYLINDER_Y);

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
HRESULT CMeshCylinder::Init()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ��ސݒ�
	SetType(TYPE_MESHCYLINDER);

	// �I�u�W�F�N�g3D���b�V���̏���������
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_CYLINDER);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMeshCylinder::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMeshCylinder::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMeshCylinder::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//// �e�N�X�`���̌J��Ԃ������ɖ߂�
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObject3DMesh::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// �e�N�X�`���̌J��Ԃ������ɖ߂�
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CMeshCylinder::SetVtx()
{
	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ���b�V���V�����_�[�I�u�W�F�N�g�̎擾
//==========================================================================
CMeshCylinder *CMeshCylinder::GetMyObject()
{
	return this;
}

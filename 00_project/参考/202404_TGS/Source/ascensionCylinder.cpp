//=============================================================================
// 
//  �ڕW�n�_���� [ascensionCylinder.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "ascensionCylinder.h"
#include "input.h"
#include "calculation.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "elevation.h"
#include "game.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\GRADATION\\orange_02.jpg";
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CAscensionCylinder::CAscensionCylinder(int nPriority) : CMeshCylinder(nPriority)
{
	

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CAscensionCylinder::~CAscensionCylinder()
{

}

//==========================================================================
// ��������
//==========================================================================
CAscensionCylinder* CAscensionCylinder::Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen)
{
	// �������̊m��
	CAscensionCylinder* pObjMeshCylinder = DEBUG_NEW CAscensionCylinder;

	if (pObjMeshCylinder != nullptr)
	{
		// �ʒu�E����
		pObjMeshCylinder->SetPosition(pos);
		pObjMeshCylinder->SetWidthBlock(16);
		pObjMeshCylinder->SetHeightBlock(1);
		pObjMeshCylinder->SetWidthLen(fWidthLen);
		pObjMeshCylinder->SetHeightLen(fHeightLen);

		// ����������
		pObjMeshCylinder->Init();
	}

	return pObjMeshCylinder;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CAscensionCylinder::Init()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ��ސݒ�
	SetType(CObject::TYPE_OBJECT3D);

	// ���b�V���V�����_�[�̏���������
	CMeshCylinder::Init();

	// �e�N�X�`���̊��蓖��
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CAscensionCylinder::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CAscensionCylinder::Update()
{

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CAscensionCylinder::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �w�ʂ̃J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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

	// �J�����O�f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================================
// 
//  �ڕW�n�_���� [targetpoint.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "targetpoint.h"
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
#define TEXTURE		"data\\TEXTURE\\gradation004.jpg"
#define MOVE_SPEED		(1.7f)							// �ړ����x

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTargetPoint::CTargetPoint(int nPriority) : CObject3DMesh(nPriority)
{
	m_nTexIdx = 0;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nAlphaCnt = 0;	// �s�����x�̃J�E���g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTargetPoint::~CTargetPoint()
{

}

//==========================================================================
// ��������
//==========================================================================
CTargetPoint *CTargetPoint::Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen)
{
	// �����p�̃I�u�W�F�N�g
	CTargetPoint *pObjMeshCylinder = nullptr;

	if (pObjMeshCylinder == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjMeshCylinder = DEBUG_NEW CTargetPoint;

		//if (pObjMeshCylinder->GetID() < 0)
		//{// �������m�ۂɎ��s���Ă�����

		//	delete pObjMeshCylinder;
		//	return nullptr;
		//}

		if (pObjMeshCylinder != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu�E����
			pObjMeshCylinder->SetPosition(pos);
			pObjMeshCylinder->SetWidthBlock(16);
			pObjMeshCylinder->SetHeightBlock(1);
			pObjMeshCylinder->SetWidthLen(fWidthLen);
			pObjMeshCylinder->SetHeightLen(fHeightLen);

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
// ����������
//==========================================================================
HRESULT CTargetPoint::Init()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ��ސݒ�
	SetType(CObject::TYPE_OBJECT3D);

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
void CTargetPoint::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTargetPoint::Update()
{

	// �F�擾
	D3DXCOLOR col = GetColor();

	// �񎟋Ȑ��␳
	UtilFunc::Correction::CuadricCurveComp(col.a, 100, 0.3f, 1.0f, m_nAlphaCnt);
	//col.a = 0.3f;

	// �F�ݒ�
	SetColor(col);

	Control();

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// ����
//==========================================================================
void CTargetPoint::Control()
{

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̌����擾
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	if (pInputKeyboard->GetPress(DIK_A) || pInputGamepad->GetStickMoveL(0).x < 0)
	{//���L�[�������ꂽ,���ړ�

		if (pInputKeyboard->GetPress(DIK_W) || pInputGamepad->GetStickMoveL(0).y > 0)
		{//A+W,����ړ�
			move.x += sinf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else if (pInputKeyboard->GetPress(DIK_S) || pInputGamepad->GetStickMoveL(0).y < 0)
		{//A+S,�����ړ�
			move.x += sinf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else
		{//A,���ړ�
			move.x += sinf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) || pInputGamepad->GetStickMoveL(0).x > 0)
	{//D�L�[�������ꂽ,�E�ړ�

		if (pInputKeyboard->GetPress(DIK_W) || pInputGamepad->GetStickMoveL(0).y > 0)
		{//D+W,�E��ړ�
			move.x += sinf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else if (pInputKeyboard->GetPress(DIK_S) || pInputGamepad->GetStickMoveL(0).y < 0)
		{//D+S,�E���ړ�
			move.x += sinf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else
		{//D,�E�ړ�
			move.x += sinf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) || pInputGamepad->GetStickMoveL(0).y > 0)
	{//W�������ꂽ�A��ړ�
		move.x += sinf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * MOVE_SPEED;
		move.z += cosf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * MOVE_SPEED;
	}
	else if (pInputKeyboard->GetPress(DIK_S) || pInputGamepad->GetStickMoveL(0).y < 0)
	{//S�������ꂽ�A���ړ�
		move.x += sinf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * MOVE_SPEED;
		move.z += cosf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * MOVE_SPEED;
	}


	// �d�͏���
	move.y -= mylib_const::GRAVITY;

	// �ʒu�X�V
	pos += move;

	// �����␳
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	// �����擾
	bool bLand = false;
	pos.y = CGame::GetInstance()->GetElevation()->GetHeight(pos, &bLand);

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);
}


//==========================================================================
// �`�揈��
//==========================================================================
void CTargetPoint::Draw()
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

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CTargetPoint::SetVtx()
{
	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// �ڕW�n�_�I�u�W�F�N�g�̎擾
//==========================================================================
CTargetPoint *CTargetPoint::GetObject3DMesh()
{
	return this;
}

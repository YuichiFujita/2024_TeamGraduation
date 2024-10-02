//============================================================
//
//	�e���� [gun.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gun.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"

//************************************************************
//	�q�N���X [CGun] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGun::CGun() : CObject3D(CObject::LABEL_GUN),
	m_pParent(nullptr)	// �e�̏��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGun::~CGun()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGun::Init(void)
{
	// �����o�ϐ���������
	m_pParent = nullptr;	// �e�̏��

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	SetVec3Sizing(D3DXVECTOR3(17.0f, 7.0f, 0.0f));
	SetOrigin(ORIGIN_DOWN);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGun::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGun::Update(const float fDeltaTime)
{
	// TODO�F�e���C����
	if (GET_INPUTMOUSE->IsTrigger(CInputMouse::KEY_LEFT))
	{
		float fGunRot = m_pParent->GetGunRotation().z;
		CBullet::Create(m_pParent->GetVec3Position(), D3DXVECTOR3(sinf(fGunRot + HALF_PI), -cosf(fGunRot + HALF_PI), 0.0f) * 29.0f);
		m_pParent->MoveGunRecoil(11.5f);
	}

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGun::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CGun *CGun::Create(CPlayer* pParent)
{
	// �e�̐���
	CGun *pGun = new CGun;
	if (pGun == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�̏�����
		if (FAILED(pGun->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �e�̔j��
			SAFE_DELETE(pGun);
			return nullptr;
		}

		// �e�v���C���[�̕ۑ�
		pGun->m_pParent = pParent;

		// �m�ۂ����A�h���X��Ԃ�
		return pGun;
	}
}

//============================================================
//	�`��}�g���b�N�X�̌v�Z����
//============================================================
void CGun::CalcDrawMatrix(void)
{
	D3DXVECTOR3 offset = D3DXVECTOR3(35.0f, 0.0f, 0.0f);	// �I�t�Z�b�g
	D3DXMATRIX *pMtxWorld = GetPtrMtxWorld();			// ���[���h�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxOffset;				// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 pos = m_pParent->GetVec3Position();		// �v���C���[�ʒu
	D3DXVECTOR3 rot = m_pParent->GetVec3Rotation();		// �v���C���[����
	D3DXVECTOR3 rotGun = m_pParent->GetGunRotation();	// �e�\������

	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 off = D3DXVECTOR3(0.0f, 15.0f, 0.0f);	// �I�t�Z�b�g

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �I�t�Z�b�g�𔽉f
	D3DXMatrixTranslation(&mtxOffset, off.x, off.y, off.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxOffset);

	// �v���C���[�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// �v���C���[�̒��S���W���v�Z
	pos = useful::GetMatrixPosition(mtxWorld);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pMtxWorld);

	// �I�t�Z�b�g�𔽉f
	D3DXMatrixTranslation(&mtxOffset, offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxOffset);

	// �v���C���[�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// �e�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotGun.y, rotGun.x, rotGun.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);
}

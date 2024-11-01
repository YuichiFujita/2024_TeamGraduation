//=============================================================================
// 
//  ���f������ [model.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "calculation.h"
#include "texture.h"
#include "Xload.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CModel::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CModel::CModel(int nPriority)
{
	D3DXMatrixIdentity(&m_mtxWorld);				// ���[���h�}�g���b�N�X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_scale = mylib_const::DEFAULT_SCALE;			// �X�P�[��
	m_nIdxTexure = NULL;							// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nIdxXFile = 0;								// X�t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxParent = -1;		// �e�̃C���f�b�N�X�ԍ�
	m_pParent = NULL;								// �e���f���̃|�C���^
	m_pModel = this;								// �����̃|�C���^
	m_nNumAll++;									// �������Z
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CModel::~CModel()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CModel::BindTexture(int *nIdx)
{
	// ���蓖�Ă�
	m_nIdxTexure = nIdx;
}

//==========================================================================
// X�t�@�C���̃f�[�^���蓖��
//==========================================================================
void CModel::BindXData(int nIdxXFile)
{
	// ��񊄂蓖��
	m_nIdxXFile = nIdxXFile;
}

//==========================================================================
// �e�̃}�g���b�N�X�ݒ�
//==========================================================================
void CModel::SetParent(CModel *pModel, int nIdx)
{
	// �e�̃|�C���^��n��
	m_pParent = pModel;
	m_nIdxParent = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CModel *CModel::Create(const char *pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �����p�̃I�u�W�F�N�g
	CModel *pModel = NULL;

	if (pModel == NULL)
	{// NULL��������

		// �������̊m��
		pModel = DEBUG_NEW CModel;

		if (pModel != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pModel->Init(pFileName);

			if (FAILED(hr))
			{// ���s���Ă�����
				return NULL;
			}

			// �ʒu�E����
			pModel->SetPosition(pos);
			pModel->m_posOrigin = pos;
			pModel->SetOriginPotision(pos);
			pModel->SetRotation(rot);
			pModel->SetOriginRotation(rot);
		}

		return pModel;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CModel::Init(const char *pFileName)
{
	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CManager::GetXLoad();

	// X�t�@�C���̃��[�h
	m_nIdxXFile = pXLoad->XLoad(pFileName);

	// X�t�@�C���̃f�[�^���蓖��
	BindXData(m_nIdxXFile);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// �S���_�`�F�b�N
	CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	// �X�P�[�����Z�b�g
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_modelname = extractFileName(pFileName);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CModel::Uninit(void)
{
	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	if (m_nIdxTexure != NULL)
	{
		m_nIdxTexure = NULL;
	}

	// �e���f���̃|�C���^
	if (m_pParent != NULL)
	{
		m_pParent = NULL;
	}

	if (m_pModel != NULL)
	{// �������̊m�ۂ�����Ă�����

		// �������̊J��
		delete m_pModel;
		m_pModel = NULL;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CModel::Update(void)
{
	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CManager::GetXLoad();

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// �S���_�`�F�b�N
	CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CModel::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �}�g���b�N�X�v�Z
	CalWorldMtx();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �������K�����f�t�H���g�ɂ���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CModel::Draw(D3DXCOLOR col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �}�g���b�N�X�v�Z
	CalWorldMtx();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL matNow;			// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = col;
	matNow.MatD3D.Ambient = col;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matNow.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �������K�����f�t�H���g�ɂ���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CModel::Draw(float fAlpha)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �}�g���b�N�X�v�Z
	CalWorldMtx();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL matNow;			// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));


	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �s�����x�ݒ�
		matNow.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, fAlpha);
		matNow.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, fAlpha);
		matNow.MatD3D.Emissive = pMat[nCntMat].MatD3D.Emissive;
		matNow.MatD3D.Power = pMat[nCntMat].MatD3D.Power;
		matNow.MatD3D.Specular = pMat[nCntMat].MatD3D.Specular;

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&matNow.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �������K�����f�t�H���g�ɂ���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================================
// ���[���h�}�g���b�N�X�v�Z
//==========================================================================
void CModel::CalWorldMtx(void)
{
#if 1
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxRotOrigin;	// �v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxnParent;			// �e�̃}�g���b�N�X

	bool bScale = false;

	// �e�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxRotOrigin);
	D3DXMatrixIdentity(&mtxnParent);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f����
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ���̌����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRotOrigin, m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotOrigin);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�̃}�g���b�N�X�Ɗ|�����킹��
	if (m_pParent == NULL)
	{// �������e�������ꍇ

		// �ŐV�̃}�g���b�N�X��n��(DirectX���o���Ă������)
		pDevice->GetTransform(D3DTS_WORLD, &mtxnParent);
	}
	else
	{// �e������ꍇ

		// �e�̃}�g���b�N�X��n��
		mtxnParent = m_pParent->GetWorldMtx();

		D3DXVECTOR3 scaleVector;

		// �}�g���b�N�X����X�P�[�����𒊏o
		scaleVector.x = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._11, mtxnParent._12, mtxnParent._13));
		scaleVector.y = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._21, mtxnParent._22, mtxnParent._23));
		scaleVector.z = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._31, mtxnParent._32, mtxnParent._33));
		if (scaleVector != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{
			bScale = true;
		}
	}

	// �����ɐe�̃��[���h�}�g���b�N�X���|����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxnParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	{// �����ł��Ⴄ�ꍇ

		bScale = true;
	}

	if (bScale)
	{
		// �������K����ON�ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	}
#else

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxRotOrigin;	// �v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxnParent;			// �e�̃}�g���b�N�X

	// �N�H�[�^�j�I���ϐ��̒ǉ�
	D3DXQUATERNION m_quaternionOrigin;
	D3DXQUATERNION m_quaternion;
	D3DXQuaternionIdentity(&m_quaternionOrigin);
	D3DXQuaternionIdentity(&m_quaternion);

	bool bScale = false;

	// �e�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxRotOrigin);
	D3DXMatrixIdentity(&mtxnParent);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���̌����𔽉f����
	D3DXQuaternionRotationYawPitchRoll(&m_quaternionOrigin, m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);
	D3DXQuaternionNormalize(&m_quaternionOrigin, &m_quaternionOrigin);  // �N�H�[�^�j�I���𐳋K��

	D3DXQUATERNION quatX;
	D3DXQuaternionRotationAxis(&quatX, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), m_rot.x);

	D3DXQUATERNION quatY;
	D3DXQuaternionRotationAxis(&quatY, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_rot.y);

	D3DXQUATERNION quatZ;
	D3DXQuaternionRotationAxis(&quatZ, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), m_rot.z);

	// x���Ay���Az���̏��ŉ�]��K�p
	m_quaternion = quatX * m_quaternion;
	m_quaternion = quatY * m_quaternion;
	m_quaternion = quatZ * m_quaternion;

	// �N�H�[�^�j�I���𐳋K��
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);  // �N�H�[�^�j�I���𐳋K��

	// �N�H�[�^�j�I����g�ݍ��킹�ĉ�]�}�g���b�N�X���쐬
	//D3DXMATRIX mtxRot, mtxRotOrigin;
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxRotOrigin);
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);
	D3DXMatrixRotationQuaternion(&mtxRotOrigin, &m_quaternionOrigin);

	// �X�P�[���𔽉f����
	//D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// �ʒu�𔽉f����
	//D3DXMATRIX mtxTrans;
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// ���[���h�}�g���b�N�X�̌v�Z
	m_mtxWorld = mtxScale * mtxRot * mtxRotOrigin * mtxTrans;

	// �e�̃}�g���b�N�X�Ɗ|�����킹��
	if (m_pParent == NULL)
	{// �������e�������ꍇ

		// �ŐV�̃}�g���b�N�X��n��(DirectX���o���Ă������)
		pDevice->GetTransform(D3DTS_WORLD, &mtxnParent);
	}
	else
	{// �e������ꍇ

		// �e�̃}�g���b�N�X��n��
		mtxnParent = m_pParent->GetWorldMtx();

		D3DXVECTOR3 scaleVector;

		// �}�g���b�N�X����X�P�[�����𒊏o
		scaleVector.x = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._11, mtxnParent._12, mtxnParent._13));
		scaleVector.y = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._21, mtxnParent._22, mtxnParent._23));
		scaleVector.z = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._31, mtxnParent._32, mtxnParent._33));
		if (scaleVector != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{
			bScale = true;
		}
	}

	// �����ɐe�̃��[���h�}�g���b�N�X���|����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxnParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	{// �����ł��Ⴄ�ꍇ

		bScale = true;
	}

	if (bScale)
	{
		// �������K����ON�ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	}

#endif

}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CModel::SetWorldMtx(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
D3DXMATRIX CModel::GetWorldMtx(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CModel::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CModel::GetPosition(void) const
{
	return m_pos;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CModel::SetOldPosition(const D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR3 CModel::GetOldPosition(void) const
{
	return m_posOld;
}

//==========================================================================
// ���̈ʒu�ݒ�
//==========================================================================
void  CModel::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// ���̈ʒu�擾
//==========================================================================
D3DXVECTOR3  CModel::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CModel::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// �����擾
//==========================================================================
D3DXVECTOR3 CModel::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// ���̌����ݒ�
//==========================================================================
void CModel::SetOriginRotation(const D3DXVECTOR3 rot)
{
	m_rotOrigin = rot;
}

//==========================================================================
// ���̌����擾
//==========================================================================
D3DXVECTOR3 CModel::GetOriginRotation(void) const
{
	return m_rotOrigin;
}

//==========================================================================
// ���̈ʒu�ݒ�
//==========================================================================
void CModel::SetOriginPotision(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// ���̈ʒu�擾
//==========================================================================
D3DXVECTOR3 CModel::GetOriginPotision(void) const
{
	return m_posOrigin;
}

//==========================================================================
// �X�P�[���ݒ�
//==========================================================================
void CModel::SetScale(const D3DXVECTOR3 scale)
{
	m_scale = scale;
}

//==========================================================================
// �X�P�[���擾
//==========================================================================
D3DXVECTOR3 CModel::GetScale(void) const
{
	return m_scale;
}

//==========================================================================
// ���_�̍ő�l�擾
//==========================================================================
D3DXVECTOR3 CModel::GetVtxMax(void) const
{
	// X�t�@�C���̃f�[�^�擾
	return CManager::GetXLoad()->GetObjectX(m_nIdxXFile)->vtxMax;
}

//==========================================================================
// ���_�̍ŏ��l�擾
//==========================================================================
D3DXVECTOR3 CModel::GetVtxMin(void) const
{
	// X�t�@�C���̃f�[�^�擾
	return CManager::GetXLoad()->GetObjectX(m_nIdxXFile)->vtxMin;
}

//==========================================================================
// ���f���I�u�W�F�N�g�̎擾
//==========================================================================
CModel *CModel::GetModel(void)
{
	return this;
}

//==========================================================================
// �����擾
//==========================================================================
int CModel::GetNumAll(void)
{
	return m_nNumAll;
}

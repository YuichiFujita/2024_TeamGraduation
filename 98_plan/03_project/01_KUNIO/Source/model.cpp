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
#include "light.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CModel::m_nNumAll = 0;	// ����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CModel::CModel(int nPriority)
{
	m_mtxParent = nullptr;							// �e�}�g���b�N�X�̃|�C���^
	m_pos = mylib_const::DEFAULT_VECTOR3;			// �ʒu
	m_posOld = mylib_const::DEFAULT_VECTOR3;		// �O��̈ʒu
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;		// ���̈ʒu
	m_rot = mylib_const::DEFAULT_VECTOR3;			// ����
	m_rotOrigin = mylib_const::DEFAULT_VECTOR3;		// ����
	m_scale = mylib_const::DEFAULT_SCALE;			// �X�P�[��
	m_scaleOrigin = mylib_const::DEFAULT_SCALE;			// �X�P�[��
	m_nIdxXFile = 0;								// X�t�@�C���̃C���f�b�N�X�ԍ�
	m_nIdxTexture = nullptr;							// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_pParent = nullptr;								// �e���f���̃|�C���^
	m_nNumAll++;									// �������Z
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CModel::~CModel()
{

}

//==========================================================================
// X�t�@�C���̃f�[�^���蓖��
//==========================================================================
void CModel::BindXData(int nIdxXFile)
{
	// ��񊄂蓖��
	m_nIdxXFile = nIdxXFile;

	// �e�N�X�`�����蓖��
	BindTexture();
}

//==========================================================================
// �e�N�X�`�����蓖��
//==========================================================================
void CModel::BindTexture()
{
	if (m_nIdxTexture != nullptr)
	{
		delete[] m_nIdxTexture;
	}

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �}�e���A�����������m��
	m_nIdxTexture = DEBUG_NEW int[(int)pXData->dwNumMat];

	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	for (int i = 0; i < (int)pXData->dwNumMat; i++)
	{
		m_nIdxTexture[i] = pXData->nIdxTexture[i];
	}
}

//==========================================================================
// �e�̃}�g���b�N�X�ݒ�
//==========================================================================
void CModel::SetParent(CModel *pModel)
{
	// �e�̃|�C���^��n��
	m_pParent = pModel;
}

void CModel::SetMtxParent(MyLib::Matrix* pMtx)
{ 
	m_mtxParent = pMtx;
}


//==========================================================================
// ��������
//==========================================================================
CModel *CModel::Create(const std::string& file, MyLib::Vector3 pos, MyLib::Vector3 rot)
{
	// �����p�̃I�u�W�F�N�g
	CModel *pModel = nullptr;

	if (pModel == nullptr)
	{// nullptr��������

		// �������̊m��
		pModel = DEBUG_NEW CModel;

		if (pModel == nullptr)
		{// nullptr��������

			delete pModel;
			pModel = nullptr;
			return nullptr;
		}

		if (pModel != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			HRESULT hr = pModel->Init(file);

			if (FAILED(hr))
			{// ���s���Ă�����
				return nullptr;
			}

			// �ʒu�E����
			pModel->SetPosition(pos);
			pModel->m_posOrigin = pos;
			pModel->SetRotation(rot);
			pModel->SetOriginRotation(rot);
		}

		return pModel;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CModel::Init(const std::string& file)
{
	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CXLoad::GetInstance();

	// X�t�@�C���̃��[�h
	m_nIdxXFile = pXLoad->XLoad(file);

	// X�t�@�C���̃f�[�^���蓖��
	BindXData(m_nIdxXFile);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	// �e�N�X�`�����蓖��
	BindTexture();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CModel::Uninit()
{
	if (m_nIdxTexture != nullptr)
	{
		delete[] m_nIdxTexture;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CModel::Update()
{
	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CXLoad::GetInstance();

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
}

//==========================================================================
// ���[���h�}�g���b�N�X�v�Z
//==========================================================================
void CModel::CalWorldMtx()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxScaleOrigin, mtxRotOrigin;	// �v�Z�p�}�g���b�N�X�錾
	MyLib::Matrix mtxParent;			// �e�̃}�g���b�N�X

	bool bScale = false;

	// �e�}�g���b�N�X�̏�����
	mtxRotOrigin.Identity();
	mtxParent.Identity();

	// ���[���h�}�g���b�N�X�̏�����
	m_mtxWorld.Identity();

	// ���̃X�P�[���𔽉f
	mtxScaleOrigin.Scaling(m_scaleOrigin);
	m_mtxWorld.Multiply(m_mtxWorld, mtxScaleOrigin);

	// �X�P�[���𔽉f����
	mtxScale.Scaling(m_scale);
	m_mtxWorld.Multiply(m_mtxWorld, mtxScale);

	// ���̌����𔽉f����
	mtxRotOrigin.RotationYawPitchRoll(m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(m_rot.y, m_rot.x, m_rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRotOrigin);

	// �ʒu�𔽉f����
	mtxTrans.Translation(m_pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

	// �e�̃}�g���b�N�X�Ɗ|�����킹��
	mtxParent = *m_mtxParent;

	if (m_pParent != nullptr &&
		mtxParent.GetScale() != MyLib::Vector3(1.0f, 1.0f, 1.0f))
	{// �e������ꍇ

		bScale = true;
	}

	// �����ɐe�̃��[���h�}�g���b�N�X���|����
	m_mtxWorld.Multiply(m_mtxWorld, mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
	{// �����ł��Ⴄ�ꍇ

		bScale = true;
	}

	if (bScale)
	{
		// �������K����ON�ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	}

}

//==========================================================================
// �V���h�E�}�g���b�N�X�̕`��
//==========================================================================
void CModel::DrawShadowMtx()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mtxShadow;
	D3DXVECTOR4 posLight;	// ���C�g�̈ʒu
	MyLib::Vector3 pos, normal;	// ���ʏ�̔C�ӂ̓_,�@���x�N�g��
	D3DXPLANE plane;		// ���ʏ��
	D3DLIGHT9 light;		// ���C�g���
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	
	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	//���C�g�̈ʒu��ݒ�
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	//�V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// ���ʏ��𐶐�
	pos = MyLib::Vector3(0.0f, 3.0f, 0.0f);
	normal = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// �V���h�E�}�g���b�N�X�̍쐬
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	D3DXMatrixMultiply(&mtxShadow, &mtx, &mtxShadow);

	// �V���h�E�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	D3DXMATERIAL matNow;	// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	matNow.MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&matNow.MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, nullptr);

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// ���Z���������Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CModel::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxRotOrigin;		// �v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxnParent;			// �e�̌v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nIdxTexture[nCntMat]));

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// �V���h�E�}�g���b�N�X�`��
	DrawShadowMtx();

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CModel::Draw(D3DXCOLOR col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL matNow;			// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = col;
	matNow.MatD3D.Ambient = col;

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matNow.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nIdxTexture[nCntMat]));

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}


	// �V���h�E�}�g���b�N�X�`��
	DrawShadowMtx();

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CModel::Draw(float fAlpha)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxRotOrigin;		// �v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxnParent;			// �e�̌v�Z�p�}�g���b�N�X

	D3DXMATERIAL matNow;			// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

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
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nIdxTexture[nCntMat]));

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}


	// �V���h�E�}�g���b�N�X�`��
	DrawShadowMtx();

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// �e�N�X�`���̃C���f�b�N�X���蓖��
//==========================================================================
void CModel::SetIdxTexture(int i, int nIdx)
{
	m_nIdxTexture[i] = nIdx;
}

//==========================================================================
// �e�N�X�`���C���f�b�N�X�ԍ����蓖��
//==========================================================================
int CModel::GetIdxTexture(int nIdx)
{
	return m_nIdxTexture[nIdx];
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CModel::SetWorldMtx(const MyLib::Matrix mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
MyLib::Matrix CModel::GetWorldMtx()
{
	return m_mtxWorld;
}

//==========================================================================
// �|�C���^�}�g���b�N�X�擾
//==========================================================================
MyLib::Matrix *CModel::GetPtrWorldMtx()
{
	return &m_mtxWorld;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CModel::SetPosition(const MyLib::Vector3 pos)
{
	m_pos = pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CModel::GetPosition() const
{
	return m_pos;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CModel::SetOldPosition(const MyLib::Vector3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CModel::GetOldPosition() const
{
	return m_posOld;
}

//==========================================================================
// ���̈ʒu�ݒ�
//==========================================================================
void  CModel::SetOriginPosition(const MyLib::Vector3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// ���̈ʒu�擾
//==========================================================================
MyLib::Vector3  CModel::GetOriginPosition() const
{
	return m_posOrigin;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CModel::SetRotation(const MyLib::Vector3 rot)
{
	m_rot = rot;
}

//==========================================================================
// �����擾
//==========================================================================
MyLib::Vector3 CModel::GetRotation() const
{
	return m_rot;
}

//==========================================================================
// ���̌����ݒ�
//==========================================================================
void CModel::SetOriginRotation(const MyLib::Vector3 rot)
{
	m_rotOrigin = rot;
}

//==========================================================================
// ���̌����擾
//==========================================================================
MyLib::Vector3 CModel::GetOriginRotation() const
{
	return m_rotOrigin;
}

//==========================================================================
// �X�P�[���ݒ�
//==========================================================================
void CModel::SetScale(const MyLib::Vector3 scale)
{
	m_scale = scale;
}

//==========================================================================
// �X�P�[���擾
//==========================================================================
MyLib::Vector3 CModel::GetScale() const
{
	return m_scale;
}

//==========================================================================
// ���_�̍ő�l�擾
//==========================================================================
MyLib::Vector3 CModel::GetVtxMax() const
{
	// X�t�@�C���̃f�[�^�擾
	return CXLoad::GetInstance()->GetMyObject(m_nIdxXFile)->vtxMax;
}

//==========================================================================
// ���_�̍ŏ��l�擾
//==========================================================================
MyLib::Vector3 CModel::GetVtxMin() const
{
	// X�t�@�C���̃f�[�^�擾
	return CXLoad::GetInstance()->GetMyObject(m_nIdxXFile)->vtxMin;
}

//==========================================================================
// ���f���I�u�W�F�N�g�̎擾
//==========================================================================
CModel *CModel::GetModel()
{
	return this;
}

//==========================================================================
// �����擾
//==========================================================================
int CModel::GetNumAll()
{
	return m_nNumAll;
}

//=============================================================================
// 
//  �I�u�W�F�N�gX���� [objectX.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectX.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "texture.h"
#include "shadow.h"
#include "collisionLine_Box.h"
#include "handle_Move.h"

//==========================================================================
// �֐��|�C���^
//==========================================================================
CObjectX::STATE_FUNC CObjectX::m_StateFunc[] =
{
	&CObjectX::StateNone,	// �Ȃɂ��Ȃ�
	&CObjectX::StateEdit,	// �G�f�B�b�g
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CObjectX> CObjectX::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectX::CObjectX(int nPriority, CObject::LAYER layer) : CObject(nPriority, layer)
{
	m_mtxWorld.Identity();						// ���[���h�}�g���b�N�X
	m_scale = MyLib::Vector3(1.0f);				// �X�P�[��
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_AABB = MyLib::AABB();						// AABB���
	m_OriginAABB = MyLib::AABB();				// ����AABB���
	m_state = STATE::STATE_NONE;				// ���
	m_pIdxTexure = nullptr;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nIdxXFile = 0;							// X�t�@�C���̃C���f�b�N�X�ԍ�
	m_pShadow = nullptr;						// �e�̏��
	m_bShadow = false;							// �e���g���Ă��邩�ǂ���
	m_pCollisionLineBox = nullptr;				// �����蔻��{�b�N�X
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectX::~CObjectX()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObjectX::BindTexture(int *nIdx)
{
	// ���蓖�Ă�
	m_pIdxTexure = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX* CObjectX::Create()
{
	// �������̊m��
	CObjectX* pObjectX = DEBUG_NEW CObjectX;

	if (pObjectX != nullptr)
	{
		// ����������
		if (FAILED(pObjectX->Init()))
		{
			return nullptr;
		}
	}

	return pObjectX;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX* CObjectX::Create(const std::string& file)
{
	// �������̊m��
	CObjectX* pObjectX = DEBUG_NEW CObjectX;

	if (pObjectX != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		if (FAILED(pObjectX->Init(file)))
		{
			return nullptr;
		}

		// ��ސݒ�
		pObjectX->SetType(TYPE_XFILE);
	}

	return pObjectX;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX* CObjectX::Create(const std::string& file, const MyLib::Vector3& pos, const MyLib::Vector3& rot, bool bShadow)
{
	// �������̊m��
	CObjectX* pObjectX = DEBUG_NEW CObjectX;

	if (pObjectX != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�E����
		pObjectX->SetPosition(pos);
		pObjectX->SetOriginPosition(pos);
		pObjectX->SetRotation(rot);
		pObjectX->m_bShadow = bShadow;

		// ����������
		HRESULT hr = pObjectX->Init(file);

		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return pObjectX;
}

//==========================================================================
// ��������
//==========================================================================
CObjectX* CObjectX::Create(int nIdxXFile, const MyLib::Vector3& pos, const MyLib::Vector3& rot, bool bShadow,
	int nPriority,
	CObject::LAYER layer)
{
	// �����p�̃I�u�W�F�N�g
	CObjectX* pObjectX = DEBUG_NEW CObjectX(nPriority, layer);

	if (pObjectX != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�E����
		pObjectX->SetPosition(pos);
		pObjectX->SetRotation(rot);
		pObjectX->m_bShadow = bShadow;

		// ����������
		if (FAILED(pObjectX->Init(nIdxXFile)))
		{
			return nullptr;
		}
	}

	return pObjectX;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectX::Init()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CXLoad::GetInstance();

	m_List.Regist(this);

	// X�t�@�C���̃��[�h
	m_nIdxXFile = pXLoad->XLoad("data\\MODEL\\radiokaikan_01.x");

	// X�t�@�C���̃f�[�^���蓖��
	BindXData(m_nIdxXFile);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
	m_AABB.vtxMin = pXData->vtxMin;
	m_AABB.vtxMax = pXData->vtxMax;
	m_pMesh = pXData->pMesh;

	// ����AABB���
	m_OriginAABB = m_AABB;

	// �e�N�X�`���C���f�b�N�X���蓖��
	BindTexture(pXData->nIdxTexture);

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectX::Init(const std::string& file)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// X�t�@�C���̃f�[�^�擾
	CXLoad *pXLoad = CXLoad::GetInstance();

	m_List.Regist(this);

	// X�t�@�C���̃��[�h
	m_nIdxXFile = pXLoad->XLoad(file);
	m_fileName = file;

	// X�t�@�C���̃f�[�^���蓖��
	BindXData(m_nIdxXFile);

	if (m_nIdxXFile < 0)
	{
		return S_OK;
	}

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
	m_AABB.vtxMin = pXData->vtxMin;
	m_AABB.vtxMax = pXData->vtxMax;
	m_pMesh = pXData->pMesh;

	// ����AABB���
	m_OriginAABB = m_AABB;

	// �e�N�X�`���C���f�b�N�X���蓖��
	BindTexture(pXData->nIdxTexture);

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectX::Init(int nIdxXFile)
{
	// X�t�@�C���̃f�[�^���蓖��
	BindXData(nIdxXFile);

	m_List.Regist(this);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// X�t�@�C���̃f�[�^�擾
	CXLoad* pXLoad = CXLoad::GetInstance();
	m_fileName = pXLoad->GetMyObject(nIdxXFile)->filename;

	// �S���_�`�F�b�N
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
	m_AABB.vtxMin = pXData->vtxMin;
	m_AABB.vtxMax = pXData->vtxMax;
	m_pMesh = pXData->pMesh;

	// ����AABB���
	m_OriginAABB = m_AABB;

	// �e�N�X�`���C���f�b�N�X���蓖��
	BindTexture(pXData->nIdxTexture);

	return S_OK;
}


//==========================================================================
// �I������
//==========================================================================
void CObjectX::Uninit()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// ���S����
//==========================================================================
void CObjectX::Kill()
{
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// �����蔻��{�b�N�X
	if (m_pCollisionLineBox != nullptr)
	{
		m_pCollisionLineBox->Kill();
		m_pCollisionLineBox = nullptr;
	}

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectX::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ߋ��̌����ݒ�
	SetOldRotation(GetRotation());

	// ��ԕʏ���
	(this->*(m_StateFunc[m_state]))();

	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(GetPosition());
	}

	// �����蔻��{�b�N�X�f�[�^�ݒ�
	SetCollisionBoxData();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CObjectX::StateNone()
{
	if (m_pCollisionLineBox != nullptr) 
	{
		m_pCollisionLineBox->SetEnableDisp(false);
	}
}

//==========================================================================
// �G�f�B�b�g���
//==========================================================================
void CObjectX::StateEdit()
{
	if (m_pCollisionLineBox != nullptr) 
	{
		m_pCollisionLineBox->SetEnableDisp(true);
	}
}

//==========================================================================
// �����蔻��{�b�N�X����
//==========================================================================
void CObjectX::CreateCollisionBox()
{
#if _DEBUG
	// �����蔻��{�b�N�X����
	m_pCollisionLineBox = CCollisionLine_Box::Create(m_AABB, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	// �����蔻��{�b�N�X�f�[�^�ݒ�
	SetCollisionBoxData();
#endif
}

//==========================================================================
// �����蔻��{�b�N�X�f�[�^�ݒ�
//==========================================================================
void CObjectX::SetCollisionBoxData()
{
	if (m_pCollisionLineBox != nullptr) 
	{

		MyLib::AABB aabb;
		aabb.vtxMin.x = m_OriginAABB.vtxMin.x * m_scale.x;
		aabb.vtxMin.y = m_OriginAABB.vtxMin.y * m_scale.y;
		aabb.vtxMin.z = m_OriginAABB.vtxMin.z * m_scale.z;

		aabb.vtxMax.x = m_OriginAABB.vtxMax.x * m_scale.x;
		aabb.vtxMax.y = m_OriginAABB.vtxMax.y * m_scale.y;
		aabb.vtxMax.z = m_OriginAABB.vtxMax.z * m_scale.z;

		// AABB�ݒ�
		m_pCollisionLineBox->SetAABB(aabb);

		// �ʒu�E�����ݒ�
		m_pCollisionLineBox->SetPosition(GetPosition());
		m_pCollisionLineBox->SetRotation(GetRotation());
	}
}

//==========================================================================
// �����擾
//==========================================================================
float CObjectX::GetHeight(const MyLib::Vector3& pos, bool &bLand)
{
	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	if (pXData == nullptr)
	{
		return 0.0f;
	}

	// �ő�̍���
	float fHeightMax = 0.0f;

	// �����̈ʒu
	MyLib::Vector3 MyPosition = GetPosition();

	WORD* pIndexBuff;

	// �C���f�b�N�X�o�b�t�@�����b�N
	pXData->pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	for (int nCntTri = 0; nCntTri < pXData->nFaceNum; nCntTri++) 
	{
		
		int nIdx1 = (int)pIndexBuff[nCntTri * 3];
		int nIdx2 = (int)pIndexBuff[nCntTri * 3 + 1];
		int nIdx3 = (int)pIndexBuff[nCntTri * 3 + 2];

		// ���_�ʒu���
		MyLib::Vector3 pos1 = pXData->pVtxPos[nIdx1] + MyPosition;
		MyLib::Vector3 pos2 = pXData->pVtxPos[nIdx2] + MyPosition;
		MyLib::Vector3 pos3 = pXData->pVtxPos[nIdx3] + MyPosition;

		// ��Βl(�}�C�i�X�����Ȃ�)�ƍő�̋�����r
		if (fabsf(pos1.x - pos.x) > pXData->fMaxVtxDistance ||
			fabsf(pos1.z - pos.z) > pXData->fMaxVtxDistance)
		{// ���_�����̕�������
			continue;
		}

		// �O�p�ō��������߂�
		
		bool bNowLand = false;
		float fNowHeight = UtilFunc::Calculation::GetVtxHeight(pos, pos1, pos3, pos2, &bNowLand);

		if (bNowLand && fNowHeight > fHeightMax)
		{// ���n���Ă���

			// �ő卂���X�V
			fHeightMax = fNowHeight;
			bLand = true;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	pXData->pMesh->UnlockIndexBuffer();

	return fHeightMax;
}

//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CObjectX::CalWorldMtx()
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Matrix mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X�錾

	// ���[���h�}�g���b�N�X�̏�����
	m_mtxWorld.Identity();

	// �X�P�[���𔽉f����
	mtxScale.Scaling(m_scale);
	m_mtxWorld.Multiply(m_mtxWorld, mtxScale);

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);
}

//==========================================================================
// �`��̂�
//==========================================================================
void CObjectX::DrawOnly()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile* pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	CTexture* pTex = CTexture::GetInstance();
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTex->GetAdress(m_pIdxTexure[nCntMat]));

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K����ON�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K�����f�t�H���g�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectX::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// �`��̂�
	DrawOnly();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectX::Draw(const D3DXCOLOR& col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	D3DXMATERIAL matNow;	// ����̃}�e���A��

	// ���̏��N���A
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = col;
	matNow.MatD3D.Ambient = col;

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// ���_�����J��Ԃ�
	CTexture *pTex = CTexture::GetInstance();
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matNow.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTex->GetAdress(m_pIdxTexure[nCntMat]));

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K����ON�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K�����f�t�H���g�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectX::Draw(float fAlpha)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^
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
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(pXData->nIdxTexture[nCntMat]));

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K����ON�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// �����ł��Ⴄ�ꍇ

			// �������K�����f�t�H���g�ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

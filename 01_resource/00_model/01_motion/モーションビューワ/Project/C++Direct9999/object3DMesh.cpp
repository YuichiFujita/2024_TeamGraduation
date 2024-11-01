//=============================================================================
// 
//  �I�u�W�F�N�g3D���� [object3DMesh.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object3DMesh.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"
#include "input.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define CAL_VTX		(1280)		// �v�Z�p�̒��_��
#define MAX_MOVE	(8.0f)		// �ړ���
#define MAX_RANGE	(200.0f)	// �͈�
#define MOVE_LEN	(1.0f)

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject3DMesh::CObject3DMesh(int nPriority) : CObject3D(nPriority)
{
	m_pVtxBuff = NULL;	// ���_�o�b�t�@�̃|�C���^
	m_pIdxBuff = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_nNumIndex = 0;		// �C���f�b�N�X��
	m_nNumVertex = 0;		// ���_��
	m_nWidth = 0;			// ��������
	m_nHeight = 0;			// �c������
	m_fWidthLen = 0.0f;		// ���̒���
	m_fHeightLen = 0.0f;	// �c�̒���
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_type = TYPE_FIELD;	// ���b�V���̃^�C�v
	m_pVtxPos = NULL;		// ���_���W
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject3DMesh::~CObject3DMesh()
{

}

//==========================================================================
// �e�N�X�`���̊��蓖��
//==========================================================================
void CObject3DMesh::BindTexture(int nIdx)
{
	// ���蓖�Ă�
	m_nTexIdx = nIdx;
}

//==========================================================================
// ��������
//==========================================================================
CObject3DMesh *CObject3DMesh::Create(void)
{
	// �����p�̃I�u�W�F�N�g
	CObject3DMesh *pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULL��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3DMesh;

		if (pObject3D->GetID() < 0)
		{// �������m�ۂɎ��s���Ă�����

			delete pObject3D;
			return NULL;
		}

		if (pObject3D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObject3D->Init();
		}

		return pObject3D;
	}

	return NULL;
}

//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CObject3DMesh *CObject3DMesh::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CObject3DMesh *pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULL��������

		// �������̊m��
		pObject3D = DEBUG_NEW CObject3DMesh(nPriority);

		if (pObject3D->GetID() < 0)
		{// �������m�ۂɎ��s���Ă�����

			delete pObject3D;
			return NULL;
		}

		if (pObject3D != NULL)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pObject3D->Init();

			// �ʒu�E����
			pObject3D->SetPosition(pos);
			pObject3D->SetRotation(rot);
		}

		return pObject3D;
	}

	return NULL;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject3DMesh::Init(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nNumIndex = (m_nHeight * ((m_nWidth + 1) * 2)) + (2 * (m_nHeight - 1));	// �C���f�b�N�X��
	m_nNumVertex = (m_nHeight + 1) * (m_nWidth + 1);							// ���_��
	m_type = TYPE_FIELD;	// ��ސݒ�

	m_pVtxPos = DEBUG_NEW D3DXVECTOR3[m_nNumVertex];	// ���_���W����

	// ���_���W�ݒ�
	hr = CreateVertex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �C���f�b�N�X����
	hr = CreateIndex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject3DMesh::Init(TYPE type)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nNumIndex = (m_nHeight * ((m_nWidth + 1) * 2)) + (2 * (m_nHeight - 1));	// �C���f�b�N�X��
	m_nNumVertex = (m_nHeight + 1) * (m_nWidth + 1);							// ���_��
	m_type = type;	// ��ސݒ�

	m_pVtxPos = DEBUG_NEW D3DXVECTOR3[m_nNumVertex];	// ���_���W����
	memset(m_pVtxPos, 0, sizeof(D3DXVECTOR3) * m_nNumVertex);

	// ���_���W�ݒ�
	hr = CreateVertex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �C���f�b�N�X����
	hr = CreateIndex();
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W���蓖��
			m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))] = pVtx[0].pos;

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==========================================================================
// ���_�o�b�t�@����
//==========================================================================
HRESULT CObject3DMesh::CreateVertex(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (m_pVtxBuff != NULL)
	{// ���ɏ�񂪓����Ă�ꍇ

		return E_FAIL;
	}

	// ���_�o�b�t�@�̐���
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	if (m_pVtxBuff == NULL)
	{// ���ɏ�񂪓����Ă�ꍇ

		return E_FAIL;
	}

	// ��ނɒ��_���W�ݒ�
	switch (m_type)
	{
	case TYPE_FIELD:
		SetVtxField();
		break;

	case TYPE_WALL:
		SetVtxWall();
		break;

	case TYPE_CYLINDER:
		SetVtxCylinder();
		break;

	case TYPE_DOME:
		SetVtxDome();
		break;

	case TYPE_DONUTS:
		SetVtxDonuts();
		break;
	}

	return S_OK;
}

//==========================================================================
// �C���f�b�N�X����
//==========================================================================
HRESULT CObject3DMesh::CreateIndex(void)
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �C���f�b�N�X�o�b�t�@�̐���
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;

	// ���_�ԍ��f�[�^�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight; nCntHeight++)
	{// �����̕��������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			pIdx[nCntIdx + 0] = nCntWidth + (m_nWidth + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((m_nWidth + 1) * nCntHeight);
			nCntIdx += 2;	// 2�����Ă邩��
		}

		if (nCntHeight + 1 < m_nHeight)
		{// �Ō�̂����͑ł��Ȃ�

			// ��ł�2��
			pIdx[nCntIdx + 0] = ((m_nWidth + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (m_nWidth + 1) * (nCntHeight + 2);
			nCntIdx += 2;	// 2�����Ă邩��
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObject3DMesh::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ���_�C���f�b�N�X�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// ���_���W�̔j��
	if (m_pVtxPos != NULL)
	{
		delete[] m_pVtxPos;
		m_pVtxPos = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObject3DMesh::Update(void)
{
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// ���_�グ����
//==========================================================================
void CObject3DMesh::UPVtxField(D3DXVECTOR3 pos)
{
	if (m_type != TYPE_FIELD)
	{
		return;
	}

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print(
		"\n"
		"���_�グ�����F[5, 6]\n"
		"���g�k�F[7, 8] �y%f, %f�z\n", m_fWidthLen, m_fHeightLen);

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			float fNowLength = 
				sqrtf((pos.x - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].x) * (pos.x - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].x)
				+ (pos.z - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].z) * (pos.z - m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].z));

			float Wariai = fNowLength / MAX_RANGE;

			if (Wariai >= 1.0f)
			{// ������1.0f�ȉ�����
				continue;
			}

			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (pInputKeyboard->GetPress(DIK_6))
			{// 6�ŏグ��
				move.y = MAX_MOVE - (MAX_MOVE * Wariai);
			}

			if (pInputKeyboard->GetPress(DIK_5))
			{// 5�ŉ�����
				move.y = -MAX_MOVE - (-MAX_MOVE * Wariai);
			}

			// �e���_���W�m�F
			m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].y += move.y;
		}
	}

	if (pInputKeyboard->GetPress(DIK_7))
	{// 7�ŏグ��

		m_fWidthLen -= MOVE_LEN;
		m_fHeightLen -= MOVE_LEN;
	}

	if (pInputKeyboard->GetPress(DIK_8))
	{// 8�ŉ�����
		m_fWidthLen += MOVE_LEN;
		m_fHeightLen += MOVE_LEN;
	}

	ValueNormalize(m_fWidthLen, 99999999.0f, 10.0f);
	ValueNormalize(m_fHeightLen, 99999999.0f, 10.0f);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObject3DMesh::Draw(void)
{
	D3DXMATRIX m_mtxWorld = GetWorldMtx();	// �}�g���b�N�X�擾
	D3DXVECTOR3 m_rot = GetRotation();	// �����擾
	D3DXVECTOR3 m_pos = GetPosition();	// �ʒu�擾

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(m_nTexIdx));

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumVertex,
		0,
		m_nNumIndex - 2);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject3DMesh::SetVtx(void)
{
	// ��ނɒ��_���W�ݒ�
	switch (m_type)
	{
	case TYPE_FIELD:
		SetVtxField();
		break;

	case TYPE_WALL:
		SetVtxWall();
		break;

	case TYPE_CYLINDER:
		SetVtxCylinder();
		break;

	case TYPE_DOME:
		SetVtxDome();
		break;

	case TYPE_DONUTS:
		SetVtxDonuts();
		break;

	case TYPE_SPHERE:
		SetVtxSphere();
		break;
	}
}


//==========================================================================
// ���̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxSphere(void)
{
	D3DXCOLOR col = GetColor();			// �F
	D3DXVECTOR3 pos = GetPosition();	// �ʒu
	D3DXVECTOR3 posVtx[CAL_VTX];		// �v�Z�p�̍��W
	D3DXVECTOR3 NormalizeNor[CAL_VTX];	// �v�Z�p�̃x�N�g��
	float fRotWidth = (D3DX_PI * 2) / (float)(m_nWidth);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotHeight = (D3DX_PI * 2) / (float)(m_nHeight);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotCalW = 0.0f;	// ���̍���̊p�x
	float fRotCalH = 0.0f;	// �c�̍���̊p�x

	VERTEX_3D* pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �Ō�̓_
	pVtx[m_nWidth + (m_nHeight * (m_nWidth + 1))].pos = D3DXVECTOR3(0.0f, m_fHeightLen, 0.0f);

	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		// ���Z�b�g
		fRotCalW = 0.0f;

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ����̊p�x������o��
			fRotCalH = (float)nCntHeight * fRotHeight;
			fRotCalW = (float)nCntWidth * fRotWidth;

			// �p�x�̐��K��
			/*UtilFunc::Transformation::RotNormalize(fRotCalW);
			UtilFunc::Transformation::RotNormalize(fRotCalH);*/

			// ���W����o��
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].x = cosf(fRotCalH) * sinf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].z = cosf(fRotCalH) * cosf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].y = sinf(fRotCalH) * m_fHeightLen;

			// ���_���W�̐ݒ�
			pVtx[0].pos = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] - pos;

			// �o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))]);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(m_nWidth)),
				nCntHeight * (1.0f / (float)(m_nHeight))
			);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �t�B�[���h�̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxField(void)
{
	D3DXCOLOR col = GetColor();	// �F

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				(m_fWidthLen * nCntWidth) - ((m_fWidthLen * m_nWidth) * 0.5f),
				m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))].y,
				-((m_fHeightLen * nCntHeight) - ((m_fHeightLen * m_nHeight) * 0.5f)));

			// ���_���W���蓖��
			m_pVtxPos[nCntWidth + (nCntHeight * (m_nWidth + 1))] = pVtx[0].pos;

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			(
				((float)nCntWidth / (1.0f / (float)(m_nWidth + 1))) * (1.0f / (float)(m_nWidth + 1)),
				((float)nCntHeight / (1.0f / (float)(m_nHeight + 1))) * (1.0f / (float)(m_nHeight + 1))
			);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �E�H�[���̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxWall(void)
{

}

//==========================================================================
// �V�����_�[�̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxCylinder(void)
{
	D3DXCOLOR col = GetColor();			// �F
	D3DXVECTOR3 pos = GetPosition();	// �ʒu
	D3DXVECTOR3 posVtx[CAL_VTX];		// �v�Z�p�̍��W
	float fRot = (D3DX_PI * 2) / (float)(m_nWidth);	//1���Ƃ̊p�x�����߂�
	D3DXVECTOR3 NormalizeNor[CAL_VTX];	// ���K���p

	for (int nCntHeight = 0; nCntHeight < m_nHeight; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			// ���_���W���߂�
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % m_nWidth * fRot) * m_fWidthLen,
				(m_fHeightLen * m_nHeight) - ((m_fHeightLen * nCntHeight)),
				cosf(nCntWidth % m_nWidth * fRot) * m_fWidthLen
			);

			// ���_���W���߂�
			posVtx[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % m_nWidth * fRot) * m_fWidthLen,
				(m_fHeightLen * m_nHeight) - ((m_fHeightLen * (nCntHeight + 1))),
				cosf(nCntWidth % m_nWidth * fRot) * m_fWidthLen
			);

			// �e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] - pos;
			NormalizeNor[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))] - pos;

			// �o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))],				&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))],	&NormalizeNor[nCntWidth + (m_nWidth + 1) + (nCntHeight * (m_nWidth + 1))]);
		}
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(m_nWidth)),
				nCntHeight * (1.0f / (float)(m_nHeight))
			);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �h�[���̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxDome(void)
{
	D3DXCOLOR col = GetColor();			// �F
	D3DXVECTOR3 pos = GetPosition();	// �ʒu
	D3DXVECTOR3 posVtx[CAL_VTX];		// �v�Z�p�̍��W
	D3DXVECTOR3 NormalizeNor[CAL_VTX];	// �v�Z�p�̃x�N�g��
	float fRotWidth = (D3DX_PI * 2) / (float)(m_nWidth);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotHeight = (D3DX_PI * 0.5f) / (float)(m_nHeight);	// 1���Ƃ̊p�x�����߂�, ����
	float fRotCalW = 0.0f;	// ���̍���̊p�x
	float fRotCalH = 0.0f;	// �c�̍���̊p�x

	// �Ō�̓_
	posVtx[m_nWidth + (m_nHeight * (m_nWidth + 1))] = D3DXVECTOR3(0.0f, m_fHeightLen, 0.0f);

	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ����̊p�x������o��
			fRotCalH = (float)(nCntHeight % (m_nHeight +1 )) * fRotHeight;
			fRotCalW = (fRotWidth * m_nWidth) - ((float)(nCntWidth % (m_nWidth + 1)) * fRotWidth);

			// �p�x�̐��K��
			RotNormalize(fRotCalW);
			RotNormalize(fRotCalH);

			// ���W����o��
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].x = cosf(fRotCalH) * sinf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].z = cosf(fRotCalH) * cosf(fRotCalW) * m_fWidthLen;
			posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))].y = sinf(fRotCalH) * m_fHeightLen;

			// �e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))] = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))] - pos;

			// �o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))]);
		}
	}
	
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < m_nHeight + 1; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = posVtx[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (m_nWidth + 1))];

			// ���_�J���[�̐ݒ�
			if (nCntHeight < m_nHeight - 1)
			{
				pVtx[0].col = col;
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f);
			}

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = -D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(m_nWidth)),
				nCntHeight * (1.0f / (float)(m_nHeight))
			);

			/*pVtx[0].tex = -D3DXVECTOR2
			(
				((float)nCntWidth / (1.0f / (float)(m_nWidth + 1))) * (1.0f / (float)(m_nWidth + 1)),
				((float)nCntHeight / (1.0f / (float)(m_nHeight + 1))) * (1.0f / (float)(m_nHeight + 1))
			);*/

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================================================================
// �h�[�i�c�̒��_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxDonuts(void)
{
	
}


//==========================================================================
// �����擾
//==========================================================================
float CObject3DMesh::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3& move)
{
	if (m_type != TYPE_FIELD)
	{
		return pos.y;
	}

	UPVtxField(pos);

	// �x�N�g���Ɩ@��
	D3DXVECTOR3 vec1, vec2, nor;
	float fHeight = pos.y;
	bool bLand = false;

	for (int nCntHeight = 0; nCntHeight < m_nHeight; nCntHeight++)
	{// �c�̕������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_nWidth; nCntWidth++)
		{// ���̕������J��Ԃ�

			if (bLand == true)
			{
				break;
			}

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (m_nWidth + 1));
			int nVerTexW = (m_nWidth + 1) + 1;

			int nLeft	= nCntWidth + (nCntHeight * (m_nWidth + 1));
			int nRight	= nCntWidth + (nCntHeight * (m_nWidth + 1)) + nVerTexW;

			if (CollisionTriangle(m_pVtxPos[nNowPoint], m_pVtxPos[nLeft], m_pVtxPos[nRight], pos, pos) == true)
			{// �O�p�ɓ����Ă�����

				// �x�N�g�����v�Z
				vec1 = m_pVtxPos[nRight] - m_pVtxPos[nNowPoint];
				vec2 = m_pVtxPos[nLeft] - m_pVtxPos[nNowPoint];

				// �O�ς����߂�
				D3DXVec3Cross(&nor, &vec1, &vec2);

				// �O�ς̐��K�������Ė@���ɂ���
				D3DXVec3Normalize(&nor, &nor);

				// �f�o�b�O�\��
				CManager::GetDebugProc()->Print(
					"\n=========================================================\n"
					"����Ă�����[%d, %d, %d]\n"
					"�@���F[%f, %f, %f]\n\n", nNowPoint, nLeft, nRight, nor.x, nor.y, nor.z);

				if (nor.y != 0.0f)
				{// �@����0.0f����Ȃ�������

					// ���������߂�
					fHeight = ((pos.x - m_pVtxPos[nNowPoint].x) * nor.x + (pos.z - m_pVtxPos[nNowPoint].z) * nor.z + (m_pVtxPos[nNowPoint].y * -nor.y)) / -nor.y;
					fHeight += GetPosition().y;
					bLand = true;
					break;
				}
			}

			if (CollisionTriangle(m_pVtxPos[nNowPoint + m_nWidth], m_pVtxPos[nRight], m_pVtxPos[nLeft], pos, pos) == true)
			{// �O�p�ɓ����Ă�����

				// �x�N�g�����v�Z
				vec1 = m_pVtxPos[nLeft] - m_pVtxPos[nNowPoint + m_nWidth];
				vec2 = m_pVtxPos[nRight] - m_pVtxPos[nNowPoint + m_nWidth];

				// �O�ς����߂�
				D3DXVec3Cross(&nor, &vec1, &vec2);

				// �O�ς̐��K�������Ė@���ɂ���
				D3DXVec3Normalize(&nor, &nor);

				// �f�o�b�O�\��
				CManager::GetDebugProc()->Print(
					"\n=========================================================\n"
					"����Ă�����[%d, %d, %d]\n"
					"�@���F[%f, %f, %f]\n\n", nNowPoint + m_nWidth, nLeft, nRight, nor.x, nor.y, nor.z);

				if (nor.y != 0.0f)
				{// �@����0.0f����Ȃ�������

					// ���������߂�
					fHeight = ((pos.x - m_pVtxPos[nNowPoint + m_nWidth].x) * nor.x + (pos.z - m_pVtxPos[nNowPoint + m_nWidth].z) * nor.z + (m_pVtxPos[nNowPoint + m_nWidth].y * -nor.y)) / -nor.y;
					fHeight += GetPosition().y;
					bLand = true;
					break;
				}
			}
		}
	}

	if (bLand == true)
	{// ���n���Ă����� && �n�ʂ�艺

		move.y = 0.0f;
		return fHeight;
	}

	// �������擾
	return pos.y;
}

//==========================================================================
// �C���f�b�N�X���擾
//==========================================================================
int CObject3DMesh::GetNumIndex(void)
{
	return m_nNumIndex;
}

//==========================================================================
// �C���f�b�N�X���ݒ�
//==========================================================================
void CObject3DMesh::SetNumIndex(int nWidth, int nHeight)
{
	m_nNumIndex = (nHeight * ((nWidth + 1) * 2)) + (2 * (nHeight - 1));	// �C���f�b�N�X��
}

//==========================================================================
// ���_���ݒ�
//==========================================================================
int CObject3DMesh::GetNumVertex(void)
{
	return m_nNumVertex;
}

//==========================================================================
// ���_���擾
//==========================================================================
void CObject3DMesh::SetNumVertex(int nWidth, int nHeight)
{
	m_nNumVertex = (nHeight + 1) * (nWidth + 1);							// ���_��
}

//==========================================================================
// ���_���W�擾
//==========================================================================
D3DXVECTOR3 *CObject3DMesh::GetVtxPos(void)
{
	return m_pVtxPos;
}

//==========================================================================
// ���_���W�ݒ�
//==========================================================================
void CObject3DMesh::SetVtxPos(D3DXVECTOR3 *pos)
{
	m_pVtxPos = pos;
}

#if 0

//==========================================================================
// ���_�x�N�g���擾
//==========================================================================
D3DXVECTOR3 *CObject3DMesh::GetVtxNor(void)
{
	return m_pVtxNor;
}

//==========================================================================
// ���_�x�N�g���擾
//==========================================================================
void CObject3DMesh::SetVtxNor(D3DXVECTOR3 *nor)
{
	
		m_pVtxNor = nor;
	
}

D3DXVECTOR2 *CObject3DMesh::GetVtxTex(void)
{
	return m_pVtxTex;
}

void CObject3DMesh::SetVtxTex(D3DXVECTOR2 *tex)
{
	
		m_pVtxTex = tex;
	
}
#endif

//==========================================================================
// ���̕������擾
//==========================================================================
int CObject3DMesh::GetWidthBlock(void)
{
	return m_nWidth;
}

//==========================================================================
// ���̕������ݒ�
//==========================================================================
void CObject3DMesh::SetWidthBlock(int nWidth)
{
	m_nWidth = nWidth;
}

//==========================================================================
// �����̕������擾
//==========================================================================
int CObject3DMesh::GetHeightBlock(void)
{
	return m_nHeight;
}

//==========================================================================
// �����̕������ݒ�
//==========================================================================
void CObject3DMesh::SetHeightBlock(int nHeight)
{
	m_nHeight = nHeight;
}

//==========================================================================
// ���̒����擾
//==========================================================================
float CObject3DMesh::GetWidthLen(void)
{
	return m_fWidthLen;
}

//==========================================================================
// ���̒����ݒ�
//==========================================================================
void CObject3DMesh::SetWidthLen(float fLen)
{
	m_fWidthLen = fLen;
}

//==========================================================================
// �����̒����擾
//==========================================================================
float CObject3DMesh::GetHeightLen(void)
{
	return m_fHeightLen;
}

//==========================================================================
// �����̒����ݒ�
//==========================================================================
void CObject3DMesh::SetHeightLen(float fLen)
{
	m_fHeightLen = fLen;
}

//==========================================================================
// �e�N�X�`���̃C���f�b�N�X�ԍ��擾
//==========================================================================
int CObject3DMesh::GetIdxTex(void)
{
	return m_nTexIdx;
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3DMesh *CObject3DMesh::GetObject3DMesh(void)
{
	return this;
}


//=============================================================================
// 
//  �N������ [waterfield.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterfield.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"
#include "debugproc.h"

// �p����
#include "waterfield_right.h"
#include "waterfield_left.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-04.jpg";
	const int BLOCK = 8;
	const float BLOCK_SIZE = 40000.0f;
	const float INTERVAL_TEXU = 900.0f;	// U���W�̊Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterField::CWaterField(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_fTexU = 0.0f;		// U�X�N���[���p
	m_fTexV = 0.0f;		// V�X�N���[���p
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterField::~CWaterField()
{

}

//==========================================================================
// ��������
//==========================================================================
CWaterField* CWaterField::Create(TYPE type)
{
	// �������̊m��
	CWaterField* pObjMeshField = nullptr;

	switch (type)
	{
	case CWaterField::TYPE_NORMAL:
		pObjMeshField = DEBUG_NEW CWaterField;
		break;

	case CWaterField::TYPE_RIGHT:
		pObjMeshField = DEBUG_NEW CWaterField_Right;
		break;

	case CWaterField::TYPE_LEFT:
		pObjMeshField = DEBUG_NEW CWaterField_Left;
		break;

	default:
		return nullptr;
		break;
	}

	if (pObjMeshField != nullptr)
	{
		// ����������
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CWaterField::Init()
{
	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 0.1f, 0.0f));			// �ʒu
	SetWidthBlock(BLOCK);		// ������
	SetHeightBlock(BLOCK);		// �c����
	SetWidthLen(BLOCK_SIZE);	// �c����
	SetHeightLen(BLOCK_SIZE);	// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();


	// �S�Ă̗v�f����������
	D3DXCOLOR* pVtxCol = GetVtxCol();
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.8f));

	return E_FAIL;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterField::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterField::Update()
{
	m_fTexU -= 0.003f;		// U�X�N���[���p

	if (m_fTexU >= 1.0f)
	{
		m_fTexU = 0.0f;
	}
	else if (m_fTexU <= 0.0f)
	{
		m_fTexU = 1.0f;
	}

	if (m_fTexV >= 1.0f)
	{
		m_fTexV = 0.0f;
	}
	else if (m_fTexV <= 0.0f)
	{
		m_fTexV = 1.0f;
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CWaterField::Draw()
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �`�揈��
	CObject3DMesh::Draw();
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CWaterField::SetVtx()
{
	int nHBlock = GetHeightBlock();
	int nWBlock = GetWidthBlock();

	// ���_���
	MyLib::Vector3* pVtxPos = GetVtxPos();
	D3DXVECTOR2* pVtxTex = GetVtxTex();

	int idx = 0;

	// �e�N�X�`�����
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU);
	float intervalU = size.x, intervalV = size.y;
	float posU = 0.0f, posV = 0.0f;

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < nHBlock + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		// ���Z�b�g
		posU = 0.0f;

		for (int nCntWidth = 0; nCntWidth < nWBlock + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			
			// UV���W
			pVtxTex[idx] = D3DXVECTOR2(posU + m_fTexU, posV + m_fTexV);

			// ���̊������i�߂�
			if (nCntWidth + 1 <= nWBlock)
			{
				int u = nCntWidth + (nCntHeight * (nWBlock + 1));
				posU += pVtxPos[u].DistanceXZ(pVtxPos[u + 1]) / INTERVAL_TEXU;
			}

			idx++;
		}

		// �c�̊������i�߂�
		if (nCntHeight != nHBlock)
		{
			int vtxIdx = (nCntHeight * (nWBlock + 1));
			int vtxIdxDown = ((nCntHeight + 1) * (nWBlock + 1));
			posV += pVtxPos[vtxIdx].DistanceXZ(pVtxPos[vtxIdxDown]) / intervalV;
		}
	}

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

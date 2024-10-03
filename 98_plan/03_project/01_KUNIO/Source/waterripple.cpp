//=============================================================================
// 
//  ���䏈�� [waterripple.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterripple.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float DEFAULT_ALPHA = 0.5f;	// �f�t�H�s�����x
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterRipple::CWaterRipple(const int block, const float blockSize, int nPriority, const LAYER layer)
	: m_Block(block), m_BlockSize(blockSize), CObject3DMesh(nPriority, layer)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterRipple::~CWaterRipple()
{

}

//==========================================================================
// ��������
//==========================================================================
CWaterRipple* CWaterRipple::Create(
	const int block, const float blockSize, 
	const MyLib::Vector3& pos, float height, float velocity, float thickness, int life)
{
	// �������̊m��
	CWaterRipple* pObjMeshField = DEBUG_NEW CWaterRipple(block, blockSize);

	if (pObjMeshField != nullptr)
	{
		pObjMeshField->m_Info = Info(height, velocity, 0.0f, thickness, life, life);

		// ����������
		pObjMeshField->SetPosition(pos);
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CWaterRipple::Init()
{
	HRESULT hr;

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e��ϐ�������
	SetWidthBlock(m_Block);		// ������
	SetHeightBlock(m_Block);	// �c����
	SetWidthLen(m_BlockSize);	// �c����
	SetHeightLen(m_BlockSize);	// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// �S�Ă̗v�f����������
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(0.6f, 0.6f, 1.0f, DEFAULT_ALPHA));


	// ���_���W�v�Z
	SetVtxPosition();

	SetVtx();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterRipple::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterRipple::Update()
{
	// ���S����̒������Z
	m_Info.length += m_Info.velocity;

	// ���_���W�v�Z
	SetVtxPosition();

	SetVtx();

	// �������Z
	m_Info.life--;

	if (m_Info.life <= 0)
	{
		Uninit();
	}
}

//==========================================================================
// ���_���W
//==========================================================================
void CWaterRipple::SetVtxPosition()
{

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3* pVtxNor = GetVtxNor();
	D3DXCOLOR* pVtxCol = GetVtxCol();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 vtxPos;

	float lifeRatio = (float)m_Info.life / (float)m_Info.maxLife;

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// ���̕������J��Ԃ�

			int idx = nCntWidth + (nWidth + 1) * nCntHeight;
			vtxPos = pVtxPos[idx] + pos;

			// ���_�ƒ��_�̋���
			float fNowLength =
				sqrtf((pos.x - vtxPos.x) * (pos.x - vtxPos.x)
					+ (pos.z - vtxPos.z) * (pos.z - vtxPos.z));


			// ���_�ƍő�͈͂܂ł̋���
			float fMaxLength = m_Info.length + m_Info.thickness;

			// ���_�ƍŏ��͈͂܂ł̋���
			float fMinLength = m_Info.length - m_Info.thickness;

			// �͈͂̒���
			float fRangeLength = fMaxLength - fMinLength;


			// �����x
			pVtxCol[idx].a = DEFAULT_ALPHA * (static_cast<float>(m_Info.life) / static_cast<float>(m_Info.maxLife));

			// ���݋����Ƃ̊���
			float ratio = m_Info.length / fNowLength;

			if (fNowLength > fMaxLength || fNowLength < fMinLength)
			{// �͈͊O�͈ړ��ʃ[��
				
				ratio = 0.0f;
				pVtxCol[idx].a = 0.0f;
			}

			if (fNowLength <= fMaxLength &&
				fNowLength >= fMinLength)
			{

				if (m_Info.length >= fNowLength)
				{
					int n = 0;
				}


				float calMinLen = fMinLength;
				if (calMinLen <= 0.0f)
				{// �͈͊O
					calMinLen = 0.0f;
					pVtxCol[idx].a = 0.0f;
				}

				float calMaxLen = fMaxLength - calMinLen;
				float calVtxLen = fNowLength - calMinLen;
				if (calMaxLen <= 0.0f)
				{// �͈͊O
					calMaxLen = 0.0f;
					pVtxCol[idx].a = 0.0f;
				}

				if (calVtxLen <= 0.0f)
				{// �͈͊O
					calVtxLen = 0.0f;
					pVtxCol[idx].a = 0.0f;
				}


				// �ő勗����1.0f�ɂȂ銄��
				float maxratio = calVtxLen / calMaxLen;

				if (maxratio > 0.5f)
				{
					maxratio = 1.0f - maxratio;
				}
				else
				{
					int n = 0;
				}

				ratio = maxratio;
			}

			// �g�̍���
			float waveHeight = m_Info.height * ratio;

			waveHeight *= lifeRatio;

			// �g�̍����ݒ�
			pVtxPos[idx].y = waveHeight;
		}
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CWaterRipple::Draw()
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (CManager::GetInstance()->IsWireframe())
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ���C���[�t���[�����[�h
	}

	// �`�揈��
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ���߂郂�[�h
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CWaterRipple::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3 *pVtxNor = GetVtxNor();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// ���̕������J��Ԃ�

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (nWidth));
			int nVerTexW = (nWidth) + 1;

			int nLeft = nCntWidth + (nCntHeight * (nWidth));
			int nRight = nCntWidth + (nCntHeight * (nWidth)) + nVerTexW;

			if (nNowPoint >= vtxNum)
			{
				continue;
			}

			if (nRight >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxRight = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * nWidth) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * nHeight) * 0.5f)));
			}
			else
			{
				VtxRight = pVtxPos[nRight];
			}

			if (nLeft >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxLeft = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * nWidth) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * nHeight) * 0.5f)));
			}
			else
			{
				VtxLeft = pVtxPos[nLeft];
			}

			if (nNowPoint >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxNow = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * nWidth) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * nHeight) * 0.5f)));
			}
			else
			{
				VtxNow = pVtxPos[nNowPoint];
			}

			// �x�N�g�����v�Z
			vec1 = VtxRight - VtxNow;
			vec2 = VtxLeft - VtxNow;

			// �O�ς����߂�
			D3DXVec3Cross(&nor, &vec1, &vec2);

			// �O�ς̐��K�������Ė@���ɂ���
			D3DXVec3Normalize(&nor, &nor);

			// �@��
			pVtxNor[nNowPoint] = nor;
		}
	}

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

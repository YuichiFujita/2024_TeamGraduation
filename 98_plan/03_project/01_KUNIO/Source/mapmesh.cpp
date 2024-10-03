//=============================================================================
// 
//  �}�b�v���b�V������ [mapmesh.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "mapmesh.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "course.h"

// �p����
#include "stonewall.h"
#include "townfield_sinuous.h"
#include "townfield_sinuous_front.h"
#include "townfield_fixedplane.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\map_object\\ishigaki000.png";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 200.0f;		// �΂߂̕�
	const float HEIGHT = 500.0f;	// ����
	const float INTERVAL_TEXU = 500.0f;	// U���W�̊Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMapMesh::CMapMesh(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_vecSegmentPosition.clear();	// ��_�̈ʒu
	m_vecVtxPosition.clear();		// �e���_�̈ʒu
	m_courceLength = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMapMesh::~CMapMesh()
{

}

//==========================================================================
// ��������
//==========================================================================
CMapMesh* CMapMesh::Create(MeshType type)
{
	// �������̊m��
	CMapMesh* pObjMeshField = nullptr;

	switch (type)
	{
	case CMapMesh::TYPE_STONEWALL_LEFT:
		pObjMeshField = DEBUG_NEW CStoneWall;
		break;

	case CMapMesh::TYPE_TOWNFIELD_SINUOUS:
		pObjMeshField = DEBUG_NEW CTownField_Sinuous;
		break;

	case CMapMesh::TYPE_TOWNFIELD_SINUOUS_FRONT:
		pObjMeshField = DEBUG_NEW CTownField_Sinuous_Front;
		break;

	case CMapMesh::TYPE_TOWNFIELD_FIXEDPLANE:
		pObjMeshField = DEBUG_NEW CTownField_FixedPlane;
		break;

	case CMapMesh::TYPE_TOWNFIELD_FIXEDPLANE_RESULT:
		pObjMeshField = DEBUG_NEW CTownField_FixedPlane_Result;
		break;

	case CMapMesh::TYPE_TOWNFIELD_FIXEDPLANE_RANKING:
		pObjMeshField = DEBUG_NEW CTownField_FixedPlane_Ranking;
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
HRESULT CMapMesh::Init()
{
	HRESULT hr;

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// ���_���W���蓖��
	BindVtxPosition();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMapMesh::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �e���_�v�Z
//==========================================================================
void CMapMesh::CalVtxPosition()
{

	// �Z�O�����g�̒������v�Z
	int segmentSize = m_vecSegmentPosition.size();
	std::vector<float> vecLength(segmentSize);

	for (int i = 0; i < segmentSize; ++i)
	{
		// ����̃C���f�b�N�X(���[�v)
		int next = (i + 1) % segmentSize;

		if (next == 0)
		{
			vecLength[i] = 10.0f;
			break;
		}

		// �_���m�̋���
		vecLength[i] = m_vecSegmentPosition[i].Distance(m_vecSegmentPosition[next]);
	}


	// ���_���N���A
	m_vecVtxPosition.clear();

	// �e���_�i�[
	m_courceLength = 0.0f;
	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += CCourse::GetCreateDistance();

			if (distance >= vecLength[i])
			{
				m_courceLength += CCourse::GetCreateDistance() - (distance - vecLength[i]);

				distance = vecLength[i];

				m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength, 20.0f));
				break;
			}

			m_courceLength += CCourse::GetCreateDistance();
			m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength));
		}
	}
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CMapMesh::Reset()
{
	// �I������
	CObject3DMesh::Uninit();

	// ���S�t���O�������Z�b�g
	SetEnableDeath(false);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e���_�v�Z
	CalVtxPosition();

	// �e��ϐ�������
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_vecVtxPosition.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();

	// ���_���W���蓖��
	BindVtxPosition();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMapMesh::Update()
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CMapMesh::Draw()
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
void CMapMesh::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3* pVtxNor = GetVtxNor();
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxPosition.size());

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH_BLOCK; nCntWidth++)
		{// ���̕������J��Ԃ�

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (WIDTH_BLOCK));
			int nVerTexW = (WIDTH_BLOCK) + 1;

			int nLeft = nCntWidth + (nCntHeight * (WIDTH_BLOCK));
			int nRight = nCntWidth + (nCntHeight * (WIDTH_BLOCK)) + nVerTexW;

			if (nNowPoint >= vtxNum)
			{
				continue;
			}

			if (nRight >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxRight = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxRight = pVtxPos[nRight];
			}

			if (nLeft >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxLeft = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxLeft = pVtxPos[nLeft];
			}

			if (nNowPoint >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxNow = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
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

	// �e���_UV���W�ݒ�
	SetVtxTexUV();

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ��_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CMapMesh::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// ��_�̈ʒu�ݒ�
//==========================================================================
void CMapMesh::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}

//==========================================================================
// �e���_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CMapMesh::GetVecVtxPosition(int idx)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecVtxPosition[idx];
}

//==========================================================================
// �e���_�̈ʒu�ݒ�
//==========================================================================
void CMapMesh::SetVecVtxPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return;

	m_vecVtxPosition[idx] = pos;
}
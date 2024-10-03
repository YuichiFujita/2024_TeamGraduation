//=============================================================================
// 
//  �Ί_���� [stonewall_front.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stonewall_front.h"
#include "manager.h"
#include "calculation.h"
#include "spline.h"
#include "course.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\map_object\\ishigaki000.png";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = -50.0f;		// �΂߂̕�
	const float HEIGHT = 100.0f;	// ����
	const float INTERVAL_TEXU = 500.0f;	// U���W�̊Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStoneWall_Front::CStoneWall_Front(int nPriority, const LAYER layer) : CStoneWall(nPriority, layer)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStoneWall_Front::~CStoneWall_Front()
{

}

//==========================================================================
// ��������
//==========================================================================
CStoneWall_Front* CStoneWall_Front::Create()
{
	// �������̊m��
	CStoneWall_Front* pObjMeshField = DEBUG_NEW CStoneWall_Front;

	if (pObjMeshField != nullptr)
	{
		// ����������
		pObjMeshField->Init();
	}

	return pObjMeshField;
}


//==========================================================================
// �_�\��
//==========================================================================
void CStoneWall_Front::DispPoint()
{
#if _DEBUG

	// �`�����
	SetEnableDisp(false);

	MyLib::Vector3* pVtxPos = GetVtxPos();

	for (int y = 0; y < static_cast<int>(m_vecVtxPosition.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		CEffect3D::Create(
			pVtxPos[nextidx],
			0.0f,
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
#endif
}

//==========================================================================
// ���_���W
//==========================================================================
void CStoneWall_Front::BindVtxPosition()
{
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	m_vecTopPosition.clear();

	for (int y = 0; y < static_cast<int>(m_vecVtxPosition.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % static_cast<int>(m_vecVtxPosition.size());

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		rot.y = m_vecVtxPosition[next].AngleXZ(m_vecVtxPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{
			rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecVtxPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);



		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(-WIDTH, HEIGHT, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(0.0f, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);


		// ���_���W���
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

		// ����̈ʒu
		m_vecTopPosition.push_back(pVtxPos[idx]);
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �e���_UV���W�ݒ�
//==========================================================================
void CStoneWall_Front::SetVtxTexUV()
{
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int nHeight = GetHeightBlock();

	if (pTex != nullptr)
	{
		float posU = 0.0f, posV = 0.0f;
		int idx = 0;

		int texID = CTexture::GetInstance()->Regist(TEXTURE);
		float intervalV = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU).y;

		for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
		{// �c�̕������J��Ԃ�

			// ���Z�b�g
			posV = 0.0f;

			for (int nCntWidth = 0; nCntWidth < WIDTH_BLOCK; nCntWidth++)
			{// ���̕������J��Ԃ�

				idx = nCntWidth + (nCntHeight * WIDTH_BLOCK);
				pTex[idx] = D3DXVECTOR2(posU, posV);

				// �c�̊������i�߂�
				posV += sqrtf((pVtxPos[idx].y - pVtxPos[idx + 1].y) * (pVtxPos[idx].y - pVtxPos[idx + 1].y)) / intervalV;

			}

			// ���̊������i�߂�
			posU += pVtxPos[idx + 1].DistanceXZ(pVtxPos[idx - 1]) / INTERVAL_TEXU;

		}
	}
}

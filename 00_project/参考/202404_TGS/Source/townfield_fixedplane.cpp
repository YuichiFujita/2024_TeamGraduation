//=============================================================================
// 
//  �X�t�B�[���h(�Œ蕽��) [townfield_fixedplane.cpp]
//  Author : ���n �Ή�
// 
//=============================================================================
#include "townfield_fixedplane.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "course.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\soil.jpg";
	const int WIDTH_BLOCK = 2;
	const float FIELD_WORLDLINE_Z = 3000.0f;		// ��΍��W�̃��C��
	const float INTERVAL_TEXU = 600.0f;	// U���W�̊Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTownField_FixedPlane::CTownField_FixedPlane(int nPriority, const LAYER layer) : CMapMesh(nPriority, layer)
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTownField_FixedPlane::~CTownField_FixedPlane()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTownField_FixedPlane::Init()
{
	HRESULT hr;

	m_vecVtxPosition =
	{
		MyLib::Vector3(-30000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(-30000.0f, 300.0f, 3000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 3000.0f)
	};
	SetVecPosition(m_vecVtxPosition);

	// �e��ϐ�������
	SetWidthBlock(1);		// ������
	SetHeightBlock(1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ����������
	CMapMesh::Init();

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTownField_FixedPlane::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// ���_���W
//==========================================================================
void CTownField_FixedPlane::BindVtxPosition()
{
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	D3DXVECTOR2* pTex = GetVtxTex();

	float wourldLenU = m_vecVtxPosition[0].x - m_vecVtxPosition[1].x;
	float wourldLenV = m_vecVtxPosition[0].z - m_vecVtxPosition[2].z;
	float posU = wourldLenU / INTERVAL_TEXU, posV = wourldLenV / INTERVAL_TEXU;

	pTex[0] = D3DXVECTOR2(0.0f, posV);
	pTex[1] = D3DXVECTOR2(posU, posV);

	pTex[2] = D3DXVECTOR2(0.0f, 0.0f);
	pTex[3] = D3DXVECTOR2(posU, 0.0f);


	for (int cnt = 0; cnt < static_cast<int>(m_vecVtxPosition.size()); cnt++)
	{
		pVtxPos[cnt] = m_vecVtxPosition[cnt];
	}

	// ���_���ݒ�
	SetVtx();

	VERTEX_3D* pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	GetVtxBuff()->Unlock();
}

//==========================================================================
// �e���_UV���W�ݒ�
//==========================================================================
void CTownField_FixedPlane::SetVtxTexUV()
{
	D3DXVECTOR2* pTex = GetVtxTex();

	float wourldLenU = m_vecVtxPosition[0].x - m_vecVtxPosition[1].x;
	float wourldLenV = m_vecVtxPosition[0].z - m_vecVtxPosition[2].z;
	float posU = wourldLenU / INTERVAL_TEXU, posV = wourldLenV / INTERVAL_TEXU;

	pTex[0] = D3DXVECTOR2(0.0f, posV);
	pTex[1] = D3DXVECTOR2(posU, posV);

	pTex[2] = D3DXVECTOR2(0.0f, 0.0f);
	pTex[3] = D3DXVECTOR2(posU, 0.0f);
}


//==========================================================================
// ���U���g�̃R���X�g���N�^
//==========================================================================
CTownField_FixedPlane_Result::CTownField_FixedPlane_Result() : CTownField_FixedPlane()
{

}

//==========================================================================
// ���U���g�̏�����
//==========================================================================
HRESULT CTownField_FixedPlane_Result::Init()
{
	// ������
	CTownField_FixedPlane::Init();

	m_vecVtxPosition =
	{
		MyLib::Vector3(-30000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(-30000.0f, 300.0f, -33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, -33000.0f)
	};
	SetVecPosition(m_vecVtxPosition);

	// �}�b�v���b�V���̏���������
	HRESULT hr = CMapMesh::Init();

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	return hr;
}


//==========================================================================
// ���U���g�̃R���X�g���N�^
//==========================================================================
CTownField_FixedPlane_Ranking::CTownField_FixedPlane_Ranking() : CTownField_FixedPlane()
{

}

//==========================================================================
// ���U���g�̏�����
//==========================================================================
HRESULT CTownField_FixedPlane_Ranking::Init()
{
	// ������
	CTownField_FixedPlane::Init();

	m_vecVtxPosition =
	{
		MyLib::Vector3(-30000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(100000.0f, 300.0f, 33000.0f),
		MyLib::Vector3(-30000.0f, 300.0f, 2200.0f),
		MyLib::Vector3(100000.0f, 300.0f, 2200.0f)
	};
	SetVecPosition(m_vecVtxPosition);

	// �}�b�v���b�V���̏���������
	HRESULT hr = CMapMesh::Init();

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	return hr;
}

//=============================================================================
// 
//  �N������ [course.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "course.h"
#include "course_title.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "convert.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-04.jpg";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 2000.0f;
	const float INTERVAL_TEXU = 500.0f;		// U���W�̊Ԋu
	const float INTERVAL_SINCURVE = 1200.0f;	// �T�C���J�[�u�̊Ԋu
	const float HEIGHT_SINCURVE = 8.4f;	// �T�C���J�[�u�̍���
	const float SCROLL_VELOCITY = 10.0f;
	const ImVec4 FILLCOLOR = ImVec4(0.80, 0.98, 1.0, 0.4f); // RGBA

}
const float CCourse::m_fCreateDistance = 600.0f;	// �����Ԋu

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCourse::CCourse(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_pCollisionLineBox.clear();	// �����蔻��{�b�N�X
	m_vecSegmentPosition.clear();	// ��_�̈ʒu
	m_vecVtxInfo.clear();			// �e���_�̏��
	m_courceLength = 0.0f;
	m_fTexU = 0.0f;	// U�X�N���[���p
	m_fTexV = 0.0f;	// V�X�N���[���p
	m_fSinCurve = 0.0f;	// �T�C���J�[�u�̈ړ���
	m_fWaveHeight = HEIGHT_SINCURVE;	// �T�C���J�[�u�̍���
	m_bEnableWave = true;	// �g�̗L������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCourse::~CCourse()
{

}

//==========================================================================
// ��������
//==========================================================================
CCourse* CCourse::Create(const std::string& file, CScene::MODE mode)
{
	// �������̊m��
	CCourse* pObjMeshField = nullptr;

	switch (mode)
	{
	case CScene::MODE_TITLE:
		pObjMeshField = DEBUG_NEW CCourse_Title;
		break;

	case CScene::MODE_GAME:
		pObjMeshField = DEBUG_NEW CCourse;
		break;
	
	default:
		return pObjMeshField;
		break;
	}

	if (pObjMeshField != nullptr)
	{
		pObjMeshField->m_FileName = file;

		// ����������
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCourse::Init()
{
	HRESULT hr;

	// ���[�h����
	Load(m_FileName);

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �I�u�W�F�N�g3D���b�V���̏���������
	Reset();

	// ���_���ݒ�
	SetVtx();


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// �S�Ă̗v�f����������
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));



	// ���_���W�v�Z
	SetVtxPosition();

	SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCourse::Uninit()
{
	m_pCollisionLineBox.clear();

	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �e���_�v�Z
//==========================================================================
void CCourse::CalVtxPosition()
{

	// �ŏ��ƍŌ�A�t�����ɏ����o��
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// �ŏ�
	angle = m_vecSegmentPosition[1].AngleXZ(m_vecSegmentPosition[0]);
	begin = MyLib::Vector3(
		m_vecSegmentPosition[0].x + sinf(angle) * -10.0f,
		m_vecSegmentPosition[0].y,
		m_vecSegmentPosition[0].z + cosf(angle) * -10.0f);

	// �Ō�
	int endIdx = m_vecSegmentPosition.size() - 1;
	angle = m_vecSegmentPosition[endIdx].AngleXZ(m_vecSegmentPosition[endIdx - 1]);
	end = MyLib::Vector3(
		m_vecSegmentPosition[endIdx].x + sinf(angle) * 10.0f,
		m_vecSegmentPosition[endIdx].y,
		m_vecSegmentPosition[endIdx].z + cosf(angle) * 10.0f);

	m_vecSegmentPosition.insert(m_vecSegmentPosition.begin(), begin);
	m_vecSegmentPosition.push_back(end);


	// ���_���N���A
	m_vecVtxInfo.clear();



	// �Z�O�����g�����v�Z
	std::vector<MyLib::Vector3> vecpos = CalSegmentDivision(m_vecSegmentPosition);

	// pos�̗v�f�n��
	std::transform(vecpos.begin(), vecpos.end(), std::back_inserter(m_vecVtxInfo), MyConvert::Vector3ToVtxInfo);
}

//==========================================================================
// �Z�O�����g�����v�Z
//==========================================================================
std::vector<MyLib::Vector3> CCourse::CalSegmentDivision(const std::vector<MyLib::Vector3>& segment)
{
	// �Z�O�����g�̒������v�Z
	int segmentSize = segment.size();
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
		vecLength[i] = segment[i].Distance(segment[next]);
	}

	// �e���_�i�[
	m_courceLength = 0.0f;
	std::vector<MyLib::Vector3> vecpos;

	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += m_fCreateDistance;

			if (distance >= vecLength[i])
			{
				m_courceLength += m_fCreateDistance - (distance - vecLength[i]);

				distance = vecLength[i];

				vecpos.push_back(MySpline::GetSplinePosition_NonLoop(segment, m_courceLength, 20.0f));
				break;
			}

			m_courceLength += m_fCreateDistance;
			vecpos.push_back(MySpline::GetSplinePosition_NonLoop(segment, m_courceLength));
		}
	}

	return vecpos;
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCourse::Reset()
{
	// �I������
	CObject3DMesh::Uninit();

	// ���S�t���O�������Z�b�g
	SetEnableDeath(false);


	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// �e���_�v�Z
	CalVtxPosition();

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 0.5f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_vecVtxInfo.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();

	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}

	// ���_���W�v�Z
	SetVtxPosition();
	SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCourse::ReCreateVtx()
{

	// �I������
	CObject3DMesh::Uninit();

	// ���S�t���O�������Z�b�g
	SetEnableDeath(false);


	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �ŏ��ƍŌ������
	m_vecSegmentPosition.erase(m_vecSegmentPosition.begin());
	m_vecSegmentPosition.pop_back();

	// �e���_�v�Z
	CalVtxPosition();

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 10.0f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_vecVtxInfo.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������
	m_fSinCurve = 0.0f;	// �T�C���J�[�u�̈ړ���

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// �S�Ă̗v�f����������
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(FILLCOLOR.x, FILLCOLOR.y, FILLCOLOR.z, FILLCOLOR.w));


	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}

	// ���_���W�v�Z
	SetVtxPosition();

	SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));
}

//==========================================================================
// �����̒��_���W�v�Z
//==========================================================================
void CCourse::CalBothVtxPosition()
{
#if 0
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;


	int segmentSize = static_cast<int>(m_vecSegmentPosition.size());

	std::vector<MyLib::Vector3> vecLeft, vecRight;
	MyLib::Vector3 rot;

	// ���E�̃Z�O�����g�v�Z
	for (int y = 0; y < segmentSize; y++)
	{
		// �C���f�b�N�X�v�Z
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// �e��}�g���b�N�X������
		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % segmentSize;

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		// ���̃Z�O�����g�Ƃ̌����擾
		rot.y = m_vecSegmentPosition[next].AngleXZ(m_vecSegmentPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{// �I�[�̂�
			rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecSegmentPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);




		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		// ���_���W���
		vecLeft.push_back(mtxLeft.GetWorldPosition());
		vecRight.push_back(mtxRight.GetWorldPosition());

		CEffect3D::Create(
			vecLeft.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			vecRight.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}


	// ���_���W
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int idx = 0;

	// �Z�O�����g�����v�Z
	std::vector<MyLib::Vector3> vecpos;

	// ��
	vecpos.clear();
	vecpos = CalSegmentDivision(vecLeft);
	idx = 0;
	for (int i = 0; i < static_cast<int>(vecpos.size()) - 1; i++)
	{
		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

	// �E
	vecpos.clear();
	vecpos = CalSegmentDivision(vecRight);
	idx = 1;
	for (int i = 0; i < static_cast<int>(vecpos.size()); i++)
	{
		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}
#else

	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;


	int segmentSize = static_cast<int>(m_vecSegmentPosition.size());

	std::vector<MyLib::Vector3> vecLeft, vecRight;
	MyLib::Vector3 rot;

	// ���E�̃Z�O�����g�v�Z
	for (int y = 0; y < segmentSize; y++)
	{
		// �C���f�b�N�X�v�Z
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// �e��}�g���b�N�X������
		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % segmentSize;

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		// ���̃Z�O�����g�Ƃ̌����擾
		rot.y = m_vecSegmentPosition[next].AngleXZ(m_vecSegmentPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{// �I�[�̂�
			rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecSegmentPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);




		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		// ���_���W���
		vecLeft.push_back(mtxLeft.GetWorldPosition());
		vecRight.push_back(mtxRight.GetWorldPosition());

		CEffect3D::Create(
			vecLeft.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			vecRight.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}


	// ���_���W
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int idx = 0;

	// �Z�O�����g�����v�Z
	std::vector<MyLib::Vector3> vecpos;

	// ��
	vecpos.clear();
	vecpos = CalSegmentDivision(vecLeft);
	idx = 0;
	for (int i = 0; i < GetNumVertex() / 2; i++)
	{
		if (vecpos.size() <= i)
		{
			break;
		}

		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

	// �E
	vecpos.clear();
	vecpos = CalSegmentDivision(vecRight);
	idx = 1;
	for (int i = 0; i < GetNumVertex() / 2; i++)
	{
		if (vecpos.size() <= i)
		{
			break;
		}

		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

#endif

}

//==========================================================================
// �X�V����
//==========================================================================
void CCourse::Update()
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

#if _DEBUG

	static float velocity = SCROLL_VELOCITY;
	if (ImGui::TreeNode("Course"))
	{
		ImGui::Checkbox("Enable Wave!!!", &m_bEnableWave);
		ImGui::DragFloat("velocity", &velocity, 0.1f, 0.0f, 0.0f, "%.2f");
		/*ImGui::DragFloat("intervalWave", &INTERVAL_SINCURVE, 10.0f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("waveHeight", &HEIGHT_SINCURVE, 0.1f, 0.0f, 0.0f, "%.2f");*/
		SetPosition(MyLib::Vector3(0.0f, m_fWaveHeight, 0.0f));

		ImGui::TreePop();
	}

	// �T�C���J�[�u�̈ړ���
	m_fSinCurve -= velocity;

	// �J���[�G�f�B�b�g
	static ImVec4 myColor = ImVec4(0.80, 0.98, 1.0, 0.46f); // RGBA

	if (ImGui::ColorEdit4("Color", &myColor.x))
	{
		D3DXCOLOR* pVtxCol = GetVtxCol();

		// �S�Ă̗v�f����������
		std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(myColor.x, myColor.y, myColor.z, myColor.w));
	}

#else

	// �T�C���J�[�u�̈ړ���
	m_fSinCurve -= SCROLL_VELOCITY;

#endif

	// ���_���W
	SetVtx();
}

//==========================================================================
// ���_���W
//==========================================================================
void CCourse::SetVtxPosition()
{
#if 0
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();

	MyLib::Vector3 beforeVec = MyLib::Vector3(0.0f, 0.0f, 1.0f);

	for (int y = 0; y < static_cast<int>(m_vecVtxInfo.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % static_cast<int>(m_vecVtxInfo.size());

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		m_vecVtxInfo[y].rot.y = m_vecVtxInfo[next].pos.AngleXZ(m_vecVtxInfo[y].pos);
		UtilFunc::Transformation::RotNormalize(m_vecVtxInfo[y].rot.y);

		if (bEnd)
		{
			m_vecVtxInfo[y].rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(m_vecVtxInfo[y].rot.y, m_vecVtxInfo[y].rot.x, m_vecVtxInfo[y].rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecVtxInfo[y].pos);
		mtxParent.Multiply(mtxParent, mtxTrans);

		if (y == 1)
		{// ����X�L�b�v
			CEffect3D::Create(
				m_vecVtxInfo[y].pos + GetPosition(),
				0.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				20.0f, 4, CEffect3D::MOVEEFFECT::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}

		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		//mtxLeft.Multiply(mtxLeft, mtxRotate);
		ImGui::Text("x:%f y:%f z:%f, rot.y:%f", m_vecVtxInfo[y].pos.x, m_vecVtxInfo[y].pos.y, m_vecVtxInfo[y].pos.z, m_vecVtxInfo[y].rot.y);

		// ���_���W���
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();


		if (y > 0)
		{// ����X�L�b�v

			int up = idx - WIDTH_BLOCK, down = nextidx - WIDTH_BLOCK;
			int before = y - 1;



			// �x�N�g��AB��90�x�����v���ɉ�]
			MyLib::Vector3 rotatedB = (m_vecVtxInfo[before].pos - m_vecVtxInfo[y].pos).rotateAroundY();

			// �V�����x�N�g���̏I�_���v�Z
			MyLib::Vector3 newB = m_vecVtxInfo[y].pos + rotatedB;


			MyLib::Vector3 AB = newB - m_vecVtxInfo[y].pos;
			MyLib::Vector3 AP = pVtxPos[idx] - m_vecVtxInfo[y].pos;

			// ����̓_���O��̂������ɂ������瓯�n�_�ɒu��
			// ��
			float crossProductY = AB.x * AP.z - AB.z * AP.x;
			if (crossProductY > 0)
			{
				pVtxPos[idx] = pVtxPos[up];

				CEffect3D::Create(
					pVtxPos[idx] + GetPosition(),
					0.0f,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					80.0f, 2, CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI, CEffect3D::TYPE_NORMAL);
			}

			AP = pVtxPos[nextidx] - m_vecVtxInfo[y].pos;

			// ��
			crossProductY = AB.x * AP.z - AB.z * AP.x;
			if (crossProductY > 0)
			{
				pVtxPos[nextidx] = pVtxPos[down];
			}

		}



	}

#else
	// �����̒��_���W�v�Z
	CalBothVtxPosition();
#endif

	int i = 0;
	MyLib::Vector3 fieldpos = GetPosition();
	for (const auto& vtx : m_vecSegmentPosition)
	{
		if (m_pCollisionLineBox.empty()) break;

		MyLib::Vector3 setpos = vtx + fieldpos;
		m_pCollisionLineBox[i]->SetPosition(setpos);
		i++;
	}

}

//==========================================================================
// �����擾
//==========================================================================
float CCourse::GetHeight(const MyLib::Vector3& pos, bool* pLand)
{
	// �x�N�g���Ɩ@��
	MyLib::Vector3 vec1, vec2, nor;
	float fHeight = 0.0f;
	MyLib::Vector3* pVtxPos = GetVtxPos();

	// �t�B�[���h�̈ʒu
	MyLib::Vector3 posfield = GetPosition();
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();

	// �����擾
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	// �ő�̒���
	float fMaxWidthLen = fWidthLen * GetWidthBlock();
	float fMaxHeightLen = -fWidthLen * GetHeightBlock();

	// ���肷�钸�_�̌v�Z
	int nCntWidth = 0;
	int nCntHeight = 0;
	int nWidthPoint = static_cast<int>(((pos.x - posfield.x) - fMaxWidthLen * 0.5f) / fWidthLen + GetWidthBlock());
	int nHeightPoint = static_cast<int>(GetHeightBlock() - ((pos.z - posfield.z) - fMaxHeightLen * 0.5f) / fHeightLen);

	for (int nCntH = 0; nCntH < 2; nCntH++)
	{
		for (int nCntW = 0; nCntW < 2; nCntW++)
		{
			// �����_
			nCntWidth = nWidthPoint + (nCntW - 1);
			nCntHeight = nHeightPoint + (nCntH - 1);

			if (nCntWidth < 0 || nWidth < nCntWidth)
			{// �͈͊O�Ő擪
				continue;
			}

			if (nCntHeight < 0 || nHeight < nCntHeight)
			{// �͈͊O�Ő擪
				continue;
			}

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (nWidth + 1));
			int nVerTexW = (nWidth + 1) + 1;

			int nLeft = nCntWidth + (nCntHeight * (nWidth + 1));
			int nRight = nCntWidth + (nCntHeight * (nWidth + 1)) + nVerTexW;

			if (nLeft >= GetNumVertex() || nRight >= GetNumVertex())
			{
				continue;
			}

			fHeight = UtilFunc::Calculation::GetVtxHeight(pos, pVtxPos[nNowPoint] + posfield, pVtxPos[nLeft] + posfield, pVtxPos[nRight] + posfield, pLand);
			if (*pLand)
			{
				break;
			}

			fHeight = UtilFunc::Calculation::GetVtxHeight(pos, pVtxPos[nNowPoint + GetWidthBlock()] + posfield, pVtxPos[nRight] + posfield, pVtxPos[nLeft] + posfield, pLand);
			if (*pLand)
			{
				break;
			}
		}
	}

	if (*pLand)
	{// ���n���Ă�����

		return fHeight;
	}

	// �������擾
	return pos.y;
}

//==========================================================================
// �����̂��钸�_�̃C���f�b�N�X�擾
//==========================================================================
int CCourse::GetMyVtxIndex(const MyLib::Vector3& _pos)
{
	int idx = -1;
	for (const auto& vtxInfo : m_vecVtxInfo)
	{
		const MyLib::Vector3& pos = vtxInfo.pos;

		if (pos.x <= _pos.x)
		{
			idx++;
		}
		else
		{
			break;
		}
	}

	return idx;
}

//==========================================================================
// ���_����Ȃ镽�ʂƂ̓����蔻��
//==========================================================================
bool CCourse::CollisionVtxQuad(int idx, MyLib::Vector3 rayPos, MyLib::Vector3* colPos)
{
	// ��O����
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return false;
	if (idx < 0) return false;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rayVec(0.0f, -1.0f, 0.0f);

	// ���C��4�_�̔���
	int calIdx = idx * 2;
	float raylen = 90000.0f;

	// ���v���
	//MyLib::Vector3 pos0(pVtxPos[calIdx + 1]), pos1(pVtxPos[calIdx + 3]), pos2(pVtxPos[calIdx + 2]), pos3(pVtxPos[calIdx + 0]);
	MyLib::Vector3 pos0(pVtxPos[calIdx + 3]), pos1(pVtxPos[calIdx + 1]), pos2(pVtxPos[calIdx + 0]), pos3(pVtxPos[calIdx + 2]);

	pos0 += GetPosition();
	pos1 += GetPosition();
	pos2 += GetPosition();
	pos3 += GetPosition();

	bool bHit = UtilFunc::Collision::CollisionRayQuad(&rayPos, &rayVec,
		&pos0, &pos1, &pos2, &pos3,
		raylen, colPos);
	//*colPos += GetPosition();

#if 0

	MyLib::Vector3 aaaa(800.0f, 2000.0f, 0.0f), koutne;
	int courseIdx = GetMyVtxIndex(aaaa);
	courseIdx *= 2;

	MyLib::Vector3 Apos0(pVtxPos[courseIdx + 3]), Apos1(pVtxPos[courseIdx + 1]), Apos2(pVtxPos[courseIdx + 0]), Apos3(pVtxPos[courseIdx + 2]);
	Apos0 += GetPosition();
	Apos1 += GetPosition();
	Apos2 += GetPosition();
	Apos3 += GetPosition();

	UtilFunc::Collision::CollisionRayQuad(&aaaa, &rayVec,
		&Apos0, &Apos1, &Apos2, &Apos3,
		raylen, &koutne);
	//koutne += GetPosition();

	CEffect3D::Create(
		koutne,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);


	CEffect3D::Create(
		pos0,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		pos1,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		pos2,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		pos3,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	ImGui::Text("rot:x%.2f, %.2f, %.2f", pos1.x, pos1.y, pos1.z);
	ImGui::Text("rot:x%.2f, %.2f, %.2f", pos2.x, pos2.y, pos2.z);
	ImGui::Text("rot:x%.2f, %.2f, %.2f", pos3.x, pos3.y, pos3.z);
#endif

	return bHit;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCourse::Draw()
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
void CCourse::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3 *pVtxNor = GetVtxNor();
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3 vec1, vec2, nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxInfo.size());

	// �e�N�X�`�����
	float posU_UP = 0.0f, posU_Down = 0.0f, posV = 0.0f;
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	float intervalV = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU).y;
	int idx = 0, nextIdx = 0;

	float sincurveDistance = 0.0f;
	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		// ���Z�b�g
		posV = 0.0f;

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// ���̕������J��Ԃ�

			// ����̒��_�C���f�b�N�X
			idx = nCntWidth + (nCntHeight * (nWidth + 1));

			// �g
			if (m_bEnableWave)
			{
				if (nCntHeight != 0)
				{
					// �O��Ƃ̋���
					sincurveDistance += m_vecVtxInfo[nCntHeight].pos.DistanceXZ(m_vecVtxInfo[nCntHeight - 1].pos);
					pVtxPos[idx].y = sinf(D3DX_PI * ((sincurveDistance + m_fSinCurve) / INTERVAL_SINCURVE)) * m_fWaveHeight;
				}
			}
			else
			{
				pVtxPos[idx].y = 0.0f;
			}

			// �ׂ̃C���f�b�N�X
			nextIdx = idx + 2;

			// UV���W�ݒ�
			if (nCntWidth == 0)
			{
				pTex[idx] = D3DXVECTOR2(posU_UP + m_fTexU, posV + m_fTexV);


				// ���̊������i�߂�
				if (nextIdx < vtxNum)
				{
					posU_UP += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 2]) / INTERVAL_TEXU;
				}
				
			}
			else
			{
				pTex[idx] = D3DXVECTOR2(posU_Down + m_fTexU, posV + m_fTexV);

				// ���̊������i�߂�
				if (nextIdx < vtxNum)
				{
					posU_Down += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 2]) / INTERVAL_TEXU;
				}
			}

			// �c�̊������i�߂�
			posV += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 1]) / intervalV;
			
			pVtxNor[idx] = nor;
		}
	}

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ���[�h����
//==========================================================================
HRESULT CCourse::Load(const std::string& file)
{
	// �t�@�C�����J��
	std::ifstream File(file, std::ios::binary);
	if (!File.is_open()) {

		// ��O����
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 500.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1000.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });

		MyLib::AABB aabb(-25.0f, 25.0f);

		for (int i = 0; i < static_cast<int>(m_vecSegmentPosition.size()); i++)
		{
			m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		}

		// �I�u�W�F�N�g3D���b�V���̏���������
		Reset();

		Save();
		return E_FAIL;
	}

	// �\���̂̃T�C�Y���擾
	std::streamsize structSize = sizeof(MyLib::Vector3);
	
	// �t�@�C���̖����܂Ńf�[�^��ǂݍ���
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);
	
	// �f�[�^�̌����v�Z
	size_t numVectors = fileSize / structSize;
	
	// �x�N�g���̔z���p��
	m_vecSegmentPosition.resize(numVectors);
	
	// �t�@�C������f�[�^��ǂݍ���
	File.read(reinterpret_cast<char*>(m_vecSegmentPosition.data()), fileSize);

	// �t�@�C�������
	File.close();

#if 0
	int i = 0;
	for (auto& vvv : m_vecSegmentPosition)
	{
		if (i != 0)
		{
			vvv.x *= 1.25f;
		}
		i++;
	}
	//Save();
#endif

	return S_OK;
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CCourse::Save()
{
	// �t�@�C�����J��
	std::ofstream File(m_FileName, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	std::vector<MyLib::Vector3> savedata = m_vecSegmentPosition;
	savedata.erase(savedata.begin());
	savedata.pop_back();


	// �f�[�^���o�C�i���t�@�C���ɏ����o��
	File.write(reinterpret_cast<char*>(savedata.data()), savedata.size() * sizeof(MyLib::Vector3));
		
	// �t�@�C�������
	File.close();
}

//==========================================================================
// �����蔻��{�b�N�X�擾
//==========================================================================
CCollisionLine_Box* CCourse::GetCollisionLineBox(int idx)
{
	if (static_cast<int>(m_pCollisionLineBox.size()) <= idx) return nullptr;

	return m_pCollisionLineBox[idx];
}

//==========================================================================
// �e���_���̈ʒu�擾
//==========================================================================
CCourse::VtxInfo CCourse::GetVecVtxinfo(int idx)
{
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return VtxInfo();

	return m_vecVtxInfo[idx];
}

//==========================================================================
// �e���_���̈ʒu�ݒ�
//==========================================================================
void CCourse::SetVecVtxinfo(int idx, const VtxInfo& vecinfo)
{
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return;

	m_vecVtxInfo[idx] = vecinfo;
}

//==========================================================================
// ��_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CCourse::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// ��_�̈ʒu�ݒ�
//==========================================================================
void CCourse::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}

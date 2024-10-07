//=============================================================================
// 
//  �}�b�v�̏�Q������ [map_obstacle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "map_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"
#include "map_obstacle_obj.h"
#include "map_obstacle_motion.h"
#include "obstacle_fisharch.h"
#include "obstacle_birdcircle.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CMap_Obstacle> CMap_Obstacle::m_List = {};	// ���X�g
std::map<int, CListManager<CMap_Obstacle>> CMap_Obstacle::m_ListBlock = {};	// ���X�g
const float CMap_Obstacle::m_DISTANCE_COLLISION_BLOCK = 2000.0f;	// �����蔻��

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMap_Obstacle::CMap_Obstacle(int nPriority,
	CObject::LAYER layer) : CObject(nPriority, layer)
{
	// �l�̃N���A
	m_nMapBlock = 0;		// �}�b�v�̃u���b�N
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMap_Obstacle::~CMap_Obstacle()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CMap_Obstacle *CMap_Obstacle::Create(const CMap_ObstacleManager::SObstacleInfo& info, const MyLib::Vector3& pos, const bool bChange, const bool bSave)
{
	CMap_Obstacle* pObj = nullptr;
	// �������̊m��
	if (bChange)	// �W���Ő������邩�ۂ�
	{// ���̂Ȃ��̐e�I�u�W�F�N�g

		switch (info.type)
		{
		case T::SAKANA:

			pObj = CObstacle_FishArch::Create(info);
			break;

		case T::BIRD:

			pObj = CObstacle_BirdCircle::Create(info);
			break;

		default:

			pObj = CMap_Obstacle_Object::Create(info,bChange,bSave);
			break;
		}
	}
	else {	// ���̂���̒P��
		switch (info.type)
		{
		case T::SAKANA:

			pObj = CMap_Obstacle_Motion::Create(info);
			break;

		case T::BIRD:

			pObj = CMap_Obstacle_Motion::Create(info);
			break;

		default:

			pObj = CMap_Obstacle_Object::Create(info, bChange, bSave);
			break;
		}
	}

	if (pObj != nullptr)
	{
		pObj->m_ObstacleInfo = info;
		pObj->m_bSave = bSave;

		// �ʒu�ݒ�
		pObj->SetPosition(pos);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMap_Obstacle::Init()
{
	// �����̃u���b�N�v�Z
	CalMyBlock();

	// ���X�g�ɒǉ�
	m_ListBlock[m_nMapBlock].Regist(this);


	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ��Q�����
	m_OriginObstacleInfo = m_ObstacleInfo;

#if _DEBUG

	for (const auto& box : m_ObstacleInfo.boxcolliders)
	{
		CCollisionLine_Box* pBox = CCollisionLine_Box::Create(MyLib::AABB(box.vtxMin, box.vtxMax), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// �����蔻��{�b�N�X
		m_pCollisionLineBox.push_back(pBox);
	}

#endif

	// ���_�召�v�Z
	//CalVtxMinMax();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMap_Obstacle::Uninit()
{
	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	// ���X�g����폜
	m_List.Delete(this);
	m_ListBlock[m_nMapBlock].Delete(this);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜
//==========================================================================
void CMap_Obstacle::Kill()
{
	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	// ���X�g����폜
	m_List.Delete(this);
	m_ListBlock[m_nMapBlock].Delete(this);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMap_Obstacle::Update()
{
	for (auto& collider : m_ObstacleInfo.boxcolliders)
	{
		collider.TransformOffset(GetWorldMtx());
	}

#if _DEBUG
	int i = 0;
	for (const auto& box : m_pCollisionLineBox)
	{
		MyLib::Collider_BOX& collider = m_ObstacleInfo.boxcolliders[i];

		collider.TransformOffset(GetWorldMtx());
		box->SetPosition(collider.worldmtx.GetWorldPosition());
		box->SetRotation(GetRotation());
		box->SetAABB(MyLib::AABB(collider.vtxMin, collider.vtxMax), GetScale().x);
		i++;
	}

	// ���_�召�v�Z
	//CalVtxMinMax();

#endif
}

//==========================================================================
// �q�b�g����
//==========================================================================
void CMap_Obstacle::Hit()
{
	return;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMap_Obstacle::Draw()
{
	
}

void CMap_Obstacle::CalVtxMinMax()
{
	m_vtxMin = MyLib::Vector3();
	m_vtxMax = MyLib::Vector3();

	for (auto& obj : m_ObstacleInfo.boxcolliders)
	{
		MyLib::Vector3 vtxMin, vtxMax;
		vtxMin = obj.vtxMin + obj.offset;
		vtxMax = obj.vtxMax + obj.offset;

		// X
		if (m_vtxMin.x > vtxMin.x)
		{// ���̍ŏ��l��������̒l����������������

			m_vtxMin.x = vtxMin.x;
		}
		if (m_vtxMax.x < vtxMax.x)
		{// ���̍ő�l��������̒l���傫��������

			m_vtxMax.x = vtxMax.x;
		}

		// Y
		if (m_vtxMin.y > vtxMin.y)
		{// ���̍ŏ��l��������̒l����������������

			m_vtxMin.y = vtxMin.y;
		}
		if (m_vtxMax.y < vtxMax.y)
		{// ���̍ő�l��������̒l���傫��������

			m_vtxMax.y = vtxMax.y;
		}

		// Z
		if (m_vtxMin.z > vtxMin.z)
		{// ���̍ŏ��l��������̒l����������������

			m_vtxMin.z = vtxMin.z;
		}
		if (m_vtxMax.z < vtxMax.z)
		{// ���̍ő�l��������̒l���傫��������

			m_vtxMax.z = vtxMax.z;
		}
	}
}

//==========================================================================
// �����̃u���b�N�v�Z
//==========================================================================
void CMap_Obstacle::CalMyBlock()
{
	m_nMapBlock = 0;
	float distanceX = GetPosition().x;
	while (1)
	{
		// �Ԋu�����Z
		distanceX -= m_DISTANCE_COLLISION_BLOCK;
		if (distanceX <= 0.0f)
		{
			break;
		}

		// �u���b�N���Z
		m_nMapBlock++;
	}
}

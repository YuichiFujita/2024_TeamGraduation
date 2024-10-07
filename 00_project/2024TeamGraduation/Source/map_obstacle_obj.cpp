//=============================================================================
// 
//  �}�b�v�̃I�u�W�F�N�g�̏�Q�� [map_obstacle_obj.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "map_obstacle_obj.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"
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
CListManager<CMap_Obstacle_Object> CMap_Obstacle_Object::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMap_Obstacle_Object::CMap_Obstacle_Object(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// �l�̃N���A
	m_pObject = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMap_Obstacle_Object::~CMap_Obstacle_Object()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CMap_Obstacle_Object *CMap_Obstacle_Object::Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange, const bool bSave)
{
	CMap_Obstacle_Object* pObj = nullptr;
	// �������̊m��
	pObj = DEBUG_NEW CMap_Obstacle_Object;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
		pObj->SetSave(bSave);

		// ����������
		//pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMap_Obstacle_Object::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// �I�u�W�F�N�g����
	m_pObject = CObjectX::Create(m_ObstacleInfo.modelFile);
	m_pObject->SetPosition(GetPosition());

	return CMap_Obstacle::Init();
}

//==========================================================================
// �I������
//==========================================================================
void CMap_Obstacle_Object::Uninit()
{
	// �����I�u�W�F�N�g�j��
	if (m_pObject != nullptr)
	{
		m_pObject = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CMap_Obstacle::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CMap_Obstacle_Object::Kill()
{
	// �����I�u�W�F�N�g�j��
	if (m_pObject != nullptr)
	{
		m_pObject->Kill();
		m_pObject = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CMap_Obstacle::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMap_Obstacle_Object::Update()
{
	CMap_Obstacle::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMap_Obstacle_Object::Draw()
{
	CMap_Obstacle::Draw();
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CMap_Obstacle_Object::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);
	
	if (m_pObject != nullptr)
	{
		m_pObject->SetPosition(pos);
	}
}

//==========================================================================
// �p�x�ݒ�
//==========================================================================
void CMap_Obstacle_Object::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);

	if (m_pObject != nullptr)
	{
		m_pObject->SetRotation(rot);
	}
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CMap_Obstacle_Object::SetScale(const MyLib::Vector3& scale)
{
	m_pObject->SetScale(scale);
}
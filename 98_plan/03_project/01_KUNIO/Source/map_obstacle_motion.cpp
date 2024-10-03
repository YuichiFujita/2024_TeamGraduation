//=============================================================================
// 
//  �}�b�v�̃��[�V���������Q�� [map_obstacle_motion.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "map_obstacle_motion.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"
#include "obstacle_fisharch.h"
#include "obstacle_birdcircle.h"
#include "model.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CMap_Obstacle_Motion> CMap_Obstacle_Motion::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMap_Obstacle_Motion::CMap_Obstacle_Motion(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// �l�̃N���A
	m_pChara = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMap_Obstacle_Motion::~CMap_Obstacle_Motion()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CMap_Obstacle_Motion *CMap_Obstacle_Motion::Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange, const bool bSave)
{
	CMap_Obstacle_Motion* pObj = nullptr;
	// �������̊m��
	pObj = DEBUG_NEW CMap_Obstacle_Motion;

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
HRESULT CMap_Obstacle_Motion::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	m_OriginObstacleInfo = m_ObstacleInfo;	// ��Q�����

	// �L��������
	if (m_pChara == nullptr)
	{
		m_pChara = CObjectChara::Create(m_ObstacleInfo.modelFile);
		m_pChara->GetMotion()->Set(0);
		m_pChara->CObject::SetType(TYPE_OBJECTX);
		m_pChara->SetPosition(GetPosition());

		return CMap_Obstacle::Init();
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMap_Obstacle_Motion::Uninit()
{
	if (m_pChara != nullptr)
	{
		m_pChara = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CMap_Obstacle::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CMap_Obstacle_Motion::Kill()
{
	if (m_pChara != nullptr)
	{
		m_pChara->Uninit();
		m_pChara = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CMap_Obstacle::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMap_Obstacle_Motion::Update()
{
	CMap_Obstacle::Update();

	// ���[�V�����擾
	CMotion* pMotion = m_pChara->GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->IsFinish())
	{
		// �ʏ탂�[�V�����Đ�
		pMotion->Set(MOTION::MOTION_DEF);
	}
}

//==========================================================================
// �q�b�g����
//==========================================================================
void CMap_Obstacle_Motion::Hit()
{
	// ���[�V�����擾
	CMotion* pMotion = m_pChara->GetMotion();
	if (pMotion == nullptr) return;

	// �q�b�g���[�V�����Đ�
	pMotion->Set(MOTION::MOTION_HIT);

	return;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMap_Obstacle_Motion::Draw()
{
	CMap_Obstacle::Draw();
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CMap_Obstacle_Motion::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);

	if (m_pChara != nullptr)
	{
		m_pChara->SetPosition(pos);
	}
}

//==========================================================================
// �p�x�ݒ�
//==========================================================================
void CMap_Obstacle_Motion::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);

	if (m_pChara != nullptr)
	{
		m_pChara->SetRotation(rot);
	}
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CMap_Obstacle_Motion::SetScale(const MyLib::Vector3& scale)
{
	CModel* p = m_pChara->GetModel(0);
	p->SetOriginScale(scale);
}

//==========================================================================
// �X�P�[���擾
//==========================================================================
MyLib::Vector3 CMap_Obstacle_Motion::GetScale()
{ 
	return m_pChara->GetModel(0)->GetScale(); 
}

//=============================================================================
// 
//  �}�b�v�̃I�u�W�F�N�g�̏�Q���w�b�_�[ [map_obstacle_obj.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_OBJ_H_
#define _MAP_OBSTACLE_OBJ_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "map_obstacle.h"
#include "listmanager.h"
#include "map_obstacleManager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���N���X
class CMap_Obstacle_Object : public CMap_Obstacle
{
public:

	CMap_Obstacle_Object(int nPriority = 5, 
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CMap_Obstacle_Object();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	virtual void Kill();	// �폜
	bool GetSave() { return m_bSave; }
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// ��Q�����擾
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// ��Q�����ݒ�
	virtual MyLib::Matrix GetWorldMtx() override { return m_pObject->GetWorldMtx(); }
	virtual MyLib::Vector3 GetScale() override { return m_pObject->GetScale(); }
	virtual void CalWorldMtx() override { m_pObject->CalWorldMtx(); }
	virtual void SetPosition(const MyLib::Vector3& pos) override;		// �ʒu�ݒ�
	virtual void SetRotation(const MyLib::Vector3& rot) override;		// �����ݒ�
	virtual void SetScale(const MyLib::Vector3& scale) override;			// �T�C�Y�ݒ�
	virtual MyLib::Vector3 GetVtxMin() override { return m_pObject->GetVtxMin(); };			// ���_�̍ŏ��l�擾
	virtual MyLib::Vector3 GetVtxMax() override { return m_pObject->GetVtxMax(); };			// ���_�̍ő�l�擾
	virtual void SetState(CObjectX::STATE state) override { m_pObject->SetState(state); }

	//=============================
	// �ÓI�֐�
	//=============================
	static CMap_Obstacle_Object *Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange = true, const bool bSave = true);	// ��������
	static CListManager<CMap_Obstacle_Object> GetListObj() { return m_List; }				// ���X�g�擾

private:
	
	
	//=============================
	// �����o�֐�
	//=============================
	// ���̑�

	//=============================
	// �����o�ϐ�
	//=============================
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// ��Q�����
	CObjectX* m_pObject;	// �\������I�u�W�F�N�g
	static CListManager<CMap_Obstacle_Object> m_List;	// ���X�g
	bool m_bSave;			// �ۑ����邩�ǂ���

};


#endif
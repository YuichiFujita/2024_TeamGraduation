//=============================================================================
// 
//  �}�b�v�̏�Q���w�b�_�[ [map_obstacle.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_H_
#define _MAP_OBSTACLE_H_		// ��d�C���N���[�h�h�~

#include "object.h"
#include "objectX.h"
#include "listmanager.h"
#include "map_obstacleManager.h"
#include "collisionLine_Box.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���N���X
class CMap_Obstacle : public CObject
{
private:

	enum T
	{
		SAKANA = 12,	// ��
		BIRD = 13,		// ��
	};

public:

	CMap_Obstacle(int nPriority = 5, 
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CMap_Obstacle();

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
	void Save();	// �Z�[�u
	void Load();	// ���[�h
	bool GetSave() { return m_bSave; }
	void SetSave(const bool bSave) { m_bSave = bSave; }
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// ��Q�����擾
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// ��Q�����ݒ�
	virtual MyLib::Matrix GetWorldMtx() { return MyLib::Matrix(); }
	virtual MyLib::Vector3 GetScale() = 0;
	virtual void CalWorldMtx() {}
	virtual void SetPosition(const MyLib::Vector3& pos) { CObject::SetPosition(pos); }		// �ʒu�ݒ�
	virtual void SetRotation(const MyLib::Vector3& rot) { CObject::SetRotation(rot); }		// �����ݒ�
	virtual void SetScale(const MyLib::Vector3& scale) = 0;
	virtual MyLib::Vector3 GetVtxMin() { return m_vtxMin; };			// ���_�̍ŏ��l�擾
	virtual MyLib::Vector3 GetVtxMax() { return m_vtxMax; };			// ���_�̍ő�l�擾
	virtual void SetState(CObjectX::STATE state){}
	std::vector<CCollisionLine_Box*> GetCollisionLineBox() { return m_pCollisionLineBox; }

	virtual void Hit();	// �q�b�g����
	void CalMyBlock();	// �����̃u���b�N�v�Z

	//=============================
	// �ÓI�֐�
	//=============================
	static CMap_Obstacle *Create(const CMap_ObstacleManager::SObstacleInfo& info, const MyLib::Vector3& pos, const bool bChange = true, const bool bSave = true);	// ��������
	static CListManager<CMap_Obstacle> GetListObj() { return m_List; }				// ���X�g�擾
	static CListManager<CMap_Obstacle> GetListByBlock(int block) { return m_ListBlock[block]; }		// ���X�g�擾
	static void ListRegist(CMap_Obstacle* pObstacle) { m_List.Regist(pObstacle); }
	static const float GetDistance_CollisionBlock() { return m_DISTANCE_COLLISION_BLOCK; };	// �����蔻��u���b�N�̊Ԋu

protected:

	//=============================
	// ���L�����o�ϐ�
	//=============================
	int m_nMapBlock;		// �}�b�v�̃u���b�N

private:

	
	//=============================
	// �����o�֐�
	//=============================
	// ���̑�
	void CalVtxMinMax();

	//=============================
	// �����o�ϐ�
	//=============================
	TYPE m_type;			// ���
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// ��Q�����
	CMap_ObstacleManager::SObstacleInfo m_OriginObstacleInfo;	// ��Q�����
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// �����蔻��{�b�N�X
	MyLib::Vector3 m_vtxMin, m_vtxMax;
	static CListManager<CMap_Obstacle> m_List;	// ���X�g

	static std::map<int, CListManager<CMap_Obstacle>> m_ListBlock;	// ���X�g
	static const float m_DISTANCE_COLLISION_BLOCK;	// �����蔻��u���b�N�̊Ԋu
	bool m_bSave;			// �ۑ����邩�ǂ���

};


#endif
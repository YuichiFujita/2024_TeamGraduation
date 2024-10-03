//=============================================================================
// 
//  �}�b�v�̃��[�V���������Q���w�b�_�[ [map_obstacle_motion.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_MOTION_H_
#define _MAP_OBSTACLE_MOTION_H_		// ��d�C���N���[�h�h�~

#include "objectChara.h"
#include "map_obstacle.h"
#include "listmanager.h"
#include "map_obstacleManager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���N���X
class CMap_Obstacle_Motion : public CMap_Obstacle
{
private:

	//=============================
	// �񋓌^��`
	//=============================
	enum T
	{
		SAKANA = 12,	// ��
		BIRD = 13,		// ��
	};

	enum MOTION
	{
		MOTION_DEF = 0,
		MOTION_HIT,
		MOTION_MAX
	};

public:

	CMap_Obstacle_Motion(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CMap_Obstacle_Motion();

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
	virtual void Kill() override;	// �폜
	bool GetSave() { return m_bSave; }
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// ��Q�����擾
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// ��Q�����ݒ�
	virtual MyLib::Matrix GetWorldMtx() override { return m_pChara->GetWorldMtx(); }
	virtual void SetPosition(const MyLib::Vector3& pos) override;		// �ʒu�ݒ�
	virtual void SetRotation(const MyLib::Vector3& rot) override;		// �����ݒ�
	virtual void SetScale(const MyLib::Vector3& scale)  override;		// �T�C�Y�ݒ�
	virtual void CalWorldMtx() override { m_pChara->CalWorldMtx(); }
	virtual MyLib::Vector3 GetScale() override;

	virtual void Hit() override;	// �q�b�g����

	//=============================
	// �ÓI�֐�
	//=============================
	static CMap_Obstacle_Motion *Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange = true, const bool bSave = true);	// ��������
	static CListManager<CMap_Obstacle_Motion> GetListObj() { return m_List; }				// ���X�g�擾

private:

	
	//=============================
	// �����o�֐�
	//=============================
	// ���̑�

	//=============================
	// �����o�ϐ�
	//=============================
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// ��Q�����
	CMap_ObstacleManager::SObstacleInfo m_OriginObstacleInfo;	// ��Q�����
	CObjectChara* m_pChara;
	static CListManager<CMap_Obstacle_Motion> m_List;	// ���X�g
	bool m_bSave;			// �ۑ����邩�ǂ���

};


#endif
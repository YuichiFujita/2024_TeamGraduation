//=============================================================================
// 
//  �A�[�`��̋���Q���w�b�_�[ [obstacle_fisharch.h]
//  Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _OBSTACLE_FISHARCH_H_
#define _OBSTACLE_FISHARCH_H_		// ��d�C���N���[�h�h�~

#include "map_obstacle.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���N���X
class CObstacle_FishArch : public CMap_Obstacle
{
private:

	// �����\����
	struct FishInfo
	{
		MyLib::Vector3 offset;	// �ݒ�ʒu
		CMap_Obstacle* pFish;	// ��Q��
		float fLength;			// ����
		int nIdx;

		// �R���X�g���N�^
		FishInfo() : offset(), pFish(nullptr), fLength(0.0f), nIdx(0) {}
	};

	// �{�̂̉Ϗ��
	struct Info
	{
		float fDefHeight;	// �f�t�H���g����
		float fPlusHeight;	// �ǉ��ŐL�т鍂��
		float fRotSpeed;	// ��]���x
		float fNowHeight;	// ���݂̍���

		// �R���X�g���N�^
		Info() : fDefHeight(0.0f), fPlusHeight(0.0f), fRotSpeed(0.0f), fNowHeight(0.0f) {}
	};

public:

	CObstacle_FishArch(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CObstacle_FishArch();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Kill() override;
	virtual MyLib::Matrix GetWorldMtx() override { return m_mtxWorld; }

	//=============================
	// �����o�֐�
	//=============================
	void SetDefHeight(const float fHeight) { m_Info.fDefHeight = fHeight; }
	void SetPlusHeight(const float fHeight) { m_Info.fPlusHeight = fHeight; }
	void SetRotSpeed(const float fRotSpeed) { m_Info.fRotSpeed = fRotSpeed; }
	void SetScale(const MyLib::Vector3& scale) { m_scale = scale; }
	MyLib::Vector3 GetScale() { return m_scale; }

	/**
	@brief	�Ϗ��̐ݒ�
	@param	fDefHeight	[in]	��{�̍���
	@param	fPlusHeight	[in]	�ω����鍂��
	@param	fRotSpd		[in]	��]���x
	@return	void
	*/
	void SetInfo(const float fDefHeight, const float fPlusHeight, const float fRotSpd);	

	//=============================
	// �ÓI�֐�
	//=============================
	static CObstacle_FishArch* Create(const CMap_ObstacleManager::SObstacleInfo& info);

private:


	//=============================
	// �����o�֐�
	//=============================
	// ���̑�
	void MainControll();	// ���C������
	void ControllFish();	// �Ǘ����Ă��鋛�̐ݒ�
	void SetFishOffSet(FishInfo& info);	// ���̃I�t�Z�b�g�ݒ�
	void SetNowHeight();	// �����ݒ�
	void CalWorldMtx();

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<FishInfo> m_FishList;	// ���̃��X�g
	Info m_Info;	// �Ϗ��
	float m_fRot;	// ��]����
	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	MyLib::Vector3 m_scale;
};


#endif

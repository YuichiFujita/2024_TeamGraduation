//=============================================================================
// 
//  �~�`�ŉ�]���钹��Q���w�b�_�[ [obstacle_birdcircle.h]
//  Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _OBSTACLE_BIRDCIRCLE_H_
#define _OBSTACLE_BIRDCIRCLE_H_		// ��d�C���N���[�h�h�~

#include "map_obstacle.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���N���X
class CObstacle_BirdCircle : public CMap_Obstacle
{
private:

	// �����\����
	struct BirdInfo
	{
		MyLib::Vector3 offset;	// �ݒ�ʒu
		CMap_Obstacle* pBird;	// ��Q��
		float fLength;			// ����
		int nIdx;

		// �R���X�g���N�^
		BirdInfo() : offset(), pBird(nullptr), fLength(0.0f), nIdx(0) {}
	};

	// �{�̂̉Ϗ��
	struct Info
	{
		float fDefLength;	// �f�t�H���g����
		float fPlusLength;	// �ǉ��ŐL�т鋗��
		float fRotSpeed;	// ��]���x
		float fNowLength;	// ���݂̋���

		// �R���X�g���N�^
		Info() : fDefLength(0.0f), fPlusLength(0.0f), fRotSpeed(0.0f), fNowLength(0.0f) {}
	};

public:

	CObstacle_BirdCircle(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CObstacle_BirdCircle();

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
	void SetDefLength(const float fLength) { m_Info.fDefLength = fLength; }
	void SetPlusLength(const float fLength) { m_Info.fPlusLength = fLength; }
	void SetRotSpeed(const float fRotSpeed) { m_Info.fRotSpeed = fRotSpeed; }
	void SetScale(const MyLib::Vector3& scale) { m_scale = scale; }
	MyLib::Vector3 GetScale() { return m_scale; }

	/**
	@brief	�Ϗ��̐ݒ�
	@param	fDefLen		[in]	��{�̋���
	@param	fPlusLen	[in]	�ω�����
	@param	fRotSpd		[in]	��]���x
	@return	void
	*/
	void SetInfo(const float fDefLen, const float fPlusLen, const float fRotSpd);

	//=============================
	// �ÓI�֐�
	//=============================
	static CObstacle_BirdCircle* Create(const CMap_ObstacleManager::SObstacleInfo& info);

private:


	//=============================
	// �����o�֐�
	//=============================
	// ���̑�
	void MainControll();	// ���C������
	void ControllBird();	// �Ǘ����Ă��鋛�̐ݒ�
	void SetBirdOffSet(BirdInfo& info);	// ���̃I�t�Z�b�g�ݒ�
	void SetNowLength();	// �����ݒ�
	void CalWorldMtx();

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<BirdInfo> m_BirdList;	// ���̃��X�g
	MyLib::Vector3 m_rot;	// ����
	Info m_Info;	// ���
	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	MyLib::Vector3 m_scale;		// �X�P�[��
	float m_fIntervalWing;		// �H�΂����̊Ԋu
};


#endif

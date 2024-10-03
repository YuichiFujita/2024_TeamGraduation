//=============================================================================
// 
//  �}�b�v�̏�Q���}�l�[�W���w�b�_�[ [map_obstacleManager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MAP_OBSTACLEMANAGER_H_
#define _MAP_OBSTACLEMANAGER_H_		// ��d�C���N���[�h�h�~

#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���}�l�[�W���N���X
class CMap_ObstacleManager
{
public:

	//=============================
	// ��Q�����
	//=============================
	// �Z�b�g�A�b�v���
	struct SObstacleSetup
	{
		int isAir;		// ��C�ђʃt���O
		int isMove;		// �ړ��t���O

		SObstacleSetup() : isAir(0), isMove(0) {}
	};

	// ��Q�����
	struct SObstacleInfo
	{
		SObstacleSetup setup;		// �Z�b�g�A�b�v���
		std::string textFile;		// �e�L�X�g�t�@�C��
		std::string modelFile;		// ���f���t�@�C��
		std::string colliderFile;	// �R���C�_�[�t�@�C��
		std::vector<MyLib::Collider_BOX> boxcolliders;	// BOX�R���C�_�[
		int type;					// ���
		
		SObstacleInfo() : setup(), modelFile(), colliderFile(), boxcolliders(), type(0) {}

		// JSON����̓ǂݍ���
		void from_json(const json& j)
		{
			for (const auto& colliderData : j.at("colliders"))
			{
				MyLib::Collider_BOX collider;
				collider.from_json(colliderData);
				boxcolliders.push_back(collider);
			}
		}

		// JSON�ւ̏�������
		void to_json(json& j) const
		{
			j["colliders"] = json::array(); // ��̔z����쐬

			for (const auto& collider : boxcolliders)
			{
				json colliderJson;
				collider.to_json(colliderJson);

				j["colliders"].emplace_back(colliderJson);
			}
		}
	};



	CMap_ObstacleManager();
	~CMap_ObstacleManager();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	//=============================
	// �����o�֐�
	//=============================
	void Kill();	// �폜

	// ���n
	void SetObstacleInfo(const std::vector<SObstacleInfo>& info) { m_ObstacleInfo = info; }		// ��Q�����ݒ�
	void SetObstacleInfo(const SObstacleInfo& info, int idx) { m_ObstacleInfo[idx] = info; }	// ��Q�����ݒ�
	std::vector<SObstacleInfo> GetObstacleInfo() { return m_ObstacleInfo; }						// ��Q�����擾
	SObstacleInfo GetObstacleInfo(int idx) { return m_ObstacleInfo[idx]; }						// ��Q�����擾
	void AddCollider(int idx);	// �R���C�_�[�ǉ�
	void SubCollider(int idx);	// �R���C�_�[�폜

	// ���o��
	void Save();	// �Z�[�u
	void SaveInfo();	// ���Z�[�u
	void Load();	// ���[�h
	void LoadInfo(const std::string& file);	// ��񃍁[�h

	//=============================
	// �ÓI�֐�
	//=============================
	static CMap_ObstacleManager* Create();	// ����
	static CMap_ObstacleManager* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾
private:

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<SObstacleInfo> m_ObstacleInfo;	// ��Q�����
	static CMap_ObstacleManager* m_ThisPtr;		// ���g�̃|�C���^
};


#endif
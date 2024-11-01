//=============================================================================
// 
//  �I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "meshsphere.h"

class CModel;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�N���X��`
class CObjectChara : public CObject
{
public:

	// �\���̒�`
	struct LoadData
	{
		std::string pModelFile;	// ���f���t�@�C����
		int nType;				// ���f�����
		int nParent;			// �e�̃C���f�b�N�X
		int nStart;				// �ŏ����炠�邩�ǂ���
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
	};

	struct Load
	{
		std::string sTextFile;		// �e�L�X�g�t�@�C����
		int nCenterIdx;				// ���S�ɂ���p�[�c�C���f�b�N�X
		D3DXVECTOR3 centerOffSet;	// ���S�ʒu�̃I�t�Z�b�g
		int nNumModel;				// ���f����
		float fVelocity;			// �ړ����x
		float fRadius;				// ���a
		D3DXVECTOR3 posOrigin;		// �ŏ��̈ʒu
		LoadData LoadData[mylib_const::MAX_MODEL];
	};

	/**
	@brief	�X�t�B�A�R���C�_�[
	*/
	struct SphereCollider
	{
		MyLib::Vector3 center;		// ���S���W
		float radius;				// ���a
		int nParentPartsIdx;		// �e�̃p�[�c�C���f�b�N�X�ԍ�
		MyLib::Vector3 offset;		// �I�t�Z�b�g�ʒu

		// �f�t�H���g�R���X�g���N�^
		SphereCollider() : center(), radius(0.0f), nParentPartsIdx(0), offset() {}

		// �p�����[�^���R���X�g���N�^
		SphereCollider(const MyLib::Vector3& center, float radius, int nIdx, const MyLib::Vector3& offset)
			: center(center), radius(radius), nParentPartsIdx(nIdx), offset(offset) {}

		// JSON����̓ǂݍ���
		void from_json(const json& j)
		{
			j.at("center").get_to(center);
			j.at("radius").get_to(radius);
			j.at("parent").get_to(nParentPartsIdx);
			j.at("offset").get_to(offset);
		}

		// JSON�ւ̏�������
		void to_json(json& j) const
		{
			j = json
			{
				{"center", center},
				{"radius", radius},
				{"parent", nParentPartsIdx},
				{"offset", offset},
			};
		}
	};

	CObjectChara(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CObjectChara();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(D3DXCOLOR col);
	void Draw(int nIdx, float fAlpha);

	void SetmtxWorld(const D3DXMATRIX mtxWorld);
	D3DXMATRIX GetmtxWorld(void) const;			// ���[���h�}�g���b�N�X�擾
	void SetPosition(const D3DXVECTOR3 pos);	// �ʒu�ݒ�
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	void SetOriginPosition(const D3DXVECTOR3 pos);	// �ŏ��̈ʒu�ݒ�
	D3DXVECTOR3 GetOriginPosition(void) const;		// �ŏ��̈ʒu�擾
	void SetOldPosition(const D3DXVECTOR3 pos);	// �O��̈ʒu�ݒ�
	D3DXVECTOR3 GetOldPosition(void) const;		// �O��̈ʒu�擾
	void SetRotation(const D3DXVECTOR3 rot);	// �����ݒ�
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	void SetMove(const D3DXVECTOR3 move);		// �ړ��ʐݒ�
	D3DXVECTOR3 GetMove(void) const;			// �ړ��ʎ擾

	// �L�������
	int GetCenterPartsIdx() { return m_nCenterPartsIdx; }
	void SetCenterPartsIdx(int i) { m_nCenterPartsIdx = i; }
	D3DXVECTOR3 GetCenterOffset() { return m_CenterOffset; }	// ���S�̃I�t�Z�b�g
	void SetCenterOffset(D3DXVECTOR3 p) { m_CenterOffset = p; }
	void SetVelocity(const float fVelocity);	// �ړ����x�ݒ�
	float GetVelocity(void) const;				// �ړ����x�擾
	void SetRadius(const float fRadius);		// ���a�ݒ�
	float GetRadius(void) const;				// ���a�擾
	void SetRotDest(const float fRotDest);		// �ڕW�̌����ݒ�
	float GetRotDest(void) const;				// �ڕW�̌����擾


	HRESULT ReadText(const std::string pTextFile);	// �O���t�@�C���ǂݍ��ݏ���
	HRESULT SetCharacter(const std::string pTextFile);

	void ChangeObject(int nDeleteParts, int nNewParts);
	void SetObject(int nNewParts);

	CObjectChara *GetObjectChara(void);
	int GetNumModel(void);		// ���f�����擾
	void SetNumModel(int nNumModel);		// ���f�����ݒ�
	int GetIdxFile(void);		// �t�@�C���̃C���f�b�N�X�ԍ��擾
	Load GetLoadData(int nIdx);	// �ǂݍ��ݏ��擾
	CModel **GetModel(void);

	SphereCollider GetNowSphereCollider(int nIdx)
	{
		if (nIdx >= 0 && nIdx < (int)m_SphereColliders.size())
		{
			return m_SphereColliders[nIdx];
		}
		return SphereCollider();
	}

	CMeshSphere* GetMeshSphere(int nIdx) { return m_MeshSphere[nIdx]; }

	void SetNowSphereCollider(int nIdx, const SphereCollider& collider)
	{
		if (nIdx < 0 || nIdx >= (int)m_SphereColliders.size())
		{
			return;
		}
		m_SphereColliders[nIdx] = collider;
		m_MeshSphere[nIdx]->SetWidthLen(collider.radius);
		m_MeshSphere[nIdx]->SetHeightLen(collider.radius);
		m_MeshSphere[nIdx]->SetPosition(collider.center);
	}

	void AddCollider(const SphereCollider& collider); // �R���C�_�[�ǉ�

	void SubCollider(void);	// �R���C�_�[�폜

	int GetNumCollider()
	{
		return m_SphereColliders.size();
	}


	// JSON����̓ǂݍ���
	void from_json(const json& j)
	{
		// ���N���X�̓ǂݍ���
		//CObjectHierarchy::from_json(j);

		for (const auto& colliderData : j.at("colliders"))
		{
			SphereCollider collider;
			collider.from_json(colliderData);
			m_SphereColliders.push_back(collider);
		}
	}

	// CObjectChara��JSON�ւ̏�������
	void to_json(json& j) const
	{
		// ���N���X�̏�������
		//CObjectHierarchy::to_json(j);

		j["colliders"] = json::array(); // ��̔z����쐬

		for (const auto& collider : m_SphereColliders)
		{
			json colliderJson;
			collider.to_json(colliderJson);

			j["colliders"].emplace_back(colliderJson);
		}
	}

	void LoadSphereColliders(const std::string textfile);
	void SaveSphereColliders();
private:


	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_posOld;		// �O��̈ʒu
	D3DXVECTOR3 m_posOrigin;	// �ŏ��̈ʒu
	D3DXVECTOR3 m_posCenter;	// ���S�ʒu
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_move;			// �ړ���

	// �L�������
	int m_nCenterPartsIdx;		// ���S�ɂ���p�[�c�̃C���f�b�N�X
	D3DXVECTOR3 m_CenterOffset;	// ���S�̃I�t�Z�b�g
	float m_fVelocity;			// �ړ����x
	float m_fRadius;			// ���a
	float m_fRotDest;			// �ڕW�̌���
	int m_nNumModel;			// ���f���̐�
	int m_nIdxFile;				// �t�@�C���̃C���f�b�N�X�ԍ�
	CModel *m_apModel[mylib_const::MAX_MODEL];	// ���f��(�p�[�c)�̃|�C���^
	std::vector<SphereCollider> m_SphereColliders;	// �X�t�B�A�R���C�_�[
	std::vector<CMeshSphere*> m_MeshSphere;			// ���b�V���X�t�B�A

	std::string m_SetUpFilename;
	static int m_nNumLoad;	// �ǂݍ��񂾐�
	static Load m_aLoadData[mylib_const::MAX_MODEL];
};



#endif
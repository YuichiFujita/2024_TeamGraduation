//=============================================================================
// 
//  �I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_	// ��d�C���N���[�h�h�~

#include "objectHierarchy.h"
#include "motion.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�N���X��`
class CObjectChara : public CObjectHierarchy
{
public:

	//=============================
	// �\���̒�`
	//=============================
	/**
	@brief	�X�t�B�A�R���C�_�[
	*/
	struct SphereCollider
	{
		MyLib::Vector3 center;	// ���S���W
		float radius;			// ���a
		int nParentPartsIdx;	// �e�̃p�[�c�C���f�b�N�X�ԍ�
		MyLib::Vector3 offset;	// �I�t�Z�b�g�ʒu

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

	/**
	@brief	�R���C�_�[���
	*/
	struct ColliderData
	{
		std::vector<SphereCollider> colliders;	// �X�t�B�A�R���C�_�[
		std::string filename;		// �t�@�C����
	};

	CObjectChara(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectChara();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Draw(D3DXCOLOR col) override;
	void Draw(float fAlpha) override;

	void SetHeight(const float fHeight);		// �g���ݒ�
	float GetHeight();						// �g���擾
	void SetVelocity(const float fVelocity);	// �ړ����x�ݒ�
	float GetVelocity() const;				// �ړ����x�擾
	void SetRotDest(const float fRotDest);		// �ڕW�̌����ݒ�
	float GetRotDest() const;				// �ڕW�̌����擾
	void SetLife(const int nLife);				// �̗͐ݒ�
	int GetLife() const;					// �̗͎擾
	void SetLifeOrigin(const int nLife);		// ���̗̑͐ݒ�
	int GetLifeOrigin() const;				// ���̗͎̑擾
	int GetMotionStartIdx() const;			// ���[�V�����J�n�̃C���f�b�N�X�ԍ��擾
	int GetAddScoreValue() const;			// �X�R�A���Z�ʎ擾

	// �R���C�_�[�֘A
	int GetSphereColliderNum();						// �X�t�B�A�R���C�_�[�̐��擾
	SphereCollider GetNowSphereCollider(int nIdx);	// �R���C�_�[�擾
	std::vector<SphereCollider> GetSphereColliders();	// �X�t�B�A�R���C�_�[�擾

	HRESULT SetCharacter(const std::string& file) override;	// �L�����N�^�[�ݒ�
	CObjectChara *GetObjectChara();
	CMotion* GetMotion();	// ���[�V�����I�u�W�F�N�g�擾
	static CObjectChara *Create(const std::string pTextFile);	// ��������


	//=============================
	// JSON����̓ǂݍ���
	//=============================
	void from_json(const json& j)
	{
		for (const auto& colliderData : j.at("colliders")) 
		{
			SphereCollider collider;
			collider.from_json(colliderData);
			m_SphereColliders.push_back(collider);
		}
	}

	//=============================
	// CObjectChara��JSON�ւ̏�������
	//=============================
	void to_json(json& j) const
	{
		j["colliders"] = json::array(); // ��̔z����쐬

		for (const auto& collider : m_SphereColliders)
		{
			json colliderJson;
			collider.to_json(colliderJson);

			j["colliders"].emplace_back(colliderJson);
		}
	}

protected:

	void LoadObjectData(FILE* pFile, const std::string& file) override;	// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
	void LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts) override;		// �p�[�c���̃f�[�^�ǂݍ���
	void BindObjectData(int nCntData) override;							// �I�u�W�F�N�g���̃f�[�^���蓖��
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK);	// �U��������
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK);			// �U�����蒆����
	void ChangeMotion(const char* pMotionFile);	// ���[�V�����t�@�C���ύX

	bool m_bInDicision;	// �U�����蒆�t���O
private:

	//=============================
	// �����o�֐�
	//=============================
	void LoadSphereColliders(const std::string& textfile);
	void SaveSphereColliders();
	void MotionInProgressAction();	// ���[�V�������̍s������

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fHeight;			// �g��
	float m_fVelocity;			// �ړ����x
	float m_fRotDest;			// �ڕW�̌���
	int m_nLife;				// �̗�
	int m_nLifeOrigin;			// ���̗̑�
	int m_nMotionStartIdx;		// ���[�V�����J�n�̃C���f�b�N�X�ԍ�
	int m_nAddScore;			// �X�R�A���Z��
	CMotion *m_pMotion;			// ���[�V�����̏��
	std::vector<SphereCollider> m_SphereColliders;	// �X�t�B�A�R���C�_�[
	static std::vector<ColliderData> m_LoadColliderData;	// �R���C�_�[���

};



#endif
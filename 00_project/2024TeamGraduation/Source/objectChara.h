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
// �O���錾
//==========================================================================
class CCharacterStatus;

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
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Draw(D3DXCOLOR col) override;
	void Draw(float fAlpha) override;
	virtual void Kill() override;	// ���I�폜����

	//--------------------------
	// �L�������
	//--------------------------
	void SetRotDest(const float fRotDest) { m_fRotDest = fRotDest; }		// �ڕW�̌����ݒ�
	float GetRotDest() const { return m_fRotDest; }							// �ڕW�̌����擾
	void SetLife(const int nLife) { m_nLife = nLife; }						// �̗͐ݒ�
	int GetLife() const { return m_nLife; }									// �̗͎擾
	void SetLifeOrigin(const int nLife) { m_nLifeOrigin = nLife; }			// ���̗̑͐ݒ�
	int GetLifeOrigin() const { return m_nLifeOrigin; }						// ���̗͎̑擾
	CCharacterStatus* GetCharStatus() { return m_pStatus; }					// �L�����N�^�[�X�e�[�^�X�擾
	CCharacterStatus::CharParameter GetParameter() { return m_pStatus->GetParameter(); }	// �L�����N�^�[�p�����[�^�[�擾

	//--------------------------
	// �R���C�_�[�֘A
	//--------------------------
	int GetSphereColliderNum() { return static_cast<int>(m_SphereColliders.size()); }	// �X�t�B�A�R���C�_�[�̐��擾
	SphereCollider GetNowSphereCollider(int nIdx);										// �R���C�_�[�擾
	std::vector<SphereCollider> GetSphereColliders() { return m_SphereColliders; }		// �X�t�B�A�R���C�_�[�擾

	HRESULT SetCharacter(const std::string& file) override;		// �L�����N�^�[�ݒ�
	CObjectChara* GetObjectChara() { return this; }				// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
	CMotion* GetMotion() const { return m_pMotion; }			// ���[�V�����I�u�W�F�N�g�擾
	static CObjectChara *Create(const std::string& pTextFile);	// ��������


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

	void LoadObjectData(FILE* pFile, const std::string& file) override;					// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
	void LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts) override;	// �p�[�c���̃f�[�^�ǂݍ���
	void BindObjectData(int nCntData) override;											// �I�u�W�F�N�g���̃f�[�^���蓖��
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK);				// �U��������
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
	void CreateStatus(const CCharacterStatus::CharParameter& parameter);	// �X�e�[�^�X����

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fRotDest;				// �ڕW�̌���
	int m_nLife;					// �̗�
	int m_nLifeOrigin;				// ���̗̑�
	CCharacterStatus* m_pStatus;	// �X�e�[�^�X
	CMotion *m_pMotion;				// ���[�V�����̏��
	std::vector<SphereCollider> m_SphereColliders;	// �X�t�B�A�R���C�_�[
	static std::vector<ColliderData> m_LoadColliderData;	// �R���C�_�[���

};



#endif
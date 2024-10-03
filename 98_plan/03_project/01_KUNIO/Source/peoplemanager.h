//=============================================================================
// 
//  �l�}�l�[�W���w�b�_�[ [peoplemanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PEOPLEMANAGER_H_
#define _PEOPLEMANAGER_H_	// ��d�C���N���[�h�h�~

#include "judge.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPeople;

//==========================================================================
// �N���X��`
//==========================================================================
// �l�}�l�[�W���N���X
class CPeopleManager
{
public:

	//=============================
	// �\���̒�`
	//=============================
	struct SPeopleData
	{
		int nType;			// �L�����N�^�[���
		MyLib::Vector3 pos;	// �ʒu

		SPeopleData() : nType(0), pos(MyLib::Vector3()) {}
		SPeopleData(int _nType,MyLib::Vector3 _pos) : nType(_nType), pos(_pos) {}

		bool operator==(const SPeopleData& other) const
		{
			return (this->nType == other.nType && this->pos == other.pos) ? true : false;
		}
	};

	struct SPattern
	{
		int nNum;	// ����
		int nRank;	// �����N
		std::vector<SPeopleData> data;
	};

	//=============================
	// �񋓌^��`
	//=============================
	enum STATE
	{
		STATE_NONE = 0,		// �����Ȃ����
		STATE_MAX
	};

	enum Type
	{
		TYPE_GAME = 0,
		TYPE_RESULT,
		TYPE_TITLE,
		TYPE_MAX
	};

	CPeopleManager();
	~CPeopleManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	HRESULT ReadText(const std::string& filename);	// �O���t�@�C���ǂݍ��ݏ���
	void SetPeople(const MyLib::Vector3& pos, const MyLib::Vector3& rot, int nPattern);	// �G�z�u
	void SetByRank();	// �����N���Ƃ̃Z�b�g����
	void DespawnPeople();	// �͈͊O�̐l������
	void LateSpawn();		// �㐶�����̐���
	void SetRank(const CJudge::JUDGE& judge) { m_Rank = judge; }			// �����N�ݒ�
	void ResetLateSpawn() { m_lateSpawnPeople.clear(); }

	static CPeopleManager* GetInstance() { return m_ThisPtr; }				// ���g�̃|�C���^
	static CPeopleManager* Create(Type type);


protected:

	//=============================
	// �����o�֐�
	//=============================
	void SetEffect();	// �G�t�F�N�g����

	//=============================
	// �����o�ϐ�
	//=============================
	CJudge::JUDGE m_Rank;							// ���݂̃����N
	CJudge::JUDGE m_OldRank;						// �O��̃����N
	float m_fTimer_EffectSpawn;						// �G�t�F�N�g�����̃^�C�}�[
	float m_fInterval_EffectSpawn;					// �G�t�F�N�g�����̊Ԋu
	std::vector<SPattern> m_vecPattern;				// �z�u�p�^�[��
	std::vector<SPattern> m_PatternByRank[CJudge::JUDGE::JUDGE_MAX];			// �z�u�p�^�[��
	std::list<SPeopleData> m_lateSpawnPeople;		// �㐶���̐l���X�g
	std::vector<std::string> m_vecMotionFileName;	// ���[�V�����t�@�C����
	static CPeopleManager* m_ThisPtr;				// ���g�̃|�C���^
};

// ���U���g�̃}�l�[�W��
class CPeopleManager_Result : public CPeopleManager
{
public:

	CPeopleManager_Result() : CPeopleManager() {}
	~CPeopleManager_Result() {}

	HRESULT Init() override;
	void Update() override;

private:

};


// �^�C�g���̃}�l�[�W��
class CPeopleManager_Title : public CPeopleManager
{
public:

	CPeopleManager_Title() : CPeopleManager() {}
	~CPeopleManager_Title() {}

	HRESULT Init() override;
	void Update() override;

private:

};


#endif
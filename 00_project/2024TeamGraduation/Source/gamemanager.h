//=============================================================================
// 
//  �Q�[���}�l�[�W���w�b�_�[ [gamemanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "constans.h"
#include "instantfade.h"
#include "scene.h"
#include "charmManager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CBall;
class CCollisionLine_Box;
class CTeamStatus;
class CGymWallManager;

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���}�l�[�W���N���X��`
class CGameManager
{
public:

	// �萔
	static constexpr float FIELD_LIMIT = 0.0f;	// �n��Y���W
	static constexpr float CENTER_LINE = 0.0f;	// �Z���^�[���C��X���W

	// �V�[���̎��
	enum ESceneType
	{
		SCENE_MAIN = 0,			// ���C��
		SCENE_START,			// �J�n���o
		SCENE_BEFOREBATTLE,		// �퓬����
		SCENE_BATTLESTART,		// �퓬�J�n
		SCENE_SPECIAL_STAG,		// �X�y�V�������o
		SCENE_DEBUG,			// �f�o�b�O
		SCENE_MAX
	};

	// �`�[���T�C�h��
	enum TeamSide
	{
		SIDE_NONE = 0,	// �R�[�g�s��
		SIDE_LEFT,		// ���R�[�g
		SIDE_RIGHT,		// �E�R�[�g
		SIDE_MAX		// ���̗񋓌^�̑���
	};

	// �`�[����ޗ�
	enum TeamType
	{
		TYPE_LEFT = 1,	// ���R�[�g
		TYPE_RIGHT,		// �E�R�[�g
		TYPE_MAX = 2	// ���̗񋓌^�̑���
	};

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetType(ESceneType type);						// �V�[���̎�ސݒ�
	ESceneType GetType() { return m_SceneType; }		// �V�[���̎�ގ擾
	ESceneType GetOldType() { return m_OldSceneType; }	// �O��̃V�[���̎�ގ擾
	void SetEnableControll(bool bControll) { m_bControll = bControll; }	// ����t���O��؂�ւ�����
	bool IsControll() { return m_bControll; }							// ����̃t���O�擾
	MyLib::Vector3 GetCourtSize() { return m_courtSize; }					// �R�[�g�T�C�Y�擾
	MyLib::Vector3 GetCourtSize(const TeamSide team, MyLib::Vector3& pos);	// �R�[�g�T�C�Y�擾(�`�[��)
	bool SetPosLimit(MyLib::Vector3& pos);									// �R�[�g�ړ�����
	CGymWallManager* GetGymWallManager() { return m_pGymWallManager; }

	void Debug();			// �f�o�b�O
	void StartSetting();	// �X�^�[�g���̐ݒ�
	CBall* GetBall();		// �{�[���擾
	CTeamStatus* GetTeamStatus(const int nTeam) { return m_pTeamStatus[nTeam]; }	// �`�[���X�e�[�^�X�擾
	void AddCharmValue(TeamSide side, CCharmManager::EType charmType);	// ���e���Z

	static CGameManager* Create(CScene::MODE mode);				// ��������
	static CGameManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:
	
	//=============================
	// �����o�֐�
	//=============================
	virtual void SceneStart();		// �J�n���o
	void UpdateAudience();			// �ϋq�X�V
	void UpdateSpecialStag();		// �X�y�V�������o�X�V
	void CreateTeamStatus();		// �`�[���X�e�[�^�X����

	//=============================
	// �����o�ϐ�
	//=============================
	ESceneType m_SceneType;		// �V�[���̎��
	ESceneType m_OldSceneType;	// �V�[���̎��
	bool m_bControll;			// ����ł��邩
	float m_fSceneTimer;		// �V�[���^�C�}�[

	CGymWallManager* m_pGymWallManager;

	MyLib::Vector3 m_courtSize;						// �R�[�g�̃T�C�Y
	CTeamStatus* m_pTeamStatus[TeamType::TYPE_MAX];	// �`�[���X�e�[�^�X
	static CGameManager* m_pThisPtr;				// ���g�̃|�C���^

#if _DEBUG
	CCollisionLine_Box* m_pCourtSizeBox = nullptr;	// �R�[�g�T�C�Y�̃{�b�N�X
#endif
};


#endif
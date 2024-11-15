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
#include "specialValueManager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CBall;
class CCollisionLine_Box;
class CTeamStatus;
class CGymWallManager;
class CTimerUI;

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
		SCENE_SPECIAL_STAG,		// �X�y�V�������o
		SCENE_END,				// �I��
		SCENE_DEBUG,			// �f�o�b�O
		SCENE_MAX
	};

	// �`�[���T�C�h��
	enum ETeamSide
	{
		SIDE_NONE = -1,	// �R�[�g�s��
		SIDE_LEFT,		// ���R�[�g
		SIDE_RIGHT,		// �E�R�[�g
		SIDE_MAX		// ���̗񋓌^�̑���
	};

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetSceneType(ESceneType type);						// �V�[���̎�ސݒ�
	ESceneType GetType() { return m_SceneType; }		// �V�[���̎�ގ擾
	ESceneType GetOldType() { return m_OldSceneType; }	// �O��̃V�[���̎�ގ擾
	void SetEnableControll(bool bControll) { m_bControll = bControll; }	// ����t���O��؂�ւ�����
	bool IsControll() { return m_bControll; }							// ����̃t���O�擾
	MyLib::Vector3 GetCourtSize() { return m_courtSize; }					// �R�[�g�T�C�Y�擾
	MyLib::Vector3 GetCourtSize(const ETeamSide team, MyLib::Vector3& pos);	// �R�[�g�T�C�Y�擾(�`�[��)
	bool SetPosLimit(MyLib::Vector3& pos);									// �R�[�g�ړ�����
	CGymWallManager* GetGymWallManager() { return m_pGymWallManager; }

	void Debug();			// �f�o�b�O
	void StartSetting();	// �X�^�[�g���̐ݒ�
	CBall* GetBall();		// �{�[���擾
	CTeamStatus* GetTeamStatus(const ETeamSide team) { return m_pTeamStatus[team]; }	// �`�[���X�e�[�^�X�擾
	void AddCharmValue(ETeamSide side, CCharmManager::ETypeAdd charmType);			// ���e���Z
	void SubCharmValue(ETeamSide side, CCharmManager::ETypeSub charmType);			// ���e���Z
	void AddSpecialValue(ETeamSide side, CSpecialValueManager::ETypeAdd charmType);	// �X�y�V�������Z

	static CGameManager* Create(CScene::MODE mode);				// ��������
	static CGameManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:
	
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CGameManager::*SCENE_FUNC)();
	static SCENE_FUNC m_SceneFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	// �V�[��
	void SceneMain();			// ���C��
	void SceneStart();			// �J�n���o
	void SceneSpecial_Stag();	// �X�y�V�������o
	void SceneEnd();			// �I�����o
	void SceneDebug();			// �f�o�b�O

	// ���̑�
	void UpdateLimitTimer();		// �������ԍX�V
	void UpdateAudience();			// �ϋq�X�V
	void UpdateSpecialStag();		// �X�y�V�������o�X�V
	void UpdateTeamStatus();		// �`�[���X�e�[�^�X�X�V
	void CreateTeamStatus();		// �`�[���X�e�[�^�X����

	//=============================
	// �����o�ϐ�
	//=============================
	ESceneType m_SceneType;		// �V�[���̎��
	ESceneType m_OldSceneType;	// �V�[���̎��
	bool m_bControll;			// ����ł��邩
	float m_fSceneTimer;		// �V�[���^�C�}�[
	MyLib::Vector3 m_courtSize;						// �R�[�g�̃T�C�Y
	CTeamStatus* m_pTeamStatus[ETeamSide::SIDE_MAX];	// �`�[���X�e�[�^�X

	//--------------------------
	// ���������I�u�W�F�N�g
	//--------------------------
	CGymWallManager* m_pGymWallManager;					// �̈�ق̕�
	CCharmManager* m_pCharmManager;						// ���e�}�l�[�W��
	CSpecialValueManager* m_pSpecialValueManager;		// �X�ؒl�}�l�[�W��
	CTimerUI* m_pTimerUI;								// �^�C�}�[UI

#if _DEBUG
	CCollisionLine_Box* m_pCourtSizeBox = nullptr;	// �R�[�g�T�C�Y�̃{�b�N�X
#endif
	static CGameManager* m_pThisPtr;				// ���g�̃|�C���^
};


#endif
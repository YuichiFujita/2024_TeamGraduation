//=============================================================================
// 
//  �Q�[���}�l�[�W���w�b�_�[ [gamemanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_	// ��d�C���N���[�h�h�~

#include "constans.h"
#include "instantfade.h"
#include "scene.h"

class CBall;

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
	enum SceneType
	{
		SCENE_MAIN = 0,			// ���C��
		SCENE_START,			// �J�n���o
		SCENE_SKIP,				// �X�L�b�v
		SCENE_COUNTDOWN,		// �J�E���g�_�E��
		SCENE_MAINRESULT,		// ���C�����U���g
		SCENE_DURING_MAINRESULT,		// ���C�����U���g��
		SCENE_MAINCLEAR,		// ���C���N���A
		SCENE_BEFOREBATTLE,		// �퓬����
		SCENE_BATTLESTART,		// �퓬�J�n
		SCENE_SKILLTREE,		// �X�L���c���[
		SCENE_BOSS,				// �{�X
		SCENE_TRANSITIONWAIT,	// �J�ڑҋ@
		SCENE_TRANSITION,		// �]��
		SCENE_RESULT,			// ���U���g
		SCENE_WAIT_AIRPUSH,		// ��C����҂�
		SCENE_GOAL,				// �S�[��
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

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetType(SceneType type);	// �V�[���̎�ސݒ�
	SceneType GetType();			// �V�[���̎�ގ擾
	SceneType GetOldType() { return m_OldSceneType; }		// �O��̃V�[���̎�ގ擾
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// ����t���O��؂�ւ�����
	bool IsControll() { return m_bControll; }				// ����̃t���O�擾
	MyLib::Vector3 GetCourtSize() { return m_courtSize; }	// �R�[�g�T�C�Y�擾
	void PosLimit(MyLib::Vector3& pos);	// �R�[�g�ړ�����

	void StartSetting();		// �X�^�[�g���̐ݒ�
	void GameClearSettings();	// �Q�[���N���A���̐ݒ�
	void GameResultSettings();	// �Q�[�����U���g�̐ݒ�

	CBall* GetBall();	// �{�[���擾

	static CGameManager *Create(CScene::MODE mode);	// ��������

protected:

	//=============================
	// �����o�֐�
	//=============================
	virtual void SceneTransition();	// �J�ڒ�
	virtual void SceneWaitAirPush();
	virtual void SceneStart();			// �J�n���o
	virtual void SceneSkip();			// �X�L�b�v
	virtual void SceneGoal();
	virtual void ContainPlayerBaggage();
	virtual void TurnAway();
	virtual void CheckJudgeZone();
	virtual void SetEnemy();
	virtual void SetBoss();

	//=============================
	// �����o�ϐ�
	//=============================
	SceneType m_SceneType;		// �V�[���̎��
	SceneType m_OldSceneType;	// �V�[���̎��
	bool m_bControll;		// ����ł��邩
	float m_fSceneTimer;	// �V�[���^�C�}�[

private:
	
	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_courtSize;		//�R�[�g�̃T�C�Y
};


#endif
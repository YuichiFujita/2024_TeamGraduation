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

class CRequestPeople;
class CReceiverPeople;
class CSkip_UI;
class CGuide;

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���}�l�[�W���N���X��`
class CGameManager
{
public:

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

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetType(SceneType type);	// �V�[���̎�ސݒ�
	SceneType GetType();		// �V�[���̎�ގ擾
	SceneType GetOldType() { return m_OldSceneType; }		// �O��̃V�[���̎�ގ擾
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// ����t���O��؂�ւ�����
	bool IsControll() { return m_bControll; }				// ����̃t���O�擾

	void StartSetting();		// �X�^�[�g���̐ݒ�
	void GameClearSettings();	// �Q�[���N���A���̐ݒ�
	void GameResultSettings();	// �Q�[�����U���g�̐ݒ�

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
};


#endif
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
class CSpawnEnvironment;

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

	enum AttackType
	{
		ATTACK_NORMAL = 0,	// �ʏ�U��
		ATTACK_STRONG,		// ���U��
		ATTACK_COUNTER,		// �����U��
		ATTACK_MAX
	};

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	void SetType(SceneType type);	// �V�[���̎�ސݒ�
	SceneType GetType();		// �V�[���̎�ގ擾
	SceneType GetOldType() { return m_OldSceneType; }		// �O��̃V�[���̎�ގ擾
	void SetEnableRush() { m_bEndRush = true; }		// ���b�V���̏I���t���O��L���ɂ���
	bool IsRushEnd() { return m_bEndRush; }			// �I���̃t���O�擾
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// ����t���O��؂�ւ�����
	bool IsControll() { return m_bControll; }				// ����̃t���O�擾
	bool IsEndNormalStage() { return m_bEndNormalStage; }	// �ʏ�X�e�[�W�̃A�I������
	void AddNowStage();	// �X�e�[�W�̉��Z
	int GetNowStage();	// ���݂̃X�e�[�W�擾
	void SetNumStage(int nStage);				// �X�e�[�W�̑����ݒ�
	int GetNumStage() { return m_nNumStage; }	// �X�e�[�W�̑����擾
	int GetJudgeRank() { return m_nJudgeRank; }

	void StartSetting();		// �X�^�[�g���̐ݒ�
	void GameClearSettings();	// �Q�[���N���A���̐ݒ�
	void GameResultSettings();	// �Q�[�����U���g�̐ݒ�

	// �]���֘A
	void AddEvaluationPoint(int point) { m_nEvaluationPoint += point; }	// �|�C���g���Z
	void SetEvaluationPoint(int point) { m_nEvaluationPoint = point; }	// �|�C���g�ݒ�
	int GetEvalutionPoint() { return m_nEvaluationPoint; }	// �|�C���g�擾
	void LoadEvaluationPoint() { m_nEvaluationPoint = m_nSaveEvaluationPoint; }	// �ۑ������|�C���g�ɕύX
	void SaveEvaluationPoint() { m_nSaveEvaluationPoint = m_nEvaluationPoint; }	// �|�C���g��ۑ�

	void SetReceiverPeople(CReceiverPeople* pPeople) { m_pReceiverPeople = pPeople; }

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
	SceneType m_SceneType;	// �V�[���̎��
	SceneType m_OldSceneType;	// �V�[���̎��
	bool m_bEndRush;		// ���b�V�����I��������
	bool m_bControll;		// ����ł��邩
	bool m_bEndNormalStage;	// �ʏ�X�e�[�W���I��������
	bool m_bGameStart;		// �Q�[���J�n���̃t���O
	int m_nNowStage;		// ���݂̃X�e�[�W
	int m_nNumStage;		// �X�e�[�W�̑���
	int m_nEvaluationPoint;	// �]���|�C���g
	int m_nSaveEvaluationPoint;	// �ۑ������]���|�C���g
	int m_nGuideTimer;		//�K�C�h�̃^�C�}�[
	float m_fCameraLengthOld;	// �O�̃J�����̋���
	float m_fPosRY;			// �v���C���[�Ɖו�����ʓ��Ɏ��߂�Ƃ��Ɏg��posR��Y���W
	float m_fSceneTimer;	// �V�[���^�C�}�[
	int m_nJudgeRank;		// �N���A�����N
	CGuide* m_pGuide;
	CRequestPeople* m_pRequestPeople;	// �˗��l�̃|�C���^
	CReceiverPeople* m_pReceiverPeople;	// �͂���̃|�C���^
	CSkip_UI* m_pSkipUI;				// �X�L�b�vUI�̃|�C���^

private:
	
	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
	CSpawnEnvironment* m_pSpawn_Air;		// ��C����
	CSpawnEnvironment* m_pSpawn_LeafFlow;	// �����t����
	CSpawnEnvironment* m_pSpawn_Leaf;		// �~��t����
};


#endif
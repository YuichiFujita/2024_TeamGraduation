//=============================================================================
// 
//  �Q�[���w�b�_�[ [game.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// ��d�C���N���[�h�h�~

#include "scene.h"
#include "gamemanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;
class CScore;
class CTimer;
class CPowerGauge;
class CStage;
class CHitScore;
class CLimitArea;
class CPlayer;
class CEdit;
class CMap_ObstacleManager;
class CBaggageManager;
class CCourse;
class CCourseManager;
class CJudgeZoneManager;
class CWaterStone_Manager;
class CPeopleManager;
class CMapUI;

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���N���X��`
class CGame : public CScene
{
public:

	// �G�f�B�b�g�^�C�v
	enum EditType
	{
		EDITTYPE_OFF = 0,		// �I�t
		EDITTYPE_MAP,			// �}�b�v
		EDITTYPE_OBSTACLE,		// ��Q��
		EDITTYPE_COURSE,		// �R�[�X
		EDITTYPE_WATERSTONE,	// ������
		EDITTYPE_JUDGEZONE,		// ����]�[��
		EDITTYPE_MAX
	};

	CGame();
	~CGame();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void ResetBeforeBoss();
	EditType GetEditType() { return m_EditType; }
	CScore *GetScore();
	CTimer *GetTimer() { return m_pTimer; }	// �^�C�}�[�擾
	CStage *GetStage();
	CGameManager *GetGameManager();
	void SetCource(CCourse* pCource) { m_pCourse = pCource; }	// �R�[�X�ݒ�
	CCourse* GetCourse() { return m_pCourse; }					// �R�[�X�̃I�u�W�F�N�g�擾
	void SetEnableClear(bool bClear) { m_clear = bClear; }		// �N���A�̃t���O
	bool IsClearFrag() { return m_clear; }						// �N���A�t���O�̎擾
	bool IsEdit() { return m_bEdit; }							// �G�f�B�b�g�̔���
	float GetRockOnDistance() { return m_fMaxRokOnDistance; }	// ���b�N�I���̍ő勗���擾

	// �ÓI�֐�
	static CGame* GetInstance();	// �C���X�^���X�擾
	static CGame* Create(CScene::MODE mode);		// ��������

protected:

	virtual void InitByMode();	// ���[�h�ʏ�����

	//=============================
	// �����o�֐�
	//=============================
	void EditReset();
	void ChangeEdit();	// �G�f�B�b�g�؂�ւ�����

	//=============================
	// �����o�ϐ�
	//=============================
	CScore *m_pScore;						// �X�R�A�̃I�u�W�F�N�g
	CTimer *m_pTimer;						// �^�C�}�[�̃I�u�W�F�N�g
	CLimitArea *m_pLimitArea;				// �G���A�����̃I�u�W�F�N�g
	CStage *m_pStage;						// �X�e�[�W�̃I�u�W�F�N�g
	CGameManager *m_pGameManager;			// �Q�[���}�l�[�W���̃I�u�W�F�N�g
	EditType m_EditType;					// �G�f�B�b�g�̎��
	CEdit* m_pEdit;							// �G�f�B�^�[
	CMap_ObstacleManager* m_pObstacleManager;	// ��Q���}�l�[�W��
	CBaggageManager* m_pBaggageManager;		// �ו��}�l�[�W��
	CCourse* m_pCourse;						// �R�[�X�̃I�u�W�F�N�g
	CCourseManager* m_pCourseManager;		// �R�[�X�}�l�[�W���̃I�u�W�F�N�g
	CJudgeZoneManager* m_pJudgeZoneManager;	// ����]�[���}�l�[�W��
	CWaterStone_Manager* m_pWaterStoneManager;	// �����΃}�l�[�W��
	CPeopleManager* m_pPeopleManager;			// �l�}�l�[�W��
	CMapUI* m_pMapUI;						// �}�b�vUI
	bool m_bEdit;				// �G�f�B�b�g�̔���
	bool m_clear; // �N���A����
	float m_fMaxRokOnDistance;			// ���b�N�I���̍ő勗��

	static CGame* m_pThisPtr;	// ���g�̃|�C���^
};



#endif
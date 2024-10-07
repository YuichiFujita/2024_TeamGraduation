//=============================================================================
// 
//  ���U���g�}�l�[�W���w�b�_�[ [resultmanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"
#include "judge.h"

// �O���錾
class CScroll;
class CTimer_Result;
class CClearRank;
class CToatalRank;

//==========================================================================
// �N���X��`
//==========================================================================
// ���U���g�}�l�[�W���N���X��`
class CResultManager
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_SCORE = 0,	// �X�R�A�\��
		STATE_PRESSENTER,	// �����҂�
		STATE_MAX
	};

	CResultManager();
	~CResultManager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Reset();	// ���Z�b�g

	void SetJudgeRank(CJudge::JUDGE rank) { m_JudgeRank = rank; }	// �ŏI�]���ݒ�
	CJudge::JUDGE GetJudgeRank() { return m_JudgeRank; }			// �ŏI�]���擾
	static void SetClearTime(float time) { m_fClearTime = time; }			// �N���A���Ԑݒ�
	static float GetClearTime() { return m_fClearTime; }					// �N���A���Ԏ擾

	void CreateResultScreen();	// ���U���g��ʐ���

	State GetState() { return m_state; }	// ���

	static CResultManager *Create();	// ��������
	static CResultManager* GetInstance() { return m_pThisPtr; }
	
private:


	//=============================
	// �����o�ϐ�
	//=============================
	State m_state;				// ���
	CJudge::JUDGE m_JudgeRank;	// �ŏI�]��
	static float m_fClearTime;			// �N���A����
	static CResultManager* m_pThisPtr;	// ���g�̃|�C���^

	CScroll* m_pScroll;			// �������̃I�u�W�F�N�g
	CTimer_Result* m_pTimer;	// �^�C�}�[�̃I�u�W�F�N�g
	CClearRank* m_pClearRank;	// �N���A���̃����N
	CToatalRank* m_pToatalRank;	// �����]��
	bool m_bScoreFinish;		// �X�R�A�̕\���I��
};



#endif
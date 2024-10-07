//=============================================================================
// 
//  �^�C�}�[�w�b�_�[ [timer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_	// ��d�C���N���[�h�h�~

#include "main.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;
class CCamera;
class CObject2D;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�}�[�N���X
class CTimer
{
public:

	enum Type
	{
		TYPE_NORMAL = 0,
		TYPE_RESULT,
		TYPE_MAX
	};

	enum eState
	{
		STATE_WAIT = 0,		// �ҋ@
		STATE_GOAL,			// �S�[��
		STATE_MAX
	};

	CTimer(int nPriority = 6);
	~CTimer();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	float GetTime() { return m_fTime; }	// ���Ԏ擾
	void SetTime(const float time);
	void SetEnableAddTime(bool frag) { m_bAddTime = frag; }	// ���ԉ��Z�t���O�ݒ�
	virtual void ApplyTimer();	// �^�C�}�[���f
	CCamera* GetCamera() { return m_pMyCamera; }

	CTimer::eState GetState() { return m_state; }
	void SetState(eState state) { m_state = state; }

	static CTimer* Create(Type type, int nPriority = 6);
	static CTimer* GetInstance() { return m_pTimer; }

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CTimer::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// ��Ԋ֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	virtual void StateWait();
	virtual void StateGoal();
	void SetCamera();

protected:

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nPriority;
	MyLib::Vector3 m_pos;			// �ʒu
	MyLib::Vector3 m_posOrigin;		// ���̈ʒu
	eState m_state;					// ���
	float m_fStateTime;				// ��Ԏ���
	float m_fTime;					// ����
	bool m_bAddTime;				// �^�C�}�[���Z�̃t���O
	CMultiNumber* m_pClearTime[3];	// ��ނ��Ƃ̐���
	static CTimer *m_pTimer;		// ���g�̃|�C���^
	CCamera* m_pMyCamera;			// �J����
	CObject2D* m_pBg;				// �w�i
};



#endif
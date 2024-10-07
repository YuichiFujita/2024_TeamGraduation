//=============================================================================
// 
//  �^�C�}�[�w�b�_�[ [timer_result.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TIMER_RESULT_H_
#define _TIMER_RESULT_H_	// ��d�C���N���[�h�h�~

#include "timer.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�}�[�N���X
class CTimer_Result : public CTimer
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_SCROLL_TEXT = 0,	// ��������
		STATE_SCROLL_VOID,		// ��ԑ���
		STATE_SCROLL_TIME,		// �^�C������
		STATE_FINISH,			// �I��
		STATE_EMPHASIZE,		// ����
		STATE_NONE,				// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CTimer_Result(int nPriority = 6);
	~CTimer_Result();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual void ApplyTimer() override;	// �^�C�}�[���f

	void SetState(State state);				// ��Ԑݒ�
	State GetState() { return m_state; }	// ��Ԏ擾


	bool IsFinish() { return m_bFinish; }	// �I��

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CTimer_Result::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();
	void StateScrollText();	// ��������
	void StateSrollVoid();	// ��ԑ���
	void StateScrollTime();	// �^�C������
	void StateFinish();		// �I��
	void StateEmphasize();	// ����
	void StateNone() {}		// �Ȃɂ��Ȃ�

	// ���̑��֐�
	void CreateText();	// ��������
	void Skip();		// �X�L�b�v

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑�
	CObject2D* m_pText;		// ����
	float m_fMoveTextLen;	// �e�L�X�g�̈ړ�����
	float m_fMoveTimeLen;	// �^�C���̈ړ�����
	bool m_bFinish;			// �I��

};



#endif
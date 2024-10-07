//=============================================================================
// 
//  �J�E���g�_�E���w�b�_�[ [countdown_start.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _COUNTDOWN_START_H_
#define _COUNTDOWN_START_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�E���g�_�E���N���X
class CCountdown_Start : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_DROP_UP,		// ����(��)
		STATE_DROP_WAIT,	// ����(�ҋ@)
		STATE_DROP_DOWN,	// ����(��)
		STATE_COMPLETE,		// �����I
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CCountdown_Start(int nPriority = 7);
	~CCountdown_Start();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// �ÓI�֐�
	//=============================
	static CCountdown_Start* Create();	// ��������

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CCountdown_Start::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateDrop_UP();	// ����(��)
	void StateDrop_Wait();	// ����(�ҋ@)
	void StateDrop_DOWN();	// ����(��)
	void StateComplete();	// ����
	void StateFadeout();	// �t�F�[�h�A�E�g

	// ���̑��֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑��ϐ�
	int m_nCount;	// �J�E���g

};


#endif
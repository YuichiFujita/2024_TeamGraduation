//=============================================================================
// 
//  �_�b�V���w�b�_�[ [dash.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DASH_H_
#define _DASH_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �_�b�V���N���X
class CDash : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_DASH,			// �_�b�V��
		STATE_MAX
	};

	enum Angle
	{
		ANGLE_UP = 0,
		ANGLE_RIGHTUP,
		ANGLE_RIGHT,
		ANGLE_RIGHTDW,
		ANGLE_DOWN,
		ANGLE_LEFTDW,
		ANGLE_LEFT,
		ANGLE_LEFTUP,
		ANGLE_MAX
	};

	struct MyStruct
	{
		bool bDash;
		Angle angle;

		MyStruct() : bDash(false), angle(ANGLE_LEFT) {}
	};

	CDash(int nPriority = 7);
	~CDash();

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
	static CDash* Create();

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CDash::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateDash();		// �_�b�V��

	// ���̑��֐�
	MyStruct Trigger(Angle angle);

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑��ϐ�
	int m_nCntTrigger[Angle::ANGLE_MAX];		// �g���K�[�̃J�E���g
	float m_fDashInterval;		// �_�b�V���̃C���^�[�o��

};


#endif
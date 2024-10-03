//=============================================================================
// 
//  �K�C�h�w�b�_�[ [guide.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GUIDE_H_
#define _GUIDE_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

// �O���錾
class CControlKeyDisp;

//==========================================================================
// �N���X��`
//==========================================================================
// �����N���X
class CGuide : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_FADEIN,		// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	enum Type
	{
		START = 0,	// �J�n��
		GOAL,		// �S�[����
		MAX
	};

	CGuide(int nPriority = 7);
	~CGuide();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	void SetState(State state);	// ��Ԑݒ�

	//=============================
	// �ÓI�֐�
	//=============================
	static CGuide* Create(Type type);	// ��������

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CGuide::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeout();	// �t�F�[�h�A�E�g

	// ���̑��֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fLifeTimer;		// ��������
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���
	CControlKeyDisp* m_pKeyDisp;	// �J�n�{�^��

};


#endif
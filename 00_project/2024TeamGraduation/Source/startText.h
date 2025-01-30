//==========================================================================
// 
//  �Q�[���X�^�[�g�̕����w�b�_�[ [startText.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _STARTTEXT_H_
#define _STARTTEXT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���X�^�[�g�̕����N���X
class CStartText : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_WAIT,		// �ҋ@
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CStartText(int nPriority = 6);
	~CStartText();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Kill() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CStartText* Create();

	//=============================
	// �����o�֐�
	//=============================
	void SetState(EState state);	// ��Ԑݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CStartText::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone() {}		// �Ȃɂ��Ȃ�
	void StateFadeIn();		// �t�F�[�h�C��
	void StateWait();		// �ҋ@
	void StateFadeOut();	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;	// ��ԃJ�E���^�[
	EState m_state;		// ���
};


#endif
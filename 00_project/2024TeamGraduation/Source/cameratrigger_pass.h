//=============================================================================
//
// �J�����g���K�[_�p�X���� [cameratrigger_pass.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERATRIGGER_PASS_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERATRIGGER_PASS_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "camera_motion_trigger.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����g���K�[_�p�X�N���X
class CCameraTrigger_Pass : public CCameraMotion_Trigger
{
public:

	CCameraTrigger_Pass();
	~CCameraTrigger_Pass();
	
	// �g���K�[�̏u��
	virtual void TriggerMoment(int idx) override
	{
		(this->*(m_TriggerFunc[idx]))();
	}

private:


	//=============================
	// �֐����X�g
	//=============================
	// ���[�V�������X�g
	typedef void(CCameraTrigger_Pass::*TRIGGER_FUNC)();
	static TRIGGER_FUNC m_TriggerFunc[];

	void waiting();	// ����
	void newMission();
	void carry();
	void carefull();
	void goodluck();

};

#endif
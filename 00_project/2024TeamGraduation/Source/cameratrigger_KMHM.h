//=============================================================================
//
// �J�����g���K�[_���߂͂ߔg���� [cameratrigger_KMHM.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERATRIGGER_KMHM_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERATRIGGER_KMHM_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "camera_motion_trigger.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����g���K�[_���߂͂ߔg�N���X
class CCameraTrigger_KMHM : public CCameraMotion_Trigger
{
public:

	CCameraTrigger_KMHM();
	~CCameraTrigger_KMHM();
	
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
	typedef void(CCameraTrigger_KMHM::*TRIGGER_FUNC)();
	static TRIGGER_FUNC m_TriggerFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void Stance();	// �\��

};

#endif
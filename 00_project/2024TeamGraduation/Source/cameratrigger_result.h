//=============================================================================
//
// �J�����g���K�[_�p�X���� [cameratrigger_result.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERATRIGGER_RESULT_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERATRIGGER_RESULT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "camera_motion_trigger.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����g���K�[_�p�X�N���X
class CCameraTrigger_Result : public CCameraMotion_Trigger
{
public:

	CCameraTrigger_Result();
	~CCameraTrigger_Result();
	
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
	typedef void(CCameraTrigger_Result::*TRIGGER_FUNC)();
	static TRIGGER_FUNC m_TriggerFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void Subtitle01();	// ����01
	void Subtitle02();	// ����02
	void CameraShake();	// �J�����h��
	void OpenScroll();	// �������J��

};

#endif
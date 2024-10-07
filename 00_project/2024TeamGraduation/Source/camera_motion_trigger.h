//=============================================================================
//
// �J�������[�V�����g���K�[���� [camera_motion_trigger.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERA_MOTION_TRIGGER_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_MOTION_TRIGGER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����


//==========================================================================
// �N���X��`
//==========================================================================
// �J�������[�V�����g���K�[�N���X
class CCameraMotion_Trigger
{
public:

	CCameraMotion_Trigger();
	~CCameraMotion_Trigger();
	
	// �g���K�[�̏u��
	virtual void TriggerMoment(int idx) = 0;

	static CCameraMotion_Trigger* Create(int motion)
	{
		return m_CreateFunc[motion]();
	}

private:

	//=============================
	// �֐����X�g
	//=============================
	// �������X�g
	using CREATE_FUNC = std::function<CCameraMotion_Trigger* ()>;
	static std::vector < CREATE_FUNC > m_CreateFunc;
};

#endif
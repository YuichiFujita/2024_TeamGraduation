//=============================================================================
// 
// �J�������[�V�������� [cameratrigger_pass.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "cameratrigger_pass.h"
#include "sound.h"

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCameraTrigger_Pass::TRIGGER_FUNC CCameraTrigger_Pass::m_TriggerFunc[] =
{
	&CCameraTrigger_Pass::waiting,		// ����1
	& CCameraTrigger_Pass::newMission,		// ����1
	& CCameraTrigger_Pass::carry,		// ����1
	& CCameraTrigger_Pass::carefull,		// ����1
	& CCameraTrigger_Pass::goodluck,		// ����1
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCameraTrigger_Pass::CCameraTrigger_Pass() : CCameraMotion_Trigger()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraTrigger_Pass::~CCameraTrigger_Pass()
{

}

//==========================================================================
// ����
//==========================================================================
void CCameraTrigger_Pass::waiting()
{
	

}
void CCameraTrigger_Pass::newMission()
{
	

}
void CCameraTrigger_Pass::carry()
{
	

}
void CCameraTrigger_Pass::carefull()
{
	

}
void CCameraTrigger_Pass::goodluck()
{
	

}



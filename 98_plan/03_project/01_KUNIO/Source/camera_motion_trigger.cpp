//=============================================================================
// 
// �J�������[�V�������� [camera_motion_trigger.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "camera_motion_trigger.h"

// �h����
#include "cameratrigger_pass.h"
#include "cameratrigger_result.h"

//==========================================================================
// �֐����X�g
//==========================================================================
std::vector < CCameraMotion_Trigger::CREATE_FUNC > CCameraMotion_Trigger::m_CreateFunc =
{
	[]() {return DEBUG_NEW CCameraTrigger_Pass(); },	// �p�X
	[]() {return nullptr; },	// �p�X
	[]() {return nullptr; },	// �p�X
	[]() {return DEBUG_NEW CCameraTrigger_Result(); },	// ���U���g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCameraMotion_Trigger::CCameraMotion_Trigger()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraMotion_Trigger::~CCameraMotion_Trigger()
{

}

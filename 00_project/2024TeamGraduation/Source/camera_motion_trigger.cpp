//=============================================================================
// 
// �J�������[�V�������� [camera_motion_trigger.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "camera_motion_trigger.h"
#include "camera_motion.h"

// �h����
#include "cameratrigger_pass.h"

//==========================================================================
// �֐����X�g
//==========================================================================
std::vector<CCameraMotion_Trigger::CREATE_FUNC>CCameraMotion_Trigger::m_CreateFunc =
{
	[]() { return DEBUG_NEW CCameraTrigger_Pass(); },	// �p�X
	[]() { return nullptr; },	// �o�ꉉ�o
	[]() { return nullptr; },	// �X�y�V��������グ
	[]() { return nullptr; },	// ���߂͂ߔg
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCameraMotion_Trigger::CCameraMotion_Trigger()
{
	MyAssert::CustomAssert((int)m_CreateFunc.size() == CCameraMotion::MOTION::MOTION_MAX, "ERROR : Motion Count Mismatch");
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraMotion_Trigger::~CCameraMotion_Trigger()
{

}

//=============================================================================
// 
// �J�������[�V�������� [cameratrigger_result.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "cameratrigger_result.h"
#include "manager.h"
#include "camera.h"
#include "subtitle.h"
#include "serifeffect.h"
#include "sound.h"
#include "resultmanager.h"

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCameraTrigger_Result::TRIGGER_FUNC CCameraTrigger_Result::m_TriggerFunc[] =
{
	&CCameraTrigger_Result::Subtitle01,		// ����01
	&CCameraTrigger_Result::Subtitle02,		// ����01
	&CCameraTrigger_Result::CameraShake,	// �J�����h��
	&CCameraTrigger_Result::OpenScroll,		// �������J��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCameraTrigger_Result::CCameraTrigger_Result() : CCameraMotion_Trigger()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraTrigger_Result::~CCameraTrigger_Result()
{

}

//==========================================================================
// ����01
//==========================================================================
void CCameraTrigger_Result::Subtitle01()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\result_01.png");
	pSubTitle->SetSizeByHeight(40.0f);

	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_HNSW_000);
}

//==========================================================================
// ����02
//==========================================================================
void CCameraTrigger_Result::Subtitle02()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\result_02.png");
	pSubTitle->SetSizeByHeight(40.0f);

	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_OKSD_001);
}

//==========================================================================
// �J�����h��
//==========================================================================
void CCameraTrigger_Result::CameraShake()
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	pCamera->SetShake(18, 25.0f, 25.0f);
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_SMASHATTACK);
}

//==========================================================================
// �������J��
//==========================================================================
void CCameraTrigger_Result::OpenScroll()
{
	// ���U���g���
	CResultManager* pResultManager = CResultManager::GetInstance();
	pResultManager->CreateResultScreen();
}
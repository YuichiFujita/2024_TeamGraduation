//=============================================================================
// 
// カメラモーション処理 [cameratrigger_result.cpp]
// Author : 相馬靜雅
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
// 関数ポインタ
//==========================================================================
CCameraTrigger_Result::TRIGGER_FUNC CCameraTrigger_Result::m_TriggerFunc[] =
{
	&CCameraTrigger_Result::Subtitle01,		// 字幕01
	&CCameraTrigger_Result::Subtitle02,		// 字幕01
	&CCameraTrigger_Result::CameraShake,	// カメラ揺れ
	&CCameraTrigger_Result::OpenScroll,		// 巻き物開封
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraTrigger_Result::CCameraTrigger_Result() : CCameraMotion_Trigger()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraTrigger_Result::~CCameraTrigger_Result()
{

}

//==========================================================================
// 字幕01
//==========================================================================
void CCameraTrigger_Result::Subtitle01()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\result_01.png");
	pSubTitle->SetSizeByHeight(40.0f);

	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_HNSW_000);
}

//==========================================================================
// 字幕02
//==========================================================================
void CCameraTrigger_Result::Subtitle02()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\result_02.png");
	pSubTitle->SetSizeByHeight(40.0f);

	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_OKSD_001);
}

//==========================================================================
// カメラ揺れ
//==========================================================================
void CCameraTrigger_Result::CameraShake()
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	pCamera->SetShake(18, 25.0f, 25.0f);
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_SMASHATTACK);
}

//==========================================================================
// 巻き物開封
//==========================================================================
void CCameraTrigger_Result::OpenScroll()
{
	// リザルト画面
	CResultManager* pResultManager = CResultManager::GetInstance();
	pResultManager->CreateResultScreen();
}
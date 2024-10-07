//=============================================================================
// 
// カメラモーション処理 [cameratrigger_pass.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "cameratrigger_pass.h"
#include "sound.h"

//==========================================================================
// 関数ポインタ
//==========================================================================
CCameraTrigger_Pass::TRIGGER_FUNC CCameraTrigger_Pass::m_TriggerFunc[] =
{
	&CCameraTrigger_Pass::waiting,		// 字幕1
	& CCameraTrigger_Pass::newMission,		// 字幕1
	& CCameraTrigger_Pass::carry,		// 字幕1
	& CCameraTrigger_Pass::carefull,		// 字幕1
	& CCameraTrigger_Pass::goodluck,		// 字幕1
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraTrigger_Pass::CCameraTrigger_Pass() : CCameraMotion_Trigger()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraTrigger_Pass::~CCameraTrigger_Pass()
{

}

//==========================================================================
// 字幕
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



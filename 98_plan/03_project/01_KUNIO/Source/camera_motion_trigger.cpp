//=============================================================================
// 
// カメラモーション処理 [camera_motion_trigger.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "camera_motion_trigger.h"

// 派生先
#include "cameratrigger_pass.h"
#include "cameratrigger_result.h"

//==========================================================================
// 関数リスト
//==========================================================================
std::vector < CCameraMotion_Trigger::CREATE_FUNC > CCameraMotion_Trigger::m_CreateFunc =
{
	[]() {return DEBUG_NEW CCameraTrigger_Pass(); },	// パス
	[]() {return nullptr; },	// パス
	[]() {return nullptr; },	// パス
	[]() {return DEBUG_NEW CCameraTrigger_Result(); },	// リザルト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraMotion_Trigger::CCameraMotion_Trigger()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraMotion_Trigger::~CCameraMotion_Trigger()
{

}

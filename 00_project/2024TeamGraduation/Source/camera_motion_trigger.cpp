//=============================================================================
// 
// カメラモーション処理 [camera_motion_trigger.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "camera_motion_trigger.h"
#include "camera_motion.h"

// 派生先
#include "cameratrigger_pass.h"

//==========================================================================
// 関数リスト
//==========================================================================
std::vector<CCameraMotion_Trigger::CREATE_FUNC>CCameraMotion_Trigger::m_CreateFunc =
{
	[]() { return DEBUG_NEW CCameraTrigger_Pass(); },	// パス
	[]() { return nullptr; },	// 登場演出
	[]() { return nullptr; },	// スペシャル盛り上げ
	[]() { return nullptr; },	// かめはめ波
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraMotion_Trigger::CCameraMotion_Trigger()
{
	MyAssert::CustomAssert((int)m_CreateFunc.size() == CCameraMotion::MOTION::MOTION_MAX, "ERROR : Motion Count Mismatch");
}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraMotion_Trigger::~CCameraMotion_Trigger()
{

}

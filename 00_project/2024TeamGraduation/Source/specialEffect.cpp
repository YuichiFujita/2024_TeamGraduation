//=============================================================================
// 
// スペシャル演出処理 [specialEffect.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "specialEffect.h"

// 派生先
#include "specialEffect_kamehameha.h"

//==========================================================================
// 関数リスト
//==========================================================================
std::vector<CSpecialEffect::CREATE_FUNC>CSpecialEffect::m_CreateFunc =
{
	[]() { return DEBUG_NEW CSpecialEffect_Kamehame(); },	// かめはめ波
};

//==========================================================================
// コンストラクタ
//==========================================================================
CSpecialEffect::CSpecialEffect()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CSpecialEffect::~CSpecialEffect()
{

}

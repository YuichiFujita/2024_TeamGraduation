//==========================================================================
// 
//  ユーザープレイヤー処理 [playerUserOut.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerUserOut.h"

// 使用クラス
#include "playerUserOut_controlMove.h"
#include "playerUsercontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserOut::CPlayerUserOut(CPlayer* pPlayer, const CGameManager::TeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerUser(pPlayer, typeTeam, typeArea),
#if 0	// TODO
	m_posLeft	(VEC3_ZERO),	// 移動可能左位置
	m_posRight	(VEC3_ZERO)		// 移動可能右位置
#else
	m_posLeft	(MyLib::Vector3(900.0f, 0.0f, 650.0f)),	// 移動可能左位置
	m_posRight	(MyLib::Vector3(50.0f, 0.0f, 650.0f))	// 移動可能右位置
#endif
{
	// 外野操作の割当	// TODO：外野操作に変更
	ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUserOut::~CPlayerUserOut()
{

}

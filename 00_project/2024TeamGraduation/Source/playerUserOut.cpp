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
CPlayerUserOut::CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerUser(pPlayer, typeTeam, typeArea),
	m_posLeft	(VEC3_ZERO),	// 移動可能左位置
	m_posRight	(VEC3_ZERO)		// 移動可能右位置
{
	// 外野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUserOut::~CPlayerUserOut()
{

}

//==========================================================================
// 位置の初期化
//==========================================================================
void CPlayerUserOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// 左右位置の中央座標を計算
	MyLib::Vector3 posCenter;	// 中央座標
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// プレイヤー位置の設定
	GetPlayer()->SetPosition(posCenter);
}

//==========================================================================
// 
//  AIプレイヤー処理 [playerAIOut.cpp]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================
#include "playerAIOut.h"

// 使用クラス
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerAI(pPlayer, typeTeam, typeArea),
	m_posLeft	(VEC3_ZERO),	// 移動可能左位置
	m_posRight	(VEC3_ZERO)		// 移動可能右位置
{
	// 外野操作の割当	// TODO：外野操作に変更
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{

}

//==========================================================================
// 位置の初期化
//==========================================================================
void CPlayerAIOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// 左右位置の中央座標を計算
	MyLib::Vector3 posCenter;	// 中央座標
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// プレイヤー位置の設定
	GetPlayer()->SetPosition(posCenter);
}

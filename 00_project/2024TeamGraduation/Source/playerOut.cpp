//==========================================================================
// 
//  外野プレイヤー処理 [playerOut.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerOut.h"

// 使用クラス
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerOut::CPlayerOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase),
	m_posLeft	(VEC3_ZERO),	// 移動可能左位置
	m_posRight	(VEC3_ZERO)		// 移動可能右位置
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerOut::~CPlayerOut()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerOut::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();

	// 操作クラスの取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 操作の更新
	pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// 操作クラスの反映
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// 位置の初期化
//==========================================================================
void CPlayerOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// 左右位置の中央座標を計算
	MyLib::Vector3 posCenter;	// 中央座標
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// プレイヤー位置の設定
	GetPlayer()->SetPosition(posCenter);
}

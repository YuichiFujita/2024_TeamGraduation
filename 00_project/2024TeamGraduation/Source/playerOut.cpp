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
#include "playerAction.h"


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerOut::CPlayerOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase),
	m_posLeft	(VEC3_ZERO),	// 移動可能左位置
	m_posRight	(VEC3_ZERO),	// 移動可能右位置
	m_bClab(false)
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

//==========================================================================
// カニ歩き判定
//==========================================================================
bool CPlayerOut::IsCrab()
{
	// プレイヤー情報の取得
	CPlayer* pPlayer = GetPlayer();

	if (!pPlayer || !m_bClab) return false;

	// ボールを持っていないか
	if (pPlayer->GetBall() != nullptr) { return false; }

	// 世界にボールがあるか
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// ボール情報
	if (pBall == nullptr) { return false; }

	// 自身の状態：ブリンク＆走りでない
	CPlayer::EAction action = pPlayer->GetActionPattern()->GetAction();	// アクションパターン
	if (action == CPlayer::EAction::ACTION_BLINK) { return false; }
	if (pPlayer->IsDash()) { return false; }

	return true;

}

//=============================================================================
// 
//  プレイヤーAIコントロール_初め処理 [playerAIcontrol_start.cpp]
//  Author : TakeruOgasawara
// 
//=============================================================================
#include "playerAIcontrol_start.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"

// player
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_idle.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlStart::CPlayerAIControlStart()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIControlStart::~CPlayerAIControlStart()
{

}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlStart* CPlayerAIControlStart::Create(CPlayer* player)
{
	if (!player) assert(("プレイヤー情報が無いよ", false));

	// メモリの確保
	CPlayerAIControlStart* pObj = nullptr;

	if (pObj == nullptr)
	{
		pObj = DEBUG_NEW CPlayerAIControlStart;

		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//================================================================================
// 初期化処理
//================================================================================
HRESULT CPlayerAIControlStart::Init()
{
	// 親クラスの初期化
	CPlayerAIControlMode::Init();

	return S_OK;
}

//================================================================================
// 終了処理
//================================================================================
void CPlayerAIControlStart::Uninit()
{
	// 親クラスの終了処理
	CPlayerAIControlMode::Uninit();
}

//================================================================================
// 更新処理
//================================================================================
void CPlayerAIControlStart::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 行けええええええ
	Gooooo();

	// 親クラスの更新（最後尾に設置）
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 行けええええええ
//==========================================================================
void CPlayerAIControlStart::Gooooo()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (!pBall)
	{// ボールがない場合
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionFlag(EActionFlag::ACTION_NONE);
		SetMode(EMode::MODE_IDLE);
		return;
	}

	if (IsPicksUpBall() ||									// 自分より近いプレイヤーがいた
		pBall->GetState() != CBall::EState::STATE_SPAWN)	// ボールが生成状態ではない場合
	{
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionFlag(EActionFlag::ACTION_NONE);
		SetMode(EMode::MODE_IDLE);
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// 誰かがボールを持った場合
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionFlag(EActionFlag::ACTION_NONE);
		SetMode(EMode::MODE_IDLE);
		return;
	}

	// ボールを見る
	SeeBall();

	SetActionFlag(EActionFlag::ACTION_JUMP);
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
}

//==========================================================================
// ボールを見る
//==========================================================================
void CPlayerAIControlStart::SeeBall()
{
	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// ボールとの角度
	float angle = pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 角度の設定
	pAI->SetRotDest(angle);
}


//==========================================================================
// 誰がボールを取りに行きますか？
//==========================================================================
bool CPlayerAIControlStart::IsPicksUpBall()
{
	float fMyDis = 1000000.0f;	// 自分のボールとの距離

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// 自分の情報取得
	CGameManager::ETeamSide typeTeam = pAI->GetTeam();	// チームタイプ
	MyLib::Vector3 Mypos = pAI->GetPosition();	// 位置情報

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return true;
	if (pBall->GetPlayer()) return true;

	// ボール位置取得
	MyLib::Vector3 posBall = pBall->GetPosition();
	fMyDis = Mypos.DistanceXZ(posBall);	// 自分からボールとの距離

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		if (pPlayer == pAI) continue;		// 自分の場合
		if (pPlayer->GetTeam() != typeTeam) continue;		// チームが違う場合
		if ((pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{// 外野&&ユーザー&&自分の場合
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		// チームからボールとの距離を求める
		float fLength = posPlayer.DistanceXZ(posBall);

		if (fLength < fMyDis)
		{ // より近い味方プレイヤーがいた場合
			return true;
		}
	}

	return false;
}

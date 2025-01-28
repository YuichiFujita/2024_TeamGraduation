//=============================================================================
// 
//  プレイヤーコントロール処理 [playerAction.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "playerAction.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "ball.h"
#include "playerStatus.h"

namespace
{
	const float JUMPTHROW_DOWN_COR = 0.5f;	//下降時ジャンプ投げ移動量補正値
}

namespace DodgeSlow
{
	const float END = 0.1f;			// 最大値
	const float START = 1.0f;		// 最小値
}

namespace ActionTime
{
	const float BLINK = 0.2f;		// ブリンク時間
	const float DODGE = 0.5f;		// 回避時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAction::ACTION_FUNC CPlayerAction::m_ActionFunc[] =	// 行動関数
{
	&CPlayerAction::ActionNone,			// なし
	&CPlayerAction::ActionUnstable,		// おっとっと
	&CPlayerAction::ActionBlink,		// ブリンク
	&CPlayerAction::ActionDodge,		// 回避
	&CPlayerAction::ActionRun,			// 走り
	&CPlayerAction::ActionJump,			// ジャンプ
	&CPlayerAction::ActionCatch,		// キャッチ
	&CPlayerAction::ActionCatchJump,	// キャッチ(ジャンプ)
	&CPlayerAction::ActionThrow,		// 投げ
	&CPlayerAction::ActionThrowJump,	// 投げ(ジャンプ)
	&CPlayerAction::ActionSpecial,		// スペシャル
};

CPlayerAction::START_FUNC CPlayerAction::m_StartFunc[] =	// 行動関数
{
	nullptr,								// なし
	nullptr,								// おっとっと
	nullptr,								// ブリンク
	&CPlayerAction::StartDodge,				// 回避
	nullptr,								// 走り
	nullptr,								// ジャンプ
	nullptr,								// キャッチ
	&CPlayerAction::StartCatchJump,			// キャッチ(ジャンプ)
	nullptr,								// 投げ
	&CPlayerAction::StartThrowJump,			// 投げ(ジャンプ)
	nullptr,								// スペシャル
};

CPlayerAction::END_FUNC CPlayerAction::m_EndFunc[] =	// 行動関数
{
	nullptr,								// なし
	&CPlayerAction::EndUnstable,			// おっとっと
	nullptr,								// ブリンク
	&CPlayerAction::EndDodge,				// 回避
	nullptr,								// 走り
	nullptr,								// ジャンプ
	nullptr,								// キャッチ
	nullptr,								// キャッチ(ジャンプ)
	nullptr,								// 投げ
	nullptr,								// 投げ(ジャンプ)
	nullptr,								// スペシャル
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAction::CPlayerAction(CPlayer* player)
{
	m_bCharm = false;
	m_Action = CPlayer::EAction::ACTION_NONE;	// アクション
	m_fActionTime = 0.0f;						// アクション時間
	m_pPlayer = player;							// プレイヤーのポインタ

	m_fSlowStart = DodgeSlow::START;		// スロー倍率(最小)
	m_fSlowEnd = DodgeSlow::END;			// スロー倍率(最大)

	// スタティックアサート
	static_assert(NUM_ARRAY(m_ActionFunc) == CPlayer::EAction::ACTION_MAX, "ERROR : Action Count Mismatch");
	static_assert(NUM_ARRAY(m_StartFunc) == CPlayer::EAction::ACTION_MAX, "ERROR : Action Count Mismatch");
	static_assert(NUM_ARRAY(m_EndFunc)  == CPlayer::EAction::ACTION_MAX, "ERROR : Action Count Mismatch");
}

//==========================================================================
// 更新
//==========================================================================
void CPlayerAction::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// アクション時間
	m_fActionTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 行動更新
	(this->*(m_ActionFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG //デバッグ

	Debug();

#endif //デバッグ
}

//==========================================================================
// なし
//==========================================================================
void CPlayerAction::ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// おっとっと
//==========================================================================
void CPlayerAction::ActionUnstable(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ブリンク
//==========================================================================
void CPlayerAction::ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fActionTime >= ActionTime::BLINK)
	{// ブリンク経過
		SetAction(CPlayer::EAction::ACTION_NONE);
	}

	CMotion* pMotion = m_pPlayer->GetMotion();

	// 自分ボール持ってる
	if (m_pPlayer->GetBall() != nullptr) return;

	//回避判定
	CBall* pObj = CGameManager::GetInstance()->GetBall();
	CGameManager::ETeamSide teamPlayer = m_pPlayer->GetTeam();

	// 攻撃状態でないor自陣
	if (!pObj->IsAttack() || pObj->GetTypeTeam() == teamPlayer) return;

	// 攻撃フレーム(回避猶予)じゃないならはじく
	if (!pMotion->IsAttacking()) return;

	// 判定
	if (UtilFunc::Collision::CollisionCircleCylinder(
		pObj->GetPosition(), m_pPlayer->GetPosition(),		// 位置
		pObj->GetRadius(), m_pPlayer->GetRadius() * 2.0f,	// 半径
		m_pPlayer->GetParameter().fHeight * 1.3f))			// 高さ
	{
		// アクション設定
		SetAction(CPlayer::EAction::ACTION_DODGE);

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_DODGE);
	}
}

//==========================================================================
// 回避
//==========================================================================
void CPlayerAction::ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 無敵時間付与
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.1f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	// スローかける
	float fRate = 1.0f;	// 割合
	float fFrame = m_pPlayer->GetMotion()->GetAllCount();
	float fFrameMax = m_pPlayer->GetMotion()->GetMaxAllCount();

	if (fFrame <= fFrameMax * 0.5f)
	{
		fRate = UtilFunc::Correction::EasingEaseIn(m_fSlowStart, m_fSlowEnd, 0.0f, fFrameMax * 0.5f, fFrame);
	}
	else
	{// 半分を超えたら
		fRate = UtilFunc::Correction::EasingEaseOut(m_fSlowEnd, m_fSlowStart, fFrameMax * 0.5f, fFrameMax, fFrame);
	}

	UtilFunc::Transformation::ValueNormalize(fRate, 1.0f, 0.0f);
	GET_MANAGER->SetSlowRate(fRate);

	if (m_pPlayer->GetMotion()->IsFinish())
	{// 終了

		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// 走り
//==========================================================================
void CPlayerAction::ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerAction::ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_pPlayer->IsJump())
	{// 着地で終了
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerAction::ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// モーションと同時に終了
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// ジャンプキャッチ
//==========================================================================
void CPlayerAction::ActionCatchJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//フワッと
	UniqueJumpUpdate(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pPlayer->GetMotion()->IsFinish())
	{// モーションと同時に終了
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerAction::ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMotion* pMotion = m_pPlayer->GetMotion();

	if (pMotion->IsFinish())
	{// 終了
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// ジャンプ投げ
//==========================================================================
void CPlayerAction::ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//フワッと
	UniqueJumpUpdate(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pPlayer->GetMotion()->IsFinish())
	{// ジャンプ移行
		SetAction(CPlayer::EAction::ACTION_JUMP);
	}
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerAction::ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// [開始] ジャンプキャッチ
//==========================================================================
void CPlayerAction::StartCatchJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ふわっと
	UniqueJumpStart();
}

//==========================================================================
// [開始] ジャンプ投げ
//==========================================================================
void CPlayerAction::StartThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ふわっと
	UniqueJumpStart();
}

//==========================================================================
// [開始] 回避
//==========================================================================
void CPlayerAction::StartDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager::ETeamSide teamPlayer = m_pPlayer->GetTeam();

	//ダメージ受付しない時間設定
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.5f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	// 回避状態に移行
	// 操作不能状態
	m_pPlayer->SetEnableMove(false);
	m_pPlayer->SetState(CPlayer::STATE_DODGE);
	m_pPlayer->SetMotion(CPlayer::MOTION_DODGE);

	// ゲームマネージャ取得
	CGameManager* pGameMgr = CGameManager::GetInstance();
	if (pGameMgr == nullptr) return;

	// モテ加算
	pGameMgr->AddCharmValue(teamPlayer, CCharmValueManager::ETypeAdd::ADD_DODGE);

	// スペシャル加算
	pGameMgr->AddSpecialValue(teamPlayer, CSpecialValueManager::ETypeAdd::ADD_DODGE);
}

//==========================================================================
// [終了] 回避
//==========================================================================
void CPlayerAction::EndDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 無敵時間解除
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.0f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	// スロー解除
	float fRate = 1.0f;
	m_pPlayer->SetEnableMove(true);
	UtilFunc::Transformation::ValueNormalize(fRate, 1.0f, 0.0f);
	GET_MANAGER->SetSlowRate(fRate);

	// 状態解除
	m_pPlayer->SetState(CPlayer::EState::STATE_NONE);
}

//==========================================================================
// [終了] おっとっと
//==========================================================================
void CPlayerAction::EndUnstable(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_pPlayer->SetEnableBrake(false);
	m_pPlayer->SetEnableMove(true);
}

//==========================================================================
// アクション設定
//==========================================================================
void CPlayerAction::SetAction(CPlayer::EAction action)
{
	float fDeltaTime = CManager::GetInstance()->GetDeltaTime();		// 1フレーム間の経過時間
	float fDeltaRate = CManager::GetInstance()->GetDeltaRate();		// 経過時間の割合
	float fSlowRate = CManager::GetInstance()->GetSlowRate();		// 速度低下の割合

	// 行動終了
	if (m_Action != action &&
		m_EndFunc[m_Action] != nullptr)
	{
		(this->*(m_EndFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	m_Action = action;
	m_fActionTime = 0.0f;		// アクション時間

	// 行動開始
	if (m_StartFunc[m_Action] != nullptr)
	{
		(this->*(m_StartFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// ジャンプ系アクションスタート
//==========================================================================
void CPlayerAction::UniqueJumpStart()
{
	//フワッと
	CCharacterStatus::CharParameter param = m_pPlayer->GetParameter();
	MyLib::Vector3 move = m_pPlayer->GetMove();
	float fMoveY = param.fJumpStartMove;

	//下降中
	if (move.y < 0.0f)
	{
		fMoveY *= JUMPTHROW_DOWN_COR;
	}

	move.y = fMoveY;
	m_pPlayer->SetMove(move);
}

//==========================================================================
// ジャンプ系更新
//==========================================================================
void CPlayerAction::UniqueJumpUpdate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//フワッと
	CCharacterStatus::CharParameter param = m_pPlayer->GetParameter();
	MyLib::Vector3 move = m_pPlayer->GetMove();
	float fMoveY = param.fJumpUpdateMove;

	//下降中
	if (move.y < 0.0f)
	{
		fMoveY *= JUMPTHROW_DOWN_COR;
	}

	fMoveY *= fDeltaRate * fSlowRate;

	move.y += fMoveY;
	m_pPlayer->SetMove(move);
}

//==========================================================================
// デバッグ
//==========================================================================
void CPlayerAction::Debug()
{
	if (ImGui::Button("Dodge"))
	{
		// アクション設定
		SetAction(CPlayer::EAction::ACTION_DODGE);

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_DODGE);
	}
}

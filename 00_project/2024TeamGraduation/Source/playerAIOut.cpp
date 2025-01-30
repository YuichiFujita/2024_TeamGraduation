//==========================================================================
// 
//  AIプレイヤー処理 [playerAIOut.cpp]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================
#include "playerAIOut.h"
#include "manager.h"
#include "camera.h"

// 使用クラス
#include "playerAIOut_controlMove.h"
#include "playerAIOut_controlAction.h"
#include "playerAction.h"
#include "playerAIOut_control.h"
#include "playerManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_AI)
{
	// 外野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerAIOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIOutControlAction());

	// AIコントロール(外野)の生成
	m_pAIOutControl = CPlayerAIOutControl::Create(pPlayer);
	m_eAIRange = RANGE_NONE;
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{
	if (m_pAIOutControl)
	{
		delete m_pAIOutControl;
		m_pAIOutControl = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIOut::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 基底クラスの更新
	CPlayerOut::Update(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pAIOutControl)
	{// AIコントロール(外野)の更新
		m_pAIOutControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// カニ歩きモーション設定
//==========================================================================
void CPlayerAIOut::MotionCrab(int nStartKey)
{
	CPlayer::CRAB_DIRECTION playerDir = CPlayer::CRAB_DIRECTION::CRAB_NONE;
	CPlayer::CRAB_DIRECTION inputDir = CPlayer::CRAB_DIRECTION::CRAB_NONE;

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 向き
	MyLib::Vector3 rot = GetPlayer()->GetRotation();

	//--------------------------------
	// プレイヤー方向
	//--------------------------------
	bool bRot = false;
	float fRotY = D3DX_PI * 1.0f + rot.y;
	UtilFunc::Transformation::RotNormalize(fRotY);

	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);

	// 外野ポジションの取得
	CPlayerManager::EOutPos eOutPos = CPlayerManager::GetInstance()->GetOutPosition(GetPlayer());

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// 進行方向の取得
	float fDirection = pControlAIOutMove->GetClabDirection();

	// 角度
	float range[] =
	{
		-1.57f,		// 左
		1.57f,		// 右
		D3DX_PI,	// 上
		0.0f,		// 下
	};

	if (eOutPos == CPlayerManager::EOutPos::OUT_LEFT ||
		eOutPos == CPlayerManager::EOutPos::OUT_RIGHT)
	{
		if (!UtilFunc::Collision::CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
		{// 下向き
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
		{// 上向き
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
		{// 左向き
			if (fDirection == range[2])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
			}
			else if (fDirection == range[3])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
			}
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
		{// 右向き
			if (fDirection == range[2])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
			}
			else if (fDirection == range[3])
			{
				playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
			}
			bRot = true;
		}
		else
		{// 抜けちゃった
			MyAssert::CustomAssert(false, "カニ歩き：どこ見てんねん");
		}
	}
	else
	{
		if (!UtilFunc::Collision::CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
		{// 下向き
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
		{// 上向き
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
		{// 左向き
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
			bRot = true;
		}
		else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
		{// 右向き
			playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
			bRot = true;
		}
		else
		{// 抜けちゃった
			MyAssert::CustomAssert(false, "カニ歩き：どこ見てんねん");
		}
	}

	
	if (fDirection == range[0])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}
	else if (fDirection == range[1])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
	}
	else if (fDirection == range[2])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
	}
	else if (fDirection == range[3])
	{
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}

	if (playerDir == CPlayer::CRAB_DIRECTION::CRAB_NONE ||
		inputDir == CPlayer::CRAB_DIRECTION::CRAB_NONE)
	{// 判定に引っかかっていない
		return;
	}

	// モーション設定
	GetPlayer()->SetMotion(Crab::MOTION_WALK[playerDir][inputDir], nStartKey);
}

//==========================================================================
// カニ
//==========================================================================
bool CPlayerAIOut::IsCrab()
{
	// プレイヤー情報の取得
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return false;

	// 世界にボールがあるか
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// ボール情報
	if (pBall == nullptr) { return false; }

	return true;
}

//==========================================================================
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerAIOut::Hit(CBall* pBall)
{
	// 基底クラスのヒット
	CPlayer::SHitInfo hitInfo = CPlayerOut::Hit(pBall);	// ヒット情報

#if 1
	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // 通常状態
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // キャッチ状態
		// 投げモード
		m_pAIOutControl->SetMode(CPlayerAIOutControl::EMode::MODE_THROW);
	}
#endif

	return hitInfo;
}

//==========================================================================
// デバッグ
//==========================================================================
void CPlayerAIOut::Debug()
{
#if _DEBUG
	// 基底クラスのデバッグ表示
	CPlayerOut::Debug();

	CPlayerAIOutControlAction* pControlAction = dynamic_cast<CPlayerAIOutControlAction*>(GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// 自動投げフラグ設定
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}
//==========================================================================
// 
//  AIプレイヤー処理 [playerAIIn.cpp]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================
#include "playerAIIn.h"
#include "manager.h"
#include "camera.h"

// 使用クラス
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIIn::CPlayerAIIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerIn(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_AI)
{
	// 内野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	// AIコントロール(内野)の生成
	m_pAIControl = CPlayerAIControl::Create(pPlayer);
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIIn::~CPlayerAIIn()
{
	if (m_pAIControl)
	{
		delete m_pAIControl;
		m_pAIControl = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 基底クラスの更新
	CPlayerIn::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// AIコントロール(内野)の更新
	if (m_pAIControl)
	{
		m_pAIControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// カニ歩きモーション設定
//==========================================================================
void CPlayerAIIn::MotionCrab(int nStartKey)
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
	D3DXCOLOR col = D3DXCOLOR();
	float fRotY = D3DX_PI * 1.0f + rot.y;
	UtilFunc::Transformation::RotNormalize(fRotY);

	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);
	if (!UtilFunc::Collision::CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// 下向き
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// 上向き
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
		bRot = true;
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// 左向き
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
	{// 右向き
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
		bRot = true;
		col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{// 抜けちゃった
		MyAssert::CustomAssert(false, "カニ歩き：どこ見てんねん");
	}

	//--------------------------------
	// 方向
	//--------------------------------
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 進行方向の取得
	float fDirection = pControlAIMove->GetClabDirection() + D3DX_PI;

	if (!UtilFunc::Collision::CollisionRangeAngle(fDirection, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// 下向き
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// 上向き
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// 左向き
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
	{// 右向き
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
	}
	else
	{// 抜けちゃった
		MyAssert::CustomAssert(false, "カニ歩き：どこ向いてんねん");
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
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerAIIn::Hit(CBall* pBall)
{
	// 基底クラスのヒット
	CPlayer::SHitInfo hitInfo = CPlayerIn::Hit(pBall);	// ヒット情報

	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // 通常状態
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // キャッチ状態
		// 投げモード
		//m_pAIControl->SetMode(CPlayerAIControl::EMode::MODE_THROW);
	}

	return hitInfo;
}

//==========================================================================
// デバッグ
//==========================================================================
void CPlayerAIIn::Debug()
{
#if _DEBUG
	// 基底クラスのデバッグ表示
	CPlayerIn::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*>(GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// 自動投げフラグ設定
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}

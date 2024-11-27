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
#include "playerAIOut_control.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	namespace Crab	// カニ歩き
	{
		const float RANGE = D3DX_PI * 0.25f;	// 判定角度の半分
		const float RANGE_MIN_MAX[] =	// 判定角度列挙
		{
			D3DX_PI * 1.0f + RANGE,		// 下Max
			D3DX_PI * 1.0f - RANGE,		// 下Min
			D3DX_PI * 0.0f + RANGE,		// 上Max
			D3DX_PI * 0.0f - RANGE,		// 上Min
			D3DX_PI * -0.5f + RANGE,	// 左Max
			D3DX_PI * -0.5f - RANGE,	// 左Min
			D3DX_PI * 0.5f + RANGE,		// 右Max
			D3DX_PI * 0.5f - RANGE,		// 右Min
		};

		// カニ歩きモーション設定列挙
		const CPlayer::EMotion MOTION_WALK[CPlayer::CRAB_DIRECTION::CRAB_MAX][CPlayer::CRAB_DIRECTION::CRAB_MAX] =
		{
			// [ プレイヤー向き ][ インプット向き ]

			{// UP(プレイヤーが上向き)
				CPlayer::EMotion::MOTION_CRAB_FRONT,	// UP 
				CPlayer::EMotion::MOTION_CRAB_BACK,		// DOWN
				CPlayer::EMotion::MOTION_CRAB_LEFT,		// LEFT
				CPlayer::EMotion::MOTION_CRAB_RIGHT		// RIGHT
			},
			{// DOWN(プレイヤーが下向き)
				CPlayer::EMotion::MOTION_CRAB_BACK,		// UP 
				CPlayer::EMotion::MOTION_CRAB_FRONT,	// DOWN
				CPlayer::EMotion::MOTION_CRAB_RIGHT,	// LEFT
				CPlayer::EMotion::MOTION_CRAB_LEFT 		// RIGHT
			},
			{// LEFT(プレイヤーが左向き)
				CPlayer::EMotion::MOTION_CRAB_RIGHT,	// UP 
				CPlayer::EMotion::MOTION_CRAB_LEFT,		// DOWN
				CPlayer::EMotion::MOTION_CRAB_FRONT,	// LEFT
				CPlayer::EMotion::MOTION_CRAB_BACK 		// RIGHT
			},
			{// RIGHT(プレイヤーが右向き)
				CPlayer::EMotion::MOTION_CRAB_LEFT,		// UP 
				CPlayer::EMotion::MOTION_CRAB_RIGHT,	// DOWN
				CPlayer::EMotion::MOTION_CRAB_BACK,		// LEFT
				CPlayer::EMotion::MOTION_CRAB_FRONT 	// RIGHT
			},
		};
	}
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
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{
	if (!m_pAIOutControl)
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
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// 進行方向の取得
	float fDirection = pControlAIOutMove->GetClabDirection() + D3DX_PI;

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

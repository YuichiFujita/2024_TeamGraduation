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
CPlayerAIIn::CPlayerAIIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerIn(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_AI)
{
	// 内野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	// プレイヤーAIコントロールの生成
	m_pAIControl = CPlayerAIControl::Create(pPlayer);
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIIn::~CPlayerAIIn()
{
	delete m_pAIControl;
	m_pAIControl = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 基底クラスの更新
	CPlayerIn::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// コントロール更新
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

	// ラムダ(bool)
	auto CollisionRangeAngle = [](float angle, float maxAngle, float minAngle)
	{
		// 正規化解除
		UtilFunc::Transformation::RotUnNormalize(angle);
		UtilFunc::Transformation::RotUnNormalize(maxAngle);
		UtilFunc::Transformation::RotUnNormalize(minAngle);

		// 度数法に変換
		int nAngle = static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(angle));
		int nMaxAngle = static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(maxAngle));
		int nMinAngle = static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(minAngle));

		if (nMaxAngle <= nMinAngle)
		{// 範囲が360°を跨ぐ場合

			// nAngleがMin以上Max以下。
			bool bRange = (nMaxAngle <= nAngle && nAngle <= nMinAngle);
			return bRange;
		}
		else
		{// 範囲が通常の順序で指定されている場合

			// nAngleがMin以上Max以下。
			bool bRange = (nMaxAngle >= nAngle && nAngle >= nMinAngle);
			return bRange;
		}

		return false;
	};

	//--------------------------------
	// プレイヤー方向
	//--------------------------------
	bool bRot = false;
	D3DXCOLOR col = D3DXCOLOR();
	float fRotY = D3DX_PI * 1.0f + rot.y;
	UtilFunc::Transformation::RotNormalize(fRotY);

	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);
	if (!CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// 下向き
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// 上向き
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
		bRot = true;
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// 左向き
		playerDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
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

	if (!CollisionRangeAngle(fDirection, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// 下向き
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;;
	}
	else if (CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// 上向き
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
	}
	else if (CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// 左向き
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}
	else if (CollisionRangeAngle(fDirection, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
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
		m_pAIControl->SetMode(CPlayerAIControl::EMode::MODE_THROW);
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

//==========================================================================
// 
//  ユーザープレイヤー処理 [playerUser.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerUser.h"
#include "manager.h"
#include "camera.h"

// 使用クラス
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
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

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUser::CPlayerUser(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUser::~CPlayerUser()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerUser::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
// 移動の操作変更
//==========================================================================
void CPlayerUser::ChangeMoveControl(CPlayerControlMove* control)
{
	// 操作クラスの取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	// 操作クラスの入替
	delete pControlMove;
	pControlMove = control;

	// 操作クラスの反映
	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerUser::ChangeActionControl(CPlayerControlAction* control)
{
	// 操作クラスの取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 操作クラスの入替
	delete pControlAction;
	pControlAction = control;

	// 操作クラスの反映
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// カニ歩きモーション設定
//==========================================================================
void CPlayerUser::MotionCrab(int nStartKey)
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
	// 入力方向
	//--------------------------------
	CPlayer::EDashAngle* angle = GetPlayerControlMove()->GetInputAngle();
	if (angle == nullptr) return;

	switch (*angle)
	{
	case CPlayer::EDashAngle::ANGLE_UP:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
		break;

	case CPlayer::EDashAngle::ANGLE_DOWN:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
		break;

	case CPlayer::EDashAngle::ANGLE_RIGHT:
	case CPlayer::EDashAngle::ANGLE_RIGHTUP:
	case CPlayer::EDashAngle::ANGLE_RIGHTDW:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
		break;

	case CPlayer::EDashAngle::ANGLE_LEFT:
	case CPlayer::EDashAngle::ANGLE_LEFTUP:
	case CPlayer::EDashAngle::ANGLE_LEFTDW:
		inputDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
		break;

	default:
		break;
	}

	if (playerDir == CPlayer::CRAB_DIRECTION::CRAB_NONE ||
		inputDir == CPlayer::CRAB_DIRECTION::CRAB_NONE)
	{// 判定に引っかかっていない
		return;
	}

	// モーション設定
	GetPlayer()->SetMotion(Crab::MOTION_WALK[playerDir][inputDir], nStartKey);
}

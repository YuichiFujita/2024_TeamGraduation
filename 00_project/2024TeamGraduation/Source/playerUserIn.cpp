//==========================================================================
// 
//  ユーザープレイヤー処理 [playerUserIn.cpp]
//  Author : 藤田勇一
//  Adder  : 髙田佳依
// 
//==========================================================================
#include "playerUserIn.h"
#include "manager.h"
#include "camera.h"

// 使用クラス
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserIn::CPlayerUserIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerIn(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_USER)
{
	// 内野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUserIn::~CPlayerUserIn()
{

}

//==========================================================================
// カニ歩きモーション設定
//==========================================================================
void CPlayerUserIn::MotionCrab(int nStartKey)
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

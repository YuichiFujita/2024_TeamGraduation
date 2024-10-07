//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playercontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "debugproc.h"
#include "collisionLine_Box.h"
#include "EffekseerObj.h"

namespace
{
	const float MULTIPLIY_DASH = 1.5f;		// ダッシュの倍率
}

//==========================================================================
// 通常移動
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player)
{
	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_GOAL) return;

	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// ダッシュ判定
	bool bDash = false;
	if (pInputGamepad->GetPress(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()) &&
		pInputGamepad->IsTipStick())
	{// 左スティックが倒れてる場合
		//bDash = true;
	}
	player->SetEnableDash(bDash);


	// 移動量取得
	float fMove = player->GetVelocity();
	if (bDash){
		// ダッシュ倍率掛ける
		fMove *= MULTIPLIY_DASH;
	}


	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();
	Camerarot.y -= pCamera->GetOriginRotation().y;

	// 目標の向き取得
	float fRotDest = player->GetRotDest();

	// 移動量取得
	MyLib::Vector3 move = player->GetMove();

	// モーション情報取得
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// 状態取得
	CPlayer::STATE state = player->GetState();

	// 移動方向
	int angle = 0;

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_DEADWAIT)
	{// 移動可能モーションの時

		

		// 移動中にする
		motionFrag.bMove = true;

		if (pInputGamepad->IsTipStick())
		{// 左スティックが倒れてる場合

		}

		// ジャンプ状況取得
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// キャンセル可能 && 移動中

			// モーション終了判定
			pMotion->ToggleFinish(true);

			// 移動モーション
			if (bDash)
			{
				//pMotion->Set(CPlayer::MOTION::MOTION_DASH);
			}
			else
			{
				pMotion->Set(CPlayer::MOTION::MOTION_WALK);
			}
		}


		if (!bJump &&
			(pInputKeyboard->GetTrigger(DIK_SPACE) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx())))
		{// ジャンプ

			//bJump = true;
			//motionFrag.bJump = true;
			//move.y += 17.0f;

			//pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			//// サウンド再生
			//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
		}

		// ジャンプ判定設定
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// 移動可能なモーションか取得
		state != CPlayer::STATE::STATE_DEAD)
	{
		if (pInputKeyboard->GetPress(DIK_A))
		{//←キーが押された,左移動

			if (pInputKeyboard->GetPress(DIK_W))
			{//A+W,左上移動
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//A+S,左下移動
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,左移動
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{//Dキーが押された,右移動

			if (pInputKeyboard->GetPress(DIK_W))
			{//D+W,右上移動
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//D+S,右下移動
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,右移動
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{//Wが押された、上移動
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{//Sが押された、下移動
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
	}

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);

}


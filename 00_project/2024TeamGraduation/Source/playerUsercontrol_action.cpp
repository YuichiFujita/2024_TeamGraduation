//=============================================================================
// 
//  プレイヤーコントロール処理 [playerUsercontrol_action.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerUsercontrol_action.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "ball.h"
#include "playerAction.h"
#include "teamStatus.h"
#include "playerStatus.h"
#include "playerManager.h"
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TAPTIME = 0.125f;		// タップの入力時間
	const float TAPRATE_MIN = 0.6f;	// タップの最小割合
	const float TAPRATE_MAX = 1.0f;	// タップの最大割合
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserControlAction::CPlayerUserControlAction()
{

}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerUserControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		// アクションパターン変更
		CPlayer::EMotion motion = CPlayer::EMotion::MOTION_CATCH_STANCE;
		if (player->IsJump())
		{
			motion = CPlayer::EMotion::MOTION_CATCH_STANCE_JUMP;
		}
		SetPattern(player, motion, CPlayer::EAction::ACTION_CATCH);
	}
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerUserControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr) return;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	// タップ取得
	CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx(), TAPTIME);

	if (pKey->GetTrigger(DIK_K) ||
		tapInfo.bInput)
	{// タップ範囲はパス
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
	}

	if (pKey->GetTrigger(DIK_RETURN) ||
		tapInfo.bComplete)
	{// タップ範囲を超えていた場合は投げ
		ThrowSetting(player);
	}
}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerUserControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	
	if (!player->IsJump() &&
		(pKey->GetTrigger(DIK_SPACE) || 
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// ジャンプしてない && ジャンプボタン

		// ジャンプトリガーON
		SetEnableJumpTrigger(true);

		// ジャンプ設定
		JumpSetting(player);
	}

	if (pKey->GetPress(DIK_SPACE) ||
		pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx()))
	{// ジャンプボタンホールドで上昇
		JumpFloat(player);
	}

	if ((!pKey->GetPress(DIK_SPACE) && !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// ジャンプボタン離した

		// ジャンプトリガーOFF
		SetEnableJumpTrigger(false);
	}
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerUserControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CGameManager::ETeamSide playerTeam = player->GetTeam();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(playerTeam);
	
	// スペシャルゲージMAX＋ボール所持か
	if (pBall == nullptr ||	!pTeamStatus->IsMaxSpecial()) return;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()))
	{
		//発動
		SpecialSetting(player, pBall, pTeamStatus);
	}
}

//==========================================================================
// モテボタン
//==========================================================================
void CPlayerUserControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	player->GetActionPattern()->SetEnableCharm(false);

	if (pKey->GetPress(DIK_C) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_RB, player->GetMyPlayerIdx()))
	{
		//モテアクション発動準備状態
		player->GetActionPattern()->SetEnableCharm(true);
	}
}

//==========================================================================
// ユーザー変更
//==========================================================================
void CPlayerUserControlAction::UserChange(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 0
	// インプット情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// プレイヤー操作権インデックス取得
	const int nPadIdx = player->GetMyPlayerIdx();

	// 右スティックのトリガー入力検知フラグ
	const bool bInput = pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_X)
					 || pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_Y);

	if (bInput && !CPlayerManager::GetInstance()->IsUserChange())
	{

		MyLib::Vector3 posThis = player->GetPosition();	// 自身のプレイヤー位置
		CPlayer* pChange = nullptr;						// 変更プレイヤー
		float fRotMinDiff = D3DX_PI * 2.0f;				// 
		float fRotStick = pPad->GetStickRotR(nPadIdx);	// スティック向き

		// TODO：方向エフェクト
#if 0
		MyLib::Vector3 posStick = player->GetPosition();	// 自身のプレイヤー位置
		posStick.x += sinf(fRotStick) * 150.0f;
		posStick.z += cosf(fRotStick) * 150.0f;

		// 演出
		CEffect3D::Create(
			posStick + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			MyLib::color::White(),
			20.0f, 6.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

		CListManager<CPlayer> list = CPlayerManager::GetInstance()->GetInList(player->GetTeam());	// 内野プレイヤーリスト
		std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
		while (list.ListLoop(itr))
		{ // リスト内の要素数分繰り返す

			CPlayer* pItrPlayer = (*itr);	// プレイヤー情報

			// 既にユーザーの場合次へ
			if (pItrPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) { continue; }

			float fRotSpace = pItrPlayer->GetPosition().AngleXZ(posThis);
			float fRotStickA = (fRotStick + D3DX_PI);
			float fRotSpaceA = (fRotSpace + D3DX_PI);
			float fRotDiff = fabsf(fRotStickA - fRotSpaceA);

			// TODO：方向エフェクト
#if 0
			MyLib::Vector3 posDir = player->GetPosition();	// 自身のプレイヤー位置
			posDir.x += sinf(fRotSpace) * 125.0f;
			posDir.z += cosf(fRotSpace) * 125.0f;

			// 演出
			CEffect3D::Create(
				posDir + MyLib::Vector3(0.0f, 50.0f, 0.0f),
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				MyLib::color::Red(),
				15.0f, 6.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

			if (pChange == nullptr || fRotDiff < fRotMinDiff)
			{ // より差分が小さい向きの場合

				// 今回の差分向きを保存
				fRotMinDiff = fRotDiff;

				// 変更するプレイヤーを保存
				pChange = pItrPlayer;
			}
		}

		if (pChange != nullptr)
		{ // 変更対象が存在する場合

			// TODO：方向エフェクト
#if 0
			float fRotChange = pChange->GetPosition().AngleXZ(posThis);
			MyLib::Vector3 posChange = player->GetPosition();	// 自身のプレイヤー位置
			posChange.x += sinf(fRotChange) * 175.0f;
			posChange.z += cosf(fRotChange) * 175.0f;

			// 演出
			CEffect3D::Create(
				posChange + MyLib::Vector3(0.0f, 50.0f, 0.0f),
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				MyLib::color::Blue(),
				10.0f, 6.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

			// ベースの入れ替え
			CPlayerManager::GetInstance()->SwapBase(player, pChange);

			CPlayerManager::GetInstance()->SetEnableUserChange(true);
		}
	}
#else
	// 最良目標情報
	struct SBestDest
	{
		CPlayer* pPlayer;	// 現在の目標プレイヤー
		float fMinValue;	// 現在の最良値
	};

	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();	// プレイヤーマネージャー
	CInputGamepad* pPad = CInputGamepad::GetInstance();				// パッド情報
	const int nPadIdx = player->GetMyPlayerIdx();					// プレイヤー操作権インデックス

	// 右スティックのトリガー入力検知フラグ
	const bool bInput = pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_X)
					 || pPad->GetRStickTrigger(nPadIdx, CInputGamepad::STICK_AXIS::STICK_Y);

	//if (bInput && !pPlayerManager->IsUserChange())
	{ // 右スティックが入力されている且つ、まだ誰も同フレームでこの操作をしていない場合

		CListManager<CPlayer> list = pPlayerManager->GetInList(player->GetTeam());	// 内野プレイヤーリスト
		std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
		MyLib::Vector3 posThis = player->GetPosition();		// 操作プレイヤー位置
		float fRotStick = pPad->GetStickRotR(nPadIdx);		// スティック向き
		float fRotStickA = fRotStick;

		// TODO：スティック向きのデバッグ表示
		GET_MANAGER->GetDebugProc()->Print("スティック向き：%f\n", fRotStickA);

		// 向き基準の最良目標情報
		SBestDest bestRot;
		bestRot.pPlayer = nullptr;
		bestRot.fMinValue = D3DX_PI * 2.0f;

		// 距離基準の最良目標情報
		SBestDest bestDis;
		bestDis.pPlayer = nullptr;
		bestDis.fMinValue = CGameManager::GetInstance()->GetHalfCourtDiagonal();

		while (list.ListLoop(itr))
		{ // リスト内の要素数分繰り返す

			CPlayer* pItrPlayer = (*itr);	// プレイヤー情報

			// 既にユーザーの場合次へ
			if (pItrPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) { continue; }

#if 1

#if 1
			float fRotSpace = pItrPlayer->GetPosition().AngleXZ(posThis);	// 操作プレイヤーから見たAIの向き
#else
			float fRotSpace = posThis.AngleXZ(pItrPlayer->GetPosition());	// 操作プレイヤーから見たAIの向き
#endif
			float fRotSpaceA = fRotSpace + D3DX_PI;
			float fRotDiff = fRotStickA - fRotSpaceA;

			// TODO：差分向きのデバッグ表示
			GET_MANAGER->GetDebugProc()->Print("ＡＩ向き：%f\n", fRotSpaceA);
			GET_MANAGER->GetDebugProc()->Print("差分向き：%f\n", fRotDiff);

			// 向き基準の最良目標を更新
			if (bestRot.pPlayer == nullptr || bestRot.fMinValue < fRotDiff)
			{ // より最良値が小さい場合

				// 今回の最良値を保存
				bestRot.fMinValue = fRotDiff;

				// 目標プレイヤーを保存
				bestRot.pPlayer = pItrPlayer;
			}
#endif

#if 0
			// 距離基準の最良目標を更新
			if (bestDis.pPlayer == nullptr || bestDis.fMinValue < )
			{ // より最良値が小さい場合

				// 今回の最良値を保存
				bestDis.fMinValue = ;

				// 目標プレイヤーを保存
				bestDis.pPlayer = pItrPlayer;
			}
#endif
		}

		// 最良の目標プレイヤーに操作権を渡す
#if 0
		if (pChange != nullptr)
		{ // 変更対象が存在する場合

			// ベースの入れ替え
			pPlayerManager->SwapBase(player, pChange);

			// ユーザー変更操作済みにする
			pPlayerManager->SetEnableUserChange(true);
		}
#endif
	}
#endif
}

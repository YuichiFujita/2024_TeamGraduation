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
#include "keyconfig_gamepad.h"
#include "map_obstacle.h"
#include "collisionLine_Box.h"
#include "waterripple.h"
#include "course.h"
#include "spline.h"
#include "EffekseerObj.h"
#include "deadplayer.h"
#include "suffocation.h"
#include "goalgametext.h"
#include "guide.h"

// キーコンフィグ
#include "keyconfig_keyboard.h"
#include "keyconfig.h"

namespace
{
	const float MULTIPLIY_DASH = 1.5f;		// ダッシュの倍率
	const float LENGTH_AUTOFACE = 200.0f;	// 自動で向く長さ
	const float LENGTH_COLLISIONRANGE = 500.0f;		// 当たり判定する範囲の長さ
	const float RATIO_COLLISIONRANGE = 0.3f;		// 範囲の長さの最小割合
	const float LENGTH_COLLISIONHEIGHT = 1000.0f;	// 当たり判定する高さ上限
	float ADD_HEIGHT = 25.0f;					// 高さの加算量
	const float MIN_HEIGHT = 100.0f;
	const float HEIGHT_VELOCITY = 10.0f;
	float MAX_SURHEIGHT = 100.0f;
	float SURHEIGHT_VELOCITY = (10.0f);
	int DEFAULT_WATERRIPPLE_INTERVAL = 21;	// 水波紋のインターバル
	const float HEIGHT_MOVETIMER = (1.0f / 0.5f);	// 高さ変化遷移タイマー
	const float COMMAND_HEIGHT = 200.0f;	// 高さ
	float INTERVAL_BRESSEFFECT = 0.32f;	// 息出すまでの間隔
	const float DEFAULT_BRESSSCALE_EFFECT = 90.0f;	// デフォの息エフェクトスケール
	const float MIN_RATIO_HEIGHT_BRESS = 0.2f;	// 息の高さの最小割合
	const float GOAL_INER = 0.02f;
	const float GOAL_GRAVITY = -0.7f;
	const int GOAL_AIRTIMER = 180;
	const float TIME_GOALWAIT = 2.0f;	// ゴールゲーム中の待機時間
}

//==========================================================================
// ブラー表現(マルチターゲットレンダリング)用定数定義
//==========================================================================
namespace MULTITARGET
{
	// OFF時
	const float OFF_ALPHA = (0.0f);		// 目標透明度
	const float OFF_MULTI = (1.0f);		// 目標倍率
	const float OFF_TIMER = (150.0f);	// 遷移タイマー
}

#define GEKIMUZU (true)

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
	m_nIntervalAddRippleCounter = 1;

	// 移動方向
	CPlayer::ANGLE moveAngle = CPlayer::ANGLE::NONE;

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_DEADWAIT &&
		state != CPlayer::STATE::STATE_RETURN &&
		state != CPlayer::STATE::STATE_RESTART)
	{// 移動可能モーションの時

		move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);
		move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);

#if _DEBUG
		if (pInputKeyboard->GetPress(DIK_A))
		{// 左移動

			// 移動中にする
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// 左上移動

				move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// 左下移動

				move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// 左移動

				move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{// 右移動

			// 移動中にする
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// 右上移動

				move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// 右下移動

				move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// 右移動

				move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}

			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{// 上移動

			// 移動中にする
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{// 下移動

			// 移動中にする
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
		else
		{
			motionFrag.bMove = false;
		}
#endif

		// 移動中にする
		motionFrag.bMove = true;

		if (pInputGamepad->IsTipStick())
		{// 左スティックが倒れてる場合

			// 移動中にする
			motionFrag.bMove = true;

			// スティックの向き取得
			float stickrot = pInputGamepad->GetStickRotL(player->GetMyPlayerIdx());
			UtilFunc::Transformation::RotNormalize(stickrot);

#if 0
			// 移動量と向き設定
			move.x += sinf(stickrot + Camerarot.y) * fMove;
			move.z += cosf(stickrot + Camerarot.y) * fMove;
			fRotDest = D3DX_PI + stickrot + Camerarot.y;
#endif
			// 移動方向
			angle = (stickrot <= 0.0f) ? -1 : 1;
			moveAngle = (static_cast<CPlayer::ANGLE>(angle) == 1) ? CPlayer::ANGLE::RIGHT : CPlayer::ANGLE::LEFT;

			if (angle == -1)
			{
				fMove *= 0.3f;
				m_nIntervalAddRippleCounter = 2;
			}
			else
			{
				fMove *= 0.35f;
			}
			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * angle);
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * angle);
			fRotDest = angle * (-D3DX_PI * 0.5f) + Camerarot.y;

			// 移動方向設定
			player->SetMoveAngle(moveAngle);
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

#if 0
		if (!bJump &&
			(pInputKeyboard->GetTrigger(DIK_SPACE) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx())))
		{// ジャンプ

			bJump = true;
			motionFrag.bJump = true;
			move.y += 17.0f;

			pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			// サウンド再生
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
		}
#endif

		// ジャンプ判定設定
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// 移動可能なモーションか取得
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_RETURN &&
		state != CPlayer::STATE::STATE_RESTART)
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

	// 移動方向設定
	player->SetMoveAngle(moveAngle);


#if _DEBUG
	if (!pInputGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_BACK, 0) &&
		!pInputKeyboard->GetPress(DIK_S))
#endif
	{
		// 移動量設定
		player->SetMove(move);
	}

	/*static float height = 30.0f, velocity = 3.1f, thickness = 10.0f;
	static int life = 45;
	static int block = 80, interval = 20;
	static float blocksize = 2.5f;*/

#if 0
	static float height = 31.5f, velocity = 4.0f, thickness = 16.0f;
	static int life = 53;
	static int block = 64;
	static float blocksize = 4.7f;
#else
	static float height = 72.0f, velocity = 3.9f, thickness = 13.0f;
	static int life = 43;
	static int block = 64;
	static float blocksize = 4.5f;
#endif

#if _DEBUG
	ImGui::DragInt("INTERVAL", &m_nIntervalWaterRipple, 1);
	ImGui::DragInt("BLOCK", &block, 1);
	ImGui::DragFloat("BLOCK SIZE", &blocksize, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("height", &height, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("velocity", &velocity, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("thickness", &thickness, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::DragInt("life", &life, 1);
#endif

	bool bCreateRipple = false;	// 波紋生成フラグ

	// 水波紋カウンター加算間隔更新
	m_nAddRippleCounter = (m_nAddRippleCounter + 1) % m_nIntervalAddRippleCounter;

	if (m_nAddRippleCounter == 0)
	{// 水波紋カウンター加算間隔が一周

		// 波紋カウンター加算
		m_nCntWaterRipple = (m_nCntWaterRipple + 1) % m_nIntervalWaterRipple;

		if (m_nCntWaterRipple == 0)
		{
			bCreateRipple = true;
		}

		// 加速中
		if (angle == 1)
		{
			m_nCntWaterRipple = (m_nCntWaterRipple + 1) % m_nIntervalWaterRipple;

			if (m_nCntWaterRipple == 0)
			{
				bCreateRipple = true;
			}
		}

		// 波紋
		if (bCreateRipple)
		{
			MyLib::Vector3 setpos = player->GetPosition();
			setpos.y -= 5.0f;

			// 生成処理
			if (moveAngle != CPlayer::ANGLE::LEFT)
			{
				CWaterRipple::Create(block, blocksize, setpos, height, velocity, thickness, life);
			}
			else
			{
				CWaterRipple::Create(block, blocksize, setpos, height, velocity, thickness, 35);
			}

			// サウンド再生
			int label = CSound::LABEL::LABEL_SE_PLAYERMOVE01 + UtilFunc::Transformation::Random(0, 7);
			CSound::GetInstance()->PlaySound(static_cast<CSound::LABEL>(label));

			// インターバルをランダム調整
			m_nIntervalWaterRipple = DEFAULT_WATERRIPPLE_INTERVAL + UtilFunc::Transformation::Random(-6, 6);
		}
	}
}



//==========================================================================
// アクションのデストラクタ
//==========================================================================
CPlayerControlBaggage::~CPlayerControlBaggage()
{
	if (m_pEffekseerObj != nullptr)
	{
		// SEストップ
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);

		// トリガー送信
		m_pEffekseerObj->SetTrigger(1);
		m_pEffekseerObj->Uninit();
		m_pEffekseerObj = nullptr;
	}


}

//==========================================================================
// アクション
//==========================================================================
void CPlayerControlBaggage::Action(CPlayer* player, CBaggage* pBaggage)
{
	if (!EndCheck(pBaggage))
	{
		if (pBaggage->GetState() == CBaggage::STATE::STATE_DEAD)
		{
			// リトライUI生成
			player->CreateRetryUI();
		}
		return;
	}

	// ゴール中
	{
		CGameManager* pManager = CGame::GetInstance()->GetGameManager();

		if (pManager->GetType() == CGameManager::SceneType::SCENE_GOAL)
		{
			GoalAction(player, pBaggage);
			return;
		}
	}

	// インプット情報取得
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKeyboard = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INKEY);
	CGameManager* pGameMgr = CGame::GetInstance()->GetGameManager();

	if (pGameMgr->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH &&
		(pKeyConfigKeyboard->GetTrigger(INGAME::ACTION::ACT_AIR) ||
			pKeyConfigPad->GetTrigger(INGAME::ACTION::ACT_AIR)))
	{// 空気送り待ちで空気発射

		// メインに移行
		pGameMgr->SetType(CGameManager::SceneType::SCENE_MAIN);
	}

	// 情報取得
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();


#if BRESSRANGE
	if (m_pBressRange == nullptr)
	{
		m_pBressRange = CDebugBressRange::Create();
	}
	if (m_pBressHeight == nullptr)
	{
		m_pBressHeight = CDebugBressRange::Create();
		m_pBressHeight->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));
	}
#endif

	static float up = 8.3f, power = 9.0f;
	//static float up = 8.3f, power = 6.8f;
	//static float up = 2.5f, power = 2.0f;
#if _DEBUG
	ImGui::DragFloat("up", &up, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("power", &power, 0.01f, 0.0f, 0.0f, "%.2f");

	ImGui::DragFloat("Add Height", &ADD_HEIGHT, 1.0f, 0.0f, 0.0f, "%.2f");
#endif

	static float starttimeDownheight = 1.5f;	// 降下が始まるまでの時間
	static float timeDownheight = 2.0f;			// 落ちきるまでの時間
	static float ratioMinDownheight = 0.2f;		// 落ちきった時の再下底割合

#if _DEBUG
	ImGui::DragFloat("Start Time DownHeight", &starttimeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Time DownHeight", &timeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Ratio Min DownHeight", &ratioMinDownheight, 0.01f, 0.0f, 0.0f, "%.2f");	
#endif

	// 荷物の高さで割合設定
	float ratio = (posBaggage.y - posBaggageOrigin.y) / LENGTH_COLLISIONHEIGHT;
	float ratioHeight = 1.0f - ratio;
	ratioHeight = UtilFunc::Transformation::Clamp(ratioHeight, 0.4f, 0.6f);

	// 割合
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.3f, 1.0f);

	// 息の届く横範囲
	float range = ratio * LENGTH_COLLISIONRANGE;

#if BRESSRANGE
	MyLib::Vector3 a = pos, b = pos;
	a.x += range; b.x -= range;
	a.y = posBaggage.y; b.y = posBaggage.y;

	MyLib::Vector3 c = pos;
	c.y = posBaggageOrigin.y + LENGTH_COLLISIONHEIGHT;

	CEffect3D::Create(
		c,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		a,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		b,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	MyLib::Vector3 leftup = posBaggageOrigin, rightup = posBaggageOrigin, leftdw = posBaggageOrigin, rightdw = posBaggageOrigin;
	leftup.y += LENGTH_COLLISIONHEIGHT; leftup.x -= LENGTH_COLLISIONRANGE;
	rightup.y += LENGTH_COLLISIONHEIGHT; rightup.x += LENGTH_COLLISIONRANGE;
	leftdw.x -= LENGTH_COLLISIONRANGE * 0.3f;
	rightdw.x += LENGTH_COLLISIONRANGE * 0.3f;

	// 息の範囲生成
	if (m_pBressRange != nullptr)
	{
		m_pBressRange->SetRange(leftup, rightup, leftdw, rightdw);
		m_pBressRange->SetPosition(pos);
	}
#endif

#if GEKIMUZU

#if _DEBUG
	if (!pInputGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_BACK, 0) &&
		!CInputKeyboard::GetInstance()->GetPress(DIK_S))
#endif
	{
		// 高さ制限
		if (posBaggage.y <= posBaggageOrigin.y)
		{
			posBaggage.y = posBaggageOrigin.y;
			if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
			{
				MyLib::HitResult_Character hitresult = player->Hit(1);
				m_bLandOld = true;

				// ラ王生成
				if (hitresult.isdeath)
				{
					// 窒息
					Suffocation(player, pBaggage);
				}
			}
		}
		else if(!pBaggage->IsLand())
		{

			// 前回が着地
			if (m_bLandOld)
			{
				CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_DROWN);
				float multi = 1.0f - static_cast<float>(player->GetLife()) / static_cast<float>(player->GetLifeOrigin());

				// フィードバックエフェクトOFF
				CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
					MULTITARGET::OFF_ALPHA,
					MULTITARGET::OFF_MULTI,
					MULTITARGET::OFF_TIMER * multi);

				// コントローラー振動停止
				pInputGamepad->SetVibMulti(0.0f);
			}

			// 体力回復
			int setLife = player->GetLife();
			setLife = UtilFunc::Transformation::Clamp(setLife + 1, 0, player->GetLifeOrigin());
			player->SetLife(setLife);

			// 前回着地していない状態に
			m_bLandOld = false;
		}
		else
		{
			// 位置設定
			posBaggage.y = posBaggageOrigin.y;
			if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
			{
				MyLib::HitResult_Character hitresult = player->Hit(1);

				// 前回着地した状態に
				m_bLandOld = true;

				// ラ王生成
				if (hitresult.isdeath)
				{
					// 窒息
					Suffocation(player, pBaggage);
				}
			}
		}
	}

	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_4))
	{
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_DROWN);
	}

	// 位置設定
	if (pBaggage->GetState() != CBaggage::STATE::STATE_DEAD)
	{
		pBaggage->SetPosition(MyLib::Vector3(pos.x, posBaggage.y, pos.z));
	}

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 向き設定
	MyLib::Vector3 setRot;
	setRot.x += sinf(D3DX_PI * 0.5f + 0.0f) * 1.0f;
	setRot.z += cosf(D3DX_PI * 0.5f + 0.0f) * 1.0f;
	pBaggage->SetRotation(MyLib::Vector3(0.0f, setRot.AngleXZ(0.0f), 0.0f));
#endif

#if BRESSRANGE
	if (m_pBressHeight != nullptr)
	{
		float ratiooo = m_fHeight / LENGTH_COLLISIONHEIGHT;

		float rangeee = ratiooo * LENGTH_COLLISIONRANGE;

		/*leftup.x = -rangeee - (LENGTH_COLLISIONRANGE * 0.3f);
		rightup.x = rangeee + (LENGTH_COLLISIONRANGE * 0.3f);*/
		leftup.y = posBaggageOrigin.y + m_fHeight;
		rightup.y = posBaggageOrigin.y + m_fHeight;
		m_pBressHeight->SetRange(leftup, rightup, leftdw, rightdw);
		m_pBressHeight->SetPosition(pos);
	}
#endif
	//=============================
	// 息エフェクト
	//=============================
	BressEffect(player, pBaggage);
	bool bKantsu = CollisionObstacle(player, pBaggage);
	if (pKeyConfigKeyboard->GetPress(INGAME::ACTION::ACT_AIR) ||
		pKeyConfigPad->GetPress(INGAME::ACT_AIR))
	{

		// 高さの降下時間加算
		m_fTimeDownHeight += CManager::GetInstance()->GetDeltaTime();

		if (m_bFall) 
		{// 落下中

			m_bFall = false;
			pBaggage->SetForce(0.0f);


			if (m_pEffekseerObj != nullptr)
			{
				// SEストップ
				CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);

				// トリガー送信
				m_pEffekseerObj->SetTrigger(1);
				m_pEffekseerObj->Uninit();
				m_pEffekseerObj = nullptr;
			}

			// 息エフェクト生成
			MyLib::Vector3 d = pos;
			d.y = posBaggageOrigin.y;

			if (m_pEffekseerObj == nullptr)
			{
				m_pEffekseerObj = CEffekseerObj::Create(
					CMyEffekseer::EFKLABEL::EFKLABEL_BRESS,
					d, 0.0f, 0.0f, DEFAULT_BRESSSCALE_EFFECT, false);
			}

			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_BRESS_STREAM);
		}

		// 息の加算計算
		m_fHeightVelocity += (0.0f - m_fHeightVelocity) * 0.2f;
		m_fHeight += ADD_HEIGHT + m_fHeightVelocity;
		m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

		// 高さの割合
		float ratio = m_fHeight / m_fMaxHeight;

		// SEのピッチ変更
		CSound::GetInstance()->SetFrequency(CSound::LABEL::LABEL_SE_BRESS_STREAM, 0.5f + ratio * 1.5f);

		ratio = UtilFunc::Transformation::Clamp(ratio, MIN_RATIO_HEIGHT_BRESS, 1.0f);

		// 振動
		pInputGamepad->SetVibMulti(1.0f * ratio);
		pInputGamepad->SetVibration(CInputGamepad::VIBRATION_STATE::VIBRATION_STATE_AIR, 0);

		if (posBaggage.y <= posBaggageOrigin.y + m_fHeight &&
			posBaggage.x <= pos.x + range &&
			posBaggage.x >= pos.x - range)
		{// 範囲内

			//if (bKantsu)
			{// 障害物の空気貫通判定

#if GEKIMUZU
				// 荷物へ空気移動量加算
				pBaggage->AddForce(MyLib::Vector3(0.0f, up * ratioHeight, 0.0f), player->GetPosition() + move);
#else
				pBaggage->SetMove(MyLib::Vector3(move.x, pBaggage->GetMove().y, move.z));
				pBaggage->AddForce(MyLib::Vector3(ratio * power, up * ratioHeight, 0.0f), player->GetPosition() + move);
#endif
			}
		}
	}
	else
	{
		// 降下状態
		m_bFall = true;

		// 高さの降下時間加算
		m_fTimeDownHeight -= CManager::GetInstance()->GetDeltaTime() * 2.0f;

		m_fHeight -= ADD_HEIGHT * 2.0f;
		m_fHeightVelocity += (m_fHeightVelocity - HEIGHT_VELOCITY) * 0.1f;
		m_fHeightVelocity = UtilFunc::Transformation::Clamp(m_fHeightVelocity, 0.0f, HEIGHT_VELOCITY);
	}

	// 高さの降下時間補正
	m_fTimeDownHeight = UtilFunc::Transformation::Clamp(m_fTimeDownHeight, 0.0f, starttimeDownheight + timeDownheight);

	// 息の高さ補正
	m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

	// 息の届く最大の高さが降下していく
	if (m_fTimeDownHeight >= starttimeDownheight)
	{
		float timeratio = (m_fTimeDownHeight - starttimeDownheight) / timeDownheight;
		timeratio = UtilFunc::Transformation::Clamp(timeratio, 0.0f, 1.0f);


		float height = (1.0f - timeratio) * LENGTH_COLLISIONHEIGHT;
		height = UtilFunc::Transformation::Clamp(height, LENGTH_COLLISIONHEIGHT * ratioMinDownheight, LENGTH_COLLISIONHEIGHT);

		if (m_bFall && m_fHeight <= height)
		{
			m_fHeight = height;
		}
		else if (!m_bFall)
		{
			m_fHeight = height;
		}
	}

	// 落下状態更新
	if (posBaggage.y <= MIN_HEIGHT)
	{
		m_bFall = true;
	}

	// 息エフェクト状態変更
	if (m_bFall && m_pEffekseerObj != nullptr)
	{
		// ゆっくり消す
		m_pEffekseerObj->SetTrigger(0);
		m_pEffekseerObj->DeleteLater(1.0f);
		m_pEffekseerObj = nullptr;

		// SEストップ
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);
	}


	MyLib::Vector3 d = pos;
	d.y = posBaggageOrigin.y;

	// 位置設定
	if (m_pEffekseerObj != nullptr)
	{
		// 高さの割合
		float ratio = m_fHeight / m_fMaxHeight;
		ratio = UtilFunc::Transformation::Clamp(ratio, MIN_RATIO_HEIGHT_BRESS, 1.0f);

		m_pEffekseerObj->SetPosition(d);
		m_pEffekseerObj->SetScale(DEFAULT_BRESSSCALE_EFFECT * ratio);

	}
}

//==========================================================================
// ゴール時の設定
//==========================================================================
void CPlayerControlBaggage::GoalSetting()
{
	// 息エフェクト状態変更
	if (m_pEffekseerObj != nullptr)
	{
		// ゆっくり消す
		m_pEffekseerObj->SetTrigger(0);
		m_pEffekseerObj->DeleteLater(1.0f);
		m_pEffekseerObj = nullptr;

		// SEストップ
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);
	}
}

//==========================================================================
// 窒息
//==========================================================================
void CPlayerControlBaggage::Suffocation(CPlayer* player, CBaggage* pBaggage)
{
	// 死亡体生成
	CDeadPlayer::Create(player->GetPosition());

	// 落下状態
	pBaggage->SetState(CBaggage::STATE::STATE_FALL);

	// 窒息UI生成
	if (m_pSuffocation == nullptr)
	{
		m_pSuffocation = CSuffocation::Create();
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_RAOU);
	}
}

//==========================================================================
// ゴールアクション
//==========================================================================
void CPlayerControlBaggage::GoalAction(CPlayer* player, CBaggage* pBaggage)
{
	if (pBaggage->GetState() == CBaggage::STATE::STATE_RETURN)
	{
		return;
	}

	// フィードバックエフェクトリセット
	CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
		0.0f,
		1.0f,
		0.01f);


	// インプット情報取得
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKeyBoard = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INKEY);
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	CCameraMotion* pCamMotion = pCamera->GetMotion();

	if (m_state != STATE::STATE_RELEASE)
	{// 手放されてないとき
		pBaggage->SetState(CBaggage::STATE::STATE_GOAL);
	}

	static float up = 0.175f, power = 0.4f;

	//=============================
	// 息エフェクト
	//=============================
	BressEffect(player, pBaggage);
	if ((m_state == STATE::STATE_WAIT || m_state == STATE::STATE_PRESS) && (pKeyConfigKeyBoard->GetPress(INGAME::ACT_AIR) ||
		pKeyConfigPad->GetPress(INGAME::ACT_AIR)))
	{
		// 待機状態時、遷移
		if (m_state == STATE::STATE_WAIT)
		{
			pCamMotion->SetMotion(CCameraMotion::MOTION::MOTION_GOALBAG, CCameraMotion::EASING::Linear);

			// ガイド削除
			if (m_pGuide != nullptr)
			{
				m_pGuide->Kill();
				m_pGuide = nullptr;
			}
		}

		m_state = STATE::STATE_PRESS;

		// 自動手放し減算
		m_nGoalTimer--;
		if (m_nGoalTimer <= 0)
		{
			m_state = STATE::STATE_RELEASE;
		}
	}
	// 入力されていない
	else if(
		!pKeyConfigKeyBoard->GetPress(INGAME::ACT_AIR) &&
		!pKeyConfigPad->GetPress(INGAME::ACT_AIR))
	{
		// 状態ごとに設定
		switch (m_state)
		{
		case STATE::STATE_NONE:

			// 荷物がおちてカメラモーションも終わり
			if (pBaggage->GetMove().y == 0.0f && pCamMotion->IsPause())
			{
				m_state = STATE::STATE_WAIT;

				// ゴールゲームテキスト生成
				CGoalGameText::Create();

				// 目的地紹介するならここ
			}
			m_nGoalTimer = GOAL_AIRTIMER;
			break;

		case STATE::STATE_WAIT:
			m_nGoalTimer = GOAL_AIRTIMER;

			// ゴールの待機時間加算
			m_fGoalWaitTimer += CManager::GetInstance()->GetDeltaTime();
			if (TIME_GOALWAIT <= m_fGoalWaitTimer &&
				m_pGuide == nullptr)
			{// 時間経過

				// 操作ガイド生成
				m_pGuide = CGuide::Create(CGuide::Type::GOAL);
			}
			break;

		case STATE::STATE_PRESS:
			m_state = STATE::STATE_RELEASE;
			break;

		case STATE::STATE_RELEASE:
			break;

		default:
			m_state = STATE::STATE_NONE;
			break;
		}		
	}

	MyLib::Vector3 move = pBaggage->GetMove();
	MyLib::Vector3 pos = pBaggage->GetPosition();

	// 状態ごとに設定
	switch (m_state)
	{
	case STATE::STATE_NONE:
	{
		move.y += GOAL_GRAVITY;
	}
		break;

	case STATE::STATE_WAIT:
	{
		move.y += GOAL_GRAVITY;
	}
		break;

	case STATE::STATE_PRESS:
	{
		// 荷物へ空気移動量加算
		move.x += power;
		move.y += up;

		// 高さ制限
		if (pBaggage->GetPosition().y <= pBaggage->GetOriginPosition().y)
		{
			MyLib::Vector3 move = pBaggage->GetMove();
			MyLib::Vector3 pos = pBaggage->GetPosition();
			pos.y = pBaggage->GetOriginPosition().y;
		}
	}
		break;

	case STATE::STATE_RELEASE:
	{
		if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
		{
			move.x += -move.x * GOAL_INER;
			move.z += -move.z * GOAL_INER;
			move.y += GOAL_GRAVITY;
		}

		// 高さ制限
		if (pBaggage->GetPosition().y <= pBaggage->GetOriginPosition().y)
		{
			MyLib::Vector3 move = pBaggage->GetMove();
			MyLib::Vector3 pos = pBaggage->GetPosition();
			pos.y = pBaggage->GetOriginPosition().y;
		}

		if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_SEND);
		}
	}
		break;

	default:
		break;
	}

	// 値変更
	if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
	{
		pos += move;
	}

	if (m_state != STATE::STATE_RELEASE &&
		pos.y <= pBaggage->GetOriginPosition().y)
	{
		pos.y = pBaggage->GetOriginPosition().y;
		move.y = 0.0f;

	}

	// リターン以外は更新
	if (pBaggage->GetState() != CBaggage::STATE::STATE_RETURN)
	{
		if (pBaggage->GetState() != CBaggage::STATE::STATE_FALL)
		{
			pBaggage->SetMove(move);
			pBaggage->SetPosition(pos);
			pCamMotion->SetPosition(pos);
		}
	}
}

//==========================================================================
// 息エフェクト
//==========================================================================
void CPlayerControlBaggage::BressEffect(CPlayer* player, CBaggage* pBaggage)
{
	// 息時間加算
	m_fBressTimer += CManager::GetInstance()->GetDeltaTime();

	static int createIdx = 4;
	static int Life = 50;
	static int Randrange = 9;
	static int XZrange = 492;
	static int Yrange = 487;
	static float defradius = 61.80f;
	static float gravity = 1.7f;
	static float movefactor = 0.4f;

	// カラーエディット
	static ImVec4 myColor = ImVec4(1.0f, 1.0, 1.0, 0.08235f); // RGBA

#if _DEBUG
	if (ImGui::TreeNode("Bress"))
	{
		ImGui::DragInt("CreateNum", &createIdx, 1);
		ImGui::DragFloat("Interval", &INTERVAL_BRESSEFFECT, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragInt("Interval_Randrange", &Randrange, 1);
		ImGui::DragInt("Life", &Life, 1);
		ImGui::DragInt("XZrange", &XZrange, 1);
		ImGui::DragInt("Yrange", &Yrange, 1);
		ImGui::DragFloat("defradius", &defradius, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("gravity", &gravity, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Move factor", &movefactor, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::ColorEdit4("Color", &myColor.x);

		ImGui::TreePop();
	}
#endif

	if (m_fIntervalBress > m_fBressTimer)
	{
		return;
	}

	m_fBressTimer = 0.0f;
	m_fIntervalBress = INTERVAL_BRESSEFFECT + UtilFunc::Transformation::Random(-Randrange, Randrange) * 0.01f;


	// 情報取得
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 rot = player->GetRotation();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();

	// 息の発生地点
	MyLib::Vector3 bresspos = pos;
	bresspos.y = posBaggageOrigin.y;

	for (int i = 0; i < createIdx; i++)
	{
		MyLib::Vector3 move;
		float randmoveXZ = UtilFunc::Transformation::Random(XZrange, XZrange + 100) * 0.1f;
		float randmoveY = UtilFunc::Transformation::Random(Yrange, Yrange + 20) * 0.1f;
		float randRadius = UtilFunc::Transformation::Random(-10, 10);



		// 移動量計算
		float moveLen = player->GetMoveLength();
		moveLen += move.x;

		// 少し先の位置との向きで算出
		MyLib::Vector3 newPosition = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), moveLen);
		float angle = pos.AngleXZ(newPosition);

		if (i % 2 == 0)
		{
			angle += D3DX_PI;
		}

		move.x = sinf(D3DX_PI * 0.5f + angle) * randmoveXZ;
		move.y = randmoveY;
		move.z = cosf(D3DX_PI * 0.5f + angle) * randmoveXZ;

		// 半径
		float radius = defradius + randRadius;


		CEffect3D* pEffect = CEffect3D::Create(
			bresspos,
			move,
			D3DXCOLOR(myColor.x, myColor.y, myColor.z, myColor.w),
			radius, Life,
			CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI,
			CEffect3D::TYPE::TYPE_SMOKE);
		pEffect->SetEnableGravity();
		pEffect->SetGravityValue(-gravity);
		pEffect->SetMoveFactor(movefactor);
	}
}

//==========================================================================
// 荷物の終了判定
//==========================================================================
bool CPlayerControlBaggage::EndCheck(CBaggage* pBaggage)
{
	if (pBaggage == nullptr) { return false; }

	if (pBaggage->IsEnd()) { return false; }

	return true;
}

//==========================================================================
// 初期状態に戻す処理
//==========================================================================
void CPlayerControlBaggage::Reset(CPlayer* player, CBaggage* pBaggage)
{
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();
	pBaggage->SetPosition(MyLib::Vector3(pos.x, posBaggageOrigin.y, pos.z));
	m_state = STATE::STATE_NONE;

	if (m_pSuffocation != nullptr)
	{
		m_pSuffocation->Uninit();
		m_pSuffocation = nullptr;
	}
}

//==========================================================================
// エフェクト停止処理
//==========================================================================
void CPlayerControlBaggage::EffectStop()
{
	// 息が使用されている
	if (m_pEffekseerObj != nullptr)
	{
		// ゆっくりけす
		m_pEffekseerObj->SetTrigger(0);
		m_pEffekseerObj->DeleteLater(1.0f);
		m_pEffekseerObj = nullptr;

		// SEストップ
		CSound::GetInstance()->StopSound(CSound::LABEL::LABEL_SE_BRESS_STREAM);

		m_fTimeDownHeight = 0.0f;
		m_bFall = true;
	}
}

//==========================================================================
// 障害物判定
//==========================================================================
bool CPlayerControlBaggage::CollisionObstacle(CPlayer* player, CBaggage* pBaggage)
{
	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	// 情報取得
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();

	float range = LENGTH_COLLISIONRANGE * RATIO_COLLISIONRANGE;
	MyLib::AABB bressAABB = MyLib::AABB(MyLib::Vector3(-range, 0.0f, -range), MyLib::Vector3(range, LENGTH_COLLISIONHEIGHT, range));

	// 位置設定
	MyLib::Matrix mtx;
	MyLib::Vector3 trans = posBaggage;
	trans.y = posBaggageOrigin.y;

	mtx.Translation(trans);

#if BRESSRANGE
	// ボックス生成
	if (m_pBox == nullptr)
	{
		m_pBox = CCollisionLine_Box::Create(bressAABB, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}

	if (m_pBox != nullptr)
	{
		m_pBox->SetPosition(trans);
	}
#endif

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		// 情報取得
		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();

		if (info.setup.isAir == 0) continue;	// 空気貫通判定

		MyLib::Vector3 ObjPos = pObj->GetPosition();

		if (posBaggage.y <= ObjPos.y) continue;	// 障害物と荷物の高さ判定

		for (const auto& collider : info.boxcolliders)
		{
			if (UtilFunc::Collision::IsAABBCollidingWithBox(bressAABB, mtx, MyLib::AABB(collider.vtxMin, collider.vtxMax), collider.worldmtx))
			{
#if BRESSRANGE
				m_pBressRange->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.3f));
#endif
				return false;
			}
		}
	}

#if BRESSRANGE
	m_pBressRange->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
#endif
	return true;
}


//==========================================================================
// 浮上操作
//==========================================================================
float CPlayerControlSurfacing::Surfacing(CPlayer* player)
{
	// インプット情報取得
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKeyboard = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	// 浮上判定
	bool bUp = false;

	// モーション情報取得
	CMotion* pMotion = player->GetMotion();

	//if (pMotion->) {	// 浮上可能ではない
	//	return m_fHeight;
	//}

	if (pKeyConfigKeyboard->GetPress(INGAME::ACTION::ACT_UPDOWN) ||
		pKeyConfigPad->GetPress(INGAME::ACTION::ACT_UPDOWN))
	{// 入力している
		bUp = true;
	}

#if _DEBUG
	ImGui::DragFloat("SurHeight", &MAX_SURHEIGHT, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("SurSpeed", &SURHEIGHT_VELOCITY, 0.1f, 0.0f, 0.0f, "%.2f");
#endif


	if (bUp) {	// 上昇
		m_fHeight += SURHEIGHT_VELOCITY;

		if (m_fHeight > MAX_SURHEIGHT) {
			m_fHeight = MAX_SURHEIGHT;
		}
	}
	else {	// 下降
		m_fHeight -= SURHEIGHT_VELOCITY;

		if (m_fHeight < 0.0f) {
			m_fHeight = 0.0f;
		}
	}

	return m_fHeight;
}

//==========================================================================
// トリック
//==========================================================================
void CPlayerControlTrick::Trick(CPlayer* player, int& nIdx, bool& bValue)
{
	int type = -1;
	m_pCommandPad->GetSuccess(nIdx, type);
	if (nIdx >= 0) bValue = true;

	CBaggage* bag = player->GetBaggage();

	// 種類によって効果を与える
	switch (type)
	{
	case CCommand::TYPE::TYPE_ROTATION:
	{
		// 荷物を高速回転させる
		MyLib::Vector3 rot = bag->GetVeloRot();
		rot.x += 1.0f;
		bag->SetVeloRot(rot);
	}
		break;

	case CCommand::TYPE::TYPE_UP:
	{
		// 荷物を上昇させる
		MyLib::Vector3 move = bag->GetMove();
		move.y += 5.0f;
		bag->SetMove(move);
	}
		break;
	case CCommand::TYPE::TYPE_FLY:
	{
		if (m_fHeightTimer < 1.0f)
		{
			m_fHeightTimer += HEIGHT_MOVETIMER * CManager::GetInstance()->GetDeltaTime();

			if (m_fHeightTimer >= 1.0f)
			{
				m_fHeightTimer = 1.0f;
			}
		}
	}
		break;
	default:
		if (m_fHeightTimer > 0.0f)
		{
			m_fHeightTimer -= HEIGHT_MOVETIMER * CManager::GetInstance()->GetDeltaTime();

			if (m_fHeightTimer <= 0.0f)
			{
				m_fHeightTimer = 0.0f;
			}
		}
		break;
	}

	// 高さを設定する
	if(m_fHeightTimer != 0.0f)
	{
		m_fHeight = UtilFunc::Correction::EasingLinear(0.0f, COMMAND_HEIGHT, m_fHeightTimer);
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerControlTrick::Uninit()
{
	if (m_pCommandPad == nullptr) return;
	m_pCommandPad->Uninit();
	delete m_pCommandPad;
	m_pCommandPad = nullptr;
}

//=============================================================================
// 
//  プレイヤー処理 [player.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "player.h"
#include "game.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "calculation.h"
#include "Xload.h"
#include "model.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"
#include "sound.h"
#include "fade.h"
#include "listmanager.h"
#include "gamemanager.h"
#include "MyEffekseer.h"
#include "map.h"
#include "edit_map.h"

// 使用クラス
#include "playerAction.h"
#include "playerStatus.h"
#include "playercontrol_action.h"
#include "playercontrol_move.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\main_01\\setup_player.txt";	// キャラクターファイル
	const float DODGE_RADIUS = 300.0f;			// 回避範囲
	const float JUST_VIEW = 90.0f;	//ジャストキャッチ時の方向ゆとり(左右1/8π)
}

namespace Knockback
{
	const float HEIGHT = 50.0f;					// 最大高度
	const float HEIGHT_OUTCOURT = 50.0f;		// 最大高度(コート越え)
	const float DAMAGE = 50.0f;		// ダメージ
	const float DEAD = 100.0f;		// 死亡
}

namespace Catch
{
	const float Impact[CBall::EAttack::ATK_MAX] =	// 衝撃
	{
		0.0f,	// なし
		0.5f,	// 通常
		1.0f,	// ジャンプ
		5.0,	// スペシャル
	};

	const float ANGLE = 160;	// キャッチ判定角度(度数法)
}

namespace StateTime
{
	const float DAMAGE = 0.5f;		// ダメージ
	const float DEAD = 2.0f;		// 死亡
	const float INVINCIBLE = 0.8f;	// 無敵
	const float CATCH = 0.5f;		// キャッチ
	const float COURT_RETURN = 2.0f;		// コートに戻ってくる
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayer::STATE_FUNC CPlayer::m_StateFunc[] =	// 状態関数
{
	&CPlayer::StateNone,				// なし
	&CPlayer::StateInvincible,			// 無敵
	&CPlayer::StateDamage,				// ダメージ
	&CPlayer::StateDead,				// 死亡
	&CPlayer::StateDeadAfter,			// 死亡後
	&CPlayer::StateDodge,				// 回避
	&CPlayer::StateCatch_Normal,		// 通常キャッチ
	&CPlayer::StateCatch_Just,			// ジャストキャッチ
	&CPlayer::StateOutCourt,			// コート越え
	&CPlayer::StateOutCourt_Return,		// コートに戻る
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CPlayer> CPlayer::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayer::CPlayer(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;			// 状態
	m_Oldstate = STATE_NONE;		// 前回の状態
	m_fStateTime = 0.0f;			// 状態時間

	// オブジェクトのパラメータ
	m_mMatcol = MyLib::Color();			// マテリアルの色
	m_sKnockback = SKnockbackInfo();		// ノックバック情報

	// 行動フラグ
	m_bPossibleMove = false;		// 移動可能フラグ
	m_bJump = false;				// ジャンプ中かどうか
	m_bDash = false;				// ダッシュ判定
	m_sMotionFrag = SMotionFrag();	// モーションのフラグ

	// パターン用インスタンス
	m_pActionPattern = nullptr;	// アクションパターン
	m_pStatus = nullptr;		// ステータス
	m_pControlMove = nullptr;	// 移動操作
	m_pControlAction = nullptr;	// アクション操作

	// その他
	m_nMyPlayerIdx = 0;				// プレイヤーインデックス番号
	m_pShadow = nullptr;			// 影の情報
	m_pBall = nullptr;				// ボールの情報
	m_sDamageInfo = SDamageInfo();	// ダメージ情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayer::~CPlayer()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayer::Init()
{
	// 種類の設定
	SetType(TYPE_PLAYER);

	// ダメージ受け付け判定
	m_sDamageInfo.bReceived = true;
	m_sDamageInfo.fReceiveTime = 0.0f;

	m_state = STATE_NONE;	// 状態
	m_Oldstate = m_state;
	m_sMotionFrag.bMove = true;
	m_bPossibleMove = true;

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// プレイヤーインデックス番号を設定
	m_nMyPlayerIdx = m_List.GetNumAll();

	// 割り当て
	m_List.Regist(this);

	// アクションパターン
	if (m_pActionPattern == nullptr)
	{
		m_pActionPattern = DEBUG_NEW CPlayerAction(this);
	}

	// ステータス
	if (m_pStatus == nullptr)
	{
		m_pStatus = DEBUG_NEW CPlayerStatus(this);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Uninit()
{
	// 影
	m_pShadow = nullptr;
	
	//ボール
	m_pBall = nullptr;

	// 操作系
	DeleteControl();

	// ステータス
	SAFE_DELETE(m_pStatus);

	// 終了処理
	CObjectChara::Uninit();

	// 削除
	m_List.Delete(this);
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CPlayer::Kill()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// アクションパターン
	if (m_pActionPattern != nullptr)
	{
		delete m_pActionPattern;
		m_pActionPattern = nullptr;
	}

	// ステータス
	SAFE_DELETE(m_pStatus);

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsDeath()) return;

	// エディット中は抜ける
	if (CGame::GetInstance()->GetEditType() != CGame::GetInstance()->EDITTYPE_OFF)
	{
		return;
	}

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// フラグリセット
	ResetFrag();

	// 親の更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 操作
	Controll(fDeltaTime, fDeltaRate, fSlowRate);

	// モーションの設定処理
	if (CGame::GetInstance()->GetGameManager()->IsControll())
	{
		MotionSet();
	}

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// アクション更新
	if (m_pActionPattern != nullptr)
	{
		m_pActionPattern->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 位置の制限
	LimitPos();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetTargetPosition(pos);
	}

	// 影の位置更新
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

#if _DEBUG	// デバッグ処理

	std::string treename = "Player" + std::to_string(m_nMyPlayerIdx);	// ツリー名
	if (ImGui::TreeNode(treename.c_str()))
	{
		Debug();
		ImGui::TreePop();
	}
#endif

}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayer::Controll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キーボード情報取得
	CInputKeyboard *pKey = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pPad = CInputGamepad::GetInstance();

	if (CGame::GetInstance()->GetGameManager()->IsControll() &&
		m_bPossibleMove)
	{// 行動できるとき

		// 操作
		Operate(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();

	// 移動量加算
	pos += move * fDeltaRate * fSlowRate;

	// 慣性補正
	EAction act = m_pActionPattern->GetAction();
	float ratio = 0.25f;
	if (act == EAction::ACTION_BLINK)
	{
		ratio = 0.1f;
	}
	move.x += (0.0f - move.x) * (ratio * fDeltaRate * fSlowRate);
	move.z += (0.0f - move.z) * (ratio * fDeltaRate * fSlowRate);

	// 重力処理
	move.y -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 現在と目標の差分を求める
	float fRotDiff = GetRotDest() - rot.y;
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	// 角度の補正をする
	rot.y += fRotDiff * (0.25f * fDeltaRate * fSlowRate);
	UtilFunc::Transformation::RotNormalize(rot.y);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// 操作関連削除
//==========================================================================
void CPlayer::DeleteControl()
{
	if (m_pActionPattern != nullptr)
	{// アクションパターン
		delete m_pActionPattern;
		m_pActionPattern = nullptr;
	}
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::SetMotion(int motionIdx) const
{
	//TAKADA: はじく条件(死んだら)
	//if (m_sMotionFrag.bDead) return;

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx);
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::MotionSet()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;
	// 現在の種類取得
	int nType = pMotion->GetType();
	int nOldType = pMotion->GetOldType();

	// 移動できないと通さない
	if (!m_bPossibleMove) return;

	// 死亡時通さない
	if (m_sMotionFrag.bDead) return;

	// 再生中
	if (!pMotion->IsFinish()) return;

	if (m_sMotionFrag.bMove)
	{// 移動していたら

		m_sMotionFrag.bMove = false;	// 移動判定OFF

		// 移動モーション
		if (m_bDash)
		{// ダッシュモーション
			m_bDash = false;
			SetMotion(MOTION_RUN);
		}
		else
		{// 歩行モーション
			SetMotion(MOTION_WALK);
		}
	}
	else if (m_sMotionFrag.bJump)
	{// ジャンプ中

		// ジャンプのフラグOFF
		m_sMotionFrag.bJump = false;

		// ジャンプモーション
	}
	else
	{
		// ニュートラルモーション
		SetMotion(MOTION_DEF);
	}
}

//==========================================================================
// フラグリセット
//==========================================================================
void CPlayer::ResetFrag()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	//キャッチできない状態
	m_sMotionFrag.bCatch = false;
	m_sMotionFrag.bCatchJust = false;

	switch (nType)
	{
	default:
		break;
	}
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CPlayer::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	switch (nType)
	{
	case EMotion::MOTION_THROW:
	case EMotion::MOTION_THROW_RUN:

		if (m_pBall != nullptr)
		{// 通常投げ
			m_pBall->ThrowNormal(this);
		}

		break;

	case EMotion::MOTION_THROW_JUMP:

		if (m_pBall != nullptr)
		{// ジャンプ投げ
			m_pBall->ThrowJump(this);
		}

		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CPlayer::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;
	int nType = pMotion->GetType();

	switch (nType)
	{
	case EMotion::MOTION_CATCH_STANCE:

		// キャッチフラグON
		m_sMotionFrag.bCatch = true;

		if (nCntATK == 1)
		{//2個目(1)がジャスト

			//ジャストフラグON
			m_sMotionFrag.bCatchJust = true;
		
			CEffect3D::Create(
				GetPosition(),
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				80.0f, 1.0f/60.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}

		break;

	default:
		break;
	}

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);

	if (ATKInfo.fRangeSize == 0.0f)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		ATKInfo.fRangeSize, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	if (ATKInfo.bEndAtk)
	{// 終了してたら抜ける
		return;
	}
}

//==========================================================================
// キャッチ時処理(地上・通常)
//==========================================================================
void CPlayer::CatchSettingLandNormal(CBall::EAttack atkBall)
{
	switch (atkBall)
	{
	case CBall::ATK_NORMAL:
		SetMotion(EMotion::MOTION_CATCH_NORMAL);
		break;

	case CBall::ATK_JUMP:
		SetMotion(EMotion::MOTION_CATCH_JUMP);
		break;

	case CBall::ATK_SPECIAL:	//TODO: スペシャルに変更
		SetMotion(EMotion::MOTION_CATCH_JUMP);
		break;

	default:
		break;
	}

	// キャッチ状態
	SetState(EState::STATE_CATCH_NORMAL);
}

//==========================================================================
// キャッチ時処理(地上・ジャスト)
//==========================================================================
void CPlayer::CatchSettingLandJust(CBall::EAttack atkBall)
{
	MyLib::Vector3 pos = GetPosition();
	pos.y += 130.0f;

	//演出
	CEffect3D::Create(
		pos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		80.0f, 4.0f / 60.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	switch (atkBall)
	{
	case CBall::ATK_NORMAL:
		SetMotion(EMotion::MOTION_JUSTCATCH_NORMAL);
		break;

	case CBall::ATK_JUMP:
		SetMotion(EMotion::MOTION_JUSTCATCH_JUMP);
		break;

	case CBall::ATK_SPECIAL:	//TODO: スペシャルに変更
		SetMotion(EMotion::MOTION_JUSTCATCH_JUMP);
		break;

	default:
		break;
	}

	// ジャストキャッチ状態
	SetState(EState::STATE_CATCH_JUST);
}

//==========================================================================
// 位置制限
//==========================================================================
void CPlayer::LimitPos()
{
	MyLib::Vector3 pos = GetPosition();
	
	if (m_state != EState::STATE_OUTCOURT &&
		m_state != EState::STATE_OUTCOURT_RETURN)
	{//コート越え状態以外はコート内補正
		CGame::GetInstance()->GetGameManager()->SetPosLimit(pos);
	}

	if (pos.y <= 0.0f)
	{
		pos.y = 0.0f;

		if (m_bJump)
		{// ジャンプ中着地
			GetMotion()->Set(EMotion::MOTION_LAND);
		}

		// 重力リセット
		m_bJump = false;
		MyLib::Vector3 move = GetMove();
		move.y = 0.0f;
		SetMove(move);
	}
	SetPosition(pos);
	return;
}

//==========================================================================
// ヒット処理
//==========================================================================
#if 0
MyLib::HitResult_Character CPlayer::Hit(const int nValue)
{
	MyLib::HitResult_Character hitresult = {};

	if (m_state == STATE::STATE_DEAD ||
		m_state == STATE::STATE_DEADWAIT)
	{
		hitresult.isdeath = true;
		return hitresult;
	}

	// 体力取得
	int nLife = GetLife();
	nLife -= nValue;
	nLife = UtilFunc::Transformation::Clamp(nLife, 0, GetLifeOrigin());

	// 体力設定
	SetLife(nLife);

	// 振動
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (nLife <= 0)
	{// 体力がなくなったら

		// 死亡時の設定
		DeadSetting(&hitresult);
	}

	// 当たった判定を返す
	return hitresult;
}
#else
bool CPlayer::Hit(CBall* pBall)
{
	CGameManager::TeamSide sideBall = pBall->GetTypeTeam();	// ボールチームサイド
	CBall::EAttack atkBall	= pBall->GetTypeAtk();	// ボール攻撃種類
	CBall::EState stateBall = pBall->GetState();	// ボール状態
	MyLib::Vector3 posB = pBall->GetPosition();		// ボール位置
	MyLib::HitResult_Character hitresult = {};

	//死亡状態ならすり抜け
	if (m_state == EState::STATE_DEAD)
	{
		return true;
	}

	if (stateBall == CBall::STATE_LAND)
	{ // ボールが着地している場合

		// ボールをキャッチ
		pBall->CatchLand(this);
		return false;
	}

	// 味方のボールならすり抜ける
	if (m_pStatus->GetTeam() == sideBall) { return false; }

	if (m_sMotionFrag.bCatch &&
		UtilFunc::Collision::CollisionViewRange3D(GetPosition(), posB, GetRotation().y, Catch::ANGLE))
	{ // キャッチアクション中だった中でも受け付け中の場合

		// キャッチ時処理
		CatchSetting(pBall);
		return false;
	}

	// ダメージを受け付けないならすり抜ける
	if (!m_sDamageInfo.bReceived) { return false; }

	// リバウンドボールの場合キャッチする
	if (stateBall == CBall::STATE_REBOUND)
	{
		pBall->CatchAttack(this);

		//TAKADA: カバー対象を回復

		return false;
	}

	// ダメージを与える
	//m_pStatus->LifeDamage(pBall->GetDamage());	// TODO : 後からBall内の攻撃演出をストラテジーにして、GetDamageを作成
	m_pStatus->LifeDamage(10);

	if (GetLife() <= 0)
	{ // 体力がない場合

		// 終活
		DeadSetting(&hitresult, pBall);
	}
	else
	{ // 体力がある場合

		// ダメージ状態にする
		DamageSetting(pBall);
	}

	return true;
}
#endif

//==========================================================================
// 死亡時の設定
//==========================================================================
void CPlayer::DeadSetting(MyLib::HitResult_Character* result, CBall* pBall)
{
	// 死亡状態にする
	SetState(STATE_DEAD);
	SetMotion(MOTION_DEAD);
	m_sMotionFrag.bDead = true;

	// ノックバックの位置設定
	MyLib::Vector3 vecBall = pBall->GetMove().Normal();
	MyLib::Vector3 posS = GetPosition();
	MyLib::Vector3 posE = posS;
	posE.x += vecBall.x * Knockback::DEAD;
	posE.z += vecBall.z * Knockback::DEAD;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	// 死んだ
	result->isdeath = true;
}

//==========================================================================
// ダメージ発生時設定
//==========================================================================
void CPlayer::DamageSetting(CBall* pBall)
{
	// ダメージ状態にする
	SetState(EState::STATE_DMG);
	SetMotion(EMotion::MOTION_DAMAGE);

	// ノックバックの位置設定
	MyLib::Vector3 vecBall = pBall->GetMove().Normal();
	MyLib::Vector3 posS = GetPosition();	//始点
	MyLib::Vector3 posE = posS;				//終点
	posE.x += vecBall.x * Knockback::DAMAGE;
	posE.z += vecBall.z * Knockback::DAMAGE;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	// ダメージ受付時間を設定
	m_sDamageInfo.fReceiveTime = StateTime::DAMAGE;
}

//==========================================================================
// キャッチ時処理
//==========================================================================
void CPlayer::CatchSetting(CBall* pBall)
{
	// ボールをキャッチ
	pBall->CatchAttack(this);

	CBall::EAttack atkBall = pBall->GetTypeAtk();	// ボール攻撃種類
	MyLib::Vector3 posB = pBall->GetPosition();	// ボール位置
	MyLib::Vector3 pos = GetPosition();	// ボール位置
	
	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 入力判定
	bool bInput = false;
	EDashAngle* angle = m_pControlMove->GetInputAngle();
	if (angle != nullptr)
	{
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		float fRot = division * (*angle) + D3DX_PI + Camerarot.y;
		UtilFunc::Transformation::RotNormalize(fRot);
		bInput = UtilFunc::Collision::CollisionViewRange3D(pos, posB, fRot, JUST_VIEW);
	}

	// モーション設定
	if (m_sMotionFrag.bCatchJust
		&& bInput)
	{// ジャストキャッチ

		CatchSettingLandJust(atkBall);		// キャッチ時処理(地上・ジャスト)
	}
	else
	{// 通常キャッチ

		CatchSettingLandNormal(atkBall);		// キャッチ時処理(地上・通常)
	}

	// 受けた種類
	m_sDamageInfo.eReiceiveType = atkBall;
}

//==========================================================================
// コート越え処理
//==========================================================================
void CPlayer::OutCourtSetting()
{
	//ダメージ
	m_pStatus->LifeDamage(10);
	SetMotion(EMotion::MOTION_DAMAGE);

	// ノックバックの位置設定
	MyLib::Vector3 rot = GetRotation();
	UtilFunc::Transformation::RotNormalize(rot.y);
	MyLib::Vector3 posS = GetPosition();	//始点
	MyLib::Vector3 posE = posS;				//終点
	posE.x += sinf(rot.y) * Knockback::DEAD;
	posE.z += cosf(rot.y) * Knockback::DEAD;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	SetState(EState::STATE_OUTCOURT);
}

//==========================================================================
// 回避範囲取得
//==========================================================================
float CPlayer::GetDodgeDistance()
{
	return 0.0f;
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayer::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ダメージ受付時間更新
	UpdateDamageReciveTimer(fDeltaTime, fDeltaRate, fSlowRate);

	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ダメージ受付時間更新
//==========================================================================
void CPlayer::UpdateDamageReciveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_sDamageInfo.bReceived = false;

	// ダメージ受け付け時間減算
	m_sDamageInfo.fReceiveTime -= fDeltaTime;
	if (m_sDamageInfo.fReceiveTime <= 0.0f)
	{
		// ダメージ受け付け判定
		m_sDamageInfo.bReceived = true;
		m_sDamageInfo.fReceiveTime = 0.0f;
	}
}

//==========================================================================
// なし
//==========================================================================
void CPlayer::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==========================================================================
// 無敵状態
//==========================================================================
void CPlayer::StateInvincible()
{
	if (m_fStateTime >= StateTime::INVINCIBLE)
	{
		m_sDamageInfo.fReceiveTime = 0.0f;
		m_sDamageInfo.bReceived = true;

		SetState(STATE_NONE);
	}
}

//==========================================================================
// ダメージ状態
//==========================================================================
void CPlayer::StateDamage()
{
	MyLib::Vector3 pos = GetPosition();
	CMotion* pMotion = GetMotion();
	
	float time = m_fStateTime / StateTime::DAMAGE;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT,time);

	SetPosition(pos);

	if (m_fStateTime >= StateTime::DAMAGE)
	{
		pMotion->ToggleFinish(true);
		SetState(STATE_INVINCIBLE);
	}
}

//==========================================================================
// 死亡状態
//==========================================================================
void CPlayer::StateDead()
{
	MyLib::Vector3 pos = GetPosition();

	float time = m_fStateTime / StateTime::DEAD;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT, time);

	SetPosition(pos);

	//死亡状態をキャンセル不能にする
	SetEnableMove(false);
	//m_sMotionFrag.bDead = true;

	if (m_fStateTime >= StateTime::DEAD)
	{
		SetState(STATE_DEAD_AFTER);
		SetMotion(MOTION_DEAD_AFTER);
	}
}

//==========================================================================
// 死亡状態
//==========================================================================
void CPlayer::StateDeadAfter()
{
	//MyLib::Vector3 pos = GetPosition();

	//float time = m_fStateTime / StateTime::DEAD;
	//time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	//pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT, time);

	//SetPosition(pos);

	//死亡状態をキャンセル不能にする
	SetEnableMove(false);
	//m_sMotionFrag.bDead = true;
}

//==========================================================================
// 回避
//==========================================================================
void CPlayer::StateDodge()
{

}

//==========================================================================
// 通常キャッチ
//==========================================================================
void CPlayer::StateCatch_Normal()
{
	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// キャンセル可能フレーム取得
	CMotion::Info motionInfo = pMotion->GetInfo();
	float fCancelableTime = static_cast<float>(motionInfo.nCancelableFrame);

	// TODO : ずざざーとする
	MyLib::Vector3 rot = GetRotation();

	// 割合
	float ratio = m_fStateTime / StateTime::CATCH;
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.4f, 1.0f);
	ratio -= 1.0f;

	// 後退する
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	// 移動量更新
	move.x += sinf(D3DX_PI + rot.y) * (Catch::Impact[m_sDamageInfo.eReiceiveType] * ratio);
	move.z += cosf(D3DX_PI + rot.y) * (Catch::Impact[m_sDamageInfo.eReiceiveType] * ratio);

	// 位置更新
	pos.x += move.x;
	pos.z += move.z;
	SetPosition(pos);
	SetMove(move);

	//スペシャル時ライン越え判定
	if (m_sDamageInfo.eReiceiveType == CBall::EAttack::ATK_SPECIAL &&
		CGame::GetInstance()->GetGameManager()->SetPosLimit(pos) &&
		m_state != EState::STATE_OUTCOURT)
	{
		OutCourtSetting();
	}
		
	if (pMotion->IsGetCancelable())
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ジャストキャッチ
//==========================================================================
void CPlayer::StateCatch_Just()
{
	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->IsGetCancelable())
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// コート越え
//==========================================================================
void CPlayer::StateOutCourt()		
{
	MyLib::Vector3 pos = GetPosition();

	//ノックバック
	float time = m_fStateTime / StateTime::DAMAGE;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT_OUTCOURT, time);

	SetPosition(pos);

	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->IsGetCancelable())
	{// キャンセル可能
		SetState(EState::STATE_OUTCOURT_RETURN);
	}
}

//==========================================================================
// コート越えから戻る
//==========================================================================
void CPlayer::StateOutCourt_Return()
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posStart = CGame::GetInstance()->GetGameManager()->GetCourtSize();
	posStart.y = 0.0f;
	posStart.z = 0.0f;
	posStart.x *= 0.5f;

	//コート内に戻る
	pos = UtilFunc::Correction::EasingLinear(m_sKnockback.posEnd, posStart, 0.0f, StateTime::COURT_RETURN, m_fStateTime);
	SetPosition(pos);

	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (m_fStateTime >= StateTime::COURT_RETURN)
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// バッファへの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	// テスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z+ステンシル成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z+ステンシル失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテストのみ失敗

	// ステンシル描画
	CObjectChara::Draw();

	// ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// 普通の描画
	if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else
	{
		CObjectChara::Draw();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CPlayer::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}


//==========================================================================
// デバッグ処理
//==========================================================================
void CPlayer::Debug()
{
#if _DEBUG

	//-----------------------------
	// 位置
	//-----------------------------
	if (ImGui::TreeNode("SetPosition"))
	{
		// 位置取得
		MyLib::Vector3 pos = GetPosition();

		if (ImGui::Button("Reset"))
		{// リセット
			pos = MyLib::Vector3();
		}
		ImGui::DragFloat3("pos", (float*)&pos, 1.0f, 0.0f, 0.0f, "%.2f");

		// 位置設定
		SetPosition(pos);
		ImGui::TreePop();
	}

	//-----------------------------
	// パラメーター
	//-----------------------------
	if (ImGui::TreeNode("Parameter"))
	{
		// 取得
		CCharacterStatus* pStatus = GetCharStatus();
		CCharacterStatus::CharParameter parameter = pStatus->GetParameter();

		ImGui::DragFloat("fVelocityNormal", (float*)&parameter.fVelocityNormal, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fVelocityDash", (float*)&parameter.fVelocityDash, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fVelocityBlink", (float*)&parameter.fVelocityBlink, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fVelocityJump", (float*)&parameter.fVelocityJump, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fRadius", (float*)&parameter.fRadius, 0.5f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fJumpStartMove", &parameter.fJumpStartMove, 0.001f, 0.0f, 100.0f, "%.3f");
		ImGui::DragFloat("fJumpUpdateMove", &parameter.fJumpUpdateMove, 0.0001f, 0.0f, 100.0f, "%.3f");

		// 設定
		SetRadius(parameter.fRadius);

		pStatus->SetParameter(parameter);
		
		ImGui::TreePop();
	}

	//-----------------------------
	// 情報表示
	//-----------------------------
	if (ImGui::TreeNode("Transform Info"))
	{
		MyLib::Vector3 pos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		MyLib::Vector3 move = GetMove();
		CMotion* motion = GetMotion();

		ImGui::Text("pos : [X : %.2f, Y : %.2f, Z : %.2f]", pos.x, pos.y, pos.z);
		ImGui::Text("rot : [X : %.2f, Y : %.2f, Z : %.2f]", rot.x, rot.y, rot.z);
		ImGui::Text("move : [X : %.2f, Y : %.2f, Z : %.2f]", move.x, move.y, move.z);
		ImGui::Text("Life : [%d]", GetLife());
		ImGui::Text("State : [%d]", m_state);
		ImGui::Text("Action : [%d]", m_pActionPattern->GetAction());
		ImGui::Text("Motion : [%d]", motion->GetType());

		//現在の入力方向を取る(向き)
		bool bInput = false;
		EDashAngle* angle = m_pControlMove->GetInputAngle();
		if (angle != nullptr)
		{
			float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
			float fRot = division * *angle;
			ImGui::Text("fRot : [%.2f]", fRot);
		}

		ImGui::TreePop();
	}

	//-----------------------------
	// 体力
	//-----------------------------
	if (ImGui::TreeNode("Life"))
	{
		int nLife = GetLife();

		ImGui::DragInt("nLife", &nLife, 1, 0, 50, "%d");

		SetLife(nLife);

		ImGui::TreePop();
	}

	if (ImGui::Button("Dead"))
	{// リセット
		MyLib::HitResult_Character* result = DEBUG_NEW MyLib::HitResult_Character;
		CBall* pBall = CBall::Create(GetPosition());
		DeadSetting(result,pBall);
		delete result;
		pBall->Uninit();
	}

	//-----------------------------
	// コート外検証
	//-----------------------------
	if (ImGui::Button("OutCourt"))
	{// リセット
		OutCourtSetting();
	}

#endif
}

//==========================================================================
// 操作取得(アクション)
//==========================================================================
CPlayerControlAction* CPlayer::GetPlayerControlAction()
{
	return m_pControlAction;
}

//==========================================================================
// 操作取得(移動)
//==========================================================================
CPlayerControlMove* CPlayer::GetPlayerControlMove()
{
	return m_pControlMove;
}

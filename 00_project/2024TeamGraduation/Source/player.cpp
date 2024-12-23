//=============================================================================
// 
//  プレイヤー処理 [player.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "player.h"
#include "playerManager.h"
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
#include "charmValueManager.h"
#include "teamStatus.h"
#include "specialEffect.h"
#include "charmManager.h"
#include "playerMarker.h"
#include "catchSpecial.h"
#include "stretcher.h"

// 派生先
#include "playerDressup.h"
#include "playerSpawn.h"
#include "playerReferee.h"
#include "playerResult.h"
#include "playerAIIn.h"
#include "playerAIOut.h"
#include "playerUserIn.h"
#include "playerUserOut.h"
#include "playerNone.h"

// デバッグ
#include "ObjectLine.h"

// 使用クラス
#include "playerPosAdj_in.h"
#include "playerPosAdj_inLeft.h"
#include "playerPosAdj_inRight.h"
#include "playerAIPosAdj_inLeft.h"
#include "playerAIPosAdj_inRight.h"
#include "playerPosAdj_out.h"
#include "playerPosAdj_none.h"
#include "playerAction.h"
#include "playerStatus.h"
#include "playercontrol_action.h"
#include "playercontrol_move.h"

// ドレスアップ
#include "dressup_hair.h"
#include "dressup_accessory.h"
#include "dressup_face.h"

#if 0
#define WALKSE()	// 徒歩音再生
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE[CPlayer::EBody::BODY_MAX][CPlayer::EHandedness::HAND_MAX] =		// キャラクターファイル
	{ 
		{
			"data\\TEXT\\character\\player\\righthand\\setup_player.txt",
			"data\\TEXT\\character\\player\\lefthand\\setup_player.txt",
		},
		{
			"data\\TEXT\\character\\player\\righthand\\setup_player_fat.txt",
			"data\\TEXT\\character\\player\\lefthand\\setup_player_fat.txt",
		},
		{
			"data\\TEXT\\character\\player\\righthand\\setup_player_gari.txt",
			"data\\TEXT\\character\\player\\lefthand\\setup_player_gari.txt",
		}
	};

	const MyLib::Vector3 LOOK_OFFSET = MyLib::Vector3(0.0f, 60.0f, 0.0f);	// 通常投げで狙う位置のオフセット
	const float SHADOW_RADIUS = 50.0f;		// 影の半径
	const float	SHADOW_MIN_ALPHA = 0.18f;	// 影の透明度
	const float	SHADOW_MAX_ALPHA = 0.48f;	// 影の透明度
	const float DODGE_RADIUS = 300.0f;		// 回避範囲
	const float JUST_VIEW = 90.0f;			// ジャストキャッチ時の方向ゆとり(左右1/8π)
}

namespace Knockback
{
	const float HEIGHT = 50.0f;					// 最大高度
	const float HEIGHT_OUTCOURT = 50.0f;		// 最大高度(コート越え)
	const float DEAD = 1.5f;					// 死亡(通常への倍率)
}

// TODO：ボールステータスに移行
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
	const float INVINCIBLE = 3.0f;	// 無敵
	const float CATCH = 0.5f;		// キャッチ
	const float COURT_RETURN = 2.0f;		// コートに戻ってくる
	const float INVADE_TOSS = 0.3f;		// 侵入後トス
}

namespace Align	// 足揃え
{
	const float MULTIPLY_MOVE = 0.25f;	// 移動倍率
	const float MULTIPLY_MOTION = 1.5f;	// モーション倍率
}

namespace EffectOffset
{
	const MyLib::Vector3 CATCH_STANCE = MyLib::Vector3(80.0f, 0.0f, 80.0f);	// キャッチ(構え)
	const MyLib::Vector3 CATCH_NORMAL = MyLib::Vector3(50.0f, 0.0f, 50.0f);	// キャッチ(通常)
}

namespace Court	// 移動制限
{
	const float VELOCITY_INVADE = 2.0f;	// 戻る速度
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
	&CPlayer::StateDeadCarry,			// 死後運搬
	&CPlayer::StateDodge,				// 回避
	&CPlayer::StateCatch_Normal,		// 通常キャッチ
	&CPlayer::StateCatch_Just,			// ジャストキャッチ
	&CPlayer::StateCatch_Special,		// スペシャルキャッチ
	&CPlayer::StateSpecial,				// スペシャル
	&CPlayer::StateOutCourt,			// コート越え
	&CPlayer::StateOutCourt_Return,		// コートに戻る
	&CPlayer::StateInvade_Toss,			// 相手コートに侵入トス
	&CPlayer::StateInvade_Return,		// 相手コート侵入から戻る
};

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayer::STATE_END_FUNC CPlayer::m_StateEndFunc[] =	// 状態終了関数
{
	nullptr,							// なし
	&CPlayer::StateEndInvincible,		// 無敵
	&CPlayer::StateEndDamage,			// ダメージ
	&CPlayer::StateEndDead,				// 死亡
	&CPlayer::StateEndDeadAfter,		// 死亡後
	nullptr,							// 死後運搬
	nullptr,							// 回避
	nullptr,							// 通常キャッチ
	nullptr,							// ジャストキャッチ
	nullptr,							// スペシャルキャッチ
	nullptr,							// スペシャル
	nullptr,							// コート越え
	nullptr,							// コートに戻る
	nullptr,							// 相手コートに侵入トス
	&CPlayer::StateEndInvade_Return,	// 相手コート侵入から戻る
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CPlayer> CPlayer::m_List = {};	// リスト
#if _DEBUG	// デバッグ用ID
int CPlayer::m_nDebugID = 0;	// デバッグ用ID
#endif

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayer::CPlayer(const CGameManager::ETeamSide typeTeam, const EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) : CObjectChara(nPriority),
	m_typeTeam	(typeTeam),	// チームサイド
	m_typeArea	(typeArea)	// ポジション
{
	// 値のクリア
	m_state = EState::STATE_NONE;		// 状態
	m_Oldstate = EState::STATE_NONE;	// 前回の状態
	m_fStateTime = 0.0f;				// 状態時間

	// オブジェクトのパラメータ
	m_mMatcol = MyLib::Color();			// マテリアルの色
	m_sKnockback = SKnockbackInfo();	// ノックバック情報

	// 行動フラグ
	m_bPossibleMove = false;		// 行動可能フラグ
	m_bPossibleAction = false;		// 移動可能フラグ
	m_bAutoMotionSet = false;		// オートモーション設定
	m_bJump = false;				// ジャンプ中かどうか
	m_bDash = false;				// ダッシュ判定
	m_bFootLR = false;				// 足左右判定
	m_bAlign = false;				// 揃え
	m_sMotionFrag = SMotionFrag();	// モーションのフラグ

	// パターン用インスタンス
	m_pActionPattern = nullptr;	// アクションパターン
	m_pPosAdj = nullptr;		// プレイヤー位置補正
	m_pStatus = nullptr;		// ステータス
	m_pBase = nullptr;			// ベース

	// スぺシャルエフェクト
	m_pSpecialEffect = nullptr;	// スぺシャルエフェクト

	// キャッチスぺシャル
	m_pCatchSpecial = nullptr;	// キャッチスぺシャル
	
	// エフェクト用
	m_pEfkCatchStance = nullptr;	// キャッチの構え用
	m_pEfkCatchNormal = nullptr;	// 通常キャッチ
	m_pEfkCatchJust = nullptr;		// ジャストキャッチ

	// ドレスアップ
	m_pDressUp_Hair = nullptr;		// ドレスアップ(髪)
	m_pDressUp_Accessory = nullptr;	// ドレスアップ(アクセ)
	m_pDressUp_Face = nullptr;		// ドレスアップ(顔)

	// その他
	m_fEscapeTime = 0.0f;	// ボール所持タイマー
	m_fHaveTime = 0.0f;		// ボール所持タイマー
	m_nMyPlayerIdx = -1;	// プレイヤーインデックス番号
	m_nPosIdx = -1;			// ポジション別インデックス
	m_pMarker = nullptr;	// マーカーの情報
	m_pShadow = nullptr;	// 影の情報
	m_pBall = nullptr;		// ボールの情報
	m_sDamageInfo = SDamageInfo();		// ダメージ情報
	m_Handedness = EHandedness::HAND_R;	// 利き手
	m_BodyType = EBody::BODY_NORMAL;	// 体型


#if _DEBUG	// デバッグ用ID
	m_nThisDebugID = m_nDebugID;
	m_nDebugID++;
#endif // _DEBUG

	// ベースタイプを初期化
	InitBase(typeBase);
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayer::~CPlayer()
{

}

//==========================================================================
// 生成処理 (ゲームプレイヤー)
//==========================================================================
CPlayer* CPlayer::Create
(
	const MyLib::Vector3&	rPos,		// 位置
	CGameManager::ETeamSide	typeTeam,	// チームサイド
	EFieldArea	typeArea,				// ポジション
	EBaseType	typeBase,				// ベースタイプ
	EBody		typeBody,				// 体型
	EHandedness	typeHand				// 利き手
)
{
	// メモリの確保
	CPlayer* pPlayer = DEBUG_NEW CPlayer(typeTeam, typeArea, typeBase);
	if (pPlayer != nullptr)
	{
		// 体型を設定
		pPlayer->m_BodyType = typeBody;

		// 利き手を設定
		pPlayer->m_Handedness = typeHand;

		// クラスの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pPlayer);
			return nullptr;
		}

		// 初期位置を設定
		pPlayer->GetBase()->InitPosition(rPos);
	}

	return pPlayer;
}

//==========================================================================
// 生成処理 (仮想プレイヤー)
//==========================================================================
CPlayer* CPlayer::Create
(
	const MyLib::Vector3& rPos,			// 位置
	CGameManager::ETeamSide typeTeam,	// チームサイド
	EHuman typeHuman,					// 人
	EBody typeBody,						// 体型
	EHandedness typeHand,				// 利き手
	EFieldArea	typeArea				// ポジション
)
{
	// メモリの確保
	CPlayer* pPlayer = nullptr;
	switch (typeHuman)
	{ // 人ごとの処理
	case EHuman::HUMAN_ENTRY:
		pPlayer = DEBUG_NEW CPlayerDressUP(typeTeam, EFieldArea::FIELD_NONE, EBaseType::TYPE_USER);
		break;

	case EHuman::HUMAN_SPAWN:
		pPlayer = DEBUG_NEW CPlayerSpawn(typeTeam, EFieldArea::FIELD_NONE, EBaseType::TYPE_USER);
		break;

	case EHuman::HUMAN_REFEREE:
		pPlayer = DEBUG_NEW CPlayerReferee;
		break;

	case EHuman::HUMAN_RESULT:
		pPlayer = DEBUG_NEW CPlayerResult(typeTeam, typeArea);
		break;

	default:
		assert(false);
		break;
	}

	if (pPlayer != nullptr)
	{
		// 体型を設定
		pPlayer->m_BodyType = typeBody;

		// 利き手を設定
		pPlayer->m_Handedness = typeHand;

		// クラスの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pPlayer);
			return nullptr;
		}

		// 初期位置を設定
		pPlayer->GetBase()->InitPosition(rPos);
	}

	return pPlayer;
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

	m_state = EState::STATE_NONE;	// 状態
	m_Oldstate = m_state;
	m_sMotionFrag.bMove = true;
	m_bPossibleMove = true;

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE[m_BodyType][m_Handedness]);
	if (FAILED(hr))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	if (m_typeArea != FIELD_NONE)
	{ // ゲームプレイヤーじゃない場合はもろもろの生成しないように

		// 影の生成
		if (FAILED(CreateShadow()))
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// マーカーの情報
		m_pMarker = CPlayerMarker::Create(this);
		if (m_pMarker == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// アクションパターン
		if (m_pActionPattern == nullptr)
		{
			m_pActionPattern = DEBUG_NEW CPlayerAction(this);
		}

		// ステータス
		if (m_pStatus == nullptr)
		{
			m_pStatus = DEBUG_NEW CPlayerStatus(this);
			m_pStatus->Init();
		}

		// スぺシャルエフェクト
		if (m_pSpecialEffect == nullptr)
		{
			m_pSpecialEffect = CSpecialEffect::Create(this, CSpecialEffect::EType::TYPE_KAMEHAMEHA);
		}

		if (GetBaseType() == EBaseType::TYPE_USER)
		{ // ユーザーベースの場合

			// プレイヤーインデックス番号を設定
			m_nMyPlayerIdx = GetNumUser();
		}
		else
		{ // AIベースの場合

			// プレイヤーインデックス番号を初期化
			m_nMyPlayerIdx = -1;
		}

		if (m_typeArea != EFieldArea::FIELD_NONE)
		{ // ポジションの指定がある場合

			// プレイヤーマネージャーに割当
			CPlayerManager* pManager = CPlayerManager::GetInstance();				// プレイヤーマネージャー
			if (pManager != nullptr) { m_nPosIdx = pManager->RegistPlayer(this); }	// マネージャーがある場合登録
		}
	}

	// ドレスアップ生成
	CreateDressUp();

	// プレイヤーリストに割当
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Uninit()
{
	// 影
	m_pShadow = nullptr;
	
	// ボール
	m_pBall = nullptr;

	// アクションパターン
	SAFE_DELETE(m_pActionPattern);

	// ベース
	SAFE_DELETE(m_pBase);

	// ステータス
	SAFE_DELETE(m_pStatus);

	// マーカー
	SAFE_UNINIT(m_pMarker);

	// プレイヤー位置補正
	SAFE_DELETE(m_pPosAdj);

	// スぺシャルエフェクト
	SAFE_DELETE(m_pSpecialEffect);

	// スペシャルキャッチ状態
	SAFE_UNINIT(m_pCatchSpecial);

	// ドレスアップ
	DeleteDressUp();

	// 終了処理
	CObjectChara::Uninit();

	if (m_typeArea != EFieldArea::FIELD_NONE)
	{ // ポジションの指定がある場合

		// プレイヤーマネージャーから削除
		CPlayerManager* pManager = CPlayerManager::GetInstance();	// プレイヤーマネージャー
		if (pManager != nullptr) { pManager->DeletePlayer(this); }	// マネージャーがある場合削除
	}

	// プレイヤーリストから削除
	m_List.Delete(this);
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CPlayer::Kill()
{
	// 影の削除
	SAFE_KILL(m_pShadow);

	// アクションパターン
	if (m_pActionPattern != nullptr)
	{
		delete m_pActionPattern;
		m_pActionPattern = nullptr;
	}

	// ステータス
	if (m_pStatus != nullptr)
	{
		m_pStatus->Kill();
		SAFE_DELETE(m_pStatus);
		m_pStatus = nullptr;
	}

	// マーカー
	if (m_pMarker != nullptr)
	{
		m_pMarker->Kill();
		m_pMarker = nullptr;
	}

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsDeath()) return;

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// フラグリセット
	ResetFrag();

	// 親の更新処理
	float nowSlowRate = fSlowRate;
	if (m_bAlign) nowSlowRate *= Align::MULTIPLY_MOTION;
	CObjectChara::Update(fDeltaTime, fDeltaRate, nowSlowRate);

	// 操作
	Controll(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// モーションの設定処理
	if (CGameManager::GetInstance()->IsControll())
	{
		MotionSet(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// モーション別更新処理
	UpdateByMotion(fDeltaTime, fDeltaRate, fSlowRate);

	// アクション更新
	if (m_pActionPattern != nullptr)
	{
		m_pActionPattern->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 位置の補正
	if (m_pPosAdj != nullptr)
	{
		m_pPosAdj->UpdateAdjuster(this);
	}

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 非モテまとめ
	CCharmManager::GetInstance()->UnCharm(this, fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG	// デバッグ処理

	// Xの向き
	ImGui::PushID(m_nThisDebugID); // ウィジェットごとに異なるIDを割り当てる
	{
		std::string treename = "Player" + std::to_string(m_nMyPlayerIdx);	// ツリー名
		if (ImGui::TreeNode(treename.c_str()))
		{
			Debug();
			ImGui::TreePop();
		}
	}
	ImGui::PopID();

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

	if (CGameManager::GetInstance()->IsControll())
	{ // 行動できるとき

		// ベースの更新
		m_pBase->Update(fDeltaTime, fDeltaRate, fSlowRate);

		// ベース変更の更新
		m_pBase->UpdateChangeBase();
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
	rot.y += fRotDiff * (0.35f * fDeltaRate * fSlowRate);
	UtilFunc::Transformation::RotNormalize(rot.y);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::SetMotion(int motionIdx, int startKey, bool bBlend) const
{
	//TAKADA: はじく条件(死んだら)
	//if (m_sMotionFrag.bDead) return;

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx, startKey, bBlend);
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::MotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_bAlign = false;	// 揃え

	// 足左右の更新
	UpdateFootLR();

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 現在の種類取得
	int nType = pMotion->GetType();
	int nOldType = pMotion->GetOldType();

	// オートモーション設定
	if (!m_bAutoMotionSet) return;

	// 移動できないと通さない
	if (!m_bPossibleMove) return;

	// 死亡時通さない
	if (m_sMotionFrag.bDead) return;

	// おっとっと中
	if (GetActionPattern()->GetAction() == EAction::ACTION_UNSTABLE) return;

	// 再生中
	if (!pMotion->IsFinish()) return;

	if (m_sMotionFrag.bMove)
	{// 移動していたら

		// 移動モーション設定
		SetMoveMotion(false);

		if (nOldType != pMotion->GetType())
		{
			//m_bFootLR = true;
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
		// デフォルトモーションの設定
		DefaultMotionSet(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// 移動モーション設定
//==========================================================================
void CPlayer::SetMoveMotion(bool bNowDrop)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 再生中モーション
	int nType = pMotion->GetType();

	// モーションの種類
	EMotion motionType;
	if (m_pBall == nullptr) // ボール所持によって分ける
	{
		motionType = m_bDash ? MOTION_RUN : MOTION_WALK;
	}
	else
	{
		motionType = m_bDash ? MOTION_RUN_BALL : MOTION_WALK_BALL;
	}

	// 歩行の情報取得
	CMotion::Info info = pMotion->GetInfo(motionType);

	// 開始キー
	int nStartKey = 0;
	if (m_bFootLR)
	{
		nStartKey = info.nNumKey / 2;
	}

	// モーション設定
	if (!m_bDash && m_pBase->IsCrab() && (motionType == MOTION_WALK || motionType == MOTION_WALK_BALL))
	{// カニ歩き
		GetBase()->MotionCrab(nStartKey);
	}
	else if(bNowDrop || nType != EMotion::MOTION_DROPCATCH_WALK)
	{
		SetMotion(motionType, nStartKey);
	}
}

//==========================================================================
// デフォルトモーションの設定
//==========================================================================
void CPlayer::DefaultMotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 情報取得
	CMotion::Info info = pMotion->GetInfo();
	int nType = pMotion->GetType();

	// 状況別設定
	if (nType != EMotion::MOTION_WALK)
	{
		// ニュートラルモーション設定
		if (m_pBall != nullptr)	{ SetMotion(MOTION_DEF_BALL); }
		else					{ SetMotion(MOTION_DEF); }
	}
	else if (nType == EMotion::MOTION_WALK)
	{// ニュートラルモーション設定
		
		if (!pMotion->IsAlignFrame(info))
		{// 足が揃ってない

			// 移動量取得
			MyLib::Vector3 move = GetMove();

			// 移動する
			float fMove = GetParameter().fVelocityNormal;
			switch (nType)
			{
			case EMotion::MOTION_WALK:
				fMove = GetParameter().fVelocityNormal;
				break;

			case EMotion::MOTION_RUN:
				fMove = GetParameter().fVelocityDash;
				break;

			default:
				break;
			}

			// 補正倍率
			fMove *= fDeltaRate;
			fMove *= fSlowRate;
			fMove *= Align::MULTIPLY_MOVE;		// 揃えるからゆっくり

			// 移動量更新
			MyLib::Vector3 rot = GetRotation();
			move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * fMove;
			move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * fMove;

			// 移動量設定
			SetMove(move);
			m_bAlign = true;	// 揃え
		 }
		else
		{
			// ニュートラルモーション設定
			if (m_pBall != nullptr) { SetMotion(MOTION_DEF_BALL); }
			else					{ SetMotion(MOTION_DEF); }
		}
	}
}

//==========================================================================
// 足左右の更新
//==========================================================================
void CPlayer::UpdateFootLR()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 情報取得
	CMotion::Info info = pMotion->GetInfo();
	int nType = pMotion->GetType();

	// 歩き以外は抜ける
	if (nType != EMotion::MOTION_DROPCATCH_WALK) return;

	if (pMotion->IsImpactFrame(info))
	{// 衝撃のフレーム

		if (nType == EMotion::MOTION_DROPCATCH_WALK)
		{// 落ちてるのを拾うとき

			// 移動モーション設定
			SetMoveMotion(true);
		}

		// 足左右切り替え : 落ちてるの拾うためにいるかも
		//m_bFootLR = !m_bFootLR;
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

	// 移動判定OFF
	m_sMotionFrag.bMove = false;

	// ダッシュリセット
	//m_bDash = false;

	//キャッチできない状態
	m_sMotionFrag.bCatch = false;
	m_sMotionFrag.bCatchJust = false;

	// オートモーション設定
	m_bAutoMotionSet = true;
}

//==========================================================================
// ユーザーベースのプレイヤー総数取得処理
//==========================================================================
int CPlayer::GetNumUser()
{
	std::list<CPlayer*>::iterator itr = m_List.GetEnd();	// 最後尾イテレーター
	int nNumBase = 0;	// ベースがユーザーのプレイヤー数

	while (m_List.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itr);	// プレイヤー情報

		// ユーザーベースの場合プレイヤー数加算
		if (pItrPlayer->GetBaseType() == EBaseType::TYPE_USER) { nNumBase++; }
	}

	// ベースがユーザーのプレイヤー総数を返す
	return nNumBase;
}

//==========================================================================
// モーション別更新処理
//==========================================================================
void CPlayer::UpdateByMotion(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	switch (nType)
	{
	case EMotion::MOTION_SPECIAL:	// スペシャル

		// 更新処理
		m_pSpecialEffect->Update(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	default:
		break;
	}

	//-----------------------------
	// キャッチ
	//-----------------------------
	if (nType != EMotion::MOTION_CATCH_STANCE &&
		nType != EMotion::MOTION_CATCH_STANCE_JUMP)
	{
		// エフェクト破棄
		if (m_pEfkCatchStance != nullptr)
		{
			m_pEfkCatchStance->SetTrigger(0);
			m_pEfkCatchStance = nullptr;
		}
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
	case EMotion::MOTION_THROW_DROP:
	case EMotion::MOTION_THROW_JUST:

		if (m_pBall != nullptr)
		{// 通常投げ
			m_pBall->ThrowNormal(this);
		}
		break;

	case EMotion::MOTION_THROW_JUMP:
	case EMotion::MOTION_THROW_JUST_JUMP:

		if (m_pBall != nullptr)
		{// ジャンプ投げ
			m_pBall->ThrowJump(this);
		}
		break;

	case EMotion::MOTION_TOSS:

		if (m_pBall != nullptr)
		{// トス
			m_pBall->Toss(this);
		}
		break;

	case EMotion::MOTION_THROW_PASS:
		if (m_pBall != nullptr)
		{// パス
			m_pBall->Pass(this);
		}
		break;

	case EMotion::MOTION_CATCH_STANCE:
	case EMotion::MOTION_CATCH_STANCE_JUMP:
	{
		// エフェクトの位置
		MyLib::Vector3 setpos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		rot.x = 0.0f, rot.z = 0.0f;

		setpos.x += sinf(D3DX_PI + rot.y) * EffectOffset::CATCH_STANCE.x;
		setpos.z += cosf(D3DX_PI + rot.y) * EffectOffset::CATCH_STANCE.z;
		setpos.y = GetCenterPosition().y;

		// エフェクト生成
		m_pEfkCatchStance = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CATCH_STANCE,
			setpos,
			rot,	// 向き
			MyLib::Vector3(),
			15.0f, true);

		if (m_pEfkCatchStance != nullptr)
		{// 位置・向き更新
			m_pEfkCatchStance->SetRotation(rot);
			m_pEfkCatchStance->SetPosition(setpos);
		}
	}
		break;

	case EMotion::MOTION_CATCH_NORMAL:
	{
		// エフェクトの位置
		MyLib::Vector3 setpos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		rot.x = 0.0f, rot.z = 0.0f;

		setpos.x += sinf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.x;
		setpos.z += cosf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.z;
		setpos.y = GetCenterPosition().y;

		// エフェクト生成
		m_pEfkCatchNormal = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CATCH_NORMAL,
			setpos,
			rot,	// 向き
			MyLib::Vector3(),
			15.0f, true);

		if (m_pEfkCatchNormal != nullptr)
		{// 位置・向き更新
			m_pEfkCatchNormal->SetRotation(rot);
			m_pEfkCatchNormal->SetPosition(setpos);
		}
	}
	break;

	case EMotion::MOTION_JUSTCATCH_NORMAL:
	case EMotion::MOTION_JUSTCATCH_JUMP:
	{
		// エフェクトの位置
		MyLib::Vector3 setpos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		rot.x = 0.0f, rot.z = 0.0f;

		setpos.x += sinf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.x;
		setpos.z += cosf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.z;
		setpos.y = GetCenterPosition().y;

		// エフェクト生成
		m_pEfkCatchJust = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CATCH_JUST,
			setpos,
			rot,	// 向き
			MyLib::Vector3(),
			20.0f, true);

		if (m_pEfkCatchJust != nullptr)
		{// 位置・向き更新
			m_pEfkCatchJust->SetRotation(rot);
			m_pEfkCatchJust->SetPosition(setpos);
		}
	}
	break;

	case EMotion::MOTION_WALK:
	case EMotion::MOTION_WALK_BALL:
	{
#ifdef WALKSE
		PLAY_SOUND(CSound::ELabel::LABEL_SE_WALK);
#endif // WALKSE

		// 設定位置計算
		MyLib::Vector3 setpos = weponpos;	// セットする位置
		float rotDest = GetRotDest();		// 目標の向き

		// 少し前に出す
		setpos.x += sinf(D3DX_PI + rotDest) * 20.0f;
		setpos.z += cosf(D3DX_PI + rotDest) * 20.0f;

		// 歩きのエフェクト
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_WALK,
			setpos,
			MyLib::Vector3(),	// 向き
			MyLib::Vector3(),
			15.0f, true);
	}
		break;

	case EMotion::MOTION_RUN:
	case EMotion::MOTION_RUN_BALL:
	{
#ifdef WALKSE
		// 設定するラベル
		CSound::ELabel label = static_cast<CSound::ELabel>(static_cast<int>(CSound::ELabel::LABEL_SE_RUN01) + nCntATK);
		PLAY_SOUND(label);
#endif // WALKSE


		// 設定位置計算
		MyLib::Vector3 setpos = weponpos;	// セットする位置
		float rotDest = GetRotDest();		// 目標の向き

		// 少し前に出す
		setpos.x += sinf(D3DX_PI + rotDest) * 40.0f;
		setpos.z += cosf(D3DX_PI + rotDest) * 40.0f;

		// 歩きのエフェクト
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_RUN,
			setpos,
			MyLib::Vector3(),	// 向き
			MyLib::Vector3(),
			10.0f, true);
	}
		break;

	case EMotion::MOTION_BLINK:	// ブリンク
	{
		// 設定位置計算
		MyLib::Vector3 setpos = weponpos;	// セットする位置
		float rotDest = GetRotDest();		// 目標の向き

		// 少し前に出す
		setpos.x += sinf(D3DX_PI + rotDest) * 120.0f;
		setpos.z += cosf(D3DX_PI + rotDest) * 120.0f;

		// ブリンクのエフェクト
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_BLINK,
			setpos,
			MyLib::Vector3(0.0f, 0.0f, -GetRotDest()),	// 向き
			MyLib::Vector3(),
			20.0f, true);
	}
		break;

	case EMotion::MOTION_GRIP_FRONT:
		PLAY_SOUND(CSound::ELabel::LABEL_SE_GRIP01);
		break;

	case EMotion::MOTION_SPECIAL:

		// トリガー処理
		m_pSpecialEffect->TriggerMoment(ATKInfo, nCntATK);
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
	case EMotion::MOTION_CATCH_STANCE_JUMP:

		// キャッチフラグON
		m_sMotionFrag.bCatch = true;

		if (nCntATK == 1)
		{//2個目(1)がジャスト

			//ジャストフラグON
			m_sMotionFrag.bCatchJust = true;
		}

		{
			// エフェクトの位置
			MyLib::Vector3 setpos = GetPosition();
			MyLib::Vector3 rot = GetRotation();
			rot.x = 0.0f, rot.z = 0.0f;

			setpos.x += sinf(D3DX_PI + rot.y) * EffectOffset::CATCH_STANCE.x;
			setpos.z += cosf(D3DX_PI + rot.y) * EffectOffset::CATCH_STANCE.z;
			setpos.y = GetCenterPosition().y;

			if (m_pEfkCatchStance != nullptr)
			{// 位置・向き更新
				m_pEfkCatchStance->SetRotation(rot);
				m_pEfkCatchStance->SetPosition(setpos);
			}
		}
		break;

	case EMotion::MOTION_CATCH_NORMAL:
	{
		// エフェクトの位置
		MyLib::Vector3 setpos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		rot.x = 0.0f, rot.z = 0.0f;

		setpos.x += sinf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.x;
		setpos.z += cosf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.z;
		setpos.y = GetCenterPosition().y;

		if (m_pEfkCatchNormal != nullptr)
		{// 位置・向き更新
			m_pEfkCatchNormal->SetRotation(rot);
			m_pEfkCatchNormal->SetPosition(setpos);
		}
	}
		break;

	case EMotion::MOTION_JUSTCATCH_NORMAL:
	case EMotion::MOTION_JUSTCATCH_JUMP:
	{
		// エフェクトの位置
		MyLib::Vector3 setpos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		rot.x = 0.0f, rot.z = 0.0f;

		setpos.x += sinf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.x;
		setpos.z += cosf(D3DX_PI + rot.y) * EffectOffset::CATCH_NORMAL.z;
		setpos.y = GetCenterPosition().y;

		if (m_pEfkCatchJust != nullptr)
		{// 位置・向き更新
			m_pEfkCatchJust->SetRotation(rot);
			m_pEfkCatchJust->SetPosition(setpos);
		}
	}
	break;

	case EMotion::MOTION_SPECIAL:

		// 進行中処理
		m_pSpecialEffect->ProgressMoment(ATKInfo, nCntATK);
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

	default:
		break;
	}

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_CATCH);

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_PUSH);

	// ゲームマネージャ取得
	CGameManager* pGameMgr = CGameManager::GetInstance();

	// モテ加算
	pGameMgr->AddCharmValue(m_typeTeam, CCharmValueManager::ETypeAdd::ADD_CATCH);

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

	default:
		break;
	}

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_JUSTCATCH);

	// ジャストキャッチ状態
	SetState(EState::STATE_CATCH_JUST);

	// ゲームマネージャ取得
	CGameManager* pGameMgr = CGameManager::GetInstance();
	
	// モテ加算
	pGameMgr->AddCharmValue(m_typeTeam, CCharmValueManager::ETypeAdd::ADD_JUSTCATCH);
	
	// スペシャル加算
	pGameMgr->AddSpecialValue(m_typeTeam, CSpecialValueManager::ETypeAdd::ADD_JUSTCATCH);
}

//==========================================================================
// キャッチ時処理(スペシャル)
//==========================================================================
void CPlayer::CatchSettingSpecial(const bool& bJust, const CBall::ESpecial& typeSpecial)
{
	// キャッチスペシャル生成＆設定
	CCatchSpecial::EState state = CCatchSpecial::Check(this, bJust, typeSpecial);

	// 生成
	if (m_pCatchSpecial == nullptr)
	{// NULLだったら
		m_pCatchSpecial = CCatchSpecial::Create(this, state);
	}
	else
	{
		m_pCatchSpecial->SetState(state);
		m_pCatchSpecial->SetPlayer(this);
	}

	// ジャストキャッチ状態
	SetState(EState::STATE_CATCH_SPECIAL);
}

//==========================================================================
// ヒット処理
//==========================================================================
CPlayer::SHitInfo CPlayer::Hit(CBall* pBall)
{
	// ベースのヒット処理
	return m_pBase->Hit(pBall);
}

//==========================================================================
// スペシャル攻撃の設定
//==========================================================================
void CPlayer::SetSpecialAttack()
{
	bool bInverse = (m_typeTeam == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// カメラモーションの反転フラグ
	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報

	// かめはめ波モーションを設定
	pCameraMotion->SetMotion(CCameraMotion::MOTION_KAMEHAMEHA, bInverse);	// TODO：スペシャルごとに変更

	// スペシャル状態にする
	SetState(STATE_SPECIAL);

	// スペシャルモーションにする
	SetMotion(CPlayer::MOTION_SPECIAL);	// TODO：スペシャルごとに変更
}

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
	float knockback = pBall->GetKnockback();
	MyLib::Vector3 posS = GetPosition();
	MyLib::Vector3 posE = posS;
	posE.x += vecBall.x * knockback * Knockback::DEAD;
	posE.z += vecBall.z * knockback * Knockback::DEAD;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	// 死んだ
	result->isdeath = true;

	// 近くのAIに操作権を移し、自身をAIにする
	CPlayerManager::GetInstance()->ChangeUserToAI(this);
}

//==========================================================================
// ダメージ発生時設定
//==========================================================================
void CPlayer::DamageSetting(CBall* pBall)
{
	// ダメージ状態にする
	SetState(EState::STATE_DMG);

	// ノックバックの位置設定
	MyLib::Vector3 vecBall = pBall->GetMove().Normal();
	float knockback = pBall->GetKnockback();
	MyLib::Vector3 posS = GetPosition();	//始点
	MyLib::Vector3 posE = posS;				//終点
	posE.x += vecBall.x * knockback;
	posE.z += vecBall.z * knockback;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	// ダメージ受付時間を設定
	m_sDamageInfo.fReceiveTime = StateTime::DAMAGE;

	// ボール攻撃種類
	CBall::EAttack atkBall = pBall->GetTypeAtk();

	// サウンド再生
	switch (atkBall)
	{
	case CBall::ATK_NORMAL:
		PLAY_SOUND(CSound::ELabel::LABEL_SE_HIT_NORMAL);
		break;

	case CBall::ATK_JUMP:
		PLAY_SOUND(CSound::ELabel::LABEL_SE_HIT_JUMP);
		break;

	case CBall::ATK_SPECIAL:
		PLAY_SOUND(CSound::ELabel::LABEL_SE_HIT_JUMP);
		break;

	default:
		break;
	}


	// 向き取得
	float rotDest = GetRotDest();

	//--------------------------------
	// プレイヤー方向
	//--------------------------------
	CPlayer::CRAB_DIRECTION hitDir = CPlayer::CRAB_DIRECTION::CRAB_NONE;
	float fRotY = D3DX_PI * 1.0f + GetPosition().AngleXZ(pBall->GetPosition()) + rotDest;
	UtilFunc::Transformation::RotNormalize(fRotY);

	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);
	if (!UtilFunc::Collision::CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// 下向き
		hitDir = CPlayer::CRAB_DIRECTION::CRAB_DOWN;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// 上向き
		hitDir = CPlayer::CRAB_DIRECTION::CRAB_UP;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// 左向き
		hitDir = CPlayer::CRAB_DIRECTION::CRAB_LEFT;
	}
	else if (UtilFunc::Collision::CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
	{// 右向き
		hitDir = CPlayer::CRAB_DIRECTION::CRAB_RIGHT;
	}
	else
	{// 抜けちゃった
		MyAssert::CustomAssert(false, "カニ歩き：どこ見てんねん");
	}


	switch (hitDir)
	{
	case CPlayer::CRAB_UP:
		SetMotion(EMotion::MOTION_DAMAGE);
		break;

	case CPlayer::CRAB_DOWN:
		SetMotion(EMotion::MOTION_DAMAGE_BACK);
		break;

	case CPlayer::CRAB_LEFT:
		SetMotion(EMotion::MOTION_DAMAGE_LEFT);
		break;

	case CPlayer::CRAB_RIGHT:
		SetMotion(EMotion::MOTION_DAMAGE_RIGHT);
		break;

	default:
		MyAssert::CustomAssert(false, "被弾：どこ見てんねん");
		break;
	}
}

//==========================================================================
// キャッチ時処理
//==========================================================================
void CPlayer::CatchSetting(CBall* pBall)
{
	// ボールをキャッチ
	pBall->CatchAttack(this);

	CBall::EAttack atkBall = pBall->GetTypeAtk();	// ボール攻撃種類
	MyLib::Vector3 posB = pBall->GetPosition();		// ボール位置
	MyLib::Vector3 pos = GetPosition();				// プレイヤー位置
	
	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 入力判定
	bool bInput = false;
	EDashAngle* angle = m_pBase->GetPlayerControlMove()->GetInputAngle();
	if (angle != nullptr)
	{// ジャスト範囲方向に入力していたら
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		float fRot = division * (*angle) + D3DX_PI + Camerarot.y;
		UtilFunc::Transformation::RotNormalize(fRot);
		bInput = UtilFunc::Collision::CollisionViewRange3D(pos, posB, fRot, JUST_VIEW);
	}

	// ジャスト判定
	bool bJust = false;

#if 0	// 常にジャスト
	if (m_sMotionFrag.bCatchJust && bInput)
#else
	if (m_sMotionFrag.bCatchJust && bInput)
#endif
	{
		bJust = true;

		// 投げの猶予設定
		GetBase()->GetPlayerControlAction()->SetThrowJust();
	}

	// スペシャル専用
	if (atkBall == CBall::ATK_SPECIAL)
	{
		CatchSettingSpecial(bJust, pBall->GetTypeSpecial());		// キャッチ時処理(スペシャル)
		return;
	}

	// モーション設定
	if (bJust)
	{// ジャストキャッチ

		CatchSettingLandJust(atkBall);		// キャッチ時処理(地上・ジャスト)
	}
	else
	{// 通常キャッチ

		CatchSettingLandNormal(atkBall);	// キャッチ時処理(地上・通常)
	}

	// ジャンプ専用
	if (atkBall == CBall::ATK_SPECIAL)
	{
		GetActionPattern()->SetAction(EAction::ACTION_CATCH_JUMP);
	}

	// 受けた種類
	m_sDamageInfo.eReiceiveType = atkBall;

	// 今切り替えられました
}

//==========================================================================
// カバーキャッチ時処理
//==========================================================================
void CPlayer::CoverCatchSetting(CBall* pBall)
{
	// カバー対象を回復
	CPlayer* pCoverPlayer = pBall->GetCover();
	if (pCoverPlayer == nullptr) return;

	// 死んでいない味方回復
	if (!pCoverPlayer->GetMotionFrag().bDead &&
		m_typeTeam == pCoverPlayer->m_typeTeam)
	{
		pCoverPlayer->GetStatus()->LifeHeal(pBall->GetCoverHeal());
	}

	// 演出
	CEffect3D::Create(
		GetPosition(),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f),
		80.0f, 4.0f / 60.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	// 攻撃キャッチ処理
	pBall->CatchAttack(this);

	// ゲームマネージャ取得
	CGameManager* pGameMgr = CGameManager::GetInstance();
	if (pGameMgr == nullptr) return;

	// モテ加算
	pGameMgr->AddCharmValue(m_typeTeam, CCharmValueManager::ETypeAdd::ADD_COVERCATCH);

	// スペシャル加算
	pGameMgr->AddSpecialValue(m_typeTeam, CSpecialValueManager::ETypeAdd::ADD_COVERCATCH);
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

	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ダメージ受付時間更新
//==========================================================================
void CPlayer::UpdateDamageReciveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ダメージ受付判定リセット
	m_sDamageInfo.bReceived = false;

	// ダメージ受け付け時間減算
	m_sDamageInfo.fReceiveTime -= fDeltaTime * fSlowRate;
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
void CPlayer::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 色設定
	m_mMatcol = MyLib::Color(MyLib::color::White());
}

//==========================================================================
// 無敵状態
//==========================================================================
void CPlayer::StateInvincible(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 点滅設定
	float fMat = m_fStateTime;
	if (fMat > 1.0f)
	{// 1を超えたら範囲内にする
		fMat = static_cast<int>(fMat * 1000) % static_cast<int>(1000);
		fMat /= 1000.0f;
	}
	m_mMatcol.a = fMat;

	// 無敵
	m_sDamageInfo.bReceived = false;

	if (m_fStateTime >= StateTime::INVINCIBLE)
	{// 通常に戻す
		m_sDamageInfo.fReceiveTime = 0.0f;
		m_sDamageInfo.bReceived = true;

		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ダメージ状態
//==========================================================================
void CPlayer::StateDamage(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 pos = GetPosition();
	CMotion* pMotion = GetMotion();
	
	// ノックバック
	float time = m_fStateTime / StateTime::DAMAGE;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);
	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT,time);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::DAMAGE)
	{
		// 移動可能
		SetEnableMove(true);

		pMotion->ToggleFinish(true);
		SetState(STATE_INVINCIBLE);
	}
}

//==========================================================================
// 死亡状態
//==========================================================================
void CPlayer::StateDead(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 pos = GetPosition();

	float time = m_fStateTime / StateTime::DEAD;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT, time);

	SetPosition(pos);

	// 死亡状態をキャンセル不能にする
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
void CPlayer::StateDeadAfter(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 死亡状態をキャンセル不能にする
	SetEnableMove(false);

	// モーションの終了で管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->IsFinish())
	{// 死亡後モーションが終了

		// 死亡運搬
		SetState(EState::STATE_DEAD_CARRY);

		// 担架生成
		CStretcher::Create(this);
	}
}

//==========================================================================
// 死亡運搬
//==========================================================================
void CPlayer::StateDeadCarry(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 回避
//==========================================================================
void CPlayer::StateDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 通常キャッチ
//==========================================================================
void CPlayer::StateCatch_Normal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	pos.x += move.x * (fDeltaRate * fSlowRate);
	pos.z += move.z * (fDeltaRate * fSlowRate);
	SetPosition(pos);
	SetMove(move);

	//TODO: 砂埃的な何か
		
	if (pMotion->IsGetCancelable())
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ジャストキャッチ
//==========================================================================
void CPlayer::StateCatch_Just(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
// ジャストキャッチ
//==========================================================================
void CPlayer::StateCatch_Special(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// スペシャルキャッチ状態更新
	if (m_pCatchSpecial != nullptr)
	{
		m_pCatchSpecial->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
	else
	{// 終了すると自動的にnullptr
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayer::StateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMotion* pMotion = GetMotion();	// プレイヤーモーション情報

	if (!pMotion->IsSpecial())
	{ // モーションがスペシャルじゃなくなった場合

		// 何もしない状態にする
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// コート越え
//==========================================================================
void CPlayer::StateOutCourt(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

	if (pMotion->IsFinish())
	{// キャンセル可能

		m_sKnockback.posStart = pos;
		m_sKnockback.posEnd = CGameManager::GetInstance()->GetCourtMiddle(GetTeam());

		SetState(EState::STATE_OUTCOURT_RETURN);
	}
}

//==========================================================================
// コート越えから戻る
//==========================================================================
void CPlayer::StateOutCourt_Return(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 pos = GetPosition();

	//コート内に戻る
	pos = UtilFunc::Correction::EasingLinear(m_sKnockback.posStart, m_sKnockback.posEnd, 0.0f, StateTime::COURT_RETURN, m_fStateTime);
	SetPosition(pos);

	CMotion* pMotion = GetMotion();
	MyAssert::CustomAssert(pMotion != nullptr,"モーションisどこ？");

	if (pMotion->GetType() != MOTION_WALK)
	{
		// モーションのキャンセルで管理
		SetMotion(MOTION_WALK);
	}

	if (m_fStateTime >= StateTime::COURT_RETURN)
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// 相手コートに侵入トス
//==========================================================================
void CPlayer::StateInvade_Toss(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// オートモーション設定解除
	m_bAutoMotionSet = false;

	// 移動不可
	m_bPossibleMove = false;

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// トスするまで以下return
	if (m_pBall != nullptr)
	{
		m_fStateTime = 0.0f;

		// ずっとトスモーション
		pMotion->Set(EMotion::MOTION_TOSS);

		// チーム別向き設定
		MyLib::Vector3 rot = GetRotation();
		switch (m_typeTeam)
		{
		case CGameManager::SIDE_LEFT:	// 左チーム
			rot.y = -HALF_PI;
			break;

		case CGameManager::SIDE_RIGHT:	// 右チーム
			rot.y = HALF_PI;
			break;

		default:
			return;
			break;
		}
		SetRotDest(rot.y);

		return;
	}

	if (pMotion->GetType() == EMotion::MOTION_TOSS && !pMotion->IsGetCancelable())
	{// トスするまでここで終了

		m_fStateTime = 0.0f;
		return;
	}

	//-----------------------------
	// ボールのトス終了後処理
	//-----------------------------
	
	// 走って戻す
	SetMotion(CPlayer::EMotion::MOTION_WALK);

	// チーム別向き設定
	float fRotDest = 0.0f;
	switch (m_typeTeam)
	{
	case CGameManager::SIDE_LEFT:	// 左チーム
		fRotDest = HALF_PI;
		break;

	case CGameManager::SIDE_RIGHT:	// 右チーム
		fRotDest = -HALF_PI;
		break;

	default:
		return;
		break;
	}

	// 向き設定
	SetRotDest(fRotDest);

	// 遷移タイマー
	if (StateTime::INVADE_TOSS <= m_fStateTime)
	{
		// 相手コート侵入から戻る
		SetState(EState::STATE_INVADE_RETURN);
	}

}

//==========================================================================
// 相手コート侵入から戻る
//==========================================================================
void CPlayer::StateInvade_Return(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 自陣サイズ取得
	MyLib::Vector3 posCourt = MyLib::Vector3();
	MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize(m_typeTeam, posCourt);
	MyLib::Vector3 pos = GetPosition();

	// チーム別でラインの位置まで戻す
	MyLib::Vector3 posDest = pos;
	switch (m_typeTeam)
	{
	case CGameManager::SIDE_LEFT:	// 左チーム

		posDest.x = -CPlayerPosAdjIn::COMEBACK_LINE;
		break;

	case CGameManager::SIDE_RIGHT:	// 右チーム

		posDest.x = CPlayerPosAdjIn::COMEBACK_LINE;
		break;

	default:
		return;
		break;
	}

	// オートモーション設定解除
	m_bAutoMotionSet = false;

	// 移動不可
	m_bPossibleMove = false;
	m_bPossibleAction = false;

	// 走って戻す
	SetMotion(CPlayer::EMotion::MOTION_WALK);

	// 移動量更新
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();
#if 1
	move.x += sinf(D3DX_PI + rot.y) * Court::VELOCITY_INVADE * fDeltaRate * fSlowRate;
	move.z += cosf(D3DX_PI + rot.y) * Court::VELOCITY_INVADE * fDeltaRate * fSlowRate;
#else
	move.x += sinf(D3DX_PI + rot.y) * GetParameter().fVelocityDash;
	move.z += cosf(D3DX_PI + rot.y) * GetParameter().fVelocityDash;
#endif
	SetMove(move);


	// 戻る方向向く
	float rotDest = pos.AngleXZ(posDest);
	UtilFunc::Transformation::RotNormalize(rotDest);
	SetRotDest(rotDest);

#if 0	// 無敵にするならこれ有効に
	// ダメージ受付判定リセット
	m_sDamageInfo.bReceived = false;
#endif

	// X軸の位置で割合
	float ratio = GetPosition().x / posDest.x;
	if (ratio >= 1.0f)
	{// 完了
		m_bPossibleMove = true;
		m_bPossibleAction = true;
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// [終了] 無敵状態
//==========================================================================
void CPlayer::StateEndInvincible()
{
	m_mMatcol.a = 1.0f;
	m_sDamageInfo.fReceiveTime = 0.0f;
	m_sDamageInfo.bReceived = true;
}

//==========================================================================
// [終了] ダメージ状態
//==========================================================================
void CPlayer::StateEndDamage()
{
	CMotion* pMotion = GetMotion();

	// 移動可能
	SetEnableMove(true);
	pMotion->ToggleFinish(true);
}

//==========================================================================
// [終了] 死亡状態
//==========================================================================
void CPlayer::StateEndDead()
{
	// 死亡状態をキャンセル不能にする
	SetEnableMove(false);

	SetMotion(MOTION_DEAD_AFTER);
}

//==========================================================================
// [終了] 死亡後状態
//==========================================================================
void CPlayer::StateEndDeadAfter()
{
	// 死亡状態をキャンセル不能にする
	SetEnableMove(false);

	// 担架生成
	CStretcher::Create(this);
}

//==========================================================================
// [終了] 相手コート侵入から戻る
//==========================================================================
void CPlayer::StateEndInvade_Return()
{
	// フラグ解除解除
	m_bAutoMotionSet = true;
	m_bPossibleMove = true;
	m_bPossibleAction = true;
}

//==========================================================================
// プレイヤー位置補正の変更
//==========================================================================
void CPlayer::ChangePosAdjuster(EBaseType base, CGameManager::ETeamSide team, EFieldArea area)
{
	// プレイヤー位置補正の破棄
	SAFE_DELETE(m_pPosAdj);

	// 位置補正クラスの変更
	switch (area)
	{ // ポジションごとの処理
	case EFieldArea::FIELD_NONE:
		m_pPosAdj = DEBUG_NEW CPlayerPosAdjNone;
		break;

	case EFieldArea::FIELD_IN:
		switch (team)
		{ // チームコートごとの処理
		case CGameManager::ETeamSide::SIDE_LEFT:

			if (EBaseType::TYPE_USER == base)
			{// ユーザー
				m_pPosAdj = DEBUG_NEW CPlayerPosAdjInLeft;
			}
			else
			{
				m_pPosAdj = DEBUG_NEW CPlayerAIPosAdjInLeft;
			}

			break;

		case CGameManager::ETeamSide::SIDE_RIGHT:
			if (EBaseType::TYPE_USER == base)
			{// ユーザー
				m_pPosAdj = DEBUG_NEW CPlayerPosAdjInRight;
			}
			else
			{
				m_pPosAdj = DEBUG_NEW CPlayerAIPosAdjInRight;
			}

			break;

		default:
			assert(false);
			break;
		}
		break;

	case EFieldArea::FIELD_OUT:
		m_pPosAdj = DEBUG_NEW CPlayerPosAdjOut;
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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
	// 状態更新
	if (m_StateEndFunc[m_state] != nullptr)
	{
		(this->*(m_StateEndFunc[m_state]))();
	}

	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// ベースの初期化処理
//==========================================================================
void CPlayer::InitBase(EBaseType type)
{
	// 既にベースが設定済みの場合エラー
	if (m_pBase != nullptr) { assert(false); return; }

	// ベースクラスの初期化
	switch (type)
	{ // ユーザー種類ごとの処理
	case TYPE_USER:
		switch (m_typeArea)
		{ // ポジションごとの処理
		case EFieldArea::FIELD_NONE:
			m_pBase = DEBUG_NEW CPlayerNone(this, m_typeTeam, m_typeArea);
			break;

		case EFieldArea::FIELD_IN:
			m_pBase = DEBUG_NEW CPlayerUserIn(this, m_typeTeam, m_typeArea);
			break;

		case EFieldArea::FIELD_OUT:
			m_pBase = DEBUG_NEW CPlayerUserOut(this, m_typeTeam, m_typeArea);
			break;

		default:
			assert(false);
			break;
		}
		break;

	case TYPE_AI:
		switch (m_typeArea)
		{ // ポジションごとの処理
		case EFieldArea::FIELD_IN:
			m_pBase = DEBUG_NEW CPlayerAIIn(this, m_typeTeam, m_typeArea);
			break;

		case EFieldArea::FIELD_OUT:
			m_pBase = DEBUG_NEW CPlayerAIOut(this, m_typeTeam, m_typeArea);
			break;

		default:
			assert(false);
			break;
		}
		break;

	default:
		assert(false);
		break;
	}

}

//==========================================================================
// ベースの変更処理
//==========================================================================
void CPlayer::ChangeBase(EBaseType type)
{
	CPlayerManager* pManager = CPlayerManager::GetInstance();	// プレイヤーマネージャー
	assert(pManager != nullptr);

	// ベースクラスの破棄
	SAFE_DELETE(m_pBase);

	// プレイヤーマネージャーから自身を削除
	pManager->DeletePlayer(this);

	// ベースの初期化
	InitBase(type);

	// プレイヤーマネージャーに自身を再登録
	m_nPosIdx = pManager->RegistPlayer(this, m_nPosIdx);
}

//==========================================================================
// ベースの取得処理
//==========================================================================
CPlayer::EBaseType CPlayer::GetBaseType() const
{
	// クラス型からベースを判定
	if		(typeid(*m_pBase) == typeid(CPlayerUserIn) || typeid(*m_pBase) == typeid(CPlayerUserOut))	{ return TYPE_USER; }
	else if	(typeid(*m_pBase) == typeid(CPlayerAIIn)   || typeid(*m_pBase) == typeid(CPlayerAIOut))		{ return TYPE_AI; }
	else if (typeid(*m_pBase) == typeid(CPlayerNone))													{ return TYPE_USER; }

	// ベース指定なし
	assert(false);
	return (EBaseType)-1;
}

//==========================================================================
// 未来位置の計算処理
//==========================================================================
MyLib::Vector3 CPlayer::CalcFuturePosition(const int nFutureFrame)
{
	MyLib::Vector3 posFuture = GetPosition();
	MyLib::Vector3 moveFuture = GetMove();
	for (int i = 0; i < nFutureFrame; i++)
	{
		// 移動量加算
		posFuture.y += moveFuture.y;

		// 着地した場合抜ける
		if (posFuture.y < 0.0f) { posFuture.y = 0.0f; moveFuture.y = 0.0f; break; }

		// 重力処理
		moveFuture.y -= mylib_const::GRAVITY;
	}

	// 更新したプレイヤーの位置を返す
	return posFuture + LOOK_OFFSET;
}

//==========================================================================
// 未来位置オフセットの取得処理
//==========================================================================
MyLib::Vector3 CPlayer::GetLookOffset() const
{
	// 狙う位置オフセットを返す
	return LOOK_OFFSET;
}

//==========================================================================
// ドレスアップ生成
//==========================================================================
void CPlayer::CreateDressUp()
{
	// ドレスアップ削除
	DeleteDressUp();

	// ドレスアップ(髪)
	m_pDressUp_Hair = CDressup::Create(
		CDressup::EType::TYPE_HAIR,		// 着せ替えの種類
		this,							// 変更するプレイヤー
		CPlayer::ID_HAIR);				// 変更箇所のインデックス

	// ドレスアップ(アクセ)
	m_pDressUp_Accessory = CDressup::Create(
		CDressup::EType::TYPE_ACCESSORY,	// 着せ替えの種類
		this,								// 変更するプレイヤー
		CPlayer::ID_ACCESSORY);				// 変更箇所のインデックス

	// ドレスアップ(顔)
	m_pDressUp_Face = CDressup::Create(
		CDressup::EType::TYPE_FACE,	// 着せ替えの種類
		this,						// 変更するプレイヤー
		CPlayer::ID_FACE);			// 変更箇所のインデックス
}

//==========================================================================
// ドレスアップ削除
//==========================================================================
void CPlayer::DeleteDressUp()
{
	SAFE_UNINIT(m_pDressUp_Hair);
	SAFE_UNINIT(m_pDressUp_Accessory);
	SAFE_UNINIT(m_pDressUp_Face);
}

//==========================================================================
// ドレスアップ割り当て
//==========================================================================
void CPlayer::BindDressUp(int nHair, int nAccessory, int nFace)
{
	// 再割り当て
	m_pDressUp_Hair->ReRegist(nHair);			// ドレスアップ(髪)
	m_pDressUp_Accessory->ReRegist(nAccessory);	// ドレスアップ(アクセ)
	m_pDressUp_Face->ReRegist(nFace);			// ドレスアップ(顔)
}

//==========================================================================
// 影の生成処理
//==========================================================================
HRESULT CPlayer::CreateShadow()
{
	// 影の生成
	m_pShadow = CShadow::Create(this, SHADOW_RADIUS, SHADOW_MIN_ALPHA, SHADOW_MAX_ALPHA);
	if (m_pShadow == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CPlayer::Debug()
{
#if _DEBUG

	// 
	ImGui::Checkbox("bMove", &m_bDebugMove);

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
		// 拡大率の調整
		float scale = GetScale();
		ImGui::DragFloat("Scale", &scale, 0.001f, 0.01f, 100.0f, "%.3f");
		SetScale(scale);

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
		ImGui::DragFloat3("ballOffset", (float*)&parameter.ballOffset, 0.1f, -2000.0f, 2000.0f, "%.3f");

		SetRadius(parameter.fRadius);	// 半径反映

		// パラメーター反映
		pStatus->SetParameter(parameter);

		// ボールのパラメータ
		if (ImGui::TreeNodeEx("Ball Parameter", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// ボールのパラメータ取得
			CBallStatus::SBallParameter ballParam = GetBallParameter();

			ImGui::DragFloat("fThrowMoveNormal", &ballParam.fThrowMoveNormal, 0.01f, 0.0f, 10000.0f, "%.3f");
			ImGui::DragFloat("fThrowMoveJump", &ballParam.fThrowMoveJump, 0.01f, 0.0f, 10000.0f, "%.3f");
			ImGui::DragFloat("fThrowMoveSpecial", &ballParam.fThrowMoveSpecial, 0.01f, 0.0f, 10000.0f, "%.3f");
			ImGui::DragFloat("fKnockbackNormal", &ballParam.fKnockbackNormal, 0.1f, 0.0f, 10000.0f, "%.3f");
			ImGui::DragFloat("fKnockbackJump", &ballParam.fKnockbackJump, 0.1f, 0.0f, 10000.0f, "%.3f");
			ImGui::DragFloat("fKnockbackSpecial", &ballParam.fKnockbackSpecial, 0.1f, 0.0f, 10000.0f, "%.3f");
			ImGui::DragFloat("fCatchRange", &ballParam.fCatchRange, 0.01f, 0.0f, 10000.0f, "%.3f");

			// パラメーター反映
			SetBallParameter(ballParam);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	//-----------------------------
	// 情報表示
	//-----------------------------
	if (ImGui::TreeNode("Transform Info"))
	{
		MyLib::Vector3 pos = GetPosition(), posOrigin = GetOriginPosition();
		MyLib::Vector3 rot = GetRotation();
		MyLib::Vector3 move = GetMove();
		CMotion* motion = GetMotion();
		CPlayer::EMotion motionType = static_cast<CPlayer::EMotion>(motion->GetType());
		CPlayer::EAction action = m_pActionPattern->GetAction();
		CPlayer::EDashAngle* angle = m_pBase->GetPlayerControlMove()->GetInputAngle();

		ImGui::Text("pos : [X : %.2f, Y : %.2f, Z : %.2f]", pos.x, pos.y, pos.z);
		ImGui::Text("posOrigin : [X : %.2f, Y : %.2f, Z : %.2f]", posOrigin.x, posOrigin.y, posOrigin.z);
		ImGui::Text("rot : [X : %.2f, Y : %.2f, Z : %.2f]", rot.x, rot.y, rot.z);
		ImGui::Text("rotDest : [Y : %.2f]", GetRotDest());
		ImGui::Text("move : [X : %.2f, Y : %.2f, Z : %.2f]", move.x, move.y, move.z);
		ImGui::Text("Life : [%d]", GetLife());
		ImGui::Text("Motion : [%s]", magic_enum::enum_name(motionType));
		ImGui::Text("Action : [%s]", magic_enum::enum_name(action));
		ImGui::Text("State : [%s]", magic_enum::enum_name(m_state));
		ImGui::Text("StateTime : [%.2f]", m_fStateTime);
		ImGui::Text("bDash : [%d]", m_bDash);
		ImGui::Text("bBrake : [%d]", m_bBrake);
		ImGui::Text("EscapeTime : [%.2f]", m_fEscapeTime);
		ImGui::Text("bDead : [%s]", (m_sMotionFrag.bDead ? "true" : "false"));

		ImGui::Text("typeBase : [%s]", magic_enum::enum_name(GetBaseType()));
		ImGui::Text("typeTeam : [%s]", magic_enum::enum_name(m_typeTeam));
		ImGui::Text("typeArea : [%s]", magic_enum::enum_name(m_typeArea));

		ImGui::Text("InputAngleCtr : [%.2f]", m_pBase->GetPlayerControlMove()->GetInputAngleCtr());
		if (angle != nullptr)
		{
			ImGui::Text("InputAngle : [%s]", magic_enum::enum_name(*angle));
		}
		else
		{
			ImGui::Text("InputAngle : [error]");
		}

#if 0
		ImGui::Text("bPossibleMove: [%s]", m_bPossibleMove ? "true" : "false");
		ImGui::Text("CrabMoveEasing : [%.3f]", m_pBase->GetPlayerControlMove()->GetCrabMoveEasingTime());
		ImGui::Text("bMove: [%d]", m_sMotionFrag.bMove);
		ImGui::Text("bDash: [%d]", m_bDash);
#endif

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

	if (ImGui::TreeNode("Dodge"))
	{// 回避時処理
		float min = m_pActionPattern->GetSlowStart();
		float max = m_pActionPattern->GetSlowEnd();

		ImGui::DragFloat("start", &min, 0.01f, 0.0f, 2.0f, "%f");
		ImGui::DragFloat("end", &max, 0.01f, 0.0f, 2.0f, "%f");

		m_pActionPattern->SetSlowStart(min);
		m_pActionPattern->SetSlowEnd(max);

		if (ImGui::Button("Let's Action!"))
		{// スペシャル

			m_pActionPattern->SetAction(EAction::ACTION_DODGE);
		}

		ImGui::TreePop();
	}

	if (ImGui::Button("Special(Motion)"))
	{// スペシャル

		SetMotion(EMotion::MOTION_SPECIAL);
	}

	if (ImGui::Button("Special"))
	{// スペシャル

		SetMotion(EMotion::MOTION_SPECIAL);

		CCamera* pCamera = CManager::GetInstance()->GetCamera();

		// カメラ位置を攻撃プレイヤーの位置にする
		pCamera->GetCameraMotion()->SetPosition(GetPosition());

		// スペシャル盛り上げモーションを設定
		pCamera->GetCameraMotion()->SetMotion(CCameraMotion::MOTION::MOTION_KAMEHAMEHA, false, true, true, true);
	}

	if (ImGui::Button("Dead"))
	{// リセット
		MyLib::HitResult_Character* result = DEBUG_NEW MyLib::HitResult_Character;
		CBall* pBall = CBall::Create(GetPosition());
		DeadSetting(result,pBall);
		delete result;
		pBall->Kill();
	}

	if (ImGui::Button("CatchSpecial"))
	{// キャッチ時処理(スペシャル)
		CatchSettingSpecial(false, CBall::ESpecial::SPECIAL_KAMEHAMEHA);
	}

	if (m_pCatchSpecial != nullptr)
	{
		m_pCatchSpecial->Debug();
	}

	//-----------------------------
	// コート外検証
	//-----------------------------
	if (ImGui::Button("OutCourt"))
	{// リセット
		OutCourtSetting();
	}

	//-----------------------------
	// ベースのデバッグ表示
	//-----------------------------
	m_pBase->Debug();

#endif
}

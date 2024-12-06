//==========================================================================
// 
//  ボール処理 [ball.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "ball.h"
#include "player.h"
#include "playerBase.h"
#include "playerStatus.h"
#include "manager.h"
#include "game.h"
#include "gamemanager.h"
#include "calculation.h"
#include "model.h"
#include "ballHolderMarker.h"
#include "ballLandingMarker.h"
#include "shadow.h"
#include "specialManager.h"
#include "playerManager.h"

#include "debugproc.h"
#include "3D_Effect.h"

// TODO：AI/User切り替え
#if 1
#define CHANGE
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char*	MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// ボールモデル
	const float GRAVITY = mylib_const::GRAVITY * 0.6f;	// ボールにかかる重力
	const float	RADIUS = 17.0f;					// 半径
	const float PLUS_RADIUS = RADIUS * 1.8f;	// 判定用半径
	const float	RADIUS_SHADOW = 20.0f;			// 影の半径
	const float	MIN_ALPHA_SHADOW = 0.32f;		// 影の透明度
	const float	MAX_ALPHA_SHADOW = 0.48f;		// 影の透明度
	const float	REV_MOVE = 0.025f;		// 移動量の補正係数
	const float	REV_INIMOVE = 0.1f;		// 初速の補正係数
	const float	MAX_DIS = 100000.0f;	// ホーミングする最大距離
	const int	VIEW_ANGLE = 104;		// 視野角
	const float DEST_POSY = 45.0f;		// 通常ボールの目標Y座標
	const float REV_POSY = 0.1f;		// 通常ボールの目標Y座標の補正係数
	const float MAX_BOUND_MOVE = 0.8f;	// バウンド時の上移動量最大値

	const char* DEBUG_STATE_PRINT[] =	// デバッグ表示用状態
	{
		"[SPAWN]    生成状態 (フリーボール)",
		"[CATCH]    キャッチ状態 (プレイヤー所持)",
		"[HOM_NOR]  通常ホーミング状態 (攻撃判定ON)",
		"[HOM_JUMP] ジャンプホーミング状態 (攻撃判定ON)",
		"[MOVE]     移動状態 (攻撃判定ON)",
		"[S_STAG]   スペシャル演出状態 (開始前演出)",
		"[S_THROW]  スペシャル投げ状態 (攻撃判定ON)",
		"[HOM_PASS] ホーミングパス状態 (内野→外野)",
		"[PASS]     パス状態 (外野→内野)",
		"[REBOUND]  リバウンド状態 (ぶつかった時の落下)",
		"[FREE]     フリー状態 (敵のみとれる)",
		"[LAND]     着地状態 (地面落下)",
	};
	const char* DEBUG_TEAM_PRINT[] =	// デバッグ表示用チーム
	{
		"[NONE]    (コート指定なし)",
		"[LEFT]    (左コート)",
		"[RIGHT]   (右コート)",
	};
	const char* DEBUG_ATK_PRINT[] =		// デバッグ表示用攻撃
	{
		"[NONE]    (攻撃判定無し)",
		"[NORMAL]  (通常攻撃)",
		"[JUMP]    (ジャンプ攻撃)",
		"[SPECIAL] (スペシャリスト攻撃)",
	};
	const char* DEBUG_SPECIAL_PRINT[] =	// デバッグ表示用スペシャル
	{
		"[NONE]    (指定なし)",
		"[かめはめ波]",
	};
	const char* DEBUG_BOOL_PRINT[] =	// デバッグ表示用フラグ
	{
		"[FALSE]",
		"[TRUE]",
	};

	namespace normal
	{
		const float THROW_MOVE = 22.5f;		// 通常投げ移動速度
		const float REV_HOMING = 0.035f;	// ホーミングの慣性補正係数
		const float TIME_HOMING = 1.2f;		// ホーミングが切れるまでの時間
	}

	namespace jump
	{
		const float THROW_MOVE = 27.0f;		// ジャンプ投げ移動速度
		const float REV_HOMING = 0.24f;		// ホーミングの慣性補正係数
		const float MIN_MOVE_DOWN = -0.3f;	// ジャンプ攻撃の最低下移動量
		const float OFFSET_TARGET_BACK = 150.0f;	// ターゲットの後ろオフセット
	}

	namespace special
	{
		const int	TIMING_ATK_IDX = 0;	// スペシャルボールの投げるタイミング
		const float	THROW_MOVE = 20.0f;	// スペシャル投げ移動速度
	}

	namespace kamehameha
	{
		const float REV_HOMING = 0.46f;	// ホーミングの慣性補正係数
	}

	namespace move
	{
		const float TIME_GRAVITY = 0.8f;		// 重力がかかり始めるまでの時間
		const float MULTIPLY_INIMOVE = 2.8f;	// 初速の倍率
	}

	namespace rebound
	{
#if _DEBUG
		const float MOVE_UP = 10.5f;	// 上移動量
#else
		const float MOVE_UP = 5.5f;		// 上移動量
#endif
		const float MOVE_SPEED = 2.5f;	// 移動速度
	}

	namespace toss
	{
		const float THROW_MOVE = 3.5f;	// トス移動速度
		const float MOVE_UP = 2.5f;		// トス上移動量
	}

	namespace pass
	{
		const float MOVE_RATE = 0.015f;			// 移動速度の距離割合
		const float HOMING_TIMERATE = 0.65f;	// Y座標のホーミングを行う時間割合
		const float TARGET_PULSY = 50.0f;		// ターゲット対象のY座標加算量
		const float TIME_NORAML = 1.2f;			// 通常パスの経過時間
		const float TIME_HOMING = 1.0f;			// ホーミングパスの経過時間
		const float MAX_HEIGHT = 250.0f;		// ホーミングの最高到達点
	}

	namespace spawn
	{
		const float THROW_MOVE = 3.5f;	// スポーン移動速度
		const float MOVE_UP = 6.5f;		// スポーン上移動量
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CBall::STATE_FUNC CBall::m_StateFuncList[] =
{
	&CBall::UpdateSpawn,		// 生成状態の更新
	&CBall::UpdateCatch,		// キャッチ状態の更新
	&CBall::UpdateHomingNormal,	// 通常ホーミング状態の更新
	&CBall::UpdateHomingJump,	// ジャンプホーミング状態の更新
	&CBall::UpdateMove,			// 移動状態の更新
	&CBall::UpdateSpecialStag,	// スペシャル演出状態の更新
	&CBall::UpdateSpecialThrow,	// スペシャル投げ状態の更新
	&CBall::UpdateHomingPass,	// パスホーミング状態の更新
	&CBall::UpdatePass,			// パス状態の更新
	&CBall::UpdateReBound,		// リバウンド状態の更新
	&CBall::UpdateFree,			// フリー状態の更新
	&CBall::UpdateLand,			// 着地状態の更新
};

// スペシャル関数
CBall::SPECIAL_FUNC CBall::m_SpecialFuncList[] =
{
	nullptr,					// スペシャル無し
	&CBall::UpdateKamehameha,	// かめはめ波の更新
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CBall> CBall::m_list = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CBall::CBall(int nPriority) : CObjectX(nPriority),
	m_typeTeam		(CGameManager::SIDE_NONE),	// チームサイド
	m_pHoldMarker	(nullptr),		// ボール所持マーカー情報
	m_pLandMarker	(nullptr),		// ボール着地点マーカー情報
	m_pShadow		(nullptr),		// 影情報
	m_pPlayer		(nullptr),		// プレイヤー情報
	m_pTarget		(nullptr),		// ホーミングターゲット情報
	m_pCover		(nullptr),		// カバー対象プレイヤー情報
	m_fMoveSpeed	(0.0f),			// 移動速度
	m_fGravity		(0.0f),			// 重力
	m_fHomingTime	(0.0f),			// ホーミング時間
	m_bLanding		(false),		// 着地フラグ
	m_posPassStart	(VEC3_ZERO),	// パス開始位置
	m_posPassEnd	(VEC3_ZERO),	// パス終了位置
	m_typeSpecial	(SPECIAL_NONE),	// スペシャル種類
	m_typeAtk		(ATK_NONE),		// 攻撃種類
	m_state			(STATE_SPAWN),	// 状態
	m_fStateTime	(0.0f),			// 状態カウンター
	m_pThrowLine	(nullptr),		// 投げのライン
	m_pAura			(nullptr),		// オーラ
	m_nDamage		(0),			// ダメージ
	m_fKnockback	(0.0f)			// ノックバック
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_StateFuncList)   == CBall::STATE_MAX,   "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_SpecialFuncList) == CBall::SPECIAL_MAX, "ERROR : Special Count Mismatch");

	static_assert(NUM_ARRAY(DEBUG_STATE_PRINT)   == CBall::STATE_MAX,   "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(DEBUG_ATK_PRINT)     == CBall::ATK_MAX,     "ERROR : Attack Count Mismatch");
	static_assert(NUM_ARRAY(DEBUG_SPECIAL_PRINT) == CBall::SPECIAL_MAX, "ERROR : Special Count Mismatch");
}

//==========================================================================
// デストラクタ
//==========================================================================
CBall::~CBall()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CBall *CBall::Create(const MyLib::Vector3& rPos)
{
	// メモリの確保
	CBall* pObj = DEBUG_NEW CBall;
	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}

		// 位置を設定
		pObj->SetPosition(rPos);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBall::Init()
{
	// リストに追加
	m_list.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 親クラスの初期化
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr)) { return E_FAIL; }

	// ボール所持マーカーの生成
	m_pHoldMarker = CBallHolderMarker::Create(nullptr);
	if (m_pHoldMarker == nullptr) { return E_FAIL; }

	// ボール着地点マーカーの生成
	m_pLandMarker = CBallLandingMarker::Create(this);
	if (m_pLandMarker == nullptr) { return E_FAIL; }

	// 影の生成
	m_pShadow = CShadow::Create(this, RADIUS_SHADOW, MIN_ALPHA_SHADOW, MAX_ALPHA_SHADOW);
	if (m_pShadow == nullptr) { return E_FAIL; }

	// オーラ生成
	m_pAura = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_BALL,
			GetPosition(),
			MyLib::Vector3(),
			MyLib::Vector3(),
			15.0f, false);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBall::Uninit()
{
	// リストから削除
	m_list.Delete(this);

	// 親クラスの終了
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CBall::Kill()
{
	// 影の削除
	SAFE_KILL(m_pShadow);

	// リストから削除
	m_list.Delete(this);

	// 親クラスの終了
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBall::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 前回の位置を更新
	SetOldPosition(GetPosition());

	// 着地していない状態にする
	m_bLanding = false;

	// 初速移動量の更新
	if (IsAttack())
	{ // 攻撃中の場合

		// 初速を減衰させる
		m_fInitialSpeed += (0.0f - m_fInitialSpeed) * (REV_INIMOVE * fDeltaRate * fSlowRate);
	}

	if (m_StateFuncList[m_state] != nullptr)
	{ // 状態更新関数がある場合

		// 状態別処理
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 現在のチームサイドを更新
	UpdateTypeTeam();

	// 現在の攻撃種類を更新
	UpdateTypeAtk();

	// 現在のスペシャル種類を更新
	UpdateTypeSpecial();

	// 投げエフェクト
	UpdateThrowLine();

	// 親クラスの更新
	CObjectX::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// カメラ情報のテキスト描画
	GET_MANAGER->GetDebugProc()->Print
	(
		"\n---------------- ボール情報 ----------------\n"
		"【ボール状態】%s\n"
		"【チーム方向】%s\n"
		"【 攻撃状態 】%s\n"
		"【スペシャル】%s\n"
		"【着地フラグ】%s\n"
		"【 所有対象 】[%s]\n"
		"【ターゲット】[%s]\n"
		"【カバー対象】[%s]\n",
		DEBUG_STATE_PRINT[m_state],
		DEBUG_TEAM_PRINT[m_typeTeam + 1],
		DEBUG_ATK_PRINT[m_typeAtk],
		DEBUG_SPECIAL_PRINT[m_typeSpecial],
		DEBUG_BOOL_PRINT[(int)m_bLanding],
		(m_pPlayer == nullptr) ? "nullptr" : "player",
		(m_pTarget == nullptr) ? "nullptr" : "player",
		(m_pCover  == nullptr) ? "nullptr" : "player"
	);


#if _DEBUG
	// デバッグ出力
	if (ImGui::TreeNode("Ball Info"))
	{
		MyLib::Vector3 pos = GetPosition();
		MyLib::Vector3 move = GetMove();
		ImGui::Text("pos : [X : %.2f, Y : %.2f, Z : %.2f]", pos.x, pos.y, pos.z);
		ImGui::Text("move : [X : %.2f, Y : %.2f, Z : %.2f]", move.x, move.y, move.z);
		ImGui::Text("m_fMoveSpeed : [%.2f]", m_fMoveSpeed);
		ImGui::Text("m_fGravity : [%.2f]", m_fGravity);
		ImGui::Text("m_nDamage : [%d]", m_nDamage);

		if (ImGui::Button("Reset"))
		{// リセット
			pos = MyLib::Vector3();
			m_state = EState::STATE_LAND;

			SetPosition(pos);
		}

		ImGui::TreePop();
	}

#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CBall::Draw()
{
	// 親クラスの描画
	CObjectX::Draw();
}

//==========================================================================
// 攻撃キャッチ処理
//==========================================================================
void CBall::CatchAttack(CPlayer* pPlayer)
{
	// 誰かがすでに持っているボールの場合抜ける
	if (m_pPlayer != nullptr) { return; }

	MyLib::Vector3 oldPosBall = GetOldPosition();				// ボール過去位置
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();			// プレイヤー位置
	float fAngleY = oldPosBall.AngleXZ(posPlayer) - D3DX_PI;	// ボール方向

	// キャッチ処理
	Catch(pPlayer);

	// 目標向きをボール方向にする
	m_pPlayer->SetRotDest(fAngleY);
}

//==========================================================================
// 着地キャッチ処理
//==========================================================================
void CBall::CatchLand(CPlayer* pPlayer)
{
	// 誰かがすでに持っているボールの場合抜ける
	if (m_pPlayer != nullptr) { return; }

	// キャッチ処理
	Catch(pPlayer);
}

//==========================================================================
// スポーン処理
//==========================================================================
void CBall::Spawn(CPlayer* pPlayer)
{
	// 投げ処理
	Throw(pPlayer);

	// 生成状態にする
	SetState(STATE_SPAWN);

	// 横位置を初期化
	MyLib::Vector3 pos = GetPosition();
	pos.x = pos.z = 0.0f;
	SetPosition(pos);

	// 上移動量を与える
	MyLib::Vector3 move = GetMove();
	move.x = move.z = 0.0f;
	move.y = spawn::MOVE_UP;
	SetMove(move);

	// 移動量を設定
	m_fMoveSpeed = spawn::THROW_MOVE;

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// 通常投げ処理
//==========================================================================
void CBall::ThrowNormal(CPlayer* pPlayer)
{
	// キャッチしていないボールを投げようとした場合エラー
	assert(m_state == STATE_CATCH);

	// ホーミング対象の設定
	m_pTarget = CollisionThrowTarget();
	if (m_pTarget != nullptr)
	{ // ターゲットがいる場合

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();		// ボール過去位置
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// プレイヤー位置
		float fAngleY = posPlayer.AngleXZ(posTarget);			// ボール方向

		// ホーミング状態にする
		SetState(STATE_HOM_NOR);

		// 目標向き/向きをボール方向にする
		m_pPlayer->SetRotDest(fAngleY);
		m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));

		// 投げ処理
		Throw(pPlayer);

		// 移動ベクトルを正規化して再設定
		SetMove(CalcVecMove(m_pTarget, pPlayer).Normal());
	}
	else
	{ // ターゲットがいない場合

		// 投げ処理
		Throw(pPlayer);

		// 移動状態にする
		SetState(STATE_MOVE);
	}

#ifdef CHANGE
	if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
	{ // 外野が投げた場合

		// 近くのAIに操作権を移し、自身をAIにする
		CPlayerManager::GetInstance()->ChangeUserToAI(pPlayer);
	}
#endif

	// 通常攻撃を設定
	m_typeAtk = ATK_NORMAL;

	// ダメージ設定
	m_nDamage = pPlayer->GetBallParameter().nDamageNormal;

	// ノックバック設定
	m_fKnockback = pPlayer->GetBallParameter().fKnockbackNormal;
	
	// 移動量設定
	m_fMoveSpeed = pPlayer->GetBallParameter().fThrowMoveNormal;

	// 初速を設定
	CalcSetInitialSpeed(m_fMoveSpeed);

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// ジャンプ投げ処理
//==========================================================================
void CBall::ThrowJump(CPlayer* pPlayer)
{
	// キャッチしていないボールを投げようとした場合エラー
	assert(m_state == STATE_CATCH);

	// ホーミング対象の設定
	m_pTarget = CollisionThrowTarget();
	if (m_pTarget != nullptr)
	{ // ターゲットがいる場合

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();		// ボール過去位置
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// プレイヤー位置
		float fAngleY = posPlayer.AngleXZ(posTarget);			// ボール方向

		// ホーミング状態にする
		SetState(STATE_HOM_JUMP);

		// 目標向き/向きをボール方向にする
		m_pPlayer->SetRotDest(fAngleY);
		m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));

		// 投げ処理
		Throw(pPlayer);
	}
	else
	{ // ターゲットがいない場合

		// 投げ処理
		Throw(pPlayer);

		// 移動ベクトルの取得
		MyLib::Vector3 vecMove = GetMove();

		// 下方向に移動ベクトルを与える
		vecMove.y = jump::MIN_MOVE_DOWN;

		// 移動ベクトルを正規化して設定
		SetMove(vecMove.Normal());

		// 移動状態にする
		SetState(STATE_MOVE);
	}

#ifdef CHANGE
	if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
	{ // 外野が投げた場合

		// 近くのAIに操作権を移し、自身をAIにする
		CPlayerManager::GetInstance()->ChangeUserToAI(pPlayer);
	}
#endif

	// ジャンプ攻撃を設定
	m_typeAtk = ATK_JUMP;

	// ダメージ設定
	m_nDamage = pPlayer->GetBallParameter().nDamageJump;

	// ノックバック設定
	m_fKnockback = pPlayer->GetBallParameter().fKnockbackJump;

	// 移動量設定
	m_fMoveSpeed = pPlayer->GetBallParameter().fThrowMoveJump;

	// 初速を設定
	CalcSetInitialSpeed(m_fMoveSpeed);

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_JUMP);
}

//==========================================================================
// スペシャル発動処理
//==========================================================================
void CBall::Special(CPlayer* pPlayer)
{
	// ホーミング対象の設定
	m_pTarget = CollisionThrowTarget(true);
	assert(m_pTarget != nullptr);	// 見つからなきゃ敵がいないよ～

	// スペシャル演出状態にする
	SetState(STATE_SPECIAL_STAG);

	// TODO：スペシャルの種類設定
	m_typeSpecial = SPECIAL_KAMEHAMEHA;

	// ダメージ設定
	m_nDamage = pPlayer->GetBallParameter().nDamageSpecial;

	// ノックバック設定
	m_fKnockback = pPlayer->GetBallParameter().fKnockbackSpecial;
	
	// スペシャル演出マネージャーの生成
	CSpecialManager::Create(m_pPlayer, m_pTarget);
}

//==========================================================================
// パス処理
//==========================================================================
void CBall::Pass(CPlayer* pPlayer)
{
	// キャッチしていないボールを投げようとした場合エラー
	assert(m_state == STATE_CATCH);

	// パス対象の設定
	m_pTarget = CollisionPassTarget();
	if (m_pTarget != nullptr)
	{ // ターゲットがいる場合

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();		// ボール過去位置
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// プレイヤー位置
		float fAngleY = posPlayer.AngleXZ(posTarget);			// ボール方向

		// 目標向き/向きをボール方向にする
		m_pPlayer->SetRotDest(fAngleY);
		m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));
	}

	// ターゲットがいない場合エラー
	assert(m_pTarget != nullptr);

	// 投げ処理
	Throw(pPlayer);

	// 現在のボール位置をパス開始位置にする
	m_posPassStart = GetPosition();

	if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
	{ // 内野の場合

		// ベースの入れ替え
		CPlayerManager::GetInstance()->SwapBase(pPlayer, m_pTarget);

		// ホーミングパス状態にする
		SetState(STATE_HOM_PASS);

		// パス終了Y座標を設定
		m_posPassEnd.y = CGameManager::FIELD_LIMIT + pass::TARGET_PULSY;
	}
	else
	{ // 外野の場合

		if (m_pTarget->GetBaseType() == CPlayer::EBaseType::TYPE_AI)
		{ // パスターゲットがAIの場合

			// ベースの入れ替え
			CPlayerManager::GetInstance()->SwapBase(pPlayer, m_pTarget);
		}
		else
		{ // パスターゲットがユーザーの場合

			// 近くのAIに操作権を移し、自身をAIにする
			CPlayerManager::GetInstance()->ChangeUserToAI(pPlayer);
		}

		// パス状態にする
		SetState(STATE_PASS);

		// パス終了位置を設定
		m_posPassEnd = m_pTarget->GetPosition();	// 現在のターゲット位置
		m_posPassEnd.y = CGameManager::FIELD_LIMIT;	// Y座標は地面固定

		// 移動量を設定
		m_fMoveSpeed = m_posPassStart.Distance(m_posPassEnd) * pass::MOVE_RATE;
	}

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// トス処理
//==========================================================================
void CBall::Toss(CPlayer* pPlayer)
{
	// 投げ処理
	Throw(pPlayer);

	// フリー状態にする
	SetState(STATE_FREE);

	// 上移動量を与える
	MyLib::Vector3 move = GetMove();
	move.y = toss::MOVE_UP;
	SetMove(move);

	// 移動量を設定
	m_fMoveSpeed = toss::THROW_MOVE;

	// サウンド再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_THROW_NORMAL);
}

//==========================================================================
// 半径の取得処理
//==========================================================================
float CBall::GetRadius() const
{
	// 半径を返す
	return RADIUS;
}

//==========================================================================
// 攻撃フラグの取得処理
//==========================================================================
bool CBall::IsAttack() const
{
	// 攻撃フラグを返す
	return (m_state == STATE_HOM_NOR || m_state == STATE_HOM_JUMP || m_state == STATE_MOVE || m_state == STATE_SPECIAL_THROW);	// TODO：攻撃状態が増えたら追加
}

//==========================================================================
// パスフラグの取得処理
//==========================================================================
bool CBall::IsPass() const
{
	// パスフラグを返す
	return (m_state == STATE_HOM_PASS || m_state == STATE_PASS);	// TODO：パス状態が増えたら追加
}

//==========================================================================
// スペシャルフラグの取得処理
//==========================================================================
bool CBall::IsSpecial() const
{
	// スペシャルフラグを返す
	return (m_state == STATE_SPECIAL_STAG || m_state == STATE_SPECIAL_THROW);	// TODO：スペシャル状態が増えたら追加
}

//==========================================================================
// ワールドマトリックスの計算処理
//==========================================================================
void CBall::CalWorldMtx()
{
	if (m_state == STATE_CATCH || m_state == STATE_SPECIAL_STAG)
	{ // キャッチ中の場合

		const int nPartsIdx = m_pPlayer->GetParameter().nBallPartsIdx;		// ボールパーツインデックス
		const MyLib::Vector3 offset = m_pPlayer->GetParameter().ballOffset;	// ボールオフセット
		MyLib::Matrix mtxParts = m_pPlayer->GetModel(nPartsIdx)->GetWorldMtx();	// パーツマトリックス
		MyLib::Vector3 rot = GetRotation();			// 自身の向き
		MyLib::Vector3 scale = GetScale();			// 自身の拡大率
		MyLib::Matrix mtxWorld = GetWorldMtx();		// 自身のワールドマトリックス
		MyLib::Matrix mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス宣言

		// ワールドマトリックスの初期化
		mtxWorld.Identity();

		// スケールを反映する
		mtxScale.Scaling(scale);
		mtxWorld.Multiply(mtxWorld, mtxScale);

		// 向きを反映する
		mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxWorld.Multiply(mtxWorld, mtxRot);

		// 位置を反映する
		mtxTrans.Translation(offset);
		mtxParts.Multiply(mtxTrans, mtxParts);

		// マトリックスを反映
		SetWorldMtx(mtxParts);

		// キャッチ時のマトリックスから位置を反映
		SetPosition(mtxParts.GetWorldPosition());

		// 影の描画を停止
		m_pShadow->SetEnableDisp(false);
	}
	else
	{ // キャッチしていない場合

		// 親クラスのワールドマトリックス計算
		CObjectX::CalWorldMtx();

		// 影の描画を再開
		m_pShadow->SetEnableDisp(true);
	}
}

//==========================================================================
// 生成状態の更新処理
//==========================================================================
void CBall::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);
}

//==========================================================================
// キャッチ状態の更新処理
//==========================================================================
void CBall::UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 通常ホーミング状態の更新処理
//==========================================================================
void CBall::UpdateHomingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量
	MyLib::Vector3 posTarget = m_pTarget->GetCenterPosition();	// ターゲット中心位置

	// 目標ベクトルを取得
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// 差分ベクトルを計算
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// 移動ベクトルを更新
	vecMove.x += vecDiff.x * normal::REV_HOMING;
	vecMove.z += vecDiff.z * normal::REV_HOMING;
	vecMove = vecMove.Normal();

	// 位置に移動量を反映
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 経過時間を加算
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= m_fHomingTime)	// TODO：ここ上に行きすぎたら落ちてくる処理も必要かも
	{
		// 移動状態にする
		SetState(STATE_MOVE);
	}

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// プレイヤーとの当たり判定
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // 当たった場合

		// リバウンド遷移
		ReBound(pHit, &vecMove);
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// ジャンプホーミング状態の更新処理
//==========================================================================
void CBall::UpdateHomingJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量
	MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// ターゲット位置
	MyLib::Vector3 rotTarget = m_pTarget->GetRotation();	// ターゲット向き

	// ターゲット位置のY座標は地面固定にする
	posTarget.y = CGameManager::FIELD_LIMIT;

	// ターゲットの少し後ろにする
	const float fTargetAngle = posTarget.AngleXZ(pos);
	posTarget.x += sinf(fTargetAngle) * jump::OFFSET_TARGET_BACK;
	posTarget.z += cosf(fTargetAngle) * jump::OFFSET_TARGET_BACK;

	// 目標ベクトルを取得
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// 差分ベクトルを計算
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// 移動ベクトルを更新
	vecMove += vecDiff * jump::REV_HOMING;
	if (vecMove.y >= jump::MIN_MOVE_DOWN)
	{
		// 最大値に補正
		vecMove.y = jump::MIN_MOVE_DOWN;
	}

	// ベクトルを正規化
	vecMove = vecMove.Normal();

	// 位置に移動量を反映
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// プレイヤーとの当たり判定
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // 当たった場合

		// リバウンド遷移
		ReBound(pHit, &vecMove);
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// 移動状態の更新処理
//==========================================================================
void CBall::UpdateMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量

	// 位置に移動量を反映
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 重力の加速
	UpdateGravity(fDeltaRate, fSlowRate);

	// 位置に重力反映
	UpdateGravityPosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// プレイヤーとの当たり判定
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // 当たった場合

		// リバウンド遷移
		ReBound(pHit, &vecMove);
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// スペシャル演出状態の更新処理
//==========================================================================
void CBall::UpdateSpecialStag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMotion* pMotion = m_pPlayer->GetMotion();	// プレイヤーモーション情報

	// モーションがスペシャルじゃない場合抜ける
	if (!pMotion->IsSpecial()) { return; }

	if (pMotion->IsImpactFrame(special::TIMING_ATK_IDX))
	{ // 攻撃タイミングが訪れた場合

		// スペシャル投げ
		ThrowSpecial();
	}
}

//==========================================================================
// スペシャル投げ状態の更新処理
//==========================================================================
void CBall::UpdateSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_SpecialFuncList[m_typeSpecial] != nullptr)
	{ // スペシャル更新関数がある場合

		// スペシャル別処理
		(this->*(m_SpecialFuncList[m_typeSpecial]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// パスホーミング状態の更新処理
//==========================================================================
void CBall::UpdateHomingPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 posBall = GetPosition();					// ボール位置
	MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// ターゲット位置
	MyLib::Vector3 vecMove = GetMove();						// 移動量

	// 経過時間を加算
	m_fStateTime += fDeltaTime * fSlowRate;
	if (m_fStateTime <= pass::TIME_HOMING)
	{
		// 経過時間の割合を計算
		float fTimeRate = m_fStateTime / pass::TIME_HOMING;
		fTimeRate = UtilFunc::Transformation::Clamp(fTimeRate, 0.0f, 1.0f);	// 割合を補正

		// XZ平面の位置をターゲット位置と同一にする
		m_posPassEnd.x = posTarget.x;
		m_posPassEnd.z = posTarget.z;

		// パス終了Y座標を更新
		if (m_posPassEnd.y < posTarget.y + pass::TARGET_PULSY && fTimeRate < pass::HOMING_TIMERATE)
		{ // 現在のパス終了Y座標がターゲット位置より低い且つ、経過時間の割合が前半の場合

			// 現在のターゲット位置に再設定
			m_posPassEnd.y = posTarget.y + pass::TARGET_PULSY;
		}

		// 放物線上に位置を補正
		posBall = UtilFunc::Calculation::GetParabola3D(m_posPassStart, m_posPassEnd, pass::MAX_HEIGHT, fTimeRate);
	}
	else
	{
		// 重力の加速
		UpdateGravity(fDeltaRate, fSlowRate);

		// 位置に重力反映
		UpdateGravityPosition(&posBall, &vecMove, fDeltaRate, fSlowRate);
	}

	// 地面の着地
	if (UpdateLanding(&posBall, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// プレイヤーとの当たり判定
	CollisionPlayer(&posBall);

	// 情報を反映
	SetPosition(posBall);	// 位置
	SetMove(vecMove);		// 移動量
}

//==========================================================================
// パス状態の更新処理
//==========================================================================
void CBall::UpdatePass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// ボール位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量

	// 経過時間を加算
	m_fStateTime += fDeltaTime * fSlowRate;
	if (m_fStateTime <= pass::TIME_NORAML)
	{
		// 経過時間の割合を計算
		float fTimeRate = m_fStateTime / pass::TIME_NORAML;
		fTimeRate = UtilFunc::Transformation::Clamp(fTimeRate, 0.0f, 1.0f);	// 割合を補正

		// 放物線上に位置を補正
		pos = UtilFunc::Calculation::GetParabola3D(m_posPassStart, m_posPassEnd, pass::MAX_HEIGHT, fTimeRate);
	}
	else
	{
		// 重力の加速
		UpdateGravity(fDeltaRate, fSlowRate);

		// 位置に重力反映
		UpdateGravityPosition(&pos, &vecMove, fDeltaRate, fSlowRate);
	}

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// リバウンド状態の更新処理
//==========================================================================
void CBall::UpdateReBound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// フリー状態の更新処理
//==========================================================================
void CBall::UpdateFree(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 地面の着地
	UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate);

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// 着地状態の更新処理
//==========================================================================
void CBall::UpdateLand(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);

	// 地面の着地
	UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate);

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// かめはめ波の更新処理
//==========================================================================
void CBall::UpdateKamehameha(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量
	MyLib::Vector3 posTarget = m_pTarget->GetCenterPosition();	// ターゲット中心位置
	MyLib::Vector3 rotTarget = m_pTarget->GetRotation();		// ターゲット向き

	// 目標ベクトルを取得
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// 差分ベクトルを計算
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// 移動ベクトルを更新
	vecMove += vecDiff * kamehameha::REV_HOMING;

	// ベクトルを正規化
	vecMove = vecMove.Normal();

	// 位置に移動量を反映
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

#if 0
	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}
#endif

	// プレイヤーとの当たり判定
	CPlayer* pHit = CollisionPlayer(&pos);
	if (pHit != nullptr)
	{ // 当たった場合

		// リバウンド遷移
		ReBound(pHit, &vecMove);
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// 重力の加速処理
//==========================================================================
void CBall::UpdateGravity(const float fDeltaRate, const float fSlowRate)
{
	// 重力を与える
	m_fGravity -= GRAVITY * fDeltaRate * fSlowRate;
}

//==========================================================================
// 移動量の減速処理
//==========================================================================
void CBall::UpdateDecay(const float fDeltaRate, const float fSlowRate)
{
	// 移動量を減衰させる
	m_fMoveSpeed += (0.0f - m_fMoveSpeed) * (REV_MOVE * fDeltaRate * fSlowRate);
}

//==========================================================================
// 位置に重力を反映する処理
//==========================================================================
void CBall::UpdateGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// 位置に重力を反映
	pPos->y += m_fGravity * fDeltaRate * fSlowRate;
}

//==========================================================================
// 位置に移動量を反映する処理
//==========================================================================
void CBall::UpdateMovePosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// 位置に移動量を反映
	*pPos += (*pMove * (m_fMoveSpeed + m_fInitialSpeed)) * fDeltaRate * fSlowRate;

	// 場外の補正
	CGameManager::GetInstance()->SetPosLimit(*pPos, 60.0f);	// TODO：いつかボールは転がってどっかにいくよ
}

//==========================================================================
// 移動処理
//==========================================================================
void CBall::UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// 重力の加速
	UpdateGravity(fDeltaRate, fSlowRate);

	// 位置に重力反映
	UpdateGravityPosition(pPos, pMove, fDeltaRate, fSlowRate);

	// 位置に移動量反映
	UpdateMovePosition(pPos, pMove, fDeltaRate, fSlowRate);

	// 移動量の減速
	UpdateDecay(fDeltaRate, fSlowRate);
}

//==========================================================================
// 地面の着地処理
//==========================================================================
bool CBall::UpdateLanding(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	if (pPos->y - RADIUS < CGameManager::FIELD_LIMIT)
	{ // ボールが地面に埋もれている場合

		// ボールの位置を補正
		pPos->y = CGameManager::FIELD_LIMIT + RADIUS;

		// 縦移動量を与える
		pMove->y = UtilFunc::Transformation::Clamp(m_fMoveSpeed, 0.0f, MAX_BOUND_MOVE);

		// 初速を初期化
		m_fInitialSpeed = 0.0f;

		// 重力を初期化
		m_fGravity = 0.0f;

		// 着地している状態にする
		m_bLanding = true;

		// サウンド再生
		// TODO : 値の調整と定数化する
		if (!UtilFunc::Calculation::IsNearlyTarget(m_fMoveSpeed, 0.0f, 0.75f))
		{
			// 割合
			float ratio = UtilFunc::Transformation::Clamp(fabsf(m_fMoveSpeed) / 5.0f, 0.0f, 1.0f);

			if (ratio > 0.5f)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
			}
			else if (ratio <= 0.5f && ratio > 0.2f)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_MEDIUM);
			}
			else
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_LOW);
			}
		}
		return true;
	}

	return false;
}

//==========================================================================
// プレイヤーとの当たり判定
//==========================================================================
CPlayer* CBall::CollisionPlayer(MyLib::Vector3* pPos)
{
	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	const float fCollRadius = GetCollRadius();			// 判定ボール半径
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		// 円と円柱の当たり判定
		bool bHit = UtilFunc::Collision::CollisionCircleCylinder
		( // 引数
			*pPos,
			pPlayer->GetPosition(),
			fCollRadius,
			pPlayer->GetRadius(),
			pPlayer->GetParameter().fHeight
		);
		if (bHit)
		{ // 当たっていた場合

			// プレイヤーヒット処理
			if (pPlayer->Hit(this).bHit)
			{ // ヒット処理内でダメージを受けた場合

				// ホーミング対象の初期化
				m_pTarget = nullptr;

				// ヒットしたプレイヤーを返す
				return pPlayer;
			}
		}
	}

	return nullptr;
}

//==========================================================================
// 投げる対象との当たり判定
//==========================================================================
CPlayer* CBall::CollisionThrowTarget(const bool bAbsLock)
{
	// 持っていたプレイヤーが初期化済みの場合エラー
	assert(m_pPlayer != nullptr);

	// 持っていたプレイヤーのチームが初期化済みの場合エラー
	assert(m_typeTeam != CGameManager::SIDE_NONE);

	MyLib::Vector3 posBall = GetPosition();						// ボール位置
	MyLib::Vector3 posThrow = m_pPlayer->GetCenterPosition();	// 投げたプレイヤーの位置
	MyLib::Vector3 rotThrow = m_pPlayer->GetRotation();			// 投げたプレイヤーの向き

	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = MAX_DIS;	// ボールから近いプレイヤー

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// 同じチームの場合次へ
		if (m_typeTeam == pPlayer->GetTeam()) { continue; }

		// ポジションが外野の場合次へ
		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) { continue; }

		// 視界内にいない場合次へ
		bool bHit = UtilFunc::Collision::CollisionViewRange3D(posThrow, posPlayer, rotThrow.y, VIEW_ANGLE);
		if (!bHit && !bAbsLock) { continue; }

		// プレイヤー視野中心ベクトルからの距離測定
		MyLib::Vector3 posFront = posThrow + MyLib::Vector3(sinf(rotThrow.y), 0.0f, cosf(rotThrow.y));
		float fCurDis = fabsf(UtilFunc::Collision::LineOuterProduct(posThrow, posFront, posPlayer));
		if (fCurDis < fMinDis)
		{ // よりボールに近いプレイヤーがいた場合

			// 距離を保存
			fMinDis = fCurDis;

			// ターゲットを更新
			pTarget = pPlayer;
		}
	}

	// 投擲ターゲットを返す
	return pTarget;
}

//==========================================================================
// パスする対象との当たり判定
//==========================================================================
CPlayer* CBall::CollisionPassTarget()
{
	// 持っていたプレイヤーが初期化済みの場合エラー
	assert(m_pPlayer != nullptr);

	// 持っていたプレイヤーのチームが初期化済みの場合エラー
	assert(m_typeTeam != CGameManager::SIDE_NONE);

	MyLib::Vector3 posBall = GetPosition();						// ボール位置
	MyLib::Vector3 posThrow = m_pPlayer->GetCenterPosition();	// 投げたプレイヤーの位置
	MyLib::Vector3 rotThrow = m_pPlayer->GetRotation();			// 投げたプレイヤーの向き
	CPlayer::EFieldArea typeArea = m_pPlayer->GetAreaType();	// 投げたプレイヤーのポジション

	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = MAX_DIS;	// ボールから近いプレイヤー

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// 違うチームの場合次へ
		if (m_typeTeam != pPlayer->GetTeam()) { continue; }

		// 自分と同じポジションの場合次へ
		if (typeArea == pPlayer->GetAreaType()) { continue; }

		// 視界内にいない場合次へ	// TODO
#if 0
		bool bHit = UtilFunc::Collision::CollisionViewRange3D(posThrow, posPlayer, rotThrow.y, VIEW_ANGLE);
		if (!bHit) { continue; }
#endif

		// プレイヤー視野中心ベクトルからの距離測定
		MyLib::Vector3 posFront = posThrow + MyLib::Vector3(sinf(rotThrow.y), 0.0f, cosf(rotThrow.y));
		float fCurDis = fabsf(UtilFunc::Collision::LineOuterProduct(posThrow, posFront, posPlayer));
		if (fCurDis < fMinDis)
		{ // よりボールに近いプレイヤーがいた場合

			// 距離を保存
			fMinDis = fCurDis;

			// ターゲットを更新
			pTarget = pPlayer;
		}
	}

	// 投擲ターゲットを返す
	return pTarget;
}

//==========================================================================
// 判定半径取得
//==========================================================================
float CBall::GetCollRadius()
{
	if (m_state == EState::STATE_HOM_PASS
	||  m_state == EState::STATE_PASS
	||  m_state == EState::STATE_REBOUND
	||  m_state == EState::STATE_FREE
	||  m_state == EState::STATE_LAND)
	{ // 判定がでかい方がよさげな状態の場合

		return PLUS_RADIUS;
	}

	return RADIUS;
}

//==========================================================================
// 状態設定
//==========================================================================
void CBall::SetState(const EState state)
{
	// 引数の状態にする
	m_state = state;

	// カウンターを初期化
	m_fStateTime = 0.0f;

	// カバー対象プレイヤーの初期化
	m_pCover = nullptr;
}

//==========================================================================
// キャッチ処理
//==========================================================================
void CBall::Catch(CPlayer* pPlayer)
{
#ifdef CHANGE
	if (IsPass() && m_pTarget != pPlayer)
	{ // パス状態且つ、ターゲット以外がキャッチした場合

		// 元ターゲットをAIに戻す
		CPlayerManager::GetInstance()->ChangeUserToAI(m_pTarget);
	}
#endif

	// 移動量を初期化
	SetMove(VEC3_ZERO);

	// キャッチ状態にする
	SetState(STATE_CATCH);

	// プレイヤーのチームを保存
	m_typeTeam = pPlayer->GetTeam();

	// ホーミング対象の初期化
	m_pTarget = nullptr;

	// キャッチしたプレイヤーを保存
	m_pPlayer = pPlayer;

	// キャッチしたプレイヤーを所持マーカーに割当
	m_pHoldMarker->BindPlayer(pPlayer);

#ifdef CHANGE
	// キャッチしたAIに操作権を移す
	CPlayerManager::GetInstance()->ChangeAIToUser(pPlayer);
#endif

	// プレイヤーにボールを保存
	pPlayer->SetBall(this);
}

//==========================================================================
// 投げ処理
//==========================================================================
void CBall::Throw(CPlayer* pPlayer)
{
	// 持っていたプレイヤーと違う場合エラー
	assert(m_pPlayer == pPlayer);

	// キャッチしていたプレイヤーを破棄
	m_pPlayer = nullptr;

	// プレイヤーから保存中のボールを破棄
	pPlayer->SetBall(nullptr);

	// 所持マーカーからプレイヤーを破棄
	m_pHoldMarker->BindPlayer(nullptr);

	// ボールの移動ベクトルを作成
	float fRotY = pPlayer->GetRotation().y + D3DX_PI;	// ボールの投げる向き
	MyLib::Vector3 vecMove = MyLib::Vector3(sinf(fRotY), 0.0f, cosf(fRotY));	// 移動ベクトル

	// 移動ベクトルを正規化して設定
	SetMove(vecMove.Normal());

	// 初速を初期化
	m_fInitialSpeed = 0.0f;

	// 重力を初期化
	m_fGravity = 0.0f;
}

//==========================================================================
// スペシャル投げ処理
//==========================================================================
void CBall::ThrowSpecial()
{
	// スペシャル演出を通過していないボールを投げようとした場合エラー
	assert(m_state == STATE_SPECIAL_STAG);

	// スペシャル投げ状態にする
	SetState(STATE_SPECIAL_THROW);

	MyLib::Vector3 posPlayer = m_pPlayer->GetPosition();	// ボール過去位置
	MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// プレイヤー位置
	float fAngleY = posPlayer.AngleXZ(posTarget);			// ボール方向

	// 目標向き/向きをボール方向にする
	m_pPlayer->SetRotDest(fAngleY);
	m_pPlayer->SetRotation(MyLib::Vector3(0.0f, fAngleY, 0.0f));

	// 投げ処理
	Throw(m_pPlayer);

	// スペシャル攻撃を設定
	m_typeAtk = ATK_SPECIAL;

	// 移動量を設定
	m_fMoveSpeed = special::THROW_MOVE;

	// 初速を設定
	CalcSetInitialSpeed(m_fMoveSpeed);
}

//==========================================================================
// 着地処理
//==========================================================================
void CBall::Landing()
{
	// 着地状態にする
	SetState(STATE_LAND);

	// ホーミング対象の初期化
	m_pTarget = nullptr;
}

//==========================================================================
// チームサイドの更新処理
//==========================================================================
void CBall::UpdateTypeTeam()
{
	// 攻撃判定がある場合は現在のチームを保持
	if (IsAttack()) { return; }

	// パスボールの場合も現在のチームを保持
	if (IsPass()) { return; }

	// フリーボールの場合も現在のチームを保持
	if (m_state == STATE_FREE) { return; }

	// プレイヤーが所持していない場合チーム指定なし
	if (m_pPlayer == nullptr) { m_typeTeam = CGameManager::SIDE_NONE; return; }

	// プレイヤーのチームを保存
	m_typeTeam = m_pPlayer->GetTeam();

	// パス開始/終了位置をリセット
	m_posPassStart	= VEC3_ZERO;
	m_posPassEnd	= VEC3_ZERO;
}

//==========================================================================
// 攻撃種類の更新処理
//==========================================================================
void CBall::UpdateTypeAtk()
{
	// 攻撃判定がある場合は現在の攻撃種類を保持
	if (IsAttack()) { return; }

	// 攻撃種類を破棄
	m_typeAtk = ATK_NONE;

	// 情報をリセット
	m_nDamage = 0;			// ダメージ量
	m_fKnockback = 0.0f;	// ノックバック
	m_fHomingTime = 0.0f;	// ホーミング時間
}

//==========================================================================
// スペシャル種類の更新処理
//==========================================================================
void CBall::UpdateTypeSpecial()
{
	// スペシャル中の場合は現在のスペシャルを保持
	if (IsSpecial()) { return; }

	// スペシャルを破棄
	m_typeSpecial = SPECIAL_NONE;
}

//==========================================================================
// リバウンド処理
//==========================================================================
void CBall::ReBound(CPlayer* pHitPlayer, MyLib::Vector3* pMove)
{
	// 移動ベクトルを反転
	*pMove = pMove->Invert();

	// 上移動量を追加
	pMove->y = rebound::MOVE_UP;

	// 初速を初期化
	m_fInitialSpeed = 0.0f;

	// 移動速度を低下
	m_fMoveSpeed = rebound::MOVE_SPEED;

	// リバウンド状態にする
	SetState(STATE_REBOUND);

	// カバー対象プレイヤーを保存
	m_pCover = pHitPlayer;
}

//==========================================================================
// 初速の計算処理
//==========================================================================
void CBall::CalcSetInitialSpeed(const float fMove)
{
	// 初速を与える
	m_fInitialSpeed = fMove * move::MULTIPLY_INIMOVE;
}

//==========================================================================
// 移動ベクトルの計算
//==========================================================================
MyLib::Vector3 CBall::CalcVecMove(CPlayer* pTarget, CPlayer* pPlayer)
{
	const float fThrowMove = pPlayer->GetBallParameter().fThrowMoveNormal;	// 通常投げ速度

	// ボールが敵に到達するまでの時間計算
	SHitTimingInfo info = CalcHitSpeedTime
	( // 引数
		pTarget->GetPosition(),	// ターゲット位置
		GetPosition(),			// ボール位置
		pPlayer->GetRadius(),	// ターゲット半径
		fThrowMove * move::MULTIPLY_INIMOVE,	// 初速
		fThrowMove								// 移動量
	);

	// ホーミング時間の保存
	m_fHomingTime = info.fHitTime;

	// ボールが当たらなかった場合現在の位置へのベクトルを返す
	if (!info.bHit) { return pTarget->GetPosition() + pTarget->GetLookOffset() - GetPosition(); }

	// 未来位置へのベクトルを返す
	return pTarget->CalcFuturePosition(info.nHitFrame) - GetPosition();
}

//==========================================================================
// ボールが敵に到達するまでの時間計算
//==========================================================================
CBall::SHitTimingInfo CBall::CalcHitSpeedTime
(
	const MyLib::Vector3& rPosTarget,	// ターゲット位置
	const MyLib::Vector3& rPosBall,		// ボール位置
	const float fRadiusTarget,			// ターゲット半径
	const float fInitSpeed,				// 初速
	const float fMoveSpeed				// 移動量
)
{
	// ボール移動ベクトルの作成
	MyLib::Vector3 vecMove = rPosTarget - rPosBall;
	vecMove = vecMove.Normal();	// ベクトルの正規化

	const float fFrameTime = 1.0f / (float)GetFPS();	// 想定1F時間
	float fNextInitSpeed = fInitSpeed;		// 次の初速
	MyLib::Vector3 nextPosBall = rPosBall;	// 次のボール位置

	int nHitFrame = 0;		// ヒットまでにかかるフレーム数
	float fHitTime = 0.0f;	// ヒットまでにかかる時間
	bool bHit = false;		// ヒットフラグ
	while (fHitTime < normal::TIME_HOMING)
	{ // ホーミングの切れる最大時間まで繰り返す

		// 位置を移動させる
		nextPosBall += (vecMove * (fMoveSpeed + fNextInitSpeed));

		// ボールが当たった場合抜ける
		if (UtilFunc::Collision::CircleRange3D(rPosTarget, nextPosBall, fRadiusTarget, RADIUS)) { bHit = true; break; }

		// 初速を減衰させる
		fNextInitSpeed += (0.0f - fNextInitSpeed) * REV_INIMOVE;

		// 1F分の時間を進める
		fHitTime += fFrameTime;

		// フレーム数を進める
		nHitFrame++;
	}

	// 当たったタイミングの情報を作成し返す
	SHitTimingInfo info;
	info.nHitFrame = nHitFrame;
	info.fHitTime = fHitTime;
	info.bHit = bHit;
	return info;
}

//==========================================================================
// 投げた線の更新
//==========================================================================
void CBall::UpdateThrowLine()
{
	// オーラ更新
	if (m_pAura != nullptr)
	{
		// ワールドマトリックス計算
		CalWorldMtx();

		// 位置設定
		m_pAura->SetPosition(GetWorldMtx().GetWorldPosition());
	}

	if (IsAttack())
	{// 攻撃中

		// 情報を取得
		MyLib::Vector3 pos = GetPosition();	// 位置
		MyLib::Vector3 vecMove = GetMove();	// 移動量

		// Y軸の向き
		MyLib::Vector3 rot;
		rot.y = vecMove.AngleXZ(0.0f) - D3DX_PI * 0.5f;

		// Z軸の向き
		rot.z = vecMove.y;

		// 投げのライン
		if (m_pThrowLine == nullptr)
		{
			switch (GetTypeAtk())
			{
			case ATK_NORMAL:	// 通常攻撃

				m_pThrowLine = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_THROWLINE_NORMAL,
					GetPosition(),
					rot,
					MyLib::Vector3(),
					8.0f);
				break;
				
			case ATK_JUMP:	// ジャンプ攻撃

				m_pThrowLine = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_THROWLINE_FAST,
					GetPosition(),
					rot,
					MyLib::Vector3(),
					8.0f);
				break;

			default:

				m_pThrowLine = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_THROWLINE_FAST,
					GetPosition(),
					rot,
					MyLib::Vector3(),
					8.0f);

				break;
			}
		}
		m_pThrowLine->SetPosition(pos);
		m_pThrowLine->SetRotation(rot);
	}
	else if(m_pThrowLine != nullptr)
	{// 投げ終わり

		// 停止
		if (m_pThrowLine != nullptr)
		{
			m_pThrowLine->SetTrigger(0);
			m_pThrowLine->SetTrigger(1);
			m_pThrowLine = nullptr;
		}
	}
}

//==========================================================================
// 
//  ボール処理 [ball.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "ball.h"
#include "player.h"
#include "playerStatus.h"
#include "manager.h"
#include "game.h"
#include "gamemanager.h"
#include "calculation.h"
#include "model.h"
#include "shadow.h"
#include "specialManager.h"

#include "debugproc.h"
#include "3D_Effect.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char*	MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// ボールモデル
	const float	RADIUS = 14.0f;			// 半径
	const float	RADIUS_SHADOW = 24.0f;	// 影の半径
	const float	REV_MOVE = 0.025f;		// 移動量の補正係数
	const float	REV_INIMOVE = 0.29f;	// 初速の補正係数
	const float	MAX_DIS = 100000.0f;	// ホーミングする最大距離
	const int	VIEW_ANGLE = 104;		// 視野角
	const float DEST_POSY = 45.0f;		// 通常ボールの目標Y座標
	const float REV_POSY = 0.1f;		// 通常ボールの目標Y座標の補正係数
	const float GRAVITY = mylib_const::GRAVITY * 0.6f;	// ボールにかかる重力
	const float MAX_BOUND_MOVE = 1.0f;	// バウンド時の上移動量最大値

	const char* DEBUG_STATE_PRINT[] =	// デバッグ表示用状態
	{
		"[SPAWN]    生成状態 (フリーボール)",
		"[CATCH]    キャッチ状態 (プレイヤー所持)",
		"[HOM_NOR]  通常ホーミング状態 (攻撃判定ON)",
		"[HOM_JUMP] ジャンプホーミング状態 (攻撃判定ON)",
		"[MOVE]     移動状態 (攻撃判定ON)",
		"[S_STAG]   スペシャル演出状態 (開始前演出)",
		"[S_THROW]  スペシャル投げ状態 (攻撃判定ON)",
		"[REBOUND]  リバウンド状態 (ぶつかった時の落下)",
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

	namespace normal
	{
		const float THROW_MOVE = 19.5f;	// 通常投げ移動速度
		const float REV_HOMING = 0.3f;	// ホーミングの慣性補正係数
		const float TIME_HOMING = 1.2f;	// ホーミングが切れるまでの時間
	}

	namespace jump
	{
		const float THROW_MOVE = 24.0f;		// ジャンプ投げ移動速度
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
		const float MULTIPLY_INIMOVE = 3.5f;	// 初速の倍率
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
	&CBall::UpdateReBound,		// リバウンド状態の更新
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
	m_pShadow		(nullptr),		// 影情報
	m_pPlayer		(nullptr),		// プレイヤー情報
	m_pTarget		(nullptr),		// ホーミングターゲット情報
	m_pCover		(nullptr),		// カバー対象プレイヤー情報
	m_fMoveSpeed	(0.0f),			// 移動速度
	m_fGravity		(0.0f),			// 重力
	m_typeSpecial	(SPECIAL_NONE),	// スペシャル種類
	m_typeAtk		(ATK_NONE),		// 攻撃種類
	m_state			(STATE_SPAWN),	// 状態
	m_fStateTime	(0.0f)			// 状態カウンター
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_StateFuncList)   == CBall::STATE_MAX,   "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_SpecialFuncList) == CBall::SPECIAL_MAX, "ERROR : Special Count Mismatch");

	static_assert(NUM_ARRAY(DEBUG_STATE_PRINT)   == CBall::STATE_MAX,       "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(DEBUG_TEAM_PRINT)    == CGameManager::SIDE_MAX, "ERROR : Team Count Mismatch");
	static_assert(NUM_ARRAY(DEBUG_ATK_PRINT)     == CBall::ATK_MAX,         "ERROR : Attack Count Mismatch");
	static_assert(NUM_ARRAY(DEBUG_SPECIAL_PRINT) == CBall::SPECIAL_MAX,     "ERROR : Special Count Mismatch");
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

	// 影の生成
	m_pShadow = CShadow::Create(this, RADIUS_SHADOW);
	if (m_pShadow == nullptr) { return E_FAIL; }
	m_pShadow->SetAlpha(0.8f);	// 色

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
	m_pShadow->Kill();

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
		"【 所有対象 】[%s]\n"
		"【ターゲット】[%s]\n"
		"【カバー対象】[%s]\n",
		DEBUG_STATE_PRINT[m_state],
		DEBUG_TEAM_PRINT[m_typeTeam],
		DEBUG_ATK_PRINT[m_typeAtk],
		DEBUG_SPECIAL_PRINT[m_typeSpecial],
		(m_pPlayer == nullptr) ? "nullptr" : "player",
		(m_pTarget == nullptr) ? "nullptr" : "player",
		(m_pCover  == nullptr) ? "nullptr" : "player"
	);
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
// 通常投げ処理
//==========================================================================
void CBall::ThrowNormal(CPlayer* pPlayer)
{
	// キャッチしていないボールを投げようとした場合エラー
	assert(m_state == STATE_CATCH);

	// ホーミング対象の設定
	m_pTarget = CollisionThrow();
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
	}
	else
	{ // ターゲットがいない場合

		// 移動状態にする
		SetState(STATE_MOVE);
	}

	// 投げ処理
	Throw(pPlayer);

	// 通常攻撃を設定
	m_typeAtk = ATK_NORMAL;

	// 移動量を設定
	m_fMoveSpeed = normal::THROW_MOVE;

	// 初速を設定
	CalcSetInitialSpeed(m_fMoveSpeed);
}

//==========================================================================
// ジャンプ投げ処理
//==========================================================================
void CBall::ThrowJump(CPlayer* pPlayer)
{
	// キャッチしていないボールを投げようとした場合エラー
	assert(m_state == STATE_CATCH);

	// ホーミング対象の設定
	m_pTarget = CollisionThrow();
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
	}
	else
	{ // ターゲットがいない場合

		// 移動状態にする
		SetState(STATE_MOVE);
	}

	// 投げ処理
	Throw(pPlayer);

	// ジャンプ攻撃を設定
	m_typeAtk = ATK_JUMP;

	// 移動量を設定
	m_fMoveSpeed = jump::THROW_MOVE;

	// 初速を設定
	CalcSetInitialSpeed(m_fMoveSpeed);
}

//==========================================================================
// スペシャル発動処理
//==========================================================================
void CBall::Special(CPlayer* pPlayer)
{
	// ホーミング対象の設定
	m_pTarget = CollisionThrow(true);
	assert(m_pTarget != nullptr);	// 見つからなきゃ敵がいないよ～

	// スペシャル演出状態にする
	SetState(STATE_SPECIAL_STAG);

	// TODO：スペシャルの種類設定
	m_typeSpecial = SPECIAL_KAMEHAMEHA;

	// スペシャル演出マネージャーの生成
	CSpecialManager::Create(m_pPlayer, m_pTarget);
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
		MyLib::Matrix* pMtxParts = m_pPlayer->GetModel(nPartsIdx)->GetPtrWorldMtx();	// パーツマトリックス
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
		mtxWorld.Multiply(mtxWorld, mtxTrans);

		// くっつけるプレイヤーパーツのマトリックスと掛け合わせる
		mtxWorld.Multiply(*pMtxParts, mtxWorld);

		// マトリックスを反映
		SetWorldMtx(*pMtxParts);

		// キャッチ時のマトリックスから位置を反映
		SetPosition(pMtxParts->GetWorldPosition());

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

	// Y座標を慣性補正
	UtilFunc::Correction::InertiaCorrection(pos.y, DEST_POSY, REV_POSY);

	// 経過時間を加算
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= normal::TIME_HOMING)
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

	// 経過時間を加算
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= move::TIME_GRAVITY)
	{
		// 重力の加速
		UpdateGravity(fDeltaRate, fSlowRate);

		// 位置に重力反映
		UpdateGravityPosition(&pos, &vecMove, fDeltaRate, fSlowRate);
	}
	else
	{
		// Y座標を慣性補正
		UtilFunc::Correction::InertiaCorrection(pos.y, DEST_POSY, REV_POSY);
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
	CGame::GetInstance()->GetGameManager()->SetPosLimit(*pPos);
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
		pMove->y = m_fMoveSpeed;

		// 上限に補正
		UtilFunc::Transformation::ValueNormalize(pMove->y, MAX_BOUND_MOVE, 0.0f);

		// 初速を初期化
		m_fInitialSpeed = 0.0f;

		// 重力を初期化
		m_fGravity = 0.0f;
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
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		// 同じチームの場合次へ
		if (pPlayer->GetStatus()->GetTeam() == m_typeTeam) { continue; }

		bool bHit = UtilFunc::Collision::CollisionCircleCylinder
		( // 引数
			*pPos,
			pPlayer->GetPosition(),
			RADIUS,
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
// ホーミング対象との当たり判定
//==========================================================================
CPlayer* CBall::CollisionThrow(const bool bAbsLock)
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
		if (m_typeTeam == pPlayer->GetStatus()->GetTeam()) { continue; }

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
	// 移動量を初期化
	SetMove(VEC3_ZERO);

	// キャッチ状態にする
	SetState(STATE_CATCH);

	// プレイヤーのチームを保存
	m_typeTeam = pPlayer->GetStatus()->GetTeam();

	// ホーミング対象の初期化
	m_pTarget = nullptr;

	// キャッチしたプレイヤーを保存
	m_pPlayer = pPlayer;

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

	// ボールの移動ベクトルを作成
	float fRotY = pPlayer->GetRotation().y + D3DX_PI;	// ボールの投げる向き
	MyLib::Vector3 vecMove = MyLib::Vector3(sinf(fRotY), 0.0f, cosf(fRotY));	// 移動ベクトル

	// 移動ベクトルを正規化して設定
	SetMove(vecMove.Normal());
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

	// プレイヤーが所持していない場合チーム指定なし
	if (m_pPlayer == nullptr) { m_typeTeam = CGameManager::SIDE_NONE; return; }

	// プレイヤーのチームを保存
	m_typeTeam = m_pPlayer->GetStatus()->GetTeam();
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

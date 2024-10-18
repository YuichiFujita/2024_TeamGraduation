//=============================================================================
// 
//  ボール処理 [ball.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "ball.h"
#include "player.h"
#include "playerStatus.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char*	MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// ボールモデル
	const float	RADIUS = 7.0f;			// 半径
	const float	REV_MOVE = 0.025f;		// 移動量の補正係数
	const float	MAX_DIS = 100000.0f;	// ホーミングする最大距離
	const int	VIEW_ANGLE = 135;		// 視野角

	const char* DEBUG_STATE_PRINT[] =	// デバッグ表示用状態
	{
		"SPAWN   生成状態       (フリーボール)",
		"CATCH   キャッチ状態   (プレイヤー所持)",
		"HOMING  ホーミング状態 (攻撃判定ON)",
		"MOVE    移動状態       (攻撃判定ON)",
		"REBOUND リバウンド状態 (ぶつかった時の落下)",
		"LAND    着地状態       (地面落下)",
	};
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CBall::STATE_FUNC CBall::m_SampleFuncList[] =
{
	&CBall::UpdateSpawn,	// 生成状態の更新
	&CBall::UpdateCatch,	// キャッチ状態の更新
	&CBall::UpdateHoming,	// ホーミング状態の更新
	&CBall::UpdateMove,		// 移動状態の更新
	&CBall::UpdateReBound,	// リバウンド状態の更新
	&CBall::UpdateLand,		// 着地状態の更新
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CBall> CBall::m_list = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CBall::CBall(int nPriority) : CObjectX(nPriority),
	m_typeTeam	 (CGameManager::SIDE_NONE),	// チームサイド
	m_pPlayer	 (nullptr),		// プレイヤー情報
	m_pTarget	 (nullptr),		// ホーミングターゲット情報
	m_fMoveSpeed (0.0f),		// 移動速度
	m_fGravity	 (0.0f),		// 重力
	m_typeAtk	 (ATK_NONE),	// 攻撃種類
	m_state		 (STATE_SPAWN),	// 状態
	m_fStateTime (0.0f)			// 状態カウンター
{

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
	// 状態別処理
	(this->*(m_SampleFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);

	// 親クラスの更新
	CObjectX::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ボールの状態表示
	GET_MANAGER->GetDebugProc()->Print("ボール状態：%s\n", DEBUG_STATE_PRINT[m_state]);
	GET_MANAGER->GetDebugProc()->Print("ターゲット：%s\n", (m_pTarget == nullptr) ? "nullptr" : "player");
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
// キャッチ処理
//==========================================================================
void CBall::Catch(CPlayer* pPlayer)
{
	// 移動量を初期化
	SetMove(VEC3_ZERO);

	// キャッチ状態にする
	m_state = STATE_CATCH;

	// プレイヤーのチームを保存
	m_typeTeam = pPlayer->GetStatus()->GetTeam();

	// キャッチしたプレイヤーを保存
	m_pPlayer = pPlayer;

	// プレイヤーにボールを保存
	pPlayer->SetBall(this);
}

//==========================================================================
// 通常投げ処理
//==========================================================================
void CBall::ThrowNormal(CPlayer* pPlayer)
{
	// 投げ処理
	Throw(pPlayer);

	// 通常攻撃を設定
	m_typeAtk = ATK_NORMAL;

	// 移動量を設定
	m_fMoveSpeed = 11.5f;

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec = vec.Normal();
	SetMove(vec);
}

//==========================================================================
// ジャンプ投げ処理
//==========================================================================
void CBall::ThrowJump(CPlayer* pPlayer)
{
	// 投げ処理
	Throw(pPlayer);

	// ジャンプ攻撃を設定
	m_typeAtk = ATK_JUMP;

	// 移動量を設定
	m_fMoveSpeed = 13.0f;

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec = vec.Normal();
	SetMove(vec);
}

//==========================================================================
// スペシャル投げ処理
//==========================================================================
void CBall::ThrowSpecial(CPlayer* pPlayer)
{
	// 投げ処理
	Throw(pPlayer);

	// スペシャル攻撃を設定
	m_typeAtk = ATK_SPECIAL;

	// 移動量を設定
	m_fMoveSpeed = 60.0f;

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec = vec.Normal();
	SetMove(vec);
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
	return (m_state == STATE_HOMING || m_state == STATE_MOVE);	// TODO：攻撃状態が増えたら追加
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
	// TODO：今後手の位置に親子付け
	SetPosition(m_pPlayer->GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));	// プレイヤー情報
}

//==========================================================================
// ホーミング状態の更新処理
//==========================================================================
void CBall::UpdateHoming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量
	MyLib::Vector3 posTarget = m_pTarget->GetCenterPosition();	// ターゲット位置

	// 目標ベクトルを取得
	MyLib::Vector3 vecDest = posTarget - pos;
	vecDest = vecDest.Normal();

	// 差分ベクトルを計算
	MyLib::Vector3 vecDiff = vecDest - vecMove;
	vecDiff = vecDiff.Normal();

	// 移動ベクトルを更新
	vecMove.x += vecDiff.x * 0.3f;
	vecMove.z += vecDiff.z * 0.3f;
	vecMove = vecMove.Normal();

	// 位置に移動量を反映
	UpdateMovePosition(&pos, &vecMove, fDeltaRate, fSlowRate);

	MyLib::Vector3 vecTarget = posTarget - pos;
	if (vecTarget.Length() <= 80.0f)
	{ // 目標ベクトル (ターゲット位置とボール位置のベクトル) の長さが短い場合

		// 移動状態にする
		m_state = STATE_MOVE;
	}

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 着地遷移
		Landing();
	}

	// プレイヤーとの当たり判定
	if (CollisionPlayer(&pos))
	{ // 当たった場合

		// リバウンド遷移
		ReBound(&vecMove);
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

	// 移動量の減速
	UpdateDecay(fDeltaRate, fSlowRate);

	if (m_fMoveSpeed <= 5.5f)
	{ // 移動量が下がった場合

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
	if (CollisionPlayer(&pos))
	{ // 当たった場合

		// リバウンド遷移
		ReBound(&vecMove);
	}

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
// 重力の加速処理
//==========================================================================
void CBall::UpdateGravity(const float fDeltaRate, const float fSlowRate)
{
	// 重力を与える
	m_fGravity -= (mylib_const::GRAVITY * 0.6f) * fDeltaRate * fSlowRate;
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
	*pPos += (*pMove * m_fMoveSpeed) * fDeltaRate * fSlowRate;
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

		// 縦移動量を初期化	// TODO：後でバウンドするよう変更
		pMove->y = 0.0f;

		// 重力を初期化
		m_fGravity = 0.0f;
		return true;
	}

	return false;
}

//==========================================================================
// プレイヤーとの当たり判定
//==========================================================================
bool CBall::CollisionPlayer(MyLib::Vector3* pPos)
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
			pPlayer->GetHeight()
		);
		if (bHit)
		{ // 当たっていた場合

			// プレイヤーヒット処理
			if (pPlayer->Hit(this)) { return true; }	// ヒット処理内でダメージを受けた場合
		}
	}

	return false;
}

//==========================================================================
// ホーミング対象との当たり判定
//==========================================================================
CPlayer* CBall::CollisionThrow(void)
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
		if (!bHit) { continue; }

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
// 投げ処理
//==========================================================================
void CBall::Throw(CPlayer* pPlayer)
{
	// 持っていたプレイヤーと違う場合エラー
	assert(m_pPlayer == pPlayer);

	// ホーミング対象の設定
	m_pTarget = CollisionThrow();
	if (m_pTarget != nullptr)	{ m_state = STATE_HOMING; }	// ターゲットがいる場合ホーミング状態に
	else						{ m_state = STATE_MOVE; }	// ターゲットがいない場合移動状態に

	// キャッチしていたプレイヤーを破棄
	m_pPlayer = nullptr;

	// プレイヤーから保存中のボールを破棄
	pPlayer->SetBall(nullptr);
}

//==========================================================================
// リバウンド処理
//==========================================================================
void CBall::ReBound(MyLib::Vector3* pMove)
{
	// 移動ベクトルを反転
	*pMove = pMove->Invert();

	// 上移動量を追加
	pMove->y = 2.5f;

	// 移動速度を低下
	m_fMoveSpeed = 2.5f;

	// リバウンド状態にする
	m_state = STATE_REBOUND;
}

//==========================================================================
// 着地処理
//==========================================================================
void CBall::Landing(void)
{
	// 着地状態にする
	m_state = STATE_LAND;

	// ホーミング対象の初期化
	m_pTarget = nullptr;

	// チームの初期化
	m_typeTeam = CGameManager::SIDE_NONE;

	// 攻撃の初期化
	m_typeAtk = ATK_NONE;
}

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
	const char* MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";	// ボールモデル
	const float RADIUS = 7.0f;		// 半径
	const float REV_MOVE = 0.1f;	// 移動量の補正係数
	const float MAX_DIS = 1000.0f;	// ホーミングする最大距離

	const char* DEBUG_STATE_PRINT[] =	// デバッグ表示用状態
	{
		"SPAWN 生成状態 (フリーボール)",
		"CATCH 所持状態 (プレイヤー所持)",
		"THROW 攻撃状態 (攻撃判定ON)",
		"FALL  落下状態 (地面落下)"
	};
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CBall::STATE_FUNC CBall::m_SampleFuncList[] =
{
	&CBall::UpdateSpawn,	// 生成状態の更新
	&CBall::UpdateCatch,	// 所持状態の更新
	&CBall::UpdateThrow,	// 攻撃状態の更新
	&CBall::UpdateFall,		// 落下状態の更新
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

	// 所持状態にする
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
	m_fMoveSpeed = 30.0f;

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot));
	vec.Normal();
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
	m_fMoveSpeed = 40.0f;

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), -0.25f, cosf(fRot));
	vec.Normal();
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
	MyLib::Vector3 vec = MyLib::Vector3(sinf(fRot), 0.01f, cosf(fRot));
	vec.Normal();
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
// 生成状態の更新処理
//==========================================================================
void CBall::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &move, fDeltaRate, fSlowRate);

	// 地面の着地
	if (UpdateLanding(&pos, &move, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 落下遷移
		Fall();
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量
}

//==========================================================================
// 所持状態の更新処理
//==========================================================================
void CBall::UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO：今後手の位置に親子付け
	SetPosition(m_pPlayer->GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));	// プレイヤー情報
}

//==========================================================================
// 攻撃状態の更新処理
//==========================================================================
void CBall::UpdateThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 vecMove = GetMove();	// 移動量

	if (m_pTarget == nullptr)
	{
		// 移動
		UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);
	}
	else
	{
		MyLib::Vector3 posTarget = m_pTarget->GetPosition();	// ターゲット位置

		// 現在向きを取得
		float fCurAngle = vecMove.GetHorizontalAngle(VEC3_ZERO);
		UtilFunc::Transformation::RotNormalize(fCurAngle);

		// 目標向きを取得
		float fDestAngle = posTarget.AngleXZ(pos);
		UtilFunc::Transformation::RotNormalize(fDestAngle);

		// 差分向きを計算
		float fDiffAngle = fDestAngle - fCurAngle;
		UtilFunc::Transformation::RotNormalize(fDiffAngle);

		// 現在向きを更新
		fCurAngle += fDiffAngle * 0.5f;
		UtilFunc::Transformation::RotNormalize(fCurAngle);

		// 現在向きから移動ベクトルを再計算
		vecMove = MyLib::Vector3(sinf(fCurAngle), -0.25f, cosf(fCurAngle));
		vecMove.Normal();

		// 移動
		UpdateMove(&pos, &vecMove, fDeltaRate, fSlowRate);
	}

	// 地面の着地
	if (UpdateLanding(&pos, &vecMove, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 落下遷移
		Fall();
	}

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(vecMove);	// 移動量
}

//==========================================================================
// 落下状態の更新処理
//==========================================================================
void CBall::UpdateFall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &move, fDeltaRate, fSlowRate);

	// 地面の着地
	UpdateLanding(&pos, &move, fDeltaRate, fSlowRate);

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量
}

//==========================================================================
// 移動処理
//==========================================================================
void CBall::UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate)
{
	// 重力を与える
	m_fGravity -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// 位置を移動させる
	*pPos += (*pMove * m_fMoveSpeed) * fDeltaRate * fSlowRate;
	pPos->y += m_fGravity * fDeltaRate * fSlowRate;

	// 移動量を減衰させる
	m_fMoveSpeed += (0.0f - m_fMoveSpeed) * (REV_MOVE * fDeltaRate * fSlowRate);
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
		bool bHit = UtilFunc::Collision::CylinderCircleCylinder
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
			pPlayer->Hit(this);
			return true;
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

	float fMinDis = MAX_DIS;	// ボールから近いプレイヤー
	CPlayer* pTarget = nullptr;	// 目標ターゲット

	MyLib::Vector3 posBall = GetPosition();	// ボール位置
	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		if (pPlayer == m_pPlayer) { continue; }	// 投げたプレイヤー本人の場合次へ

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		// TODO：ここに視界内の判定

		// プレイヤー間の距離の測定
		float fCurDis = posPlayer.Distance(posBall);
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

	// 攻撃状態にする
	m_state = STATE_THROW;

	// ホーミング対象の設定
	m_pTarget = CollisionThrow();

	// キャッチしていたプレイヤーを破棄
	m_pPlayer = nullptr;

	// プレイヤーから保存中のボールを破棄
	pPlayer->SetBall(nullptr);
}

//==========================================================================
// 落下処理
//==========================================================================
void CBall::Fall(void)
{
	// 落下状態にする
	m_state = STATE_FALL;

	// ホーミング対象の初期化
	m_pTarget = nullptr;

	// チームの初期化
	m_typeTeam = CGameManager::SIDE_NONE;

	// 攻撃の初期化
	m_typeAtk = ATK_NONE;
}

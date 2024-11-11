//==========================================================================
// 
//  観客処理 [audience.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "audience.h"
#include "gameManager.h"

// 派生先
#include "audienceAnim.h"
#include "audienceLowPoly.h"
#include "audienceHighPoly.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int MIN_JUMP = 6;		// 最低ジャンプ量
	const int MAX_JUMP = 12;	// 最大ジャンプ量
	const float GRAVITY_RATE = 0.5f;	// 重力にかける割合
	const float JUMP_RATE	 = 0.5f;	// ジャンプ力にかける割合
	const float TIME_SPAWN	 = 2.4f;	// 入場時間
	const float TIME_DESPAWN = 3.2f;	// 退場時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CAudience::STATE_FUNC CAudience::m_StateFuncList[] =
{
	&CAudience::UpdateSpawn,	// 入場状態の更新
	&CAudience::UpdateNormal,	// 通常状態の更新
	&CAudience::UpdateJump,		// 盛り上がり状態の更新
	&CAudience::UpdateSpecial,	// スペシャル状態の更新
	&CAudience::UpdateDespawn,	// 退場状態の更新
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CAudience> CAudience::m_list = {};	// リスト
int CAudience::m_aNumWatchAll[2] = {};	// 観戦中の人数

//==========================================================================
// コンストラクタ
//==========================================================================
CAudience::CAudience(EObjType type, CGameManager::TeamSide team, int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fJumpLevel		(UtilFunc::Transformation::Random(MIN_JUMP * 100, MAX_JUMP * 100) * 0.01f),	// ジャンプ量
	m_team				(team),			// 応援チーム
	m_type				(type),			// オブジェクト種類
	m_posSpawn			(VEC3_ZERO),	// 入場位置
	m_posWatch			(VEC3_ZERO),	// 観戦位置
	m_posDespawnStart	(VEC3_ZERO),	// 退場開始位置
	m_state				(STATE_SPAWN),	// 状態
	m_fTimeState		(0.0f)			// 状態管理時間
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CAudience::~CAudience()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CAudience* CAudience::Create(EObjType type, CGameManager::TeamSide team)
{
	// メモリの確保
	CAudience* pAudience = nullptr;
	switch (type)
	{ // オブジェクト種類ごとの処理
	case CAudience::OBJTYPE_ANIM:
		pAudience = DEBUG_NEW CAudienceAnim(type, team);
		break;

	case CAudience::OBJTYPE_LOWPOLY:
		pAudience = DEBUG_NEW CAudienceLowPoly(type, team);
		break;

	case CAudience::EObjType::OBJTYPE_HIGHPOLY:
		pAudience = DEBUG_NEW CAudienceHighPoly(type, team);
		break;

	default:
		assert(false);
		break;
	}

	if (pAudience != nullptr)
	{
		// クラスの初期化
		if (FAILED(pAudience->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pAudience);
			return nullptr;
		}
	}

	return pAudience;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudience::Init()
{
	// リストに追加
	m_list.Regist(this);

	// 観戦中の人数を加算
	int nIdxTeam = m_team - 1;
	m_aNumWatchAll[nIdxTeam]++;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudience::Uninit()
{
	// リストから削除
	m_list.Delete(this);

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CAudience::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudience::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態別処理
	int nMotion = (this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);

	// モーションを設定
	SetMotion(nMotion);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudience::Draw()
{

}

//==========================================================================
// 盛り上がりの設定処理
//==========================================================================
void CAudience::SetEnableJump(const bool bJump)
{
	// 通常状態でも盛り上がり状態でもスペシャル状態でもない場合抜ける
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP && m_state != STATE_SPECIAL) { return; }

	if (bJump)
	{ // 盛り上がる場合

		// 盛り上がり状態にする
		m_state = STATE_JUMP;
	}
	else
	{ // 盛り上がらない場合

		// 通常状態にする
		m_state = STATE_NORMAL;
	}
}

//==========================================================================
// スペシャルの設定処理
//==========================================================================
void CAudience::SetSpecial()
{
	// 通常状態でも盛り上がり状態でもスペシャル状態でもない場合抜ける
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP && m_state != STATE_SPECIAL) { return; }

	// スペシャル状態にする
	m_state = STATE_SPECIAL;
}

//==========================================================================
// 退場の設定処理
//==========================================================================
bool CAudience::SetDespawn()
{
	// 既に退場中の場合抜ける
	if (m_state == STATE_DESPAWN) { return false; }

	// 退場開始位置を保存
	m_posDespawnStart = GetPosition();	// 現在の位置

	// 観戦中の人数を減算
	int nIdxTeam = m_team - 1;
	m_aNumWatchAll[nIdxTeam]--;

	// 退場状態にする
	m_state = STATE_DESPAWN;
	return true;
}

//==========================================================================
// 観戦中の人数設定処理
//==========================================================================
HRESULT CAudience::SetNumWatch(const int nNumWatch, CGameManager::TeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return E_FAIL; }

	// 観戦人数が同一の場合抜ける
	int nIdxTeam = team - 1;	// チームインデックス
	if (nNumWatch == m_aNumWatchAll[nIdxTeam]) { return E_FAIL; }

	if (nNumWatch < m_aNumWatchAll[nIdxTeam])
	{ // 観戦人数が多い場合

		// 人数分退場
		int nNumDespawn = m_aNumWatchAll[nIdxTeam] - nNumWatch;	// 退場人数
		SetDespawnAll(team, nNumDespawn);
	}
	else if (nNumWatch > m_aNumWatchAll[nIdxTeam])
	{ // 観戦人数が少ない場合

		int nNumSpawn = nNumWatch - m_aNumWatchAll[nIdxTeam];	// 登場人数
		for (int i = 0; i < nNumSpawn; i++)
		{ // 登場人数分繰り返す

			// 観客を生成
			if (FAILED(CAudience::Create(CAudience::EObjType::OBJTYPE_LOWPOLY, team)))
			{ // 生成に失敗した場合

				return E_FAIL;
			}
		}
	}

	return S_OK;
}

//==========================================================================
// 全観戦中の人数取得処理
//==========================================================================
int CAudience::GetNumWatchAll(CGameManager::TeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return -1; }

	// 引数サイドの観戦人数を返す
	int nIdxTeam = team - 1;
	return m_aNumWatchAll[nIdxTeam];
}

//==========================================================================
// 全盛り上がりの設定処理
//==========================================================================
void CAudience::SetEnableJumpAll(const bool bJump, CGameManager::TeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return; }

	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CAudience* pAudience = (*itr);	// 観客情報

		// 指定チームではない場合次へ
		if (pAudience->m_team != team) { continue; }

		// 盛り上がり状況を設定
		pAudience->SetEnableJump(bJump);
	}
}

//==========================================================================
// 全スペシャルの設定処理
//==========================================================================
void CAudience::SetSpecialAll(CGameManager::TeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return; }

	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CAudience* pAudience = (*itr);	// 観客情報

		// 指定チームではない場合次へ
		if (pAudience->m_team != team) { continue; }

		// スペシャル状態を設定
		pAudience->SetSpecial();
	}
}

//==========================================================================
// 全退場の設定処理
//==========================================================================
void CAudience::SetDespawnAll(CGameManager::TeamSide team, const int nNumDespawn)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::TeamSide::SIDE_LEFT && team != CGameManager::TeamSide::SIDE_RIGHT) { return; }

	int nCurDespawn = 0;	// 現在の退場人数
	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CAudience* pAudience = (*itr);	// 観客情報

		// 指定チームではない場合次へ
		if (pAudience->m_team != team) { continue; }

		// 退場を設定
		if (!pAudience->SetDespawn()) { continue; }	// 既に退場中の場合は次へ

		// 現在の退場人数を加算
		nCurDespawn++;

		// 引数の人数分退場させた場合抜ける
		if (nCurDespawn == nNumDespawn) { return; }
	}
}

//==========================================================================
// 入場状態の更新処理
//==========================================================================
int CAudience::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 経過時間を加算
	m_fTimeState += fDeltaTime * fSlowRate;

	// 入場位置から観戦位置に移動 (Y座標は無視する)
	MyLib::Vector3 posDest = UtilFunc::Correction::EasingLinear(m_posSpawn, m_posWatch, 0.0f, TIME_SPAWN, m_fTimeState);
	pos.x = posDest.x;
	pos.z = posDest.z;

	// 重力の更新
	if (UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate, GRAVITY_RATE))
	{ // 着地した場合

		// TODO：ここで盛り上がるタイミング化を確認
		if (true)
		{ // 盛り上がっている場合

			// 縦移動量を与える
			move.y = m_fJumpLevel * JUMP_RATE;
		}
	}

	if (m_fTimeState >= TIME_SPAWN)
	{ // 時間が経過した場合

		// 経過時間を初期化
		m_fTimeState = 0.0f;

		// 観戦位置に補正
		pos.x = m_posWatch.x;
		pos.z = m_posWatch.z;

		// スポーン終了時の設定
		EndSettingSpawn();

		// TODO：ここで盛り上がるタイミング化を確認
		if (true)	{ m_state = STATE_JUMP; }	// 盛り上がっているなら盛り上がり状態にする
		else		{ m_state = STATE_NORMAL; }	// それ以外なら通常状態にする
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量

	return 0;	// この関数の返り値は使用しない
}

//==========================================================================
// 通常状態の更新処理
//==========================================================================
int CAudience::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 重力の更新
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量

	return 0;	// この関数の返り値は使用しない
}

//==========================================================================
// 盛り上がり状態の更新処理
//==========================================================================
int CAudience::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 重力の更新
	if (UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 縦移動量を与える
		move.y = m_fJumpLevel;
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量

	return 0;	// この関数の返り値は使用しない
}

//==========================================================================
// スペシャル状態の更新処理
//==========================================================================
int CAudience::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 重力の更新
	if (UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 縦移動量を与える
		move.y = m_fJumpLevel;
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量

	return 0;	// この関数の返り値は使用しない
}

//==========================================================================
// 退場状態の更新処理
//==========================================================================
int CAudience::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 退場位置を作成
	MyLib::Vector3 posDespawn = m_posSpawn;	// 退場位置
	posDespawn.x = -m_posSpawn.x;			// X座標を反転させる

	// 経過時間を加算
	m_fTimeState += fDeltaTime * fSlowRate;

	// 観戦位置から入場位置の逆方向に移動 (Y座標は無視する)
	MyLib::Vector3 posDest = UtilFunc::Correction::EasingLinear(m_posDespawnStart, posDespawn, 0.0f, TIME_DESPAWN, m_fTimeState);
	pos.x = posDest.x;
	pos.z = posDest.z;

	// 重力の更新
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	if (m_fTimeState >= TIME_DESPAWN)
	{ // 時間が経過した場合

		// 経過時間を初期化
		m_fTimeState = 0.0f;

		// 観戦位置に補正
		pos.x = posDespawn.x;
		pos.z = posDespawn.z;

		// 自身を削除する
		Kill();
		return 0;	// この関数の返り値は使用しない
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量

	return 0;	// この関数の返り値は使用しない
}

//==========================================================================
// 重力の更新処理
//==========================================================================
bool CAudience::UpdateGravity(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaTime, const float fDeltaRate, const float fSlowRate, const float fGravityRate)
{
	// 重力を与える
	pMove->y -= (mylib_const::GRAVITY * fGravityRate) * fDeltaRate * fSlowRate;

	// 位置に重力を反映
	pPos->y += pMove->y * fDeltaRate * fSlowRate;

	if (pPos->y < CGameManager::FIELD_LIMIT)
	{ // 観客が地面に埋もれている場合

		// 観客の位置を補正
		pPos->y = CGameManager::FIELD_LIMIT;
		return true;
	}

	return false;
}

//==========================================================================
// 
//  観客処理 [audience.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "audience.h"
#include "charmManager.h"
#include "shadow.h"
#include "manager.h"

// 派生先
#include "audienceAnim.h"
#include "audienceLowPoly.h"
#include "audienceHighPoly.h"
#include "audienceAnim_result.h"
#include "audienceLowPoly_result.h"
#include "audienceHighPoly_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int MIN_JUMP = 5;		// 最低ジャンプ量
	const int MAX_JUMP = 8;	// 最大ジャンプ量
	const float	RADIUS_SHADOW	 = 44.0f;	// 影の半径
	const float	MIN_ALPHA_SHADOW = 0.02f;	// 影の透明度
	const float	MAX_ALPHA_SHADOW = 0.08f;	// 影の透明度
	const float GRAVITY_RATE	 = 0.2f;	// 重力にかける割合
	const float JUMP_RATE		 = 0.5f;	// ジャンプ力にかける割合
	const float TIME_SPAWN		 = 2.4f;	// 入場時間
	const float TIME_DESPAWN	 = 3.2f;	// 退場時間

#if _DEBUG	// TODO：ローポリ完成したら見直し
#if 1	// TODO：ポリゴンのみ
	const float RATE_HIGH = 0.05f;	// ハイポリ比率
	const float RATE_LOW  = 0.125f;	// ローポリ比率
#else
	const float RATE_HIGH = 0.0f;	// ハイポリ比率
	const float RATE_LOW  = 0.0f;	// ローポリ比率
#endif
#else
	const float RATE_HIGH = 0.05f;	// ハイポリ比率
	const float RATE_LOW  = 0.125f;	// ローポリ比率
#endif
	const float RATE_ANIM = 1.0f - (RATE_HIGH + RATE_LOW);	// アニメーション比率
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
CAudience::CAudience(EObjType type, CGameManager::ETeamSide team, int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fJumpLevel		(UtilFunc::Transformation::Random(MIN_JUMP * 100, MAX_JUMP * 100) * 0.01f),	// ジャンプ量
	m_fLandY			(CGameManager::FIELD_LIMIT),	// 着地Y座標
	m_pShadow			(nullptr),		// 影情報
	m_team				(team),			// 応援チーム
	m_type				(type),			// オブジェクト種類
	m_nArea				(0),			// 観戦エリア
	m_posSpawn			(VEC3_ZERO),	// 入場位置
	m_posWatch			(VEC3_ZERO),	// 観戦位置
	m_posDespawn		(VEC3_ZERO),	// 退場位置
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
CAudience* CAudience::Create(EObjType type, CGameManager::ETeamSide team, CScene::MODE mode)
{
	// メモリの確保
	CAudience* pAudience = nullptr;
	switch (type)
	{ // オブジェクト種類ごとの処理
	case CAudience::OBJTYPE_ANIM:
		if (mode == CScene::MODE::MODE_RESULT)
		{
			pAudience = DEBUG_NEW CAudienceAnimResult(type, team);
		}
		else
		{
			pAudience = DEBUG_NEW CAudienceAnim(type, team);
		}
		break;

	case CAudience::OBJTYPE_LOWPOLY:
		if (mode == CScene::MODE::MODE_RESULT)
		{
			pAudience = DEBUG_NEW CAudienceLowPolyResult(type, team);
		}
		else
		{
			pAudience = DEBUG_NEW CAudienceLowPoly(type, team);
		}
		break;

	case CAudience::EObjType::OBJTYPE_HIGHPOLY:
		if (mode == CScene::MODE::MODE_RESULT)
		{
			pAudience = DEBUG_NEW CAudienceHighPolyResult(type, team);
		}
		else
		{
			pAudience = DEBUG_NEW CAudienceHighPoly(type, team);
		}
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
// 影の生成処理
//==========================================================================
HRESULT CAudience::CreateShadow(CObject* pParent)
{
	// 影の生成
	m_pShadow = CShadow::Create(pParent, RADIUS_SHADOW, MIN_ALPHA_SHADOW, MAX_ALPHA_SHADOW, m_fLandY);
	if (m_pShadow == nullptr) { return E_FAIL; }

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudience::Init()
{
	// リストに追加
	m_list.Regist(this);

	// 観戦中の人数を加算
	int nIdxTeam = m_team;
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
	// 影の削除
	SAFE_KILL(m_pShadow);

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

	// ペンライトの更新
	UpdatePenlight(fDeltaTime, fDeltaRate, fSlowRate);
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
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP && m_state != STATE_SPECIAL) 
	{ 
		return;
	}

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
bool CAudience::SetDespawn(EObjType type)
{
	// 既に退場中の場合抜ける
	if (m_state == STATE_DESPAWN) { return false; }

	// 指定種類ではない場合抜ける
	if (type != OBJTYPE_NONE && type != m_type) { return false; }

	// 退場開始位置を保存
	m_posDespawnStart = GetPosition();	// 現在の位置

	// 観戦中の人数を減算
	int nIdxTeam = m_team;
	m_aNumWatchAll[nIdxTeam]--;

	// 退場状態にする
	m_state = STATE_DESPAWN;
	return true;
}

//==========================================================================
// NTR設定
//==========================================================================
bool CAudience::SetNTR(CGameManager::ETeamSide team)
{ 
	m_state = STATE_SPAWN; 
	m_team = team;
	return true; 
}

//==========================================================================
// 観戦中の人数設定処理
//==========================================================================
HRESULT CAudience::SetNumWatch(const int nNumWatch, CGameManager::ETeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return E_FAIL; }

	// 観戦人数が同一の場合抜ける
	if (nNumWatch == m_aNumWatchAll[team]) { return E_FAIL; }

	if (nNumWatch < m_aNumWatchAll[team])
	{ // 観戦人数が多い場合

		// 人数分退場
		int nNumDespawn = m_aNumWatchAll[team] - nNumWatch;	// 退場人数
		SetDespawnAll(team, nNumDespawn);
	}
	else if (nNumWatch > m_aNumWatchAll[team])
	{ // 観戦人数が少ない場合

		int nNumSpawn = nNumWatch - m_aNumWatchAll[team];	// 登場人数
		for (int i = 0; i < nNumSpawn; i++)
		{ // 登場人数分繰り返す

			CAudience::EObjType type = OBJTYPE_NONE;	// オブジェクト種類
			if		(m_aNumWatchAll[team] < (int)(MAX_WATCH * RATE_HIGH))				{ type = OBJTYPE_HIGHPOLY; }	// ハイポリ
			else if	(m_aNumWatchAll[team] < (int)(MAX_WATCH * (RATE_HIGH + RATE_LOW)))	{ type = OBJTYPE_LOWPOLY; 	}	// ローポリ
			else																		{ type = OBJTYPE_ANIM; }		// アニメーション

			// リザルト = リザルト用生成
			CScene::MODE mode = GET_MANAGER->GetMode();

			// 観客を生成
			CAudience* pAudience = CAudience::Create(type, team, mode);
			if (pAudience == nullptr)
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
int CAudience::GetNumWatchAll(CGameManager::ETeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return -1; }

	// 引数サイドの観戦人数を返す
	return m_aNumWatchAll[team];
}

//==========================================================================
// 全盛り上がりの設定処理
//==========================================================================
void CAudience::SetEnableJumpAll(const bool bJump, CGameManager::ETeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

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
void CAudience::SetSpecialAll(CGameManager::ETeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

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
void CAudience::SetDespawnAll(CGameManager::ETeamSide team, const int nNumDespawn)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

	CAudience::EObjType type = OBJTYPE_NONE;	// オブジェクト種類

	if (nNumDespawn > 0)
	{ // 削除数が指定されている場合

		if		(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH + RATE_LOW)))	{ type = OBJTYPE_ANIM; }		// アニメーション
		else if	(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH)))				{ type = OBJTYPE_LOWPOLY; }		// ローポリ
		else																		{ type = OBJTYPE_HIGHPOLY; }	// ハイポリ
	}

	int nCurDespawn = 0;	// 現在の退場人数
	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CAudience* pAudience = (*itr);	// 観客情報

		// 指定チームではない場合次へ
		if (pAudience->m_team != team) { continue; }

		// 退場を設定
		if (!pAudience->SetDespawn(type)) { continue; }	// 既に退場中の場合は次へ

		if (nNumDespawn > 0)
		{ // 削除数が指定されている場合

			// 現在の退場人数を加算
			nCurDespawn++;

			// 引数の人数分退場させた場合抜ける
			if (nCurDespawn == nNumDespawn) { return; }

			// 次の削除対象オブジェクト種類を設定
			if		(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH + RATE_LOW)))	{ type = OBJTYPE_ANIM; }		// アニメーション
			else if	(m_aNumWatchAll[team] > (int)(MAX_WATCH * (RATE_HIGH)))				{ type = OBJTYPE_LOWPOLY; }		// ローポリ
			else																		{ type = OBJTYPE_HIGHPOLY; }	// ハイポリ
		}
	}
}

//==========================================================================
// 全NTRの設定処理
//==========================================================================
void CAudience::SetNTRAll(CGameManager::ETeamSide team)
{
	// チームが設定されていない場合抜ける
	if (team != CGameManager::ETeamSide::SIDE_LEFT && team != CGameManager::ETeamSide::SIDE_RIGHT) { return; }

	CAudience::EObjType type = OBJTYPE_NONE;	// オブジェクト種類

	int nCurDespawn = 0;	// 現在の退場人数
	std::list<CAudience*>::iterator itr = m_list.GetEnd();
	while (m_list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CAudience* pAudience = (*itr);	// 観客情報

		// 指定チームではない場合次へ
		//if (pAudience->m_team != team) { continue; }

		// NTR設定
		if (!pAudience->SetNTR(team)) { continue; }	// 既に退場中の場合は次へ

	}
}

//==========================================================================
// 観戦中の人数リセット
//==========================================================================
void CAudience::ReSetNumWatchAll()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_aNumWatchAll[i] = 0;
	}
}

//==========================================================================
// 入場状態の更新処理
//==========================================================================
int CAudience::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCharmManager* pCharm = CCharmManager::GetInstance();	// モテマネージャー
	bool bHype = false;										// 盛り上がりフラグ

	if (pCharm != nullptr)
	{
		bHype = pCharm->IsHype(GetTeam());
	}

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

	}

	if (m_fTimeState >= TIME_SPAWN)
	{ // 時間が経過した場合

		// 経過時間を初期化
		m_fTimeState = 0.0f;

		// 観戦位置に補正
		pos.x = m_posWatch.x;
		pos.z = m_posWatch.z;
		SetPosition(pos);	// 位置

		// スポーン終了時の設定
		EndSettingSpawn();

		// 状態を遷移させる
		if (bHype ||
			GET_MANAGER->GetScene()->GetMode() == CScene::MODE::MODE_RESULT)
		{// 盛り上がっているなら盛り上がり状態にする
			m_state = STATE_JUMP; 
		}	
		else		
		{ // それ以外なら通常状態にする
			m_state = STATE_NORMAL; 
		}
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
	CCharmManager* pCharm = CCharmManager::GetInstance();	// モテマネージャー
	bool bHype = false;										// 盛り上がりフラグ

	if (pCharm != nullptr)
	{
		bHype = pCharm->IsHype(GetTeam());
	}

	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 重力の更新
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	if (bHype)
	{ // 盛り上がっている場合

		// 盛り上がり状態にする
		m_state = STATE_JUMP;
	}

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
	CCharmManager* pCharm = CCharmManager::GetInstance();	// モテマネージャー
	bool bHype = false;										// 盛り上がりフラグ

	if (pCharm != nullptr)
	{
		bHype = pCharm->IsHype(GetTeam());
	}
	else if (GET_MANAGER->GetScene()->GetMode() == CScene::MODE::MODE_RESULT)
	{// リザルトは確定盛り上がり
		bHype = true;
	}

	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 重力の更新
	if (UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate))
	{ // 着地した場合

		if (!bHype)
		{ // 盛り上がっていない場合

			// 通常状態にする
			m_state = STATE_NORMAL;
		}
		else
		{ // 盛り上がっている場合

			// 縦移動量を与える
			move.y = m_fJumpLevel;
		}
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

	// 経過時間を加算
	m_fTimeState += fDeltaTime * fSlowRate;

	// 観戦位置から入場位置に移動 (Y座標は無視する)
	MyLib::Vector3 posDest = UtilFunc::Correction::EasingLinear(m_posDespawnStart, m_posDespawn, 0.0f, TIME_DESPAWN, m_fTimeState);
	pos.x = posDest.x;
	pos.z = posDest.z;

	// 重力の更新
	UpdateGravity(&pos, &move, fDeltaTime, fDeltaRate, fSlowRate);

	if (m_fTimeState >= TIME_DESPAWN)
	{ // 時間が経過した場合

		// 経過時間を初期化
		m_fTimeState = 0.0f;

		// 入場位置に補正
		pos.x = m_posDespawn.x;
		pos.z = m_posDespawn.z;

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

	if (pPos->y < m_fLandY)
	{ // 観客が地面に埋もれている場合

		// 観客の位置を補正
		pPos->y = m_fLandY;
		return true;
	}

	return false;
}

//==========================================================================
// 状態管理時間の最大値設定処理
//==========================================================================
void CAudience::SetTimeStateByTimeStateMax()
{
	m_fTimeState = TIME_SPAWN;
	UpdateSpawn(0.0f, 0.0f, 1.0f);
}

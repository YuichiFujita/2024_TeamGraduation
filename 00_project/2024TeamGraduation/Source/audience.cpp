//==========================================================================
// 
//  観客処理 [audience.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "audience.h"
#include "audienceAnim.h"
#include "gameManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int MIN_JUMP = 6;		// 最低ジャンプ量
	const int MAX_JUMP = 12;	// 最大ジャンプ量

	const float RADIUS = 60.0f;	// TODO：いらん
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CAudience::STATE_FUNC CAudience::m_StateFuncList[] =
{
	&CAudience::UpdateSpawn,	// 入場状態の更新
	&CAudience::UpdateNormal,	// 通常状態の更新
	&CAudience::UpdateJump,		// 盛り上がり状態の更新
	&CAudience::UpdateDespawn,	// 退場状態の更新
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CAudience> CAudience::m_list = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CAudience::CAudience(EObjType type, int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fJumpLevel (UtilFunc::Transformation::Random(MIN_JUMP * 100, MAX_JUMP * 100) * 0.01f),	// ジャンプ量
	m_type		 (type),		// オブジェクト種類
	m_state		 (STATE_SPAWN)	// 状態
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
CAudience* CAudience::Create(EObjType type)
{
	// メモリの確保
	CAudience* pAudience = nullptr;
	switch (type)
	{ // オブジェクト種類ごとの処理
	case CAudience::OBJTYPE_ANIM:
		pAudience = DEBUG_NEW CAudienceAnim(type);
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
	(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudience::Draw()
{

}

//==========================================================================
// 入場状態の更新処理
//==========================================================================
void CAudience::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量



	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量
}

//==========================================================================
// 通常状態の更新処理
//==========================================================================
void CAudience::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 盛り上がり状態の更新処理
//==========================================================================
void CAudience::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 重力を与える
	move.y -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// 位置に重力を反映
	pos.y += move.y * fDeltaRate * fSlowRate;

	if (pos.y - RADIUS < CGameManager::FIELD_LIMIT)
	{ // ボールが地面に埋もれている場合

		// ボールの位置を補正
		pos.y = CGameManager::FIELD_LIMIT + RADIUS;

		// 縦移動量を与える
		move.y = m_fJumpLevel;
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量
}

//==========================================================================
// 退場状態の更新処理
//==========================================================================
void CAudience::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量



	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量
}

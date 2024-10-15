//=============================================================================
// 
//  ボール処理 [ball.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "ball.h"
#include "player.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\dadgeball\\dodgeball.x";
	const float RADIUS = 7.0f;
	const float REV_MOVE = 0.1f;

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
CListManager<CBall> CBall::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CBall::CBall(int nPriority) : CObjectX(nPriority),
	m_typeTeam	 (CGameManager::SIDE_NONE),	// チームサイド
	m_typeAtk	 (ATK_NONE),	// 攻撃種類
	m_state		 (STATE_SPAWN),	// 状態
	m_fStateTime (0.0f),		// 状態カウンター
	m_pPlayer	 (nullptr)		// プレイヤー情報
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
	m_List.Regist(this);

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
	m_List.Delete(this);

	// 親クラスの終了
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CBall::Kill()
{
	// リストから削除
	m_List.Delete(this);

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

		// 落下状態にする
		m_state = STATE_FALL;
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
	SetPosition(m_pPlayer->GetPosition() + MyLib::Vector3(0.0f, 100.0f, 0.0f));	// プレイヤー情報
}

//==========================================================================
// 攻撃状態の更新処理
//==========================================================================
void CBall::UpdateThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 情報を取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 move = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &move, fDeltaRate, fSlowRate);

	// 地面の着地
	if (UpdateLanding(&pos, &move, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 落下状態にする
		m_state = STATE_FALL;
	}

	// 情報を反映
	SetPosition(pos);	// 位置
	SetMove(move);		// 移動量
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
	pMove->y -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// 位置を移動させる
	*pPos += *pMove * fDeltaRate * fSlowRate;

	// 移動量を減衰させる
	pMove->x += (0.0f - pMove->x) * (REV_MOVE * fDeltaRate * fSlowRate);
	pMove->z += (0.0f - pMove->z) * (REV_MOVE * fDeltaRate * fSlowRate);
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
		return true;
	}

	return false;
}

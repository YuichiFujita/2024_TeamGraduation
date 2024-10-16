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
CListManager<CBall> CBall::m_list = {};	// リスト

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

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	SetMove(MyLib::Vector3(sinf(fRot), 0.0f, cosf(fRot)) * 40.0f);
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

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	SetMove(MyLib::Vector3(sinf(fRot), -0.25f, cosf(fRot)) * 80.0f);
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

	// TODO：仮
	float fRot = pPlayer->GetRotation().y + D3DX_PI;
	SetMove(MyLib::Vector3(sinf(fRot), 0.01f, cosf(fRot)) * 120.0f);
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
	MyLib::Vector3 move = GetMove();	// 移動量

	// 移動
	UpdateMove(&pos, &move, fDeltaRate, fSlowRate);

	// 地面の着地
	if (UpdateLanding(&pos, &move, fDeltaRate, fSlowRate))
	{ // 着地した場合

		// 落下遷移
		Fall();
	}

	// プレイヤーとの当たり判定
	CollisionPlayer(&pos);

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
		bool bHit = UtilFunc::Collision::CircleRange3D
		( // 引数
			*pPos,
			pPlayer->GetPosition(),
			RADIUS,
			pPlayer->GetRadius()
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
// 投げ処理
//==========================================================================
void CBall::Throw(CPlayer* pPlayer)
{
	// 持っていたプレイヤーと違う場合エラー
	assert(m_pPlayer == pPlayer);

	// 攻撃状態にする
	m_state = STATE_THROW;

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

	// チームの初期化
	m_typeTeam = CGameManager::SIDE_NONE;

	// 攻撃の初期化
	m_typeAtk = ATK_NONE;
}

//==========================================================================
// 
//  観客_アニメーション3D処理 [audienceAnim.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "audienceAnim.h"
#include "gameManager.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\frisk.txt";	// プレイヤーセットアップテキスト
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// 優先順位

	const int LEFT_LINE[]  = { (int)CAudienceAnim::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };		// チームサイドごとの左ライン
	const int RIGHT_LINE[] = { (int)CGameManager::CENTER_LINE, (int)CAudienceAnim::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
}

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type, CGameManager::TeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnimChara	(nullptr),		// キャラクターアニメーション情報
	m_moveMotion	(MOTION_IDOL)	// 移動モーション
{
}

//==========================================================================
// デストラクタ
//==========================================================================
CAudienceAnim::~CAudienceAnim()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudienceAnim::Init()
{
	// 親クラスの初期化
	if (FAILED(CAudience::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam() - 1;
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(LEFT_LINE[nIdxTeam], RIGHT_LINE[nIdxTeam]);
	posWatch.y = CGameManager::FIELD_LIMIT;
	posWatch.z = (float)UtilFunc::Transformation::Random((int)NEAR_LINE, (int)FAR_LINE);
	SetWatchPosition(posWatch);

	// ランダムに生成方向を設定
	float fTurn = 0.0f;	// 方向係数
	if ((bool)(rand() % 2))
	{
		// 右からの入場にする
		m_moveMotion = MOTION_MOVE_L;
		fTurn = 1.0f;
	}
	else
	{
		// 左からの入場にする
		m_moveMotion = MOTION_MOVE_R;
		fTurn = -1.0f;
	}

	// 生成位置を設定
	MyLib::Vector3 posSpawn = posWatch;
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// オブジェクトキャラクターアニメーションの生成
	m_pAnimChara = CObjectCharaAnim::Create(posSpawn);
	if (m_pAnimChara == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// キャラクター情報の割当
	m_pAnimChara->BindCharaData(SETUP_TXT);

	// モーションを設定
	m_pAnimChara->SetMotion(m_moveMotion);

	// アニメーション3Dの自動更新/自動描画/自動破棄をしない種類にする
	m_pAnimChara->SetType(CObject::TYPE::TYPE_NONE);

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudienceAnim::Uninit()
{
	// 親クラスの終了
	CAudience::Uninit();

	// オブジェクトキャラクターアニメーションの終了
	SAFE_UNINIT(m_pAnimChara);
}

//==========================================================================
// 削除処理
//==========================================================================
void CAudienceAnim::Kill()
{
	// 親クラスの削除
	CAudience::Kill();

	// オブジェクトキャラクターアニメーションの終了
	SAFE_UNINIT(m_pAnimChara);
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudienceAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pAnimChara != nullptr)
	{
		// オブジェクトキャラクターアニメーションの更新
		m_pAnimChara->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 親クラスの更新
	CAudience::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudienceAnim::Draw()
{
	if (m_pAnimChara != nullptr)
	{
		LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

		// アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 180);

		// オブジェクトキャラクターアニメーションの描画
		m_pAnimChara->Draw();

		// アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	}

	// 親クラスの描画
	CAudience::Draw();
}

//==========================================================================
// 入場状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入場状態の更新
	CAudience::UpdateSpawn(fDeltaTime, fDeltaRate, fSlowRate);

	// 移動モーションを返す
	return m_moveMotion;
}

//==========================================================================
// 通常状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 通常状態の更新
	CAudience::UpdateNormal(fDeltaTime, fDeltaRate, fSlowRate);

	// 待機モーションを返す
	return MOTION_IDOL;
}

//==========================================================================
// 盛り上がり状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 盛り上がり状態の更新
	CAudience::UpdateJump(fDeltaTime, fDeltaRate, fSlowRate);

	// ジャンプモーションを返す
	return MOTION_JUMP;
}

//==========================================================================
// 退場状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 退場状態の更新
	CAudience::UpdateDespawn(fDeltaTime, fDeltaRate, fSlowRate);

	// 移動モーションを返す
	return m_moveMotion;
}

//==========================================================================
// モーションの設定処理
//==========================================================================
void CAudienceAnim::SetMotion(const int nMotion)
{
	// キャラクターアニメーション情報がない場合抜ける
	if (m_pAnimChara == nullptr) { return; }

	int nAnimMotion = m_pAnimChara->GetMotion();	// 現在再生中のモーション
	if (m_pAnimChara->IsLoop())
	{ // ループするモーションだった場合

		if (nAnimMotion != nMotion)
		{ // 現在のモーションが再生中のモーションと一致しない場合

			// 現在のモーションの設定
			m_pAnimChara->SetMotion(nMotion);
		}
	}
}

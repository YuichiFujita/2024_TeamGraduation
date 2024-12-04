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
#include "objectX.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\frisk.txt";	// プレイヤーセットアップテキスト
	const char* LIGHT_PATH = "data\\MODEL\\penlight.x";		// ペンライトのモデルパス
	const int PRIORITY = mylib_const::PRIORITY_DEFAULT;		// 優先順位

	namespace Far
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= 1360;	// 手前の生成位置上限
		const int FAR_LINE		= 1700;	// 奥の生成位置上限
	}

	namespace Up
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= 1700;		// 手前の生成位置上限
		const int FAR_LINE		= 1900;		// 奥の生成位置上限
		const float LANDY		= 540.0f;	// 着地Y座標
	}

	namespace Down
	{
		const int LEFT_LINE[]	= { (int)CAudience::MAX_LEFT_LINE, (int)CGameManager::CENTER_LINE };	// チームサイドごとの左ライン
		const int RIGHT_LINE[]	= { (int)CGameManager::CENTER_LINE, (int)CAudience::MAX_RIGHT_LINE };	// チームサイドごとの右ライン
		const int NEAR_LINE		= -1700;	// 手前の生成位置上限
		const int FAR_LINE		= -1360;	// 奥の生成位置上限
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 観戦位置計算リスト
CAudienceAnim::WATCH_POS_FUNC CAudienceAnim::m_CalcWatchPositionFunc[] =
{
	&CAudienceAnim::CalcWatchPositionFar,	// 奥観戦位置
	&CAudienceAnim::CalcWatchPositionUp,	// 上観戦位置
	&CAudienceAnim::CalcWatchPositionNear,	// 手前観戦位置
};

//==========================================================================
// コンストラクタ
//==========================================================================
CAudienceAnim::CAudienceAnim(EObjType type, CGameManager::ETeamSide team) : CAudience(type, team, PRIORITY, CObject::LAYER_DEFAULT),
	m_pAnimChara	(nullptr),			// キャラクターアニメーション情報
	m_pLight		(nullptr),			// ペンライト情報
	m_idolMotion	(MOTION_IDOL_U),	// 待機モーション
	m_jumpMotion	(MOTION_IDOL_U),	// ジャンプモーション
	m_moveMotion	(MOTION_IDOL_U)		// 移動モーション
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

	// 観戦エリアの設定
	SetArea((int)(rand() % EArea::AREA_MAX));

	// 観戦位置の設定
	(this->*(m_CalcWatchPositionFunc[GetArea()]))();

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

	// 生成位置の設定
	MyLib::Vector3 posSpawn = GetWatchPosition();
	posSpawn.x = CAudience::SPAWN_SIDE_LINE * fTurn;
	SetSpawnPosition(posSpawn);

	// 退場位置の設定
	MyLib::Vector3 posDespawn = posSpawn;
	posDespawn.x = -posSpawn.x;
	SetDespawnPosition(posDespawn);

	// アニメーションキャラクターの生成
	if (FAILED(CreateAnimCharacter(posSpawn)))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// ペンライトの生成
	if (FAILED(CreatePenLight()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

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

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

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

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

	// 待機モーションを返す
	return m_idolMotion;
}

//==========================================================================
// 盛り上がり状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 盛り上がり状態の更新
	CAudience::UpdateJump(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

	// ジャンプモーションを返す
	return m_jumpMotion;
}

//==========================================================================
// スペシャル状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// スペシャル状態の更新
	CAudience::UpdateSpecial(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオンにする
	m_pLight->SetEnableDisp(true);

	// ライトの位置を頭の上にする
	m_pLight->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 230.0f, 0.0f));	// TODO：後で振ったりさせる

	// ジャンプモーションを返す
	return m_jumpMotion;
}

//==========================================================================
// 退場状態の更新処理
//==========================================================================
int CAudienceAnim::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 退場状態の更新
	CAudience::UpdateDespawn(fDeltaTime, fDeltaRate, fSlowRate);

	// ライトの自動描画をオフにする
	m_pLight->SetEnableDisp(false);

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

//==========================================================================
// キャラクター生成
//==========================================================================
HRESULT CAudienceAnim::CreateAnimCharacter(const MyLib::Vector3& rPos)
{
	// オブジェクトキャラクターアニメーションの生成
	m_pAnimChara = CObjectCharaAnim::Create(rPos);
	if (m_pAnimChara == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// キャラクター情報の割当
	m_pAnimChara->BindCharaData(SETUP_TXT);

	// モーションの設定
	m_pAnimChara->SetMotion(m_moveMotion);

	// アニメーション3Dの自動更新/自動描画/自動破棄をしない種類にする
	m_pAnimChara->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// ペンライト生成
//==========================================================================
HRESULT CAudienceAnim::CreatePenLight()
{
	// ペンライトの生成
	m_pLight = CObjectX::Create(LIGHT_PATH, VEC3_ZERO);
	if (m_pLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を設定
	m_pLight->SetType(CObject::TYPE::TYPE_OBJECTX);

	// 自動描画フラグをオフにする
	m_pLight->SetEnableDisp(false);

	// TODO：スケール調整後で消す
#if 1
	m_pLight->SetScale(MyLib::Vector3(1.0f, 0.4f, 1.0f));
#endif

	return S_OK;
}

//==========================================================================
// 観戦位置の計算処理 (奥)
//==========================================================================
void CAudienceAnim::CalcWatchPositionFar()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Far::LEFT_LINE[nIdxTeam], Far::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Far::NEAR_LINE, Far::FAR_LINE);
	SetWatchPosition(posWatch);

	// モーションの設定
	m_idolMotion = MOTION_IDOL_D;	// 下待機モーション
	m_jumpMotion = MOTION_JUMP_D;	// 下ジャンプモーション
}

//==========================================================================
// 観戦位置の計算処理 (上)
//==========================================================================
void CAudienceAnim::CalcWatchPositionUp()
{
	// 着地Y座標の設定
	SetLandY(Up::LANDY);

	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Up::LEFT_LINE[nIdxTeam], Up::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Up::NEAR_LINE, Up::FAR_LINE);
	SetWatchPosition(posWatch);

	// モーションの設定
	m_idolMotion = MOTION_IDOL_D;	// 下待機モーション
	m_jumpMotion = MOTION_JUMP_D;	// 下ジャンプモーション
}

//==========================================================================
// 観戦位置の計算処理 (手前)
//==========================================================================
void CAudienceAnim::CalcWatchPositionNear()
{
	// ランダムに観戦位置を設定
	int nIdxTeam = GetTeam();
	MyLib::Vector3 posWatch;
	posWatch.x = (float)UtilFunc::Transformation::Random(Down::LEFT_LINE[nIdxTeam], Down::RIGHT_LINE[nIdxTeam]);
	posWatch.y = GetLandY();
	posWatch.z = (float)UtilFunc::Transformation::Random(Down::NEAR_LINE, Down::FAR_LINE);
	SetWatchPosition(posWatch);

	// モーションの設定
	m_idolMotion = MOTION_IDOL_U;	// 上待機モーション
	m_jumpMotion = MOTION_JUMP_U;	// 上ジャンプモーション
}

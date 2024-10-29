//============================================================
//
//	スペシャル演出マネージャー処理 [specialManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "specialManager.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"

#include "game.h"
#include "gameManager.h"
#include "player.h"
#include "playerStatus.h"
#include "lightManager.h"
#include "lightPoint.h"
#include "cutin.h"
#include "audience.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 4;			// 優先順位
	const float	LIGHT_RANGE = 600.0f;	// 光源範囲
	const MyLib::Vector3 LIGHT_OFFSET = MyLib::Vector3(0.0f, 160.0f, 0.0f);	// ライトオフセット

	namespace fade
	{
		const float	MOVE_TIME	 = 0.5f;	// 変動時間
		const float DEST_ALPHA	 = 0.5f;	// 目標透明度
		const float INIT_ALPHA	 = 0.0f;	// 初期透明度
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// 差分透明度
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// 初期色
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 280.0f, 0.0f);	// 大きさ
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(SCREEN_CENT.x, 548.0f, 0.0f);	// 位置
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;	// 向き
	}

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// テクスチャパス
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f);	// 大きさ
		const float	MOVE_TIME	= 0.68f;	// 移動時間
		const D3DXCOLOR DEST_COL	= MyLib::color::White();		// 目標色
		const D3DXCOLOR INIT_COL	= MyLib::color::White(0.0f);	// 初期色
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;			// 差分色
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 505.0f, 0.0f);		// 目標位置
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// 初期位置
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// 差分位置
	}

	namespace line
	{
		const float	MOVE_TIME	= 0.5f;	// 移動時間
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 590.0f, 0.0f);	// 位置
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 10.0f, 0.0f);		// 目標大きさ
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// 初期大きさ
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// 差分大きさ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CSpecialManager::AFuncUpdateState CSpecialManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,								// 何もしない更新
	&CSpecialManager::UpdateCutIn,			// カットイン更新
	&CSpecialManager::UpdatePlayerHype,		// プレイヤー盛り上げ更新
	&CSpecialManager::UpdateAudienceHype,	// 観客盛り上げ更新
	&CSpecialManager::UpdatePlayerSpecial,	// プレイヤースペシャル演出更新
	&CSpecialManager::UpdateNormal,			// 通常更新
	&CSpecialManager::UpdateEnd,			// 終了更新
};
CSpecialManager::AFuncUpdateSpecial CSpecialManager::m_aFuncUpdateSpecial[] =	// スペシャル更新関数
{
	nullptr,							// スペシャル無し
	&CSpecialManager::UpdateKamehameha,	// かめはめ波の更新
};
CSpecialManager* CSpecialManager::m_pThisClass = nullptr;	// 自身のインスタンス

//************************************************************
//	親クラス [CSpecialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSpecialManager::CSpecialManager(const CPlayer* pAttack, const CPlayer* pTarget) : CObject(PRIORITY),
	m_pAttackPlayer	(pAttack),		// 攻撃プレイヤー
	m_pTargetPlayer	(pTarget),		// 標的プレイヤー
	m_pAttackLight	(nullptr),		// 攻撃プレイヤーを照らすライト
	m_pTargetLight	(nullptr),		// 標的プレイヤーを照らすライト
	m_pCutIn		(nullptr),		// カットイン情報
	m_state			(STATE_NONE),	// 状態
	m_fCurTime		(0.0f)			// 現在の待機時間
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CSpecialManager::STATE_MAX, "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_aFuncUpdateSpecial) == CBall::SPECIAL_MAX, "ERROR : Special Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CSpecialManager::~CSpecialManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSpecialManager::Init(void)
{
	// メンバ変数を初期化
	m_state		= STATE_CUTIN;	// 状態
	m_fCurTime	= 0.0f;			// 現在の待機時間

	// 種類をマネージャーにする
	SetType(CObject::TYPE::TYPE_MANAGER);

	// 世界停止中に動けるようにする
	SetEnablePosibleMove_WorldPause(true);

	// 攻撃プレイヤーを照らすライトの生成
	m_pAttackLight = CLightPoint::Create();
	if (m_pAttackLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 拡散光を設定
	m_pAttackLight->SetDiffuse(MyLib::color::White());

	// 光源範囲を設定
	m_pAttackLight->SetRange(LIGHT_RANGE);

	// 標的プレイヤーを照らすライトの生成
	m_pTargetLight = CLightPoint::Create();
	if (m_pTargetLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 拡散光を設定
	m_pTargetLight->SetDiffuse(MyLib::color::White());

	// 光源範囲を設定
	m_pTargetLight->SetRange(LIGHT_RANGE);

	// カットインの生成
	m_pCutIn = CCutIn::Create();
	if (m_pCutIn == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

#if 1
	// 体育館を暗くする
	GET_MANAGER->GetLight()->SetEnableBright(false);
#endif

#if 1
	// 攻撃側プレイヤーチームの観客を盛り上げる
	CAudience::SetSpecialAll(m_pAttackPlayer->GetStatus()->GetTeam());
#endif

#if 1
	// 世界の時を止める
	GET_MANAGER->SerEnableWorldPaused(true);
#endif

#if 1
	// ゲームをスペシャル演出シーンに変更
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::ESceneType::SCENE_SPECIAL_STAG);
#endif

// 成功を返す
return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSpecialManager::Uninit(void)
{
	// 攻撃プレイヤーを照らすライトの終了
	SAFE_UNINIT(m_pAttackLight);

	// 標的プレイヤーを照らすライトの終了
	SAFE_UNINIT(m_pTargetLight);

	// オブジェクトを破棄
	Release();

	// 自身の保存していたインスタンスを初期化
	m_pThisClass = nullptr;
}

//============================================================
//	削除処理
//============================================================
void CSpecialManager::Kill()
{
	// 自身の終了
	Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSpecialManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // 更新関数が指定されている場合

		// 各状態ごとの更新
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	描画処理
//============================================================
void CSpecialManager::Draw()
{

}

//============================================================
//	生成処理
//============================================================
CSpecialManager *CSpecialManager::Create(const CPlayer* pAttack, const CPlayer* pTarget)
{
	if (m_pThisClass != nullptr)
	{ // 自クラスの他インスタンスがある場合

		// インスタンスを破棄
		SAFE_UNINIT(m_pThisClass);
	}

	// スペシャル演出マネージャーの生成
	CSpecialManager *pSpecialManager = DEBUG_NEW CSpecialManager(pAttack, pTarget);
	if (pSpecialManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// スペシャル演出マネージャーの初期化
		if (FAILED(pSpecialManager->Init()))
		{ // 初期化に失敗した場合

			// スペシャル演出マネージャーの破棄
			SAFE_DELETE(pSpecialManager);
			return nullptr;
		}

		// インスタンスを保存
		assert(m_pThisClass == nullptr);
		m_pThisClass = pSpecialManager;

		// 確保したアドレスを返す
		return pSpecialManager;
	}
}

//============================================================
//	カットインの更新処理
//============================================================
void CSpecialManager::UpdateCutIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pCutIn->IsEnd())
	{ // カットイン演出が終了した場合

		// 世界の時はうごきだす
		GET_MANAGER->SerEnableWorldPaused(false);

		// カットインの終了
		SAFE_UNINIT(m_pCutIn);

		// プレイヤー盛り上げ状態にする
		m_state = STATE_PLAYER_HYPE;
	}

	// ライト位置の設定
	SetLightPosition();
}

//============================================================
//	プレイヤー盛り上げの更新処理
//============================================================
void CSpecialManager::UpdatePlayerHype(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO：ここにプレイヤーカメラ

#if 1
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= 1.0f)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 観客盛り上げ状態にする
		m_state = STATE_AUDIENCE_HYPE;
	}
#endif

	// ライト位置の設定
	SetLightPosition();
}

//============================================================
//	観客盛り上げの更新処理
//============================================================
void CSpecialManager::UpdateAudienceHype(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO：ここに観客カメラ

#if 1
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= 1.0f)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// プレイヤーをスペシャルモーションにする
		m_pAttackPlayer->SetMotion(CPlayer::MOTION_SPECIAL);

		// プレイヤースペシャル演出状態にする
		m_state = STATE_PLAYER_SPECIAL;
	}
#endif

	// ライト位置の設定
	SetLightPosition();
}

//============================================================
//	プレイヤースペシャル演出の更新処理
//============================================================
void CSpecialManager::UpdatePlayerSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall::ESpecial typeSpecial = CGame::GetInstance()->GetGameManager()->GetBall()->GetTypeSpecial();	// スペシャル種類
	assert(typeSpecial > CBall::SPECIAL_NONE && typeSpecial < CBall::SPECIAL_MAX);
	if (m_aFuncUpdateState[typeSpecial] != nullptr)
	{ // 更新関数が指定されている場合

		// 各スペシャルごとの更新
		(this->*(m_aFuncUpdateSpecial[typeSpecial]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// TODO：投げた瞬間の解除はちょっと...
	if (m_pAttackPlayer->GetBall() == nullptr)
	{ // ボールを投げている場合

		// 終了状態にする
		m_state = STATE_END;
	}

	// ライト位置の設定
	SetLightPosition();
}

//============================================================
//	通常の更新処理
//============================================================
void CSpecialManager::UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 1
	if (m_pAttackPlayer->GetBall() == nullptr)
	{ // ボールを投げている場合

		// 終了状態にする
		m_state = STATE_END;
	}
#endif

	// ライト位置の設定
	SetLightPosition();
}

//============================================================
//	終了の更新処理
//============================================================
void CSpecialManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 体育館を明るくする
	GET_MANAGER->GetLight()->SetEnableBright(true);

	// 攻撃側プレイヤーチームの観客を通常状態にする
	CAudience::SetEnableJumpAll(false, m_pAttackPlayer->GetStatus()->GetTeam());

	// ゲームをメインシーンに変更
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::ESceneType::SCENE_MAIN);

	// 自身の終了
	Uninit();
}

//============================================================
//	かめはめ波の更新処理
//============================================================
void CSpecialManager::UpdateKamehameha(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO：ここに各スペシャルのカメラ動作

#if 0
	// タイマーを加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= 2.0f)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}
#endif
}

//============================================================
//	ライト位置の設定処理
//============================================================
void CSpecialManager::SetLightPosition()
{
	// 攻撃プレイヤーの位置に移動
	m_pAttackLight->SetPosition(m_pAttackPlayer->GetPosition() + LIGHT_OFFSET);

	// 標的プレイヤーの位置に移動
	m_pTargetLight->SetPosition(m_pTargetPlayer->GetPosition() + LIGHT_OFFSET);
}

//============================================================
//
//	ゲーム終了演出マネージャー処理 [gameEndManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gameEndManager.h"
#include "manager.h"
#include "camera.h"
#include "lightManager.h"
#include "player.h"
#include "gamemanager.h"
#include "lightPoint.h"
#include "gamesetUI.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY	= 4;		// 優先順位
	const float	LIGHT_RANGE	= 600.0f;	// 光源範囲
	const float	WAIT_TIME	= 2.0f;		// 待機時間
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGameEndManager::AFuncUpdateState CGameEndManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	&CGameEndManager::UpdateNone,		// 何もしない更新
	&CGameEndManager::UpdateLookPlayer,	// プレイヤーカメラ更新
	&CGameEndManager::UpdateDeathWait,	// プレイヤー死亡待機更新
	&CGameEndManager::UpdateGameSet,	// ゲームセット出現更新
	&CGameEndManager::UpdateWait,		// 待機更新
	&CGameEndManager::UpdateEnd,		// 終了更新
};
CGameEndManager* CGameEndManager::m_pInstance = nullptr;	// 自身のインスタンス

//************************************************************
//	親クラス [CGameEndManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGameEndManager::CGameEndManager(CPlayer* pDeathPlayer) : CObject(PRIORITY),
	m_pDeathPlayer	(pDeathPlayer),	// 死亡プレイヤー
	m_pGameSetUI	(nullptr),		// ゲームセットUI
	m_state			(STATE_NONE),	// 状態
	m_fCurTime		(0.0f)			// 現在の待機時間
{
	// メンバ変数をクリア
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ライト数分繰り返す

		m_apLight[i] = nullptr;	// ライト情報
	}

	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CGameEndManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CGameEndManager::~CGameEndManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGameEndManager::Init(void)
{
	CGameManager* pGameManager = CGameManager::GetInstance();	// ゲームマネージャー
	CManager* pManager = GET_MANAGER;	// マネージャー

	// 種類をマネージャーにする
	SetType(CObject::TYPE::TYPE_MANAGER);

	// 世界停止中に動けるようにする
	SetEnablePosibleMove_WorldPause(true);

	// タイマーを停止
	pGameManager->SetEnableTimerStop(true);

	// ゲームを終了演出シーンに変更
	pGameManager->SetSceneType(CGameManager::ESceneType::SCENE_END_STAG);

	if (m_pDeathPlayer != nullptr)
	{ // プレイヤー死亡で終了する場合

		// 初期状態を設定
		m_state = STATE_LOOK_PLAYER;

		for (int i = 0; i < NUM_LIGHT; i++)
		{ // ライト数分繰り返す

			// ライトの生成
			m_apLight[i] = CLightPoint::Create();
			if (m_apLight[i] == nullptr)
			{ // 生成に失敗した場合

				return E_FAIL;
			}

			// 拡散光を設定
			m_apLight[i]->SetDiffuse(MyLib::color::White());

			// 光源範囲を設定
			m_apLight[i]->SetRange(LIGHT_RANGE);
		}

		// スロー倍率を設定
		pManager->SetSlowRate(0.25f);

		// 体育館を暗くする
		pManager->GetLight()->SetEnableBright(false, 1.5f);

		// ゲーム終了カメラの設定
		CCamera* pCamera = pManager->GetCamera();	// カメラ情報
		pCamera->SetState(CCamera::STATE_GAME_END, false);

		// 注視点対象を死亡プレイヤーにする
		pCamera->SetGameEndInfo(m_pDeathPlayer, MyLib::Vector3(0.0f, 80.0f, 0.0f), 1.0f);

		// ライト位置の設定
		SetLightPosition();
	}
	else
	{ // タイマーなどの他要因で終了する場合

		// ゲームセットUIの生成
		m_pGameSetUI = CGameSetUI::Create();
		if (m_pGameSetUI == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 世界の時を止める
		pManager->SetEnableWorldPaused(true);

		// 初期状態を設定
		m_state = STATE_GAME_SET;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameEndManager::Uninit(void)
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ライト数分繰り返す

		// ライトの終了
		SAFE_UNINIT(m_apLight[i]);
	}

	// オブジェクトを破棄
	Release();

	// 自身の保存していたインスタンスを初期化
	m_pInstance = nullptr;
}

//============================================================
//	削除処理
//============================================================
void CGameEndManager::Kill()
{
	// 自身の終了
	Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGameEndManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CGameEndManager::Draw()
{

}

//============================================================
//	生成処理
//============================================================
CGameEndManager *CGameEndManager::Create(CPlayer* pDeathPlayer)
{
	if (m_pInstance != nullptr)
	{ // 自クラスの他インスタンスがある場合

		// インスタンスを破棄
		SAFE_UNINIT(m_pInstance);
	}

	// ゲーム終了演出マネージャーの生成
	CGameEndManager *pGameEndManager = DEBUG_NEW CGameEndManager(pDeathPlayer);
	if (pGameEndManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ゲーム終了演出マネージャーの初期化
		if (FAILED(pGameEndManager->Init()))
		{ // 初期化に失敗した場合

			// ゲーム終了演出マネージャーの破棄
			SAFE_DELETE(pGameEndManager);
			return nullptr;
		}

		// インスタンスを保存
		assert(m_pInstance == nullptr);
		m_pInstance = pGameEndManager;

		// 確保したアドレスを返す
		return pGameEndManager;
	}
}

//============================================================
//	何もしないの更新処理
//============================================================
void CGameEndManager::UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//============================================================
//	プレイヤーカメラの更新処理
//============================================================
void CGameEndManager::UpdateLookPlayer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CManager* pManager = GET_MANAGER;			// マネージャー
	CCamera* pCamera = pManager->GetCamera();	// カメラ情報
	if (pCamera->IsGameEnd())
	{ // ゲーム終了カメラの移動が終わった場合

		// スロー倍率を初期化
		pManager->SetSlowRate(1.0f);

		// プレイヤー死亡待機状態にする
		m_state = STATE_DEATH_WAIT;
	}

	// ライト位置の設定
	SetLightPosition();
}

//============================================================
//	プレイヤー死亡待機の更新処理
//============================================================
void CGameEndManager::UpdateDeathWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CManager* pManager = GET_MANAGER;			// マネージャー
	CCamera* pCamera = pManager->GetCamera();	// カメラ情報
	if (m_pDeathPlayer->GetState() == CPlayer::EState::STATE_DEAD_CARRY)
	{ // 真っ白に燃え尽きた場合

		// ゲームセットUIの生成
		m_pGameSetUI = CGameSetUI::Create();

		// 固定カメラの設定
		pCamera->SetState(CCamera::STATE_NONE, false);

		// ゲームセット出現状態にする
		m_state = STATE_GAME_SET;
	}

	// ライト位置の設定
	SetLightPosition();
}

//============================================================
//	ゲームセット出現の更新処理
//============================================================
void CGameEndManager::UpdateGameSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pGameSetUI->IsEnd())
	{ // UIの演出が終了した場合

		// 待機状態にする
		m_state = STATE_WAIT;
	}
}

//============================================================
//	待機の更新処理
//============================================================
void CGameEndManager::UpdateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= WAIT_TIME)
	{ // 待機終了した場合

		// 待機時間の初期化
		m_fCurTime = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}
}

//============================================================
//	終了の更新処理
//============================================================
void CGameEndManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ゲームを終了シーンに変更
	CGameManager::GetInstance()->SetSceneType(CGameManager::ESceneType::SCENE_END);

	// 自身の終了
	Uninit();
}

//============================================================
//	ライト位置の設定処理
//============================================================
void CGameEndManager::SetLightPosition()
{
	const MyLib::Vector3 posPlayer = m_pDeathPlayer->GetPosition();	// プレイヤー位置
	float fRot = 0.0f;	// オフセット方向
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ライト数分繰り返す

		if (m_apLight[i] != nullptr)
		{
			// ライト位置の設定
			MyLib::Vector3 posOffset = MyLib::Vector3(sinf(fRot) * 80.0f, 160.0f, cosf(fRot) * 80.0f);	// オフセット
			m_apLight[i]->SetPosition(posPlayer + posOffset);
		}

		// オフセット方向を回転
		fRot += HALF_PI;
	}
}

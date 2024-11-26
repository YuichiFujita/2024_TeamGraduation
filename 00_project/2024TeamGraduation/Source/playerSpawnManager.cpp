//============================================================
//
//	プレイヤー登場演出マネージャー処理 [playerSpawnManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "playerSpawnManager.h"
#include "specialEffect.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"

#include "game.h"
#include "gameManager.h"
#include "player.h"
#include "playerStatus.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 4;	// 優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CPlayerSpawnManager::AFuncUpdateState CPlayerSpawnManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,							// 何もしない更新
	&CPlayerSpawnManager::UpdateCutIn,	// カットイン更新
	&CPlayerSpawnManager::UpdateEnd,	// 終了更新
};
CPlayerSpawnManager* CPlayerSpawnManager::m_pThisClass = nullptr;	// 自身のインスタンス

//************************************************************
//	親クラス [CPlayerSpawnManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayerSpawnManager::CPlayerSpawnManager() : CObject(PRIORITY),
	m_state		(STATE_NONE),	// 状態
	m_fCurTime	(0.0f)			// 現在の待機時間
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CPlayerSpawnManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CPlayerSpawnManager::~CPlayerSpawnManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayerSpawnManager::Init(void)
{
	// FUJITA：エントリーで外部に保存されたプレイヤー情報を取得しプレイヤーを生成

	// メンバ変数を初期化
	m_state		= STATE_CUTIN;	// 状態
	m_fCurTime	= 0.0f;			// 現在の待機時間

	// 種類をマネージャーにする
	SetType(CObject::TYPE::TYPE_MANAGER);

	// 左チームプレイヤー生成
	for (int i = 0; i < 5; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(-200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pLeftPlayer = CPlayer::Create
		(
			pos + offset, 					// 位置
			CGameManager::SIDE_LEFT,		// チームサイド
			CPlayer::EFieldArea::FIELD_IN,	// ポジション
			CPlayer::EBaseType::TYPE_USER,	// ベースタイプ
			CPlayer::EBody::BODY_GARI,		// 体系
			CPlayer::EHandedness::HAND_L	// 利き手
		);
		if (pLeftPlayer == nullptr)
		{
			return E_FAIL;
		}
	}

	// 右チームプレイヤー生成
	for (int i = 0; i < 5; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pRightPlayer = CPlayer::Create
		(
			pos + offset, 					// 位置
			CGameManager::SIDE_RIGHT,		// チームサイド
			CPlayer::EFieldArea::FIELD_IN,	// ポジション
			CPlayer::EBaseType::TYPE_USER,	// ベースタイプ
			CPlayer::EBody::BODY_DEBU,		// 体系
			CPlayer::EHandedness::HAND_R	// 利き手
		);
		if (pRightPlayer == nullptr)
		{
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayerSpawnManager::Uninit(void)
{
	// オブジェクトを破棄
	Release();

	// 自身の保存していたインスタンスを初期化
	m_pThisClass = nullptr;
}

//============================================================
//	削除処理
//============================================================
void CPlayerSpawnManager::Kill()
{
	// 自身の終了
	Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayerSpawnManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerSpawnManager::Draw()
{

}

//============================================================
//	生成処理
//============================================================
CPlayerSpawnManager *CPlayerSpawnManager::Create()
{
	if (m_pThisClass != nullptr)
	{ // 自クラスの他インスタンスがある場合

		// インスタンスを破棄
		SAFE_UNINIT(m_pThisClass);
	}

	// プレイヤー登場演出マネージャーの生成
	CPlayerSpawnManager *pPlayerSpawnManager = DEBUG_NEW CPlayerSpawnManager;
	if (pPlayerSpawnManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤー登場演出マネージャーの初期化
		if (FAILED(pPlayerSpawnManager->Init()))
		{ // 初期化に失敗した場合

			// プレイヤー登場演出マネージャーの破棄
			SAFE_DELETE(pPlayerSpawnManager);
			return nullptr;
		}

		// インスタンスを保存
		assert(m_pThisClass == nullptr);
		m_pThisClass = pPlayerSpawnManager;

		// 確保したアドレスを返す
		return pPlayerSpawnManager;
	}
}

//============================================================
//	カットインの更新処理
//============================================================
void CPlayerSpawnManager::UpdateCutIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//if ()
	{ // 

		// 終了状態にする
		m_state = STATE_END;
	}
}

//============================================================
//	終了の更新処理
//============================================================
void CPlayerSpawnManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 自身の終了
	Uninit();
}

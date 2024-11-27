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
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "player.h"

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
	nullptr,							// 終了更新
};
CPlayerSpawnManager* CPlayerSpawnManager::m_pInstance = nullptr;	// 自身のインスタンス

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

	//// 左チームプレイヤー生成
	//for (int i = 0; i < 5; i++)
	//{
	//	MyLib::Vector3 pos = MyLib::Vector3(-200.0f, 0.0f, -100.0f);
	//	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
	//	CPlayer* pLeftPlayer = CPlayer::Create
	//	(
	//		pos + offset, 					// 位置
	//		CGameManager::SIDE_LEFT,		// チームサイド
	//		CPlayer::EHuman::HUMAN_SPAWN,	// 人
	//		CPlayer::EBody::BODY_GARI,		// 体系
	//		CPlayer::EHandedness::HAND_L	// 利き手
	//	);
	//	if (pLeftPlayer == nullptr)
	//	{
	//		return E_FAIL;
	//	}
	//}

	//// 右チームプレイヤー生成
	//for (int i = 0; i < 5; i++)
	//{
	//	MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);
	//	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
	//	CPlayer* pRightPlayer = CPlayer::Create
	//	(
	//		pos + offset, 					// 位置
	//		CGameManager::SIDE_RIGHT,		// チームサイド
	//		CPlayer::EHuman::HUMAN_SPAWN,	// 人
	//		CPlayer::EBody::BODY_DEBU,		// 体系
	//		CPlayer::EHandedness::HAND_R	// 利き手
	//	);
	//	if (pRightPlayer == nullptr)
	//	{
	//		return E_FAIL;
	//	}
	//}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayerSpawnManager::Uninit(void)
{
	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		// プレイヤーの終了
		SAFE_UNINIT(pPlayer);
	}

	// オブジェクトを破棄
	Release();

	// 自身の保存していたインスタンスを初期化
	m_pInstance = nullptr;
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

	// 終了状態にする
	m_state = STATE_END;
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
	// インスタンス生成済み
	assert(m_pInstance == nullptr);

	// プレイヤー登場演出マネージャーの生成
	m_pInstance = DEBUG_NEW CPlayerSpawnManager;
	if (m_pInstance == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤー登場演出マネージャーの初期化
		if (FAILED(m_pInstance->Init()))
		{ // 初期化に失敗した場合

			// プレイヤー登場演出マネージャーの破棄
			SAFE_DELETE(m_pInstance);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pInstance;
	}
}

//============================================================
//	インスタンス取得処理
//============================================================
CPlayerSpawnManager* CPlayerSpawnManager::GetInstance()
{
	// インスタンス未生成
	assert(m_pInstance != nullptr);

	// 自身のインスタンスを返す
	return m_pInstance;
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

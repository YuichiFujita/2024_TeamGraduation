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
#include "playerManager.h"
#include "gymWallManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 4;			// 優先順位
	const float	ZLINE_OFFSET = 200.0f;	// Z軸オフセット
	const MyLib::Vector3 POS_LEFT = MyLib::Vector3(-972.85f, 0.0f, 1817.35f);	// 左先頭プレイヤー初期位置
	const MyLib::Vector3 POS_RIGHT = MyLib::Vector3(972.85f, 0.0f, 1817.35f);	// 右先頭プレイヤー初期位置
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CPlayerSpawnManager::AFuncUpdateState CPlayerSpawnManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,								// 何もしない状態
	&CPlayerSpawnManager::UpdateOpenDoor,	// ドア開放状態
	&CPlayerSpawnManager::UpdateWalkAxisZ,	// Z軸移動状態
	&CPlayerSpawnManager::UpdateRotate,		// 回転状態
	&CPlayerSpawnManager::UpdateWalkAxisX,	// X軸移動状態
	&CPlayerSpawnManager::UpdateBow,		// お辞儀状態
	nullptr,								// 終了更新
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
	// プレイヤーマネージャー
	CPlayerManager::Create(CPlayerManager::EType::TYPE_SPAWN);

	// メンバ変数を初期化
	m_state		= STATE_OPEN_DOOR;	// 状態
	m_fCurTime	= 0.0f;				// 現在の待機時間

	// 種類をマネージャーにする
	SetType(CObject::TYPE::TYPE_MANAGER);

	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報

	// カメラ位置を原点にする
	pCameraMotion->SetPosition(VEC3_ZERO);

	// 登場演出モーションを設定
	pCameraMotion->SetMotion(CCameraMotion::MOTION_SPAWN);

	// BGM再生
	CSound* pSound = CSound::GetInstance();
	pSound->StopSound();
	pSound->PlaySound(CSound::ELabel::LABEL_BGM_SPAWN);

	// 体育館の壁生成
	CGymWallManager* pWall = CGameManager::GetInstance()->GetGymWallManager();
	pWall->SetIsWall(true);

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
		if (pPlayer != nullptr)
		{
			pPlayer->Kill();
			pPlayer = nullptr;
		}
	}

	// プレイヤーマネージャー
	CPlayerManager* pManager = CPlayerManager::GetInstance();
	SAFE_UNINIT(pManager);

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
	// 体育館の壁削除
	CGymWallManager* pWall = CGameManager::GetInstance()->GetGymWallManager();
	pWall->SetIsWall(false);

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
// プレイヤー登録処理
//============================================================
void CPlayerSpawnManager::RegistPlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // チームサイドごとの処理
	case CGameManager::ETeamSide::SIDE_LEFT:

		// 左リストに登録
		m_listLeft.Regist(pPlayer);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// 右リストに登録
		m_listRight.Regist(pPlayer);
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
// プレイヤー削除処理
//============================================================
void CPlayerSpawnManager::DeletePlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // チームサイドごとの処理
	case CGameManager::ETeamSide::SIDE_LEFT:

		// 左リストから削除
		m_listLeft.Delete(pPlayer);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// 右リストから削除
		m_listRight.Delete(pPlayer);
		break;

	default:
		assert(false);
		break;
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
//	ドア開放状態の更新処理
//============================================================
void CPlayerSpawnManager::UpdateOpenDoor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const float TIME_END = 1.35f;	// 終了時間

	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
	if (pCameraMotion->GetNowTriggerIdx() <= 0) { return; }		// トリガーが次に行くまで待機

	// モーションのドア開ける瞬間
	CMotion* pMotion = m_listLeft.GetData(0)->GetMotion();
	if (pMotion->GetType() == CPlayer::EMotion::MOTION_DOOR &&
		pMotion->IsImpactFrame(pMotion->GetInfo()))
	{
		// ドアを開放する
		CGameManager* pManager = CGameManager::GetInstance();	// ゲームマネージャー
		pManager->SetEnableOpen(true, 0.2f);
	}

	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // 経過しきった場合

		// 待機時間を初期化
		m_fCurTime = 0.0f;

		// Z軸移動状態にする
		m_state = EState::STATE_WALK_Z;

		// 歩かせる
		m_listLeft.GetData(0)->SetMotion(CPlayer::EMotion::MOTION_WALK_SPAWN);
		m_listRight.GetData(0)->SetMotion(CPlayer::EMotion::MOTION_WALK_SPAWN);
	}
}

//============================================================
//	Z軸移動状態の更新処理
//============================================================
void CPlayerSpawnManager::UpdateWalkAxisZ(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const MyLib::Vector3 LEFT_POSEND = MyLib::Vector3(POS_LEFT.x, 0.0f, -300.0f);	// 終了地点	// TODO：まともな終了地点に
	const MyLib::Vector3 RIGHT_POSEND = MyLib::Vector3(POS_RIGHT.x, 0.0f, -300.0f);	// 終了地点	// TODO：まともな終了地点に
	const float TIME_END = 7.5f;	// 終了時間

	bool bEndState = false;	// 状態終了フラグ

	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // 経過しきった場合

		// 経過時間を補正
		m_fCurTime = TIME_END;

		// 状態の終了を保存
		bEndState = true;
	}

	CGameManager* pManager = CGameManager::GetInstance();	// ゲームマネージャー
	if (pManager->IsOpen() && m_fCurTime >= 1.6f)
	{ // ドアが閉じていない場合

		// ドアをゆっくり閉める
		pManager->SetEnableOpen(false, 1.6f);
	}

	std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// 左チームの最後尾イテレーター
	int nCntLeft = 0;	// 左チームループ回数
	while (m_listLeft.ListLoop(itrLeft))
	{ // 左チームリスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itrLeft);	// プレイヤー情報

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntLeft);	// Z軸オフセット

		// 線形補間で移動
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(POS_LEFT + offset, LEFT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ループ数の加算
		nCntLeft++;
	}

	std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// 右チームの最後尾イテレーター
	int nCntRight = 0;	// 右チームループ回数
	while (m_listRight.ListLoop(itrRight))
	{ // 右チームリスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itrRight);	// プレイヤー情報

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntRight);	// Z軸オフセット

		// 線形補間で移動
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(POS_RIGHT + offset, RIGHT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ループ数の加算
		nCntRight++;
	}

	if (bEndState)
	{ // 状態を終了する場合

		// 待機時間を初期化
		m_fCurTime = 0.0f;

		// 回転状態にする
		m_state = EState::STATE_ROTATE;
	}
}

//============================================================
//	回転状態の更新処理
//============================================================
void CPlayerSpawnManager::UpdateRotate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const MyLib::Vector3 LEFT_ROTEND = MyLib::Vector3(0.0f, -HALF_PI, 0.0f);	// 終了地点	// TODO：まともな終了地点に
	const MyLib::Vector3 RIGHT_ROTEND = MyLib::Vector3(0.0f, HALF_PI, 0.0f);	// 終了地点	// TODO：まともな終了地点に
	const float TIME_END = 0.5f;	// 終了時間

	bool bEndState = false;	// 状態終了フラグ

	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // 経過しきった場合

		// 経過時間を補正
		m_fCurTime = TIME_END;

		// 状態の終了を保存
		bEndState = true;
	}

	std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// 左チームの最後尾イテレーター
	while (m_listLeft.ListLoop(itrLeft))
	{ // 左チームリスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itrLeft);	// プレイヤー情報

		// 線形補間で回転
		pItrPlayer->SetRotation(UtilFunc::Correction::EasingLinear(VEC3_ZERO, LEFT_ROTEND, 0.0f, TIME_END, m_fCurTime));
	}

	std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// 右チームの最後尾イテレーター
	while (m_listRight.ListLoop(itrRight))
	{ // 右チームリスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itrRight);	// プレイヤー情報

		// 線形補間で回転
		pItrPlayer->SetRotation(UtilFunc::Correction::EasingLinear(VEC3_ZERO, RIGHT_ROTEND, 0.0f, TIME_END, m_fCurTime));
	}

	if (bEndState)
	{ // 状態を終了する場合

		// 待機時間を初期化
		m_fCurTime = 0.0f;

		// X軸移動状態にする
		m_state = EState::STATE_WALK_X;


		//-----------------------------
		// 全員まあまあ走る
		//-----------------------------
		std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// 左チームの最後尾イテレーター
		while (m_listLeft.ListLoop(itrLeft))
		{ // 左チームリスト内の要素数分繰り返す

			CPlayer* pItrPlayer = (*itrLeft);	// プレイヤー情報

			// 歩きモーション設定
			CMotionManager::Info info = pItrPlayer->GetMotion()->GetInfo(CPlayer::EMotion::MOTION_WALK);	// モーション情報
			int nKey = rand() % info.nNumKey;														// 開始キー
			float fFrame = (float)(rand() % info.aKey[nKey].nFrame);								// 開始フレーム
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_WALK, nKey, true, fFrame);		// モーション設定
		}

		std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// 右チームの最後尾イテレーター
		while (m_listRight.ListLoop(itrRight))
		{ // 右チームリスト内の要素数分繰り返す

			CPlayer* pItrPlayer = (*itrRight);	// プレイヤー情報

			// 歩きモーション設定
			CMotionManager::Info info = pItrPlayer->GetMotion()->GetInfo(CPlayer::EMotion::MOTION_WALK);	// モーション情報
			int nKey = rand() % info.nNumKey;														// 開始キー
			float fFrame = (float)(rand() % info.aKey[nKey].nFrame);								// 開始フレーム
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_WALK, nKey, true, fFrame);		// モーション設定
		}
	}
}

//============================================================
//	X軸移動状態の更新処理
//============================================================
void CPlayerSpawnManager::UpdateWalkAxisX(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const MyLib::Vector3 LEFT_OLD_POSEND = MyLib::Vector3(POS_LEFT.x, 0.0f, -300.0f);	// 終了地点	// TODO：まともな終了地点に
	const MyLib::Vector3 RIGHT_OLD_POSEND = MyLib::Vector3(POS_RIGHT.x, 0.0f, -300.0f);	// 終了地点	// TODO：まともな終了地点に
	const MyLib::Vector3 LEFT_POSEND = MyLib::Vector3(-100.0f, 0.0f, -300.0f);	// 終了地点	// TODO：まともな終了地点に
	const MyLib::Vector3 RIGHT_POSEND = MyLib::Vector3(100.0f, 0.0f, -300.0f);	// 終了地点	// TODO：まともな終了地点に
	const float TIME_END = 2.5f;	// 終了時間

	bool bEndState = false;	// 状態終了フラグ

	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // 経過しきった場合

		// 経過時間を補正
		m_fCurTime = TIME_END;

		// 状態の終了を保存
		bEndState = true;
	}

	std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// 左チームの最後尾イテレーター
	int nCntLeft = 0;	// 左チームループ回数
	while (m_listLeft.ListLoop(itrLeft))
	{ // 左チームリスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itrLeft);	// プレイヤー情報

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntLeft);	// Z軸オフセット

		// 線形補間で移動
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(LEFT_OLD_POSEND + offset, LEFT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ループ数の加算
		nCntLeft++;
	}

	std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// 右チームの最後尾イテレーター
	int nCntRight = 0;	// 右チームループ回数
	while (m_listRight.ListLoop(itrRight))
	{ // 右チームリスト内の要素数分繰り返す

		CPlayer* pItrPlayer = (*itrRight);	// プレイヤー情報

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntRight);	// Z軸オフセット

		// 線形補間で移動
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(RIGHT_OLD_POSEND + offset, RIGHT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ループ数の加算
		nCntRight++;
	}

	if (bEndState)
	{ // 状態を終了する場合

		// 待機時間を初期化
		m_fCurTime = 0.0f;

		// お辞儀状態にする
		m_state = EState::STATE_BOW;

		//-----------------------------
		// 全員礼
		//-----------------------------
		std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// 左チームの最後尾イテレーター
		while (m_listLeft.ListLoop(itrLeft))
		{ // 左チームリスト内の要素数分繰り返す

			CPlayer* pItrPlayer = (*itrLeft);	// プレイヤー情報

			// 礼モーション
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_BOW);
		}

		std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// 右チームの最後尾イテレーター
		while (m_listRight.ListLoop(itrRight))
		{ // 右チームリスト内の要素数分繰り返す

			CPlayer* pItrPlayer = (*itrRight);	// プレイヤー情報

			// 礼モーション
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_BOW);
		}
	}
}

//============================================================
//	お辞儀状態の更新処理
//============================================================
void CPlayerSpawnManager::UpdateBow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

#if 0
	const float TIME_END = 1.0f;	// 終了時間

	// TODO：ここはお辞儀モーションから遷移タイミングを決める

	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報

	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END
	&&  pCameraMotion->IsFinish())
	{ // 経過しきった場合

		// 待機時間を初期化
		m_fCurTime = 0.0f;

		// 終了状態にする
		m_state = EState::STATE_END;
	}
#else

	// 先頭取得
	CPlayer* pFirstPlayer = m_listRight.GetData(0);
	CMotion* pMotion = pFirstPlayer->GetMotion();

	if (pMotion->GetType() == CPlayer::EMotion::MOTION_BOW &&
		pMotion->IsFinish())
	{// 礼 && 終了

		// 待機時間を初期化
		m_fCurTime = 0.0f;

		// 終了状態にする
		m_state = EState::STATE_END;
	}

#endif
}

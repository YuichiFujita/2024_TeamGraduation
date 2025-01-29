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
#include "specialEffect.h"
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
#include "gymWallManager.h"
#include "spotlight.h"
#include "playerManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	PRIORITY = 4;			// 優先順位
	const float	LIGHT_RANGE = 600.0f;	// 光源範囲
	const float LIGHTUP_TIME = 1.2f;	// 明るくなるまでの時間
	const MyLib::Vector3 LIGHT_POS = MyLib::Vector3(0.0f, 160.0f, 0.0f);	// ライトオフセット

	const float	JUMP_HEIGHT = 1000.0f;		// ジャンプの最高到達点
	const float	JUMP_TIME = 1.24f;			// ジャンプ時間
	const float	JUMP_OFFSET_TEAM = 150.0f;	// 攻撃プレイヤーチームメンバーのジャンプオフセット
	const MyLib::Vector3 JUMP_POS_TEAM[] = { MyLib::Vector3(-300.0f, 0.0f, 400.0f), MyLib::Vector3(300.0f, 0.0f, 400.0f) };	// 攻撃プレイヤーチームメンバーのジャンプ位置

	namespace hype
	{
		namespace trans
		{
			const float END_TIME = 4.2f;
		}
	}

	namespace follow
	{
		namespace trans
		{
			const float END_TIME = 1.8f;
		}
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CSpecialManager::AFuncUpdateState CSpecialManager::m_aFuncUpdateState[] =	// 状態更新関数
{
	nullptr,								// 何もしない更新
	&CSpecialManager::UpdateCutIn,			// カットイン更新
	&CSpecialManager::UpdateHypeTrans,		// 盛り上がり遷移更新
	&CSpecialManager::UpdateHype,			// 盛り上がり更新
	&CSpecialManager::UpdateStag,			// スペシャル演出更新
	&CSpecialManager::UpdateFollowTrans,	// 追従遷移更新
	&CSpecialManager::UpdateEnd,			// 終了更新
};
CSpecialManager::AFuncUpdateSpecial CSpecialManager::m_aFuncUpdateSpecial[] =	// スペシャル更新関数
{
	nullptr,							// スペシャル無し
	&CSpecialManager::UpdateKamehameha,	// かめはめ波の更新
};
CSpecialManager* CSpecialManager::m_pInstance = nullptr;	// 自身のインスタンス

//************************************************************
//	親クラス [CSpecialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSpecialManager::CSpecialManager(CPlayer* pAttack, CPlayer* pTarget) : CObject(PRIORITY),
	m_pCenterLight	(nullptr),		// ライト情報
	m_pAttackLight	(nullptr),		// 攻撃プレイヤーを照らすライト
	m_pTargetLight	(nullptr),		// 標的プレイヤーを照らすライト
	m_pAttackPlayer	(pAttack),		// 攻撃プレイヤー
	m_pTargetPlayer	(pTarget),		// 標的プレイヤー
	m_pCutIn		(nullptr),		// カットイン情報
	m_state			(STATE_NONE),	// 状態
	m_fCurTime		(0.0f),			// 現在の待機時間
	m_fJumpTime		(0.0f),			// 現在のジャンプ時間
	m_bJump			(false),		// ジャンプフラグ
	m_vecJump		({})			// ジャンププレイヤー情報
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncUpdateState)   == CSpecialManager::STATE_MAX, "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_aFuncUpdateSpecial) == CBall::SPECIAL_MAX,         "ERROR : Special Count Mismatch");
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
	m_state = STATE_CUTIN;	// 状態

	// 種類をマネージャーにする
	SetType(CObject::TYPE::TYPE_MANAGER);

	// 世界停止中に動けるようにする
	SetEnablePosibleMove_WorldPause(true);

	// カットインの生成
	m_pCutIn = CCutIn::Create(m_pAttackPlayer);
	if (m_pCutIn == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 両チームの観客を通常状態にする
	CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_LEFT);
	CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_RIGHT);

	// 世界の時を止める
	GET_MANAGER->SetEnableWorldPaused(true);

	// タイマー停止
	CGameManager::GetInstance()->SetEnableTimerStop(true);

	// ゲームをスペシャル演出シーンに変更
	CGameManager::GetInstance()->SetSceneType(CGameManager::ESceneType::SCENE_SPECIAL);

	// ジャンプモーションにする
	m_pAttackPlayer->SetMotion(CPlayer::EMotion::MOTION_JUMP);

	// 通常カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_NONE, false);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSpecialManager::Uninit(void)
{
	// ライトの終了
	SAFE_UNINIT(m_pCenterLight);

	// 攻撃プレイヤーを照らすライトの終了
	SAFE_UNINIT(m_pAttackLight);

	// 標的プレイヤーを照らすライトの終了
	SAFE_UNINIT(m_pTargetLight);

	// オブジェクトを破棄
	Release();

	// 自身の保存していたインスタンスを初期化
	m_pInstance = nullptr;
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

	if (m_pAttackPlayer->GetBall() != nullptr)
	{ // ボールを投げていない場合

		CBall::ESpecial typeSpecial = CGameManager::GetInstance()->GetBall()->GetTypeSpecial();	// スペシャル種類
		assert(typeSpecial > CBall::SPECIAL_NONE && typeSpecial < CBall::SPECIAL_MAX);
		if (m_aFuncUpdateState[typeSpecial] != nullptr)
		{ // 更新関数が指定されている場合

			// 各スペシャルごとの更新
			(this->*(m_aFuncUpdateSpecial[typeSpecial]))(fDeltaTime, fDeltaRate, fSlowRate);
		}
	}

	if (m_bJump)
	{ // ジャンプ中の場合

		// 経過時間を加算
		m_fJumpTime += fDeltaTime * fSlowRate;
		if (m_fJumpTime >= JUMP_TIME * 0.5f)
		{ // 一番てっぺんまで言った場合

			// 経過時間を加速
			m_fJumpTime += m_fJumpTime * 0.0075f;
		}

		// 経過時間の割合を計算
		float fTimeRate = m_fJumpTime / JUMP_TIME;
		fTimeRate = UtilFunc::Transformation::Clamp(fTimeRate, 0.0f, 1.0f);	// 割合を補正

		for (const auto& rJump : m_vecJump)
		{ // ジャンプするプレイヤー数分繰り返す

			// ジャンプさせる
			MyLib::Vector3 posPlayer = UtilFunc::Calculation::GetParabola3D(rJump.posJumpStart, rJump.posJumpEnd, JUMP_HEIGHT, fTimeRate);
			if (m_fJumpTime >= JUMP_TIME)
			{ // 時間が経過しきった場合

				// ジャンプ終了
				m_bJump = false;

				// ヒーロー着地モーションにする
				rJump.pPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND_SP);

				// カメラ揺れ
				GET_MANAGER->GetCamera()->SetSwing(CCamera::SSwing(15.0f, 2.0f, 0.4f));

				// サウンドの再生
				PLAY_SOUND(CSound::ELabel::LABEL_SE_SP_LAND);
			}

			// 位置を設定
			rJump.pPlayer->SetPosition(posPlayer);
		}
	}

	// ライト位置の設定
	SetLightPosition();
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
CSpecialManager *CSpecialManager::Create(CPlayer* pAttack, CPlayer* pTarget)
{
	if (m_pInstance != nullptr)
	{ // 自クラスの他インスタンスがある場合

		// インスタンスを破棄
		SAFE_UNINIT(m_pInstance);
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
		assert(m_pInstance == nullptr);
		m_pInstance = pSpecialManager;

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

		CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報

		// 世界の時はうごきだす
		GET_MANAGER->SetEnableWorldPaused(false);

		// カットインの終了
		SAFE_UNINIT(m_pCutIn);

		// 攻撃側プレイヤーチームの観客を盛り上げる
		CAudience::SetSpecialAll(m_pAttackPlayer->GetTeam());

		// 壁の表示をONにする
		CGameManager* pGameManager = CGameManager::GetInstance();		// ゲームマネージャー
		CGymWallManager* pGymWall = pGameManager->GetGymWallManager();	// 体育館壁マネージャー
		pGymWall->SetIsWall(true);

		// カメラの原点を設定
		pCamera->SetPositionROrigin(pCamera->GetPositionR());	// 注視点
		pCamera->SetOriginRotation(pCamera->GetRotation());		// 向き
		pCamera->SetDistanceOrigin(pCamera->GetDistance());		// 距離

		// 体育館暗くする
		SetDarkGym();

		// 盛り上がり遷移状態にする
		m_state = STATE_HYPE_TRANS;
	}
}

//============================================================
//	盛り上がり遷移の更新処理
//============================================================
void CSpecialManager::UpdateHypeTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
	CCameraMotion::MotionKey key = pCameraMotion->GetKeyData(CCameraMotion::MOTION::MOTION_SPECIAL_HYPE, 0);	// 先頭キー情報
	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;			// カメラモーションの反転フラグ
	if (bInverse)
	{ // 反転する場合

		// 向きをY軸に反転させる
		key.rotDest.y *= -1.0f;

		// 向きを正規化
		UtilFunc::Transformation::RotNormalize(key.rotDest.y);
	}

	// カメラ情報の取得
	MyLib::Vector3 posR	= pCamera->GetPositionR();	// 注視点
	MyLib::Vector3 rot	= pCamera->GetRotation();	// 向き
	float fDis			= pCamera->GetDistance();	// 距離

	// 現在の待機時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;

	// 追加で時間を加算
	m_fCurTime += UtilFunc::Correction::EasingQuintOut(0.08f, 0.01f, 0.0f, hype::trans::END_TIME, m_fCurTime);

	// カメラ情報の線形補正
	posR = UtilFunc::Correction::EasingQuintOut(pCamera->GetPositionROrigin(), key.posRDest + GetDestAttackPosition(), 0.0f, hype::trans::END_TIME, m_fCurTime);
	rot  = UtilFunc::Correction::EasingQuintOut(pCamera->GetOriginRotation(),  key.rotDest,  0.0f, hype::trans::END_TIME, m_fCurTime);
	fDis = UtilFunc::Correction::EasingQuintOut(pCamera->GetDistanceOrigin(),  key.distance, 0.0f, hype::trans::END_TIME, m_fCurTime);

	// カメラ情報の反映
	pCamera->SetPositionR(posR);	// 注視点
	pCamera->SetRotation(rot);		// 向き
	pCamera->SetDistance(fDis);		// 距離

	if (m_fCurTime >= hype::trans::END_TIME)
	{ // 時間が経過しきった場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// カメラ情報の補正
		pCamera->SetPositionR(key.posRDest + GetDestAttackPosition());	// 注視点
		pCamera->SetRotation(key.rotDest);	// 向き
		pCamera->SetDistance(key.distance);	// 距離

		// カメラ位置を攻撃プレイヤーの位置にする
		pCameraMotion->SetPosition(GetDestAttackPosition());

		// スペシャル盛り上げモーションを設定
		pCameraMotion->SetMotion(CCameraMotion::MOTION_SPECIAL_HYPE, bInverse, true, true, true);

		// 盛り上がり状態にする
		m_state = STATE_HYPE;
	}
}

//============================================================
//	盛り上がりの更新処理
//============================================================
void CSpecialManager::UpdateHype(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
	if (pCameraMotion->IsFinish())
	{ // カメラモーションが終了した場合

		// 攻撃側プレイヤーにスペシャル攻撃を設定
		m_pAttackPlayer->SetSpecialAttack();

		// プレイヤースペシャル演出状態にする
		m_state = STATE_STAG;
	}
}

//============================================================
//	スペシャル演出の更新処理
//============================================================
void CSpecialManager::UpdateStag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
	if (pCameraMotion->IsFinish())
	{ // カメラモーションが終わった場合

		// 壁の表示をOFFにする
		CGameManager* pGameManager = CGameManager::GetInstance();		// ゲームマネージャー
		CGymWallManager* pGymWall = pGameManager->GetGymWallManager();	// 体育館壁マネージャー
		pGymWall->SetIsWall(false);

		// 追従遷移状態にする
		m_state = STATE_FOLLOW_TRANS;
	}
}

//============================================================
//	追従遷移の更新処理
//============================================================
void CSpecialManager::UpdateFollowTrans(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// カメラ情報の取得
	CCamera* pCamera	= GET_MANAGER->GetCamera();	// カメラ情報
	MyLib::Vector3 posR	= pCamera->GetPositionR();	// 注視点
	MyLib::Vector3 rot	= pCamera->GetRotation();	// 向き
	float fDis			= pCamera->GetDistance();	// 距離
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
	CCamera::SCameraPoint key = pCamera->CameraPoint();			// 追従目標情報
	CCameraMotion::MotionKey keyEnd = pCameraMotion->GetKeyData((CCameraMotion::MOTION)pCameraMotion->GetNowMotionIdx(), pCameraMotion->GetNowKeyMax() - 1);	// 終端キー情報

	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// カメラモーションの反転フラグ
	if (bInverse)
	{ // 反転する場合

		// 向きをY軸に反転させる
		keyEnd.rotDest.y *= -1.0f;
		key.rot.y *= -1.0f;

		// 向きを正規化
		UtilFunc::Transformation::RotNormalize(keyEnd.rotDest.y);
		UtilFunc::Transformation::RotNormalize(key.rot.y);
	}

	// 現在の待機時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;

	// カメラ情報の線形補正
	posR = UtilFunc::Correction::EaseInOutExpo(keyEnd.posRDest + pCameraMotion->GetPosition(), key.posR, 0.0f, follow::trans::END_TIME, m_fCurTime);
	rot  = UtilFunc::Correction::EaseInOutExpo(keyEnd.rotDest,  key.rot,       0.0f, follow::trans::END_TIME, m_fCurTime);
	fDis = UtilFunc::Correction::EaseInOutExpo(keyEnd.distance, key.fDistance, 0.0f, follow::trans::END_TIME, m_fCurTime);

	// カメラ情報の反映
	pCamera->SetPositionR(posR);	// 注視点
	pCamera->SetRotation(rot);		// 向き
	pCamera->SetDistance(fDis);		// 距離

	if (m_fCurTime >= follow::trans::END_TIME)
	{ // 時間が経過しきった場合

		// タイマーを初期化
		m_fCurTime = 0.0f;

		// カメラ情報の補正
		pCamera->SetPositionR(key.posR);		// 注視点
		pCamera->SetRotation(key.rot);			// 向き
		pCamera->SetDistance(key.fDistance);	// 距離

		// 終了状態にする
		m_state = STATE_END;
	}
}

//============================================================
//	終了の更新処理
//============================================================
void CSpecialManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 体育館を明るくする
	GET_MANAGER->GetLight()->SetEnableBright(true, LIGHTUP_TIME);

	// 攻撃側プレイヤーチームの観客を通常状態にする
	CAudience::SetEnableJumpAll(false, m_pAttackPlayer->GetTeam());

	// ゲームをメインシーンに変更
	CGameManager* pGameManager = CGameManager::GetInstance();	// ゲームマネージャー
	pGameManager->SetSceneType(CGameManager::ESceneType::SCENE_MAIN);

	// 追従カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_FOLLOW);

	// スペシャル演出の終了時設定
	CSpecialEffect* pSPEffect = m_pAttackPlayer->GetSpecialEffect();
	if (pSPEffect != nullptr)
	{
		pSPEffect->FinishSetting();
	}

	// タイマー再開
	CGameManager::GetInstance()->SetEnableTimerStop(false);

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
//	攻撃プレイヤー目標位置の取得処理
//============================================================
MyLib::Vector3 CSpecialManager::GetDestAttackPosition() const
{
	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// カメラモーションの反転フラグ
	float fOffset = (bInverse) ? 1.0f : -1.0f;	// オフセット方向
	return MyLib::Vector3(800.0f * fOffset, 0.0f, 0.0f);
}

//============================================================
//	プレイヤー盛り上げ位置の設定処理
//============================================================
void CSpecialManager::SetPlayerHypePosition()
{
	bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// カメラモーションの反転フラグ
	float fOffset = (bInverse) ? 1.0f : -1.0f;	// オフセット方向
	SPlayerJump info;

	// プレイヤーを保存
	info.pPlayer = m_pAttackPlayer;

	// ジャンプ開始位置を設定
	info.posJumpStart = m_pAttackPlayer->GetPosition();

	// ジャンプ終了位置を設定
	info.posJumpEnd = GetDestAttackPosition();

	// 攻撃プレイヤーの向きを設定
	m_pAttackPlayer->SetRotation(MyLib::Vector3(0.0f, HALF_PI * fOffset, 0.0f));
	m_pAttackPlayer->SetRotDest(HALF_PI * fOffset);

	// ジャンプ強制終了
	m_pAttackPlayer->SetEnableJump(false);

	// ジャンプ中にする
	m_bJump = true;

	// 最後尾に追加
	m_vecJump.push_back(info);

	// サウンドの再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_SP_JUMP);
}

//============================================================
//	攻撃チームのジャンプの設定処理
//============================================================
void CSpecialManager::SetJumpAttackTeam()
{
	int nNumJump = 0;	// ジャンププレイヤー数
	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	CGameManager::ETeamSide team = m_pAttackPlayer->GetTeam();	// 攻撃プレイヤーチーム
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		// 死亡状態の場合次へ
		if (pPlayer->IsDeathState()) { continue; }

		// 内野じゃない場合次へ
		if (pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN) { continue; }
		
		// 攻撃プレイヤーチームじゃない場合次へ
		if (pPlayer->GetTeam() != team) { continue; }

		// 攻撃プレイヤーと同一の場合次へ
		if (pPlayer == m_pAttackPlayer) { continue; }

		// ジャンプするプレイヤーを追加する
		SPlayerJump info = {};

		// プレイヤーを保存
		info.pPlayer = pPlayer;

		// ジャンプ開始位置を設定
		info.posJumpStart = pPlayer->GetPosition();

		// ジャンプ終了位置を設定
		float fTeam = (team == CGameManager::ETeamSide::SIDE_LEFT) ? 1.0f : -1.0f;	// チームオフセット
		info.posJumpEnd = JUMP_POS_TEAM[team] + MyLib::Vector3(JUMP_OFFSET_TEAM * fTeam * (float)nNumJump, 0.0f, 0.0f);

		// 最後尾に追加
		m_vecJump.push_back(info);

		// プレイヤーの向きを設定
		pPlayer->SetRotation(VEC3_ZERO);
		pPlayer->SetRotDest(0.0f);

		// ジャンプ強制終了
		pPlayer->SetEnableJump(false);

		// ジャンプ人数加算
		nNumJump++;
	}
}

//============================================================
//	ライト位置の設定処理
//============================================================
void CSpecialManager::SetLightPosition()
{
	// 攻撃プレイヤーの位置に移動
	if (m_pAttackLight != nullptr)
	{
		MyLib::Vector3 posLight = (m_bJump) ? GetDestAttackPosition() : m_pAttackPlayer->GetPosition();
		m_pAttackLight->SetLightPosition(posLight);
	}

	// 標的プレイヤーの位置に移動
	if (m_pTargetLight != nullptr)
	{
		m_pTargetLight->SetLightPosition(m_pTargetPlayer->GetPosition());
	}
}

//============================================================
//	体育館の暗くする設定処理
//============================================================
HRESULT CSpecialManager::SetDarkGym()
{
	// 中心ライトの生成
	m_pCenterLight = CLightPoint::Create();
	if (m_pCenterLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 拡散光を設定
	m_pCenterLight->SetDiffuse(MyLib::color::White());

	// 光源範囲を設定
	m_pCenterLight->SetRange(LIGHT_RANGE);

	// 位置を設定
	m_pCenterLight->SetPosition(LIGHT_POS);

	// 攻撃プレイヤーを照らすライトの生成
	m_pAttackLight = CSpotLight::Create();
	if (m_pAttackLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 標的プレイヤーを照らすライトの生成
	m_pTargetLight = CSpotLight::Create();
	if (m_pTargetLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 体育館を暗くする
	GET_MANAGER->GetLight()->SetEnableBright(false);

	// プレイヤー盛り上げ位置の設定
	SetPlayerHypePosition();

	// 攻撃チームのジャンプ設定
	SetJumpAttackTeam();

	// 標的チーム/外野の警戒設定
	CPlayerManager* pPlayerManager = CPlayerManager::GetInstance();
	pPlayerManager->CautionInAll(m_pTargetPlayer->GetTeam());
	pPlayerManager->CautionOutAll();

	return S_OK;
}

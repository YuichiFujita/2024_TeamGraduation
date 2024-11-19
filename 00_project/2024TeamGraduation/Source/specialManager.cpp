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

	namespace hype
	{
		namespace trans
		{
			const float END_TIME = 2.6f;
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
CSpecialManager* CSpecialManager::m_pThisClass = nullptr;	// 自身のインスタンス

//************************************************************
//	親クラス [CSpecialManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSpecialManager::CSpecialManager(CPlayer* pAttack, CPlayer* pTarget) : CObject(PRIORITY),
	m_pAttackPlayer	(pAttack),		// 攻撃プレイヤー
	m_pTargetPlayer	(pTarget),		// 標的プレイヤー
	m_pAttackLight	(nullptr),		// 攻撃プレイヤーを照らすライト
	m_pTargetLight	(nullptr),		// 標的プレイヤーを照らすライト
	m_pCutIn		(nullptr),		// カットイン情報
	m_state			(STATE_NONE),	// 状態
	m_fCurTime		(0.0f)			// 現在の待機時間
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

	// 両チームの観客を通常状態にする
	CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_LEFT);
	CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_RIGHT);

#if 1
	// 体育館を暗くする
	GET_MANAGER->GetLight()->SetEnableBright(false);
#endif

#if 1
	// 世界の時を止める
	GET_MANAGER->SerEnableWorldPaused(true);
#endif

#if 1
	// ゲームをスペシャル演出シーンに変更
	CGameManager::GetInstance()->SetSceneType(CGameManager::ESceneType::SCENE_SPECIAL_STAG);
#endif

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

		CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
		CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報
		bool bInverse = (m_pAttackPlayer->GetTeam() == CGameManager::ETeamSide::SIDE_LEFT) ? false : true;	// カメラモーションの反転フラグ

		// 世界の時はうごきだす
		GET_MANAGER->SerEnableWorldPaused(false);

		// カットインの終了
		SAFE_UNINIT(m_pCutIn);

		// 攻撃側プレイヤーチームの観客を盛り上げる
		CAudience::SetSpecialAll(m_pAttackPlayer->GetTeam());

		// プレイヤー盛り上げ位置の設定
		SetPlayerHypePosition(bInverse);

		// カメラの原点を設定
		pCamera->SetPositionROrigin(pCamera->GetPositionR());	// 注視点
		pCamera->SetOriginRotation(pCamera->GetRotation());		// 向き
		pCamera->SetDistanceOrigin(pCamera->GetDistance());		// 距離

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
	posR = UtilFunc::Correction::EasingQuintOut(pCamera->GetPositionROrigin(), key.posRDest + m_pAttackPlayer->GetPosition(), 0.0f, hype::trans::END_TIME, m_fCurTime);
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
		pCamera->SetPositionR(key.posRDest + m_pAttackPlayer->GetPosition());	// 注視点
		pCamera->SetRotation(key.rotDest);	// 向き
		pCamera->SetDistance(key.distance);	// 距離

		// カメラ位置を攻撃プレイヤーの位置にする
		pCameraMotion->SetPosition(m_pAttackPlayer->GetPosition());

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
	CCamera::SCameraData key = pCamera->FollowPoint();			// 追従目標情報
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

#if 0
	// TODO：投げた瞬間の解除はちょっと...
	if (m_pAttackPlayer->GetBall() == nullptr)
	{ // ボールを投げている場合

		// 終了状態にする
		m_state = STATE_END;
	}
#endif
}

//============================================================
//	終了の更新処理
//============================================================
void CSpecialManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 体育館を明るくする
	GET_MANAGER->GetLight()->SetEnableBright(true);

	// 攻撃側プレイヤーチームの観客を通常状態にする
	CAudience::SetEnableJumpAll(false, m_pAttackPlayer->GetTeam());

	// ゲームをメインシーンに変更
	CGameManager* pGameManager = CGameManager::GetInstance();	// ゲームマネージャー
	pGameManager->SetSceneType(CGameManager::ESceneType::SCENE_MAIN);

	// 追従カメラの設定
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_FOLLOW);

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
//	プレイヤー盛り上げ位置の設定処理
//============================================================
void CSpecialManager::SetPlayerHypePosition(const bool bInverse)
{
	float fOffset = (bInverse) ? 1.0f : -1.0f;	// オフセット方向

	// 攻撃プレイヤーの位置を設定
	m_pAttackPlayer->SetPosition(MyLib::Vector3(800.0f * fOffset, 0.0f, 0.0f));

	// 攻撃プレイヤーの向きを設定
	m_pAttackPlayer->SetRotation(MyLib::Vector3(0.0f, HALF_PI * fOffset, 0.0f));
	m_pAttackPlayer->SetRotDest(HALF_PI * fOffset);
}

//============================================================
//	ライト位置の設定処理
//============================================================
void CSpecialManager::SetLightPosition()
{
	if (m_pAttackLight != nullptr)
	{
		// 攻撃プレイヤーの位置に移動
		m_pAttackLight->SetPosition(m_pAttackPlayer->GetPosition() + LIGHT_OFFSET);
	}

	if (m_pAttackLight != nullptr)
	{
		// 標的プレイヤーの位置に移動
		m_pTargetLight->SetPosition(m_pTargetPlayer->GetPosition() + LIGHT_OFFSET);
	}
}

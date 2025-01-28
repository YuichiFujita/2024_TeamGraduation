//=============================================================================
// 
// スペシャル演出(かめはめ波)処理 [specialEffect_kamehameha.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "specialEffect_kamehameha.h"
#include "player.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float SCALE_CHARGESTART = 5.0f;	// チャージ開始
	const float SCALE_CHARGEEND = 50.0f;	// チャージ終了
	const float SCALE_CONDENCEEND = 3.0f;	// 凝縮終了
}

namespace Wind	// 風
{
	const float INTERVAL = 0.3f;	// 間隔
	const int RANDRANGE_SCALE = 5;	// スケールのランダム値
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// トリガー時
CSpecialEffect_Kamehame::TRIGGER_FUNC CSpecialEffect_Kamehame::m_TriggerFunc[] =
{
	&CSpecialEffect_Kamehame::Trigger_Brust,		// 発射
	&CSpecialEffect_Kamehame::Trigger_Stance,		// 構え
	&CSpecialEffect_Kamehame::Trigger_CreateEnergy,	// かめはめ波の中心生成時
	&CSpecialEffect_Kamehame::Trigger_ChargeStart,	// チャージ開始時
};

// 進行中
CSpecialEffect_Kamehame::PROGRESS_FUNC CSpecialEffect_Kamehame::m_ProgressFunc[] =
{
	&CSpecialEffect_Kamehame::Progress_Brust,		// 発射
	&CSpecialEffect_Kamehame::Progress_ChargeStart,	// チャージ開始
	&CSpecialEffect_Kamehame::Progress_Chargeing,	// チャージ中
	&CSpecialEffect_Kamehame::Progress_Condense,	// 凝縮
	&CSpecialEffect_Kamehame::Progress_TransBrust,	// 発射へ遷移
};

//==========================================================================
// コンストラクタ
//==========================================================================
CSpecialEffect_Kamehame::CSpecialEffect_Kamehame() : CSpecialEffect(),
	m_bWindCreate(false),				// 風生成フラグ
	m_fIntervalWind(Wind::INTERVAL),	// 風生成インターバル
	m_fCreateWindTime(0.0f)				// 風生成タイマー
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CSpecialEffect_Kamehame::~CSpecialEffect_Kamehame()
{
	// 漂う空間オーラ
	SAFE_UNINIT(m_pAtmosphere);

	// チャージ時の雷
	SAFE_UNINIT(m_pChargeThunder);

	// がれき
	SAFE_UNINIT(m_pBallast);
}

//==========================================================================
// 更新
//==========================================================================
void CSpecialEffect_Kamehame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 風更新
	UpdateWind(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 終了時の設定
//==========================================================================
void CSpecialEffect_Kamehame::FinishSetting()
{
	// 漂う空間オーラ
	m_pAtmosphere->SetTrigger(0);
	m_pAtmosphere->DeleteLater(1.0f);
	m_pAtmosphere = nullptr;

	// チャージ時の雷
	m_pChargeThunder->SetTrigger(0);
	m_pChargeThunder->DeleteLater(1.0f);
	m_pChargeThunder = nullptr;

	// がれき
	m_pBallast->SetTrigger(0);
	m_pBallast->DeleteLater(1.0f);
	m_pBallast = nullptr;

	// サウンド停止
	CSound::GetInstance()->StopSound(CSound::ELabel::LABEL_SE_KMHM_LOOP);
}

//==========================================================================
// 風更新
//==========================================================================
void CSpecialEffect_Kamehame::UpdateWind(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 風生成しない場合は抜ける
	if (!m_bWindCreate) return;

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 風生成タイマー加算
	m_fCreateWindTime += fDeltaTime * fSlowRate;

	if (m_fIntervalWind <= m_fCreateWindTime)
	{// インターバル経過

		// タイマーリセット
		m_fCreateWindTime = 0.0f;

		// インターバル
		m_fIntervalWind = Wind::INTERVAL + UtilFunc::Transformation::Random(-30, 10) * 0.01f;

		// 風生成
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_CHARGE_WIND,
			pPlayer->GetPosition(),
			MyLib::Vector3(),
			MyLib::Vector3(),
			15.0f + UtilFunc::Transformation::Random(-Wind::RANDRANGE_SCALE, Wind::RANDRANGE_SCALE) * 0.1f, true);

	}
}

//==========================================================================
// 発射
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Brust(CMotionManager::AttackInfo ATKInfo)
{
	// かめはめ波のエネルギー部分削除
	if (m_pEnergy != nullptr)
	{
		m_pEnergy->SetTrigger(0);
		m_pEnergy->Uninit();
		m_pEnergy = nullptr;
	}

	// TODO : 発射
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);

	// かめはめ波のエネルギー部分生成
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_BRUST,
		posAtkInfo,
		MyLib::Vector3(0.0f, pPlayer->GetRotation().y, 0.0f),
		MyLib::Vector3(),
		20.0f, true);

	// サウンドの再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_BRUST);
}

//==========================================================================
// 構え
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Stance(CMotionManager::AttackInfo ATKInfo)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();

	// 漂う空間オーラ
	m_pAtmosphere = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ATMOSPHERE,
		pPlayer->GetPosition(),
		MyLib::Vector3(),
		MyLib::Vector3(),
		40.0f, false);
}

//==========================================================================
// かめはめ波の中心(開始)生成時
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_CreateEnergy(CMotionManager::AttackInfo ATKInfo)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// かめはめ波の中心(開始時)生成
	m_pEnergyStart = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ENERGY,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		SCALE_CHARGESTART, false);

	// チャージの雷生成
	m_pChargeThunder = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_CHARGE_THUNDER,
		playerPos,
		MyLib::Vector3(),
		MyLib::Vector3(),
		40.0f, false);

	// がれき生成
	m_pBallast = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_BALLAST,
		playerPos,
		MyLib::Vector3(),
		MyLib::Vector3(),
		20.0f, false);

	// 風生成をON
	m_bWindCreate = true;

	// サウンドの再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_START);

	// サウンドの再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_CHARGE);
}

//==========================================================================
// チャージ開始時
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_ChargeStart(CMotionManager::AttackInfo ATKInfo)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);

	// かめはめ波の中心(開始時)終了
	m_pEnergyStart->SetTrigger(0);
	SAFE_UNINIT(m_pEnergyStart);

	// かめはめ波のエネルギー部分生成
	m_pEnergy = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ENERGY,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		SCALE_CHARGESTART, false);

	// 開始の風生成
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_CHARGE,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		20.0f, true);

	// フラッシュ生成
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_FLASH,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		40.0f, true);

	// サウンド停止
	CSound::GetInstance()->StopSound(CSound::ELabel::LABEL_SE_KMHM_CHARGE);

	// サウンドの再生
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_LOOP);
}




//==========================================================================
// チャージ開始
//==========================================================================
void CSpecialEffect_Kamehame::Progress_ChargeStart(CMotionManager::AttackInfo ATKInfo)
{
	if (m_pEnergy == nullptr) return;

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// チャージ中
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Chargeing(CMotionManager::AttackInfo ATKInfo)
{
	if (m_pEnergy == nullptr) return;

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 割合に応じて拡大
	m_pEnergy->SetScale(SCALE_CHARGESTART + (SCALE_CHARGEEND - SCALE_CHARGESTART) * ATKInfo.fCntRatio);

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// 凝縮
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Condense(CMotionManager::AttackInfo ATKInfo)
{
	if (m_pEnergy == nullptr) return;

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 割合に応じて拡大
	m_pEnergy->SetScale(SCALE_CHARGEEND + (SCALE_CONDENCEEND - SCALE_CHARGEEND) * ATKInfo.fCntRatio);

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// 発射へ遷移
//==========================================================================
void CSpecialEffect_Kamehame::Progress_TransBrust(CMotionManager::AttackInfo ATKInfo)
{
	// 風生成停止
	m_bWindCreate = false;

	if (m_pEnergy == nullptr) return;

	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// 発射
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Brust(CMotionManager::AttackInfo ATKInfo)
{
	
}
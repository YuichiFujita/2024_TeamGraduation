//=============================================================================
// 
// スペシャル演出(かめはめ波)処理 [specialEffect_kamehameha.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "specialEffect_kamehameha.h"
#include "player.h"
#include "sound.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float SCALE_CHARGESTART = 5.0f;	// チャージ開始
	const float SCALE_CHARGEEND = 50.0f;	// チャージ終了
	const float SCALE_CONDENCEEND = 3.0f;	// 凝縮終了
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
};

//==========================================================================
// コンストラクタ
//==========================================================================
CSpecialEffect_Kamehame::CSpecialEffect_Kamehame() : CSpecialEffect()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CSpecialEffect_Kamehame::~CSpecialEffect_Kamehame()
{

}


//==========================================================================
// 発射
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Brust(CMotion::AttackInfo ATKInfo)
{
	// かめはめ波のエネルギー部分削除
	m_pEnergy->SetTrigger(0);
	m_pEnergy->Uninit();
	m_pEnergy = nullptr;

	// TODO : 発射
}

//==========================================================================
// 構え
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Stance(CMotion::AttackInfo ATKInfo)
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
// かめはめ波の中心生成時
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_CreateEnergy(CMotion::AttackInfo ATKInfo)
{
	// プレイヤー取得
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// 攻撃情報の位置へ設定
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);

	// かめはめ波の中心(開始時)生成
	m_pEnergyStart = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ENERGY,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		SCALE_CHARGESTART, false);
}

//==========================================================================
// チャージ開始時
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_ChargeStart(CMotion::AttackInfo ATKInfo)
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

	// TODO : フラッシュ生成

}




//==========================================================================
// チャージ開始
//==========================================================================
void CSpecialEffect_Kamehame::Progress_ChargeStart(CMotion::AttackInfo ATKInfo)
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
void CSpecialEffect_Kamehame::Progress_Chargeing(CMotion::AttackInfo ATKInfo)
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
void CSpecialEffect_Kamehame::Progress_Condense(CMotion::AttackInfo ATKInfo)
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
// 発射
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Brust(CMotion::AttackInfo ATKInfo)
{
	
}
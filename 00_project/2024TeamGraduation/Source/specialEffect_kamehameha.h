//=============================================================================
//
// スペシャル演出(かめはめ波)処理 [specialEffect_kamehameha.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERATRIGGER_PASS_H_		// このマクロ定義がされていなかったら
#define _CAMERATRIGGER_PASS_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "motion.h"
#include "specialEffect.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラトリガー_パスクラス
class CSpecialEffect_Kamehame : public CSpecialEffect
{
public:

	CSpecialEffect_Kamehame();
	~CSpecialEffect_Kamehame();
	
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	// トリガーの瞬間
	virtual void TriggerMoment(CMotionManager::AttackInfo ATKInfo, int idx) override
	{
		(this->*(m_TriggerFunc[idx]))(ATKInfo);
	}

	// 進行中
	virtual void ProgressMoment(CMotionManager::AttackInfo ATKInfo, int idx) override
	{
		(this->*(m_ProgressFunc[idx]))(ATKInfo);
	}

	virtual void FinishSetting() override;	// 終了時の設定

private:

	//=============================
	// 関数リスト
	//=============================
	// トリガー時の処理
	typedef void(CSpecialEffect_Kamehame::*TRIGGER_FUNC)(CMotionManager::AttackInfo);
	static TRIGGER_FUNC m_TriggerFunc[];

	// 進行中の処理
	typedef void(CSpecialEffect_Kamehame::*PROGRESS_FUNC)(CMotionManager::AttackInfo);
	static TRIGGER_FUNC m_ProgressFunc[];

	//=============================
	// メンバ関数
	//=============================
	// トリガー
	void Trigger_Brust(CMotionManager::AttackInfo ATKInfo);		// 発射
	void Trigger_Stance(CMotionManager::AttackInfo ATKInfo);		// 構え
	void Trigger_CreateEnergy(CMotionManager::AttackInfo ATKInfo);	// かめはめ波の中心生成時
	void Trigger_ChargeStart(CMotionManager::AttackInfo ATKInfo);	// チャージ開始時

	// 進行中
	void Progress_Brust(CMotionManager::AttackInfo ATKInfo);		// 発射
	void Progress_ChargeStart(CMotionManager::AttackInfo ATKInfo);	// チャージ開始
	void Progress_Chargeing(CMotionManager::AttackInfo ATKInfo);	// チャージ中
	void Progress_Condense(CMotionManager::AttackInfo ATKInfo);	// 凝縮
	void Progress_TransBrust(CMotionManager::AttackInfo ATKInfo);	// 発射へ遷移

	// その他
	void UpdateWind(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 風更新

	//=============================
	// メンバ変数
	//=============================
	CEffekseerObj* m_pEnergyStart = nullptr;	// かめはめ波の中心(開始時)
	CEffekseerObj* m_pEnergy = nullptr;			// かめはめ波の中心
	CEffekseerObj* m_pAtmosphere = nullptr;		// 漂う空間オーラ
	CEffekseerObj* m_pChargeThunder = nullptr;	// チャージ時の雷
	CEffekseerObj* m_pBallast = nullptr;		// がれき

	// 風用
	bool m_bWindCreate;			// 風生成フラグ
	float m_fIntervalWind;		// 風生成インターバル
	float m_fCreateWindTime;	// 風生成タイマー
};

#endif
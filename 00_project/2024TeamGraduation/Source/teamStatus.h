//=============================================================================
// 
//  チームステータスヘッダー [teamStatus.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _TEAMSTATUS_H_
#define _TEAMSTATUS_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// チームステータスクラス
//==========================================================================
class CTeamStatus
{
public:

	struct SCharmInfo
	{
		float fValue;
		float fValueMax;

		//コンストラクタ
		SCharmInfo() : fValue(0.0f), fValueMax(0.0f) {}
	};

	struct SSpecialInfo
	{
		float fValue;
		float fValueMax;
		CObject2D* pGauge;

		//コンストラクタ
		SSpecialInfo() : fValue(0.0f), fValueMax(0.0f), pGauge(nullptr) {}
	};

	// コンストラクタ
	CTeamStatus();

	// デストラクタ
	~CTeamStatus();

	HRESULT Init();		// 初期化
	void Uninit();		// 終了

	//=============================
	// メンバ関数
	//=============================
	void SetTeam(const CGameManager::ETeamSide team) { m_typeTeam = team; }		// チームサイド設定
	CGameManager::ETeamSide GetTeam() { return m_typeTeam; }					// チームサイド取得
	void TeamSetting(const CGameManager::ETeamSide team);						// チームサイドからなる設定
	bool CheckAllDead();	// 全滅で終了

	//-----------------------------
	// モテ情報
	//-----------------------------
	void InitCharmInfo();													// モテ情報初期化
	void AddCharmValue(float fValue);										// モテゲージ値増加
	void SubCharmValue(float fValue);										// モテゲージ値減少
	void SetCharmInfo(SCharmInfo sInfo) { m_sCharmInfo = sInfo; }			// モテ情報設定
	SCharmInfo GetCharmInfo() { return m_sCharmInfo; }						// モテ情報取得

	//-----------------------------
	// スペシャル情報
	//-----------------------------
	void InitSpecialInfo();													// スペシャル情報初期化
	void SetSpecialValue(float fValue);										// スペシャルゲージ値設定
	float GetSpecialValue() { return m_sSpecialInfo.fValue; }				// スペシャルゲージ値取得
	void AddSpecialValue(float fValue);										// スペシャルゲージ値増加
	void SubSpecialValue(float fValue);										// スペシャルゲージ値減少
	void ZeroSpecialValue() { SetSpecialValue(0); }							// スペシャルゲージ値消去
	void SetSpecialInfo(SSpecialInfo sInfo) { m_sSpecialInfo = sInfo; }		// スペシャル情報設定
	SSpecialInfo GetSpecialInfo() { return m_sSpecialInfo; }				// スペシャル情報取得
	bool IsMaxSpecial() { return (m_sSpecialInfo.fValue == m_sSpecialInfo.fValueMax); }		//マックスフラグ取得

	void Debug();		//デバッグ

	static CTeamStatus* Create();		//生成

private:
	
	//=============================
	// メンバ変数
	//=============================
	CGameManager::ETeamSide m_typeTeam;	// チームサイド
	SCharmInfo m_sCharmInfo;			// モテ情報
	SSpecialInfo m_sSpecialInfo;		// スペシャル情報
};

#endif
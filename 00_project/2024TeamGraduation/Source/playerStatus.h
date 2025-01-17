//=============================================================================
// 
//  プレイヤーステータスヘッダー [playerStatus.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERSTATUS_H_
#define _PLAYERSTATUS_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"
#include "listManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;
class CObjectCircleGauge2D;	// 円のゲージ

//==========================================================================
// プレイヤーステータスクラス
//==========================================================================
class CPlayerStatus
{
public:

	// コンストラクタ
	CPlayerStatus(CPlayer* pPlayer);

	// デストラクタ
	~CPlayerStatus();

	//=============================
	// メンバ関数
	//=============================
	void Init();	// 初期化処理
	void Kill();	// 削除処理
	void BindPlayer(CPlayer* pPlayer)	{ m_pPlayer = pPlayer; }	// プレイヤー割当
	void LifeDamage(const int nDmg);	// 体力減算
	void LifeHeal(const int nHeal);		// 体力加算

private:

	//=============================
	// メンバ関数
	//=============================
	void CreateLifeGuge();	// 体力ゲージ生成

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pPlayer;					// プレイヤーのポインタ
	CObjectCircleGauge2D* m_pLifeGauge;	// 体力ゲージ
	static CListManager<CObjectCircleGauge2D> m_LifeGaugeListLeft;	// 左体力ゲージのリスト
	static CListManager<CObjectCircleGauge2D> m_LifeGaugeListRight;	// 右体力ゲージのリスト

};

#endif
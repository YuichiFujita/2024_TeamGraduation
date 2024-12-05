//==========================================================================
// 
//  プレイヤー位置補正_内野コートヘッダー [playerPosAdj_in.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_POSADJ_IN_H_
#define _PLAYER_POSADJ_IN_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerPosAdj.h"

//==========================================================================
// プレイヤー位置補正_内野コート定義
//==========================================================================
class CPlayerPosAdjIn : public CPlayerPosAdj
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr float COMEBACK_LINE = 100.0f;	// 相手コートから戻ってくるライン
	static constexpr float UNSTABLE_LINE = 1.0f;	// おっとっとライン

	//=============================
	// 列挙型
	//=============================
	enum EInputUnstable
	{
		INPUT_NONE = 0,		// 入力なし
		INPUT_FRIEND,		// 味方側
		INPUT_ENEMY,		// 敵側
		INPUT_MAX,
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerPosAdjIn();
	virtual ~CPlayerPosAdjIn() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void UpdateAdjuster(CPlayer* pPlayer) override;	// 調整

protected:

	//=============================
	// 定数
	//=============================
	static constexpr float RETURN_DISTANCE = 60.0f;	// 戻るための間隔

	//=============================
	// 純粋仮想関数
	//=============================
	virtual bool IsLineOut(CPlayer* pPlayer) = 0;		// ライン越えフラグ取得
	virtual bool IsUnstable(CPlayer* pPlayer) = 0;		// おっとっとフラグ取得
	virtual EInputUnstable IsInputLine(CPlayer* pPlayer) = 0;	// おっとっと時入力フラグ取得

private:
	//=============================
	// メンバ関数
	//=============================
	void CheckReturn(CPlayer* pPlayer);		// チームコート復帰
	void CheckUnstable(CPlayer* pPlayer);	// おっとっとする
	void ReturnSetting(CPlayer* pPlayer);	// 復帰設定
};

#endif
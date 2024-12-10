//==========================================================================
// 
//  外野プレイヤーアクションコントロールヘッダー [playerUserOut_controlAction.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYERUSER_OUT_CONTROL_ACTION_H_
#define _PLAYERUSER_OUT_CONTROL_ACTION_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerUsercontrol_action.h"

//==========================================================================
// 外野外野プレイヤーアクションコントロールクラス定義
//==========================================================================
//=============================
// アクション
//=============================
class CPlayerUserOutControlAction : public CPlayerUserControlAction
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerUserOutControlAction();
	virtual ~CPlayerUserOutControlAction() override;

	//=============================
	// 仮想関数
	//=============================
	virtual CPlayerUserOutControlAction* GetUserOut() override { return this; }

private:
	
	//=============================
	// メンバ関数
	//=============================
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override {}	// スペシャル
};

#endif

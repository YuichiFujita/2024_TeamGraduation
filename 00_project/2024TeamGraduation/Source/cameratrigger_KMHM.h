//=============================================================================
//
// カメラトリガー_かめはめ波処理 [cameratrigger_KMHM.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERATRIGGER_KMHM_H_		// このマクロ定義がされていなかったら
#define _CAMERATRIGGER_KMHM_H_		// 二重インクルード防止のマクロを定義する

#include "camera_motion_trigger.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラトリガー_かめはめ波クラス
class CCameraTrigger_KMHM : public CCameraMotion_Trigger
{
public:

	CCameraTrigger_KMHM();
	~CCameraTrigger_KMHM();
	
	// トリガーの瞬間
	virtual void TriggerMoment(int idx) override
	{
		(this->*(m_TriggerFunc[idx]))();
	}

private:


	//=============================
	// 関数リスト
	//=============================
	// モーションリスト
	typedef void(CCameraTrigger_KMHM::*TRIGGER_FUNC)();
	static TRIGGER_FUNC m_TriggerFunc[];

	//=============================
	// メンバ関数
	//=============================
	void Stance();	// 構え

};

#endif
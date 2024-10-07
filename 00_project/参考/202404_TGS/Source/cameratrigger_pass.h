//=============================================================================
//
// カメラトリガー_パス処理 [cameratrigger_pass.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERATRIGGER_PASS_H_		// このマクロ定義がされていなかったら
#define _CAMERATRIGGER_PASS_H_		// 二重インクルード防止のマクロを定義する

#include "camera_motion_trigger.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラトリガー_パスクラス
class CCameraTrigger_Pass : public CCameraMotion_Trigger
{
public:

	CCameraTrigger_Pass();
	~CCameraTrigger_Pass();
	
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
	typedef void(CCameraTrigger_Pass::*TRIGGER_FUNC)();
	static TRIGGER_FUNC m_TriggerFunc[];

	void waiting();	// 字幕
	void newMission();
	void carry();
	void carefull();
	void goodluck();

};

#endif
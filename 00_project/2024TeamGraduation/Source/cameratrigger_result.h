//=============================================================================
//
// カメラトリガー_パス処理 [cameratrigger_result.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERATRIGGER_RESULT_H_		// このマクロ定義がされていなかったら
#define _CAMERATRIGGER_RESULT_H_		// 二重インクルード防止のマクロを定義する

#include "camera_motion_trigger.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラトリガー_パスクラス
class CCameraTrigger_Result : public CCameraMotion_Trigger
{
public:

	CCameraTrigger_Result();
	~CCameraTrigger_Result();
	
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
	typedef void(CCameraTrigger_Result::*TRIGGER_FUNC)();
	static TRIGGER_FUNC m_TriggerFunc[];

	//=============================
	// メンバ関数
	//=============================
	void Subtitle01();	// 字幕01
	void Subtitle02();	// 字幕02
	void CameraShake();	// カメラ揺れ
	void OpenScroll();	// 巻き物開封

};

#endif
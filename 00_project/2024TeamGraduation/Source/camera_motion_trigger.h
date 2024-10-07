//=============================================================================
//
// カメラモーショントリガー処理 [camera_motion_trigger.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERA_MOTION_TRIGGER_H_		// このマクロ定義がされていなかったら
#define _CAMERA_MOTION_TRIGGER_H_		// 二重インクルード防止のマクロを定義する


//==========================================================================
// クラス定義
//==========================================================================
// カメラモーショントリガークラス
class CCameraMotion_Trigger
{
public:

	CCameraMotion_Trigger();
	~CCameraMotion_Trigger();
	
	// トリガーの瞬間
	virtual void TriggerMoment(int idx) = 0;

	static CCameraMotion_Trigger* Create(int motion)
	{
		return m_CreateFunc[motion]();
	}

private:

	//=============================
	// 関数リスト
	//=============================
	// 生成リスト
	using CREATE_FUNC = std::function<CCameraMotion_Trigger* ()>;
	static std::vector < CREATE_FUNC > m_CreateFunc;
};

#endif
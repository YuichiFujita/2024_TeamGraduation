//=============================================================================
// 
//  タイマーヘッダー [timer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_	// 二重インクルード防止

#include "main.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CMultiNumber;
class CCamera;
class CObject2D;

//==========================================================================
// クラス定義
//==========================================================================
// タイマークラス
class CTimer
{
public:

	enum Type
	{
		TYPE_NORMAL = 0,
		TYPE_RESULT,
		TYPE_MAX
	};

	enum eState
	{
		STATE_WAIT = 0,		// 待機
		STATE_GOAL,			// ゴール
		STATE_MAX
	};

	CTimer(int nPriority = 6);
	~CTimer();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	float GetTime() { return m_fTime; }	// 時間取得
	void SetTime(const float time);
	void SetEnableAddTime(bool frag) { m_bAddTime = frag; }	// 時間加算フラグ設定
	virtual void ApplyTimer();	// タイマー反映
	CCamera* GetCamera() { return m_pMyCamera; }

	CTimer::eState GetState() { return m_state; }
	void SetState(eState state) { m_state = state; }

	static CTimer* Create(Type type, int nPriority = 6);
	static CTimer* GetInstance() { return m_pTimer; }

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CTimer::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 状態関数リスト

	//=============================
	// メンバ関数
	//=============================
	virtual void StateWait();
	virtual void StateGoal();
	void SetCamera();

protected:

	//=============================
	// メンバ変数
	//=============================
	int m_nPriority;
	MyLib::Vector3 m_pos;			// 位置
	MyLib::Vector3 m_posOrigin;		// 元の位置
	eState m_state;					// 状態
	float m_fStateTime;				// 状態時間
	float m_fTime;					// 時間
	bool m_bAddTime;				// タイマー加算のフラグ
	CMultiNumber* m_pClearTime[3];	// 種類ごとの数字
	static CTimer *m_pTimer;		// 自身のポインタ
	CCamera* m_pMyCamera;			// カメラ
	CObject2D* m_pBg;				// 背景
};



#endif
//=============================================================================
//
// 入力(ゲームパッド)処理 [input_gamepad.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _INPUT_GAMEPAD_H_	//このマクロ定義がされていなかったら
#define _INPUT_GAMEPAD_H_	//二重インクルード防止のマクロを定義する

#include "input.h"

//==========================================================================
// クラス定義
//==========================================================================
// ゲームパッドクラス
class CInputGamepad : public CInput
{
public:

	// 列挙型定義
	enum BUTTON
	{
		BUTTON_UP = 0,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_START,
		BUTTON_BACK,
		BUTTON_LSTICKPUSH,
		BUTTON_RSTICKPUSH,
		BUTTON_LB,
		BUTTON_RB,
		BUTTON_LT,
		BUTTON_RT,
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_LX,
		BUTTON_LY,
		BUTTON_18,
		BUTTON_19,
		BUTTON_MAX
	};

	// 列挙型定義
	enum STICK
	{
		STICK_X = 0,
		STICK_Y,
		STICK_MAX
	};

	enum VIBRATION_STATE
	{
		VIBRATION_STATE_NONE = 0,	//何もしてない状態
		VIBRATION_STATE_AIR,		// 空気噴出状態
		VIBRATION_STATE_DMG,		//ダメージ状態

	};

	CInputGamepad();
	~CInputGamepad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

	// ボタン系
	bool GetPress(BUTTON nKey, int nCntPlayer);
	bool GetTrigger(BUTTON nKey, int nCntPlayer);
	bool GetRepeat(BUTTON nKey, int nCntPlayer);
	bool GetRelease(int nKey, int nCntPlayer);

	// トリガー系
	bool GetPressLT(int nCntPlayer);	// LTのプレス判定
	bool GetPressRT(int nCntPlayer);	// RTのプレス判定
	bool GetTriggerLT(int nCntPlayer);	// LTのトリガー判定
	bool GetTriggerRT(int nCntPlayer);	// RTのトリガー判定

	// スティック系
	bool GetLStickTrigger(STICK XY);	// スティックのトリガー判定
	bool GetRStickTrigger(STICK XY);	// スティックのトリガー判定
	MyLib::Vector3 GetStickMoveL(int nCntPlayer);
	MyLib::Vector3 GetStickMoveR(int nCntPlayer);
	MyLib::Vector3 GetStickPositionRatioL(int nCntPlayer);	// 左スティックの割合取得
	MyLib::Vector3 GetStickPositionRatioR(int nCntPlayer);	// 右スティックの割合取得
	float GetStickRotL(int nCntPlayer);					// スティックの向き取得
	float GetStickRotR(int nCntPlayer);					// スティックの向き取得
	bool IsTipStick() { return m_bLStickTip; }		// スティックが倒れてるかの判定
	float GetVibMulti() { return m_fVibrationMulti; }
	void SetVibMulti(float fMulti) { m_fVibrationMulti = fMulti; }

	// バイブレーション系
	void SetEnableVibration(bool bUse) { m_bVibrationUse = bUse; }	// バイブの使用状況切り替え
	bool IsEnableVibration() { return m_bVibrationUse; }			// バイブの使用状況取得
	void SetVibration(VIBRATION_STATE VibState, int nCntPlayer);
	int GetnCntPad();

	static CInputGamepad* GetInstance() { return m_pThisPtr; }		// インスタンス取得
	static CInputGamepad* Create(HINSTANCE hInstance, HWND hWnd);	// 生成処理

private:

	// トリガー
	struct sTrigger
	{
		bool bPress;	// プレス判定
		bool bTrigger;	// トリガー判定

		sTrigger() :bPress(false), bTrigger(false) {}
	};

	//=============================
	// メンバ変数
	//=============================
	void UpdateStickTrigger();		// スティックのトリガー判定
	void UpdateTriggerState(int nCntPlayer, XINPUT_STATE inputstate);	// トリガーの判定処理
	void UpdateVibration(int nCntPlayer);	// 振動の更新処理

	//=============================
	// メンバ変数
	//=============================
	XINPUT_STATE m_aGamepadState[mylib_const::MAX_PLAYER];				// プレス情報
	XINPUT_STATE m_aGamepadStateTrigger[mylib_const::MAX_PLAYER];		// トリガー情報
	XINPUT_STATE m_aGamepadStateRepeat[mylib_const::MAX_PLAYER];		// リピート情報
	XINPUT_STATE m_aGamepadStateRelease[mylib_const::MAX_PLAYER];		// リリース情報
	XINPUT_VIBRATION m_aGamepadStateVib[mylib_const::MAX_PLAYER];		// バイブレーション
	XINPUT_VIBRATION m_aUpdateVib[mylib_const::MAX_PLAYER];				// バイブレーション更新用
	VIBRATION_STATE m_VibrationState[mylib_const::MAX_PLAYER];			// 振動の種類
	int m_nCntVibration[mylib_const::MAX_PLAYER];						// 振動の時間
	int m_nMaxCntVibration[mylib_const::MAX_PLAYER];					// 振動の時間
	int m_nCntPadrepeat;									// リピート用カウント

	// トリガー
	sTrigger m_StateLT[mylib_const::MAX_PLAYER];	// LTの判定
	sTrigger m_StateRT[mylib_const::MAX_PLAYER];	// RTの判定

	// スティック
	bool m_bLeftStickSelect[STICK_MAX];						// 左スティックの選択判定
	bool m_bLeftStickTrigger[STICK_MAX];					// 左スティックのトリガー判定
	bool m_bRightStickSelect[STICK_MAX];					// 右スティックの選択判定
	bool m_bRightStickTrigger[STICK_MAX];					// 右スティックのトリガー判定
	bool m_bLStickTip;										// 左スティックの傾き判定
	bool m_bVibrationUse;									// バイブを使用するかどうか
	float m_fVibrationMulti;								// バイブレーション倍率

	static CInputGamepad* m_pThisPtr;	// 自身のポインタ
};

#endif
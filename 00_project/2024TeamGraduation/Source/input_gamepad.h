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

	//=============================
	// 列挙型定義
	//=============================
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

	// スティック軸
	enum STICK_AXIS
	{
		STICK_X = 0,
		STICK_Y,
		STICK_MAX
	};

	// バイブレーション
	enum VIBRATION_STATE
	{
		VIBRATION_STATE_NONE = 0,	// 何もしてない状態
		VIBRATION_STATE_DMG,		// ダメージ時
		VIBRATION_STATE_MAX
	};


	//=============================
	// 構造体定義
	//=============================
	struct STapInfo	// タップ情報
	{
		bool bInput;	// 入力フラグ
		bool bComplete;	// 完了の瞬間フラグ
		float fRatio;	// 割合

		// コンストラクタ
		STapInfo() : bInput(false), bComplete(false), fRatio(0.0f) {}
	};

	CInputGamepad();
	~CInputGamepad();

	//--------------------------
	// パイプライン
	//--------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	//--------------------------
	// ボタン系
	//--------------------------
	bool GetPress(BUTTON nKey, int nCntPlayer);
	bool GetTrigger(BUTTON nKey, int nCntPlayer);
	bool GetRepeat(BUTTON nKey, int nCntPlayer);
	bool GetRelease(BUTTON nKey, int nCntPlayer);
	STapInfo GetTap(BUTTON nKey, int nCntPlayer, float tapTime);

	//--------------------------
	// トリガー系
	//--------------------------
	bool GetPressLT(int nCntPlayer);	// LTのプレス判定
	bool GetPressRT(int nCntPlayer);	// RTのプレス判定
	bool GetTriggerLT(int nCntPlayer);	// LTのトリガー判定
	bool GetTriggerRT(int nCntPlayer);	// RTのトリガー判定

	//--------------------------
	// スティック系
	//--------------------------
	bool GetLStickTrigger(int nCntPlayer, STICK_AXIS XY);	// スティックのトリガー判定
	bool GetRStickTrigger(int nCntPlayer, STICK_AXIS XY);	// スティックのトリガー判定
	MyLib::Vector3 GetStickMoveL(int nCntPlayer);
	MyLib::Vector3 GetStickMoveR(int nCntPlayer);
	MyLib::Vector3 GetStickPositionRatioL(int nCntPlayer);	// 左スティックの割合取得
	MyLib::Vector3 GetStickPositionRatioR(int nCntPlayer);	// 右スティックの割合取得
	float GetStickRotL(int nCntPlayer);					// スティックの向き取得
	float GetStickRotR(int nCntPlayer);					// スティックの向き取得
	bool IsTipStickL(int nCntPlayer, STICK_AXIS XY) { return m_bLStickTip[nCntPlayer][XY]; }		// Lスティックが倒れてるかの判定
	bool IsTipStickR(int nCntPlayer, STICK_AXIS XY) { return m_bRStickTip[nCntPlayer][XY]; }		// Rスティックが倒れてるかの判定
	float GetVibMulti() { return m_fVibrationMulti; }
	void SetVibMulti(float fMulti) { m_fVibrationMulti = fMulti; }

	//--------------------------
	// バイブレーション系
	//--------------------------
	void SetEnableVibration(bool bUse) { m_bVibrationUse = bUse; }	// バイブの使用状況切り替え
	bool IsEnableVibration() { return m_bVibrationUse; }			// バイブの使用状況取得
	void SetVibration(VIBRATION_STATE VibState, int nCntPlayer);
	int GetnCntPad();

	static CInputGamepad* GetInstance() { return m_pThisPtr; }		// インスタンス取得
	static CInputGamepad* Create(HINSTANCE hInstance, HWND hWnd);	// 生成処理

private:

	//=============================
	// 構造体定義
	//=============================
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
	void UpdateStickTrigger(int nCntPlayer);		// スティックのトリガー判定
	void UpdateTriggerState(int nCntPlayer, XINPUT_STATE inputstate);	// トリガーの判定処理
	void UpdateVibration(int nCntPlayer);	// 振動の更新処理
	void UpdateTapTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer);	// タップ判定タイマーの更新

	//=============================
	// メンバ変数
	//=============================
	//--------------------------
	// 機能
	//--------------------------
	XINPUT_STATE m_aGamepadState[mylib_const::MAX_PLAYER];				// プレス情報
	XINPUT_STATE m_aGamepadStateTrigger[mylib_const::MAX_PLAYER];		// トリガー情報
	XINPUT_STATE m_aGamepadStateRepeat[mylib_const::MAX_PLAYER];		// リピート情報
	XINPUT_STATE m_aGamepadStateRelease[mylib_const::MAX_PLAYER];		// リリース情報
	XINPUT_VIBRATION m_aGamepadStateVib[mylib_const::MAX_PLAYER];		// バイブレーション
	XINPUT_VIBRATION m_aUpdateVib[mylib_const::MAX_PLAYER];				// バイブレーション更新用
	VIBRATION_STATE m_VibrationState[mylib_const::MAX_PLAYER];			// 振動の種類
	int m_nCntVibration[mylib_const::MAX_PLAYER];						// 振動の時間
	int m_nMaxCntVibration[mylib_const::MAX_PLAYER];					// 振動の時間
	int m_nCntPadrepeat;												// リピート用カウント
	float m_fTapTimer[BUTTON::BUTTON_MAX][mylib_const::MAX_PLAYER];		// タップ判定用のタイマー
	float m_fOldTapTimer[BUTTON::BUTTON_MAX][mylib_const::MAX_PLAYER];	// 前回のタップ判定用のタイマー

	//--------------------------
	// トリガー
	//--------------------------
	sTrigger m_StateLT[mylib_const::MAX_PLAYER];	// LTの判定
	sTrigger m_StateRT[mylib_const::MAX_PLAYER];	// RTの判定

	//--------------------------
	// スティック
	//--------------------------
	bool m_bLStickSelect[mylib_const::MAX_PLAYER][STICK_MAX];		// 左スティックの選択判定
	bool m_bLStickTrigger[mylib_const::MAX_PLAYER][STICK_MAX];		// 左スティックのトリガー判定
	bool m_bRStickTrigger[mylib_const::MAX_PLAYER][STICK_MAX];		// 右スティックの選択判定
	bool m_bRStickSelect[mylib_const::MAX_PLAYER][STICK_MAX];		// 右スティックのトリガー判定
	bool m_bLStickTip[mylib_const::MAX_PLAYER][STICK_MAX];			// 左スティックの傾き判定
	bool m_bRStickTip[mylib_const::MAX_PLAYER][STICK_MAX];			// 右スティックの傾き判定
	bool m_bVibrationUse;					// バイブを使用するかどうか
	float m_fVibrationMulti;				// バイブレーション倍率
	float m_fDeadZone;						// デッドゾーン

	static CInputGamepad* m_pThisPtr;	// 自身のポインタ
};

#endif
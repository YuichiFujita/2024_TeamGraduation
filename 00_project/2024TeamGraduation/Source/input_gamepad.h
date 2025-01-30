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

	// スティック十字軸
	enum STICK_CROSS_AXIS
	{
		STICK_CROSS_LEFT = 0,
		STICK_CROSS_RIGHT,
		STICK_CROSS_UP,
		STICK_CROSS_DOWN,
		STICK_CROSS_MAX
	};

	// バイブレーション
	enum EVibType
	{
		VIBTYPE_NONE = 0,		// 何もしてない状態
		VIBTYPE_CATCH_NORMAL,	// キャッチ(通常)
		VIBTYPE_CATCH_FAST,		// キャッチ(早めの)
		VIBTYPE_THROW_NORMAL,	// 投げ(通常)
		VIBTYPE_THROW_FAST,		// 投げ(早めの)
		VIBTYPE_HIT,			// 被弾
		VIBTYPE_HIT_SP,			// 被弾(スペシャル)
		VIBTYPE_DEAD,			// 死亡
		VIBTYPE_MAX
	};

	// 補正種類
	enum EEasing
	{
		Linear = 0,
		EaseIn,
		EaseOut,
		EaseInOut,
		EaseInExpo,
		EaseOutExpo,
		EaseInOutExpo,
		MAX
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

	struct SVib	// バイブ
	{
		float timer;		// 時間
		float maxTimer;		// 時間の最大値
		float startSpeed;	// 初期速度
		float endSpeed;		// 目標速度
		EEasing easeType;	// 補正種類

		// コンストラクタ
		SVib() :timer(0.0f), maxTimer(0.0f), startSpeed(0.0f), endSpeed(0.0f), easeType(EEasing::Linear) {}
		
		// パラメータ付きコンストラクタ
		SVib
		(
			float _timer,		// 時間
			float _maxTimer,	// 時間の最大値
			float _startSpeed,	// 初期速度
			float _endSpeed,	// 目標速度
			EEasing _easeType	// 補正種類
		) : timer(_timer), maxTimer(_maxTimer), startSpeed(_startSpeed), endSpeed(_endSpeed), easeType(_easeType) {}
	};

	struct SAllTrigger	// トリガー
	{
		bool bInput;	// 入力判定
		int nID;		// 入力者のID

		SAllTrigger() : bInput(false), nID(-1) {}
		SAllTrigger(bool _bInput, int _nID) : bInput(_bInput), nID(_nID) {}
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
	void InitPress(int nCntPlayer);		// プレス初期化
	void InitTrigger(int nCntPlayer);	// トリガー初期化
	void InitRepeat(int nCntPlayer);	// リピート初期化
	void InitRelease(int nCntPlayer);	// リリース初期化
	bool GetPress(BUTTON nKey, int nCntPlayer);
	bool GetTrigger(BUTTON nKey, int nCntPlayer);
	bool GetRepeat(BUTTON nKey, int nCntPlayer);
	bool GetRelease(BUTTON nKey, int nCntPlayer);
	STapInfo GetTap(BUTTON nKey, int nCntPlayer, float tapTime);
	bool GetAllPress(BUTTON nKey);
	SAllTrigger GetAllTrigger(BUTTON nKey);
	bool GetAllRepeat(BUTTON nKey);
	bool GetAllRelease(BUTTON nKey);
	int GetnCntPad();

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
	bool GetLStickTrigger(int nCntPlayer, STICK_CROSS_AXIS closs);	// Lスティックのトリガー判定
	bool GetRStickTrigger(int nCntPlayer, STICK_CROSS_AXIS closs);	// Rスティックのトリガー判定
	bool GetLStickTrigger(int nCntPlayer, STICK_AXIS XY);	// Lスティックのトリガー判定
	bool GetRStickTrigger(int nCntPlayer, STICK_AXIS XY);	// Rスティックのトリガー判定
	MyLib::Vector3 GetStickMoveL(int nCntPlayer);			// Lスティックの移動量取得
	MyLib::Vector3 GetStickMoveR(int nCntPlayer);			// Rスティックの移動量取得
	MyLib::Vector3 GetStickPositionRatioL(int nCntPlayer);	// Lスティックの割合取得
	MyLib::Vector3 GetStickPositionRatioR(int nCntPlayer);	// Rスティックの割合取得
	float GetStickRotL(int nCntPlayer);						// Lスティックの向き取得
	float GetStickRotR(int nCntPlayer);						// Rスティックの向き取得
	bool IsTipStickL(int nCntPlayer, STICK_AXIS XY) { return m_bLStickTip[nCntPlayer][XY]; }	// Lスティックが倒れてるかの判定
	bool IsTipStickR(int nCntPlayer, STICK_AXIS XY) { return m_bRStickTip[nCntPlayer][XY]; }	// Rスティックが倒れてるかの判定

	//--------------------------
	// バイブレーション系
	//--------------------------
	void SetEnableVibration(bool bUse) { m_bVibrationUse = bUse; }	// バイブの使用状況切り替え
	bool IsEnableVibration() { return m_bVibrationUse; }			// バイブの使用状況取得
	void SetVibration(EVibType VibState, int nCntPlayer);			// バイブ設定
	void SetVibMulti(float fMulti)	{ m_fVibrationMulti = fMulti; }	// バイブの倍率設定
	float GetVibMulti()				{ return m_fVibrationMulti; }	// バイブの倍率取得

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
	void UpdateVibration(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer);	// 振動の更新処理
	void SetVibrationParam(int nCntPlayer);																			// 振動のパラメータ設定
	void UpdateTapTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer);	// タップ判定タイマーの更新
	void Debug();	// デバッグ

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
	int m_nCntPadrepeat;												// リピート用カウント
	float m_fTapTimer[BUTTON::BUTTON_MAX][mylib_const::MAX_PLAYER];		// タップ判定用のタイマー
	float m_fOldTapTimer[BUTTON::BUTTON_MAX][mylib_const::MAX_PLAYER];	// 前回のタップ判定用のタイマー

	//--------------------------
	// 振動
	//--------------------------
	// float線形補正リスト
	typedef float(*FLOAT_EASING_FUNC)(float, float, float, float, float);
	static FLOAT_EASING_FUNC m_FloatEasingFunc[];

	XINPUT_VIBRATION m_aGamepadStateVib[mylib_const::MAX_PLAYER];	// バイブレーション
	EVibType m_aVibType[mylib_const::MAX_PLAYER];					// バイブの種類
	SVib m_aVibInfo[mylib_const::MAX_PLAYER];						// 振動情報

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

	//--------------------------
	// デバッグ
	//--------------------------
	EVibType m_EditVibType;	// 編集するバイブの種類

	static CInputGamepad* m_pThisPtr;	// 自身のポインタ
};

#endif
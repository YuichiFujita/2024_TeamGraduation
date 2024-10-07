//=============================================================================
//
// 入力(ゲームパッド)処理 [input_gamepad.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input_gamepad.h"
#include "debugproc.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int SHOT_FPS = 15;	// 弾の間隔
	const int DMG_TIME = 30;	// バイブの時間
	const int WIN_TIME = 15;	// バイブの時間
	const float DEFAULT_TIME = (1.0f / 2.0f);
}
CInputGamepad* CInputGamepad::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CInputGamepad::CInputGamepad()
{
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;	// 振動の種類
		m_nCntVibration[nCntPlayer] = 0;						// 振動の時間
		m_nMaxCntVibration[nCntPlayer] = 0;						// 振動の時間
	}
	m_bLeftStickSelect[STICK_X] = false;	// 左トリガーの選択判定
	m_bLeftStickSelect[STICK_Y] = false;	// 左トリガーの選択判定
	m_bVibrationUse = false;				// バイブを使用するかどうか
	m_bLStickTip = false;					// 左スティックの傾き判定

	m_nCntPadrepeat = 0;									// リピート用カウント
	m_fVibrationMulti = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CInputGamepad::~CInputGamepad()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CInputGamepad* CInputGamepad::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	// インスタンス生成
	m_pThisPtr = DEBUG_NEW CInputGamepad;
	if (m_pThisPtr != nullptr) {

		// 初期化処理
		m_pThisPtr->Init(hInstance, hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
//ゲームパッドの初期化処理
//==========================================================================
HRESULT CInputGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 初期化処理
	CInput::Init(hInstance, hWnd);

	//　XInputを有効化
	XInputEnable(true);

	// 左スティックの情報を初期化
	m_bLeftStickSelect[STICK_X] = false;	// 左トリガーの選択判定
	m_bLeftStickSelect[STICK_Y] = false;	// 左トリガーの選択判定
	m_bLStickTip = false;					// 左スティックの傾き判定

	memset(m_StateLT, 0, sizeof(m_StateLT));	// LTの判定
	memset(m_StateRT, 0, sizeof(m_StateRT));	// RTの判定

	// バイブの情報を初期化
	m_bVibrationUse = true;

	// メモリクリア
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		m_nCntVibration[nCntPlayer] = 0;
		m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;
		m_nMaxCntVibration[nCntPlayer] = 0;

		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		memset(&m_aUpdateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));

	}

	return S_OK;
}

//==========================================================================
//ゲームパッドの終了処理
//==========================================================================
void CInputGamepad::Uninit()
{
	// 入力デバイスからデータを取得
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		// コントローラーにバイブの情報をXINPUTに送る
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = 0.0f;
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = 0.0f;
		XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
	}

	// XInputを無効化
	XInputEnable(false);

	// 終了処理
	CInput::Uninit();

	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// ゲームパッドの更新処理
//==========================================================================
void CInputGamepad::Update()
{
	XINPUT_STATE  aGamepadState[mylib_const::MAX_PLAYER];
	int nCntPlayer;

	// 入力デバイスからデータを取得
	for (nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
	{
		if (XInputGetState(nCntPlayer, &aGamepadState[nCntPlayer]) == ERROR_SUCCESS)
		{
			// トリガーの判定処理
			UpdateTriggerState(nCntPlayer, aGamepadState[nCntPlayer]);

			// トリガー情報を保存
			m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons =
				(~m_aGamepadState[nCntPlayer].Gamepad.wButtons) & aGamepadState[nCntPlayer].Gamepad.wButtons;

			// リリース情報を保存
			m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons =
				(m_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & m_aGamepadState[nCntPlayer].Gamepad.wButtons;


			// プレス情報を保存
			m_aGamepadState[nCntPlayer] = aGamepadState[nCntPlayer];

			if ((m_nCntPadrepeat % SHOT_FPS) == 0)
			{
				// リピート情報を保存
				m_nCntPadrepeat = (m_nCntPadrepeat + 1) % SHOT_FPS;
				m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons = aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
			else
			{
				// リピートにリリースの情報保存
				m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons =
					(m_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & m_aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
		}

		// タイマーを減算
		m_nCntVibration[nCntPlayer]--;

		if (m_nCntVibration[nCntPlayer] < 0)
		{// カウントがゼロ以下

			// 何もしてない状態に設定
			m_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;

			// メモリ初期化
			memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		}

		// 振動更新
		UpdateVibration(nCntPlayer);
	}

	// スティックのトリガー判定更新
	UpdateStickTrigger();
}

//==========================================================================
// スティックのトリガー
//==========================================================================
void CInputGamepad::UpdateStickTrigger()
{
	bool bTipX = false, bTipY = false;
	bTipX = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).x, 0.0f, 0.01f);
	bTipY = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).y, 0.0f, 0.01f);

	// 左トリガーのトリガー判定OFF
	m_bLeftStickTrigger[STICK_X] = false;
	m_bLeftStickTrigger[STICK_Y] = false;

	if (!m_bLeftStickSelect[STICK_X] && bTipX)
	{
		m_bLeftStickSelect[STICK_X] = true;
		m_bLeftStickTrigger[STICK_X] = true;	// 左トリガーのトリガー判定
	}
	if (!m_bLeftStickSelect[STICK_Y] && bTipY)
	{
		m_bLeftStickSelect[STICK_Y] = true;
		m_bLeftStickTrigger[STICK_Y] = true;	// 左トリガーのトリガー判定
	}

	if (m_bLeftStickSelect[STICK_X] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).x, 0.0f, 0.01f))
	{// スティックが戻っているとき
		m_bLeftStickSelect[STICK_X] = false;
	}

	if (m_bLeftStickSelect[STICK_Y] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).y, 0.0f, 0.01f))
	{// スティックが戻っているとき
		m_bLeftStickSelect[STICK_Y] = false;
	}



	bTipX = false, bTipY = false;
	bTipX = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).x, 0.0f, 0.01f);
	bTipY = !UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).y, 0.0f, 0.01f);

	// 左トリガーのトリガー判定OFF
	m_bRightStickTrigger[STICK_X] = false;
	m_bRightStickTrigger[STICK_Y] = false;

	if (!m_bRightStickSelect[STICK_X] && bTipX)
	{
		m_bRightStickSelect[STICK_X] = true;
		m_bRightStickTrigger[STICK_X] = true;	// 左トリガーのトリガー判定
	}
	if (!m_bRightStickSelect[STICK_Y] && bTipY)
	{
		m_bRightStickSelect[STICK_Y] = true;
		m_bRightStickTrigger[STICK_Y] = true;	// 左トリガーのトリガー判定
	}

	if (m_bRightStickSelect[STICK_X] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).x, 0.0f, 0.01f))
	{// スティックが戻っているとき
		m_bRightStickSelect[STICK_X] = false;
	}

	if (m_bRightStickSelect[STICK_Y] &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveR(0).y, 0.0f, 0.01f))
	{// スティックが戻っているとき
		m_bRightStickSelect[STICK_Y] = false;
	}


	if (UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).x, 0.0f, 0.01f) &&
		UtilFunc::Calculation::IsNearlyTarget(GetStickMoveL(0).y, 0.0f, 0.01f))
	{
		m_bLStickTip = false;
	}
	else
	{
		m_bLStickTip = true;
	}

}

//==========================================================================
// トリガーの判定処理
//==========================================================================
void CInputGamepad::UpdateTriggerState(int nCntPlayer, XINPUT_STATE inputstate)
{
	// 判定リセット
	m_StateLT[nCntPlayer] = sTrigger();	// LTの判定
	m_StateRT[nCntPlayer] = sTrigger();	// RTの判定

	// トリガー情報を保存
	// 左トリガー
	if (inputstate.Gamepad.bLeftTrigger > 0)
	{
		// プレス判定
		m_StateLT[nCntPlayer].bPress = true;

		// トリガー判定
		if (m_aGamepadState[nCntPlayer].Gamepad.bLeftTrigger == 0)
		{// 前回操作なし
			m_StateLT[nCntPlayer].bTrigger = true;
		}
	}

	// 右トリガー
	if (inputstate.Gamepad.bRightTrigger > 0)
	{
		// プレス判定
		m_StateRT[nCntPlayer].bPress = true;

		// トリガー判定
		if (m_aGamepadState[nCntPlayer].Gamepad.bRightTrigger == 0)
		{// 前回操作なし
			m_StateRT[nCntPlayer].bTrigger = true;
		}
	}
}

//==========================================================================
// バイブの更新処理
//==========================================================================
void CInputGamepad::UpdateVibration(int nCntPlayer)
{
	switch (m_VibrationState[nCntPlayer])
	{
	case VIBRATION_STATE_NONE:
	{
		
	}
		break;

	case VIBRATION_STATE_AIR:
	{

	}
		break;

	case VIBRATION_STATE_DMG:
	{
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = m_aUpdateVib[nCntPlayer].wLeftMotorSpeed;
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = m_aUpdateVib[nCntPlayer].wRightMotorSpeed;
		m_aUpdateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
		m_aUpdateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	}
		break;

	default:
	{

	}
		break;
	}

	if (m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed <= 0)
	{// スピードが0以下

		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = 0;
	}
	else if (m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed <= 0)
	{// スピードが0以下

		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = 0;
	}

	// コントローラーにバイブの情報をXINPUTに送る
	m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
	m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
}

//==========================================================================
// バイブの設定処理
//==========================================================================
void CInputGamepad::SetVibration(VIBRATION_STATE VibState, int nCntPlayer)
{
	if (!m_bVibrationUse) return;

	// 状態を代入
	m_VibrationState[nCntPlayer] = VibState;

	switch (VibState)
	{
	case VIBRATION_STATE_DMG:

		m_nCntVibration[nCntPlayer] = 15;
		m_nMaxCntVibration[nCntPlayer] = m_nCntVibration[nCntPlayer];
		m_aUpdateVib[nCntPlayer].wLeftMotorSpeed = (USHRT_MAX * 0.8f);
		m_aUpdateVib[nCntPlayer].wRightMotorSpeed = (USHRT_MAX * 0.8f);
		break;

	case VIBRATION_STATE_AIR:

		m_nCntVibration[nCntPlayer] = 10;
		m_nMaxCntVibration[nCntPlayer] = m_nCntVibration[nCntPlayer];
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = (USHRT_MAX * 0.3f);
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = (USHRT_MAX * 0.3f);
		break;
	}

	// コントローラーにバイブの情報をXINPUTに送る
	XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
}

//==========================================================================
// ゲームパッドのプレス処理
//==========================================================================
bool CInputGamepad::GetPress(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadState[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// ゲームパッドのトリガー処理
//==========================================================================
bool CInputGamepad::GetTrigger(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// ゲームパッドのリリース処理
//==========================================================================
bool CInputGamepad::GetRelease(int nKey, int nCntPlayer)
{
	return (m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// ゲームパッドのリピート処理
//==========================================================================
bool CInputGamepad::GetRepeat(BUTTON nKey, int nCntPlayer)
{
	return (m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// LTのプレス判定
//==========================================================================
bool CInputGamepad::GetPressLT(int nCntPlayer)
{
	return m_StateLT[nCntPlayer].bPress;
}

//==========================================================================
// RTのプレス判定
//==========================================================================
bool CInputGamepad::GetPressRT(int nCntPlayer)
{
	return m_StateRT[nCntPlayer].bPress;
}

//==========================================================================
// LTのトリガー判定
//==========================================================================
bool CInputGamepad::GetTriggerLT(int nCntPlayer)
{
	return m_StateLT[nCntPlayer].bTrigger;
}

//==========================================================================
// RTのトリガー判定
//==========================================================================
bool CInputGamepad::GetTriggerRT(int nCntPlayer)
{
	return m_StateRT[nCntPlayer].bTrigger;
}

//==========================================================================
//  Lスティックの移動量取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveL(int nCntPlayer)
{

	// Lスティックの移動量
	MyLib::Vector3 StickMoveL =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX * 0.001f,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY * 0.001f,
			0.0f);

	return StickMoveL;
}

//==========================================================================
//  Rスティックの移動量取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveR(int nCntPlayer)
{
	// Rスティックの移動量
	MyLib::Vector3 StickMoveR =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX * 0.001f,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY * 0.001f,
			0.0f);

	return StickMoveR;
}


//==========================================================================
//  Lスティックの割合取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioL(int nCntPlayer)
{
	// Lスティックの割合
	MyLib::Vector3 StickRatio =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX / (float)SHRT_MAX,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY / (float)SHRT_MAX,
			0.0f);

	return StickRatio;
}

//==========================================================================
//  Rスティックの割合取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioR(int nCntPlayer)
{
	// Lスティックの割合
	MyLib::Vector3 StickRatio =
		MyLib::Vector3(
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX / (float)SHRT_MAX,
			(float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY / (float)SHRT_MAX,
			0.0f);

	return StickRatio;
}

//==========================================================================
// ゲームパッドのスティックの向き取得
//==========================================================================
float CInputGamepad::GetStickRotL(int nCntPlayer)
{
	// 角度を求める
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbLY - 0.0f));

	return fRot;
}

//==========================================================================
// ゲームパッドのスティックの向き取得
//==========================================================================
float CInputGamepad::GetStickRotR(int nCntPlayer)
{
	// 角度を求める
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY - 0.0f));

	return fRot;
}

//==========================================================================
// スティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetLStickTrigger(STICK XY)
{
	return m_bLeftStickTrigger[XY];
}

//==========================================================================
// スティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetRStickTrigger(STICK XY)
{
	return m_bRightStickTrigger[XY];
}

//==========================================================================
// パッドのリピートFPS
//==========================================================================
int CInputGamepad::GetnCntPad()
{
	return m_nCntPadrepeat;
}
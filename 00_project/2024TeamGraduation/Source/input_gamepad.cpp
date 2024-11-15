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
	const float DEADZONE = 0.2f;	// デッドゾーン
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
	memset(&m_fTapTimer, 0, sizeof(m_fTapTimer));	// タップ判定用のタイマー

	m_bVibrationUse = false;				// バイブを使用するかどうか
	m_nCntPadrepeat = 0;									// リピート用カウント
	m_fVibrationMulti = 0.0f;
	m_fDeadZone = 0.0f;						// デッドゾーン
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

	// スティックの情報を初期化
	memset(m_bLStickSelect, false, sizeof(m_bLStickSelect));	// 左トリガーの選択判定
	memset(m_bRStickSelect, false, sizeof(m_bRStickSelect));	// 右トリガーの選択判定

	memset(m_bLStickTip, false, sizeof(m_bLStickTip));	// 左スティックの傾き判定
	memset(m_bRStickTip, false, sizeof(m_bRStickTip));	// 右スティックの傾き判定

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

	// デッドゾーン
	m_fDeadZone = DEADZONE;

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
void CInputGamepad::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	XINPUT_STATE  aGamepadState[mylib_const::MAX_PLAYER];

	// 入力デバイスからデータを取得
	for (int nCntPlayer = 0; nCntPlayer < mylib_const::MAX_PLAYER; nCntPlayer++)
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

		// スティックのトリガー判定更新
		UpdateStickTrigger(nCntPlayer);

		// タップ判定タイマーの更新
		UpdateTapTimer(fDeltaTime, fDeltaRate, fSlowRate, nCntPlayer);
	}
}

//==========================================================================
// タップ判定タイマーの更新
//==========================================================================
void CInputGamepad::UpdateTapTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer)
{
	// タップ判定用のタイマー加算
	float time = fDeltaTime * fDeltaRate * fSlowRate;
	for (int i = 0; i < static_cast<int>(BUTTON::BUTTON_MAX); i++)
	{
		if (GetPress(static_cast<BUTTON>(i), nCntPlayer))
		{
			// タップ判定用のタイマー加算
			m_fTapTimer[i][nCntPlayer] += time;
		}
		else
		{
			m_fTapTimer[i][nCntPlayer] = 0.0f;
		}
	}
}

//==========================================================================
// スティックのトリガー
//==========================================================================
void CInputGamepad::UpdateStickTrigger(int nCntPlayer)
{
	
	//=============================
	// フラグリセット
	//=============================
	//--------------------------
	// トリガー
	//--------------------------
	memset(m_bLStickTrigger[nCntPlayer], false, sizeof(m_bLStickTrigger[nCntPlayer]));	// 左スティックのトリガー判定
	memset(m_bRStickTrigger[nCntPlayer], false, sizeof(m_bRStickTrigger[nCntPlayer]));	// 右スティックのトリガー判定

	//--------------------------
	// 傾き
	//--------------------------
	// Lスティック
	m_bLStickTip[nCntPlayer][STICK_AXIS::STICK_X] = fabsf(GetStickPositionRatioL(nCntPlayer).x) > 0.0f;
	m_bLStickTip[nCntPlayer][STICK_AXIS::STICK_Y] = fabsf(GetStickPositionRatioL(nCntPlayer).y) > 0.0f;

	// Rスティック
	m_bRStickTip[nCntPlayer][STICK_AXIS::STICK_X] = fabsf(GetStickPositionRatioR(nCntPlayer).x) > 0.0f;
	m_bRStickTip[nCntPlayer][STICK_AXIS::STICK_Y] = fabsf(GetStickPositionRatioR(nCntPlayer).y) > 0.0f;

	for (int i = 0; i < static_cast<int>(STICK_AXIS::STICK_MAX); i++)
	{
		// 左スティック
		if (!m_bLStickSelect[nCntPlayer][i] && m_bLStickTip[nCntPlayer][i])
		{// 前回選択判定OFF && 倒されてる
			m_bLStickSelect[nCntPlayer][i] = true;	// 選択判定
			m_bLStickTrigger[nCntPlayer][i] = true;	// トリガー判定
		}

		// 右スティック
		if (!m_bRStickSelect[nCntPlayer][i] && m_bRStickTip[nCntPlayer][i])
		{// 前回選択判定OFF && 倒されてる
			m_bRStickSelect[nCntPlayer][i] = true;	// 選択判定
			m_bRStickTrigger[nCntPlayer][i] = true;	// トリガー判定
		}


		//--------------------------
		// 戻し判定
		//--------------------------
		// 左スティック
		if (m_bLStickSelect[nCntPlayer][i] && !m_bLStickTip[nCntPlayer][i])
		{// スティックが戻っているとき
			m_bLStickSelect[nCntPlayer][i] = false;
		}

		// 右スティック
		if (m_bRStickSelect[nCntPlayer][i] && !m_bRStickTip[nCntPlayer][i])
		{// スティックが戻っているとき
			m_bRStickSelect[nCntPlayer][i] = false;
		}
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

	case VIBRATION_STATE_DMG:
	{
		// 減らしていく
		m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = m_aUpdateVib[nCntPlayer].wLeftMotorSpeed;
		m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = m_aUpdateVib[nCntPlayer].wRightMotorSpeed;
		m_aUpdateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
		m_aUpdateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	}

	default:
		break;
	}

	// 補正
	UtilFunc::Transformation::Clamp(m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed, static_cast<WORD>(0.0f), static_cast<WORD>(USHRT_MAX));
	UtilFunc::Transformation::Clamp(m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed, static_cast<WORD>(0.0f), static_cast<WORD>(USHRT_MAX));

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
bool CInputGamepad::GetRelease(BUTTON nKey, int nCntPlayer)
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
// タップ取得
//==========================================================================
CInputGamepad::STapInfo CInputGamepad::GetTap(BUTTON nKey, int nCntPlayer, float tapTime)
{
	// タップ情報
	STapInfo returnInfo;

	// タップの時間ないのは即
	if (tapTime <= 0.0f)
	{
		returnInfo.bInput = true;
		returnInfo.fRatio = 1.0f;
		return returnInfo;
	}

	// ボタン範囲外
	if (nKey >= BUTTON::BUTTON_MAX) return returnInfo;

	// 離された && 指定の時間内
	returnInfo.bInput = GetRelease(nKey, nCntPlayer) && m_fTapTimer[nKey][nCntPlayer] <= tapTime;

	// タップの割合
	returnInfo.fRatio = UtilFunc::Transformation::Clamp(m_fTapTimer[nKey][nCntPlayer] / tapTime, 0.0f, 1.0f);

	return returnInfo;
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
	return MyLib::Vector3(
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLX),
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLY),
		0.0f);
}

//==========================================================================
//  Rスティックの移動量取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveR(int nCntPlayer)
{
	// Rスティックの移動量
	return MyLib::Vector3(
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRX),
		static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRY),
		0.0f);
}


//==========================================================================
//  Lスティックの割合取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioL(int nCntPlayer)
{
	// デッドゾーン反映後の割合
	const float shortMax = static_cast<float>(SHRT_MAX);
	float deadZoneRate = (shortMax * m_fDeadZone);

	// X軸
	float thumbLX = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLX);
	float ratioX = 0.0f;

	if (fabs(thumbLX) > deadZoneRate)
	{// デッドゾーン外

		if (thumbLX > 0) 
		{
			// 正の値の場合
			ratioX = (thumbLX - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else 
		{
			// 負の値の場合
			ratioX = (thumbLX + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	// Y軸
	float thumbLY = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbLY);
	float ratioY = 0.0f;

	if (fabs(thumbLY) > deadZoneRate)
	{// デッドゾーン外

		if (thumbLY > 0)
		{
			// 正の値の場合
			ratioY = (thumbLY - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else
		{
			// 負の値の場合
			ratioY = (thumbLY + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	return MyLib::Vector3(ratioX, ratioY, 0.0f);
}

//==========================================================================
//  Rスティックの割合取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickPositionRatioR(int nCntPlayer)
{
	// デッドゾーン反映後の割合
	const float shortMax = static_cast<float>(SHRT_MAX);
	float deadZoneRate = (shortMax * m_fDeadZone);

	// X軸
	float thumbRX = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRX);
	float ratioX = 0.0f;

	if (fabs(thumbRX) > deadZoneRate)
	{// デッドゾーン外

		if (thumbRX > 0)
		{
			// 正の値の場合
			ratioX = (thumbRX - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else
		{
			// 負の値の場合
			ratioX = (thumbRX + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	// Y軸
	float thumbRY = static_cast<float>(m_aGamepadState[nCntPlayer].Gamepad.sThumbRY);
	float ratioY = 0.0f;

	if (fabs(thumbRY) > deadZoneRate)
	{// デッドゾーン外

		if (thumbRY > 0)
		{
			// 正の値の場合
			ratioY = (thumbRY - deadZoneRate) / (shortMax - deadZoneRate);
		}
		else
		{
			// 負の値の場合
			ratioY = (thumbRY + deadZoneRate) / (shortMax - deadZoneRate);
		}
	}

	return MyLib::Vector3(ratioX, ratioY, 0.0f);
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
bool CInputGamepad::GetLStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	return m_bLStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// スティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetRStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	return m_bRStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// パッドのリピートFPS
//==========================================================================
int CInputGamepad::GetnCntPad()
{
	return m_nCntPadrepeat;
}
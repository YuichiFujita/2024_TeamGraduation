//=============================================================================
//
// 入力(ゲームパッド)処理 [input_gamepad.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input_gamepad.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int SHOT_FPS = 15;	// 弾の間隔
	const float DEADZONE = 0.99f;	// デッドゾーン

	namespace Vib
	{
		CInputGamepad::SVib SETINFO[CInputGamepad::EVibType::VIBTYPE_MAX] =		// 設定する情報
		{
			CInputGamepad::SVib(0.0f, 0.00f, 0.00f, 0.00f, CInputGamepad::EEasing::Linear),		// VIBTYPE_NONE
			CInputGamepad::SVib(0.0f, 0.12f, 0.80f, 0.50f, CInputGamepad::EEasing::Linear),		// VIBTYPE_CATCH_NORMAL
			CInputGamepad::SVib(0.0f, 0.27f, 0.67f, 0.34f, CInputGamepad::EEasing::EaseInOut),	// VIBTYPE_CATCH_FAST
			CInputGamepad::SVib(0.0f, 0.15f, 0.55f, 0.76f, CInputGamepad::EEasing::EaseOut),	// VIBTYPE_THROW_NORMAL
			CInputGamepad::SVib(0.0f, 0.20f, 0.55f, 0.77f, CInputGamepad::EEasing::EaseOut),	// VIBTYPE_THROW_FAST
			CInputGamepad::SVib(0.0f, 0.18f, 1.00f, 0.85f, CInputGamepad::EEasing::EaseInOut),	// VIBTYPE_HIT
			CInputGamepad::SVib(0.0f, 0.60f, 1.00f, 0.85f, CInputGamepad::EEasing::EaseInOut),	// VIBTYPE_HIT_SP
			CInputGamepad::SVib(0.0f, 2.19f, 0.80f, 0.00f, CInputGamepad::EEasing::EaseInExpo),	// VIBTYPE_HIT_SP
		};
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// float線形補正リスト
CInputGamepad::FLOAT_EASING_FUNC CInputGamepad::m_FloatEasingFunc[] =
{
	&UtilFunc::Correction::EasingLinear,
	&UtilFunc::Correction::EasingQuintIn,
	&UtilFunc::Correction::EasingQuintOut,
	&UtilFunc::Correction::EasingEaseInOutQuart,
	&UtilFunc::Correction::EaseInExpo,
	&UtilFunc::Correction::EaseOutExpo,
	&UtilFunc::Correction::EaseInOutExpo,
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
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
		m_aVibType[nCntPlayer] = VIBTYPE_NONE;	// 振動の種類
		m_aVibInfo[nCntPlayer] = SVib();				// 振動情報
	}
	memset(&m_fTapTimer, 0, sizeof(m_fTapTimer));		// タップ判定用のタイマー
	memset(&m_fOldTapTimer, 0, sizeof(m_fOldTapTimer));	// 前回のタップ判定用のタイマー

	m_bVibrationUse = false;				// バイブを使用するかどうか
	m_nCntPadrepeat = 0;					// リピート用カウント
	m_fVibrationMulti = 1.0f;				// バイブレーション倍率
	m_fDeadZone = 0.0f;						// デッドゾーン

	// デバッグ
	m_EditVibType = EVibType::VIBTYPE_NONE;	// 編集するバイブの種類
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
		m_aVibType[nCntPlayer] = VIBTYPE_NONE;			// バイブの種類
		m_aVibInfo[nCntPlayer] = SVib();				// 振動情報
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));

		memset(&m_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&m_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
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

		// 振動更新
		UpdateVibration(fDeltaTime, fDeltaRate, fSlowRate, nCntPlayer);

		// スティックのトリガー判定更新
		UpdateStickTrigger(nCntPlayer);

		// タップ判定タイマーの更新
		UpdateTapTimer(fDeltaTime, fDeltaRate, fSlowRate, nCntPlayer);
	}

#if _DEBUG

	// デバッグ
	Debug();
#endif // _DEBUG

}

//==========================================================================
// タップ判定タイマーの更新
//==========================================================================
void CInputGamepad::UpdateTapTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

	// タップ判定用のタイマー加算
	float time = fDeltaTime * fDeltaRate * fSlowRate;
	for (int i = 0; i < static_cast<int>(BUTTON::BUTTON_MAX); i++)
	{
		// 前回のタップ判定用のタイマー
		m_fOldTapTimer[i][nCntPlayer] = m_fTapTimer[i][nCntPlayer];

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
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

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
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

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
void CInputGamepad::UpdateVibration(const float fDeltaTime, const float fDeltaRate, const float fSlowRate, int nCntPlayer)
{
	// バイブ情報
	SVib* pVibInfo = &m_aVibInfo[nCntPlayer];

	// 振動タイマーを加算
	pVibInfo->timer += fDeltaTime;	// 振動情報

	// 振動のパラメータ設定
	SetVibrationParam(nCntPlayer);

	// 完了
	if (pVibInfo->timer >= pVibInfo->maxTimer)
	{
		// 振動タイマーリセット
		pVibInfo->timer = 0.0;

		// 何もしてない状態に設定
		SetVibration(EVibType::VIBTYPE_NONE, nCntPlayer);

		// メモリ初期化
		memset(&m_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
	}
}

//==========================================================================
// 振動のパラメータ設定
//==========================================================================
void CInputGamepad::SetVibrationParam(int nCntPlayer)
{
	// バイブ情報
	SVib* pVibInfo = &m_aVibInfo[nCntPlayer];

	// イージング補間
	float speed = m_FloatEasingFunc[pVibInfo->easeType](pVibInfo->startSpeed, pVibInfo->endSpeed, 0.0f, pVibInfo->maxTimer, pVibInfo->timer);
	speed *= static_cast<float>(USHRT_MAX);

	// 補正
	speed = UtilFunc::Transformation::Clamp(speed, static_cast<float>(0.0f), static_cast<float>(USHRT_MAX));
	m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = speed;
	m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = speed;

	// コントローラーにバイブの情報をXINPUTに送る
	m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed *= m_fVibrationMulti;
	m_aGamepadStateVib[nCntPlayer].wRightMotorSpeed *= m_fVibrationMulti;
	XInputSetState(nCntPlayer, &m_aGamepadStateVib[nCntPlayer]);
}

//==========================================================================
// バイブの設定処理
//==========================================================================
void CInputGamepad::SetVibration(EVibType vibType, int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return;

	// バイブ使用しないときはやめる
	if (!m_bVibrationUse)
	{
		// 振動のパラメータ設定
		m_aVibInfo[nCntPlayer] = Vib::SETINFO[EVibType::VIBTYPE_NONE];
		SetVibrationParam(nCntPlayer);
		return;
	}

	// バイブの種類
	m_aVibType[nCntPlayer] = vibType;

	// 初期化
	m_aVibInfo[nCntPlayer].timer = 0.0f;

	// 振動のパラメータ設定
	m_aVibInfo[nCntPlayer] = Vib::SETINFO[vibType];
	SetVibrationParam(nCntPlayer);
}

//==========================================================================
// プレス初期化処理 (ボタン)
//==========================================================================
void CInputGamepad::InitPress(int nPadIdx)
{
	// メモリをクリア
	memset(&m_aGamepadState[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadState[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// トリガー初期化処理 (ボタン)
//==========================================================================
void CInputGamepad::InitTrigger(int nPadIdx)
{
	// メモリをクリア
	memset(&m_aGamepadStateTrigger[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadStateTrigger[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// リピート初期化処理 (ボタン)
//==========================================================================
void CInputGamepad::InitRepeat(int nPadIdx)
{
	// メモリをクリア
	memset(&m_aGamepadStateRepeat[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadStateRepeat[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// リリース初期化処理 (ボタン)
//==========================================================================
void CInputGamepad::InitRelease(int nPadIdx)
{
	// メモリをクリア
	memset(&m_aGamepadStateRelease[nPadIdx].Gamepad.wButtons, 0, sizeof(m_aGamepadStateRelease[nPadIdx].Gamepad.wButtons));
}

//==========================================================================
// ゲームパッドのプレス処理
//==========================================================================
bool CInputGamepad::GetPress(BUTTON nKey, int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadState[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// ゲームパッドのトリガー処理
//==========================================================================
bool CInputGamepad::GetTrigger(BUTTON nKey, int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// ゲームパッドのリリース処理
//==========================================================================
bool CInputGamepad::GetRelease(BUTTON nKey, int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// ゲームパッドのリピート処理
//==========================================================================
bool CInputGamepad::GetRepeat(BUTTON nKey, int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return (m_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==========================================================================
// タップ取得
//==========================================================================
CInputGamepad::STapInfo CInputGamepad::GetTap(BUTTON nKey, int nCntPlayer, float tapTime)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return {};

	// タップ情報
	STapInfo returnInfo;

	// タップの時間ないのは即
	if (tapTime <= 0.0f)
	{
		returnInfo.bInput = true;
		returnInfo.bComplete = true;
		returnInfo.fRatio = 1.0f;
		return returnInfo;
	}

	// ボタン範囲外
	if (nKey >= BUTTON::BUTTON_MAX) return returnInfo;

	// 離された && 指定の時間内
	returnInfo.bInput = GetRelease(nKey, nCntPlayer) && m_fTapTimer[nKey][nCntPlayer] <= tapTime;

	// タップの割合
	returnInfo.fRatio = UtilFunc::Transformation::Clamp(m_fTapTimer[nKey][nCntPlayer] / tapTime, 0.0f, 1.0f);

	if (returnInfo.fRatio >= 1.0f)
	{// 時間経過

		// 過去は経過していなかったとき
		float oldRatio = UtilFunc::Transformation::Clamp(m_fOldTapTimer[nKey][nCntPlayer] / tapTime, 0.0f, 1.0f);
		returnInfo.bComplete = oldRatio < 1.0f;
	}

	return returnInfo;
}

//==========================================================================
// ゲームパッドのプレス処理
//==========================================================================
bool CInputGamepad::GetAllPress(BUTTON nKey)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // プレイヤーの総数分繰り返す

		// ゲームパッドのプレス
		if (GetPress(nKey, i))
		{
			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//==========================================================================
// ゲームパッドのトリガー処理
//==========================================================================
CInputGamepad::SAllTrigger CInputGamepad::GetAllTrigger(BUTTON nKey)
{
	SAllTrigger info;
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // プレイヤーの総数分繰り返す

		// ゲームパッドのトリガー
		if (GetTrigger(nKey, i))
		{
			// 入力情報ありを返す
			return SAllTrigger(true, i);
		}
	}

	// 入力情報無しを返す
	return info;
}

//==========================================================================
// ゲームパッドのリピート処理
//==========================================================================
bool CInputGamepad::GetAllRepeat(BUTTON nKey)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // プレイヤーの総数分繰り返す

		// ゲームパッドのリピート
		if (GetRepeat(nKey, i))
		{
			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//==========================================================================
// ゲームパッドのリリース処理
//==========================================================================
bool CInputGamepad::GetAllRelease(BUTTON nKey)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // プレイヤーの総数分繰り返す

		// ゲームパッドのリリース
		if (GetRelease(nKey, i))
		{
			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//==========================================================================
// LTのプレス判定
//==========================================================================
bool CInputGamepad::GetPressLT(int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateLT[nCntPlayer].bPress;
}

//==========================================================================
// RTのプレス判定
//==========================================================================
bool CInputGamepad::GetPressRT(int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateRT[nCntPlayer].bPress;
}

//==========================================================================
// LTのトリガー判定
//==========================================================================
bool CInputGamepad::GetTriggerLT(int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateLT[nCntPlayer].bTrigger;
}

//==========================================================================
// RTのトリガー判定
//==========================================================================
bool CInputGamepad::GetTriggerRT(int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	return m_StateRT[nCntPlayer].bTrigger;
}

//==========================================================================
//  Lスティックの移動量取得
//==========================================================================
MyLib::Vector3 CInputGamepad::GetStickMoveL(int nCntPlayer)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

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
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

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
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

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
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return VEC3_ZERO;

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
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return 0.0f;

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
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return 0.0f;

	// 角度を求める
	float fRot = atan2f(
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRX - 0.0f),
		((float)m_aGamepadState[nCntPlayer].Gamepad.sThumbRY - 0.0f));

	return fRot;
}

//==========================================================================
// Lスティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetLStickTrigger(int nCntPlayer, STICK_CROSS_AXIS closs)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	switch (closs)
	{ // 十字軸ごとの処理
	case STICK_CROSS_LEFT:
	case STICK_CROSS_RIGHT:
	{ // X軸の場合

		// X軸入力がない場合抜ける
		if (!m_bLStickTrigger[nCntPlayer][STICK_X]) return false;

		// スティック傾きベクトル取得
		MyLib::Vector3 vec = GetStickPositionRatioL(nCntPlayer);
		if		(closs == STICK_CROSS_LEFT)	 return (vec.x <= 0.0f);
		else if	(closs == STICK_CROSS_RIGHT) return (vec.x >= 0.0f);
		break;
	}
	case STICK_CROSS_UP:
	case STICK_CROSS_DOWN:
	{ // Y軸の場合

		// Y軸入力がない場合抜ける
		if (!m_bLStickTrigger[nCntPlayer][STICK_Y]) return false;

		// スティック傾きベクトル取得
		MyLib::Vector3 vec = GetStickPositionRatioL(nCntPlayer);
		if		(closs == STICK_CROSS_UP) 	return (vec.y >= 0.0f);
		else if	(closs == STICK_CROSS_DOWN)	return (vec.y <= 0.0f);
		break;
	}
	default:
		assert(false);
		break;
	}

	assert(false);
	return false;
}

//==========================================================================
// Rスティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetRStickTrigger(int nCntPlayer, STICK_CROSS_AXIS closs)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return false;

	switch (closs)
	{ // 十字軸ごとの処理
	case STICK_CROSS_LEFT:
	case STICK_CROSS_RIGHT:
	{ // X軸の場合

		// X軸入力がない場合抜ける
		if (!m_bRStickTrigger[nCntPlayer][STICK_X]) return false;

		// スティック傾きベクトル取得
		MyLib::Vector3 vec = GetStickPositionRatioR(nCntPlayer);
		if		(closs == STICK_CROSS_LEFT)	 return (vec.x <= 0.0f);
		else if (closs == STICK_CROSS_RIGHT) return (vec.x >= 0.0f);
		break;
	}
	case STICK_CROSS_UP:
	case STICK_CROSS_DOWN:
	{ // Y軸の場合

		// Y軸入力がない場合抜ける
		if (!m_bRStickTrigger[nCntPlayer][STICK_Y]) return false;

		// スティック傾きベクトル取得
		MyLib::Vector3 vec = GetStickPositionRatioR(nCntPlayer);
		if		(closs == STICK_CROSS_UP) 	return (vec.y >= 0.0f);
		else if (closs == STICK_CROSS_DOWN)	return (vec.y <= 0.0f);
		break;
	}
	default:
		assert(false);
		break;
	}

	assert(false);
	return false;
}

//==========================================================================
// Lスティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetAllLStickTrigger(STICK_CROSS_AXIS closs)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // プレイヤーの総数分繰り返す

		if (GetLStickTrigger(i, closs))
		{
			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//==========================================================================
// Rスティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetAllRStickTrigger(STICK_CROSS_AXIS closs)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // プレイヤーの総数分繰り返す

		if (GetRStickTrigger(i, closs))
		{
			// 入力情報ありを返す
			return true;
		}
	}

	// 入力情報無しを返す
	return false;
}

//==========================================================================
// スティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetLStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return {};

	return m_bLStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// スティックのトリガー判定
//==========================================================================
bool CInputGamepad::GetRStickTrigger(int nCntPlayer, STICK_AXIS XY)
{
	// プレイヤーインデックスが範囲外の場合抜ける
	if (nCntPlayer <= -1 || nCntPlayer >= mylib_const::MAX_PLAYER) return {};

	return m_bRStickTrigger[nCntPlayer][XY];
}

//==========================================================================
// パッドのリピートFPS
//==========================================================================
int CInputGamepad::GetnCntPad()
{
	return m_nCntPadrepeat;
}

//==========================================================================
// デバッグ
//==========================================================================
void CInputGamepad::Debug()
{
	if (ImGui::TreeNode("Gamepad"))
	{
		// 起動
		if (ImGui::Button("Vib!"))
		{
			SetVibration(m_EditVibType, 0);
		}

		// 編集するバイブの種類
		int type = (int)m_EditVibType;
		ImGui::Text(magic_enum::enum_name(m_EditVibType).data());
		ImGui::SliderInt("EditVibType", &type, 0, EVibType::VIBTYPE_MAX - 1);
		m_EditVibType = (EVibType)type;

		// 調整
		ImGui::DragFloat("maxTimer", (float*)&Vib::SETINFO[m_EditVibType].maxTimer, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("startSpeed", (float*)&Vib::SETINFO[m_EditVibType].startSpeed, 0.01f, 0.0f, 1.0f, "%.2f");
		ImGui::DragFloat("endSpeed", (float*)&Vib::SETINFO[m_EditVibType].endSpeed, 0.01f, 0.0f, 1.0f, "%.2f");

		// モータースピード
		ImGui::Text("m_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed : %.2f", (float)m_aGamepadStateVib[0].wLeftMotorSpeed);

		// イージング
		int esseType = Vib::SETINFO[m_EditVibType].easeType;
		ImGui::SliderInt("EaseType", &esseType, 0, static_cast<int>(EEasing::MAX) - 1, "%d");
		Vib::SETINFO[m_EditVibType].easeType = static_cast<EEasing>(esseType);
		ImGui::SameLine();
		ImGui::Text("[%s]", magic_enum::enum_name(Vib::SETINFO[m_EditVibType].easeType));

		ImGui::TreePop();
	}
}

//=============================================================================
//
// 入力(キーボード)処理 [input_keyboard.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input_keyboard.h"
#include "debugproc.h"
#include "calculation.h"

CInputKeyboard* CInputKeyboard::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < mylib_const::NUM_KEY_MAX; nCntKey++)
	{
		m_nCntKeyRepeat[nCntKey] = 0;		// リピート用カウント
		m_nPatternRepeat[nCntKey] = 1;		// リピ―トの間隔
	}
	memset(m_aKeyState, 0, sizeof(m_aKeyState));				// プレス情報
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));	// トリガー情報
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));	// リリース情報
	memset(m_aKeyStateRepeat, 0, sizeof(m_aKeyStateRepeat));	// リピート情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CInputKeyboard* CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	// インスタンス生成
	m_pThisPtr = DEBUG_NEW CInputKeyboard;
	if (m_pThisPtr != nullptr) {

		// 初期化処理
		m_pThisPtr->Init(hInstance, hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CInputKeyboard::Uninit()
{
	// 終了処理
	CInput::Uninit();

	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[mylib_const::NUM_KEY_MAX];	// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < mylib_const::NUM_KEY_MAX; nCntKey++)
		{
			// トリガー情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// リリース情報を保存
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];

			// キーボードのプレス情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];

			if (GetPress(nCntKey))
			{
				// リピートカウント加算
				m_nCntKeyRepeat[nCntKey] = (m_nCntKeyRepeat[nCntKey] + 1) % m_nPatternRepeat[nCntKey];

				if ((m_nCntKeyRepeat[nCntKey] % m_nPatternRepeat[nCntKey]) == 0)
				{
					// リリース情報を保存
					m_nCntKeyRepeat[nCntKey] = (m_nCntKeyRepeat[nCntKey] + 1) % m_nPatternRepeat[nCntKey];

					// プレス情報を保存
					m_aKeyStateRepeat[nCntKey] = m_aKeyState[nCntKey];
				}
				else
				{
					// リピートにリリースの情報保存
					m_aKeyStateRepeat[nCntKey] = m_aKeyStateRelease[nCntKey];
				}
			}
			else
			{
				// リピートにリリースの情報保存
				m_aKeyStateRepeat[nCntKey] = m_aKeyStateRelease[nCntKey];
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を獲得
		m_pDevice->Acquire();
	}
}

//==========================================================================
// キーボードのプレス情報を取得
//==========================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//==========================================================================
// キーボードのトリガー情報を取得
//==========================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==========================================================================
// キーボードのリリース情報を取得
//==========================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==========================================================================
// キーボードのリピート情報を取得
//==========================================================================
bool CInputKeyboard::GetRepeat(int nKey, int nPattern)
{
	m_nPatternRepeat[nKey] = nPattern;		// リピ―トの間隔
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}
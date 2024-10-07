//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input.h"
#include "debugproc.h"
#include "manager.h"
#include "calculation.h"

#include "input_mouse.h"
#include "input_keyboard.h"
#include "input_gamepad.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInputオブジェクトへのポインタ
CInput* CInput::m_pThisPtr = nullptr;		// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CInput::CInput()
{
	m_pMouse = nullptr;
	m_pKeyboard = nullptr;
	m_pGamepad = nullptr;
	m_pDevice = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CInput::~CInput()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CInput* CInput::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	m_pThisPtr = DEBUG_NEW CInput;
	if (m_pThisPtr == nullptr) {
		return nullptr;
	}

	// マウス
	m_pThisPtr->m_pMouse = CInputMouse::Create(hInstance, hWnd);
	if (m_pThisPtr->m_pMouse == nullptr) {
		return nullptr;
	}

	// キーボード
	m_pThisPtr->m_pKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	if (m_pThisPtr->m_pKeyboard == nullptr) {
		return nullptr;
	}

	// ゲームパッド
	m_pThisPtr->m_pGamepad = CInputGamepad::Create(hInstance, hWnd);
	if (m_pThisPtr->m_pGamepad == nullptr) {
		return nullptr;
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (m_pInput == nullptr)
	{// まだ生成していなかったら

		// DirectInputオブジェクトの生成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void CInput::Release()
{
	if (m_pMouse != nullptr) {
		m_pMouse->Uninit();
		m_pMouse = nullptr;
	}

	if (m_pKeyboard != nullptr) {
		m_pKeyboard->Uninit();
		m_pKeyboard = nullptr;
	}

	if (m_pGamepad != nullptr) {
		m_pGamepad->Uninit();
		m_pGamepad = nullptr;
	}

	// オブジェクトの破棄
	if (m_pInput != nullptr) {
		m_pInput->Release();
		m_pInput = nullptr;
	}

	if (m_pThisPtr != nullptr) {
		delete m_pThisPtr;
		m_pThisPtr = nullptr;
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CInput::Uninit()
{
	// 入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();	// アクセス権を放棄
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// オブジェクトの破棄
	if (m_pInput != nullptr){
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CInput::Update()
{
	m_pMouse->Update();
	m_pKeyboard->Update();
	m_pGamepad->Update();
}
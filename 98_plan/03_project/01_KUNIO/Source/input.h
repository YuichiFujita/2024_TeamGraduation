//=============================================================================
//
// 入力処理 [input.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _INPUT_H_	//このマクロ定義がされていなかったら
#define _INPUT_H_	//二重インクルード防止のマクロを定義する

#include "constans.h"
#pragma comment(lib, "xinput.lib")	//コントローラーに必要

class CInputMouse;
class CInputKeyboard;
class CInputGamepad;
class CKeyConfigManager;

//==========================================================================
// クラス定義
//==========================================================================
// 入力クラス
class CInput
{
public:

	// 列挙型定義
	enum TYPE
	{
		TYPE_KEYBOARD = 0,
		TYPE_PAD,
		TYPE_MOUSE,
		TYPE_MAX
	};

	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update();
	void Release();

	static CInput* Create(HINSTANCE hInstance, HWND hWnd);	// 生成処理

protected:
	static LPDIRECTINPUT8 m_pInput;		// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;		// 入力デバイスへのポインタ

private:

	CInputMouse* m_pMouse;
	CInputKeyboard* m_pKeyboard;
	CInputGamepad* m_pGamepad;
	CKeyConfigManager* m_pKeyConfig;
	static CInput* m_pThisPtr;	// 自身のポインタ
};



#endif
//=============================================================================
//
// 入力(キーボード)処理 [input_keyboard.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _INPUT_KEYBOARD_H_	//このマクロ定義がされていなかったら
#define _INPUT_KEYBOARD_H_	//二重インクルード防止のマクロを定義する

#include "input.h"

//==========================================================================
// クラス定義
//==========================================================================
// キーボードクラス
class CInputKeyboard : public CInput
{
public:

	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey, int nPattern);

	static CInputKeyboard* GetInstance() { return m_pThisPtr; }		// インスタンス取得
	static CInputKeyboard* Create(HINSTANCE hInstance, HWND hWnd);	// 生成処理

private:
	int m_nPatternRepeat[mylib_const::NUM_KEY_MAX];		// リピ―トの間隔
	int m_nCntKeyRepeat[mylib_const::NUM_KEY_MAX];		// リピート用カウント
	BYTE m_aKeyState[mylib_const::NUM_KEY_MAX];			// プレス情報
	BYTE m_aKeyStateTrigger[mylib_const::NUM_KEY_MAX];	// トリガー情報
	BYTE m_aKeyStateRelease[mylib_const::NUM_KEY_MAX];	// リリース情報
	BYTE m_aKeyStateRepeat[mylib_const::NUM_KEY_MAX];	// リピート情報

	static CInputKeyboard* m_pThisPtr;	// 自身のポインタ
};

#endif
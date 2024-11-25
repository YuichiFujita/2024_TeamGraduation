//==========================================================================
// 
//  左移動キー処理 [bindKeyLeft.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "bindKeyLeft.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const CInputGamepad::BUTTON BUTTON = CInputGamepad::BUTTON_LEFT;
	const int KEY = DIK_A;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBindKeyLeft::CBindKeyLeft()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CBindKeyLeft::~CBindKeyLeft()
{

}

//==========================================================================
//	割当キープレスの取得処理
//==========================================================================
bool CBindKeyLeft::IsPress(const int nPadIdx) const
{
	// インプット情報の取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報

	// 入力フラグを返す
	return (pKey->GetPress(KEY) || pPad->GetPress(BUTTON, nPadIdx));
}

//==========================================================================
//	割当キートリガーの取得処理
//==========================================================================
bool CBindKeyLeft::IsTrigger(const int nPadIdx) const
{
	// インプット情報の取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報

	// 入力フラグを返す
	return (pKey->GetTrigger(KEY) || pPad->GetTrigger(BUTTON, nPadIdx));
}

//==========================================================================
//	割当スティック傾きの取得処理
//==========================================================================
bool CBindKeyLeft::IsStickTilt(const int nPadIdx) const
{
	// インプット情報の取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報

	// 傾きフラグを返す
	return (pPad->GetStickMoveL(nPadIdx).x < 0.0f);
}

//==========================================================================
//	割当キー向きの取得処理
//==========================================================================
float CBindKeyLeft::GetRotation() const
{
	// 左の向きを返す
	return -HALF_PI;
}

//==========================================================================
//	割当キー方向列挙の取得処理
//==========================================================================
CPlayer::EDashAngle CBindKeyLeft::GetAngle() const
{
	// 左の方向列挙を返す
	return CPlayer::EDashAngle::ANGLE_LEFT;
}

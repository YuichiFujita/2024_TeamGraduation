//==========================================================================
// 
//  右移動キー処理 [bindKeyRight.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "bindKeyRight.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const CInputGamepad::BUTTON BUTTON = CInputGamepad::BUTTON_RIGHT;
	const int KEY = DIK_D;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBindKeyRight::CBindKeyRight()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CBindKeyRight::~CBindKeyRight()
{

}

//==========================================================================
//	割当キープレスの取得処理
//==========================================================================
bool CBindKeyRight::IsPress(const int nPadIdx) const
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
bool CBindKeyRight::IsTrigger(const int nPadIdx) const
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
bool CBindKeyRight::IsStickTilt(const int nPadIdx) const
{
	// インプット情報の取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報

	// 傾きフラグを返す
	return (pPad->GetStickMoveL(nPadIdx).x > 0.0f);
}

//==========================================================================
//	割当キー向きの取得処理
//==========================================================================
float CBindKeyRight::GetRotation() const
{
	// 右の向きを返す
	return HALF_PI;
}

//==========================================================================
//	割当キー方向列挙の取得処理
//==========================================================================
CPlayer::EDashAngle CBindKeyRight::GetAngle() const
{
	// 右の方向列挙を返す
	return CPlayer::EDashAngle::ANGLE_RIGHT;
}

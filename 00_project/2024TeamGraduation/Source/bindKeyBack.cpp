//==========================================================================
// 
//  後退キー処理 [bindKeyBack.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "bindKeyBack.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const CInputGamepad::BUTTON BUTTON = CInputGamepad::BUTTON_DOWN;
	const int KEY = DIK_S;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBindKeyBack::CBindKeyBack()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CBindKeyBack::~CBindKeyBack()
{

}

//==========================================================================
//	割当キープレスの取得処理
//==========================================================================
bool CBindKeyBack::IsPress(const int nPadIdx) const
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
bool CBindKeyBack::IsTrigger(const int nPadIdx) const
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
bool CBindKeyBack::IsStickTilt(const int nPadIdx) const
{
	// インプット情報の取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報

	// 傾きフラグを返す
	return (pPad->GetStickMoveL(nPadIdx).y < 0.0f);
}

//==========================================================================
//	割当キー向きの取得処理
//==========================================================================
float CBindKeyBack::GetRotation() const
{
	// 後方の向きを返す
	return 0.0f;
}

//==========================================================================
//	割当キー方向列挙の取得処理
//==========================================================================
CPlayer::EDashAngle CBindKeyBack::GetAngle() const
{
	// 後方の方向列挙を返す
	return CPlayer::EDashAngle::ANGLE_DOWN;
}

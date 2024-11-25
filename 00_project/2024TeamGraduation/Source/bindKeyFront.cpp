//==========================================================================
// 
//  前進キー処理 [bindKeyFront.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "bindKeyFront.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const CInputGamepad::BUTTON BUTTON = CInputGamepad::BUTTON_UP;
	const int KEY = DIK_W;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBindKeyFront::CBindKeyFront()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CBindKeyFront::~CBindKeyFront()
{

}

//==========================================================================
//	割当キープレスの取得処理
//==========================================================================
bool CBindKeyFront::IsPress(const int nPadIdx) const
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
bool CBindKeyFront::IsTrigger(const int nPadIdx) const
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
bool CBindKeyFront::IsStickTilt(const int nPadIdx) const
{
	// インプット情報の取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報

	// 傾きフラグを返す
	return (pPad->GetStickMoveL(nPadIdx).y > 0.0f);
}

//==========================================================================
//	割当キー向きの取得処理
//==========================================================================
float CBindKeyFront::GetRotation() const
{
	// 前方の向きを返す
	return D3DX_PI;
}

//==========================================================================
//	割当キー方向列挙の取得処理
//==========================================================================
CPlayer::EDashAngle CBindKeyFront::GetAngle() const
{
	// 前方の方向列挙を返す
	return CPlayer::EDashAngle::ANGLE_UP;
}

//==========================================================================
// 
//  前進キーヘッダー [bindKeyFront.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _BIND_KEY_FRONT_H_
#define _BIND_KEY_FRONT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "bindKey.h"

//==========================================================================
// クラス定義
//==========================================================================
// 前進キークラス
class CBindKeyFront : public CBindKey
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CBindKeyFront();
	virtual ~CBindKeyFront();

	//=============================
	// オーバーライド関数
	//=============================
	virtual bool IsPress(const int nPadIdx) const override;		// 割当キープレス取得
	virtual bool IsTrigger(const int nPadIdx) const override;	// 割当キートリガー取得
	virtual bool IsStickTilt(const int nPadIdx) const override;	// 割当スティック傾き取得
	virtual float GetRotation() const override;					// 割当キー向き取得
	virtual CPlayer::EDashAngle GetAngle() const override;		// 割当キー方向列挙取得
};

#endif

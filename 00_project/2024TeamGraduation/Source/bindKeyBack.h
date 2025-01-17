//==========================================================================
// 
//  後退キーヘッダー [bindKeyBack.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _BIND_KEY_BACK_H_
#define _BIND_KEY_BACK_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "bindKey.h"

//==========================================================================
// クラス定義
//==========================================================================
// 後退キークラス
class CBindKeyBack : public CBindKey
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CBindKeyBack();
	virtual ~CBindKeyBack();

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

//=============================================================================
// 
//  定数ヘッダー [constans.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CONSTANS_H_
#define _CONSTANS_H_	// 二重インクルード防止

#include "main.h"

//==========================================================================
// 名前空間
//==========================================================================
// 定数定義
namespace mylib_const
{
	const int MAX_OBJ = 1600;			// オブジェクトの最大数
	const int PRIORITY_NUM = 8;			// 優先順位の最大数
	const int DEFAULT_PRIORITY = 3;		// 優先順位のデフォルト
	const int DEF2D_PRIORITY = 4;		// 優先順位の2Dデフォルト
	const int MAX_STRING = 512;			// 最大文字数
	const int MAX_MODEL = 64;			// モデル最大数
	const int MAX_PARTS = 32;			// パーツ最大数
	const int MAX_PATTEN_ENEMY = 32;	// 敵の最大パターン
	const int MAX_ENEMY = 256;			// 敵の最大数
	const float GRAVITY = 1.95f;		// 重力
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// デフォルトの色
	const D3DXVECTOR3 DEFAULT_SCALE = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// スケールのデフォルト値
	const D3DXVECTOR3 DEFAULT_VECTOR3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// Vector3のデフォルト値
	const D3DXVECTOR2 DEFAULT_VECTOR2 = D3DXVECTOR2(0.0f, 0.0f);		// Vector2のデフォルト値
}

#endif
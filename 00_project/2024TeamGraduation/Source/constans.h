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
	const int DEFAULT_FPS = 60;			// 通常のFPS
	const int PRIORITY_NUM = 12;		// 優先順位の最大数
	const int PRIORITY_DEFAULT = 3;		// 優先順位のデフォルト
	const int PRIORITY_DEF2D = 4;		// 優先順位の2Dデフォルト
	const int PRIORITY_ZSORT = 10;		// Zソート専用の優先順位
	const int NUM_KEY_MAX = 256;		// キーの最大数
	const int MAX_STRING = 512;			// 最大文字数
	const int MAX_PARTS = 32;			// パーツ最大数
	const int MAX_OBJ = 128;			// パーツ最大数
	const float GRAVITY = 0.7f;			// 重力
	const int MAX_VTX = 51200;			// 頂点の最大数
	const float KILL_Y = -600.0f;		// 消滅する座標
	const int MAX_PLAYER = 4;			// プレイヤーの数
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// デフォルトの色
	const MyLib::Vector3 DEFAULT_SCALE = MyLib::Vector3(1.0f, 1.0f, 1.0f);	// スケールのデフォルト値
	const D3DXVECTOR2 DEFAULT_VECTOR2 = D3DXVECTOR2(0.0f, 0.0f);		// Vector2のデフォルト値
}

#endif
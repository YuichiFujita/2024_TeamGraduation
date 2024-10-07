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
	const int MAX_OBJ = 1600;			// オブジェクトの最大数
	const int PRIORITY_NUM = 12;			// 優先順位の最大数
	const int PRIORITY_DEFAULT = 3;		// 優先順位のデフォルト
	const int PRIORITY_DEF2D = 4;		// 優先順位の2Dデフォルト
	const int PRIORITY_ZSORT = 10;		// Zソート専用の優先順位
	const int NUM_KEY_MAX = 256;		// キーの最大数
	const int MAX_STRING = 512;			// 最大文字数
	const int MAX_PARTS = 32;			// パーツ最大数
	const int MAX_PATTEN_ENEMY = 64;	// 敵の最大パターン
	const int MAX_WAVE_ENEMY = 32;		// 敵の最大パターン
	const int MAX_BASE_ENEMY = 32;		// 敵の拠点最大数
	const int MAX_ENEMY = 256;			// 敵の最大数
	const int ENEMY_PRIORITY = 2;		// 敵の優先順位
	const int MAX_FIXEDMOVE = 32;		// 一定の動き最大数
	const int MAX_FIXEDMOVE_INFO = 16;	// 一定の動き情報最大数
	const int MAX_CAMERAAXIS = 62;		// カメラ軸の最大数
	const int MAX_BULLET = 256;			// 弾の最大数
	const int MAX_LIMITEREA = 16;		// エリア制限の最大数
	const int MAX_STAGE = 64;			// ステージの最大数
	const float GRAVITY = 0.7f;			// 重力
	const int MAX_VTX = 51200;			// 頂点の最大数
	const float KILL_Y = -600.0f;		// 消滅する座標
	const int DMG_BOUNCE = 15;			// 跳ね返しの威力
	const int DMG_SLASH = 2;			// 斬撃の威力
	const int BULLETAPPEARANCE_NUM = 2;	// 見た目の弾の数
	const int MAX_PLAYER = 4;			// プレイヤーの数
	const int SHAPE_LIMITEREA = 4;		// エリア制限の多角形
	const float MAX_ROCKONDISTANCE_GAME = 1600.0f;		// (ゲーム時)最大ロックオン距離
	const float MAX_ROCKONDISTANCE_BOSS = 4000.0f;		// (ボス戦時)最大ロックオン距離
	const float RADIUS_STAGE = 2300.0f;					// ステージの半径
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// デフォルトの色
	const D3DXCOLOR FIELDCOLOR_01 = D3DXCOLOR(0.6f, 0.4f, 0.1f, 1.0f);	// フィールドの色
	const D3DXCOLOR PLAYERBEAM_COLOR = D3DXCOLOR(0.5f, 0.5f, 0.9f, 1.0f);	// プレイヤーのビーム色
	const D3DXCOLOR UNIONBEAM_COLOR = D3DXCOLOR(0.2f, 0.2f, 0.9f, 1.0f);	// プレイヤーのビーム色
	const D3DXCOLOR ENEMYBEAM_COLOR = D3DXCOLOR(0.8f, 0.1f, 0.8f, 1.0f);	// 敵のビーム色
	const MyLib::Vector3 DEFAULT_SCALE = MyLib::Vector3(1.0f, 1.0f, 1.0f);	// スケールのデフォルト値
	const MyLib::Vector3 DEFAULT_VECTOR3 = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// Vector3のデフォルト値
	const D3DXVECTOR2 DEFAULT_VECTOR2 = D3DXVECTOR2(0.0f, 0.0f);		// Vector2のデフォルト値
}

#endif
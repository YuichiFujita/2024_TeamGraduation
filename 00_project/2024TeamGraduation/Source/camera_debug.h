//=============================================================================
//
// カメラデバッグ処理 [camera_debug.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERA_DEBUG_H_		// このマクロ定義がされていなかったら
#define _CAMERA_DEBUG_H_		// 二重インクルード防止のマクロを定義する

#include "camera.h"	// カメラ

//==========================================================================
// クラス定義
//==========================================================================
// カメラクラス定義
class CCamera_Debug
{
public:

	CCamera_Debug(CCamera* pCamera) : m_pCamera(pCamera), m_move() {}
	CCamera_Debug(CCamera* pCamera, const MyLib::Vector3& move) : m_pCamera(pCamera), m_move(move) {}
	~CCamera_Debug() {}

	void Update();	// 更新処理

private:


	//=============================
	// 関数リスト
	//=============================

	//=============================
	// メンバ関数
	//=============================
	void UpdateWASD();	// WASDの更新
	void UpdateDistance();	// 距離の更新

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_move;		// 移動量
	MyLib::Vector3 m_moveRot;	// 回転移動量
	CCamera* m_pCamera;		// カメラのポインタ
};

#endif
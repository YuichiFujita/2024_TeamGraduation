//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERA_H_		// このマクロ定義がされていなかったら
#define _CAMERA_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラクラス定義
class CCamera
{
public:

	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetRotation(const D3DXVECTOR3 rot);	// 向き設定
	D3DXVECTOR3 GetRotation(void) const;		// 向き取得
	void SetShake(int nTime, float fLength, float fLengthY);	// 振動設定
	void EnableChase(void);

private:

	// 列挙型定義
	typedef enum
	{
		CAMERASTATE_NONE = 0,	// 通常
		CAMERASTATE_SHAKE,		// 振動
	}STATE;

	void MoveCameraInput(void);
	void MoveCameraStick(void);
	void MoveCameraMouse(void);
	void MoveCameraVR(void);
	void MoveCameraV(void);
	void MoveCameraR(void);
	void MoveCameraDistance(void);
	void SetCameraV(void);
	void SetCameraR(void);
	void MoveCameraFollow(void);
	void Shake(void);
	void UpdateState(void);

	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	D3DVIEWPORT9 m_viewport;		// ビューポート
	D3DXVECTOR3 m_posV;				// 視点(カメラの位置)
	D3DXVECTOR3 m_posR;				// 注視点(見たい場所)
	D3DXVECTOR3 m_posVDest;			// 目標の視点
	D3DXVECTOR3 m_posRDest;			// 目標の注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_rotVDest;			// 目標の視点の向き
	float m_fDistance;				// 距離
	float m_nShakeLength;			// 揺れの長さ
	float m_nShakeLengthY;			// Yの揺れの長さ
	float m_fMoveShake;				// 揺れの移動量
	float m_fMoveShakeY;			// Yの揺れの移動量
	bool m_bFollow;					// 追従するかどうか
	int m_nCntState;				// 状態カウンター
	STATE m_state;					// 状態
};

#endif
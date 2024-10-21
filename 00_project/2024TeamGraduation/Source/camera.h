//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERA_H_		// このマクロ定義がされていなかったら
#define _CAMERA_H_		// 二重インクルード防止のマクロを定義する

#include "scene.h"
#include "camera_motion.h"	// カメラモーション

class CCameraControlState;	// 状態別操作

class CCamera_Debug;	// デバッグ処理

//==========================================================================
// クラス定義
//==========================================================================
// カメラクラス定義
class CCamera
{
private:

	// 列挙型定義
	enum STATE
	{
		CAMERASTATE_NONE = 0,	// 通常
		CAMERASTATE_SHAKE,		// 振動
		CAMERASTATE_PRAYER,		// 祈り
		CAMERASTATE_MAX
	};

public:

	// 注視点の状態
	enum PosRState
	{
		POSR_STATE_NORMAL = 0,
		POSR_STATE_MAX
	};

	CCamera();
	~CCamera();

	HRESULT Init();		// 初期化処理
	void Uninit();		// 終了処理
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新処理
	void SetCamera();	// カメラ設定
	void Reset(CScene::MODE mode);	// リセット





	
	//=============================
	// 位置情報
	//=============================
	//--------------------------
	// 注視点
	//--------------------------
	void SetPositionR(const MyLib::Vector3& pos) { m_posR = pos; }			// 注視点設定
	MyLib::Vector3 GetPositionR() { return m_posR; }						// 注視点取得
	void SetPositionRDest(const MyLib::Vector3& pos) { m_posRDest = pos; }	// 注視点の目標値設定
	MyLib::Vector3 GetPositionRDest() { return m_posRDest; }				// 注視点の目標値取得
	MyLib::Vector3 GetPositionROrigin() { return m_posROrigin; }			// 元の注視点

	//--------------------------
	// 視点
	//--------------------------
	void SetPositionV(const MyLib::Vector3& pos) { m_posV = pos; }			// 視点設定
	MyLib::Vector3 GetPositionV() { return m_posV; }						// 視点取得
	void SetPositionVDest(const MyLib::Vector3& pos) { m_posVDest = pos; }	// 視点の目標値設定
	MyLib::Vector3 GetPositionVDest() { return m_posVDest; }				// 視点の目標値取得

	//--------------------------
	// 追従目標値
	//--------------------------
	void SetTargetPosition(const MyLib::Vector3& pos) { m_posTarget = pos; }	// 追従目標の位置設定
	MyLib::Vector3 GetTargetPosition() { return m_posTarget; }					// 追従目標の位置取得

	//--------------------------
	// 距離
	//--------------------------
	void SetDistance(float distance) { m_fDistance = distance; }				// 距離設定
	float GetDistance() { return m_fDistance; }									// 距離取得
	void SetDistanceDest(float distance) { m_fDestDistance = distance; }		// 距離の目標値取得
	float GetDistanceDest() { return m_fDestDistance; }							// 距離の目標値取得
	void SetOriginDistance(float distance) { m_fOriginDistance = distance; }	// 元になるカメラの距離設定
	float GetOriginDistance() { return m_fOriginDistance; }						// 元になるカメラの距離取得

	//--------------------------
	// 向き
	//--------------------------
	void SetRotation(const MyLib::Vector3& rot) { m_rot = rot; }			// 向き設定
	MyLib::Vector3 GetRotation() const { return m_rot; }					// 向き取得
	void SetDestRotation(const MyLib::Vector3& rot) { m_rotDest = rot; }	// 目標の向き設定
	MyLib::Vector3 GetDestRotation() { return m_rotDest; }					// 目標の向き取得
	void SetOriginRotation(const MyLib::Vector3& rot) { m_rotOrigin = rot; }	// 目標の向き設定
	MyLib::Vector3 GetOriginRotation() { return m_rotOrigin; }					// 目標の向き取得

	//--------------------------
	// スクリーン
	//--------------------------
	bool IsOnScreen(const MyLib::Vector3& pos);				// スクリーン内の判定
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3& pos);	// スクリーン座標取得

	//--------------------------
	// カメラモーション
	//--------------------------
	CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }		// カメラモーションのポインタ取得

	//--------------------------
	// フラグ
	//--------------------------
	bool IsFollow() { return m_bFollow; }							// 追従判定取得
	void SetEnableFollow(bool bFollow) { m_bFollow = bFollow; }		// 追従の判定設定
	bool IsMotion() { return m_bMotion; }						// モーション中判定取得
	void SetEnableMotion(bool frag) { m_bMotion = frag; }		// モーション中判定設定
	
	//--------------------------
	// 視点マトリックス
	//--------------------------
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX* GetMtxViewPtr() { return &m_mtxView; }
	D3DXMATRIX GetMtxProjection() { return m_mtxProjection; }
	D3DXMATRIX* GetMtxProjectionPtr() { return &m_mtxProjection; }
	void SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size);		// ビューポートの設定
	D3DVIEWPORT9 GetViewPort() { return m_viewport; };			// ビューポート

	void SetShake(float fTime, float fLength, float fLengthY);	// 振動設定	// TODO：後で検証する
	void MoveCameraStick(int nIdx = 0);			// スティック操作
	void WarpCamera(const MyLib::Vector3& pos);	// カメラワープ

	// ステートパターン設定
	void SetControlState(CCameraControlState* state);	// 操作の状態設定

private:

	// メンバ変数
	void UpdateByMode();	// モード別更新処理
	void MoveCameraInput();	// 入力機器のカメラ移動
	void ReflectCameraR();		// 注視点の反映
	void ReflectCameraV();		// 視点反映
	void Shake();


	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void UpdateSpotLightVec();	// ライトのベクトル更新

	// リセット
	void ResetGame();

	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	D3DVIEWPORT9 m_viewport;		// ビューポート
	MyLib::Vector3 m_posV;			// 視点(カメラの位置)
	MyLib::Vector3 m_posVDest;		// 目標の視点
	MyLib::Vector3 m_posR;			// 注視点(見たい場所)
	MyLib::Vector3 m_posRDest;		// 目標の注視点
	MyLib::Vector3 m_posROrigin;	// 元の注視点
	MyLib::Vector3 m_vecU;			// 上方向ベクトル
	MyLib::Vector3 m_move;			// 移動量
	MyLib::Vector3 m_rot;			// 向き
	MyLib::Vector3 m_rotOrigin;		// 元の向き
	MyLib::Vector3 m_rotDest;		// 目標の向き
	MyLib::Vector3 m_posTarget;		// 追従目標の位置
	MyLib::Vector3 m_posTargetDest;	// 目標の追従目標の位置

	float m_fRockOnDistance;
	float m_fDistance;			// 距離
	float m_fDestDistance;		// 目標の距離
	float m_fOriginDistance;	// 元の距離
	float m_nShakeLength;		// 揺れの長さ
	float m_nShakeLengthY;		// Yの揺れの長さ
	float m_fMoveShake;			// 揺れの移動量
	float m_fMoveShakeY;		// Yの揺れの移動量
	
	bool m_bFollow;	// 追従判定
	bool m_bMotion;	// モーション中判定
	
	float m_fTimerState;			// 状態カウンター
	float m_fTimerShake;			// 振動カウンター
	STATE m_state;					// 状態
	CCameraMotion* m_pCameraMotion;	// カメラモーションのポインタ


	CCameraControlState* m_pControlState;	// 操作の状態ポインタ
	CCamera_Debug* m_pDebugControll;	// デバッグ処理
};


//=============================
// 状態パターン
//=============================
// 基底クラス
class CCameraControlState
{
public:
	CCameraControlState(CCamera* pCamera) : m_moveRot() {}

	virtual void Controll(CCamera* pCamera);	// 移動処理

private:

	// メンバ変数
	MyLib::Vector3 m_moveRot;	// 回転移動量
};

// 通常
class CCameraControlState_Normal : public CCameraControlState
{
public:
	CCameraControlState_Normal(CCamera* pCamera) : CCameraControlState(pCamera) {}
};

#endif
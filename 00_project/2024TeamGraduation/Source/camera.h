//==========================================================================
//
// カメラ処理 [camera.h]
// Author : 相馬靜雅
// Adder  : 藤田勇一
//
//==========================================================================

#ifndef _CAMERA_H_	// このマクロ定義がされていなかったら
#define _CAMERA_H_	// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "scene.h"
#include "lightDir.h"
#include "camera_motion.h"

//==========================================================================
// 前方宣言
//==========================================================================
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
		CAMERASTATE_FOLLOW,		// 追従
		CAMERASTATE_MAX
	};

public:

	// 注視点の状態
	enum PosRState
	{
		POSR_STATE_NORMAL = 0,
		POSR_STATE_MAX
	};

	CCamera();	// コンストラクタ
	~CCamera();	// デストラクタ

	HRESULT Init();		// 初期化
	void Uninit();		// 終了
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新
	void SetCamera();	// カメラ設定
	void Reset();		// カメラリセット
	void WarpCamera(const MyLib::Vector3& pos);	// カメラワープ設定

	//--------------------------
	// スクリーン
	//--------------------------
	bool IsOnScreen(const MyLib::Vector3& pos);				// スクリーン内判定
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3& pos);	// スクリーン座標取得

	//--------------------------
	// 注視点
	//--------------------------
	inline void SetPositionR(const MyLib::Vector3& pos)		{ m_posR = pos; }			// 注視点の設定
	inline MyLib::Vector3 GetPositionR()					{ return m_posR; }			// 注視点の取得
	inline void SetPositionRDest(const MyLib::Vector3& pos)	{ m_posRDest = pos; }		// 目標注視点の設定
	inline MyLib::Vector3 GetPositionRDest()				{ return m_posRDest; }		// 目標注視点の取得
	inline MyLib::Vector3 GetPositionROrigin()				{ return m_posROrigin; }	// 原点注視点の取得

	//--------------------------
	// 視点
	//--------------------------
	inline void SetPositionV(const MyLib::Vector3& pos)		{ m_posV = pos; }		// 視点の設定
	inline MyLib::Vector3 GetPositionV()					{ return m_posV; }		// 視点の取得
	inline void SetPositionVDest(const MyLib::Vector3& pos)	{ m_posVDest = pos; }	// 目標視点の設定
	inline MyLib::Vector3 GetPositionVDest()				{ return m_posVDest; }	// 目標視点の取得

	//--------------------------
	// 追従目標値
	//--------------------------
	inline void SetTargetPosition(const MyLib::Vector3& pos)		{ m_posTarget = pos; }		// 追従位置の設定
	inline MyLib::Vector3 GetTargetPosition()						{ return m_posTarget; }		// 追従位置の取得
	inline void SetTargetPositionDest(const MyLib::Vector3& pos)	{ m_posTargetDest = pos; }	// 追従目標位置の設定
	inline MyLib::Vector3 GetTargetPositionDest()					{ return m_posTargetDest; }	// 追従目標位置の取得

	//--------------------------
	// 距離
	//--------------------------
	inline void SetDistance(float distance)			{ m_fDistance = distance; }			// 距離の設定
	inline float GetDistance()						{ return m_fDistance; }				// 距離の取得
	inline void SetDistanceDest(float distance)		{ m_fDestDistance = distance; }		// 目標距離の取得
	inline float GetDistanceDest()					{ return m_fDestDistance; }			// 目標距離の取得
	inline void SetDistanceOrigin(float distance)	{ m_fOriginDistance = distance; }	// 原点距離の設定
	inline float GetDistanceOrigin()				{ return m_fOriginDistance; }		// 原点距離の取得

	//--------------------------
	// 向き
	//--------------------------
	inline void SetRotation(const MyLib::Vector3& rot)			{ m_rot = rot; }		// 向きの設定
	inline MyLib::Vector3 GetRotation() const					{ return m_rot; }		// 向きの取得
	inline void SetDestRotation(const MyLib::Vector3& rot)		{ m_rotDest = rot; }	// 目標向きの設定
	inline MyLib::Vector3 GetDestRotation()						{ return m_rotDest; }	// 目標向きの取得
	inline void SetOriginRotation(const MyLib::Vector3& rot)	{ m_rotOrigin = rot; }	// 原点向きの設定
	inline MyLib::Vector3 GetOriginRotation()					{ return m_rotOrigin; }	// 原点向きの取得

	//--------------------------
	// カメラモーション
	//--------------------------
	inline CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }	// カメラモーションポインタ取得

	//--------------------------
	// フラグ
	//--------------------------
	inline void SetEnableLight(bool bLight)		{ m_pLight->SetEnableLight(bLight); }	// ライトフラグの設定
	inline void SetEnableFollow(bool bFollow)	{ m_bFollow = bFollow; }	// 追従判定設定
	inline bool IsFollow()						{ return m_bFollow; }		// 追従判定取得
	inline void SetEnableMotion(bool frag)		{ m_bMotion = frag; }		// モーション中判定設定
	inline bool IsMotion()						{ return m_bMotion; }		// モーション中判定取得
	
	//--------------------------
	// 視点マトリックス
	//--------------------------
	void SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size);		// ビューポート設定
	inline D3DVIEWPORT9 GetViewPort()			{ return m_viewport; };			// ビューポート取得
	inline D3DXMATRIX GetMtxView()				{ return m_mtxView; }			// ビューマトリックス取得
	inline D3DXMATRIX* GetMtxViewPtr()			{ return &m_mtxView; }			// ビューマトリックスポインタ取得
	inline D3DXMATRIX GetMtxProjection()		{ return m_mtxProjection; }		// プロジェクションマトリックス取得
	inline D3DXMATRIX* GetMtxProjectionPtr()	{ return &m_mtxProjection; }	// プロジェクションマトリックスポインタ取得

private:

	// メンバ関数
	void ReflectCameraR();		// 注視点の反映
	void ReflectCameraV();		// 視点の反映
	void UpdateSpotLightVec();	// ライトのベクトル更新
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新

	// メンバ変数
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			// ビューマトリックス
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

	float m_fDistance;			// 距離
	float m_fDestDistance;		// 目標の距離
	float m_fOriginDistance;	// 元の距離
	float m_nShakeLength;		// 揺れの長さ
	float m_nShakeLengthY;		// Yの揺れの長さ
	float m_fMoveShake;			// 揺れの移動量
	float m_fMoveShakeY;		// Yの揺れの移動量
	
	bool m_bFollow;	// 追従判定
	bool m_bMotion;	// モーション中判定
	
	float m_fTimerState;	// 状態カウンター
	STATE m_state;			// 状態
	CLightDir* m_pLight;	// ディレクショナルライト
	CCameraMotion* m_pCameraMotion;		// カメラモーションのポインタ
	CCamera_Debug* m_pDebugControll;	// デバッグ処理
};

#endif
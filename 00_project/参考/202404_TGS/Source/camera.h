//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERA_H_		// このマクロ定義がされていなかったら
#define _CAMERA_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"
#include "camera_motion.h"	// カメラモーション

class CStateCameraR;	// 注視点の状態
class CStateCameraV;	// 視点の状態
class CCameraControlState;	// 状態別操作

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

	// 追従の種類
	enum CHASETYPE
	{
		CHASETYPE_NORMAL = 0,	// 通常
		CHASETYPE_MAP,			// マップに追従
		CHASETYPE_NONE,			// 追従なし
		CHASETYPE_MAX
	};

	// ロックオン時の向き
	enum RockOnDir
	{
		ROCKON_DIR_RIGHT = 0,
		ROCKON_DIR_LEFT,
		ROCKON_DIR_MAX
	};

	// 注視点の状態
	enum PosRState
	{
		POSR_STATE_NORMAL = 0,
		POSR_STATE_ROCKON,		// ロックオン状態
		POSR_STATE_PRAYER,		// 祈り状態
		POSR_STATE_MAX
	};

	enum RockOnState
	{
		ROCKON_NORMAL = 0,	// 通常
		ROCKON_COUNTER,	// カウンター
		ROCKON_ROCKON,	// ロックオン
		ROCKON_MAX
	};

	CCamera();
	~CCamera();

	HRESULT Init();
	void Uninit();
	void Update();
	void SetCamera();

	void SetRotation(const MyLib::Vector3 rot);	// 向き設定
	MyLib::Vector3 GetRotation() const;			// 向き取得
	void SetOriginRotation(const MyLib::Vector3 rot);	// 元の向き設定
	MyLib::Vector3 GetOriginRotation() const { return m_rotOrigin; }		// 元の向き取得
	MyLib::Vector3 GetPositionV() const;		// カメラの位置取得
	MyLib::Vector3 GetPositionR() const;		// カメラの注視点取得
	void SetOriginDistance(float fDistance);	// 元になるカメラの距離設定
	float GetOriginDistance();				// 元になるカメラの距離取得
	void SetDestRotation(const MyLib::Vector3& rot);	// 目標の向き設定
	MyLib::Vector3 GetDestRotation();				// 目標の向き取得
	void SetTargetPosition(const MyLib::Vector3 pos);	// 追従目標の位置設定
	MyLib::Vector3 GetTargetPosition();					// 追従目標の位置取得
	void SetTargetPositionDest(const MyLib::Vector3& pos) { m_TargetPosDest = pos; }	// 追従目標の目標位置設定
	MyLib::Vector3 GetTargetPositionDest() { return m_TargetPosDest; }					// 追従目標の目標位置取得
	void SetTargetRotation(const MyLib::Vector3 rot);	// ロックオンの向き設定
	void SetPositionV();

	// 移動量系
	MyLib::Vector3 GetMoveRot() { return m_Moverot; }	// 向きの移動量取得
	void SetMoveRot(const MyLib::Vector3& rot) { m_Moverot = rot; }	// 向きの移動量取得


	// ロックオン
	void SetRockOnPosition(const MyLib::Vector3 pos);	// ロックオンの位置設定
	void SetRockOnDistance(const float distance);	// ロックオンの下限距離設定
	MyLib::Vector3 GetRockOnPosition();			// 追従目標の位置取得
	void SetRockOn(const MyLib::Vector3 pos, bool bSet);	// ロックオン設定
	void SetRockDir(RockOnDir dir) { m_RockOnDir = dir; }	// ロックオン時のズレ向き設定

	// カメラモーション
	bool IsMotion() { return m_bMotion; }					// モーション中かどうか
	void SetEnableMotion(bool frag) { m_bMotion = frag; }	// モーション中かどうか
	CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }		// カメラモーションのポインタ


	// 関数リスト
	typedef void(CCamera::* ROCKON_STATE_FUNC)();
	static ROCKON_STATE_FUNC m_RockOnStateFunc[];
	void SetRockOnState(RockOnState state, float height);	// ロックオン状態設定
	CCamera::RockOnState GetRockOnState() { return m_stateRockOn; }	// ロックオン状態取得

	MyLib::Vector3 GetTargetRotation();			// 追従目標の向き取得
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX* GetMtxViewPtr() { return &m_mtxView; }
	D3DXMATRIX GetMtxProjection() { return m_mtxProjection; }
	D3DXMATRIX* GetMtxProjectionPtr() { return &m_mtxProjection; }
	D3DVIEWPORT9 GetViewPort() { return m_viewport; };			// ビューポート

	void SetShake(int nTime, float fLength, float fLengthY);	// 振動設定

	/**
	@brief	目標の長さ設定
	@param	fLength			[in]	目標の長さ
	@param	nCntTime		[in]	減算するまでの時間
	@param	DecrementValue	[in]	減少量
	@param	fCorrection		[in]	減少補正係数
	@return	void
	*/
	void SetLenDest(float fLength, int nCntTime = 120, float DecrementValue = 2.0f, float fCorrection = 0.1f);
	void SetPlayerChaseIndex(int nIdx);				// 追従するプレイヤーのインデックス番号設定
	int GetPlayerChaseIndex();					// 追従するプレイヤーのインデックス番号取得
	bool IsFollow();										// 追従状態取得
	bool IsRockOn() { return m_bRockON; }					// 追従状態取得
	bool IsRotationZ() { return m_bRotationZ; }
	bool IsRotationY() { return m_bRotationY; }
	void SetEnableFollow(bool bFollow);							// 追従の判定設定
	void SetEnableRotationY(bool bRotation) { m_bRotationY = bRotation; }	// Y回転のフラグ
	void SetViewPort(MyLib::Vector3 pos, D3DXVECTOR2 size);		// ビューポートの設定
	void Reset(CScene::MODE mode);				// リセット
	void ResetBoss();						// リセット
	bool IsOnScreen(const MyLib::Vector3 pos);	// スクリーン内の判定
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3 pos);	// スクリーン座標取得
	void MoveCameraStick(int nIdx = 0);			// スティック操作
	void WarpCamera(MyLib::Vector3 pos);		// カメラワープ
	void SetAutoDistanceDest(float fDistance) { m_fAutoDistance_Dest = fDistance; }
	void SetDistance(float fDistance) { m_fDistance = fDistance; }
	int GetCntDistance() { return m_nCntDistance; }
	int GetCntDistanceOrigin() { return m_nOriginCntDistance; }

	// 情報取得
	void SetPositionR(const MyLib::Vector3& pos) { m_posR = pos; }
	MyLib::Vector3 GetPositionR() { return m_posR; }
	void SetPositionRDest(const MyLib::Vector3& pos) { m_posRDest = pos; }
	MyLib::Vector3 GetPositionRDest() { return m_posRDest; }
	void SetPositionVDest(const MyLib::Vector3& pos) { m_posVDest = pos; }
	MyLib::Vector3 GetPositionVDest() { return m_posVDest; }
	float GetAutoDistanceDest() { return m_fAutoDistance_Dest; }
	float GetDistance() { return m_fDistance; }
	float GetDistanceDecrementValue() { return m_fDistanceDecrementValue; }
	float GetDistanceDest() { return m_fDestDistance; }
	int GetDistanceCnt() { return m_nCntDistance; }
	void SetAutoMovingPosR(const MyLib::Vector3& pos) { m_AutoMovingPosR = pos; }	// 自動カメラ移動時の注視点
	MyLib::Vector3 GetAutoMovingPosR() { return m_AutoMovingPosR; }					// 自動カメラ移動時の注視点取得
	CCameraMotion* GetMotion() { return m_pCameraMotion; }

	// ステートパターン設定
	void SetStateCamraR(CStateCameraR* state);	// 注視点の状態設定
	void SetStateCameraV(CStateCameraV* state);	// 視点の状態設定
	void SetStateCameraR(PosRState state) { m_StateCameraR = state; }
	PosRState GetStateCameraR() { return m_StateCameraR; }

	void SetControlState(CCameraControlState* state);	// 操作の状態設定
private:

	// メンバ変数
	void UpdateByMode();	// モード別更新処理
	void MoveCameraInput();
	void MoveCameraMouse();
	void MoveCameraVR();
	void MoveCameraV();
	void MoveCameraR();
	void MoveCameraDistance();
	void SetCameraV();
	void SetCameraVTitle();
	void SetCameraVGame();
	void SetCameraVResult();
	void SetCameraVRanking();
	void SetCameraR();
	void SetCameraRTitle();
	void SetCameraRGame();
	void SetCameraRResult();
	void SetCameraRRanking();
	void MoveCameraFollow();
	void Shake();
	void StatePrayer();	// 祈り
	void UpdateState();
	void UpdateSpotLightVec();
	void RockOnStateNormal();	// 通常状態のロックオン処理
	void RockOnStateCounter();	// カウンター状態のロックオン処理

	

	// リセット
	void ResetGame();
	void ResetTitle();
	void ResetResult();
	void ResetRanking();

	D3DXMATRIX	m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;				// ビューマトリックス
	D3DVIEWPORT9 m_viewport;			// ビューポート
	MyLib::Vector3 m_posV;				// 視点(カメラの位置)
	MyLib::Vector3 m_posR;				// 注視点(見たい場所)
	MyLib::Vector3 m_posVDest;			// 目標の視点
	MyLib::Vector3 m_posRDest;			// 目標の注視点
	MyLib::Vector3 m_vecU;				// 上方向ベクトル
	MyLib::Vector3 m_move;				// 移動量
	MyLib::Vector3 m_moveWASD;			// WASDの移動量
	MyLib::Vector3 m_rot;				// 向き
	MyLib::Vector3 m_rotOrigin;			// 元の向き
	MyLib::Vector3 m_rotDest;			// 目標の向き
	MyLib::Vector3 m_Moverot;			// 向きの移動量
	MyLib::Vector3 m_rotVDest;			// 目標の視点の向き
	MyLib::Vector3 m_TargetPos;			// 追従目標の位置
	MyLib::Vector3 m_TargetPosDest;		// 目標の追従目標の位置
	MyLib::Vector3 m_TargetRot;			// 追従目標の向き
	MyLib::Vector3 m_RockOnPos;			// ロックオン対象の位置
	MyLib::Vector3 m_AutoMovingPosR;	// 自動カメラ移動時の注視点
	float m_fRockOnDistance;
	float m_fDistance;					// 距離
	float m_fDestDistance;				// 目標の距離
	float m_fOriginDistance;			// 元の距離
	float m_nShakeLength;				// 揺れの長さ
	float m_nShakeLengthY;				// Yの揺れの長さ
	float m_fMoveShake;					// 揺れの移動量
	float m_fMoveShakeY;				// Yの揺れの移動量
	float m_fHeightMaxDest;				// カメラの最大高さの目標
	float m_fHeightMax;					// カメラの最大高さ
	float m_fDiffHeight;				// 高さの差分
	float m_fDiffHeightSave;			// 高さの差分保存用
	float m_fDiffHeightDest;			// 目標の高さの差分
	float m_fChaseLerpStart;			// 追従補正の初期値
	float m_fChaseDistance;				// 追従の間隔
	float m_fDestChaseDistance;			// 目標の追従の間隔
	bool m_bFollow;						// 追従するかどうか
	bool m_bMotion;						// モーション中かどうか
	bool m_bRotationZ;					// Z回転出来るかどうか
	bool m_bRotationY;					// Y回転出来るかどうか
	bool m_bRockON;						// ロックオン中か
	int m_nCntState;					// 状態カウンター
	int m_nCntDistance;					// 距離カウンター
	int m_nOriginCntDistance;			// 元の距離カウンター
	float m_fDistanceCorrection;		// 距離の慣性補正係数
	float m_fDistanceDecrementValue;	// 距離の減少係数
	STATE m_state;						// 状態
	CHASETYPE m_ChaseType;				// 追従の種類
	CHASETYPE m_OldChaseType;			// 前回の追従の種類
	int m_nCntChaseType;				// 追従のカウンター
	int m_nChasePlayerIndex;			// 追従するプレイヤーのインデックス番号
	RockOnDir m_RockOnDir;				// ロックオン時の向き
	RockOnState m_stateRockOn;			// ロックオン時の状態
	CCameraMotion* m_pCameraMotion;		// カメラモーションのポインタ


	float m_fAutoDistance_Dest;					// 距離
	float m_fAutoRot_Dest;					// 距離



	PosRState m_StateCameraR;		// 注視点の状態
	CStateCameraR* m_pStateCameraR;	// 注視点の状態ポインタ
	CStateCameraV* m_pStateCameraV;	// 視点の状態ポインタ
	CCameraControlState* m_pControlState;	// 操作の状態ポインタ
};

//=============================
// 注視点の状態クラス
//=============================
class CStateCameraR
{
public:
	CStateCameraR() {}

	virtual void SetCameraR(CCamera* pCamera);
};

class CStateCameraR_RockOn : public CStateCameraR
{
public:
	CStateCameraR_RockOn() {}

	virtual void SetCameraR(CCamera* pCamera) override;
};

class CStateCameraR_Prayer : public CStateCameraR
{
public:
	CStateCameraR_Prayer() {}

	virtual void SetCameraR(CCamera* pCamera) override;
};

//=============================
// 視点の状態クラス
//=============================
class CStateCameraV
{
public:
	CStateCameraV() {}

	virtual void LimitPos(CCamera* pCamera);	// 位置制限
	virtual void Distance(CCamera* pCamera);	// 距離調整
	virtual void SetMotion(CCamera* pCamera){}	// モーション設定

protected:
	float GetDistance(CCamera* pCamera, const float fMultiply);
};

class CStateCameraV_Main : public CStateCameraV
{
public:
	CStateCameraV_Main() {}
};

class CStateCameraV_Enhance : public CStateCameraV
{
public:
	CStateCameraV_Enhance() {}

	virtual void LimitPos(CCamera* pCamera) override;	// 位置制限
};

class CStateCameraV_Goal : public CStateCameraV
{
public:
	CStateCameraV_Goal() {}

	virtual void LimitPos(CCamera* pCamera) override;	// 位置制限
	virtual void Distance(CCamera* pCamera) override;	// 距離調整
	virtual void SetMotion(CCamera* pCamera) override;	// モーション設定
};

class CStateCameraV_Distance : public CStateCameraV
{
public:
	CStateCameraV_Distance() {
		m_fMultiPly = 0.0f;
		m_fStartDistance = 0.0f;
	}

	virtual void Distance(CCamera* pCamera) override;	// 距離調整
	void SetStartDistance(float fStartDistance) { m_fStartDistance = fStartDistance; }

	float m_fMultiPly;
	float m_fStartDistance;
};

//=============================
// 状態パターン
//=============================
// 基底クラス
class CCameraControlState
{
public:
	CCameraControlState(CCamera* pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }

	virtual void MoveCamera(CCamera* pCamera);	// 移動処理
};

// 通常
class CCameraControlState_Normal : public CCameraControlState
{
public:
	CCameraControlState_Normal(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }
};

// ロックオン
class CCameraControlState_RockOn : public CCameraControlState
{
public:
	CCameraControlState_RockOn(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_ROCKON); }

	virtual void MoveCamera(CCamera* pCamera) override;	// 移動処理
};

// 祈り準備
class CCameraControlState_BeforePrayer : public CCameraControlState
{
public:
	CCameraControlState_BeforePrayer(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }

	virtual void MoveCamera(CCamera* pCamera) override;	// 移動処理
};

// 祈り後
class CCameraControlState_AfterPrayer : public CCameraControlState_BeforePrayer
{
public:
	CCameraControlState_AfterPrayer(CCamera* pCamera) : CCameraControlState_BeforePrayer(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }
};

// 祈り
class CCameraControlState_Prayer : public CCameraControlState
{
public:
	CCameraControlState_Prayer(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_PRAYER); }

	virtual void MoveCamera(CCamera* pCamera) override;	// 移動処理
};

// ボス演出
class CCameraControlState_BossAppearance : public CCameraControlState
{
public:
	CCameraControlState_BossAppearance(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }

};

#endif
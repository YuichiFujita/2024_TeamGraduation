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
public:

	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,	// 通常
		STATE_FOLLOW,	// 追従
		STATE_OUTFIELD,	// 外野
		STATE_MAX
	};

	// カメラ揺れ構造体
	struct SSwing
	{
	public:
		// コンストラクタ
		SSwing() :
			shiftPos	 (VEC3_ZERO),	// 位置ずれ量
			fShiftAngle	 (0.0f),		// 位置をずらす角度
			fShiftLength (0.0f),		// 位置をずらす距離
			fSubAngle	 (0.0f),		// ずらす角度の減算量
			fSubLength	 (0.0f)			// ずらす距離の減算量
		{}

		// 引数付きコンストラクタ
		SSwing(const float in_fShiftLength, const float in_fSubAngle, const float in_fSubLength) :
			shiftPos	 (VEC3_ZERO),		// 位置ずれ量
			fShiftAngle	 (0.0f),			// 位置をずらす角度
			fShiftLength (in_fShiftLength),	// 位置をずらす距離
			fSubAngle	 (in_fSubAngle),	// ずらす角度の減算量
			fSubLength	 (in_fSubLength)	// ずらす距離の減算量
		{}

		// デストラクタ
		~SSwing() {}

		// メンバ変数
		MyLib::Vector3 shiftPos;	// 位置ずれ量
		float fShiftAngle;	// 位置をずらす角度
		float fShiftLength;	// 位置をずらす距離
		float fSubAngle;	// ずらす角度の減算量
		float fSubLength;	// ずらす距離の減算量
	};

	// 左右座標構造体
	struct SSide
	{
	public:
		// デフォルトコンストラクタ
		SSide() :
			l (0.0f),	// 左
			r (0.0f)	// 右
		{}

		// 引数付きコンストラクタ
		SSide(const float in_fLR) :
			l (in_fLR),	// 左
			r (in_fLR)	// 右
		{}
		SSide(const float in_fL, const float in_fR) :
			l (in_fL),	// 左
			r (in_fR)	// 右
		{}

		// デストラクタ
		~SSide() {}

		// メンバ変数
		float l;	// 左
		float r;	// 右
	};

	// カメラポイント情報
	struct SCameraPoint
	{
		// デフォルトコンストラクタ
		SCameraPoint() :
			posR		(VEC3_ZERO),	// 注視点
			posV		(VEC3_ZERO),	// 視点
			rot			(VEC3_ZERO),	// 向き
			fDistance	(0.0f)			// 距離
		{}

		// デストラクタ
		~SCameraPoint() {}

		// メンバ変数
		MyLib::Vector3 posR;	// 注視点
		MyLib::Vector3 posV;	// 視点
		MyLib::Vector3 rot;		// 向き
		float fDistance;		// 距離
	};

	//-----------------------------
	// コンストラクタ/デストラクタ
	//-----------------------------
	CCamera();
	virtual ~CCamera();

	//-----------------------------
	// 仮想関数
	//-----------------------------
	virtual HRESULT Init();	// 初期化
	virtual void Uninit();	// 終了
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新
	virtual void Reset();	// カメラリセット

	//-----------------------------
	// メンバ関数
	//-----------------------------
	void SetCamera();							// カメラ設定 (通常)
	void SetCameraDressup();					// カメラ設定 (着せ替え)
	void ResetByMode(CScene::MODE mode);		// モード別リセット
	void SetSwing(const SSwing& swing);			// カメラ揺れ設定
	void ResetSwing();							// カメラ揺れリセット
	void SetWarp(const MyLib::Vector3& pos);	// カメラワープ設定
	void SetState(const STATE state, const bool bReset = true);	// 状態設定
	inline STATE GetState() const { return m_state; }			// 状態取得

	//-----------------------------
	// スクリーン
	//-----------------------------
	bool IsOnScreen(const MyLib::Vector3& pos);				// スクリーン内判定
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3& pos);	// スクリーン座標取得

	//-----------------------------
	// 注視点
	//-----------------------------
	inline void SetPositionR(const MyLib::Vector3& pos)			{ m_posR = pos; }			// 注視点の設定
	inline MyLib::Vector3 GetPositionR()						{ return m_posR; }			// 注視点の取得
	inline void SetPositionRDest(const MyLib::Vector3& pos)		{ m_posRDest = pos; }		// 目標注視点の設定
	inline MyLib::Vector3 GetPositionRDest()					{ return m_posRDest; }		// 目標注視点の取得
	inline void SetPositionROrigin(const MyLib::Vector3& pos)	{ m_posROrigin = pos; }		// 原点注視点の設定
	inline MyLib::Vector3 GetPositionROrigin()					{ return m_posROrigin; }	// 原点注視点の取得

	//-----------------------------
	// 視点
	//-----------------------------
	inline void SetPositionV(const MyLib::Vector3& pos)		{ m_posV = pos; }		// 視点の設定
	inline MyLib::Vector3 GetPositionV()					{ return m_posV; }		// 視点の取得
	inline void SetPositionVDest(const MyLib::Vector3& pos)	{ m_posVDest = pos; }	// 目標視点の設定
	inline MyLib::Vector3 GetPositionVDest()				{ return m_posVDest; }	// 目標視点の取得

	//-----------------------------
	// 距離
	//-----------------------------
	inline void SetDistance(float distance)			{ m_fDistance = distance; }			// 距離の設定
	inline float GetDistance()						{ return m_fDistance; }				// 距離の取得
	inline void SetDistanceDest(float distance)		{ m_fDestDistance = distance; }		// 目標距離の取得
	inline float GetDistanceDest()					{ return m_fDestDistance; }			// 目標距離の取得
	inline void SetDistanceOrigin(float distance)	{ m_fOriginDistance = distance; }	// 原点距離の設定
	inline float GetDistanceOrigin()				{ return m_fOriginDistance; }		// 原点距離の取得

	//-----------------------------
	// 向き
	//-----------------------------
	inline void SetRotation(const MyLib::Vector3& rot)			{ m_rot = rot; }		// 向きの設定
	inline MyLib::Vector3 GetRotation() const					{ return m_rot; }		// 向きの取得
	inline void SetDestRotation(const MyLib::Vector3& rot)		{ m_rotDest = rot; }	// 目標向きの設定
	inline MyLib::Vector3 GetDestRotation()						{ return m_rotDest; }	// 目標向きの取得
	inline void SetOriginRotation(const MyLib::Vector3& rot)	{ m_rotOrigin = rot; }	// 原点向きの設定
	inline MyLib::Vector3 GetOriginRotation()					{ return m_rotOrigin; }	// 原点向きの取得

	//-----------------------------
	// 視野角
	//-----------------------------
	inline void SetViewAngle(const float fAngle)		{ m_fViewAngle = fAngle; }		// 視野角
	inline float GetViewAngle()							{ return m_fViewAngle; }		// 視野角
	inline void SetDestViewAngle(const float fAngle)	{ m_fDestViewAngle = fAngle; }	// 目標視野角
	inline float GetDestViewAngle()						{ return m_fDestViewAngle; }	// 目標視野角

	//-----------------------------
	// カメラモーション
	//-----------------------------
	inline CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }	// カメラモーションポインタ取得

	//-----------------------------
	// フラグ
	//-----------------------------
	inline void SetEnableLight(bool bLight)	{ m_pLight->SetEnableLight(bLight); }	// ライトフラグの設定
	inline void SetEnableMotion(bool frag)	{ m_bMotion = frag; }	// モーション中判定設定
	inline bool IsMotion()					{ return m_bMotion; }	// モーション中判定取得
	
	//-----------------------------
	// 視点マトリックス
	//-----------------------------
	void SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size);		// ビューポート設定
	inline D3DVIEWPORT9 GetViewPort()			{ return m_viewport; };			// ビューポート取得
	inline D3DXMATRIX GetMtxView()				{ return m_mtxView; }			// ビューマトリックス取得
	inline D3DXMATRIX* GetMtxViewPtr()			{ return &m_mtxView; }			// ビューマトリックスポインタ取得
	inline D3DXMATRIX GetMtxProjection()		{ return m_mtxProjection; }		// プロジェクションマトリックス取得
	inline D3DXMATRIX* GetMtxProjectionPtr()	{ return &m_mtxProjection; }	// プロジェクションマトリックスポインタ取得

	//-----------------------------
	// ゲームカメラ
	//-----------------------------
	SCameraPoint CameraPoint(const STATE state = STATE::STATE_NONE);	// 現在のカメラポイント取得
	SCameraPoint FollowPoint();		// 現在の追従ポイント取得
	SCameraPoint OutFieldPoint();	// 現在の外野ポイント取得

protected:
	//-----------------------------
	// メンバ関数
	//-----------------------------
	void UpdateSwing();	// カメラ揺れの更新

private:

	//-----------------------------
	// 関数リスト
	//-----------------------------
	// 状態関数
	typedef void(CCamera::*STATE_FUNC)(const float, const float, const float);	// 関数型
	static STATE_FUNC m_StateFuncList[];	// 関数リスト

	// 状態リセット関数
	typedef void(CCamera::*RESET_FUNC)();	// 関数型
	static RESET_FUNC m_ResetFuncList[];	// 関数リスト

	//-----------------------------
	// メンバ関数
	//-----------------------------
	// 状態関数
	void UpdateNoneState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 通常状態の更新
	void UpdateFollowState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 追従状態の更新
	void UpdateOutFieldState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 外野状態の更新

	// リセット関数
	void ResetNoneState();		// 通常状態リセット
	void ResetFollowState();	// 追従状態リセット
	void ResetOutFieldState();	// 外野状態リセット

	// ゲームカメラ関数
	void UpdateFollow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 追従カメラの更新
	void UpdateOutField(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 外野カメラの更新
	bool UpdateTrans(const STATE state);	// カメラ状態遷移の更新
	bool IsPlayerOutWithBall();	// 外野がボールを持っているか
	SSide GetPlayerMaxSide();	// プレイヤー最大左右座標取得
	float CalcDistanceRate(const SSide& rSide);		// 左右間の距離割合計算
	float CalcFollowDistance(const float fDisRate);	// 追従カメラの距離計算
	MyLib::Vector3 CalcFollowPositionR(const SSide& rSide, const float fDisRate);	// 追従カメラの注視点計算
	void RevFollowPositionR(float* pTargetX, const float fDisRate);					// X注視点の範囲補正

	// 汎用関数
	MyLib::Vector3 CalcSpherePosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot, const float fDis);	// 球面座標変換による相対位置取得
	void UpdateSpotLightVec(const MyLib::Vector3& rPosR, const MyLib::Vector3& rPosV);	// ライトベクトルの更新

	//-----------------------------
	// メンバ変数
	//-----------------------------
	CCamera_Debug* m_pDebugControll;	// デバッグ情報
	CCameraMotion* m_pCameraMotion;		// カメラモーション情報
	CLightDir* m_pLight;				// ディレクショナルライト情報
	D3DXMATRIX m_mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;				// ビューマトリックス
	D3DVIEWPORT9 m_viewport;			// ビューポート情報
	SSwing m_swing;						// カメラ揺れ情報
	MyLib::Vector3 m_posV;				// 視点
	MyLib::Vector3 m_posVDest;			// 目標視点
	MyLib::Vector3 m_posR;				// 注視点
	MyLib::Vector3 m_posRDest;			// 目標注視点
	MyLib::Vector3 m_posROrigin;		// 原点注視点
	MyLib::Vector3 m_vecU;				// 上方向ベクトル
	MyLib::Vector3 m_rot;				// 向き
	MyLib::Vector3 m_rotDest;			// 目標向き
	MyLib::Vector3 m_rotOrigin;			// 原点向き
	float m_fDistance;					// 距離
	float m_fDestDistance;				// 目標距離
	float m_fOriginDistance;			// 原点距離
	float m_fViewAngle;					// 視野角
	float m_fDestViewAngle;				// 目標視野角
	SSide m_side;						// 左右最大位置
	SSide m_sideDest;					// 目標左右最大位置
	STATE m_state;						// 状態
	bool m_bMotion;						// モーション中判定
	bool m_bOldWithBall;				// 前回の外野ボール所持フラグ
	float m_fTransTime;					// ゲームカメラ状態遷移の時間
	float m_fEndTime;					// ゲームカメラ状態遷移の終了時間
	bool b;
	SCameraPoint m_transStartPoint;		// ゲームカメラ状態遷移の開始ポイント
};

#endif

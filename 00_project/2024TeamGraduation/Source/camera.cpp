//=============================================================================
// 
// カメラ処理 [camera.cpp]
// Author : 相馬靜雅
// Adder  : 藤田勇一
// 
//=============================================================================
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "calculation.h"
#include "player.h"
#include "title.h"
#include "instantfade.h"
#include "3D_effect.h"
#include "calculation.h"
#include "pause.h"
#include "objectX.h"
#include "camera_debug.h"

//==========================================================================
// 追従デバッグ表示のON/OFF
//==========================================================================
#if 0
#define FOLLOW_DEBUG
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector3 INIT_VECU = MyLib::Vector3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル初期値
	const CCamera::SSide INIT_SIDE = CCamera::SSide(0.0f, 0.0f);		// 左右最大位置初期値
	const D3DXCOLOR COL_LIGHT = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// ライト拡散光
	const float MIN_NEAR	= 10.0f;		// 手前の描画制限
	const float MAX_FAR		= 1500000.0f;	// 奥の描画制限
	const float MIN_DIS		= 50.0f;		// 最少距離
	const float REV_SWING	= 0.001f;		// カメラ揺れ時のカメラ距離補正係数

	namespace dressup
	{
		const MyLib::Vector3 POSV	= MyLib::Vector3(0.0f, 70.0f, -284.0f);	// 視点
		const MyLib::Vector3 POSR	= MyLib::Vector3(0.0f, 70.0f, 0.0f);	// 注視点
		const MyLib::Vector3 VECU	= MyLib::Vector3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
		const float VIEW_ANGLE		= D3DXToRadian(45.0f);					// 視野角
	}

	namespace none
	{
		const MyLib::Vector3 INIT_POSR	= VEC3_ZERO;		// 注視点の初期値
		const MyLib::Vector3 INIT_ROT	= VEC3_ZERO;		// 向きの初期値
		const float INIT_DISTANCE	= 800.0f;				// 距離の初期値
		const float INIT_VIEWANGLE	= D3DXToRadian(45.0f);	// 視野角の初期値
	}

	namespace game
	{
		const float END_TIME = 1.4f;	// ゲームカメラ状態遷移の時間
		const float MIN_TIME = 0.5f;	// ゲームカメラ状態遷移の最小時間
	}

	namespace follow
	{
		const MyLib::Vector3 INIT_ROT	= MyLib::Vector3(0.0f, 0.0f, -0.32f);	// 向きの初期値
		const float INIT_VIEWANGLE		= D3DXToRadian(30.0f);	// 視野角の初期値
		const float DEST_POSR_BOXSIZE	= 50.0f;	// 目標注視点を動かさない範囲

		const float REV_SIDE		= 0.35f;		// 最大左右座標の補正係数
		const float REV_POSR		= 0.05f;		// 注視点の補正係数
		const float REV_VIEWANGLE	= 0.1f;			// 視野角の補正係数
		const float MAX_DIS	= 2500.0f - MIN_DIS;	// 追従カメラの最大距離

		const float POSRY_START	= 550.0f;	// カメラY注視点の開始座標
		const float POSRY_END	= 320.0f;	// カメラY注視点の終了座標
		const float POSRZ_START	= -1600.0f;	// カメラZ注視点の開始座標
		const float POSRZ_END	= -100.0f;	// カメラZ注視点の終了座標
	}

	namespace out
	{
		const MyLib::Vector3 ROT = follow::INIT_ROT;	// カメラの向き
		const float POSRY = follow::POSRY_END;			// カメラY注視点の座標
		const float POSRZ = follow::POSRZ_END - 160.0f;	// カメラZ注視点の座標
		const float DIS = follow::MAX_DIS + 160.0f;		// カメラの距離
	}

	namespace Reset
	{
		const MyLib::Vector3 POSITION[CScene::MODE::MODE_MAX] = // 位置
		{
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// NONE
			MyLib::Vector3(0.0f, 50.0f, 0.0f),	// タイトル
			MyLib::Vector3(0.0f, 173.33f, -770.77f),	// エントリー
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// チュートリアル
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// ゲーム
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// リザルト
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// ランキング
		};

		const MyLib::Vector3 ROTATION[CScene::MODE::MODE_MAX] = // 向き
		{
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// NONE
			MyLib::Vector3(0.0f, 0.0f, -0.15f),	// タイトル
			MyLib::Vector3(0.0f, 0.0f, -0.04f),	// エントリー
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// チュートリアル
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// ゲーム
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// リザルト
			MyLib::Vector3(0.0f, 0.0f, 0.0f),	// ランキング
		};

		const float DISTANCE[CScene::MODE::MODE_MAX] = // 向き
		{
			0.0f,	// NONE
			800.0f,	// タイトル
			800.0f,	// エントリー
			800.0f,	// チュートリアル
			800.0f,	// ゲーム
			800.0f,	// リザルト
			800.0f,	// ランキング
		};
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CCamera::STATE_FUNC CCamera::m_StateFuncList[] =
{
	&CCamera::UpdateNoneState,		// 通常カメラ更新
	&CCamera::UpdateFollowState,	// 追従カメラ更新
	&CCamera::UpdateOutFieldState,	// 外野カメラ更新
};

// リセット関数
CCamera::RESET_FUNC CCamera::m_ResetFuncList[] =
{
	&CCamera::ResetNoneState,		// 通常カメラリセット
	&CCamera::ResetFollowState,		// 追従カメラリセット
	&CCamera::ResetOutFieldState,	// 外野カメラリセット
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCamera::CCamera()
{
	// 基本情報のクリア
	m_pDebugControll	= nullptr;			// デバッグ情報
	m_pCameraMotion		= nullptr;			// カメラモーション情報
	m_pLight			= nullptr;			// ディレクショナルライト情報
	m_posV				= VEC3_ZERO;		// 視点
	m_posVDest			= VEC3_ZERO;		// 目標視点
	m_posR				= VEC3_ZERO;		// 注視点
	m_posRDest			= VEC3_ZERO;		// 目標注視点
	m_posROrigin		= VEC3_ZERO;		// 原点注視点
	m_vecU				= INIT_VECU;		// 上方向ベクトル
	m_rot				= VEC3_ZERO;		// 向き
	m_rotDest			= VEC3_ZERO;		// 目標向き
	m_rotOrigin			= VEC3_ZERO;		// 原点向き
	m_fDistance			= 0.0f;				// 距離
	m_fDestDistance		= 0.0f;				// 目標距離
	m_fOriginDistance	= 0.0f;				// 原点距離
	m_fViewAngle		= 0.0f;				// 視野角
	m_fDestViewAngle	= 0.0f;				// 目標視野角
	m_side				= INIT_SIDE;		// 左右最大位置
	m_sideDest			= INIT_SIDE;		// 目標左右最大位置
	m_state				= STATE_NONE;		// 状態
	m_bMotion			= false;			// モーション中判定
	m_bOldWithBall		= false;			// 前回の外野ボール所持フラグ
	m_fTransTime		= 0.0f;				// ゲームカメラ状態遷移の時間
	m_fEndTime			= 0.0f;				// ゲームカメラ状態遷移の終了時間
	m_transStartPoint	= SCameraPoint();	// ゲームカメラ状態遷移の開始ポイント
	b = false;

	// マトリックス情報のクリア
	D3DXMatrixIdentity(&m_mtxProjection);	// プロジェクションマトリックス
	D3DXMatrixIdentity(&m_mtxView);			// ビューマトリックス

	// ビューポート情報のクリア
	m_viewport.X		= 0;	// 画面の左上X座標
	m_viewport.Y		= 0;	// 画面の左上Y座標
	m_viewport.Width	= 0;	// 画面の幅
	m_viewport.Height	= 0;	// 画面の高さ
	m_viewport.MinZ		= 0.0f;
	m_viewport.MaxZ		= 0.0f;

	// カメラ揺れ情報のクリア
	m_swing.shiftPos	 = VEC3_ZERO;	// 位置ずれ量
	m_swing.fShiftAngle	 = 0.0f;		// 位置をずらす角度
	m_swing.fShiftLength = 0.0f;		// 位置をずらす距離
	m_swing.fSubAngle	 = 0.0f;		// ずらす角度の減算量
	m_swing.fSubLength	 = 0.0f;		// ずらす距離の減算量

	// スタティックアサート
	static_assert(NUM_ARRAY(m_StateFuncList) == STATE::STATE_MAX, "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_ResetFuncList) == STATE::STATE_MAX, "ERROR : State Count Mismatch");
}

//==========================================================================
// デストラクタ
//==========================================================================
CCamera::~CCamera()
{

}

//==========================================================================
// カメラの初期化処理
//==========================================================================
HRESULT CCamera::Init()
{
	// ビューポートの設定
	SetViewPort(MyLib::Vector3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// カメラのリセット
	Reset();

	// ディレクショナルライトの生成
	m_pLight = CLightDir::Create();
	if (m_pLight == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// オブジェクト種類を未設定にする
	m_pLight->SetType(CObject::TYPE::TYPE_NONE);

	// 拡散光の色を設定
	m_pLight->SetDiffuse(COL_LIGHT);

	// カメラモーションの生成
	m_pCameraMotion = CCameraMotion::Create();
	if (m_pCameraMotion == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

#if _DEBUG
	// デバッグ専用の生成
	m_pDebugControll = DEBUG_NEW CCamera_Debug(this);
	if (m_pDebugControll == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}
#endif

	return S_OK;
}

//==========================================================================
// ビューポートの設定
//==========================================================================
void CCamera::SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size)
{
	// 引数情報の設定
	m_viewport.X = (DWORD)pos.x;		// 画面の左上X座標
	m_viewport.Y = (DWORD)pos.y;		// 画面の左上Y座標
	m_viewport.Width  = (DWORD)size.x;	// 画面の幅
	m_viewport.Height = (DWORD)size.y;	// 画面の高さ

	// 情報の初期化
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

//==========================================================================
// カメラの終了処理
//==========================================================================
void CCamera::Uninit()
{
	// ライトの終了
	SAFE_UNINIT(m_pLight);

	// カメラモーションの終了
	SAFE_UNINIT(m_pCameraMotion);

	// デバッグ専用の終了
	SAFE_DELETE(m_pDebugControll);
}

//==========================================================================
// カメラの更新処理
//==========================================================================
void CCamera::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pDebugControll != nullptr)
	{
		// デバッグ専用の更新
		m_pDebugControll->Update();
	}

	// ポーズ中の場合抜ける
	CManager* pManager = GET_MANAGER;	// マネージャー
	if (pManager->GetPause()->IsPause()) { return; }

	if (m_StateFuncList[m_state] != nullptr && !m_bMotion)
	{ // 状態更新関数がある且つモーション中ではない場合

		// 状態別処理
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// カメラ揺れの更新
	UpdateSwing();

	if (m_pLight != nullptr)
	{
		// ライトの更新
		m_pLight->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	if (m_pCameraMotion != nullptr)
	{
		// カメラモーションの更新
		m_pCameraMotion->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

#ifdef _DEBUG
	// カメラ交差点の取得
	MyLib::Vector3 posScreen = UtilFunc::Transformation::CalcScreenToXZ
	(
		CInputMouse::GetInstance()->GetPosition(),	// マウス座標
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),	// スクリーンサイズ
		m_mtxView,		// ビューマトリックス
		m_mtxProjection	// プロジェクションマトリックス
	);

	// カメラ情報のテキスト描画
	GET_MANAGER->GetDebugProc()->Print
	(
		"\n---------------- カメラ情報 ----------------\n"
		"【向き】[X：%f Y：%f Z：%f]\n"
		"【距離】[%f]\n"
		"【視点】[X：%f Y：%f Z：%f]\n"
		"【注視点】[X：%f Y：%f Z：%f]\n"
		"【交差点】[X：%f Y：%f Z：%f]\n",
		m_rot.x, m_rot.y, m_rot.z,
		m_fDistance,
		m_posV.x, m_posV.y, m_posV.z,
		m_posR.x, m_posR.y, m_posR.z,
		posScreen.x, posScreen.y, posScreen.z
	);
#endif
}

//==========================================================================
// カメラの設定処理 (通常)
//==========================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

	// ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	float fAspect = (float)m_viewport.Width / (float)m_viewport.Height;	// アスペクト比
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,	// プロジェクションマトリックス
		m_fViewAngle,		// 視野角
		fAspect,			// アスペクト比
		MIN_NEAR,			// 手前の描画制限
		MAX_FAR				// 奥の描画制限
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	(
		&m_mtxView,	// ビューマトリックス
		&m_posV,	// 視点
		&m_posR,	// 注視点
		&m_vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// スポットライトベクトルの更新
	UpdateSpotLightVec(m_posR, m_posV);
}

//==========================================================================
// カメラの設定処理 (着せ替え)
//==========================================================================
void CCamera::SetCameraDressup()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

	// ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	float fAspect = (float)m_viewport.Width / (float)m_viewport.Height;	// アスペクト比
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,		// プロジェクションマトリックス
		dressup::VIEW_ANGLE,	// 視野角
		fAspect,	// アスペクト比
		MIN_NEAR,	// 手前の描画制限
		MAX_FAR		// 奥の描画制限
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	(
		&m_mtxView,		// ビューマトリックス
		&dressup::POSV,	// 視点
		&dressup::POSR,	// 注視点
		&dressup::VECU	// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// スポットライトベクトルの更新
	UpdateSpotLightVec(dressup::POSR, dressup::POSV);
}

//==========================================================================
// カメラリセット
//==========================================================================
void CCamera::Reset()
{
	if (m_StateFuncList[m_state] != nullptr)
	{ // 状態リセット関数がある場合

		// 状態別処理
		(this->*(m_ResetFuncList[m_state]))();
	}

	// カメラ揺れのリセット
	ResetSwing();

	if (m_pCameraMotion != nullptr)
	{
		// カメラモーションの位置リセット
		m_pCameraMotion->SetPosition(0.0f);
	}

	// プロジェクションマトリックスの初期化
	float fAspect = (float)m_viewport.Width / (float)m_viewport.Height;	// アスペクト比
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,	// プロジェクションマトリックス
		m_fViewAngle,		// 視野角
		fAspect,			// アスペクト比
		MIN_NEAR,			// 手前の描画制限
		MAX_FAR				// 奥の描画制限
	);

	// ビューマトリックスの初期化
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
}

//==========================================================================
// モード別リセット
//==========================================================================
void CCamera::ResetByMode(CScene::MODE mode)
{
	// カメラモーションリセット
	m_pCameraMotion->SetFinish(true);

	// 視点情報
	m_posR = Reset::POSITION[mode];			// 注視点
	m_rot = Reset::ROTATION[mode];			// 向き
	m_fDistance = Reset::DISTANCE[mode];	// 距離
	m_fDestDistance = m_fDistance;			// 目標の距離
	m_fOriginDistance = m_fDistance;		// 元の距離

	// 状態
	m_state = STATE::STATE_NONE;

	// その他
	m_fViewAngle = m_fDestViewAngle = none::INIT_VIEWANGLE;

	// カメラワープ
	SetWarp(m_posR);
}

//==========================================================================
// カメラ揺れリセット
//==========================================================================
void CCamera::ResetSwing()
{
	// カメラ揺れ情報を初期化
	m_swing.shiftPos	 = VEC3_ZERO;	// 位置ずれ量
	m_swing.fShiftAngle	 = 0.0f;		// 位置をずらす角度
	m_swing.fShiftLength = 0.0f;		// 位置をずらす距離
	m_swing.fSubAngle	 = 0.0f;		// ずらす角度の減算量
	m_swing.fSubLength	 = 0.0f;		// ずらす距離の減算量
}

//==========================================================================
// カメラ揺れの設定処理
//==========================================================================
void CCamera::SetSwing(const SSwing& swing)
{
	// 引数のカメラ揺れ情報を設定
	m_swing = swing;
}

//==========================================================================
// カメラワープ処理
//==========================================================================
void CCamera::SetWarp(const MyLib::Vector3& pos)
{
	// 注視点の設定
	m_posR = pos;			// 注視点
	m_posRDest = m_posR;	// 目標の注視点

	// 球面座標変換による視点の相対位置取得
	m_posV = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// 視点
	m_posVDest = m_posV;	// 目標の視点
}

//==========================================================================
// 状態の設定処理
//==========================================================================
void CCamera::SetState(const STATE state, const bool bReset)
{
	// 引数状態を設定
	m_state = state;

	// リセットフラグがONならカメラリセット
	if (bReset) { Reset(); }
}

//==========================================================================
// ライト拡散光の設定処理
//==========================================================================
void CCamera::SetLightDiffuse(const D3DXCOLOR& rCol)
{
	return m_pLight->SetDiffuse(rCol);
}

//==========================================================================
// ライト拡散光の取得処理
//==========================================================================
D3DXCOLOR CCamera::GetLightDiffuse() const
{
	return m_pLight->GetLight().Diffuse;
}

//==========================================================================
// ライト目標拡散光の取得処理
//==========================================================================
D3DXCOLOR CCamera::GetDestLightDiffuse() const
{
	return COL_LIGHT;
}

//==========================================================================
// 通常状態の更新処理
//==========================================================================
void CCamera::UpdateNoneState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 向きの正規化
#if _DEBUG
	if (!m_pCameraMotion->IsEdit())
	{
		UtilFunc::Transformation::RotNormalize(m_rot);
	}
#else
	UtilFunc::Transformation::RotNormalize(m_rot);
#endif

	// 目標距離の補正
	if (m_fDestDistance < MIN_DIS)	 { m_fDestDistance = MIN_DIS; }
	if (m_fOriginDistance < MIN_DIS) { m_fOriginDistance = MIN_DIS; }

	// 球面座標変換による目標視点の相対位置取得
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);
}

//==========================================================================
// 追従状態の更新処理
//==========================================================================
void CCamera::UpdateFollowState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ゲームモード以外では使用できない
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// 現在の外野ボール所持フラグを取得
	bool bCurWithBall = IsPlayerOutWithBall();
	if (bCurWithBall && !m_bOldWithBall)
	{ // 外野がボールを持った瞬間の場合

		// 現在のカメラポイントを開始ポイントとして保存
		m_transStartPoint.posR = m_posR;
		m_transStartPoint.posV = m_posV;
		m_transStartPoint.rot = m_rot;
		m_transStartPoint.fDistance = m_fDistance;

		// 状態遷移の経過時間を初期化
		m_fTransTime = 0.0f;

		// 遷移の終了時間を保存
		m_fEndTime = game::END_TIME;

		b = false;
	}

	if (bCurWithBall)
	{ // 外野がボールを持っていた場合

		// 状態遷移の経過時間を加算
		m_fTransTime += fDeltaTime * fSlowRate;

		// カメラ状態遷移の更新
		if (UpdateTrans(STATE::STATE_OUTFIELD))
		{ // 遷移しきった場合

			// 外野状態にする
			SetState(STATE::STATE_OUTFIELD);

			// 状態遷移の経過時間を初期化
			m_fTransTime = 0.0f;

			// 遷移の終了時間を初期化
			m_fEndTime = 0.0f;
		}
	}
	else
	{ // 外野がボールを持っていない場合

		if (m_fEndTime > 0.0f)
		{ // 状態が遷移しかけていた場合

			// 終了時間を設定
			if (!b)
			{
				// 現在のカメラポイントを開始ポイントとして保存
				m_transStartPoint.posR = m_posR;
				m_transStartPoint.posV = m_posV;
				m_transStartPoint.rot = m_rot;
				m_transStartPoint.fDistance = m_fDistance;

				// 
				m_fEndTime = m_fTransTime;
				UtilFunc::Transformation::ValueNormalize(m_fEndTime, game::END_TIME, game::MIN_TIME);
				m_fTransTime = 0.0f;
				b = true;
			}

			// 状態遷移の経過時間を加算
			m_fTransTime += fDeltaTime * fSlowRate;

			// TODO
#if 0
			// 
			SCameraPoint transDestStart = CameraPoint(STATE::STATE_FOLLOW);
			SCameraPoint transDiffStart;

			// 
			transDiffStart.posR			= transDestStart.posR - m_transStartPoint.posR;
			transDiffStart.posV			= transDestStart.posV - m_transStartPoint.posV;
			transDiffStart.rot			= transDestStart.rot  - m_transStartPoint.rot;
			transDiffStart.fDistance	= transDestStart.fDistance - m_transStartPoint.fDistance;

			// 
			m_transStartPoint.posR		+= transDiffStart.posR * 0.2f;
			m_transStartPoint.posV		+= transDiffStart.posV * 0.2f;
			m_transStartPoint.rot		+= transDiffStart.rot  * 0.2f;
			m_transStartPoint.fDistance	+= transDiffStart.fDistance * 0.2f;
#endif

			// TODO
#if 0
			m_transStartPoint = CameraPoint(STATE::STATE_FOLLOW);
#endif

			// カメラ状態遷移の更新
			if (UpdateTrans(STATE::STATE_FOLLOW))
			{ // 遷移しきった場合

				// 状態遷移の経過時間を初期化
				m_fTransTime = 0.0f;

				// 遷移の終了時間を初期化
				m_fEndTime = 0.0f;

				Reset();
				b = false;
			}
		}
		else
		{ // 状態が遷移済みの場合

			// 追従カメラの更新
			UpdateFollow(fDeltaTime, fDeltaRate, fSlowRate);

			// 状態遷移の経過時間を初期化
			m_fTransTime = 0.0f;

			// 遷移の終了時間を初期化
			m_fEndTime = 0.0f;

			b = false;
		}
	}

	// 現在の外野ボール所持フラグを保存
	m_bOldWithBall = bCurWithBall;
}

//==========================================================================
// 外野状態の更新処理
//==========================================================================
void CCamera::UpdateOutFieldState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ゲームモード以外では使用できない
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// 現在の外野ボール所持フラグを取得
	bool bCurWithBall = IsPlayerOutWithBall();
	if (!bCurWithBall && m_bOldWithBall)
	{ // 外野がボールを投げた瞬間の場合

		// 現在のカメラポイントを開始ポイントとして保存
		m_transStartPoint.posR = m_posR;
		m_transStartPoint.posV = m_posV;
		m_transStartPoint.rot = m_rot;
		m_transStartPoint.fDistance = m_fDistance;

		// 状態遷移の経過時間を初期化
		m_fTransTime = 0.0f;

		// 遷移の終了時間を保存
		m_fEndTime = game::END_TIME;

		b = false;
	}

	if (!bCurWithBall)
	{ // 外野がボールを持っていない場合

		// 状態遷移の経過時間を加算
		m_fTransTime += fDeltaTime * fSlowRate;

		// カメラ状態遷移の更新
		if (UpdateTrans(STATE::STATE_FOLLOW))
		{ // 遷移しきった場合

			// 追従状態にする
			SetState(STATE::STATE_FOLLOW);

			// 状態遷移の経過時間を初期化
			m_fTransTime = 0.0f;

			// 遷移の終了時間を初期化
			m_fEndTime = 0.0f;
		}
	}
	else
	{ // 外野がボールを持っていた場合

		if (m_fEndTime > 0.0f)
		{ // 状態が遷移しかけていた場合

			// 終了時間を設定
			if (!b)
			{
				// 現在のカメラポイントを開始ポイントとして保存
				m_transStartPoint.posR = m_posR;
				m_transStartPoint.posV = m_posV;
				m_transStartPoint.rot = m_rot;
				m_transStartPoint.fDistance = m_fDistance;

				// 
				m_fEndTime = m_fTransTime;
				UtilFunc::Transformation::ValueNormalize(m_fEndTime, game::END_TIME, game::MIN_TIME);
				m_fTransTime = 0.0f;
				b = true;
			}

			// 状態遷移の経過時間を加算
			m_fTransTime += fDeltaTime * fSlowRate;

			// カメラ状態遷移の更新
			if (UpdateTrans(STATE::STATE_OUTFIELD))
			{ // 遷移しきった場合

				// 状態遷移の経過時間を初期化
				m_fTransTime = 0.0f;

				// 遷移の終了時間を初期化
				m_fEndTime = 0.0f;

				Reset();
				b = false;
			}
		}
		else
		{ // 状態が遷移済みの場合

			// 外野カメラの更新
			UpdateOutField(fDeltaTime, fDeltaRate, fSlowRate);

			// 状態遷移の経過時間を初期化
			m_fTransTime = 0.0f;

			// 遷移の終了時間を初期化
			m_fEndTime = 0.0f;

			b = false;
		}
	}

	// 現在の外野ボール所持フラグを保存
	m_bOldWithBall = bCurWithBall;
}

//==========================================================================
// 通常状態のリセット
//==========================================================================
void CCamera::ResetNoneState()
{
	// 注視点の設定
	m_posR = m_posRDest = m_posROrigin = none::INIT_POSR;

	// 距離の設定
	m_fDistance = m_fDestDistance = m_fOriginDistance = none::INIT_DISTANCE;

	// 視野角の設定
	m_fViewAngle = m_fDestViewAngle = none::INIT_VIEWANGLE;

	// 原点向きの設定
	m_rotOrigin = none::INIT_ROT;

	// 原点向きの正規化
	UtilFunc::Transformation::RotNormalize(m_rotOrigin);

	// 向きの設定
	m_rot = m_rotDest = m_rotOrigin;

	// 球面座標変換による目標視点の相対位置取得
	m_posV = m_posVDest = CalcSpherePosition(m_posROrigin, m_rotOrigin, -m_fOriginDistance);
}

//==========================================================================
// 追従状態のリセット
//==========================================================================
void CCamera::ResetFollowState()
{
	// ゲームモード以外では使用できない
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// プレイヤー左右最大位置を計算
	m_side = m_sideDest = GetPlayerMaxSide();

	// 左右間の距離割合を計算
	const float fDisRate = CalcDistanceRate(m_sideDest);

	// 注視点を計算
	m_posR = m_posRDest = m_posROrigin = CalcFollowPositionR(m_sideDest, fDisRate);

	// 距離を計算
	m_fDistance = m_fDestDistance = m_fOriginDistance = CalcFollowDistance(fDisRate);

	// 向きを設定
	m_rot = m_rotDest = m_rotOrigin = follow::INIT_ROT;

	// 視野角を設定
	if (m_bMotion)	{ m_fViewAngle = m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fViewAngle = m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// 球面座標変換による目標視点の相対位置取得
	m_posV = m_posVDest = CalcSpherePosition(m_posROrigin, m_rotOrigin, -m_fOriginDistance);
}

//==========================================================================
// 外野状態のリセット
//==========================================================================
void CCamera::ResetOutFieldState()
{
	// ゲームモード以外では使用できない
	if (GET_MANAGER->GetMode() != CScene::MODE_GAME) { return; }

	// 注視点を設定
	m_posR = m_posRDest = MyLib::Vector3(CGameManager::CENTER_LINE, out::POSRY, out::POSRZ);

	// 距離を設定
	m_fDistance = m_fDestDistance = out::DIS;

	// 向きを設定
	m_rot = m_rotDest = out::ROT;

	// 目標視野角を設定
	if (m_bMotion)	{ m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// 現在視野角を慣性補正
	UtilFunc::Correction::InertiaCorrection(m_fViewAngle, m_fDestViewAngle, follow::REV_VIEWANGLE);

	// 球面座標変換による相対位置の取得
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// 目標視点に設定
}

//==========================================================================
// 追従カメラの更新
//==========================================================================
void CCamera::UpdateFollow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// プレイヤー左右最大位置を計算
	m_sideDest = GetPlayerMaxSide();

	// 慣性補正
	UtilFunc::Correction::InertiaCorrection(m_side.l, m_sideDest.l, follow::REV_SIDE);
	UtilFunc::Correction::InertiaCorrection(m_side.r, m_sideDest.r, follow::REV_SIDE);

	// 左右間の距離割合を計算
	const float fDisRate = CalcDistanceRate(m_side);

	// 目標注視点を計算
	MyLib::Vector3 posCurDest = CalcFollowPositionR(m_side, fDisRate);

	// 目標注視点を補正
	if (m_posRDest.x + follow::DEST_POSR_BOXSIZE < posCurDest.x)
	{ // 目標注視点を動かさない範囲より右側の場合

		// 注視点を右端に補正
		m_posRDest.x = posCurDest.x - follow::DEST_POSR_BOXSIZE;
	}
	else if (m_posRDest.x - follow::DEST_POSR_BOXSIZE > posCurDest.x)
	{ // 目標注視点を動かさない範囲より左側の場合

		// 注視点を左端に補正
		m_posRDest.x = posCurDest.x + follow::DEST_POSR_BOXSIZE;
	}

	// X注視点の範囲補正
	RevFollowPositionR(&m_posRDest.x, fDisRate);

#ifdef FOLLOW_DEBUG
	CEffect3D::Create(MyLib::Vector3(posCurDest.x, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Red(), 20.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Yellow(), 15.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);

	CEffect3D::Create(MyLib::Vector3(m_posRDest.x + follow::DEST_POSR_BOXSIZE, 50.0f, -follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x - follow::DEST_POSR_BOXSIZE, 50.0f, -follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x + follow::DEST_POSR_BOXSIZE, 50.0f, +follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(m_posRDest.x - follow::DEST_POSR_BOXSIZE, 50.0f, +follow::DEST_POSR_BOXSIZE), VEC3_ZERO, MyLib::color::Green(), 5.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
#endif

	// 現在注視点を慣性補正
	UtilFunc::Correction::InertiaCorrection(m_posR, m_posRDest, follow::REV_POSR);

	// 現在注視点のY/Z座標は目標座標にし続ける
	m_posR.y = m_posRDest.y = posCurDest.y;
	m_posR.z = m_posRDest.z = posCurDest.z;

	// 距離を計算
	m_fDistance = m_fDestDistance = CalcFollowDistance(fDisRate);

	// 向きを設定
	m_rot = m_rotDest = follow::INIT_ROT;

	// 目標視野角を設定
	if (m_bMotion)	{ m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// 現在視野角を慣性補正
	UtilFunc::Correction::InertiaCorrection(m_fViewAngle, m_fDestViewAngle, follow::REV_VIEWANGLE);

	// 球面座標変換による相対位置の取得
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// 目標視点に設定
}

//==========================================================================
// 外野カメラの更新
//==========================================================================
void CCamera::UpdateOutField(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 注視点を設定
	m_posR = m_posRDest = MyLib::Vector3(CGameManager::CENTER_LINE, out::POSRY, out::POSRZ);

	// 距離を設定
	m_fDistance = m_fDestDistance = out::DIS;

	// 向きを設定
	m_rot = m_rotDest = out::ROT;

	// 目標視野角を設定
	if (m_bMotion)	{ m_fDestViewAngle = none::INIT_VIEWANGLE; }
	else			{ m_fDestViewAngle = follow::INIT_VIEWANGLE; }

	// 現在視野角を慣性補正
	UtilFunc::Correction::InertiaCorrection(m_fViewAngle, m_fDestViewAngle, follow::REV_VIEWANGLE);

	// 球面座標変換による相対位置の取得
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// 目標視点に設定
}

//==========================================================================
// カメラ状態遷移の更新
//==========================================================================
bool CCamera::UpdateTrans(const STATE state)
{
	// 状態遷移の経過時間を補正
	bool bRev = UtilFunc::Transformation::ValueNormalize(m_fTransTime, m_fEndTime, 0.0f);

	// ゲームカメラ状態遷移の終了ポイントを取得
	SCameraPoint transEndPoint = CameraPoint(state);

	// 注視点を設定
	m_posR = m_posRDest = UtilFunc::Correction::EasingQuintOut(m_transStartPoint.posR, transEndPoint.posR, 0.0f, m_fEndTime, m_fTransTime);

	// 距離を設定
	m_fDistance = m_fDestDistance = UtilFunc::Correction::EasingQuintOut(m_transStartPoint.fDistance, transEndPoint.fDistance, 0.0f, m_fEndTime, m_fTransTime);

	// 向きを設定
	m_rot = m_rotDest = UtilFunc::Correction::EasingQuintOut(m_transStartPoint.rot, transEndPoint.rot, 0.0f, m_fEndTime, m_fTransTime);

	// 球面座標変換による相対位置の取得
	m_posV = m_posVDest = CalcSpherePosition(m_posR, m_rot, -m_fDistance);	// 目標視点に設定

	// 経過時間が補正されたかを返す
	return bRev;
}

//==========================================================================
// 外野がボールを持っているか
//==========================================================================
bool CCamera::IsPlayerOutWithBall()
{
	CGameManager* pGameManager = CGameManager::GetInstance();	// ゲームマネージャー
	assert(pGameManager != nullptr);		// ゲームマネージャーがない場合エラー
	CBall* pBall = pGameManager->GetBall();	// ボール情報
	assert(pBall != nullptr);				// ボールがない場合エラー
	CPlayer* pTarget = pBall->GetTarget();	// ボール標的プレイヤー情報

	if (pTarget != nullptr
	&&  pTarget->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
	{ // ボールのターゲットが外野の場合

		return true;
	}

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤー内部リスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// プレイヤーイテレーター
	while (list.ListLoop(itr))
	{ // 要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT
		&&  pPlayer->GetBall() != nullptr)
		{ // ボールを持った外野がいた場合

			return true;
		}
	}

	return false;
}

//==========================================================================
// プレイヤー最大左右座標の取得
//==========================================================================
CCamera::SSide CCamera::GetPlayerMaxSide()
{
	SSide pos;	// 左右座標
	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤー内部リスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// プレイヤーイテレーター
	while (list.ListLoop(itr))
	{ // 要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		if (pPlayer == nullptr) continue;

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		// 外野の場合次へ
		if (pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) { continue; }

		// 死後運搬状態の場合次へ
		if (pPlayer->GetState() == CPlayer::EState::STATE_DEAD_CARRY) { continue; }

		// 左の更新
		if (posPlayer.x < pos.l) { pos.l = posPlayer.x; }

		// 右の更新
		if (posPlayer.x > pos.r) { pos.r = posPlayer.x; }
	}

	// 判定した左右座標を返す
	return pos;
}

//==========================================================================
// 左右間の距離割合の計算
//==========================================================================
float CCamera::CalcDistanceRate(const SSide& rSide)
{
	const MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();	// コートサイズ
	const float fCurDis = rSide.r - rSide.l;	// 左右間の距離
	const float fMaxDis = sizeCourt.x * 2.0f;	// 左右間の最大距離

	// 左右間の距離割合を返す
	return fCurDis / fMaxDis;
}

//==========================================================================
// 追従カメラの距離計算
//==========================================================================
float CCamera::CalcFollowDistance(const float fDisRate)
{
	// カメラ距離を距離割合から計算し返す
	return (follow::MAX_DIS * fDisRate + MIN_DIS);
}

//==========================================================================
// 追従カメラの注視点計算
//==========================================================================
MyLib::Vector3 CCamera::CalcFollowPositionR(const SSide& rSide, const float fDisRate)
{
	const MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();	// コートサイズ

	// 左右座標の平均からX注視点を計算
	float fTargetX = (rSide.l + rSide.r) * 0.5f;

	// X注視点の範囲補正
	RevFollowPositionR(&fTargetX, fDisRate);

	// Y/Z注視点を距離割合から計算
	float fTargetY = UtilFunc::Correction::EasingLinear(follow::POSRY_START, follow::POSRY_END, fDisRate);	// カメラY注視点
	float fTargetZ = UtilFunc::Correction::EasingLinear(follow::POSRZ_START, follow::POSRZ_END, fDisRate);	// カメラZ注視点

	// 計算した注視点を返す
	return MyLib::Vector3(fTargetX, fTargetY, fTargetZ);
}

//==========================================================================
// X注視点の範囲補正
//==========================================================================
void CCamera::RevFollowPositionR(float* pTargetX, const float fDisRate)
{
	const MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();	// コートサイズ

	// X注視点の補正
	float fCourtHalfSize = sizeCourt.x * 0.5f;		// コートの半分の大きさ
	float fPlusOffset = fCourtHalfSize * fDisRate;	// 注視点範囲オフセット
	UtilFunc::Transformation::ValueNormalize(*pTargetX, CGameManager::CENTER_LINE + fCourtHalfSize - fPlusOffset, CGameManager::CENTER_LINE - fCourtHalfSize + fPlusOffset);

#ifdef FOLLOW_DEBUG
	CEffect3D::Create(MyLib::Vector3(CGameManager::CENTER_LINE - fCourtHalfSize + fPlusOffset, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Cyan(), 10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
	CEffect3D::Create(MyLib::Vector3(CGameManager::CENTER_LINE + fCourtHalfSize - fPlusOffset, 50.0f, 0.0f), VEC3_ZERO, MyLib::color::Cyan(), 10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);
#endif
}

//==========================================================================
// 球面座標変換による相対位置の取得処理
//==========================================================================
MyLib::Vector3 CCamera::CalcSpherePosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot, const float fDis)
{
	// 球面座標変換で相対位置を計算
	MyLib::Vector3 out;
	out.x = rPos.x + cosf(rRot.z) * sinf(rRot.y) * fDis;
	out.z = rPos.z + cosf(rRot.z) * cosf(rRot.y) * fDis;
	out.y = rPos.y + sinf(rRot.z) * fDis;

	// 変換した相対座標を返す
	return out;
}

//==========================================================================
// カメラ揺れの更新
//==========================================================================
void CCamera::UpdateSwing()
{
	// 注視点のずらし量が設定されていない場合抜ける
	if (m_swing.fShiftLength <= 0.0f) { return; }

	float fRotY;			// 位置ずれ向き
	D3DXQUATERNION quat;	// クォータニオン
	D3DXMATRIX mtxRot;		// 回転マトリックス
	MyLib::Vector3 offset;	// 位置ずれオフセット
	MyLib::Vector3 vecAxis = m_posR - m_posV;	// 回転軸ベクトル

	// クォータニオンを作成
	D3DXVec3Normalize(&vecAxis, &vecAxis);	// 回転軸を正規化
	D3DXQuaternionRotationAxis(&quat, &vecAxis, m_swing.fShiftAngle);

	// 回転マトリックスを作成
	D3DXMatrixRotationQuaternion(&mtxRot, &quat);

	// 位置をずらす向きを求める
	fRotY = atan2f(-vecAxis.z, vecAxis.x);

	// 位置ずれオフセットを設定
	float fCalcTemp = m_swing.fShiftLength * (fabsf(m_fDistance) * REV_SWING);
	offset = MyLib::Vector3(sinf(fRotY) * fCalcTemp, 0.0f, cosf(fRotY) * fCalcTemp);

	// オフセットを反映した回転マトリックスを座標変換
	D3DXVec3TransformCoord(&m_swing.shiftPos, &offset, &mtxRot);

	// 視点に位置のずれを加算
	m_posV += m_swing.shiftPos;

	// 位置ずれ量を減算
	m_swing.fShiftAngle  -= m_swing.fSubAngle;
	m_swing.fShiftLength -= m_swing.fSubLength;

	// 角度を補正
	UtilFunc::Transformation::RotNormalize(m_swing.fShiftAngle);

	// 距離を補正
	if (m_swing.fShiftLength < 0.0f) { m_swing.fShiftLength = 0.0f; }
}

//==========================================================================
// スポットライトのベクトル更新
//==========================================================================
void CCamera::UpdateSpotLightVec(const MyLib::Vector3& rPosR, const MyLib::Vector3& rPosV)
{
	// 視点から注視点へのベクトルを計算
	MyLib::Vector3 vec = rPosR - rPosV;

	// スポットライトの方向設定
	m_pLight->SetDirection(vec);
}

//==========================================================================
// スクリーン内の判定
//==========================================================================
bool CCamera::IsOnScreen(const MyLib::Vector3& pos)
{
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };	// ビューポート
	D3DXMATRIX mtxWorld; // ワールドマトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 敵のスクリーン座標を算出
	MyLib::Vector3 screenPos;
	D3DXVec3Project
	(
		&screenPos,
		&pos,
		&vp,
		&m_mtxProjection,
		&m_mtxView,
		&mtxWorld
	);

	// 判定
	if (screenPos.x >= 0.0f && screenPos.x <= SCREEN_WIDTH &&
		screenPos.y >= 0.0f && screenPos.y <= SCREEN_HEIGHT &&
		screenPos.z < 1.0f)
	{
		return true;
	}

	return false;
}

//==========================================================================
// スクリーン座標取得
//==========================================================================
MyLib::Vector3 CCamera::GetScreenPos(const MyLib::Vector3& pos)
{
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };	// ビューポート
	D3DXMATRIX mtxWorld; // ワールドマトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スクリーン座標を算出
	MyLib::Vector3 screenPos;	// スクリーン座標
	D3DXVec3Project
	(
		&screenPos,
		&pos,
		&vp,
		&m_mtxProjection,
		&m_mtxView,
		&mtxWorld
	);

	return screenPos;
}

//==========================================================================
// 現在の追従ポイント取得処理
//==========================================================================
CCamera::SCameraPoint CCamera::CameraPoint(const STATE state)
{
	if (state == STATE::STATE_NONE)
	{ // 状態の指定がなかった場合

		if (IsPlayerOutWithBall())
		{ // 外野がボールを持っていた場合

			// 現在の追従ポイントを返す
			return FollowPoint();
		}
		else
		{ // 外野がボールを持っていない場合

			// 現在の外野ポイントを返す
			return OutFieldPoint();
		}
	}

	switch (state)
	{ // 状態ごとの処理
	case STATE::STATE_FOLLOW:

		// 現在の追従ポイントを返す
		return FollowPoint();

	case STATE::STATE_OUTFIELD:

		// 現在の外野ポイントを返す
		return OutFieldPoint();

	default:
		assert(false);
		break;
	}

	assert(false);
	return {};
}

//==========================================================================
// 現在の追従ポイント取得処理
//==========================================================================
CCamera::SCameraPoint CCamera::FollowPoint()
{
	SCameraPoint data;	// カメラ情報

	// プレイヤー左右最大位置を計算
	const SSide posSide = GetPlayerMaxSide();

	// 左右間の距離割合を計算
	const float fDisRate = CalcDistanceRate(posSide);

	// 注視点を計算
	data.posR = CalcFollowPositionR(posSide, fDisRate);

	// 距離を計算
	data.fDistance = CalcFollowDistance(fDisRate);

	// 向きを設定
	data.rot = follow::INIT_ROT;

	// 球面座標変換による目標視点の相対位置取得
	data.posV = CalcSpherePosition(data.posR, data.rot, -data.fDistance);

	// 追従カメラ情報を返す
	return data;
}

//==========================================================================
// 現在の外野ポイント取得処理
//==========================================================================
CCamera::SCameraPoint CCamera::OutFieldPoint()
{
	SCameraPoint data;	// カメラ情報

	// 注視点を計算
	data.posR = MyLib::Vector3(CGameManager::CENTER_LINE, out::POSRY, out::POSRZ);

	// 距離を計算
	data.fDistance = m_fDestDistance = out::DIS;

	// 向きを設定
	data.rot = out::ROT;

	// 球面座標変換による目標視点の相対位置取得
	data.posV = CalcSpherePosition(data.posR, data.rot, -data.fDistance);

	// 外野カメラ情報を返す
	return data;
}

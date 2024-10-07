//=============================================================================
// 
// カメラ処理 [camera.cpp]
// Author : 相馬靜雅
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
#include "light.h"
#include "3D_effect.h"
#include "calculation.h"
#include "pause.h"
#include "objectX.h"

#include "camera_debug.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define MOVE			(2.5f)				// 移動量
#define MAX_LENGTH		(50000.0f)			// 最大距離
#define MIN_LENGTH		(10.0f)				// 最少距離
#define START_CAMERALEN	(1700.0f)			// 元の距離
#define ROT_MOVE_MOUSE	(0.01f)				// 回転移動量
#define ROT_MOVE		(0.025f)			// 回転移動量
#define MIN_STICKROT			(-D3DX_PI * 0.25f)	// カメラ固定用
#define MIN_ROT			(-D3DX_PI * 0.49f)	// カメラ固定用
#define MAX_ROT			(D3DX_PI * 0.49f)	// カメラ固定用
#define BACKFOLLOW_TIME	(20)				// 背面補正までの時間
#define TITLESTATE_CHANGE	(60 * 14)
#define TITLESTATE_CHASE	(60 * 20)
#define RESULT_LEN	(500.0f)
//#define RESULT_LEN	(1000.0f)
#define DECIDECAMERAROT_NONE		(MyLib::Vector3(0.0f, 0.0f, 0.0f))
#define DECIDECAMERAPOS_NONE		(MyLib::Vector3(0.0f, 230.0f, -50.0f))
#define DECIDE_LEN	(500.0f)


namespace
{
	const MyLib::Vector3 TITLE_POSR_DEST = MyLib::Vector3(45271.0f, -34.0f, 591.0f);
	const MyLib::Vector3 RANKING_POSR_DEST = MyLib::Vector3(625.34f, 503.34f, 2667.39f);	// ランキングの注視点
	//const MyLib::Vector3 DEFAULT_GAMEROT = MyLib::Vector3(0.0f, 0.0f, -0.10f);	// ゲームのデフォルト向き
	const MyLib::Vector3 DEFAULT_TITLEROT = MyLib::Vector3(0.0f, 0.67f, -0.08f);	// タイトルのデフォルト向き
	const MyLib::Vector3 DEFAULT_RESULTROT = MyLib::Vector3(0.0f, 0.0f, -0.15f);	// リザルトのデフォルト向き
	const MyLib::Vector3 DEFAULT_RANKINGROT = MyLib::Vector3(0.0f, 0.0f, -0.05f);	// ランキングのデフォルト向き
	const MyLib::Vector3 DEFAULT_GAMEROT = MyLib::Vector3(0.0f, 0.38f, -0.05f);	// ゲームのデフォルト向き
	const float DEFAULT_TITLELEN = 1265.0f;		// タイトルのデフォルト長さ
	const float DEFAULT_RANKINGLEN = 1540.0f;	// ランキングのデフォルト長さ
	const float MULTIPLY_POSV_CORRECTION = 2.1f;	// (ゲーム時)視点の補正係数倍率
	const float MULTIPLY_POSR_CORRECTION = 2.1f;	// (ゲーム時)注視点の補正係数倍率
	const float DISATNCE_POSR_PLAYER = 0.0f;		// (ゲーム時)プレイヤーとの注視点距離
	const float MIN_ROCKONDISTANCE = 1.0f;
	const float ROTDISTANCE_ROCKON = D3DX_PI * 0.095f;	// ロックオン向きのズレ
	const MyLib::Vector3 ROTDISTANCE_COUNTER = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, -D3DX_PI * 0.05f);	// 反撃時の向きズレ
	const float LENGTH_COUNTER = 400.0f;					// カウンター時のカメラ長さ
	const MyLib::Vector3 ROTATION_PRAYER = MyLib::Vector3(0.0f, -0.89f, 0.06f);	// 祈り時の向き
	const float UPDISTANCE_MULTIPLY = (0.25f);
	const float NOTUPDISTANCE_MULTIPLY = (0.05f);
	const float MIN_DISNTANCE = (1500.0f);
	const float DISTANCE_TIMER = (1.0f / 120.0f);
	const float MAX_AUTODISTANCEHEIGHT = 600.0f;	// 自動高さ制御の最大値
	const MyLib::Vector3 GOAL_ROT = MyLib::Vector3(0.0f, D3DX_PI * 0.3f, 0.06f);	// 祈り時の向き
	const float GOAL_MULTI = (0.03f);
	const float GOAL_LEN = (500.0f);
	const float MOVE_WASD = 10.0f;


	const float  ROT_MOVE_STICK_Y = 0.00040f;	// 回転移動量
	const float  ROT_MOVE_STICK_Z = 0.00020f;	// 回転移動量
}

//==========================================================================
// コンストラクタ
//==========================================================================
CCamera::CCamera()
{
	m_viewport.X = 0;							// 描画する画面の左上X座標
	m_viewport.Y = 0;							// 描画する画面の左上Y座標
	m_viewport.Width = 0;						// 描画する画面の幅
	m_viewport.Height = 0;						// 描画する画面の高さ
	m_viewport.Width = 0;						// 描画する画面の幅
	m_viewport.Height = 0;						// 描画する画面の高さ
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;
	m_posR = MyLib::Vector3();		// 注視点(見たい場所)
	m_posV = MyLib::Vector3();		// 視点(カメラの位置)
	m_posVDest = MyLib::Vector3();	// 目標の視点
	m_posRDest = MyLib::Vector3();	// 目標の注視点
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_move = MyLib::Vector3();		// 移動量
	m_rot = MyLib::Vector3();		// 向き
	m_rotDest = 0.0f;							// 目標の向き
	m_TargetPos = MyLib::Vector3();	// 追従目標の位置
	m_fDistance = 0.0f;							// 距離
	m_fDestDistance = 0.0f;						// 目標の距離
	m_fOriginDistance = 0.0f;					// 元の距離
	m_nShakeLength = 0.0f;						// 揺れの長さ
	m_nShakeLengthY = 0.0f;						// Yの揺れの長さ
	m_fMoveShake = 0.0f;						// 揺れの移動量
	m_fMoveShakeY = 0.0f;						// Yの揺れの移動量
	m_bFollow = false;							// 追従するかどうか
	m_bMotion = false;							// モーション中かどうか
	m_state = CAMERASTATE_NONE;					// 状態
	m_nCntState = 0;							// 状態カウンター
	
	m_pCameraMotion = nullptr;					// カメラモーションのポインタ
	m_pControlState = nullptr;	// 操作の状態ポインタ
	m_pDebugControll = nullptr;	// デバッグ処理

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

	// リセット
	m_bFollow = true;	// 追従するかどうか
	Reset(CScene::MODE_GAME);

	// 操作の状態設定
	SetControlState(DEBUG_NEW CCameraControlState_Normal(this));

	// デバッグ専用処理
	if (m_pDebugControll == nullptr)
	{
		m_pDebugControll = DEBUG_NEW CCamera_Debug(this);
	}

	// カメラモーション作成
	m_pCameraMotion = CCameraMotion::Create();
	return S_OK;
}

//==========================================================================
// ビューポートの設定
//==========================================================================
void CCamera::SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size)
{
	m_viewport.X = (DWORD)pos.x;			// 描画する画面の左上X座標
	m_viewport.Y = (DWORD)pos.y;			// 描画する画面の左上Y座標
	m_viewport.Width = (DWORD)size.x;		// 描画する画面の幅
	m_viewport.Height = (DWORD)size.y;		// 描画する画面の高さ
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

//==========================================================================
// カメラの終了処理
//==========================================================================
void CCamera::Uninit()
{
	
	if (m_pControlState != nullptr)
	{
		delete m_pControlState;
		m_pControlState = nullptr;
	}

	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->Uninit();
		m_pCameraMotion = nullptr;
	}

	// デバッグ専用処理
	if (m_pDebugControll != nullptr)
	{
		delete m_pDebugControll;
		m_pDebugControll = nullptr;
	}
}

//==========================================================================
// カメラの更新処理
//==========================================================================
void CCamera::Update()
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// デバッグ専用処理
	if (m_pDebugControll != nullptr)
	{
		m_pDebugControll->Update();
	}

	// 視点・注視点移動
	MoveCameraInput();
	UpdateSpotLightVec();

	// 注視点の反映
	ReflectCameraR();

	// 視点反映
	ReflectCameraV();

	// 状態更新
	UpdateState();


	CInputMouse* pMouse = CInputMouse::GetInstance();

	MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ(pMouse->GetPosition(), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), m_mtxView, m_mtxProjection);

	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- カメラ情報 ----------------\n"
		"【向き】[X：%f Y：%f Z：%f]\n"
		"【距離】[%f]\n"
		"【視点】[X：%f Y：%f Z：%f]\n"
		"【注視点】[X：%f Y：%f Z：%f]\n",
		m_rot.x, m_rot.y, m_rot.z,
		m_fDistance,
		m_posV.x, m_posV.y, m_posV.z,
		m_posR.x, m_posR.y, m_posR.z);

	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- カメラ情報 ----------------\n"
		"【交差点】[X：%f Y：%f Z：%f]\n",
		pos.x, pos.y, pos.z);

	// カメラモーション作成
	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->Update();
	}
}

//==========================================================================
// モード別更新処理
//==========================================================================
void CCamera::UpdateByMode()
{
	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
		break;

	default:
		break;
	}
}

//==========================================================================
// 入力機器のカメラ移動
//==========================================================================
void CCamera::MoveCameraInput()
{
	// スティックでの移動処理
	MoveCameraStick();
}

//==========================================================================
// コントローラーでの移動処理
//==========================================================================
void CCamera::MoveCameraStick(int nIdx)
{
#if 0
	// 操作処理
	m_pControlState->Controll(this);
#endif

}

//==========================================================================
// カメラワープ処理
//==========================================================================
void CCamera::WarpCamera(const MyLib::Vector3& pos)
{
	// 注視点の代入
	m_posR = pos;			// 注視点
	m_posRDest = m_posR;	// 目標の注視点

	// 視点の代入
	ReflectCameraV();
	m_posVDest = m_posV;	// 目標の視点
}

//==========================================================================
// カメラの視点代入処理
//==========================================================================
void CCamera::ReflectCameraV()
{

	if (!m_bFollow)
	{// 追従しないとき

		// 視点の代入処理
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else
	{// 追従ON

		// 視点の代入処理
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		// 補正する
		m_posV += (m_posVDest - m_posV) * 0.15f;
	}
}


//==========================================================================
// カメラの注視点代入処理
//==========================================================================
void CCamera::ReflectCameraR()
{

	if (!m_bFollow ||
		(m_bMotion && m_bFollow))
	{// 追従しないとき

		// 注視点の代入処理
		m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{// 追従ON

		// 補正する
		m_posR += (m_posRDest - m_posR) * (0.08f * MULTIPLY_POSR_CORRECTION);
	}

}

//==========================================================================
// カメラの設定処理
//==========================================================================
void CCamera::SetCamera()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),	// 視野角
								(float)m_viewport.Width / (float)m_viewport.Height,	// アスペクト比
								10.0f,		// 手前の制限
								150000.0f);	// 奥行きの制限

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,	// 視点
		&m_posR,	// 注視点
		&m_vecU);	// 上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

}

//==========================================================================
// スポットライトのベクトル更新
//==========================================================================
void CCamera::UpdateSpotLightVec()
{
	// 方向ベクトル
	MyLib::Vector3 vec = MyLib::Vector3();

	// 視点から注視点への向き
	vec = m_posR - m_posV;

	// 正規化
	D3DXVec3Normalize(&vec, &vec);

	// スポットライトの方向設定
	CManager::GetInstance()->GetLight()->UpdateSpotLightDirection(vec);
}

//==========================================================================
// カメラの振動設定処理
//==========================================================================
void CCamera::SetShake(int nTime, float fLength, float fLengthY)
{
	// 振動状態に設定
	m_state = CAMERASTATE_SHAKE;

	if (m_nCntState > 0)
	{
		if (m_nShakeLength <= fLength)
		{
			m_nShakeLength = fLength;	// 揺れの大きさ
		}
		if (m_nShakeLengthY <= fLengthY)
		{
			m_nShakeLengthY = fLengthY;	// 揺れの大きさ
		}
		if (m_nCntState <= nTime)
		{
			m_nCntState = nTime;	// 状態遷移カウンター
		}
	}
	else
	{
		m_nShakeLength = fLength;	// 揺れの大きさ
		m_nShakeLengthY = fLengthY;	// Yの揺れの大きさ
		m_nCntState = nTime;		// 状態遷移カウンター
	}
}

//==========================================================================
// カメラの状態更新処理
//==========================================================================
void CCamera::UpdateState()
{
	switch (m_state)
	{
	case CAMERASTATE_NONE:
		break;

	case CAMERASTATE_SHAKE:

		if (m_nCntState % 3 == 0)
		{
			// 画面揺れ
			Shake();
		}
		else
		{
			// 視点の代入処理
			m_posV.x += m_fMoveShake;
			m_posV.y += m_fMoveShakeY;
			m_posV.z += m_fMoveShake;

			// 注視点の代入処理
			m_posR.x += m_fMoveShake;
			m_posR.y += m_fMoveShakeY;
			m_posR.z += m_fMoveShake;

			// 慣性補正
			UtilFunc::Correction::InertiaCorrection(m_nShakeLength, 0.0f, 0.01f);
			UtilFunc::Correction::InertiaCorrection(m_nShakeLengthY, 0.0f, 0.01f);
		}

		// 状態遷移カウンター減算
		m_nCntState--;

		if (m_nCntState <= 0)
		{
			m_nCntState = 0;
			m_state = CAMERASTATE_NONE;
			m_nShakeLength = 0.0f;	// 揺れの大きさ
			m_nShakeLengthY = 0.0f;	// Yの揺れの大きさ
		}
		break;
	}
}

//==========================================================================
// カメラの振動処理
//==========================================================================
void CCamera::Shake()
{
	// 長さ取得
	int nLength = (int)m_nShakeLength;
	int nLengthY = (int)m_nShakeLengthY;

	m_fMoveShake = (float)UtilFunc::Transformation::Random(-nLength, nLength) * sinf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);	// 揺れの移動量
	m_fMoveShakeY = (float)UtilFunc::Transformation::Random(-nLengthY, nLengthY);						// Yの揺れの移動量

	// 視点の代入処理
	m_posV.x += (float)UtilFunc::Transformation::Random(-nLength, nLength) * sinf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);
	m_posV.y += (float)UtilFunc::Transformation::Random(-nLengthY, nLengthY);
	m_posV.z += (float)UtilFunc::Transformation::Random(-nLength, nLength) * cosf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);

	// 注視点の代入処理
	m_posR.x += (float)UtilFunc::Transformation::Random(-nLength, nLength) * sinf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);
	m_posR.y += (float)UtilFunc::Transformation::Random(-nLengthY, nLengthY);
	m_posR.z += (float)UtilFunc::Transformation::Random(-nLength, nLength) * cosf((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f);

	// 慣性補正
	UtilFunc::Correction::InertiaCorrection(m_nShakeLength, 0.0f, 0.1f);
	UtilFunc::Correction::InertiaCorrection(m_nShakeLengthY, 0.0f, 0.1f);
}


//==========================================================================
// リセット
//==========================================================================
void CCamera::Reset(CScene::MODE mode)
{
	// 操作の状態設定
	SetControlState(DEBUG_NEW CCameraControlState_Normal(this));

	m_bFollow = true;	// 追従するかどうか

	// カメラモーションの位置リセット
	if (m_pCameraMotion != nullptr)
	{
		m_pCameraMotion->SetPosition(0.0f);
	}

	// リセット
	ResetGame();


	// プロジェクションマトリックスの初期化
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f),
								(float)m_viewport.Width / (float)m_viewport.Height,
								10.0f,		// 奥行きの制限
								1500000.0f);	// 奥行きの制限

	// ビューマトリックスの初期化
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
}

//==========================================================================
// ゲームのリセット
//==========================================================================
void CCamera::ResetGame()
{
	m_posR = MyLib::Vector3(0.0f, 200.0f, 0.0f);				// 注視点(見たい場所)
	m_posV = MyLib::Vector3(0.0f, 300.0f, m_posR.z + -400.0f);	// 視点(カメラの位置)
	m_posVDest = m_posV;									// 目標の視点
	m_posRDest = m_posR;									// 目標の注視点
	m_vecU = MyLib::Vector3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	m_move = MyLib::Vector3(0.0f, 0.0f, 0.0f);				// 移動量
	m_rot = DEFAULT_GAMEROT;								// 向き
	m_rotOrigin = m_rot;									// 元の向き
	m_rotDest = DEFAULT_GAMEROT;							// 目標の向き
	m_TargetPos = MyLib::Vector3(0.0f, 0.0f, 0.0f);			// 目標の位置
	m_fDistance = START_CAMERALEN;							// 距離
	m_fDestDistance = START_CAMERALEN;						// 目標の距離
	m_fOriginDistance = START_CAMERALEN;					// 元の距離
	m_state = CAMERASTATE_NONE;								// 状態
	m_nCntState = 0;							// 状態カウンター
}

//==========================================================================
//  スクリーン内の判定
//==========================================================================
bool CCamera::IsOnScreen(const MyLib::Vector3& pos)
{
	// 返り値用の変数
	bool bIn = false;

	// 判定
	//ビューポートの設定
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//計算用変数宣言
	D3DXMATRIX mtxWorld; //ワールドマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//敵のスクリーン座標を算出
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
		bIn = true;
	}

	// 返す
	return bIn;
}

//==========================================================================
//  スクリーン座標取得
//==========================================================================
MyLib::Vector3 CCamera::GetScreenPos(const MyLib::Vector3& pos)
{
	//ビューポートの設定
	D3DVIEWPORT9 vp = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

	//計算用変数宣言
	D3DXMATRIX mtxWorld; //ワールドマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//スクリーン座標を算出
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

	// 返す
	return screenPos;
}

//==========================================================================
// 操作の状態設定
//==========================================================================
void CCamera::SetControlState(CCameraControlState* state)
{
	delete m_pControlState;
	m_pControlState = state;
}

//==========================================================================
// 基底の操作処理
//==========================================================================
void CCameraControlState::Controll(CCamera* pCamera)
{
	// ゲームパッド情報取得
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	MyLib::Vector3 rot = pCamera->GetRotation();

	// 横回転
	m_moveRot.y += pInputGamepad->GetStickMoveR(0).x * ROT_MOVE_STICK_Y;

	// 縦回転
	if (rot.z > MIN_STICKROT && pInputGamepad->GetStickMoveR(0).y < 0.0f)
	{
		m_moveRot.z += pInputGamepad->GetStickMoveR(0).y * ROT_MOVE_STICK_Z;
	}
	else if (rot.z <= MIN_STICKROT && pInputGamepad->GetStickMoveR(0).y > 0.0f)
	{
		m_moveRot.z += pInputGamepad->GetStickMoveR(0).y * ROT_MOVE_STICK_Z;
	}

	// 移動する
	rot += m_moveRot;

	// 0補正
	m_moveRot += (MyLib::Vector3(0.0f) - m_moveRot) * 0.25f;

	// 回転移動量設定
	pCamera->SetRotation(rot);
}

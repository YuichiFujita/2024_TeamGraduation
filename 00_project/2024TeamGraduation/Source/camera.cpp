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
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector3 DEFAULT_GAMEROT = MyLib::Vector3(0.0f, 0.0f, -0.32f);	// デフォルト向き
	const float MIN_NEAR	= 10.0f;		// 手前の描画制限
	const float MAX_FAR		= 1500000.0f;	// 奥の描画制限
	const float MIN_DISNTANCE	= 500.0f;	// 最少距離
	const float DISNTANCE		= 1160.0f;	// 距離
	const float MULTIPLY_CHASE_POSR	= 1.5f;	// 注視点追従の倍率
	const float MULTIPLY_CHASE_POSV	= 1.5f;	// 注視点追従の倍率
}

//==========================================================================
// コンストラクタ
//==========================================================================
CCamera::CCamera()
{
	m_pDebugControll	= nullptr;		// デバッグ情報
	m_pCameraMotion		= nullptr;		// カメラモーション情報
	m_pLight			= nullptr;		// ディレクショナルライト情報
	m_posV				= VEC3_ZERO;	// 視点
	m_posVDest			= VEC3_ZERO;	// 目標視点
	m_posR				= VEC3_ZERO;	// 注視点
	m_posRDest			= VEC3_ZERO;	// 目標注視点
	m_posROrigin		= VEC3_ZERO;	// 原点注視点
	m_vecU				= MyLib::Vector3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル
	m_move				= VEC3_ZERO;	// 移動量
	m_rot				= VEC3_ZERO;	// 向き
	m_rotDest			= VEC3_ZERO;	// 目標向き
	m_rotOrigin			= VEC3_ZERO;	// 原点向き
	m_posTarget			= VEC3_ZERO;	// 追従位置
	m_posTargetDest		= VEC3_ZERO;	// 目標追従位置
	m_fDistance			= 0.0f;			// 距離
	m_fDestDistance		= 0.0f;			// 目標距離
	m_fOriginDistance	= 0.0f;			// 原点距離
	m_bFollow			= false;		// 追従判定
	m_bMotion			= false;		// モーション中判定
	m_state				= STATE_NONE;	// 状態

	// マトリックス情報のクリア
	D3DXMatrixIdentity(&m_mtxProjection);	// プロジェクションマトリックス
	D3DXMatrixIdentity(&m_mtxView);			// ビューマトリックス

	// ビューポート情報のクリア
	m_viewport.X = 0;		// 画面の左上X座標
	m_viewport.Y = 0;		// 画面の左上Y座標
	m_viewport.Width = 0;	// 画面の幅
	m_viewport.Height = 0;	// 画面の高さ
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;
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

	// カメラリセット
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
	m_pLight->SetDiffuse(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

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

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// 向きの正規化
	UtilFunc::Transformation::RotNormalize(m_rot);
	UtilFunc::Transformation::RotNormalize(m_rotDest);
	UtilFunc::Transformation::RotNormalize(m_rotOrigin);

	// 注視点の反映
	ReflectCameraR();

	// 視点の反映
	ReflectCameraV();

	// スポットライトベクトルの更新
	UpdateSpotLightVec();

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

	// テキストの描画
	GET_MANAGER->GetDebugProc()->Print
	(
		"---------------- カメラ情報 ----------------\n"
		"【向き】[X：%f Y：%f Z：%f]\n"
		"【距離】[%f]\n"
		"【視点】[X：%f Y：%f Z：%f]\n"
		"【注視点】[X：%f Y：%f Z：%f]\n",
		m_rot.x, m_rot.y, m_rot.z,
		m_fDistance,
		m_posV.x, m_posV.y, m_posV.z,
		m_posR.x, m_posR.y, m_posR.z
	);

	// テキストの描画
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ
	(
		pMouse->GetPosition(),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		m_mtxView,
		m_mtxProjection
	);
	GET_MANAGER->GetDebugProc()->Print
	(
		"---------------- カメラ情報 ----------------\n"
		"【交差点】[X：%f Y：%f Z：%f]\n",
		pos.x, pos.y, pos.z
	);
}

//==========================================================================
// カメラの設定処理
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
		&m_mtxProjection,		// プロジェクションマトリックス
		D3DXToRadian(30.0f),	// 視野角
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
		&m_mtxView,	// ビューマトリックス
		&m_posV,	// 視点
		&m_posR,	// 注視点
		&m_vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//==========================================================================
// カメラリセット
//==========================================================================
void CCamera::Reset()
{
#if 1
	m_posR = MyLib::Vector3(0.0f, 200.0f, -560.0f);				// 注視点(見たい場所)
	m_posV = MyLib::Vector3(0.0f, 300.0f, m_posR.z + -400.0f);	// 視点(カメラの位置)
	m_posVDest = m_posV;				// 目標の視点
	m_posRDest = m_posR;				// 目標の注視点
	m_posROrigin = m_posR;				// 元の注視点
	m_rot = DEFAULT_GAMEROT;			// 向き
	m_rotOrigin = m_rot;				// 元の向き
	m_rotDest = m_rot;					// 目標の向き
	m_fDistance = DISNTANCE;			// 距離
	m_fDestDistance = m_fDistance;		// 目標の距離
	m_fOriginDistance = m_fDistance;	// 元の距離
#endif

	if (m_pCameraMotion != nullptr)
	{
		// カメラモーションの位置リセット
		m_pCameraMotion->SetPosition(0.0f);
	}

	// プロジェクションマトリックスの初期化
	float fAspect = (float)m_viewport.Width / (float)m_viewport.Height;	// アスペクト比
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,		// プロジェクションマトリックス
		D3DXToRadian(45.0f),	// 視野角
		fAspect,	// アスペクト比
		MIN_NEAR,	// 手前の描画制限
		MAX_FAR		// 奥の描画制限
	);

	// ビューマトリックスの初期化
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
}

//==========================================================================
// カメラワープ処理
//==========================================================================
void CCamera::WarpCamera(const MyLib::Vector3& pos)
{
	// 注視点の設定
	m_posR = pos;			// 注視点
	m_posRDest = m_posR;	// 目標の注視点

	// 視点の相対設定
	ReflectCameraV();		// 視点
	m_posVDest = m_posV;	// 目標の視点
}

//==========================================================================
// カメラの視点代入処理
//==========================================================================
void CCamera::ReflectCameraV()
{
	if (!m_bFollow)
	{ // 追従しないとき

		// 視点の代入処理
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else
	{ // 追従ON

		// 視点の代入処理
		m_posVDest.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;

		// 補正する
		m_posV += (m_posVDest - m_posV) * MULTIPLY_CHASE_POSV;
	}
}

//==========================================================================
// カメラの注視点代入処理
//==========================================================================
void CCamera::ReflectCameraR()
{
	if (!m_bFollow ||
		(m_bMotion && m_bFollow))
	{ // 追従しないとき

		//// 注視点の代入処理
		//m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		//m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		//m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{ // 追従ON

		// ターゲットみる
		m_posRDest = m_posTarget;

		// 補正する
		m_posR += (m_posRDest - m_posR) * MULTIPLY_CHASE_POSR;
	}
}

//==========================================================================
// スポットライトのベクトル更新
//==========================================================================
void CCamera::UpdateSpotLightVec()
{
	// 視点から注視点へのベクトルを計算
	MyLib::Vector3 vec = m_posR - m_posV;

	// スポットライトの方向設定
	m_pLight->SetDirection(vec);
}

//==========================================================================
// カメラの状態更新処理
//==========================================================================
void CCamera::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
		break;

	case STATE_FOLLOW:
		break;

	default:
		assert(false);
		break;
	}
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

//=============================================================================
// 
// カメラ処理 [camera.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "calculation.h"
#include "player.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define MOVE			(2.5f)				// 移動量
#define MAX_LENGTH		(20000.0f)			// 最大距離
#define MIN_LENGTH		(10.0f)				// 最少距離
#define ROT_MOVE_MOUSE	(0.01f)				// 回転移動量
#define ROT_MOVE_STICK	(0.0015f)			// 回転移動量
#define ROT_MOVE		(0.025f)			// 回転移動量
#define MIN_ROT			(-D3DX_PI * 0.49f)	// カメラ固定用
#define MAX_ROT			(D3DX_PI * 0.49f)	// カメラ固定用
#define BACKFOLLOW_TIME	(20)				// 背面補正までの時間
#define START_CAMERALEN	(300.0f)			// 元の距離

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
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点(見たい場所)
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 視点(カメラの位置)
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_rotVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の視点の向き
	m_fDistance = 0.0f;							// 距離
	m_nShakeLength = 0.0f;						// 揺れの長さ
	m_nShakeLengthY = 0.0f;						// Yの揺れの長さ
	m_fMoveShake = 0.0f;						// 揺れの移動量
	m_fMoveShakeY = 0.0f;						// Yの揺れの移動量
	m_bFollow = false;							// 追従するかどうか
	m_state = CAMERASTATE_NONE;					// 状態
	m_nCntState = 0;							// 状態カウンター
}

//==========================================================================
// デストラクタ
//==========================================================================
CCamera::~CCamera()
{

}

//==================================================================================
// カメラの初期化処理
//==================================================================================
HRESULT CCamera::Init(void)
{
	m_viewport.X = 0;										// 描画する画面の左上X座標
	m_viewport.Y = 0;										// 描画する画面の左上Y座標
	m_viewport.Width = SCREEN_WIDTH;						// 描画する画面の幅
	m_viewport.Height = SCREEN_HEIGHT;						// 描画する画面の高さ
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);				// 注視点(見たい場所)
	m_posV = D3DXVECTOR3(0.0f, 300.0f, m_posR.z + -400.0f);	// 視点(カメラの位置)
	m_posVDest = m_posV;									// 目標の視点
	m_posRDest = m_posR;									// 目標の注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					// 上方向ベクトル
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, -0.2f);					// 向き
	m_rotVDest = m_rot;										// 目標の視点の向き
	m_fDistance = START_CAMERALEN;							// 距離
	m_bFollow = false;										// 追従するかどうか
	m_state = CAMERASTATE_NONE;								// 状態

	// 視点の代入処理
	SetCameraV();

	return S_OK;
}

//==================================================================================
// カメラの終了処理
//==================================================================================
void CCamera::Uninit(void)
{

}

//==================================================================================
// カメラの更新処理
//==================================================================================
void CCamera::Update(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	//if (m_state == CAMERASTATE_NONE)
	{
		// 視点・注視点移動
		MoveCameraFollow();
		MoveCameraInput();
		MoveCameraVR();
		/*MoveCameraR();
		MoveCameraV();*/
		MoveCameraDistance();

		if (m_state == CAMERASTATE_SHAKE)
		{
			UpdateState();
		}
	}

//#ifdef _DEBUG
//#endif
}

//==================================================================================
// 入力機器のカメラ移動
//==================================================================================
void CCamera::MoveCameraInput(void)
{
	// マウスでの移動処理
	MoveCameraMouse();

	// スティックでの移動処理
	MoveCameraStick();
}

//==================================================================================
// コントローラーでの移動処理
//==================================================================================
void CCamera::MoveCameraStick(void)
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad* pInputGamepad = CManager::GetInputGamepad();
	m_rot.y += pInputGamepad->GetStickMoveR(0).x * ROT_MOVE_STICK;
	m_rot.z += pInputGamepad->GetStickMoveR(0).y * ROT_MOVE_STICK;

	// 角度の正規化
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// 視点の代入処理
	SetCameraV();
}

//==================================================================================
// マウスでの移動処理
//==================================================================================
void CCamera::MoveCameraMouse(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// キーボード情報取得
	CInputMouse *pInputMouse = CManager::GetInputMouse();

	if (!pInputKeyboard->GetPress(DIK_LALT) &&
		!pInputKeyboard->GetPress(DIK_RALT))
	{
		return;
	}

	if (pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) == true &&
		pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT) == true)
	{// 左右同時押し

//#if _DEBUG
		m_move.x += (pInputMouse->GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) -
			(pInputMouse->GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);

		m_move.z += (pInputMouse->GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE) +
			(pInputMouse->GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + m_rot.y) * MOVE);
//#endif

		// 移動量補正
		MoveCameraVR();

		// 角度の正規化
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// 注視点設定
		SetCameraR();
	}
	else if (pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) == true)
	{// 左クリックしてるとき,視点回転

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;

//#if _DEBUG

		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;	
//#endif

		// 角度の正規化
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// 値の正規化
		ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// 視点の代入処理
		SetCameraV();

	}
	else if (pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT) == true)
	{// 右クリックしてるとき,注視点回転

		m_rot.y += pInputMouse->GetMouseMove().x * ROT_MOVE_MOUSE;

//#if _DEBUG
		m_rot.z += pInputMouse->GetMouseMove().y * ROT_MOVE_MOUSE;
//#endif

		// 角度の正規化
		RotNormalize(m_rot.y);
		RotNormalize(m_rot.z);

		// 値の正規化
		ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

		// 注視点の位置更新
		SetCameraR();
	}

	// マウスホイールで距離調整
	m_fDistance += pInputMouse->GetMouseMove().z * (MOVE * 0.3f);
	MoveCameraDistance();

	// 視点の代入処理
	SetCameraV();
}

//==================================================================================
// カメラの視点移動
//==================================================================================
void CCamera::MoveCameraV(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

//#ifdef _DEBUG
	// 視点移動
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{// Yキーが押された,視点上移動

		m_rot.z -= ROT_MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_N) == true)
	{// Nキーが押された,視点下移動

		m_rot.z += ROT_MOVE;
	}
//#endif

	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{// Zキーが押された

		m_rot.y += ROT_MOVE;
	}
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{// Cキーが押された

		m_rot.y -= ROT_MOVE;
	}


	// 角度の正規化
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// 値の正規化
	ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// 視点の代入処理
	SetCameraV();

}

//==================================================================================
// カメラの注視点移動
//==================================================================================
void CCamera::MoveCameraR(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// 旋回
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{// Qキーが押された,左旋回


		m_rot.y -= ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_E) == true)
	{// Eキーが押された,左旋回

		m_rot.y += ROT_MOVE;
	}

//#ifdef _DEBUG
	// 上昇下降
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{// Tキーが押された

		m_rot.z += ROT_MOVE;

	}
	else if (pInputKeyboard->GetPress(DIK_B) == true)
	{// Bキーが押された

		m_rot.z -= ROT_MOVE;

	}
//#endif


	// 角度の正規化
	RotNormalize(m_rot.y);
	RotNormalize(m_rot.z);

	// 値の正規化
	ValueNormalize(m_rot.z, MAX_ROT, MIN_ROT);

	// 注視点の位置更新
	SetCameraR();
}

//==================================================================================
// カメラの視点・注視点移動
//==================================================================================
void CCamera::MoveCameraVR(void)
{

	// 移動量分を加算
	m_posV.x += m_move.x;
	m_posV.z += m_move.z;

	// 移動量をリセット
	m_move.x = 0.0f;
	m_move.z = 0.0f;

	// 注視点の位置更新
	SetCameraR();
}

//==================================================================================
//  視点・注視点間距離
//==================================================================================
void CCamera::MoveCameraDistance(void)
{

	// 限界値補正
	if (m_fDistance >= MAX_LENGTH)
	{
		m_fDistance = MAX_LENGTH;
	}
	else if (m_fDistance <= MIN_LENGTH)
	{
		m_fDistance = MIN_LENGTH;
	}

}

//==================================================================================
// カメラの視点代入処理
//==================================================================================
void CCamera::SetCameraV(void)
{

	if (m_bFollow == false)
	{// 追従しないとき

		// 視点の代入処理
		m_posV.x = m_posR.x + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fDistance;
	}
	else if(m_bFollow == true)
	{// 追従ON

		// プレイヤーの情報取得
		CPlayer *pPlayer = CManager::GetPlayer();

		if (pPlayer == NULL)
		{
			return;
		}

		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
		D3DXVECTOR3 PlayerRot = pPlayer->GetRotation();

		float fYcamera = 100.0f + (PlayerPos.y + 100.0f) - 100.0f;

		// 視点の代入処理
		m_posVDest.x = (PlayerPos.x + sinf(D3DX_PI + PlayerRot.y) * 90.0f) + cosf(m_rot.z) * sinf(m_rot.y) * -m_fDistance;
		m_posVDest.z = (PlayerPos.z + cosf(D3DX_PI + PlayerRot.y) * 90.0f) + cosf(m_rot.z) * cosf(m_rot.y) * -m_fDistance;
		m_posVDest.y = fYcamera + sinf(m_rot.z) * -m_fDistance;

		// 補正する
		m_posV += (m_posVDest - m_posV) * 0.12f;
	}

}

//==================================================================================
// カメラの注視点代入処理
//==================================================================================
void CCamera::SetCameraR(void)
{

	if (m_bFollow == false)
	{// 追従しないとき

		// 注視点の代入処理
		m_posR.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + sinf(m_rot.z) * m_fDistance;
	}
	else
	{// 追従ON

		// 注視点の代入処理
		m_posRDest.x = m_posV.x + cosf(m_rot.z) * sinf(m_rot.y) * m_fDistance;
		m_posRDest.z = m_posV.z + cosf(m_rot.z) * cosf(m_rot.y) * m_fDistance;
		m_posRDest.y = m_posV.y + sinf(m_rot.z) * m_fDistance;

		// 補正する
		m_posR += (m_posRDest - m_posR) * 0.08f;
	}

}

//==================================================================================
// カメラの背面自動追従
//==================================================================================
void CCamera::MoveCameraFollow(void)
{
	//float fRotDiff = 0.0f;	// 差分

	//// プレイヤーの情報取得
	//Player *pPlayer = GetPlayer();

	//if (m_bBackFollow == true)
	//{// 追従するとき

	//	m_rotVDest.y = (D3DX_PI + pPlayer->rot.y);

	//	if (m_bSPush == false)
	//	{// S系が押されてない

	//		m_nCntBackFollow = BACKFOLLOW_TIME;
	//	}
	//	else
	//	{// S系が押されてるとき

	//		// m_nCntBackFollow++;	// カウント加算
	//	}
	//	

	//	if (m_nCntBackFollow >= BACKFOLLOW_TIME)
	//	{// 背面補正のカウントが規定値超えたら

	//		m_nCntBackFollow = 0;

	//		// 補正する
	//		fRotDiff = (m_rotVDest.y - m_rot.y);

	//		// 角度の正規化
	//		RotNormalize(m_rot.y);
	//		RotNormalize(m_rot.z);

	//		if (fRotDiff > D3DX_PI)
	//		{
	//			fRotDiff += (-D3DX_PI * 2.0f);
	//		}
	//		else if (fRotDiff < -D3DX_PI)
	//		{
	//			fRotDiff += (D3DX_PI * 2.0f);
	//		}

	//		// 差分で補正する
	//		m_rot.y += fRotDiff * 0.015f;

	//		// 角度の正規化
	//		RotNormalize(m_rot.y);
	//		RotNormalize(m_rot.z);
	//	}
	//}
}

//==================================================================================
// カメラの設定処理
//==================================================================================
void CCamera::SetCamera(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),	// 視野角
								(float)m_viewport.Width / (float)m_viewport.Height,	// アスペクト比
								10.0f,		// 奥行きの制限
								20000.0f);	// 奥行きの制限

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


//==================================================================================
// カメラの振動設定処理
//==================================================================================
void CCamera::SetShake(int nTime, float fLength, float fLengthY)
{
	// 振動状態に設定
	m_state = CAMERASTATE_SHAKE;
	m_nCntState = nTime;		// 状態遷移カウンター
	m_nShakeLength = fLength;	// 揺れの大きさ
	m_nShakeLengthY = fLengthY;	// Yの揺れの大きさ
}

//==================================================================================
// カメラの振動処理
//==================================================================================
void CCamera::Shake(void)
{
	// 長さ取得
	int nLength = (int)m_nShakeLength;
	int nLengthY = (int)m_nShakeLengthY;

	m_fMoveShake = (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);						// 揺れの移動量
	m_fMoveShakeY = (float)Random(-nLengthY, nLengthY);						// Yの揺れの移動量

	// 視点の代入処理
	m_posV.x += (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);
	m_posV.y += (float)Random(-nLengthY, nLengthY);
	m_posV.z += (float)Random(-nLength, nLength) * cosf((float)Random(-314, 314) * 0.01f);

	// 注視点の代入処理
	m_posR.x += (float)Random(-nLength, nLength) * sinf((float)Random(-314, 314) * 0.01f);
	m_posR.y += (float)Random(-nLengthY, nLengthY);
	m_posR.z += (float)Random(-nLength, nLength) * cosf((float)Random(-314, 314) * 0.01f);

	// 慣性補正
	InertiaCorrection(m_nShakeLength, 0.0f, 0.1f);
	InertiaCorrection(m_nShakeLengthY, 0.0f, 0.1f);
}

//==================================================================================
// カメラの状態更新処理
//==================================================================================
void CCamera::UpdateState(void)
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
			InertiaCorrection(m_nShakeLength, 0.0f, 0.01f);
			InertiaCorrection(m_nShakeLengthY, 0.0f, 0.01f);
		}

		// 状態遷移カウンター減算
		m_nCntState--;

		if (m_nCntState <= 0)
		{
			m_nCntState = 0;
			m_state = CAMERASTATE_NONE;
		}
		break;
	}
}

//==========================================================================
// 向き設定
//==========================================================================
void CCamera::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// 向き取得
//==========================================================================
D3DXVECTOR3 CCamera::GetRotation(void) const
{
	return m_rot;
}

void CCamera::EnableChase(void)
{
	m_bFollow = m_bFollow ? false : true;
}

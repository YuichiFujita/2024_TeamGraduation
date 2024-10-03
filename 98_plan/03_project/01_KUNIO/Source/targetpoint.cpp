//=============================================================================
// 
//  目標地点処理 [targetpoint.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "targetpoint.h"
#include "input.h"
#include "calculation.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "elevation.h"
#include "game.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TEXTURE		"data\\TEXTURE\\gradation004.jpg"
#define MOVE_SPEED		(1.7f)							// 移動速度

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CTargetPoint::CTargetPoint(int nPriority) : CObject3DMesh(nPriority)
{
	m_nTexIdx = 0;	// テクスチャのインデックス番号
	m_nAlphaCnt = 0;	// 不透明度のカウント
}

//==========================================================================
// デストラクタ
//==========================================================================
CTargetPoint::~CTargetPoint()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTargetPoint *CTargetPoint::Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen)
{
	// 生成用のオブジェクト
	CTargetPoint *pObjMeshCylinder = nullptr;

	if (pObjMeshCylinder == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjMeshCylinder = DEBUG_NEW CTargetPoint;

		//if (pObjMeshCylinder->GetID() < 0)
		//{// メモリ確保に失敗していたら

		//	delete pObjMeshCylinder;
		//	return nullptr;
		//}

		if (pObjMeshCylinder != nullptr)
		{// メモリの確保が出来ていたら

			// 位置・向き
			pObjMeshCylinder->SetPosition(pos);
			pObjMeshCylinder->SetWidthBlock(16);
			pObjMeshCylinder->SetHeightBlock(1);
			pObjMeshCylinder->SetWidthLen(fWidthLen);
			pObjMeshCylinder->SetHeightLen(fHeightLen);

			// テクスチャの割り当て
			pObjMeshCylinder->m_nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);

			// テクスチャの割り当て
			pObjMeshCylinder->BindTexture(pObjMeshCylinder->m_nTexIdx);

			// 初期化処理
			pObjMeshCylinder->Init();
		}

		return pObjMeshCylinder;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTargetPoint::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 種類設定
	SetType(CObject::TYPE_OBJECT3D);

	// オブジェクト3Dメッシュの初期化処理
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_CYLINDER);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTargetPoint::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTargetPoint::Update()
{

	// 色取得
	D3DXCOLOR col = GetColor();

	// 二次曲線補正
	UtilFunc::Correction::CuadricCurveComp(col.a, 100, 0.3f, 1.0f, m_nAlphaCnt);
	//col.a = 0.3f;

	// 色設定
	SetColor(col);

	Control();

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 操作
//==========================================================================
void CTargetPoint::Control()
{

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// カメラの向き取得
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	if (pInputKeyboard->GetPress(DIK_A) || pInputGamepad->GetStickMoveL(0).x < 0)
	{//←キーが押された,左移動

		if (pInputKeyboard->GetPress(DIK_W) || pInputGamepad->GetStickMoveL(0).y > 0)
		{//A+W,左上移動
			move.x += sinf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else if (pInputKeyboard->GetPress(DIK_S) || pInputGamepad->GetStickMoveL(0).y < 0)
		{//A+S,左下移動
			move.x += sinf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else
		{//A,左移動
			move.x += sinf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) || pInputGamepad->GetStickMoveL(0).x > 0)
	{//Dキーが押された,右移動

		if (pInputKeyboard->GetPress(DIK_W) || pInputGamepad->GetStickMoveL(0).y > 0)
		{//D+W,右上移動
			move.x += sinf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else if (pInputKeyboard->GetPress(DIK_S) || pInputGamepad->GetStickMoveL(0).y < 0)
		{//D+S,右下移動
			move.x += sinf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
		else
		{//D,右移動
			move.x += sinf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
			move.z += cosf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) || pInputGamepad->GetStickMoveL(0).y > 0)
	{//Wが押された、上移動
		move.x += sinf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * MOVE_SPEED;
		move.z += cosf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * MOVE_SPEED;
	}
	else if (pInputKeyboard->GetPress(DIK_S) || pInputGamepad->GetStickMoveL(0).y < 0)
	{//Sが押された、下移動
		move.x += sinf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * MOVE_SPEED;
		move.z += cosf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * MOVE_SPEED;
	}


	// 重力処理
	move.y -= mylib_const::GRAVITY;

	// 位置更新
	pos += move;

	// 慣性補正
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	// 高さ取得
	bool bLand = false;
	pos.y = CGame::GetInstance()->GetElevation()->GetHeight(pos, &bLand);

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);
}


//==========================================================================
// 描画処理
//==========================================================================
void CTargetPoint::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 背面のカリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject3DMesh::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングデフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CTargetPoint::SetVtx()
{
	// 頂点情報更新
	CObject3DMesh::SetVtx();
}

//==========================================================================
// 目標地点オブジェクトの取得
//==========================================================================
CTargetPoint *CTargetPoint::GetObject3DMesh()
{
	return this;
}

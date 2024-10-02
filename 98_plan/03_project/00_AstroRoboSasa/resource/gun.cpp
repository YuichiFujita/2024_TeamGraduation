//============================================================
//
//	銃処理 [gun.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gun.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"

//************************************************************
//	子クラス [CGun] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGun::CGun() : CObject3D(CObject::LABEL_GUN),
	m_pParent(nullptr)	// 親の情報
{

}

//============================================================
//	デストラクタ
//============================================================
CGun::~CGun()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGun::Init(void)
{
	// メンバ変数を初期化
	m_pParent = nullptr;	// 親の情報

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	SetVec3Sizing(D3DXVECTOR3(17.0f, 7.0f, 0.0f));
	SetOrigin(ORIGIN_DOWN);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGun::Uninit(void)
{
	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CGun::Update(const float fDeltaTime)
{
	// TODO：銃発砲操作
	if (GET_INPUTMOUSE->IsTrigger(CInputMouse::KEY_LEFT))
	{
		float fGunRot = m_pParent->GetGunRotation().z;
		CBullet::Create(m_pParent->GetVec3Position(), D3DXVECTOR3(sinf(fGunRot + HALF_PI), -cosf(fGunRot + HALF_PI), 0.0f) * 29.0f);
		m_pParent->MoveGunRecoil(11.5f);
	}

	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CGun::Draw(CShader *pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CGun *CGun::Create(CPlayer* pParent)
{
	// 銃の生成
	CGun *pGun = new CGun;
	if (pGun == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 銃の初期化
		if (FAILED(pGun->Init()))
		{ // 初期化に失敗した場合

			// 銃の破棄
			SAFE_DELETE(pGun);
			return nullptr;
		}

		// 親プレイヤーの保存
		pGun->m_pParent = pParent;

		// 確保したアドレスを返す
		return pGun;
	}
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CGun::CalcDrawMatrix(void)
{
	D3DXVECTOR3 offset = D3DXVECTOR3(35.0f, 0.0f, 0.0f);	// オフセット
	D3DXMATRIX *pMtxWorld = GetPtrMtxWorld();			// ワールドマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxOffset;				// 計算用マトリックス
	D3DXVECTOR3 pos = m_pParent->GetVec3Position();		// プレイヤー位置
	D3DXVECTOR3 rot = m_pParent->GetVec3Rotation();		// プレイヤー向き
	D3DXVECTOR3 rotGun = m_pParent->GetGunRotation();	// 銃構え向き

	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 off = D3DXVECTOR3(0.0f, 15.0f, 0.0f);	// オフセット

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// オフセットを反映
	D3DXMatrixTranslation(&mtxOffset, off.x, off.y, off.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxOffset);

	// プレイヤー向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// プレイヤーの中心座標を計算
	pos = useful::GetMatrixPosition(mtxWorld);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtxWorld);

	// オフセットを反映
	D3DXMatrixTranslation(&mtxOffset, offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxOffset);

	// プレイヤー向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// 銃向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotGun.y, rotGun.x, rotGun.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);
}

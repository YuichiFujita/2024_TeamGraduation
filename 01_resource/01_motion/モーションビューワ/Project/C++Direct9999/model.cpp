//=============================================================================
// 
//  モデル処理 [model.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "calculation.h"
#include "texture.h"
#include "Xload.h"

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CModel::m_nNumAll = 0;	// 総数

//==========================================================================
// コンストラクタ
//==========================================================================
CModel::CModel(int nPriority)
{
	D3DXMatrixIdentity(&m_mtxWorld);				// ワールドマトリックス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_scale = mylib_const::DEFAULT_SCALE;			// スケール
	m_nIdxTexure = NULL;							// テクスチャのインデックス番号
	m_nIdxXFile = 0;								// Xファイルのインデックス番号
	m_nIdxParent = -1;		// 親のインデックス番号
	m_pParent = NULL;								// 親モデルのポインタ
	m_pModel = this;								// 自分のポインタ
	m_nNumAll++;									// 総数加算
}

//==========================================================================
// デストラクタ
//==========================================================================
CModel::~CModel()
{

}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CModel::BindTexture(int *nIdx)
{
	// 割り当てる
	m_nIdxTexure = nIdx;
}

//==========================================================================
// Xファイルのデータ割り当て
//==========================================================================
void CModel::BindXData(int nIdxXFile)
{
	// 情報割り当て
	m_nIdxXFile = nIdxXFile;
}

//==========================================================================
// 親のマトリックス設定
//==========================================================================
void CModel::SetParent(CModel *pModel, int nIdx)
{
	// 親のポインタを渡す
	m_pParent = pModel;
	m_nIdxParent = nIdx;
}

//==========================================================================
// 生成処理
//==========================================================================
CModel *CModel::Create(const char *pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 生成用のオブジェクト
	CModel *pModel = NULL;

	if (pModel == NULL)
	{// NULLだったら

		// メモリの確保
		pModel = DEBUG_NEW CModel;

		if (pModel != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pModel->Init(pFileName);

			if (FAILED(hr))
			{// 失敗していたら
				return NULL;
			}

			// 位置・向き
			pModel->SetPosition(pos);
			pModel->m_posOrigin = pos;
			pModel->SetOriginPotision(pos);
			pModel->SetRotation(rot);
			pModel->SetOriginRotation(rot);
		}

		return pModel;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CModel::Init(const char *pFileName)
{
	// Xファイルのデータ取得
	CXLoad *pXLoad = CManager::GetXLoad();

	// Xファイルのロード
	m_nIdxXFile = pXLoad->XLoad(pFileName);

	// Xファイルのデータ割り当て
	BindXData(m_nIdxXFile);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// 全頂点チェック
	CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	// スケールリセット
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_modelname = extractFileName(pFileName);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CModel::Uninit(void)
{
	// テクスチャのインデックス番号
	if (m_nIdxTexure != NULL)
	{
		m_nIdxTexure = NULL;
	}

	// 親モデルのポインタ
	if (m_pParent != NULL)
	{
		m_pParent = NULL;
	}

	if (m_pModel != NULL)
	{// メモリの確保がされていたら

		// メモリの開放
		delete m_pModel;
		m_pModel = NULL;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CModel::Update(void)
{
	// Xファイルのデータ取得
	CXLoad *pXLoad = CManager::GetXLoad();

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// 全頂点チェック
	CheckVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
}

//==========================================================================
// 描画処理
//==========================================================================
void CModel::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マトリックス計算
	CalWorldMtx();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// 頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 自動正規化をデフォルトにする
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================================
// 描画処理
//==========================================================================
void CModel::Draw(D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マトリックス計算
	CalWorldMtx();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATERIAL matNow;			// 今回のマテリアル

	// 他の情報クリア
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = col;
	matNow.MatD3D.Ambient = col;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// 頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&matNow.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 自動正規化をデフォルトにする
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================================
// 描画処理
//==========================================================================
void CModel::Draw(float fAlpha)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マトリックス計算
	CalWorldMtx();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATERIAL matNow;			// 今回のマテリアル

	// 他の情報クリア
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));


	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CManager::GetXLoad()->GetObjectX(m_nIdxXFile);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// 頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// 不透明度設定
		matNow.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, fAlpha);
		matNow.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, fAlpha);
		matNow.MatD3D.Emissive = pMat[nCntMat].MatD3D.Emissive;
		matNow.MatD3D.Power = pMat[nCntMat].MatD3D.Power;
		matNow.MatD3D.Specular = pMat[nCntMat].MatD3D.Specular;

		//マテリアルの設定
		pDevice->SetMaterial(&matNow.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(pXData->nIdxTexture[nCntMat]));

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 自動正規化をデフォルトにする
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================================
// ワールドマトリックス計算
//==========================================================================
void CModel::CalWorldMtx(void)
{
#if 1
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxRotOrigin;	// 計算用マトリックス宣言
	D3DXMATRIX mtxnParent;			// 親のマトリックス

	bool bScale = false;

	// 親マトリックスの初期化
	D3DXMatrixIdentity(&mtxRotOrigin);
	D3DXMatrixIdentity(&mtxnParent);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映する
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 元の向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRotOrigin, m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);

	// 向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotOrigin);

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親のマトリックスと掛け合わせる
	if (m_pParent == NULL)
	{// 自分が親だった場合

		// 最新のマトリックスを渡す(DirectXが覚えているもの)
		pDevice->GetTransform(D3DTS_WORLD, &mtxnParent);
	}
	else
	{// 親がいる場合

		// 親のマトリックスを渡す
		mtxnParent = m_pParent->GetWorldMtx();

		D3DXVECTOR3 scaleVector;

		// マトリックスからスケール情報を抽出
		scaleVector.x = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._11, mtxnParent._12, mtxnParent._13));
		scaleVector.y = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._21, mtxnParent._22, mtxnParent._23));
		scaleVector.z = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._31, mtxnParent._32, mtxnParent._33));
		if (scaleVector != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{
			bScale = true;
		}
	}

	// 自分に親のワールドマトリックスを掛ける
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxnParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	{// 少しでも違う場合

		bScale = true;
	}

	if (bScale)
	{
		// 自動正規化をONにする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	}
#else

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxRotOrigin;	// 計算用マトリックス宣言
	D3DXMATRIX mtxnParent;			// 親のマトリックス

	// クォータニオン変数の追加
	D3DXQUATERNION m_quaternionOrigin;
	D3DXQUATERNION m_quaternion;
	D3DXQuaternionIdentity(&m_quaternionOrigin);
	D3DXQuaternionIdentity(&m_quaternion);

	bool bScale = false;

	// 親マトリックスの初期化
	D3DXMatrixIdentity(&mtxRotOrigin);
	D3DXMatrixIdentity(&mtxnParent);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 元の向きを反映する
	D3DXQuaternionRotationYawPitchRoll(&m_quaternionOrigin, m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);
	D3DXQuaternionNormalize(&m_quaternionOrigin, &m_quaternionOrigin);  // クォータニオンを正規化

	D3DXQUATERNION quatX;
	D3DXQuaternionRotationAxis(&quatX, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), m_rot.x);

	D3DXQUATERNION quatY;
	D3DXQuaternionRotationAxis(&quatY, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_rot.y);

	D3DXQUATERNION quatZ;
	D3DXQuaternionRotationAxis(&quatZ, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), m_rot.z);

	// x軸、y軸、z軸の順で回転を適用
	m_quaternion = quatX * m_quaternion;
	m_quaternion = quatY * m_quaternion;
	m_quaternion = quatZ * m_quaternion;

	// クォータニオンを正規化
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);  // クォータニオンを正規化

	// クォータニオンを組み合わせて回転マトリックスを作成
	//D3DXMATRIX mtxRot, mtxRotOrigin;
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxRotOrigin);
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);
	D3DXMatrixRotationQuaternion(&mtxRotOrigin, &m_quaternionOrigin);

	// スケールを反映する
	//D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// 位置を反映する
	//D3DXMATRIX mtxTrans;
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// ワールドマトリックスの計算
	m_mtxWorld = mtxScale * mtxRot * mtxRotOrigin * mtxTrans;

	// 親のマトリックスと掛け合わせる
	if (m_pParent == NULL)
	{// 自分が親だった場合

		// 最新のマトリックスを渡す(DirectXが覚えているもの)
		pDevice->GetTransform(D3DTS_WORLD, &mtxnParent);
	}
	else
	{// 親がいる場合

		// 親のマトリックスを渡す
		mtxnParent = m_pParent->GetWorldMtx();

		D3DXVECTOR3 scaleVector;

		// マトリックスからスケール情報を抽出
		scaleVector.x = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._11, mtxnParent._12, mtxnParent._13));
		scaleVector.y = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._21, mtxnParent._22, mtxnParent._23));
		scaleVector.z = D3DXVec3Length(&D3DXVECTOR3(mtxnParent._31, mtxnParent._32, mtxnParent._33));
		if (scaleVector != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
		{
			bScale = true;
		}
	}

	// 自分に親のワールドマトリックスを掛ける
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxnParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_scale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	{// 少しでも違う場合

		bScale = true;
	}

	if (bScale)
	{
		// 自動正規化をONにする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	}

#endif

}

//==========================================================================
// マトリックス設定
//==========================================================================
void CModel::SetWorldMtx(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// マトリックス取得
//==========================================================================
D3DXMATRIX CModel::GetWorldMtx(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// 位置設定
//==========================================================================
void CModel::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CModel::GetPosition(void) const
{
	return m_pos;
}

//==========================================================================
// 位置設定
//==========================================================================
void CModel::SetOldPosition(const D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CModel::GetOldPosition(void) const
{
	return m_posOld;
}

//==========================================================================
// 元の位置設定
//==========================================================================
void  CModel::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 元の位置取得
//==========================================================================
D3DXVECTOR3  CModel::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// 向き設定
//==========================================================================
void CModel::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// 向き取得
//==========================================================================
D3DXVECTOR3 CModel::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// 元の向き設定
//==========================================================================
void CModel::SetOriginRotation(const D3DXVECTOR3 rot)
{
	m_rotOrigin = rot;
}

//==========================================================================
// 元の向き取得
//==========================================================================
D3DXVECTOR3 CModel::GetOriginRotation(void) const
{
	return m_rotOrigin;
}

//==========================================================================
// 元の位置設定
//==========================================================================
void CModel::SetOriginPotision(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 元の位置取得
//==========================================================================
D3DXVECTOR3 CModel::GetOriginPotision(void) const
{
	return m_posOrigin;
}

//==========================================================================
// スケール設定
//==========================================================================
void CModel::SetScale(const D3DXVECTOR3 scale)
{
	m_scale = scale;
}

//==========================================================================
// スケール取得
//==========================================================================
D3DXVECTOR3 CModel::GetScale(void) const
{
	return m_scale;
}

//==========================================================================
// 頂点の最大値取得
//==========================================================================
D3DXVECTOR3 CModel::GetVtxMax(void) const
{
	// Xファイルのデータ取得
	return CManager::GetXLoad()->GetObjectX(m_nIdxXFile)->vtxMax;
}

//==========================================================================
// 頂点の最小値取得
//==========================================================================
D3DXVECTOR3 CModel::GetVtxMin(void) const
{
	// Xファイルのデータ取得
	return CManager::GetXLoad()->GetObjectX(m_nIdxXFile)->vtxMin;
}

//==========================================================================
// モデルオブジェクトの取得
//==========================================================================
CModel *CModel::GetModel(void)
{
	return this;
}

//==========================================================================
// 総数取得
//==========================================================================
int CModel::GetNumAll(void)
{
	return m_nNumAll;
}

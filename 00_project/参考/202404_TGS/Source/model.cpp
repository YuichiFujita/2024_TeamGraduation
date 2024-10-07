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
#include "light.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CModel::m_nNumAll = 0;	// 総数

//==========================================================================
// コンストラクタ
//==========================================================================
CModel::CModel(int nPriority)
{
	m_mtxParent = nullptr;							// 親マトリックスのポインタ
	m_pos = mylib_const::DEFAULT_VECTOR3;			// 位置
	m_posOld = mylib_const::DEFAULT_VECTOR3;		// 前回の位置
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;		// 元の位置
	m_rot = mylib_const::DEFAULT_VECTOR3;			// 向き
	m_rotOrigin = mylib_const::DEFAULT_VECTOR3;		// 向き
	m_scale = mylib_const::DEFAULT_SCALE;			// スケール
	m_scaleOrigin = mylib_const::DEFAULT_SCALE;			// スケール
	m_nIdxXFile = 0;								// Xファイルのインデックス番号
	m_nIdxTexture = nullptr;							// テクスチャのインデックス番号
	m_pParent = nullptr;								// 親モデルのポインタ
	m_nNumAll++;									// 総数加算
}

//==========================================================================
// デストラクタ
//==========================================================================
CModel::~CModel()
{

}

//==========================================================================
// Xファイルのデータ割り当て
//==========================================================================
void CModel::BindXData(int nIdxXFile)
{
	// 情報割り当て
	m_nIdxXFile = nIdxXFile;

	// テクスチャ割り当て
	BindTexture();
}

//==========================================================================
// テクスチャ割り当て
//==========================================================================
void CModel::BindTexture()
{
	if (m_nIdxTexture != nullptr)
	{
		delete[] m_nIdxTexture;
	}

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// マテリアル分メモリ確保
	m_nIdxTexture = DEBUG_NEW int[(int)pXData->dwNumMat];

	// テクスチャのインデックス番号
	for (int i = 0; i < (int)pXData->dwNumMat; i++)
	{
		m_nIdxTexture[i] = pXData->nIdxTexture[i];
	}
}

//==========================================================================
// 親のマトリックス設定
//==========================================================================
void CModel::SetParent(CModel *pModel)
{
	// 親のポインタを渡す
	m_pParent = pModel;
}

void CModel::SetMtxParent(MyLib::Matrix* pMtx)
{ 
	m_mtxParent = pMtx;
}


//==========================================================================
// 生成処理
//==========================================================================
CModel *CModel::Create(const std::string& file, MyLib::Vector3 pos, MyLib::Vector3 rot)
{
	// 生成用のオブジェクト
	CModel *pModel = nullptr;

	if (pModel == nullptr)
	{// nullptrだったら

		// メモリの確保
		pModel = DEBUG_NEW CModel;

		if (pModel == nullptr)
		{// nullptrだったら

			delete pModel;
			pModel = nullptr;
			return nullptr;
		}

		if (pModel != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pModel->Init(file);

			if (FAILED(hr))
			{// 失敗していたら
				return nullptr;
			}

			// 位置・向き
			pModel->SetPosition(pos);
			pModel->m_posOrigin = pos;
			pModel->SetRotation(rot);
			pModel->SetOriginRotation(rot);
		}

		return pModel;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CModel::Init(const std::string& file)
{
	// Xファイルのデータ取得
	CXLoad *pXLoad = CXLoad::GetInstance();

	// Xファイルのロード
	m_nIdxXFile = pXLoad->XLoad(file);

	// Xファイルのデータ割り当て
	BindXData(m_nIdxXFile);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// 全頂点チェック
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	// テクスチャ割り当て
	BindTexture();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CModel::Uninit()
{
	if (m_nIdxTexture != nullptr)
	{
		delete[] m_nIdxTexture;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CModel::Update()
{
	// Xファイルのデータ取得
	CXLoad *pXLoad = CXLoad::GetInstance();

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// 全頂点チェック
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
}

//==========================================================================
// ワールドマトリックス計算
//==========================================================================
void CModel::CalWorldMtx()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxScaleOrigin, mtxRotOrigin;	// 計算用マトリックス宣言
	MyLib::Matrix mtxParent;			// 親のマトリックス

	bool bScale = false;

	// 親マトリックスの初期化
	mtxRotOrigin.Identity();
	mtxParent.Identity();

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// 元のスケールを反映
	mtxScaleOrigin.Scaling(m_scaleOrigin);
	m_mtxWorld.Multiply(m_mtxWorld, mtxScaleOrigin);

	// スケールを反映する
	mtxScale.Scaling(m_scale);
	m_mtxWorld.Multiply(m_mtxWorld, mtxScale);

	// 元の向きを反映する
	mtxRotOrigin.RotationYawPitchRoll(m_rotOrigin.y, m_rotOrigin.x, m_rotOrigin.z);

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(m_rot.y, m_rot.x, m_rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRotOrigin);

	// 位置を反映する
	mtxTrans.Translation(m_pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

	// 親のマトリックスと掛け合わせる
	mtxParent = *m_mtxParent;

	if (m_pParent != nullptr &&
		mtxParent.GetScale() != MyLib::Vector3(1.0f, 1.0f, 1.0f))
	{// 親がいる場合

		bScale = true;
	}

	// 自分に親のワールドマトリックスを掛ける
	m_mtxWorld.Multiply(m_mtxWorld, mtxParent);

	// ワールドマトリックスの設定
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
	{// 少しでも違う場合

		bScale = true;
	}

	if (bScale)
	{
		// 自動正規化をONにする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	}

}

//==========================================================================
// シャドウマトリックスの描画
//==========================================================================
void CModel::DrawShadowMtx()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mtxShadow;
	D3DXVECTOR4 posLight;	// ライトの位置
	MyLib::Vector3 pos, normal;	// 平面上の任意の点,法線ベクトル
	D3DXPLANE plane;		// 平面情報
	D3DLIGHT9 light;		// ライト情報
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	
	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	//ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	//シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// 平面情報を生成
	pos = MyLib::Vector3(0.0f, 3.0f, 0.0f);
	normal = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// シャドウマトリックスの作成
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	D3DXMatrixMultiply(&mtxShadow, &mtx, &mtxShadow);

	// シャドウマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	D3DXMATERIAL matNow;	// 今回のマテリアル

	// 他の情報クリア
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	matNow.MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&matNow.MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// 減算合成をもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================
// 描画処理
//==========================================================================
void CModel::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言
	D3DXMATRIX mtxRotOrigin;		// 計算用マトリックス宣言
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxnParent;			// 親の計算用マトリックス

	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// 頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nIdxTexture[nCntMat]));

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);
	}

	// シャドウマトリックス描画
	DrawShadowMtx();

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// 描画処理
//==========================================================================
void CModel::Draw(D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATERIAL matNow;			// 今回のマテリアル

	// 他の情報クリア
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));
	matNow.MatD3D.Diffuse = col;
	matNow.MatD3D.Ambient = col;

	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// 頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&matNow.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nIdxTexture[nCntMat]));

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);
	}


	// シャドウマトリックス描画
	DrawShadowMtx();

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// 描画処理
//==========================================================================
void CModel::Draw(float fAlpha)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言
	D3DXMATRIX mtxRotOrigin;		// 計算用マトリックス宣言
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxnParent;			// 親の計算用マトリックス

	D3DXMATERIAL matNow;			// 今回のマテリアル

	// 他の情報クリア
	ZeroMemory(&matNow, sizeof(D3DXMATERIAL));

	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

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
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nIdxTexture[nCntMat]));

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);
	}


	// シャドウマトリックス描画
	DrawShadowMtx();

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// テクスチャのインデックス割り当て
//==========================================================================
void CModel::SetIdxTexture(int i, int nIdx)
{
	m_nIdxTexture[i] = nIdx;
}

//==========================================================================
// テクスチャインデックス番号割り当て
//==========================================================================
int CModel::GetIdxTexture(int nIdx)
{
	return m_nIdxTexture[nIdx];
}

//==========================================================================
// マトリックス設定
//==========================================================================
void CModel::SetWorldMtx(const MyLib::Matrix mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// マトリックス取得
//==========================================================================
MyLib::Matrix CModel::GetWorldMtx()
{
	return m_mtxWorld;
}

//==========================================================================
// ポインタマトリックス取得
//==========================================================================
MyLib::Matrix *CModel::GetPtrWorldMtx()
{
	return &m_mtxWorld;
}

//==========================================================================
// 位置設定
//==========================================================================
void CModel::SetPosition(const MyLib::Vector3 pos)
{
	m_pos = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CModel::GetPosition() const
{
	return m_pos;
}

//==========================================================================
// 位置設定
//==========================================================================
void CModel::SetOldPosition(const MyLib::Vector3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CModel::GetOldPosition() const
{
	return m_posOld;
}

//==========================================================================
// 元の位置設定
//==========================================================================
void  CModel::SetOriginPosition(const MyLib::Vector3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 元の位置取得
//==========================================================================
MyLib::Vector3  CModel::GetOriginPosition() const
{
	return m_posOrigin;
}

//==========================================================================
// 向き設定
//==========================================================================
void CModel::SetRotation(const MyLib::Vector3 rot)
{
	m_rot = rot;
}

//==========================================================================
// 向き取得
//==========================================================================
MyLib::Vector3 CModel::GetRotation() const
{
	return m_rot;
}

//==========================================================================
// 元の向き設定
//==========================================================================
void CModel::SetOriginRotation(const MyLib::Vector3 rot)
{
	m_rotOrigin = rot;
}

//==========================================================================
// 元の向き取得
//==========================================================================
MyLib::Vector3 CModel::GetOriginRotation() const
{
	return m_rotOrigin;
}

//==========================================================================
// スケール設定
//==========================================================================
void CModel::SetScale(const MyLib::Vector3 scale)
{
	m_scale = scale;
}

//==========================================================================
// スケール取得
//==========================================================================
MyLib::Vector3 CModel::GetScale() const
{
	return m_scale;
}

//==========================================================================
// 頂点の最大値取得
//==========================================================================
MyLib::Vector3 CModel::GetVtxMax() const
{
	// Xファイルのデータ取得
	return CXLoad::GetInstance()->GetMyObject(m_nIdxXFile)->vtxMax;
}

//==========================================================================
// 頂点の最小値取得
//==========================================================================
MyLib::Vector3 CModel::GetVtxMin() const
{
	// Xファイルのデータ取得
	return CXLoad::GetInstance()->GetMyObject(m_nIdxXFile)->vtxMin;
}

//==========================================================================
// モデルオブジェクトの取得
//==========================================================================
CModel *CModel::GetModel()
{
	return this;
}

//==========================================================================
// 総数取得
//==========================================================================
int CModel::GetNumAll()
{
	return m_nNumAll;
}

//=============================================================================
// 
//  オブジェクトX処理 [objectX.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectX.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "texture.h"
#include "shadow.h"
#include "elevation.h"
#include "collisionLine_Box.h"
#include "handle_Move.h"

//==========================================================================
// 関数ポインタ
//==========================================================================
CObjectX::STATE_FUNC CObjectX::m_StateFunc[] =
{
	&CObjectX::StateNone,	// なにもなし
	&CObjectX::StateEdit,	// エディット
};

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CObjectX::m_nNumAll = 0;	// 総数
CListManager<CObjectX> CObjectX::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectX::CObjectX(int nPriority, CObject::LAYER layer) : CObject(nPriority, layer)
{
	m_mtxWorld.Identity();							// ワールドマトリックス
	m_scale = MyLib::Vector3(1.0f, 1.0f, 1.0f);		// スケール
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_fSize = MyLib::Vector3();						// サイズ
	m_AABB = MyLib::AABB();							// AABB情報
	m_OriginAABB = MyLib::AABB();					// 元のAABB情報
	m_state = STATE::STATE_NONE;					// 状態
	m_bShadow = false;								// 影を使っているかどうか
	m_nIdxTexure = 0;								// テクスチャのインデックス番号
	m_nIdxXFile = 0;								// Xファイルのインデックス番号
	m_pShadow = nullptr;							// 影の情報
	m_pCollisionLineBox = nullptr;					// 当たり判定ボックス
	m_nNumAll++;									// 総数加算
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectX::~CObjectX()
{

}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CObjectX::BindTexture(int *nIdx)
{
	// 割り当てる
	m_nIdxTexure = *nIdx;
}

//==========================================================================
// Xファイルのデータ割り当て
//==========================================================================
void CObjectX::BindXData(int nIdxXFile)
{
	// 情報割り当て
	m_nIdxXFile = nIdxXFile;
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectX *CObjectX::Create()
{
	// 生成用のオブジェクト
	CObjectX *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjectX = DEBUG_NEW CObjectX;

		if (pObjectX != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pObjectX->Init();
		}

		return pObjectX;
	}

	return nullptr;
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectX *CObjectX::Create(const std::string& file)
{
	// 生成用のオブジェクト
	CObjectX *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjectX = DEBUG_NEW CObjectX;

		if (pObjectX != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pObjectX->Init(file);

			if (FAILED(hr))
			{// 失敗していたら
				return nullptr;
			}

			// 種類設定
			pObjectX->SetType(TYPE_XFILE);
		}

		return pObjectX;
	}

	return nullptr;
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectX *CObjectX::Create(const std::string& file, const MyLib::Vector3& pos, const MyLib::Vector3& rot, bool bShadow)
{
	// 生成用のオブジェクト
	CObjectX *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjectX = DEBUG_NEW CObjectX;

		if (pObjectX != nullptr)
		{// メモリの確保が出来ていたら

			// 位置・向き
			pObjectX->SetPosition(pos);
			pObjectX->SetRotation(rot);
			pObjectX->m_bShadow = bShadow;

			// 初期化処理
			HRESULT hr = pObjectX->Init(file);

			if (FAILED(hr))
			{// 失敗していたら
				return nullptr;
			}

			if (bShadow)
			{
				// 影設定
				float f = pObjectX->GetVtxMax().x * 0.5f;
				pObjectX->m_pShadow = CShadow::Create(pObjectX->GetPosition(), f);
			}
		}

		return pObjectX;
	}

	return nullptr;
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectX *CObjectX::Create(int nIdxXFile, const MyLib::Vector3& pos, const MyLib::Vector3& rot, bool bShadow,
	int nPriority,
	CObject::LAYER layer)
{
	// 生成用のオブジェクト
	CObjectX *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjectX = DEBUG_NEW CObjectX(nPriority, layer);

		if (pObjectX != nullptr)
		{// メモリの確保が出来ていたら

			// 位置・向き
			pObjectX->SetPosition(pos);
			pObjectX->SetRotation(rot);
			pObjectX->m_bShadow = bShadow;

			// 初期化処理
			HRESULT hr = pObjectX->Init(nIdxXFile);

			if (FAILED(hr))
			{// 失敗していたら
				return nullptr;
			}

			// 種類設定
			//pObjectX->SetType(TYPE_XFILE);

			if (bShadow)
			{
				// 影設定
				float f = pObjectX->GetVtxMax().x * 0.5f;
				pObjectX->m_pShadow = CShadow::Create(pObjectX->GetPosition(), f);
			}
		}

		return pObjectX;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectX::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Xファイルのデータ取得
	CXLoad *pXLoad = CXLoad::GetInstance();

	m_List.Regist(this);

	// Xファイルのロード
	m_nIdxXFile = pXLoad->XLoad("data\\MODEL\\radiokaikan_01.x");

	// Xファイルのデータ割り当て
	BindXData(m_nIdxXFile);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// 全頂点チェック
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
	m_AABB.vtxMin = pXData->vtxMin;
	m_AABB.vtxMax = pXData->vtxMax;
	m_pMesh = pXData->pMesh;

	// 元のAABB情報
	m_OriginAABB = m_AABB;

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectX::Init(const std::string& file)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Xファイルのデータ取得
	CXLoad *pXLoad = CXLoad::GetInstance();

	m_List.Regist(this);

	// Xファイルのロード
	m_nIdxXFile = pXLoad->XLoad(file);
	m_fileName = file;

	// Xファイルのデータ割り当て
	BindXData(m_nIdxXFile);

	if (m_nIdxXFile < 0)
	{
		return S_OK;
	}

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// 全頂点チェック
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
	m_AABB.vtxMin = pXData->vtxMin;
	m_AABB.vtxMax = pXData->vtxMax;
	m_pMesh = pXData->pMesh;

	// 元のAABB情報
	m_OriginAABB = m_AABB;

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectX::Init(int nIdxXFile)
{
	// Xファイルのデータ割り当て
	BindXData(nIdxXFile);

	m_List.Regist(this);

	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// Xファイルのデータ取得
	CXLoad* pXLoad = CXLoad::GetInstance();
	m_fileName = pXLoad->GetMyObject(nIdxXFile)->filename;

	// 全頂点チェック
	UtilFunc::Calculation::CalModelVtx(GetRotation().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);
	m_AABB.vtxMin = pXData->vtxMin;
	m_AABB.vtxMax = pXData->vtxMax;
	m_pMesh = pXData->pMesh;

	// 元のAABB情報
	m_OriginAABB = m_AABB;

	return S_OK;
}


//==========================================================================
// 終了処理
//==========================================================================
void CObjectX::Uninit()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		//m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	m_List.Delete(this);

	m_nIdxTexure = 0;

	// オブジェクトの破棄
	Release();

	m_nNumAll--;	// 総数減算
}

//==========================================================================
// 死亡処理
//==========================================================================
void CObjectX::Kill()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	m_List.Delete(this);

	if (m_pCollisionLineBox != nullptr) {
		m_pCollisionLineBox->Kill();
		m_pCollisionLineBox = nullptr;
	}

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectX::Update()
{
	// 過去の向き設定
	SetOldRotation(GetRotation());

	// 状態別処理
	(this->*(m_StateFunc[m_state]))();

	if (m_pShadow != nullptr){
		m_pShadow->SetPosition(GetPosition());
	}


	// 当たり判定ボックスデータ設定
	SetCollisionBoxData();
}

//==========================================================================
// なにもなし
//==========================================================================
void CObjectX::StateNone()
{
	if (m_pCollisionLineBox != nullptr) {
		m_pCollisionLineBox->SetEnableDisp(false);
	}
}

//==========================================================================
// エディット状態
//==========================================================================
void CObjectX::StateEdit()
{
	if (m_pCollisionLineBox != nullptr) {
		m_pCollisionLineBox->SetEnableDisp(true);
	}
}

//==========================================================================
// 当たり判定ボックス生成
//==========================================================================
void CObjectX::CreateCollisionBox()
{
#if _DEBUG
	// 当たり判定ボックス生成
	m_pCollisionLineBox = CCollisionLine_Box::Create(m_AABB, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	// 当たり判定ボックスデータ設定
	SetCollisionBoxData();
#endif
}

//==========================================================================
// 当たり判定ボックスデータ設定
//==========================================================================
void CObjectX::SetCollisionBoxData()
{
	if (m_pCollisionLineBox != nullptr) {

		MyLib::AABB aabb;
		aabb.vtxMin.x = m_OriginAABB.vtxMin.x * m_scale.x;
		aabb.vtxMin.y = m_OriginAABB.vtxMin.y * m_scale.y;
		aabb.vtxMin.z = m_OriginAABB.vtxMin.z * m_scale.z;

		aabb.vtxMax.x = m_OriginAABB.vtxMax.x * m_scale.x;
		aabb.vtxMax.y = m_OriginAABB.vtxMax.y * m_scale.y;
		aabb.vtxMax.z = m_OriginAABB.vtxMax.z * m_scale.z;

		m_pCollisionLineBox->SetAABB(aabb);

		// 位置・向き設定
		m_pCollisionLineBox->SetPosition(GetPosition());
		m_pCollisionLineBox->SetRotation(GetRotation());
	}
}

//==========================================================================
// 高さ取得
//==========================================================================
float CObjectX::GetHeight(MyLib::Vector3 pos, bool &bLand)
{
	// Xファイルのデータ取得
	CXLoad::SXFile *pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	if (pXData == nullptr)
	{// nullptrだったら
		return 0.0f;
	}

	// 最大の高さ
	float fHeightMax = 0.0f;

	// 自分の位置
	MyLib::Vector3 MyPosition = GetPosition();

	WORD* pIndexBuff;

	// インデックスバッファをロック
	pXData->pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	for (int nCntTri = 0; nCntTri < pXData->nFaceNum; nCntTri++) 
	{
		
		int nIdx1 = (int)pIndexBuff[nCntTri * 3];
		int nIdx2 = (int)pIndexBuff[nCntTri * 3 + 1];
		int nIdx3 = (int)pIndexBuff[nCntTri * 3 + 2];

		// 頂点位置代入
		MyLib::Vector3 pos1 = pXData->pVtxPos[nIdx1] + MyPosition;
		MyLib::Vector3 pos2 = pXData->pVtxPos[nIdx2] + MyPosition;
		MyLib::Vector3 pos3 = pXData->pVtxPos[nIdx3] + MyPosition;

		// 絶対値(マイナスいかない)と最大の距離比較
		if (fabsf(pos1.x - pos.x) > pXData->fMaxVtxDistance ||
			fabsf(pos1.z - pos.z) > pXData->fMaxVtxDistance)
		{// 頂点距離の方が長い
			continue;
		}

		// 三角で高さを求める
		
		bool bNowLand = false;
		float fNowHeight = UtilFunc::Calculation::GetVtxHeight(pos, pos1, pos3, pos2, &bNowLand);

		if (bNowLand && fNowHeight > fHeightMax)
		{// 着地してたら

			// 最大高さ更新
			fHeightMax = fNowHeight;
			bLand = true;
		}
	}

	// インデックスバッファをアンロック
	pXData->pMesh->UnlockIndexBuffer();

	return fHeightMax;
}

//==========================================================================
// ワールドマトリックスの計算処理
//==========================================================================
void CObjectX::CalWorldMtx()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Matrix mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス宣言

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// スケールを反映する
	mtxScale.Scaling(m_scale);
	m_mtxWorld.Multiply(m_mtxWorld, mtxScale);

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// 位置を反映する
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

	// ワールドマトリックスの設定
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
}

//==========================================================================
// 描画のみ
//==========================================================================
void CObjectX::DrawOnly()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// Xファイルのデータ取得
	CXLoad::SXFile* pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// 頂点数分繰り返し
	CTexture* pTex = CTexture::GetInstance();
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTex->GetAdress(pXData->nIdxTexture[nCntMat]));

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// 少しでも違う場合

			// 自動正規化をONにする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// 少しでも違う場合

			// 自動正規化をデフォルトにする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectX::Draw()
{

	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// 描画のみ
	DrawOnly();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectX::Draw(D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	D3DXMATERIAL matNow;	// 今回のマテリアル

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
	CTexture *pTex = CTexture::GetInstance();
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&matNow.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTex->GetAdress(pXData->nIdxTexture[nCntMat]));

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// 少しでも違う場合

			// 自動正規化をONにする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// 少しでも違う場合

			// 自動正規化をデフォルトにする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectX::Draw(float fAlpha)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
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
		pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(pXData->nIdxTexture[nCntMat]));

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// 少しでも違う場合

			// 自動正規化をONにする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// パーツの描画
		pXData->pMesh->DrawSubset(nCntMat);

		if (m_scale != MyLib::Vector3(1.0f, 1.0f, 1.0f))
		{// 少しでも違う場合

			// 自動正規化をデフォルトにする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================
// マトリックス設定
//==========================================================================
void CObjectX::SetWorldMtx(const MyLib::Matrix mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// マトリックス取得
//==========================================================================
MyLib::Matrix CObjectX::GetWorldMtx() const
{
	return m_mtxWorld;
}

//==========================================================================
// スケール設定
//==========================================================================
void CObjectX::SetScale(const MyLib::Vector3 scale)
{
	m_scale = scale;
}

//==========================================================================
// スケール取得
//==========================================================================
MyLib::Vector3 CObjectX::GetScale() const
{
	return m_scale;
}

//==========================================================================
// 色設定
//==========================================================================
void CObjectX::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CObjectX::GetColor() const
{
	return m_col;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CObjectX::SetSize(const MyLib::Vector3 size)
{
	m_fSize = size;
}

//==========================================================================
// サイズ取得
//==========================================================================
MyLib::Vector3 CObjectX::GetSize() const
{
	return m_fSize;
}

//==========================================================================
// 頂点の最大値取得
//==========================================================================
MyLib::Vector3 CObjectX::GetVtxMax() const
{
	return m_AABB.vtxMax;
}

//==========================================================================
// 頂点の最小値取得
//==========================================================================
MyLib::Vector3 CObjectX::GetVtxMin() const
{
	return m_AABB.vtxMin;
}

//==========================================================================
// AABB情報取得
//==========================================================================
MyLib::AABB CObjectX::GetAABB() const
{
	return m_AABB;
}

//==========================================================================
// Xファイルのインデックス取得
//==========================================================================
int CObjectX::GetIdxXFile() const
{
	return m_nIdxXFile;
}

//==========================================================================
// 影を使っているかどうか
//==========================================================================
bool CObjectX::GetUseShadow() const
{
	return m_bShadow;
}

//==========================================================================
// オブジェクトXオブジェクトの取得
//==========================================================================
CObjectX *CObjectX::GetObjectX()
{
	return this;
}

//==========================================================================
// 総数取得
//==========================================================================
int CObjectX::GetNumAll()
{
	return m_nNumAll;
}

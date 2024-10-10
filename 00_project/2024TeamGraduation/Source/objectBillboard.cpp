//=============================================================================
// 
//  ビルボード処理 [objectBillboard.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectBillboard.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int POLYGON_TOP = 4;	// ポリゴンの頂点数
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject(nPriority)
{
	m_mtxWorld.Identity();			// ワールドマトリックス
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_size = D3DXVECTOR2(0.0f, 0.0f);			// サイズ
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);		// 元のサイズ
	m_fLength = 0.0f;							// 対角線の長さ
	m_fAngle = 0.0f;							// 対角線の向き
	m_nTexIdx = 0;	// テクスチャのインデックス番号
	m_pVtxBuff = nullptr;		// 頂点バッファ
	m_vecUV.clear();			// テクスチャ座標
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectBillboard::~CObjectBillboard()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectBillboard* CObjectBillboard::Create(int nPriority)
{
	// メモリの確保
	CObjectBillboard* pObjBillBoard = DEBUG_NEW CObjectBillboard(nPriority);

	if (pObjBillBoard != nullptr)
	{
		// 初期化処理
		pObjBillBoard->Init();
	}

	return pObjBillBoard;
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectBillboard* CObjectBillboard::Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot)
{
	// メモリの確保
	CObjectBillboard* pObjBillBoard = DEBUG_NEW CObjectBillboard;

	if (pObjBillBoard != nullptr)
	{// メモリの確保が出来ていたら

		// 位置・向き
		pObjBillBoard->SetPosition(pos);
		pObjBillBoard->SetRotation(rot);

		// 初期化処理
		pObjBillBoard->Init();
	}

	return pObjBillBoard;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectBillboard::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (m_pVtxBuff != nullptr)
	{// 既に情報が入ってる場合

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// テクスチャ座標
	m_vecUV.resize(POLYGON_TOP);
	m_vecUV[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_vecUV[1] = D3DXVECTOR2(1.0f, 0.0f);
	m_vecUV[2] = D3DXVECTOR2(0.0f, 1.0f);
	m_vecUV[3] = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectBillboard::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクトの破棄
	Release();

}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectBillboard::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectBillboard::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 計算用マトリックス宣言
	MyLib::Matrix mtxRot, mtxTrans;

	// 情報取得
	MyLib::Vector3 pos = GetPosition();

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// ビューマトリックスを取得
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMATRIX mtxInvert;
	D3DXMatrixInverse(&mtxInvert, nullptr, &mtxView);	// 逆配列を求める

	m_mtxWorld = mtxInvert;

#if 0

	// X軸に沿ってる
	m_mtxWorld._12 = 0.0f;
	m_mtxWorld._13 = 0.0f;
	m_mtxWorld._14 = 0.0f;

#else

	// 上で消える
	/*m_mtxWorld._21 = 0.0f;
	m_mtxWorld._23 = 0.0f;
	m_mtxWorld._24 = 0.0f;*/

#endif

	// 通常
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映する
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

	// ワールドマトリックスの設定
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObjectBillboard::SetVtx()
{

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = GetSize();
	D3DXCOLOR col = GetColor();
	MyLib::Vector3 rot = GetRotation();

	// 頂点座標の設定
	pVtx[0].pos.x = sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.y = cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.y = cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[2].pos.y = cosf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[3].pos.y = cosf(rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;


	// 法線ベクトルの設定
	pVtx[0].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// テクスチャ座標の設定
	pVtx[0].tex = m_vecUV[0];
	pVtx[1].tex = m_vecUV[1];
	pVtx[2].tex = m_vecUV[2];
	pVtx[3].tex = m_vecUV[3];

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//==========================================================================
// サイズ設定
//==========================================================================
void CObjectBillboard::SetSize(const D3DXVECTOR2& size)
{
	m_size = size;		// サイズ
	SetLength(sqrtf(m_size.x * m_size.x + m_size.y * m_size.y));	// 対角線の長さ
	SetAngle(atan2f(m_size.x, m_size.y));							// 対角線の向き
}
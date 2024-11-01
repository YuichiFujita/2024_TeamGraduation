//=============================================================================
// 
//  ビルボード処理 [objectBillboard.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define POLYGON_TOP		(4)		// ポリゴンの頂点数

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectBillBoard::CObjectBillBoard(int nPriority) : CObject(nPriority)
{
	D3DXMatrixIdentity(&m_mtxWorld);				// ワールドマトリックス
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 位置
	SetOldPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 前回の位置
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 移動量
	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
	SetSize(D3DXVECTOR2(0.0f, 0.0f));				// サイズ
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色
	m_nTexIdx = 0;	// テクスチャのインデックス番号
	m_pVtxBuff = NULL;		// 頂点バッファ

}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectBillBoard::~CObjectBillBoard()
{

}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CObjectBillBoard::BindTexture(int nIdx)
{
	// 割り当てる
	m_nTexIdx = nIdx;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectBillBoard::Init(void)
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (m_pVtxBuff != NULL)
	{// 既に情報が入ってる場合

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (FAILED(hr))
	{// 失敗したとき

		return E_FAIL;
	}

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectBillBoard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectBillBoard::Update(void)
{
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectBillBoard::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ビューマトリックス取得用
	D3DXMATRIX mtxView;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	// 逆配列を求める

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
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(m_nTexIdx));

	// ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectBillBoard *CObjectBillBoard::Create(void)
{
	// 生成用のオブジェクト
	CObjectBillBoard *pObjBillBoard = NULL;

	if (pObjBillBoard == NULL)
	{// NULLだったら

		// メモリの確保
		pObjBillBoard = DEBUG_NEW CObjectBillBoard;

		if (pObjBillBoard->GetID() < 0)
		{// メモリ確保に失敗していたら

			delete pObjBillBoard;
			return NULL;
		}

		if (pObjBillBoard != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pObjBillBoard->Init();
		}

		return pObjBillBoard;
	}

	return NULL;
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectBillBoard *CObjectBillBoard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 生成用のオブジェクト
	CObjectBillBoard *pObjBillBoard = NULL;

	if (pObjBillBoard == NULL)
	{// NULLだったら

		// メモリの確保
		pObjBillBoard = DEBUG_NEW CObjectBillBoard;

		if (pObjBillBoard->GetID() < 0)
		{// メモリ確保に失敗していたら

			delete pObjBillBoard;
			return NULL;
		}

		if (pObjBillBoard != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pObjBillBoard->Init();

			// 位置・向き
			pObjBillBoard->SetPosition(pos);
			pObjBillBoard->SetRotation(rot);
		}

		return pObjBillBoard;
	}

	return NULL;
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObjectBillBoard::SetVtx(void)
{

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = GetSize();
	D3DXCOLOR col = GetColor();

	// 位置を更新
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//==========================================================================
// マトリックス設定
//==========================================================================
void CObjectBillBoard::SetWorldMtx(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// マトリックス取得
//==========================================================================
D3DXMATRIX CObjectBillBoard::GetWorldMtx(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// 位置設定
//==========================================================================
void CObjectBillBoard::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObjectBillBoard::GetPosition(void) const
{
	return m_pos;
}

//==========================================================================
// 位置設定
//==========================================================================
void CObjectBillBoard::SetOldPosition(const D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObjectBillBoard::GetOldPosition(void) const
{
	return m_posOld;
}

//==========================================================================
// 移動量設定
//==========================================================================
void CObjectBillBoard::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//==========================================================================
// 移動量取得
//==========================================================================
D3DXVECTOR3 CObjectBillBoard::GetMove(void) const
{
	return m_move;
}

//==========================================================================
// 向き設定
//==========================================================================
void CObjectBillBoard::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// 向き取得
//==========================================================================
D3DXVECTOR3 CObjectBillBoard::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// 色設定
//==========================================================================
void CObjectBillBoard::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CObjectBillBoard::GetColor(void) const
{
	return m_col;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CObjectBillBoard::SetSize(const D3DXVECTOR2 size)
{
	m_fSize = size;
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CObjectBillBoard::GetSize(void) const
{
	return m_fSize;
}

//==========================================================================
// 頂点バッファの取得
//==========================================================================
LPDIRECT3DVERTEXBUFFER9 CObjectBillBoard::GetVtxBuff(void) const
{
	return m_pVtxBuff;
}

//==========================================================================
// ビルボードオブジェクトの取得
//==========================================================================
CObjectBillBoard *CObjectBillBoard::GetObjectBillBoard(void)
{
	return this;
}

//=============================================================================
// 
//  オブジェクト2D処理 [object2D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object2D.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "texture.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TEXTURE_NAME	"data\\TEXTURE\\titlelog_00.png"	// テクスチャ名
#define POLYGON_TOP		(4)								// ポリゴンの頂点数
#define WIDTH			(80.0f)							// 横幅
#define HEIGHT			(60.0f)							// 縦幅
#define ANIM_SPEED		(10)							// 読み込み間隔
#define MAX_PATTERN_U	(5)								// Uの分割数
#define MAX_PATTERN_V	(2)								// Vの分割数
#define MAX_PATTERN		(MAX_PATTERN_U * MAX_PATTERN_V)
#define MOVE_U			(1.0f / (float)MAX_PATTERN_U)	// U座標移動量
#define MOVE_V			(1.0f / (float)MAX_PATTERN_V)	// V座標移動量

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = NULL;		// 頂点バッファ
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 位置
	SetOldPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 前回の位置
	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 移動量
	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き
	SetLength(0.0f);								// 対角線の長さ
	SetAngle(0.0f);									// 対角線の向き
	SetSize(D3DXVECTOR2(0.0f, 0.0f));				// サイズ
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色
	m_nTexIdx = 0;	// テクスチャのインデックス番号
}

//==========================================================================
// デストラクタ
//==========================================================================
CObject2D::~CObject2D()
{

}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CObject2D::BindTexture(int nIdx)
{
	// 割り当てる
	m_nTexIdx = nIdx;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject2D::Init(void)
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (m_pVtxBuff != NULL)
	{// 既に情報が入ってる場合

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
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
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObject2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexture()->GetAdress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// 生成処理
//==========================================================================
CObject2D *CObject2D::Create(void)
{
	// 生成用のオブジェクト
	CObject2D *pObject2D = NULL;

	if (pObject2D == NULL)
	{// NULLだったら

		// メモリの確保
		pObject2D = DEBUG_NEW CObject2D;

		if (pObject2D->GetID() < 0)
		{// メモリ確保に失敗していたら

			delete pObject2D;
			return NULL;
		}

		if (pObject2D != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pObject2D->Init();
		}

		return pObject2D;
	}

	return NULL;
}

//==========================================================================
// 生成処理
//==========================================================================
CObject2D *CObject2D::Create(int nPriority)
{
	// 生成用のオブジェクト
	CObject2D *pObject2D = NULL;

	if (pObject2D == NULL)
	{// NULLだったら

		// メモリの確保
		pObject2D = DEBUG_NEW CObject2D(nPriority);

		if (pObject2D->GetID() < 0)
		{// メモリ確保に失敗していたら

			delete pObject2D;
			return NULL;
		}

		if (pObject2D != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pObject2D->Init();
		}

		return pObject2D;
	}

	return NULL;
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject2D::SetVtx(void)
{

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXCOLOR col = GetColor();
	float fAngle = GetAngle();
	float fLength = GetLength();

	// 頂点座標の設定
	pVtx[0].pos.x = pos.x + sinf(rot.z - D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
// 位置設定
//==========================================================================
void CObject2D::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObject2D::GetPosition(void) const
{
	return m_pos;
}

//==========================================================================
// 位置設定
//==========================================================================
void CObject2D::SetOldPosition(const D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObject2D::GetOldPosition(void) const
{
	return m_posOld;
}

//==========================================================================
// 移動量設定
//==========================================================================
void CObject2D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//==========================================================================
// 移動量取得
//==========================================================================
D3DXVECTOR3 CObject2D::GetMove(void) const
{
	return m_move;
}

//==========================================================================
// 向き設定
//==========================================================================
void CObject2D::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// 向き取得
//==========================================================================
D3DXVECTOR3 CObject2D::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// 色設定
//==========================================================================
void CObject2D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CObject2D::GetColor(void) const
{
	return m_col;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CObject2D::SetSize(const D3DXVECTOR2 size)
{
	m_fSize = size;		// サイズ
	SetLength(sqrtf(m_fSize.x * m_fSize.x + m_fSize.y * m_fSize.y));	// 対角線の長さ
	SetAngle(atan2f(m_fSize.x, m_fSize.y));								// 対角線の向き
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CObject2D::GetSize(void) const
{
	return m_fSize;
}

//==========================================================================
// 対角線の長さ設定
//==========================================================================
void CObject2D::SetLength(const float fLength)
{
	m_fLength = fLength;
}

//==========================================================================
// 対角線の長さ取得
//==========================================================================
float CObject2D::GetLength(void) const
{
	return m_fLength;
}

//==========================================================================
// 対角線の向き取得
//==========================================================================
void CObject2D::SetAngle(const float fAngle)
{
	m_fAngle = fAngle;
}

//==========================================================================
// 対角線の向き取得
//==========================================================================
float CObject2D::GetAngle(void) const
{
	return m_fAngle;
}

//==========================================================================
// 頂点バッファの取得
//==========================================================================
LPDIRECT3DVERTEXBUFFER9 CObject2D::GetVtxBuff(void) const
{
	return m_pVtxBuff;
}

//==========================================================================
// オブジェクト2Dオブジェクトの取得
//==========================================================================
CObject2D *CObject2D::GetObject2D(void)
{
	return this;
}

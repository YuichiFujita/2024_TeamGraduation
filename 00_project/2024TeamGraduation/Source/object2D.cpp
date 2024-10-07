//=============================================================================
// 
//  オブジェクト2D処理 [object2D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object2D.h"
#include "manager.h"
#include "pause.h"
#include "2D_effect.h"
#include "debugproc.h"

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
CObject2D::CObject2D(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_size = D3DXVECTOR2(0.0f, 0.0f);			// サイズ
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);		// 元のサイズ
	m_fLength = 0.0f;							// 対角線の長さ
	m_fAngle = 0.0f;							// 対角線の向き
	m_pVtxBuff = nullptr;							// 頂点バッファ
	m_nTexIdx = 0;								// テクスチャのインデックス番号
	m_AnchorType = AnchorPoint::CENTER;			// アンカーポイントの種類

	for (int nCntVtx = 0; nCntVtx < 32; nCntVtx++)
	{
		m_fTex[nCntVtx] = D3DXVECTOR2(0.0f, 0.0f);			// テクスチャ座標
		m_VtxPos[nCntVtx] = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 頂点座標
	}

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
// 生成処理
//==========================================================================
CObject2D *CObject2D::Create()
{
	// メモリの確保
	CObject2D* pObject2D = DEBUG_NEW CObject2D;

	if (pObject2D != nullptr)
	{
		// 初期化処理
		pObject2D->Init();
	}

	return pObject2D;
}

//==========================================================================
// 生成処理
//==========================================================================
CObject2D* CObject2D::Create(int nPriority)
{
	// メモリの確保
	CObject2D* pObject2D = DEBUG_NEW CObject2D(nPriority);

	if (pObject2D != nullptr)
	{
		// 初期化処理
		pObject2D->Init();
	}

	return pObject2D;
}

//==========================================================================
// 生成処理
//==========================================================================
CObject2D* CObject2D::Create(int nPriority, int nNumVtx)
{

	// メモリの確保
	CObject2D* pObject2D = DEBUG_NEW CObject2D(nPriority);

	if (pObject2D != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		pObject2D->Init(nNumVtx);
	}

	return pObject2D;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject2D::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_fTex[0] = D3DXVECTOR2(0.0f, 0.0f);	// テクスチャ座標
	m_fTex[1] = D3DXVECTOR2(1.0f, 0.0f);	// テクスチャ座標
	m_fTex[2] = D3DXVECTOR2(0.0f, 1.0f);	// テクスチャ座標
	m_fTex[3] = D3DXVECTOR2(1.0f, 1.0f);	// テクスチャ座標

	// 頂点バッファの生成
	if (m_pVtxBuff != nullptr)
	{// 既に情報が入ってる場合

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr))
	{// 失敗したとき

		return E_FAIL;
	}

	// 頂点情報設定
	SetVtx();

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject2D::Init(int nNumVertex)
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_fTex[0] = D3DXVECTOR2(0.0f, 0.0f);	// テクスチャ座標
	m_fTex[1] = D3DXVECTOR2(1.0f, 0.0f);	// テクスチャ座標
	m_fTex[2] = D3DXVECTOR2(0.0f, 1.0f);	// テクスチャ座標
	m_fTex[3] = D3DXVECTOR2(1.0f, 1.0f);	// テクスチャ座標

	// 頂点バッファの生成
	if (m_pVtxBuff != nullptr)
	{// 既に情報が入ってる場合

		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr))
	{// 失敗したとき

		return E_FAIL;
	}

	// 頂点情報設定
	SetVtx(nNumVertex);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObject2D::Uninit()
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
void CObject2D::Update()
{
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObject2D::Draw()
{
#if _DEBUG
	if (CManager::GetInstance()->GetPause() != nullptr)
	{
		if (CManager::GetInstance()->GetPause()->IsPause()) return;
		if (CManager::GetInstance()->Is2DDisp()) return;
	}
#endif

	// マネージャのインスタンス取得
	CManager* pMgr = CManager::GetInstance();

	if (GetType() == CObject::TYPE::TYPE_UI && !pMgr->IsDisp_UI())
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pMgr->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// 描画処理(マルチテクスチャ)
//==========================================================================
void CObject2D::Draw(LPDIRECT3DTEXTURE9 mutitex)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));
	pDevice->SetTexture(1, mutitex);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObject2D::Draw(int nNumVertex)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject2D::SetVtx()
{

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	D3DXCOLOR col = GetColor();

	// アンカーポイントの設定
	float anchorX = pos.x;
	float anchorY = pos.y;

	D3DXVECTOR2 size = GetSize();

	// センター以外は計算
	if (m_AnchorType != AnchorPoint::CENTER)
	{
		float distanceX = size.x, distanceY = size.y;
		switch (m_AnchorType)
		{
		case CObject2D::LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y;
			break;

		case CObject2D::TOP_LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y + distanceY;
			break;

		case CObject2D::TOP_CENTER:
			anchorX = pos.x;
			anchorY = pos.y + distanceY;
			break;

		case CObject2D::TOP_RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y + distanceY;
			break;

		case CObject2D::RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y;
			break;

		case CObject2D::UNDER_RIGHT:
			anchorX = pos.x - distanceX;
			anchorY = pos.y - distanceY;
			break;

		case CObject2D::UNDER_CENTER:
			anchorX = pos.x;
			anchorY = pos.y - distanceY;
			break;

		case CObject2D::UNDER_LEFT:
			anchorX = pos.x + distanceX;
			anchorY = pos.y - distanceY;
			break;

		default:
			anchorX = pos.x;
			anchorY = pos.y;
			break;
		}
	}

	// 頂点座標の設定
	if (m_AnchorType == AnchorPoint::CENTER)
	{
		float fAngle = GetAngle();
		float fLength = GetLength();

		pVtx[0].pos.x = anchorX + sinf(rot.z - D3DX_PI + fAngle) * fLength;
		pVtx[0].pos.y = anchorY + cosf(rot.z - D3DX_PI + fAngle) * fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = anchorX + sinf(rot.z + D3DX_PI - fAngle) * fLength;
		pVtx[1].pos.y = anchorY + cosf(rot.z + D3DX_PI - fAngle) * fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = anchorX + sinf(rot.z - fAngle) * fLength;
		pVtx[2].pos.y = anchorY + cosf(rot.z - fAngle) * fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = anchorX + sinf(rot.z + fAngle) * fLength;
		pVtx[3].pos.y = anchorY + cosf(rot.z + fAngle) * fLength;
		pVtx[3].pos.z = 0.0f;
	}
	else
	{
		pVtx[0].pos = RotateVtx(MyLib::Vector3(anchorX - size.x, anchorY - size.y, 0.0f), pos);
		pVtx[1].pos = RotateVtx(MyLib::Vector3(anchorX + size.x, anchorY - size.y, 0.0f), pos);
		pVtx[2].pos = RotateVtx(MyLib::Vector3(anchorX - size.x, anchorY + size.y, 0.0f), pos);
		pVtx[3].pos = RotateVtx(MyLib::Vector3(anchorX + size.x, anchorY + size.y, 0.0f), pos);

		pVtx[0].pos.z = pVtx[1].pos.z = pVtx[2].pos.z = pVtx[3].pos.z = 0.0f;
	}

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
	pVtx[0].tex = m_fTex[0];
	pVtx[1].tex = m_fTex[1];
	pVtx[2].tex = m_fTex[2];
	pVtx[3].tex = m_fTex[3];

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//==========================================================================
// 頂点回転処理
//==========================================================================
MyLib::Vector3 CObject2D::RotateVtx(const MyLib::Vector3& vtx, const MyLib::Vector3& center)
{
	MyLib::Matrix mtxRot;
	MyLib::Matrix mtxWepon;
	mtxWepon.Identity();

	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 pos = GetPosition();

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	mtxWepon.Multiply(mtxWepon, mtxRot);

	// オフセットを反映する
	MyLib::Vector3 offset(vtx.x - pos.x, vtx.y - pos.y, 0.0f);

	// オフセットに回転行列を適用
	offset = mtxRot.Coord(offset);

	return (offset + center);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void  CObject2D::SetVtx(int nNumVertex)
{
	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	D3DXCOLOR col = GetColor();
	MyLib::Vector3 *pVtxPos = GetVtxPos();

	// 頂点座標の設定
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = pos.x + pVtxPos[nCntVtx].x;
		pVtx[nCntVtx].pos.y = pos.y + pVtxPos[nCntVtx].y;
		pVtx[nCntVtx].pos.z = 0.0f;

		// rhwの設定
		pVtx[nCntVtx].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[nCntVtx].col = col;

		// テクスチャ座標の設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
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
D3DXCOLOR CObject2D::GetColor() const
{
	return m_col;
}

//==========================================================================
// 不透明度設定
//==========================================================================
void CObject2D::SetAlpha(const float alpha)
{
	m_col.a = alpha;
}

//==========================================================================
// 不透明度取得
//==========================================================================
float CObject2D::GetAlpha() const
{
	return m_col.a;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CObject2D::SetSize(const D3DXVECTOR2 size)
{
	m_size = size;		// サイズ
	SetLength(sqrtf(m_size.x * m_size.x + m_size.y * m_size.y));	// 対角線の長さ
	SetAngle(atan2f(m_size.x, m_size.y));								// 対角線の向き
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CObject2D::GetSize() const
{
	return m_size;
}

//==========================================================================
// 元のサイズの設定
//==========================================================================
void CObject2D::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_sizeOrigin = size;
}

//==========================================================================
// 元のサイズの取得
//==========================================================================
D3DXVECTOR2 CObject2D::GetSizeOrigin() const
{
	return m_sizeOrigin;
}

//==========================================================================
// テクスチャ座標設定
//==========================================================================
void CObject2D::SetTex(D3DXVECTOR2 *tex)
{
	memcpy(&m_fTex[0], tex, sizeof(m_fTex));
}

//==========================================================================
// テクスチャ座標取得
//==========================================================================
D3DXVECTOR2 *CObject2D::GetTex()
{
	return &m_fTex[0];
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
float CObject2D::GetLength() const
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
float CObject2D::GetAngle() const
{
	return m_fAngle;
}


//==========================================================================
// 頂点座標取得
//==========================================================================
MyLib::Vector3 *CObject2D::GetVtxPos()
{
	return &m_VtxPos[0];
}

//==========================================================================
// 頂点座標設定
//==========================================================================
void CObject2D::SetVtxPos(MyLib::Vector3 *pos)
{
	memcpy(&m_VtxPos[0], pos, sizeof(m_VtxPos));
}

//==========================================================================
// 頂点バッファの取得
//==========================================================================
LPDIRECT3DVERTEXBUFFER9 CObject2D::GetVtxBuff() const
{
	return m_pVtxBuff;
}

//==========================================================================
// オブジェクト2Dオブジェクトの取得
//==========================================================================
CObject2D *CObject2D::GetObject2D()
{
	return this;
}

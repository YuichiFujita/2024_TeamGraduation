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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int DEF_VTXNUM = 4;	// デフォルトの頂点数
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObject2D::CObject2D(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{

	m_col = MyLib::color::White();			// 色
	m_colOrigin = MyLib::color::White();	// 元の色
	m_size = D3DXVECTOR2(0.0f, 0.0f);		// サイズ
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);	// 元のサイズ
	m_AnchorType = AnchorPoint::CENTER;		// アンカーポイントの種類
	m_fLength = 0.0f;						// 対角線の長さ
	m_fAngle = 0.0f;						// 対角線の向き
	m_nTexIdx = 0;							// テクスチャのインデックス番号
	m_nNumVertex = DEF_VTXNUM;				// 頂点数
	m_pVtxBuff = nullptr;					// 頂点バッファ
	m_vecUV.clear();						// テクスチャ座標
	m_vecVtxPos.clear();					// 頂点座標

}

//==========================================================================
// デストラクタ
//==========================================================================
CObject2D::~CObject2D()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObject2D* CObject2D::Create(int nPriority, int nNumVtx)
{
	// メモリ確保
	CObject2D* pObject2D = DEBUG_NEW CObject2D(nPriority);

	if (pObject2D != nullptr)
	{
		// 引数情報設定
		pObject2D->m_vecVtxPos.resize(nNumVtx);
		pObject2D->m_vecUV.resize(nNumVtx);

		// 初期化処理
		pObject2D->Init();
	}

	return pObject2D;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject2D::Init()
{
	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_vecVtxPos.empty())	{ m_vecVtxPos.resize(DEF_VTXNUM); }
	if (m_vecUV.empty())		{ m_vecUV.resize(DEF_VTXNUM); }

	if ((int)m_vecUV.size() >= DEF_VTXNUM)
	{// リサイズ済み

		// テクスチャ座標
		m_vecUV[0] = D3DXVECTOR2(0.0f, 0.0f);
		m_vecUV[1] = D3DXVECTOR2(1.0f, 0.0f);
		m_vecUV[2] = D3DXVECTOR2(0.0f, 1.0f);
		m_vecUV[3] = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファの生成
	if (m_pVtxBuff != nullptr) return E_FAIL;

	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * DEF_VTXNUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr)) return E_FAIL;	// 失敗

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject2D::Init(int nNumVertex)
{
	// 頂点数
	m_nNumVertex = nNumVertex;

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_vecVtxPos.empty()) { m_vecVtxPos.resize(nNumVertex); }
	if (m_vecUV.empty()) { m_vecUV.resize(nNumVertex); }

	if ((int)m_vecUV.size() >= nNumVertex)
	{// リサイズ済み

		// テクスチャ座標
		m_vecUV[0] = D3DXVECTOR2(0.0f, 0.0f);
		m_vecUV[1] = D3DXVECTOR2(1.0f, 0.0f);
		m_vecUV[2] = D3DXVECTOR2(0.0f, 1.0f);
		m_vecUV[3] = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファの生成
	if (m_pVtxBuff != nullptr) return E_FAIL;

	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr)) return E_FAIL;	// 失敗

	// 頂点情報設定
	SetVtx();

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
	CObject::Release();
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CObject2D::Kill()
{
	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObject2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObject2D::Draw()
{

	// マネージャのインスタンス取得
	CManager* pMgr = CManager::GetInstance();

#if 0
#if _DEBUG
	
	if ((pMgr->GetPause()->IsPause() || pMgr->Is2DDisp()) &&
		(!GetType() == CObject::TYPE::TYPE_UI || !GetType() == CObject::TYPE::TYPE_NONE))
	{
		return;
	}
#endif
#endif

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pMgr->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVertex - 2);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject2D::SetVtx()
{
	// 情報取得
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

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].tex = m_vecUV[0];
	pVtx[1].tex = m_vecUV[1];
	pVtx[2].tex = m_vecUV[2];
	pVtx[3].tex = m_vecUV[3];

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//==========================================================================
// 頂点回転処理
//==========================================================================
MyLib::Vector3 CObject2D::RotateVtx(const MyLib::Vector3& vtx, const MyLib::Vector3& center)
{
	// 計算用マトリックス
	MyLib::Matrix mtxRot;
	MyLib::Matrix mtxWepon;
	mtxWepon.Identity();

	// 情報取得
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

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	D3DXCOLOR col = GetColor();

	// 頂点座標の設定
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = pos.x + m_vecVtxPos[nCntVtx].x;
		pVtx[nCntVtx].pos.y = pos.y + m_vecVtxPos[nCntVtx].y;
		pVtx[nCntVtx].pos.z = 0.0f;

		// rhwの設定
		pVtx[nCntVtx].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[nCntVtx].col = col;

		// テクスチャ座標の設定
		pVtx[nCntVtx].tex = m_vecUV[nCntVtx];
	}

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//==========================================================================
// サイズ設定
//==========================================================================
void CObject2D::SetSize(const D3DXVECTOR2& size)
{
	m_size = size;		// サイズ
	SetLength(sqrtf(m_size.x * m_size.x + m_size.y * m_size.y));	// 対角線の長さ
	SetAngle(atan2f(m_size.x, m_size.y));							// 対角線の向き
}

//==========================================================================
// テクスチャ座標の設定
//==========================================================================
void CObject2D::SetTexUV(const std::vector<D3DXVECTOR2>& uv)
{
	m_vecUV = uv;

	// 頂点情報設定
	SetVtx();
}

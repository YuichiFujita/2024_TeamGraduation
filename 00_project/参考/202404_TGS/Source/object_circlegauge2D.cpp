//=============================================================================
// 
//  オブジェクト円ゲージ(2D)処理 [object_circlegauge2D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object_circlegauge2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "debugproc.h"

namespace
{
	int MIN_POLYGON = 4;	// 最小多角形
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectCircleGauge2D::CObjectCircleGauge2D(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	m_col = mylib_const::DEFAULT_COLOR;	// 色
	m_fRate = 0.0f;		// 割合
	m_fRateDest = 0.0f;	// 目標の割合
	m_fSize = 0.0f;		// サイズ
	m_nNumVertex = 0;	// 頂点数
	m_nTexIdx = 0;						// テクスチャのインデックス番号
	m_pVtxBuff = nullptr;					// 頂点バッファ

}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectCircleGauge2D::~CObjectCircleGauge2D()
{

}

//==========================================================================
// テクスチャの割り当て
//==========================================================================
void CObjectCircleGauge2D::BindTexture(int nIdx)
{
	// 割り当てる
	m_nTexIdx = nIdx;
}

//==========================================================================
// 生成処理
//==========================================================================
CObjectCircleGauge2D *CObjectCircleGauge2D::Create(int nPolygon, float fSize)
{
	// 生成用のオブジェクト
	CObjectCircleGauge2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{// nullptrだったら

		if (nPolygon < MIN_POLYGON)
		{// 四角形未満は円にならないから抜ける
			return nullptr;
		}

		// メモリの確保
		pObject2D = DEBUG_NEW CObjectCircleGauge2D;

		if (pObject2D != nullptr)
		{// メモリの確保が出来ていたら

			// 頂点数
			pObject2D->m_nNumVertex = nPolygon + 2;

			// サイズ
			pObject2D->m_fSize = fSize;

			// 初期化処理
			pObject2D->Init();
		}

		return pObject2D;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectCircleGauge2D::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (m_pVtxBuff != nullptr)
	{// 既に情報が入ってる場合
		return E_FAIL;
	}

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_nNumVertex,
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

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectCircleGauge2D::Uninit()
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
void CObjectCircleGauge2D::Update()
{
	// 割合更新
	m_fRate += (m_fRateDest - m_fRate) * 0.15f;

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectCircleGauge2D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_nNumVertex - 2);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObjectCircleGauge2D::SetVtx()
{

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// 頂点座標の設定
	pVtx[0].pos = pos;

	// rhwの設定
	pVtx[0].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = mylib_const::DEFAULT_VECTOR2;
	pVtx += 1;

	// 円ゲージ計算用の変数
	bool bEnd = false;	// 終端判定
	float fRotDivision = (D3DX_PI * 2.0f) / (m_nNumVertex - 2);	// 分割毎の向き
	float fOldRot = m_fRate * (D3DX_PI * 2.0f);	// 前回の向き
	float fEndRot = 0.0f;	// 最後の向き
	MyLib::Vector3 EndPos = mylib_const::DEFAULT_VECTOR3;

	// 頂点座標の設定
	for (int i = 0; i < m_nNumVertex - 1; i++)
	{
		// 今回の向き
		float fRot = fRotDivision * i;
		
		if (bEnd == false)
		{
			// 前回のインデックス番号
			int nIdx = i - 1;
			UtilFunc::Transformation::ValueNormalize(nIdx, m_nNumVertex, 0);

			// 今回分の向き
			float fRotDiff = (fRotDivision * i) - (fRotDivision * nIdx);

			// 今までの分から今回分引く
			fOldRot -= fRotDiff;

			if (fOldRot < 0.0f)
			{
				// 最後の向き割り出し
				fEndRot = (fRotDivision * i) + fOldRot;

				// 割合
				float fRate = (fEndRot - (fRotDivision * nIdx)) / fRotDivision;

				// 前回の頂点と今回の頂点
				MyLib::Vector3 p1 = MyLib::Vector3(
					pos.x + cosf(D3DX_PI * -0.5f + (fRotDivision * nIdx) + rot.z) * m_fSize,
					pos.y + sinf(D3DX_PI * -0.5f + (fRotDivision * nIdx) + rot.z) * m_fSize,
					0.0f);

				MyLib::Vector3 p2 = MyLib::Vector3(
					pos.x + cosf(D3DX_PI * -0.5f + (fRotDivision * i) + rot.z) * m_fSize,
					pos.y + sinf(D3DX_PI * -0.5f + (fRotDivision * i) + rot.z) * m_fSize,
					0.0f);

				// 辺上の位置取得(2D)
				EndPos = UtilFunc::Calculation::GetPointOnEdge2D(p1, p2, fRate);

				// 終端状態
				bEnd = true;
			}
		}

		if (bEnd == false)
		{
			// 頂点座標の設定
			pVtx[0].pos = MyLib::Vector3(
				pos.x + cosf(D3DX_PI * -0.5f + fRot + rot.z) * m_fSize,
				pos.y + sinf(D3DX_PI * -0.5f + fRot + rot.z) * m_fSize,
				0.0f);
		}
		else
		{
			// 頂点座標の設定
			pVtx[0].pos = EndPos;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = m_col;

		// テクスチャ座標の設定
		pVtx[0].tex = mylib_const::DEFAULT_VECTOR2;

		pVtx += 1;
	}

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

//==========================================================================
// 色設定
//==========================================================================
void CObjectCircleGauge2D::SetColor(const D3DXCOLOR col)
{
	m_col = col;
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CObjectCircleGauge2D::GetColor() const
{
	return m_col;
}

//==========================================================================
// 割合設定
//==========================================================================
void CObjectCircleGauge2D::SetRate(const float fRate)
{
	m_fRate = fRate;
}

//==========================================================================
// 割合取得
//==========================================================================
float CObjectCircleGauge2D::GetRate() const
{
	return m_fRate;
}

//==========================================================================
// 目標の割合設定
//==========================================================================
void CObjectCircleGauge2D::SetRateDest(const float fRate)
{
	m_fRateDest = fRate;
}

//==========================================================================
// 目標の割合取得
//==========================================================================
float CObjectCircleGauge2D::GetRateDest() const
{
	return m_fRateDest;
}

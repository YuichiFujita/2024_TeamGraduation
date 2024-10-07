//=============================================================================
// 
//  オブジェクトビルボードアニメ処理 [objectBillboard_Anim.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectBillboard_Anim.h"
#include "calculation.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectBillboardAnim::CObjectBillboardAnim(int nPriority) : CObjectBillboard(nPriority)
{
	m_nCntAnim = 0;			// アニメーションのカウンター
	m_nPatternAnim = 0;		// アニメーションのパターン
	m_nDivisionU = 0;		// Uの分割数
	m_nDivisionV = 0;		// Vの分割数
	m_nIntervalAnim = 0;	// アニメーションのインターバル
	m_fSplitValueU = 0.0f;	// Uのスプライト量
	m_fSplitValueV = 0.0f;	// Vのスプライト量
	m_bAutoDeath = false;	// 自動削除のフラグ
	m_bFinish = false;		// アニメーションが終わった判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectBillboardAnim::~CObjectBillboardAnim()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectBillboardAnim *CObjectBillboardAnim::Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// 生成用のオブジェクト
	CObjectBillboardAnim *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObject3D = DEBUG_NEW CObjectBillboardAnim;

		if (pObject3D != nullptr)
		{// メモリの確保が出来ていたら

			// 引数情報
			pObject3D->SetPosition(pos);
			pObject3D->SetOriginPosition(pos);
			pObject3D->m_nDivisionU = nDivisionU;
			pObject3D->m_nDivisionV = nDivisionV;
			pObject3D->m_nIntervalAnim = nInterval;
			pObject3D->m_bAutoDeath = bAutoDeath;

			// 初期化処理
			pObject3D->Init();
		}

		return pObject3D;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectBillboardAnim::Init()
{
	// 初期化処理
	HRESULT hr = CObjectBillboard::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// スプライト量
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectBillboardAnim::Init(const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// 引数情報
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_nIntervalAnim = nInterval;
	m_bAutoDeath = bAutoDeath;

	// 初期化処理
	HRESULT hr = CObjectBillboard::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// スプライト量
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectBillboardAnim::Update()
{

	// カウントを更新
	m_nCntAnim = (m_nCntAnim + 1) % m_nIntervalAnim;

	// パターン更新
	if (m_nCntAnim == 0)
	{
		// パターンNo.を更新
		m_nPatternAnim = (m_nPatternAnim + 1) % (m_nDivisionU * m_nDivisionV);

		if (m_nPatternAnim == 0)
		{// パターンが一周した時

			// 終了状態
			m_bFinish = true;

			if (m_bAutoDeath)
			{
				// オブジェクト破棄
				Uninit();
				return;
			}
		}
	}

	// 更新処理
	CObjectBillboard::Update();
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObjectBillboardAnim::SetVtx()
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = GetSize();
	D3DXCOLOR col = GetColor();
	MyLib::Vector3 rot = GetRotation();

	// 頂点座標の設定
	pVtx[0].pos.x = sinf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.y = cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = cosf(rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(rot.z + m_fAngle) * m_fLength;
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
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

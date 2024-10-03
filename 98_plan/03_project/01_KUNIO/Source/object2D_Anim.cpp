//=============================================================================
// 
//  オブジェクト2Dアニメ処理 [object2D_Anim.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object2D_Anim.h"
#include "calculation.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CObject2D_Anim::CObject2D_Anim(int nPriority) : CObject2D(nPriority)
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
CObject2D_Anim::~CObject2D_Anim()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObject2D_Anim *CObject2D_Anim::Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// 生成用のオブジェクト
	CObject2D_Anim *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObject2D = DEBUG_NEW CObject2D_Anim;

		if (pObject2D != nullptr)
		{// メモリの確保が出来ていたら

			// 引数情報
			pObject2D->SetPosition(pos);
			pObject2D->SetOriginPosition(pos);
			pObject2D->m_nDivisionU = nDivisionU;
			pObject2D->m_nDivisionV = nDivisionV;
			pObject2D->m_nIntervalAnim = nInterval;
			pObject2D->m_bAutoDeath = bAutoDeath;

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
HRESULT CObject2D_Anim::Init()
{
	// 初期化処理
	HRESULT hr = CObject2D::Init();
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
HRESULT CObject2D_Anim::Init(const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// 引数情報
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_nIntervalAnim = nInterval;

	// 初期化処理
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// スプライト量
	m_fSplitValueU = 1.0f / static_cast<float>(m_nDivisionU);
	m_fSplitValueV = 1.0f / static_cast<float>(m_nDivisionV);

	// 自動削除
	m_bAutoDeath = bAutoDeath;

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CObject2D_Anim::Update()
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
	CObject2D::Update();
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject2D_Anim::SetVtx()
{
	// 更新処理
	CObject2D::SetVtx();

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[1].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);
	pVtx[3].tex = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);

	// 頂点バッファをアンロックロック
	GetVtxBuff()->Unlock();
}

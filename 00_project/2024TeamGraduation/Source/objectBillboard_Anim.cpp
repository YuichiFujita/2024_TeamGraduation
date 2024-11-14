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
	m_fTimerAnim = 0.0f;	// アニメーションのカウンター
	m_fIntervalAnim = 0.0f;	// アニメーションのインターバル
	m_nPatternAnim = 0;		// アニメーションのパターン
	m_nDivisionU = 0;		// Uの分割数
	m_nDivisionV = 0;		// Vの分割数
	m_fSplitValueU = 0.0f;	// Uのスプライト量
	m_fSplitValueV = 0.0f;	// Vのスプライト量
	m_bAutoDeath = false;	// 自動削除のフラグ
	m_bAutoPlay = true;		// 自動再生のフラグ
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
CObjectBillboardAnim* CObjectBillboardAnim::Create(const MyLib::Vector3& pos, const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath, int nPriority)
{
	// メモリの確保
	CObjectBillboardAnim* pObject3D = DEBUG_NEW CObjectBillboardAnim(nPriority);

	if (pObject3D != nullptr)
	{// メモリの確保が出来ていたら

		// 引数情報
		pObject3D->SetPosition(pos);
		pObject3D->SetOriginPosition(pos);
		pObject3D->m_nDivisionU = nDivisionU;
		pObject3D->m_nDivisionV = nDivisionV;
		pObject3D->m_fIntervalAnim = fInterval;
		pObject3D->m_bAutoDeath = bAutoDeath;

		// 初期化処理
		pObject3D->Init();
	}

	return pObject3D;
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
HRESULT CObjectBillboardAnim::Init(const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath)
{
	// 引数情報
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_fIntervalAnim = fInterval;
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
void CObjectBillboardAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// カウントを更新
	if (m_bAutoPlay)
	{
		m_fTimerAnim += fDeltaTime * fDeltaRate * fSlowRate;
	}

	// パターン更新
	if (m_fTimerAnim >= m_fIntervalAnim)
	{
		// パターンNo.を更新
		m_nPatternAnim = (m_nPatternAnim + 1) % (m_nDivisionU * m_nDivisionV);
		if (m_nPatternAnim == 0)
		{ // パターンが一周した時

			// 終了状態
			m_bFinish = true;
			if (m_bAutoDeath)
			{
				// オブジェクト破棄
				Uninit();
				return;
			}
		}

		// インターバル分減算
		m_fTimerAnim -= m_fIntervalAnim;
	}

	// 更新処理
	CObjectBillboard::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObjectBillboardAnim::SetVtx()
{
	// テクスチャ座標取得
	std::vector<D3DXVECTOR2> vecUV = GetUV();

	// テクスチャ座標の設定
	vecUV[0] = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	vecUV[1] = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV);
	vecUV[2] = D3DXVECTOR2(m_nPatternAnim * m_fSplitValueU,			(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);
	vecUV[3] = D3DXVECTOR2((m_nPatternAnim + 1) * m_fSplitValueU,	(m_nPatternAnim / m_nDivisionU) * m_fSplitValueV + m_fSplitValueV);

	// テクスチャ座標設定
	SetUV(vecUV);

	// 頂点情報設定処理
	CObjectBillboard::SetVtx();
}

//=============================================================================
// 
//  オブジェクト3Dアニメ処理 [object3D_Anim.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object3D_Anim.h"
#include "calculation.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CObject3DAnim::CObject3DAnim(int nPriority) : CObject3D(nPriority)
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
CObject3DAnim::~CObject3DAnim()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObject3DAnim *CObject3DAnim::Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath)
{
	// 生成用のオブジェクト
	CObject3DAnim *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObject3D = DEBUG_NEW CObject3DAnim;

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
HRESULT CObject3DAnim::Init()
{
	// 初期化処理
	HRESULT hr = CObject3D::Init();
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
HRESULT CObject3DAnim::Init(const int nDivisionU, const int nDivisionV, const int nInterval)
{
	// 引数情報
	m_nDivisionU = nDivisionU;
	m_nDivisionV = nDivisionV;
	m_nIntervalAnim = nInterval;

	// 初期化処理
	HRESULT hr = CObject3D::Init();
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
void CObject3DAnim::Update()
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
	CObject3D::Update();
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject3DAnim::SetVtx()
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 境界線のベクトル
	MyLib::Vector3 vecLine0 = pVtx[1].pos - pVtx[0].pos;
	MyLib::Vector3 vecLine1 = pVtx[2].pos - pVtx[0].pos;

	// 外積求める
	MyLib::Vector3 Nor[4];

	// 外積
	D3DXVec3Cross(&Nor[0], &vecLine0, &vecLine1);
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	Nor[0] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	// 境界線のベクトル
	vecLine0 = pVtx[2].pos - pVtx[3].pos;
	vecLine1 = pVtx[1].pos - pVtx[3].pos;

	// 外積求める
	Nor[3] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// 外積
	D3DXVec3Cross(&Nor[3], &vecLine0, &vecLine1);
	D3DXVec3Normalize(&Nor[3], &Nor[3]);

	Nor[1] = (Nor[0] + Nor[3]) / 2.0f;
	Nor[2] = (Nor[0] + Nor[3]) / 2.0f;

	// 法線ベクトルの設定
	pVtx[0].nor = Nor[0];
	pVtx[1].nor = Nor[1];
	pVtx[2].nor = Nor[2];
	pVtx[3].nor = Nor[3];

	MyLib::Vector3 size = GetSize();
	D3DXCOLOR col = GetColor();

	// 位置を更新
	pVtx[0].pos = MyLib::Vector3(-size.x, +size.y, +size.z);
	pVtx[1].pos = MyLib::Vector3(+size.x, +size.y, +size.z);
	pVtx[2].pos = MyLib::Vector3(-size.x, -size.y, -size.z);
	pVtx[3].pos = MyLib::Vector3(+size.x, -size.y, -size.z);

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

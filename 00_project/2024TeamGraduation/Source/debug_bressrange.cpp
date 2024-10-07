//=============================================================================
// 
// デバッグ息範囲処理 [debug_bressrange.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "debug_bressrange.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CDebugBressRange::CDebugBressRange(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CDebugBressRange::~CDebugBressRange()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CDebugBressRange* CDebugBressRange::Create()
{
	// 生成用のオブジェクト
	CDebugBressRange* pEffect = nullptr;

	// メモリの確保
	pEffect = DEBUG_NEW CDebugBressRange;

	if (pEffect != nullptr)
	{
		// 初期化処理
		pEffect->Init();
	}

	return pEffect;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDebugBressRange::Init()
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 初期化処理
	HRESULT hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{
		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDebugBressRange::Uninit()
{
	
	// 終了処理
	CObjectBillboard::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CDebugBressRange::Update()
{
	// 過去の位置設定
	SetOldPosition(GetPosition());

	CObjectBillboard::Update();

	// 頂点座標の設定
	SetVtx();

}

//==========================================================================
// 描画処理
//==========================================================================
void CDebugBressRange::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObjectBillboard::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CDebugBressRange::SetVtx()
{
	// 頂点設定
	CObjectBillboard::SetVtx();

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = m_LeftUP;
	pVtx[1].pos = m_RightUP;
	pVtx[2].pos = m_LeftDW;
	pVtx[3].pos = m_RightDW;

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

void CDebugBressRange::SetRange(const MyLib::Vector3& leftup, const MyLib::Vector3& rightup, const MyLib::Vector3& leftdw, const MyLib::Vector3& rightdw)
{
	m_LeftUP = leftup;
	m_RightUP = rightup;
	m_LeftDW = leftdw;
	m_RightDW = rightdw;
}

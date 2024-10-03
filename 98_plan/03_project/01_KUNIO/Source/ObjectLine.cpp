//=============================================================================
// 
//  オブジェクトライン処理 [objectLine.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectLine.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectLine::CObjectLine(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	m_Info = SLineInfo();
	m_bZSort = false;				// Zソートのフラグ
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectLine::~CObjectLine()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectLine *CObjectLine::Create(MyLib::Vector3 left, MyLib::Vector3 right, D3DXCOLOR col)
{
	// 生成用のオブジェクト
	CObjectLine *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObject3D = DEBUG_NEW CObjectLine;

		if (pObject3D != nullptr)
		{// メモリの確保が出来ていたら

			// 引数情報
			pObject3D->m_Info.left = left;
			pObject3D->m_Info.right = right;
			pObject3D->m_Info.col = col;

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
HRESULT CObjectLine::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// 頂点情報設定
	SetVtx();

	SetType(CObject::TYPE::TYPE_OBJECT3D);

	SetPosition(0.0f);
	SetOriginPosition((m_Info.left + m_Info.right) * 0.5f);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectLine::Uninit()
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
void CObjectLine::Update()
{

	// 頂点情報設定
	SetVtx();
}


//==========================================================================
// 描画処理
//==========================================================================
void CObjectLine::Draw()
{
	// ボックスコライダーの描画
	if (!CManager::IsDisp_BoxColliders()) return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_bZSort)
	{
		// Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// 常に描画する
	}



	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// 計算用マトリックス宣言
	MyLib::Matrix mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// 位置を反映する
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

	// ワールドマトリックスの設定
	D3DXMATRIX mtx = m_mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


}

//==========================================================================
// 頂点情報設定
//==========================================================================
void CObjectLine::SetVtx()
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置を更新
	pVtx[0].pos = m_Info.left;
	pVtx[1].pos = m_Info.right;

	// 頂点カラーの設定
	pVtx[0].col = m_Info.col;
	pVtx[1].col = m_Info.col;

	//// 法線の設定
	//pVtx[0].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	//pVtx[1].nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);

	// 頂点バッファをアンロックロック
	m_pVtxBuff->Unlock();
}

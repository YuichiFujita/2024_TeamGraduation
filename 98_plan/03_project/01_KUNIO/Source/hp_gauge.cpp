//=============================================================================
// 
//  体力ゲージ処理 [hp_gauge.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "hp_gauge.h"
#include "manager.h"
#include "hp_gauge_tip.h"
#include "camera.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define WIDTH			(35.0f)	// 横幅
#define HEIGHT			(5.0f)	// 縦幅

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CHP_Gauge::m_apTextureFile[] =					// テクスチャのファイル
{
	"data\\TEXTURE\\hpgauge\\black.png",
	"data\\TEXTURE\\hpgauge\\hpgauge.png",
};
int CHP_Gauge::m_nNumAll = 0;	// 総数

//==========================================================================
// コンストラクタ
//==========================================================================
CHP_Gauge::CHP_Gauge(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		m_HPGauge[nCntGauge].fMaxWidth = 0.0f;		// 幅の最大値
		m_HPGauge[nCntGauge].fMaxHeight = 0.0f;		// 高さの最大値
		m_HPGauge[nCntGauge].fWidthDest = 0.0f;		// 幅の差分
		m_nTexIdx[nCntGauge] = 0;					// テクスチャのインデックス番号
	}

	m_nLife = 0;	// 体力
	m_nMaxLife = 0;	// 最大体力

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CHP_Gauge::~CHP_Gauge()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CHP_Gauge* CHP_Gauge::Create(float fPosLen, int nMaxLife, float fSizeBuff)
{
	// 生成用のオブジェクト
	CHP_Gauge* pHPGauge = nullptr;

	// メモリの確保
	pHPGauge = DEBUG_NEW CHP_Gauge;

	if (pHPGauge != nullptr)
	{// メモリの確保が出来ていたら

		// 原点からの長さを渡す
		pHPGauge->m_fPosLength = fPosLen;

		// 最大体力
		pHPGauge->m_nMaxLife = nMaxLife;

		for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
		{
			// 生成処理
			pHPGauge->m_HPGauge[nCntGauge].pObjBillboard =
				pHPGauge->m_HPGauge[nCntGauge].pObjBillboard->Create();

			if (pHPGauge->m_HPGauge[nCntGauge].pObjBillboard == nullptr)
			{// nullptrだったら
				pHPGauge->Uninit();
				return nullptr;
			}

			pHPGauge->m_HPGauge[nCntGauge].pObjBillboard->SetSize(
				D3DXVECTOR2(WIDTH * fSizeBuff, HEIGHT * fSizeBuff));	// サイズ
		}

		// 初期化処理
		pHPGauge->Init();

		// 種類の設定
		pHPGauge->SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);
	}

	return pHPGauge;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CHP_Gauge::Init()
{
	// 最大体力
	m_nLife = m_nMaxLife;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{

		// 各種変数の初期化
		m_HPGauge[nCntGauge].fMaxWidth = m_HPGauge[nCntGauge].pObjBillboard->GetSize().x;		// 幅の最大値
		m_HPGauge[nCntGauge].fMaxHeight = m_HPGauge[nCntGauge].pObjBillboard->GetSize().y;		// 高さの最大値

		// テクスチャの割り当て
		m_nTexIdx[nCntGauge] = CTexture::GetInstance()->Regist(m_apTextureFile[nCntGauge]);

		// テクスチャの割り当て
		m_HPGauge[nCntGauge].pObjBillboard->BindTexture(m_nTexIdx[nCntGauge]);
	}

	// 先端生成
	m_pTip = CHPGaugeTip::Create(GetPosition() - MyLib::Vector3(WIDTH, 0.0f, 0.0f), GetPosition() + MyLib::Vector3(WIDTH, 0.0f, 0.0f), CHPGaugeTip::TYPE::TYPE_BILLBOARD);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CHP_Gauge::Uninit()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptrじゃなかったら

			// 終了処理
			m_HPGauge[nCntGauge].pObjBillboard->Uninit();
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	if (m_pTip != nullptr)
	{
		m_pTip->Uninit();
		m_pTip = nullptr;
	}

	// 情報削除
	Release();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptrじゃなかったら

			// 終了処理
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	m_nNumAll--;
}


//==========================================================================
// 終了処理
//==========================================================================
void CHP_Gauge::Kill()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptrじゃなかったら

			// 終了処理
			m_HPGauge[nCntGauge].pObjBillboard->Uninit();
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	if (m_pTip != nullptr)
	{
		m_pTip->Kill();
		m_pTip = nullptr;
	}

	// 情報削除
	Release();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptrじゃなかったら

			// 終了処理
			m_HPGauge[nCntGauge].pObjBillboard = nullptr;
		}
	}

	m_nNumAll--;
}

//==========================================================================
// 更新処理
//==========================================================================
void CHP_Gauge::Update()
{

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptrじゃなかったら

			if (nCntGauge == VTXTYPE_PINK)
			{// ゲージ部分だけ　

				// 減少処理
				GaugeDecrement(nCntGauge);
			}

			// 頂点座標設定
			SetVtx(nCntGauge);
		}
	}

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	MyLib::Vector3 cameraRot = CManager::GetInstance()->GetCamera()->GetRotation();

	// 先端
	if (m_pTip != nullptr) {
		MyLib::Vector3 left = pos, right = pos;
		float maxlen = m_HPGauge[0].fMaxWidth;

		left.x = pos.x - sinf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		left.z = pos.z - cosf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		left.x -= sinf(cameraRot.y) * 3.0f;
		left.z -= cosf(cameraRot.y) * 3.0f;

		right.x = pos.x + sinf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		right.z = pos.z + cosf(D3DX_PI * 0.5f + cameraRot.y) * maxlen;
		right.x -= sinf(cameraRot.y) * 3.0f;
		right.z -= cosf(cameraRot.y) * 3.0f;

		m_pTip->SetLeftPosition(left);
		m_pTip->SetRightPosition(right);
	}
}

//==========================================================================
// 位置更新
//==========================================================================
void CHP_Gauge::UpdatePosition(MyLib::Vector3 pos, int nLife)
{
	// 高さ分上にあげる
	pos.y += m_fPosLength;

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_HPGauge[nCntGauge].pObjBillboard != nullptr)
		{// nullptrじゃなかったら

			// 位置設定
			m_HPGauge[nCntGauge].pObjBillboard->SetPosition(pos);

			// 現在の体力設定
			m_nLife = nLife;

			//目標の幅設定
			m_HPGauge[nCntGauge].fWidthDest =
				m_HPGauge[nCntGauge].fMaxWidth * ((float)m_nLife / (float)m_nMaxLife);
		}
	}
	SetPosition(pos);
}

//==========================================================================
// 減少処理
//==========================================================================
void CHP_Gauge::GaugeDecrement(int nCntGauge)
{
	// サイズ取得
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	// 差分で徐々に減らしていく
	size.x +=
		(m_HPGauge[nCntGauge].fWidthDest - size.x) * 0.15f;

	// サイズ設定
	m_HPGauge[nCntGauge].pObjBillboard->SetSize(size);

	// 頂点座標設定
	SetVtx(nCntGauge);

}

//==========================================================================
// 色更新
//==========================================================================
void CHP_Gauge::ChangeColor(int nCntGauge)
{
	// サイズ取得
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	// 色取得
	D3DXCOLOR col = m_HPGauge[nCntGauge].pObjBillboard->GetColor();

	if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.95f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.6f)
	{//HPゲージ8割

		col = D3DXCOLOR(0.2f, 0.6f, 0.2f, 1.0f);
	}
	else if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.6f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.4f)
	{//HPゲージ6割

		col = D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f);
	}
	else if (
		size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.4f &&
		size.x / m_HPGauge[nCntGauge].fMaxWidth > 0.2f)
	{//HPゲージ4割

		col = D3DXCOLOR(0.8f, 0.5f, 0.2f, 1.0f);
	}
	else if (size.x / m_HPGauge[nCntGauge].fMaxWidth <= 0.2f)
	{//危険エリア

		col = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	}
	else
	{//満タン

		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

	// 色設定
	m_HPGauge[nCntGauge].pObjBillboard->SetColor(col);

	// サイズ取得
	m_HPGauge[nCntGauge].pObjBillboard->SetSize(size);

}

//==========================================================================
// 描画処理
//==========================================================================
void CHP_Gauge::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 描画処理
		m_HPGauge[nCntGauge].pObjBillboard->Draw();

		// ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CHP_Gauge::SetVtx()
{

}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CHP_Gauge::SetVtx(int nCntGauge)
{
	// 頂点設定
	m_HPGauge[nCntGauge].pObjBillboard->SetVtx();

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_HPGauge[nCntGauge].pObjBillboard->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// サイズ取得
	D3DXVECTOR2 size = m_HPGauge[nCntGauge].pObjBillboard->GetSize();

	//頂点座標の設定
	pVtx[0].pos = MyLib::Vector3(-size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), size.y, 0.0f);
	pVtx[1].pos = MyLib::Vector3(size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), size.y, 0.0f);
	pVtx[2].pos = MyLib::Vector3(-size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), -size.y, 0.0f);
	pVtx[3].pos = MyLib::Vector3(size.x - (m_HPGauge[nCntGauge].fMaxWidth - size.x), -size.y, 0.0f);

	// 頂点バッファをアンロックロック
	m_HPGauge[nCntGauge].pObjBillboard->GetVtxBuff()->Unlock();

}

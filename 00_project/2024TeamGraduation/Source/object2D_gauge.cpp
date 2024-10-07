//=============================================================================
// 
//  オブジェクト2Dゲージ処理 [object2D_gauge.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "object2D_gauge.h"
#include "texture.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float DEFAULT_MOVEFACTOR = 0.15f;	// デフォルトの移動係数
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObject2D_Gauge::CObject2D_Gauge(int nPriority) : CObject2D(nPriority)
{
	m_fOriginWidth = 0.0f;	// 幅の初期値
	m_fMaxWidth = 0.0f;		// 幅の最大値
	m_fWidthDest = 0.0f;	// 目標の幅
	m_fMaxHeight = 0.0f;	// 高さの最大値
	m_fMoveFactor = 0.0f;	// 移動の係数
	m_nValue = 0;			// 現在の値
	m_nOriginValue = 0;		// 初期値
	m_nMaxValue = 0;		// 最大値
}

//==========================================================================
// デストラクタ
//==========================================================================
CObject2D_Gauge::~CObject2D_Gauge()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObject2D_Gauge *CObject2D_Gauge::Create(float width, float height, int maxvalue, std::string texturepath, int nPriority)
{
	// 生成用のオブジェクト
	CObject2D_Gauge *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObject2D = DEBUG_NEW CObject2D_Gauge(nPriority);

		if (pObject2D != nullptr)
		{// メモリの確保が出来ていたら

			// 引数情報
			pObject2D->m_fMaxWidth = width;
			pObject2D->m_fMaxHeight = height;
			pObject2D->m_nMaxValue = maxvalue;


			pObject2D->m_fOriginWidth = width;		// 幅の初期値
			pObject2D->m_fMaxHeight = height;		// 高さの最大値
			pObject2D->m_nOriginValue = maxvalue;	// 初期値


			if (texturepath != "")
			{
				int nIdx = CTexture::GetInstance()->Regist(texturepath);

				// テクスチャの割り当て
				pObject2D->BindTexture(nIdx);
			}

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
HRESULT CObject2D_Gauge::Init()
{
	// 初期化処理
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 頂点情報設定
	SetVtx();

	// 値設定
	SetValue(m_nMaxValue);
	m_fMoveFactor = DEFAULT_MOVEFACTOR;	// 移動の係数
	m_fMaxWidth = m_fOriginWidth;	// 幅の最大値
	m_fWidthDest = m_fOriginWidth;	// 目標の幅
	m_nMaxValue = m_nOriginValue;	// 最大値
	m_nValue = m_nOriginValue;		// 現在の値

	// 値設定
	SetSize(D3DXVECTOR2(m_fMaxWidth, m_fMaxHeight));
	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObject2D_Gauge::Init(float width, float height, int maxvalue, std::string texturepath)
{
	// 引数情報
	m_fOriginWidth = width;		// 幅の初期値
	m_fMaxWidth = width;		// 幅の最大値
	m_fWidthDest = width;		// 目標の幅
	m_fMaxHeight = height;		// 高さの最大値
	m_fMoveFactor = DEFAULT_MOVEFACTOR;	// 移動の係数
	m_nOriginValue = maxvalue;	// 初期値
	m_nMaxValue = maxvalue;		// 最大値
	m_nValue = maxvalue;		// 現在の値

	// 値設定
	SetValue(m_nMaxValue);

	// 初期化処理
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	if (texturepath != "")
	{
		int nIdx = CTexture::GetInstance()->Regist(texturepath);

		// テクスチャの割り当て
		BindTexture(nIdx);
	}


	// 値設定
	SetSize(D3DXVECTOR2(m_fMaxWidth, m_fMaxHeight));

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 更新処理
//==========================================================================
void CObject2D_Gauge::Update()
{
	// 減少処理
	GaugeDecrement();

	// サイズ取得
	D3DXVECTOR2 size = GetSize();

	D3DXVECTOR2* pTex = GetTex();

	pTex[0] = D3DXVECTOR2(0.0f, 0.0f);
	pTex[1] = D3DXVECTOR2(size.x / m_fMaxWidth, 0.0f);
	pTex[2] = D3DXVECTOR2(0.0f, 1.0f);
	pTex[3] = D3DXVECTOR2(size.x / m_fMaxWidth, 1.0f);

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 減少処理
//==========================================================================
void CObject2D_Gauge::GaugeDecrement()
{
	// サイズ取得
	D3DXVECTOR2 size = GetSize();

	// 差分で徐々に減らしていく
	size.x += (m_fWidthDest - size.x) * m_fMoveFactor;

	// サイズ設定
	SetSize(size);
}

//==========================================================================
// 値設定
//==========================================================================
void CObject2D_Gauge::SetValue(int value)
{
	// 現在の体力設定
	m_nValue = value;

	//目標の幅設定
	m_fWidthDest = m_fMaxWidth * ((float)m_nValue / (float)m_nMaxValue);
}

//==========================================================================
// 最大値のアップグレード
//==========================================================================
MyLib::Vector3 CObject2D_Gauge::UpgradeMaxValue(int addvalue, bool bChangePos)
{
	// 最大値
	m_nMaxValue = m_nOriginValue + addvalue;
	float nowlen = m_fOriginWidth * (static_cast<float>(m_nMaxValue) / static_cast<float>(m_nOriginValue));

	// 最大の長さ
	m_fMaxWidth = nowlen;

	//// ゲージリセット
	//D3DXVECTOR2 size = GetSize();
	//size.x = nowlen;
	//SetSize(size);

	// 値設定
	SetValue(m_nMaxValue);

	// 元の長さとの差分
	float difflen = nowlen - m_fOriginWidth;

	// 位置設定
	MyLib::Vector3 newpos = GetOriginPosition();

	if (bChangePos)
	{
		newpos.x += difflen;
		SetPosition(newpos);
	}

	return newpos;
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CObject2D_Gauge::SetVtx()
{
	// 更新処理
	CObject2D::SetVtx();

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// サイズ取得
	D3DXVECTOR2 size = GetSize();

	D3DXVECTOR2* pTex = GetTex();

	// 頂点座標の設定
	pVtx[0].pos = MyLib::Vector3(pos.x + -size.x - (m_fMaxWidth - size.x), pos.y + -size.y, 0.0f);
	pVtx[1].pos = MyLib::Vector3(pos.x + size.x - (m_fMaxWidth - size.x), pos.y + -size.y, 0.0f);
	pVtx[2].pos = MyLib::Vector3(pos.x + -size.x - (m_fMaxWidth - size.x), pos.y + size.y, 0.0f);
	pVtx[3].pos = MyLib::Vector3(pos.x + size.x - (m_fMaxWidth - size.x), pos.y + size.y, 0.0f);

	// 頂点バッファをアンロックロック
	GetVtxBuff()->Unlock();
}

//=============================================================================
// 
// エフェクト処理 [2D_effect.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "2D_effect.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define DEF_RADIUS			(20.0f)
#define EFFECT_2D_LIFE		(0.5f)
#define EFFECT_2DSIZE1		(0.97f)
#define EFFECT_2DSIZE2		(0.98f)
#define EFFECT_2DSIZE3		(0.99f)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CEffect2D::m_apTextureFile[] =		// ファイル読み込み
{
	"data\\TEXTURE\\effect\\effect000.jpg",	   // 通常エフェクト
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // 煙エフェクト
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // 黒煙
	"data\\TEXTURE\\effect\\effect000.png",	   // 黒エフェクト
	"data\\TEXTURE\\effect\\effect001.png",	   // 十字エフェクト
	"data\\TEXTURE\\effect\\Star01.png",	   // 十字エフェクト
	"data\\TEXTURE\\effect\\Star02.png",	   // 十字エフェクト
};
int CEffect2D::m_nNumAll = 0;	// 総数

//==========================================================================
// コンストラクタ
//==========================================================================
CEffect2D::CEffect2D(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色の元
	m_fRadius = 0.0f;				// 半径
	m_fMaxRadius = 0.0f;			// 最大半径
	m_fLife = 0.0f;					// 寿命
	m_fMaxLife = 0.0f;				// 最大寿命(固定)
	m_moveType = MOVEEFFECT_NONE;	// 移動の種類
	m_nType = TYPE_NORMAL;			// 種類
	m_bAddAlpha = true;				// 加算合成の判定
	
	// テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{
		m_nTexIdx[nCntTex] = 0;		// テクスチャのインデックス番号
	}

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CEffect2D::~CEffect2D()
{
	// 総数減算
	m_nNumAll--;
}

//==========================================================================
// 生成処理
//==========================================================================
CEffect2D *CEffect2D::Create()
{
	// 生成用のオブジェクト
	CEffect2D *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptrだったら

		// メモリの確保
		pEffect = DEBUG_NEW CEffect2D;

		//if (pEffect->GetID() < 0)
		//{// メモリ確保に失敗していたら

		//	delete pEffect;
		//	return nullptr;
		//}

		if (pEffect != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pEffect->Init();
		}

		return pEffect;
	}

	return nullptr;
}

//==========================================================================
// 生成処理
//==========================================================================
CEffect2D *CEffect2D::Create(const MyLib::Vector3 pos, const MyLib::Vector3 move, const D3DXCOLOR col, const float fRadius, const float fLife, const int moveType, const TYPE type)
{
	// 生成用のオブジェクト
	CEffect2D *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptrだったら

		// メモリの確保
		pEffect = DEBUG_NEW CEffect2D;

		if (pEffect != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pEffect->Init(pos, move, col, fRadius, fLife, moveType, type);
		}

		return pEffect;
	}

	return nullptr;
}


//==========================================================================
// 生成処理
//==========================================================================
CEffect2D *CEffect2D::Create(const MyLib::Vector3 pos, const MyLib::Vector3 move, const MyLib::Vector3 rot, const D3DXCOLOR col, const float fRadius, const float fLife, const int moveType, const TYPE type)
{
	// 生成用のオブジェクト
	CEffect2D *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptrだったら

		// メモリの確保
		pEffect = DEBUG_NEW CEffect2D;

		if (pEffect != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pEffect->Init(pos, move, col, fRadius, fLife, moveType, type);
			pEffect->SetRotation(rot);
		}

		return pEffect;
	}

	return nullptr;
}

//==========================================================================
// エフェクトの初期化処理
//==========================================================================
HRESULT CEffect2D::Init()
{
	HRESULT hr;

	// 各種変数の初期化
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色の元
	m_fRadius = DEF_RADIUS;						// 半径
	m_fMaxRadius = m_fRadius;					// 最大半径
	m_fLife = EFFECT_2D_LIFE;					// 寿命
	m_fMaxLife = m_fLife;						// 最大寿命(固定)
	m_moveType = MOVEEFFECT_NONE;				// 移動の種類
	m_nType = TYPE_NORMAL;						// 種類
	m_bAddAlpha = true;							// 加算合成の判定

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{
		// テクスチャの割り当て
		m_nTexIdx[nCntTex] = CTexture::GetInstance()->Regist(m_apTextureFile[nCntTex]);

		// テクスチャの割り当て
		BindTexture(m_nTexIdx[nCntTex]);
	}

	// 初期化処理
	hr = CObject2D::Init();

	if (FAILED(hr))
	{// 失敗したとき

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// エフェクトの初期化処理
//==========================================================================
HRESULT CEffect2D::Init(const MyLib::Vector3 pos, const MyLib::Vector3 move, const D3DXCOLOR col, const float fRadius, const float fLife, const int moveType, const TYPE type)
{
	HRESULT hr;

	// 各種変数の初期化
	SetPosition(pos);
	SetMove(move);
	m_colOrigin = col;							// 色の元
	SetColor(col);								// 色
	m_fRadius = fRadius;						// 半径
	m_fMaxRadius = m_fRadius;					// 最大半径
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));	// サイズ設定
	m_fLife = fLife;							// 寿命
	m_fMaxLife = m_fLife;						// 最大寿命(固定)
	m_moveType = moveType;						// 移動の種類
	m_nType = type;								// 種類

	// 加算合成の判定
	switch (m_nType)
	{
	case TYPE_NORMAL:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKE:
		m_bAddAlpha = true;
		break;

	case TYPE_SMOKEBLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_BLACK:
		m_bAddAlpha = false;
		break;

	case TYPE_JUJI:
		m_bAddAlpha = true;
		break;

	case TYPE_JUJI2:
		m_bAddAlpha = true;
		break;

	case TYPE_JUJI3:
		m_bAddAlpha = true;
		break;

	default:
		m_bAddAlpha = true;
		break;
	}

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャの割り当て
	m_nTexIdx[m_nType] = CTexture::GetInstance()->Regist(m_apTextureFile[m_nType]);

	// テクスチャの割り当て
	BindTexture(m_nTexIdx[m_nType]);

	// 初期化処理
	hr = CObject2D::Init();

	if (FAILED(hr))
	{// 失敗したとき

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// エフェクトの終了処理
//==========================================================================
void CEffect2D::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// エフェクトの更新処理
//==========================================================================
void CEffect2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 色取得
	D3DXCOLOR col = GetColor();

	// 位置更新
	pos += move * fDeltaRate * fSlowRate;

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 小さくしていく処理の場合
	if (m_moveType == MOVEEFFECT_SUB)
	{// エフェクトを小さくしていく

		// 縮小処理
		SubSize(fDeltaTime, fDeltaRate, fSlowRate);
	}
	else if (m_moveType == MOVEEFFECT_SUPERSUB)
	{// エフェクトを小さくしていく

		// 超縮小処理
		SuperSubSize(fDeltaTime, fDeltaRate, fSlowRate);
	}
	else if (m_moveType == MOVEEFFECT_ADD)
	{// エフェクトを大きくしていく

		// 拡大処理
		AddSize(fDeltaTime, fDeltaRate, fSlowRate);
	}
	else if (m_moveType == MOVEEFFECT_GENSUI)
	{
		Gensui(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 寿命の更新
	m_fLife -= fDeltaTime * fSlowRate;

	// 不透明度の更新
	col.a = m_colOrigin.a * (m_fLife / m_fMaxLife);
	SetAlpha(col.a);

	// サイズ設定
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

	// 色設定
	SetColor(col);

	if (m_fLife <= 0.0f)
	{// 寿命が尽きたら

		// エフェクトの削除
		Uninit();
		return;
	}

	// 頂点座標の設定
	SetVtx();
}

//==========================================================================
// エフェクトの縮小処理
//==========================================================================
void CEffect2D::SubSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//int nEffect_2DType = rand() % 3;

	//if (nEffect_2DType == 0)
	//{
	//	m_fRadius *= EFFECT_2DSIZE1 * fDeltaRate * fSlowRate;
	//}
	//else if (nEffect_2DType == 1)
	//{
	//	m_fRadius *= EFFECT_2DSIZE2 * fDeltaRate * fSlowRate;
	//}
	//else if (nEffect_2DType == 2)
	//{
	//	m_fRadius *= EFFECT_2DSIZE3 * fDeltaRate * fSlowRate;
	//}
}

//==========================================================================
// エフェクトの縮小処理
//==========================================================================
void CEffect2D::SuperSubSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fRadius = m_fMaxRadius * m_fLife / m_fMaxLife;
}

//==========================================================================
// エフェクトの拡大処理
//==========================================================================
void CEffect2D::AddSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//int nEffect_2DType = rand() % 3;

	//if (nEffect_2DType == 0)
	//{
	//	m_fRadius += 1.8f * fDeltaTime * fSlowRate;
	//}
	//else if (nEffect_2DType == 1)
	//{
	//	m_fRadius += 0.8f * fDeltaTime * fSlowRate;
	//}
	//else if (nEffect_2DType == 2)
	//{
	//	m_fRadius += 0.0f * fDeltaTime * fSlowRate;
	//}
}

//==========================================================================
// エフェクトの減衰処理
//==========================================================================
void CEffect2D::Gensui(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 移動量取得
	MyLib::Vector3 move = GetMove();

	move.x += (0.0f - move.x) * (0.15f * fDeltaRate * fSlowRate);
	move.y += (0.0f - move.y) * (0.15f * fDeltaRate * fSlowRate);

	// 移動量設定
	SetMove(move);

	m_fRadius = m_fMaxRadius * m_fLife / m_fMaxLife;
}

//==========================================================================
// エフェクトの描画処理
//==========================================================================
void CEffect2D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	if (m_bAddAlpha)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CEffect2D::SetVtx()
{
	// 頂点設定
	CObject2D::SetVtx();
}

//==========================================================================
// 総数取得
//==========================================================================
int CEffect2D::GetNumAll()
{
	return m_nNumAll;
}

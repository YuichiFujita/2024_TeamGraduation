//=============================================================================
// 
// エフェクト処理 [3D_effect.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "3D_effect.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define DEF_RADIUS			(20.0f)
#define EFFECT_3D_LIFE		(30)
#define EFFECT_3DSIZE1		(0.97f)
#define EFFECT_3DSIZE2		(0.98f)
#define EFFECT_3DSIZE3		(0.99f)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CEffect3D::m_apTextureFile[] =					// ファイル読み込み
{
	"data\\TEXTURE\\effect\\effect000.jpg",	   // 通常エフェクト
	"data\\TEXTURE\\effect\\effect_point01.tga",	   // 点エフェクト
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // 煙エフェクト
	"data\\TEXTURE\\effect\\smoke_05.tga",	   // 黒煙
	"data\\TEXTURE\\effect\\effect000.png",	   // 黒エフェクト
	"data\\TEXTURE\\effect\\effect001.png",		// 十字エフェクト
	"data\\TEXTURE\\effect\\Star01.png",		// 十字エフェクト
	"data\\TEXTURE\\effect\\thunder_02.tga",	// 雷エフェクト
	"data\\TEXTURE\\effect\\water.png",			// 水エフェクト
	"",											// nullptrエフェクト
};
int CEffect3D::m_nNumAll = 0;	// 総数
int CEffect3D::m_nTexIdx[TYPE_MAX] = {};	// テクスチャのインデックス番号

//==========================================================================
// コンストラクタ
//==========================================================================
CEffect3D::CEffect3D(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;			// 原点
	m_updatePosition = mylib_const::DEFAULT_VECTOR3;	// 更新後の位置
	m_setupPosition = mylib_const::DEFAULT_VECTOR3;		// セットアップ位置
	m_posDest = mylib_const::DEFAULT_VECTOR3;			// 目標の位置
	m_colOrigin = mylib_const::DEFAULT_COLOR;	// 色の元
	m_pMtxParent = nullptr;						// 親マトリックスのポインタ
	m_fRadius = 0.0f;							// 半径
	m_fMaxRadius = 0.0f;						// 最大半径
	m_fAddSizeValue = 0.0f;						// サイズ変更量
	m_fGravity = 0.0f;							// 重力
	m_fMoveFactor = 0.0f;						// 移動補正係数
	m_nLife = 0;								// 寿命
	m_nMaxLife = 0;								// 最大寿命(固定)
	m_moveType = MOVEEFFECT_NONE;				// 移動の種類
	m_nType = TYPE_NORMAL;						// 種類
	m_pParent = nullptr;							// 親のポインタ
	m_bAddAlpha = true;							// 加算合成の判定
	m_bZSort = false;							// Zソートのフラグ
	m_bGravity = false;							// 重力のフラグ
	m_bChaseDest = false;						// 目標の位置へ向かうフラグ

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CEffect3D::~CEffect3D()
{
	
}

//==========================================================================
// テクスチャ読み込み
//==========================================================================
void CEffect3D::LoadTexture()
{
	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < sizeof(m_apTextureFile) / sizeof(*m_apTextureFile); nCntTex++)
	{// テクスチャデータの配列分繰り返す
		m_nTexIdx[nCntTex] = CTexture::GetInstance()->Regist(m_apTextureFile[nCntTex]);
	}
}

//==========================================================================
// 生成処理
//==========================================================================
CEffect3D *CEffect3D::Create()
{
	// 生成用のオブジェクト
	CEffect3D *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptrだったら

		// メモリの確保
		pEffect = DEBUG_NEW CEffect3D;

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
CEffect3D* CEffect3D::Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type, const float fAddSizeValue)
{
	// メモリの確保
	CEffect3D* pEffect = DEBUG_NEW CEffect3D;

	if (pEffect != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		pEffect->m_fAddSizeValue = fAddSizeValue;	// サイズ変更量
		HRESULT hr = pEffect->Init(pos, move, col, fRadius, nLife, moveType, type);
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	return pEffect;
}

//==========================================================================
// エフェクトの初期化処理
//==========================================================================
HRESULT CEffect3D::Init()
{
	HRESULT hr;

	// 各種変数の初期化
	m_posOrigin = MyLib::Vector3(0.0f, 0.0f, 0.0f);		// 原点
	m_updatePosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 更新後の位置
	m_setupPosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// セットアップ位置
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色の元
	m_fRadius = DEF_RADIUS;						// 半径
	m_fMaxRadius = m_fRadius;					// 最大半径
	m_nLife = EFFECT_3D_LIFE;					// 寿命
	m_nMaxLife = m_nLife;						// 最大寿命(固定)
	m_moveType = MOVEEFFECT_NONE;				// 移動の種類
	m_nType = TYPE_NORMAL;						// 種類
	m_bAddAlpha = true;							// 加算合成の判定
	m_fGravity = mylib_const::GRAVITY;

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 初期化処理
	hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{// 失敗したとき

		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// エフェクトの初期化処理
//==========================================================================
HRESULT CEffect3D::Init(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type)
{
	HRESULT hr;

	// 各種変数の初期化
	m_posOrigin = pos;							// 原点
	m_updatePosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 更新後の位置
	m_setupPosition = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// セットアップ位置
	SetPosition(pos);							// 位置
	SetMove(move);								// 移動量
	m_colOrigin = col;							// 色の元
	SetColor(col);								// 色
	m_fRadius = fRadius;						// 半径
	m_fMaxRadius = m_fRadius;					// 最大半径
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));	// サイズ設定
	m_nLife = nLife;							// 寿命
	m_nMaxLife = m_nLife;						// 最大寿命(固定)
	m_moveType = moveType;						// 移動の種類
	m_nType = type;								// 種類

	if (m_nType >= TYPE_MAX)
	{
		int n = 0;
	}

	// 加算合成の判定
	switch (m_nType)
	{
	case TYPE_NORMAL:
		m_bAddAlpha = true;
		break;

	case TYPE_POINT:
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

	case TYPE_THUNDER:
		m_bAddAlpha = true;
		break;

	case TYPE_WATER:
		m_bAddAlpha = true;
		break;

	default:
		m_bAddAlpha = true;
		break;
	}

	// Zソートのフラグ
	m_bZSort = true;

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// テクスチャの割り当て
	if (m_nTexIdx[m_nType] == 0)
	{
		m_nTexIdx[m_nType] = CTexture::GetInstance()->Regist(m_apTextureFile[m_nType]);
	}

	// テクスチャの割り当て
	BindTexture(m_nTexIdx[m_nType]);

	// 初期化処理
	hr = CObjectBillboard::Init();

	if (FAILED(hr))
	{// 失敗したとき
		Uninit();
		return E_FAIL;
	}

	// 移動補正係数
	m_fMoveFactor = 0.15f;

	return S_OK;
}

//==========================================================================
// エフェクトの終了処理
//==========================================================================
void CEffect3D::Uninit()
{
	if (m_pParent != nullptr)
	{
		// エフェクトの開放
		m_pParent->ReleaseEffect(m_nParentIdx);
		m_pParent = nullptr;
	}

	// 総数減算
	m_nNumAll--;

	// 終了処理
	CObjectBillboard::Uninit();
}

//==========================================================================
// 親の破棄
//==========================================================================
void CEffect3D::UninitParent()
{
	// 親をnullptrにする
	m_pParent = nullptr;
}

//==========================================================================
// エフェクトの更新処理
//==========================================================================
void CEffect3D::Update()
{
	// 過去の位置設定
	SetOldPosition(GetPosition());

	// 移動処理
	UpdateMove();

	switch (m_moveType)
	{
	case MOVEEFFECT_ADD:
		AddSize();
		break;

	case MOVEEFFECT_SUB:
		SubSize();
		break;

	case MOVEEFFECT_SUPERSUB:
		SuperSubSize();
		break;

	case MOVEEFFECT_GENSUI:
		Gensui();
		break;
	}

	// サイズ設定
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

	// 寿命の更新
	m_nLife--;

	// 色取得
	D3DXCOLOR col = GetColor();

	// 不透明度の更新
	col.a = m_colOrigin.a * ((float)m_nLife / (float)m_nMaxLife);

	// 色設定
	SetColor(col);

	if (m_nLife <= 0)
	{// 寿命が尽きたら

		// エフェクトの削除
		Uninit();
		return;
	}

	// 頂点座標の設定
	SetVtx();

}

//==========================================================================
// 移動処理
//==========================================================================
void CEffect3D::UpdateMove()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posOld = GetOldPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 位置更新
	if (m_bGravity)
	{
		move.y -= m_fGravity;
	}

	// 補正別処理
	if (m_bChaseDest == false)
	{
		m_updatePosition += move;
		pos = m_posOrigin + m_updatePosition;
	}
	else
	{
		// 等速線形補間
		float fRatio = 1.0f - ((float)m_nLife / (float)m_nMaxLife);
		pos.x = UtilFunc::Correction::EasingLinear(m_posOrigin.x, m_posDest.x, fRatio);
		pos.y = UtilFunc::Correction::EasingLinear(m_posOrigin.y, m_posDest.y, fRatio);
		pos.z = UtilFunc::Correction::EasingLinear(m_posOrigin.z, m_posDest.z, fRatio);
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);
}


//==========================================================================
// セットアップ
//==========================================================================
void CEffect3D::SetUp(MyLib::Vector3 setup, D3DXMATRIX *pMtxParent, CObject *pObj, int nParentIdx)
{
	// 親のポインタ渡す
	if (m_pParent == nullptr)
	{
		m_pParent = pObj;
	}

	// 親マトリックスのポインタ
	m_pMtxParent = pMtxParent;

	// 親のインデックス番号
	m_nParentIdx = nParentIdx;

	// セットアップ位置
	m_setupPosition = setup;
}

//==========================================================================
// 位置更新
//==========================================================================
void CEffect3D::UpdatePosition(MyLib::Vector3 rot)
{

	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// 計算用マトリックス宣言

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, m_setupPosition.x, m_setupPosition.y, m_setupPosition.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// 自分に親のワールドマトリックスを掛ける
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, m_pMtxParent);

	m_posOrigin = UtilFunc::Transformation::WorldMtxChangeToPosition(mtxWorld);

}

//==========================================================================
// エフェクトの縮小処理
//==========================================================================
void CEffect3D::SubSize()
{
	int nEffect_3DType = rand() % 3;

	if (nEffect_3DType == 0)
	{
		m_fRadius *= EFFECT_3DSIZE1;
	}
	else if (nEffect_3DType == 1)
	{
		m_fRadius *= EFFECT_3DSIZE2;
	}
	else if (nEffect_3DType == 2)
	{
		m_fRadius *= EFFECT_3DSIZE3;
	}
}

//==========================================================================
// エフェクトの縮小処理
//==========================================================================
void CEffect3D::SuperSubSize()
{
	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;
}

//==========================================================================
// エフェクトの拡大処理
//==========================================================================
void CEffect3D::AddSize()
{
	// 拡大
	m_fRadius += m_fAddSizeValue;
}

//==========================================================================
// エフェクトの減衰処理
//==========================================================================
void CEffect3D::Gensui()
{
	// 移動量取得
	MyLib::Vector3 move = GetMove();

	move.x += (0.0f - move.x) * m_fMoveFactor;
	move.y += (0.0f - move.y) * m_fMoveFactor;
	move.z += (0.0f - move.z) * m_fMoveFactor;

	// 移動量設定
	SetMove(move);

	m_fRadius = m_fMaxRadius * (float)m_nLife / (float)m_nMaxLife;

}

//==========================================================================
// エフェクトの描画処理
//==========================================================================
void CEffect3D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (!m_bZSort)
	{
		// Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// 常に描画する
	}

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
	CObjectBillboard::Draw();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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
void CEffect3D::SetVtx()
{
	// 頂点設定
	CObjectBillboard::SetVtx();
}

//==========================================================================
// 目標の位置設定
//==========================================================================
void CEffect3D::SetPositionDest(MyLib::Vector3 pos)
{
	m_posDest = pos;
	m_bChaseDest = true;	// 有効
}

//==========================================================================
// 重力の値設定
//==========================================================================
void CEffect3D::SetGravityValue(float fValue)
{ 
	m_fGravity = fValue; 
	m_bGravity = true;	// 有効
}

//==========================================================================
// 総数取得
//==========================================================================
int CEffect3D::GetNumAll()
{
	return m_nNumAll;
}

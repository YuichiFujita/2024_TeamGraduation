//==========================================================================
// 
//  影処理 [shadow.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================
#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
//	定数宣言
//==========================================================================
namespace
{
	const float	MAX_DIS_HEIGHT	= 260.0f;	// 影と親の縦の距離の最大値
	const float	MAX_PLUS_RADIUS	= 50.0f;	// 影の大きさ加算量の最大値
}

//==========================================================================
// コンストラクタ
//==========================================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority),
	m_pObject	(nullptr),	// オブジェクトのポインタ
	m_fLandY	(0.0f),		// 表示Y座標
	m_nTexIdx	(0)			// テクスチャのインデックス番号
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CShadow::~CShadow()
{
	
}

//==========================================================================
// 生成処理(オーバーロード)
//==========================================================================
CShadow *CShadow::Create
(
	CObject* pObject,	// 親オブジェクト
	float fRadius,		// 元の半径
	float fMinAlpha,	// 最小透明度
	float fMaxAlpha,	// 最大透明度
	float fLandY		// 表示Y座標
)
{
	// 生成用のオブジェクト
	CShadow *pShadow = nullptr;
	if (pShadow == nullptr)
	{// nullptrだったら

		// メモリの確保
		pShadow = DEBUG_NEW CShadow;
		if (pShadow != nullptr)
		{// メモリの確保が出来ていたら

			// 追従オブジェクトの割当
			pShadow->BindObject(pObject);

			// 相対位置の設定
			pShadow->SetPositionRelative();

			// 大きさの設定
			pShadow->SetSize(MyLib::Vector3(fRadius, 0.0f, fRadius));

			// 元の半径の設定
			pShadow->m_fOriginRadius = fRadius;

			// 表示Y座標の設定
			pShadow->m_fLandY = fLandY;

			// 透明度の最小値
			pShadow->m_fMinAlpha = fMinAlpha;

			// 透明度の最大値
			pShadow->m_fMaxAlpha = fMaxAlpha;

			// 初期化処理
			pShadow->Init();
		}
		return pShadow;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CShadow::Init()
{
	HRESULT hr;

	// 各種変数の初期化
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));	// 色

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 初期化処理
	hr = CObject3D::Init();

	if (FAILED(hr))
	{// 失敗したとき

		return E_FAIL;
	}

	// テクスチャの割り当て
	m_nTexIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\effect\\effect000.jpg");

	// テクスチャの割り当て
	BindTexture(m_nTexIdx);

	// 向き設定
	SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CShadow::Uninit()
{
	// 終了処理
	CObject3D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CShadow::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 死亡の判定
	if (IsDeath()) { return; }

	// 相対位置の設定
	SetPositionRelative();

	// 頂点情報設定
	CObject3D::SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CShadow::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	// 描画処理
	CObject3D::Draw();

	// 減算合成をもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//==========================================================================
// 相対位置の設定処理
//==========================================================================
void CShadow::SetPositionRelative()
{
	MyLib::Vector3 posParent = m_pObject->GetPosition();	// 親オブジェクト位置
	MyLib::Vector3 posShadow = posParent;	// 影位置
	MyLib::Vector2 sizeShadow;	// 影大きさ
	float fDis = 0.0f;		// 影と親の距離
	float fAlpha = 0.0f;	// 透明度

	// Y座標を地面に設定
	posShadow.y = m_fLandY + 1.0f;

	// 影と親の縦座標の距離を求める
	fDis = fabsf(posParent.y - posShadow.y);								// 縦の距離を求める
	UtilFunc::Transformation::ValueNormalize(fDis, MAX_DIS_HEIGHT, 0.0f);	// 縦の距離を制限
	fDis *= 1.0f / MAX_DIS_HEIGHT;											// 距離を割合化

	// 影の大きさを求める
	float fRadius = m_fOriginRadius + (MAX_PLUS_RADIUS * fDis);	// 半径
	sizeShadow = fRadius;

	// α値を求める
	fAlpha = fabsf(fDis - 1.0f);	// α値を設定
	UtilFunc::Transformation::ValueNormalize(fAlpha, m_fMaxAlpha, m_fMinAlpha);	// α値を制限

	// 影の描画情報を設定
	SetPosition(posShadow);	// 位置
	SetSize(sizeShadow);	// 大きさ
	SetAlpha(fAlpha);		// 透明度
}

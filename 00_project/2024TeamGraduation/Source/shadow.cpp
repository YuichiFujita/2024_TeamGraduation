//=============================================================================
// 
//  影処理 [shadow.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define POS_SHADOW	(50.0f)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CShadow::m_nNumAll = 0;						// 影の総数

//==========================================================================
// コンストラクタ
//==========================================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{

	m_pObject = nullptr;	// オブジェクトのポインタ
	m_nNumAll = 0;		// 総数
	m_nTexIdx = 0;		// テクスチャのインデックス番号

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CShadow::~CShadow()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CShadow *CShadow::Create()
{
	// 生成用のオブジェクト
	CShadow *pShadow = nullptr;

	if (pShadow == nullptr)
	{// nullptrだったら

		// メモリの確保
		pShadow = DEBUG_NEW CShadow;

		if (pShadow != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pShadow->Init();
		}

		return pShadow;
	}

	return nullptr;
}

//==========================================================================
// 生成処理(オーバーロード)
//==========================================================================
CShadow *CShadow::Create(MyLib::Vector3 pos, float size)
{
	// 生成用のオブジェクト
	CShadow *pShadow = nullptr;

	if (pShadow == nullptr)
	{// nullptrだったら

		// メモリの確保
		pShadow = DEBUG_NEW CShadow;

		//if (pShadow->GetID() < 0)
		//{// メモリ確保に失敗していたら

		//	delete pShadow;
		//	return nullptr;
		//}

		if (pShadow != nullptr)
		{// メモリの確保が出来ていたら

			// 位置割り当て
			pShadow->SetPosition(pos);
			pShadow->SetSize(MyLib::Vector3(size, 0.0f, size));	// サイズ

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

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CShadow::Uninit()
{
	// 終了処理
	CObject3D::Uninit();

	// 総数減算
	m_nNumAll--;
}

//==========================================================================
// 更新処理
//==========================================================================
void CShadow::Update()
{
	// 死亡の判定
	if (IsDeath())
	{// 死亡フラグが立っていたら
		return;
	}

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 位置設定
	SetPosition(pos);

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
}

//==========================================================================
// 総数取得
//==========================================================================
int CShadow::GetNumAll()
{
	return m_nNumAll;
}

//=============================================================================
// 
// ステージ名処理 [stagename.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "stagename.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\stagename\\enhance.png";
	const float SPEED_VERTICAL = 0.3f;		// 上昇速度
	const float LIFE = 2.0f;				// 寿命
	const float TIME_START_FADEOUT = 1.3f;	// 生成してからフェードアウトされるまでの時間
}
CStageName* CStageName::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CStageName::CStageName(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_fAlphaTime = 0.0f;			// 不透明度タイマー
}

//==========================================================================
// デストラクタ
//==========================================================================
CStageName::~CStageName()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStageName* CStageName::Create()
{
	if (m_pThisPtr != nullptr)
	{
		return m_pThisPtr;
	}

	// メモリの確保
	m_pThisPtr = DEBUG_NEW CStageName;

	if (m_pThisPtr != nullptr)
	{
		// 初期化処理
		if (FAILED(m_pThisPtr->Init()))
		{
			return nullptr;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStageName::Init()
{
	// 初期化処理
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// テクスチャの割り当て
	int nIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nIdx);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nIdx);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 50.0f);
	SetSize(size);
	SetSizeOrigin(GetSize());

	// 位置設定
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));


	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStageName::Uninit()
{
	m_pThisPtr = nullptr;

	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CStageName::Update()
{
	// 不透明度時間加算
	m_fAlphaTime += CManager::GetInstance()->GetDeltaTime();

	if (m_fAlphaTime >= TIME_START_FADEOUT)
	{
		// 位置移動
		AddPosition(MyLib::Vector3(0.0f, -SPEED_VERTICAL, 0.0f));

		// 不透明度更新
		float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, TIME_START_FADEOUT, LIFE, m_fAlphaTime);
		SetAlpha(alpha);
	}

	// 更新処理
	CObject2D::Update();

	if (m_fAlphaTime >= LIFE)
	{
		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CStageName::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject2D::Draw();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

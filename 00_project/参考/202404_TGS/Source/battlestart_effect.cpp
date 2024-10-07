//=============================================================================
// 
//  戦闘開始処理 [battlestart_effect.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "battlestart_effect.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "particle.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\battlestart\\don3.png";	// テクスチャのファイル
	const float LIFE = 0.7f;	// 集まる時間
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBattleStart_Effect::CBattleStart_Effect(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_fLife = 0.0f;	// 寿命
}

//==========================================================================
// デストラクタ
//==========================================================================
CBattleStart_Effect::~CBattleStart_Effect()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CBattleStart_Effect* CBattleStart_Effect::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CBattleStart_Effect* pMarker = DEBUG_NEW CBattleStart_Effect;

	if (pMarker != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBattleStart_Effect::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 寿命
	m_fLife = LIFE;
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBattleStart_Effect::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBattleStart_Effect::Update()
{
	// 寿命減算
	m_fLife -= CManager::GetInstance()->GetDeltaTime();

	if (m_fLife <= 0.0f)
	{
		Uninit();
		return;
	}

	// 拡大
	D3DXVECTOR2 size = GetSize();
	size += GetSizeOrigin() * 0.1f;
	UtilFunc::Correction::InertiaCorrection(size.x, GetSizeOrigin().x * 3.0f, 0.08f);
	UtilFunc::Correction::InertiaCorrection(size.y, GetSizeOrigin().y * 3.0f, 0.08f);
	SetSize(size);

	float alpha = (m_fLife / LIFE) * 0.4f;
	SetAlpha(alpha);

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CBattleStart_Effect::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// オブジェクト2Dの描画
	CObject2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

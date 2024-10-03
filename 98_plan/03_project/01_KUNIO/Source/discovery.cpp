//=============================================================================
// 
//  発見マーク処理 [discovery.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "discovery.h"
#include "manager.h"
#include "calculation.h"
#include "instantfade.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\start\\gogolamp.png";	// テクスチャのファイル
}

// 状態別タイマー
namespace StateTime
{
	const float APPEARANCE = 0.2f;	// 出現
	const float WAIT = 1.0f;		// 待機
	const float FADEOUT = 0.15f;	// フェードアウト
}

//==========================================================================
// 関数リスト
//==========================================================================
CDiscovery::STATE_FUNC CDiscovery::m_StateFuncList[] =
{
	&CDiscovery::StateNone,			// なし
	&CDiscovery::StateAppearance,	// 出現
	&CDiscovery::StateWait,			// 待機
	&CDiscovery::StateFadeOut,		// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CDiscovery::CDiscovery(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	m_state = State::STATE_NONE;// 状態
	m_fStateTime = 0.0f;		// 状態カウンター
}

//==========================================================================
// デストラクタ
//==========================================================================
CDiscovery::~CDiscovery()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDiscovery* CDiscovery::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CDiscovery* pMarker = DEBUG_NEW CDiscovery;

	if (pMarker != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pMarker->SetPosition(pos);

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDiscovery::Init()
{
	// ビルボードの初期化
	CObjectBillboard::Init();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);
	SetSizeOrigin(size);
	SetSize(D3DXVECTOR2(0.0f, 0.0f));

	// 状態
	m_state = State::STATE_APPEARANCE;
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDiscovery::Uninit()
{
	// 終了処理
	CObjectBillboard::Uninit();
}

//==========================================================================
// 終了処理
//==========================================================================
void CDiscovery::Kill()
{
	// 終了処理
	CObjectBillboard::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CDiscovery::Update()
{
	// 状態更新
	UpdateState();

	if (CManager::GetInstance()->GetInstantFade()->GetState() != CInstantFade::STATE::STATE_NONE)
	{
		Uninit();
	}

	if (IsDeath()) return;

	// 更新処理
	CObjectBillboard::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CDiscovery::UpdateState()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// 通常
//==========================================================================
void CDiscovery::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// 出現
//==========================================================================
void CDiscovery::StateAppearance()
{
	// サイズ補正
	D3DXVECTOR2 size, sizeDest = GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeDest.x, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeDest.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		// 目標値に補正
		size = sizeDest;

		// 状態設定
		SetState(State::STATE_WAIT);
	}

	// サイズ設定
	SetSize(size);
}

//==========================================================================
// 待機
//==========================================================================
void CDiscovery::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		SetState(State::STATE_FADEOUT);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CDiscovery::StateFadeOut()
{
	// 不透明度設定
	SetAlpha(1.0f - (m_fStateTime / StateTime::FADEOUT));

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// 終了処理
		Uninit();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CDiscovery::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画処理
//==========================================================================
void CDiscovery::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ビルボードの描画
	CObjectBillboard::Draw();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

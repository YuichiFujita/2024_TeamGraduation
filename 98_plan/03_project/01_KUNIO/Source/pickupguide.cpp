//=============================================================================
// 
//  ピックアップガイド処理 [pickupguide.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "pickupguide.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE[] = // テクスチャのファイル
	{ 
		"data\\TEXTURE\\pickupguide\\transferguid_01.png",	// 転移
		"data\\TEXTURE\\pickupguide\\skilltreeguid_01.png",	// スキルツリー開始
	};
	const float TIME_FADE = 0.5f;	// フェードにかかる時間
	const float DISTANCE_UP = 20.0f;	// 上昇する間隔
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPickupGuide::STATE_FUNC CPickupGuide::m_StateFuncList[] =
{
	&CPickupGuide::StateNone,		// なにもなし
	&CPickupGuide::StateFadeIn,		// フェードイン
	&CPickupGuide::StateFadeOut,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPickupGuide::CPickupGuide(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
	m_type = TYPE_TRANSFER;	// 種類
}

//==========================================================================
// デストラクタ
//==========================================================================
CPickupGuide::~CPickupGuide()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPickupGuide* CPickupGuide::Create(const MyLib::Vector3& pos, TYPE type)
{
	// メモリの確保
	CPickupGuide* pMarker = DEBUG_NEW CPickupGuide;

	if (pMarker != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);

		// 種類
		pMarker->m_type = type;

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPickupGuide::Init()
{
	// ビルボードの初期化
	CObjectBillboard::Init();

	// 種類の設定
	SetType(TYPE_OBJECTBILLBOARD);

	// テクスチャ設定
	int type = static_cast<int>(m_type);
	int textureIdx = CTexture::GetInstance()->Regist(TEXTURE[type]);
	BindTexture(textureIdx);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(textureIdx);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 15.0f);
	SetSize(size);

	// 色設定
	SetAlpha(0.0f);

	// 初期値
	m_state = STATE_NONE;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPickupGuide::Uninit()
{
	CObjectBillboard::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPickupGuide::Update()
{
	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();

	// ビルボードの更新
	CObjectBillboard::Update();
}

//==========================================================================
// フェードイン
//==========================================================================
void CPickupGuide::StateFadeIn()
{
	// 状態カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTime >= TIME_FADE)
	{
		m_fStateTime = TIME_FADE;
		m_state = STATE_NONE;
	}

	// 位置設定
	MyLib::Vector3 pos = GetPosition();
	pos.y = GetOriginPosition().y + (m_fStateTime / TIME_FADE) * DISTANCE_UP;
	SetPosition(pos);

	// 不透明度設定
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, m_fStateTime / TIME_FADE);
	SetAlpha(alpha);
}

//==========================================================================
// フェードアウト
//==========================================================================
void CPickupGuide::StateFadeOut()
{
	// 状態カウンター減算
	m_fStateTime -= CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTime <= 0.0f)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_NONE;
	}

	// 位置設定
	MyLib::Vector3 pos = GetPosition();
	pos.y = GetOriginPosition().y + (m_fStateTime / TIME_FADE) * DISTANCE_UP;
	SetPosition(pos);

	// 不透明度設定
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, m_fStateTime / TIME_FADE);
	SetAlpha(alpha);
}

//==========================================================================
// 描画処理
//==========================================================================
void CPickupGuide::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

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

//==========================================================================
// 状態設定
//==========================================================================
void CPickupGuide::SetState(STATE state)
{
	m_state = state;
}

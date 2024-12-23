//==========================================================================
// 
//  カットイン処理 [cutin.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "cutin.h"
#include "manager.h"
#include "camera.h"
#include "calculation.h"
#include "renderTexture.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_BLINK_BG = "data\\TEXTURE\\cutin\\blink000.jpg";	// 背景テクスチャ
	const int	PRIORITY = 7;		// 優先順位
	const float	END_TIME = 1.2f;	// 終了時間

	namespace bg
	{
		const float START_ALPHA = 0.66f;	// 開始透明度
		const float END_ALPHA = 0.0f;		// 終了透明度
	}

	namespace blink
	{
		const MyLib::PosGrid3 START = MyLib::PosGrid3(63, 12, 100);	// 開始色
		const MyLib::PosGrid3 END = MyLib::PosGrid3(0, 100, 100);	// 終了色
		const float START_ALPHA = 0.7f;	// 開始透明度
		const float END_ALPHA = 0.1f;	// 終了透明度
	}
}

//==========================================================================
// コンストラクタ
//==========================================================================
CCutIn::CCutIn() : CObject2D(PRIORITY),
	m_pRenderScene	(nullptr),		// シーンレンダーテクスチャ
	m_pBlink		(nullptr),		// 輝き情報
	m_pPlayer2D		(nullptr),		// プレイヤーテクスチャ情報
	m_fBlinkTime	(0.0f)			// 背景輝きカウンター
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCutIn::~CCutIn()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCutIn* CCutIn::Create()
{
	// メモリの確保
	CCutIn* pObj = DEBUG_NEW CCutIn;
	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCutIn::Init()
{
	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置/大きさを設定
	SetPosition(VEC3_SCREEN_CENT);	// 画面中心にする
	SetSize(VEC2_SCREEN_SIZE);		// 画面全体にする

	// 半透明にする
	SetAlpha(bg::START_ALPHA);

	// 世界停止中に動けるようにする
	SetEnablePosibleMove_WorldPause(true);

	// レンダーテクスチャの生成
	if (FAILED(CreateRenderTexture()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// テクスチャ用オブジェクトの生成
	if (FAILED(CreateTextureObject()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// プレイヤーテクスチャ貼付ポリゴン/輝きの生成
	if (FAILED(CreateObject2D()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCutIn::Uninit()
{
	// 輝きの終了
	SAFE_UNINIT(m_pBlink);

	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CCutIn::Kill()
{
	// 輝きの削除
	SAFE_KILL(m_pBlink);

	// 削除処理
	CObject2D::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCutIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 背景色の更新
	UpdateBlinkColor(fDeltaTime, fDeltaRate, fSlowRate);

#if 0
	// TODO
	if (IsEnd())
	{
		Uninit();
		return;
	}
#endif

	// 輝きの更新
	m_pBlink->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 更新処理
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CCutIn::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

	// 描画処理
	CObject2D::Draw();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 輝きの描画
	m_pBlink->Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================
// 終了フラグ取得処理
//==========================================================================
bool CCutIn::IsEnd() const
{
	// 経過時間が終了時間を超えているかを返す
	return (m_fBlinkTime >= END_TIME);
}

//==========================================================================
// レンダーテクスチャ生成処理
//==========================================================================
HRESULT CCutIn::CreateRenderTexture()
{
	// シーンレンダーテクスチャの生成
	m_pRenderScene = CRenderTexture::Create
	( // 引数
		CRenderTextureManager::LAYER_BALLOON,		// 描画順レイヤー
		std::bind(&CCutIn::CreateTexture, this),	// テクスチャ作成関数ポインタ
		std::bind(&CCamera::SetCamera, GET_MANAGER->GetCamera())	// カメラ設定関数ポインタ
	);
	if (m_pRenderScene == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// テクスチャ用オブジェクト生成処理
//==========================================================================
HRESULT CCutIn::CreateTextureObject()
{
	return S_OK;
}

//==========================================================================
// プレイヤーテクスチャ貼付ポリゴン/輝き生成処理
//==========================================================================
HRESULT CCutIn::CreateObject2D()
{
	// 輝きの生成
	m_pBlink = CObject2D::Create(PRIORITY);
	if (m_pBlink == nullptr)
	{ // 生成に失敗した場合

		assert(false);
	}

	// 自動破棄/更新/描画をしない種類にする
	m_pBlink->SetType(CObject::TYPE::TYPE_NONE);

	// 位置/大きさを設定
	m_pBlink->SetPosition(VEC3_SCREEN_CENT);	// 画面中心にする
	m_pBlink->SetSize(VEC2_SCREEN_SIZE);		// 画面全体にする

	// 半透明にする
	m_pBlink->SetAlpha(blink::START_ALPHA);

	// テクスチャインデックスを設定
	m_pBlink->BindTexture(CTexture::GetInstance()->Regist(TEXTURE_BLINK_BG));

	// プレイヤーテクスチャ貼り付け用ポリゴンの生成
	m_pPlayer2D = CObject2D::Create(PRIORITY);
	if (m_pPlayer2D == nullptr)
	{ // 生成に失敗した場合

		assert(false);
	}

	// 位置/大きさを設定
	m_pPlayer2D->SetPosition(VEC3_SCREEN_CENT);	// 画面中心にする
	m_pPlayer2D->SetSize(VEC2_SCREEN_SIZE);		// 画面全体にする

	// プレイヤーのレンダーテクスチャインデックスを設定
	assert(m_pRenderScene != nullptr);	// 先レンダーテクスチャ生成して
	m_pPlayer2D->BindTexture(m_pRenderScene->GetTextureIndex());

	return S_OK;
}

//==========================================================================
// テクスチャ作成処理
//==========================================================================
void CCutIn::CreateTexture()
{

}

//==========================================================================
// 背景色の更新処理
//==========================================================================
void CCutIn::UpdateBlinkColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 start = blink::START;	// 開始時の背景HSV色
	MyLib::Vector3 end = blink::END;		// 終了時の背景HSV色
	MyLib::Vector3 current = VEC3_ZERO;		// 現在時の背景HSV色
	float fAlphaBG = 0.0f;					// 現在時の背景透明度
	float fAlphaBlink = 0.0f;				// 現在時の輝き透明度

	// 経過時間を加算
	m_fBlinkTime += fDeltaTime * fSlowRate;

	// 現在の背景HSV色を線形補完
	current = UtilFunc::Correction::EasingLinear(start, end, 0.0f, END_TIME, m_fBlinkTime);

	// 現在の背景透明度を線形補完
	fAlphaBG = UtilFunc::Correction::EaseInExpo(bg::START_ALPHA, bg::END_ALPHA, 0.0f, END_TIME, m_fBlinkTime);

	// 現在の透明度を線形補完
	fAlphaBlink = UtilFunc::Correction::EaseInExpo(blink::START_ALPHA, blink::END_ALPHA, 0.0f, END_TIME, m_fBlinkTime);

	// 背景の色情報をHSVからRGBに変換し反映
	D3DXCOLOR colRGB = UtilFunc::Transformation::HSVtoRGB(current.x, current.y, current.z);	// RGB色情報
	SetColor(D3DXCOLOR(colRGB.r, colRGB.g, colRGB.b, fAlphaBG));	// 色を反映

	// 輝きの透明度を反映
	m_pBlink->SetAlpha(fAlphaBlink);
}

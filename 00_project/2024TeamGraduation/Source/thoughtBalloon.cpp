//=============================================================================
// 
//  実況吹き出し処理 [thoughtBalloon.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "thoughtBalloon.h"
#include "renderTexture.h"
#include "balloonFrame.h"
#include "camera.h"
#include "loadtext.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	namespace text
	{
		const char*	FONT		= "data\\FONT\\Kazesawa Bold.ttf";		// フォントパス
		const int	PRIORITY	= 6;			// テキストの優先順位
		const bool	ITALIC		= false;		// イタリック
		const float	CHAR_HEIGHT	= 36.0f;		// 文字縦幅
		const float	LINE_HEIGHT	= 90.0f;		// 行間縦幅
		const float	WAIT_TIME	= 0.045f;		// 文字表示の待機時間
		const EAlignX ALIGN_X	= XALIGN_CENTER;	// 横配置
		const EAlignY ALIGN_Y	= YALIGN_CENTER;	// 縦配置
	}
	const float DEFAULTSCALE = 1.25f;	// 吹き出し全体のスケール
}

//==========================================================================
// コンストラクタ
//==========================================================================
CThoughtBalloon::CThoughtBalloon(int nPriority) : CObjectBillboard(nPriority),
	m_pRenderScene	(nullptr),	// シーンレンダーテクスチャ
	m_pText			(nullptr),	// テキスト情報
	m_pFrame		(nullptr)	// フレーム
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CThoughtBalloon::~CThoughtBalloon()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CThoughtBalloon* CThoughtBalloon::Create(CGameManager::ETeamSide side)
{
	// メモリの確保
	CThoughtBalloon* pObj = DEBUG_NEW CThoughtBalloon;
	if (pObj != nullptr)
	{
		// 引数情報設定
		pObj->m_TeamSide = side;	// チームサイド

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
HRESULT CThoughtBalloon::Init()
{
	// オブジェクトビルボードの初期化
	if (FAILED(CObjectBillboard::Init()))
	{ // 初期化に失敗した場合

		return E_FAIL;
	}

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

	// テクスチャインデックスの設定
	BindTexture(m_pRenderScene->GetTextureIndex());

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CThoughtBalloon::Uninit()
{
	// レンダーテクスチャの破棄
	SAFE_REF_RELEASE(m_pRenderScene);

	// テキストの終了
	SAFE_UNINIT(m_pText);

	// 枠の終了
	SAFE_UNINIT(m_pFrame);

	// オブジェクトビルボードの終了
	CObjectBillboard::Uninit();
}

//==========================================================================
// 削除処理
//==========================================================================
void CThoughtBalloon::Kill()
{
	// テキストの削除
	SAFE_KILL(m_pText);

	// 枠の削除
	SAFE_KILL(m_pFrame);

	// 自身の終了
	CThoughtBalloon::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CThoughtBalloon::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// 枠の更新
	assert(m_pFrame != nullptr);
	m_pFrame->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// テキストの更新
	assert(m_pText != nullptr);
	m_pText->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// オブジェクトビルボードの更新
	CObjectBillboard::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CThoughtBalloon::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// オブジェクトビルボードの描画
	CObjectBillboard::Draw();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// レンダーテクスチャ生成処理
//==========================================================================
HRESULT CThoughtBalloon::CreateRenderTexture()
{
	// シーンレンダーテクスチャの生成
	m_pRenderScene = CRenderTexture::Create
	( // 引数
		CRenderTextureManager::LAYER_BALLOON,						// 描画順レイヤー
		std::bind(&CThoughtBalloon::CreateTexture, this),			// テクスチャ作成関数ポインタ
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
HRESULT CThoughtBalloon::CreateTextureObject()
{
	// 設定する助０る
	const float scale = DEFAULTSCALE;

	// テキストの生成
	m_pText = CScrollText2D::Create
	( // 引数
		text::FONT,			// フォントパス
		text::ITALIC,		// イタリック
		MyLib::Vector3(640.0f, 370.0f, 0.0f),			// 原点位置
		text::WAIT_TIME,	// 文字表示の待機時間
		text::CHAR_HEIGHT * scale,	// 文字縦幅
		text::LINE_HEIGHT * scale,	// 行間縦幅
		text::ALIGN_X,		// 横配置
		text::ALIGN_Y		// 縦配置
	);
	if (m_pText == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 自動破棄/更新/描画をしない種類に変更
	m_pText->SetType(CObject::TYPE::TYPE_NONE);

	// 文字送りを開始する
	m_pText->SetEnableScroll(true);

	// 黒文字
	m_pText->SetColor(MyLib::color::Black());

	// テキストを割当
	loadtext::BindText(m_pText, loadtext::LoadText("data\\TEXT\\thought\\speech.txt", UtilFunc::Transformation::Random(0, 18)));

	// 枠の生成
	MyLib::Vector2 setSize = MyLib::Vector2(m_pText->GetTextWidth() * 2.0f, m_pText->GetTextHeight() + 50.0f);
	m_pFrame = CBalloonFrame::Create(setSize * scale, m_TeamSide);
	m_pFrame->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// テクスチャ作成処理
//==========================================================================
void CThoughtBalloon::CreateTexture()
{
	// 枠の描画
	assert(m_pFrame != nullptr);
	m_pFrame->Draw();

	// テキストの描画
	assert(m_pText != nullptr);
	m_pText->Draw();
}

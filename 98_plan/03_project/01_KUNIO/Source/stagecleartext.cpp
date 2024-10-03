//=============================================================================
// 
// ステージクリアテキスト処理 [stagecleartext.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "stagecleartext.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"
#include "player.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\battlewin\\goal.png";
	const float TIME_EXPANSION = 0.6f;			// 拡大
	const float TIME_EXPNONE = 1.0f;			// 拡大後何もしない
	const float TIME_FADEOUT = 0.3f;			// フェードアウト時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CStageClearText::STATE_FUNC CStageClearText::m_StateFuncList[] =
{
	&CStageClearText::StateExpansion,
	&CStageClearText::StateExpNone,
	&CStageClearText::StateFadeOut,
};

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CStageClearText::CStageClearText(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = STATE_EXPANSION;			// 状態
	m_fStateTimer = 0.0f;			// 状態タイマー
	m_bCreateResultWindow = false;	// リザルト画面の呼び出しフラグ
}

//==========================================================================
// デストラクタ
//==========================================================================
CStageClearText::~CStageClearText()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStageClearText* CStageClearText::Create(const MyLib::Vector3 pos)
{
	// メモリの確保
	CStageClearText* pEffect = DEBUG_NEW CStageClearText;

	if (pEffect != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		pEffect->Init();
		pEffect->SetPosition(pos);
	}

	return pEffect;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStageClearText::Init()
{
	HRESULT hr;

	// 初期化処理
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// テクスチャの割り当て
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 500.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// 開始
	m_fStateTimer = 0.0f;
	m_state = eState::STATE_EXPANSION;

	// 向き設定
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	SetOriginRotation(GetRotation());

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStageClearText::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CStageClearText::Update()
{
	// 状態更新
	UpdateState();

	if (IsDeath())return;;

	// 頂点座標の設定
	SetVtx();
}

//==========================================================================
// 状態更新
//==========================================================================
void CStageClearText::UpdateState()
{
	// 状態遷移カウンター加算
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// 拡大
//==========================================================================
void CStageClearText::StateExpansion()
{
	
	float alpha = UtilFunc::Correction::EaseInBack(0.2f, 1.0f, 0.0f, TIME_EXPANSION, m_fStateTimer);
	SetAlpha(alpha);
	
	// サイズ設定
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EaseInBack(GetSizeOrigin().x * 0.2f, GetSizeOrigin().x, 0.0f, TIME_EXPANSION, m_fStateTimer);
	size.y = UtilFunc::Correction::EaseInBack(GetSizeOrigin().y * 0.2f, GetSizeOrigin().y, 0.0f, TIME_EXPANSION, m_fStateTimer);
	SetSize(size);

	if (m_fStateTimer >= TIME_EXPANSION)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE_EXPNONE;
	}
}

//==========================================================================
// 整い状態
//==========================================================================
void CStageClearText::StateExpNone()
{
	
	if (m_fStateTimer >= TIME_EXPNONE)
	{
		m_fStateTimer = 0.0f;
		m_state = eState::STATE_FADEOUT;
		return;
	}
}

//==========================================================================
// フェードアウト状態
//==========================================================================
void CStageClearText::StateFadeOut()
{
	
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_FADEOUT, m_fStateTimer);
	SetAlpha(alpha);

	if (TIME_FADEOUT * 0.7f <= m_fStateTimer &&
		!m_bCreateResultWindow)
	{
		m_bCreateResultWindow = true;

		// 戦果生成
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_GOAL);
	}

	if (m_fStateTimer >= TIME_FADEOUT)
	{
		m_fStateTimer = 0.0f;
		Uninit();
		return;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CStageClearText::Draw()
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

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CStageClearText::SetVtx()
{
	// 頂点設定
	CObject2D::SetVtx();
}


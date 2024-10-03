//=============================================================================
// 
// ゴールゲームテキスト処理 [goalgametext.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "goalgametext.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\battlewin\\message.png";
	const float MULTIPLY_SIZE = 8.0f;	// サイズの倍率
	const float TIME_EXPANSION = 0.6f;	// 拡大
	const float TIME_EXPNONE = 1.0f;	// 拡大後何もしない
	const float TIME_FADEOUT = 0.3f;	// フェードアウト時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CGoalGameText::STATE_FUNC CGoalGameText::m_StateFuncList[] =
{
	&CGoalGameText::StateExpansion,	// 拡大
	&CGoalGameText::StateExpNone,	// 拡大後何もしない
	&CGoalGameText::StateFadeOut,	// フェードアウト状態
};

//==========================================================================
// コンストラクタ
//==========================================================================
CGoalGameText::CGoalGameText(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = STATE_EXPANSION;		// 状態
	m_fStateTimer = 0.0f;			// 状態タイマー
}

//==========================================================================
// デストラクタ
//==========================================================================
CGoalGameText::~CGoalGameText()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGoalGameText* CGoalGameText::Create()
{
	// メモリの確保
	CGoalGameText* pEffect = DEBUG_NEW CGoalGameText;

	if (pEffect != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		pEffect->Init();
	}

	return pEffect;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGoalGameText::Init()
{
	// 初期化処理
	if (FAILED(CObject2D::Init()))
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
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 400.0f);
	SetSize(size * 8.0f);
	SetSizeOrigin(size);

	// 位置設定
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// 開始
	m_fStateTimer = 0.0f;
	m_state = eState::STATE_EXPANSION;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGoalGameText::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGoalGameText::Update()
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
void CGoalGameText::UpdateState()
{
	// 状態遷移カウンター加算
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// 拡大
//==========================================================================
void CGoalGameText::StateExpansion()
{
	// 割合
	float ratio = UtilFunc::Correction::EaseInExpo(0.0f, 1.0f, 0.0f, TIME_EXPANSION, m_fStateTimer);

	// 透明度設定
	SetAlpha(ratio);

	// サイズ設定
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = (sizeOrigin.x * MULTIPLY_SIZE) + (sizeOrigin.x - (sizeOrigin.x * MULTIPLY_SIZE)) * ratio;
	size.y = (sizeOrigin.y * MULTIPLY_SIZE) + (sizeOrigin.y - (sizeOrigin.y * MULTIPLY_SIZE)) * ratio;
	SetSize(size);

	// 時間経過
	if (m_fStateTimer >= TIME_EXPANSION)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE_EXPNONE;
	}
}

//==========================================================================
// 整い状態
//==========================================================================
void CGoalGameText::StateExpNone()
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
void CGoalGameText::StateFadeOut()
{
	
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_FADEOUT, m_fStateTimer);
	SetAlpha(alpha);

	if (m_fStateTimer >= TIME_FADEOUT)
	{
		m_fStateTimer = 0.0f;
		Uninit();
		return;
	}
}

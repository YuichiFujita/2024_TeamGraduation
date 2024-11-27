//==========================================================================
// 
//  ゲームスタートの文字処理 [startText.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "startText.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\visual\\startText.jpg";	// テクスチャのファイル
}

namespace StateTime
{
	const float FADEIN = 0.3f;	// フェードイン
	const float FADEOUT = 0.3f;	// フェードアウト
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CStartText::STATE_FUNC CStartText::m_StateFuncList[] =
{
	&CStartText::StateNone,		// なにもない状態
	&CStartText::StateFadeIn,	// フェードイン状態
	&CStartText::StateFadeOut,	// フェードアウト状態
};

//==========================================================================
// コンストラクタ
//==========================================================================
CStartText::CStartText(int nPriority) : CObject2D(nPriority),
	m_fStateTime	(0.0f),					// 状態カウンター
	m_state			(EState::STATE_NONE)	// 状態
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CStartText::~CStartText()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStartText* CStartText::Create()
{
	// メモリの確保
	CStartText* pObj = DEBUG_NEW CStartText;
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
HRESULT CStartText::Init()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);
#endif

	SetSize(size);
	SetSizeOrigin(size);

	// 画面中央の位置にする
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 状態設定
	SetState(EState::STATE_FADEIN);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStartText::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CStartText::Kill()
{
	// 削除処理
	CObject2D::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CStartText::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// 更新中に消えてたら終了
	if (IsDeath()) return;

	// 更新処理
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CStartText::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CStartText::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマーを加算
	m_fStateTime += fDeltaTime * fSlowRate;
	
	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// フェードイン
//==========================================================================
void CStartText::StateFadeIn()
{
	// 不透明度割り出し
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		// 状態遷移
		SetState(EState::STATE_FADEOUT);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CStartText::StateFadeOut()
{
	// 不透明度割り出し
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		// 削除
		Kill();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CStartText::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

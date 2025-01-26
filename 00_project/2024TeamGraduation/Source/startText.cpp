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
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\visual\\startText.png";	// テクスチャのファイル
	const MyLib::Vector3 INIT_POS = MyLib::Vector3(-VEC3_SCREEN_SIZE.x, 360.0f, 0.0f);		// 初期位置
	const MyLib::Vector3 MIDDLE_POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, INIT_POS.y, 0.0f);	// 中間位置
	const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_SIZE.x * 2.0f, INIT_POS.y, 0.0f);	// 目標位置
	const float WIDTH = 380.0f;	// 横幅
}

namespace StateTime
{
	const float FADEIN	= 0.4f;	// フェードイン
	const float WAIT	= 0.9f;	// 待機
	const float FADEOUT	= 0.4f;	// フェードアウト
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CStartText::STATE_FUNC CStartText::m_StateFuncList[] =
{
	&CStartText::StateNone,		// なにもない状態
	&CStartText::StateFadeIn,	// フェードイン状態
	&CStartText::StateWait,		// 待機状態
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

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, WIDTH);
	SetSize(size);
	SetSizeOrigin(size);

	// 画面中央の位置にする
	SetPosition(INIT_POS);

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
	// 背景の位置を移動
	MyLib::Vector3 posGameSet = UtilFunc::Correction::EaseOutBack(INIT_POS, MIDDLE_POS, 0.0f, StateTime::FADEIN, m_fStateTime, 1.0f);
	SetPosition(posGameSet);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		// 状態遷移
		SetState(EState::STATE_WAIT);
	}
}

//==========================================================================
// 待機
//==========================================================================
void CStartText::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
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
	// 背景の位置を移動
	MyLib::Vector3 posGameSet = UtilFunc::Correction::EasingCubicOut(MIDDLE_POS, DEST_POS, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetPosition(posGameSet);

	if (m_fStateTime >= StateTime::FADEOUT)
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

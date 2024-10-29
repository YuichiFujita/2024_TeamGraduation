//==========================================================================
// 
//  カットイン処理 [cutin.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "cutin.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// テクスチャのファイル
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CCutIn::STATE_FUNC CCutIn::m_StateFuncList[] =
{
	nullptr,				// なにもない状態
	&CCutIn::UpdateTime,	// 時間経過状態
	nullptr,				// 終了状態
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCutIn::CCutIn(int nPriority) : CObject2D(nPriority),
	m_fStateTime	(0.0f),			// 状態カウンター
	m_state			(STATE_TIME)	// 状態
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
	SetPosition(MyLib::Vector3(100.0f, 100.0f, 0.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCutIn::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CCutIn::Kill()
{
	// 削除処理
	CObject2D::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCutIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態カウンター加算
	m_fStateTime += fDeltaTime * fSlowRate;

	if (m_StateFuncList[m_state] != nullptr)
	{ // 関数がある場合

		// 状態別処理
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 更新処理
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CCutIn::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// 時間経過状態の更新処理
//==========================================================================
void CCutIn::UpdateTime(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマーを加算
	m_fStateTime += fDeltaTime;
	if (m_fStateTime >= 3.0f)
	{ // 待機が終了した場合

		// タイマーを初期化
		m_fStateTime = 0.0f;

		// 終了状態にする
		m_state = STATE_END;
	}
}

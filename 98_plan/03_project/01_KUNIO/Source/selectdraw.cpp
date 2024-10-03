//=============================================================================
// 
//  選択肢の筆処理 [selectdraw.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "selectdraw.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\ui_setting\\front.png";
	const float HEIGHT = 50.0f;

}

// 状態別タイマー
namespace StateTime
{
	const float FADEIN = 0.15f;		// フェードイン
	const float FADEOUT = 0.15f;		// フェードアウト
	const float DRAW = 0.15f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CSelectDraw::STATE_FUNC CSelectDraw::m_StateFuncList[] =
{
	&CSelectDraw::StateNone,		// なし
	&CSelectDraw::StateFadeIn,	// フェードイン
	&CSelectDraw::StateFadeout,	// フェードアウト
	&CSelectDraw::StateDrawing,	// 書く
};

//==========================================================================
// コンストラクタ
//==========================================================================
CSelectDraw::CSelectDraw(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = State::STATE_NONE;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
}

//==========================================================================
// デストラクタ
//==========================================================================
CSelectDraw::~CSelectDraw()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSelectDraw* CSelectDraw::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CSelectDraw* pObj = DEBUG_NEW CSelectDraw;

	if (pObj != nullptr)
	{
		// 引数情報設定
		pObj->SetPosition(pos);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSelectDraw::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// 各種変数初期化
	m_state = State::STATE_FADEIN;	// 状態
	m_fStateTime = 0.0f;			// 状態カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);


	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, HEIGHT);

	// サイズ設定
	SetSize(D3DXVECTOR2(0.0f, size.y));
	SetSizeOrigin(size);

	// アンカーポイント設定
	SetAnchorType(CObject2D::AnchorPoint::LEFT);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSelectDraw::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSelectDraw::Update()
{

	// 状態更新
	UpdateState();

	if (IsDeath()) return;

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CSelectDraw::UpdateState()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 実行
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CSelectDraw::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードイン
//==========================================================================
void CSelectDraw::StateFadeIn()
{
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADEIN, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEIN)
	{
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CSelectDraw::StateFadeout()
{
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADEOUT, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		Uninit();
	}
}

//==========================================================================
// 書く
//==========================================================================
void CSelectDraw::StateDrawing()
{
	// 書くように拡縮
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = UtilFunc::Correction::EaseInExpo(0.0f, sizeOrigin.x, 0.0f, StateTime::DRAW, m_fStateTime);
	SetSize(size);

	// UV座標設定
	D3DXVECTOR2* pTex = GetTex();
	pTex[1].x = pTex[3].x = UtilFunc::Transformation::Clamp(size.x / sizeOrigin.x, 0.0f, 1.0f);

	if (m_fStateTime >= StateTime::DRAW)
	{
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CSelectDraw::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// 状態設定
//==========================================================================
void CSelectDraw::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 位置設定
//==========================================================================
void CSelectDraw::SetPosition(const MyLib::Vector3& pos)
{
	CObject2D::SetPosition(pos - MyLib::Vector3(GetSizeOrigin().x, 0.0f, 0.0f));
}

//==========================================================================
// 元の位置設定
//==========================================================================
void CSelectDraw::SetOriginPosition(const MyLib::Vector3& pos)
{
	CObject2D::SetOriginPosition(pos - MyLib::Vector3(GetSizeOrigin().x, 0.0f, 0.0f));
}
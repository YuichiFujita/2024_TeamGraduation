//==========================================================================
// 
//  モテ文字処理 [charmText.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "charmText.h"
#include "player.h"
#include "object2D.h"
#include "charmManager.h"

// 派生先
#include "charmText_Left.h"
#include "charmText_Right.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_FACE = "data\\TEXTURE\\faceicon\\000.png";			// 顔アイコンのテクスチャ
	const std::string TEXTURE_TEXT = "data\\TEXTURE\\charmtext\\window_full.png";	// テキストのテクスチャ
	const float SIZE_FACE = 20.0f;	// 顔アイコンのサイズ
	const MyLib::Vector3 OFFSET_TEXT = MyLib::Vector3(SIZE_FACE, 0.0f, 0.0f);	// テキストのオフセット位置
	const int COUNT_FADESTART = 5;	// フェード開始するカウント
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CCharmText::STATE_FUNC CCharmText::m_StateFunc[] =	// シーン関数
{
	&CCharmText::StateFadeIn,	// フェードイン
	&CCharmText::StateWait,		// 待機
	&CCharmText::StateFadeOut,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCharmText::CCharmText(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state(EState::STATE_FADEIN),	// 状態
	m_fStateTime(0.0f),				// 状態タイマー
	m_nCntUp(0)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCharmText::~CCharmText()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCharmText* CCharmText::Create(CGameManager::ETeamSide side)
{
	// メモリの確保
	CCharmText* pMarker = nullptr;

	switch (side)
	{
	case CGameManager::SIDE_LEFT:
		pMarker = DEBUG_NEW CCharmText_Left;
		break;

	case CGameManager::SIDE_RIGHT:
		pMarker = DEBUG_NEW CCharmText_Right;
		break;

	default:
		assert(false);
		break;
	}

	if (pMarker != nullptr)
	{
		// クラスの初期化
		if (FAILED(pMarker->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pMarker);
			return nullptr;
		}
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCharmText::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 顔アイコン生成
	if (FAILED(CreateFace()))
	{
		return E_FAIL;
	}

	// 文字生成
	if (FAILED(CreateText()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 顔アイコン生成
//==========================================================================
HRESULT CCharmText::CreateFace()
{
	// 生成処理
	m_pFace = CObject2D::Create(GetPriority());
	if (m_pFace == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pFace->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_FACE);
	m_pFace->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SIZE_FACE);
	m_pFace->SetSize(size);
	m_pFace->SetSizeOrigin(size);

	return S_OK;
}

//==========================================================================
// 文字生成
//==========================================================================
HRESULT CCharmText::CreateText()
{
	// 生成処理
	m_pText = CObject2D::Create(GetPriority());
	if (m_pText == nullptr) return E_FAIL;

	// 左端をアンカーポイントにする
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);

	// オブジェクトの種類設定
	m_pText->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_TEXT);
	m_pText->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_FACE);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCharmText::Uninit()
{
	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CCharmText::Kill()
{
	// 顔アイコン
	if (m_pFace != nullptr)
	{
		m_pFace->Kill();
		m_pFace = nullptr;
	}

	// 文字
	if (m_pText != nullptr)
	{
		m_pText->Kill();
		m_pText = nullptr;
	}

	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCharmText::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	CCharmText::UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態更新
//==========================================================================
void CCharmText::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// シーン別更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// フェードイン
//==========================================================================
void CCharmText::StateFadeIn()
{

	if (m_fStateTime >= STATETIME_FADEIN)
	{// フェードイン完了
		SetState(EState::STATE_WAIT);
	}
}

//==========================================================================
// 待機
//==========================================================================
void CCharmText::StateWait()
{
	if (m_nCntUp >= COUNT_FADESTART ||
		m_fStateTime >= STATETIME_WAIT)
	{// 待機終了
		SetState(EState::STATE_FADEOUT);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CCharmText::StateFadeOut()
{

	if (m_fStateTime >= STATETIME_FADEOUT)
	{// フェードアウト
		Kill();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCharmText::Draw()
{

}

//==========================================================================
// 状態設定
//==========================================================================
void CCharmText::SetState(EState state)
{
	m_fStateTime = 0.0f;
	m_state = state;
}

//==========================================================================
// 描画状況の設定処理
//==========================================================================
void CCharmText::SetEnableDisp(bool bDisp)
{
	// 基底クラスの描画状況設定
	CObject::SetEnableDisp(bDisp);

	// 描画状況設定
	m_pFace->SetEnableDisp(bDisp);
	m_pText->SetEnableDisp(bDisp);
}

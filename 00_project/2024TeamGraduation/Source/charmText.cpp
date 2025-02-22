//==========================================================================
// 
//  モテ実況吹き出し処理 [charmText.cpp]
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
	const std::string TEXTURE_FACE = "data\\TEXTURE\\faceicon\\000.png";	// 顔アイコンのテクスチャ
	const std::string TEXTURE_TEXT = "data\\TEXTURE\\speech\\sample.png";	// テキストのテクスチャ

	const float SIZE_FACE = 40.0f;	// 顔アイコンのサイズ
	const float SIZE_SPEECH = 120.0f;	// スピーチのサイズ
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
	m_state				(EState::STATE_FADEIN),	// 状態
	m_fStateTime		(0.0f),					// 状態タイマー
	m_nCntUp			(0),					// 上昇カウント
	m_bPossibleChain	(true)					// チェイン可能フラグ
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
		// 引数情報設定
		pMarker->m_TeamSide = side;	// チームサイド

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

	// 実況吹き出し生成
	if (FAILED(CreateText()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 実況吹き出し生成
//==========================================================================
HRESULT CCharmText::CreateText()
{
	// 生成処理
	m_pThoughtBalloon = CThoughtBalloon::Create(m_TeamSide);
	if (m_pThoughtBalloon == nullptr) return E_FAIL;

	// 左端をアンカーポイントにする
	m_pThoughtBalloon->SetAnchorType(CObjectBillboard::EAnchorPoint::LEFT);

	// オブジェクトの種類設定
	m_pThoughtBalloon->CObject::SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// サイズ設定
	MyLib::Vector2 size = MyLib::Vector2(80.0f, 40.0f);

	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_SPEECH);
	m_pThoughtBalloon->SetSize(MyLib::Vector2());
	m_pThoughtBalloon->SetSizeOrigin(size);

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
	// 実況吹き出し
	if (m_pThoughtBalloon != nullptr)
	{
		m_pThoughtBalloon->Kill();
		m_pThoughtBalloon = nullptr;
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
	// サイズ更新
	MyLib::Vector2 size, sizeOrigin;
	
	{// テキスト
		sizeOrigin = m_pThoughtBalloon->GetSizeOrigin();
		size.x = UtilFunc::Correction::EasingQuintOut(sizeOrigin.x * 0.0f, sizeOrigin.x, 0.0f, STATETIME_FADEIN, m_fStateTime);
		size.y = UtilFunc::Correction::EasingQuintOut(sizeOrigin.y * 0.0f, sizeOrigin.y, 0.0f, STATETIME_FADEIN, m_fStateTime);
		m_pThoughtBalloon->SetSize(size);
	}

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
	// チェイン可能
	//m_bPossibleChain = true;

	if (m_nCntUp >= COUNT_FADESTART ||
		m_fStateTime >= STATETIME_WAIT)
	{// 待機終了

		// チェイン不可能
		m_bPossibleChain = false;

		// 状態遷移
		SetState(EState::STATE_FADEOUT);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CCharmText::StateFadeOut()
{
	// チェイン不可能
	m_bPossibleChain = false;

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
	m_pThoughtBalloon->SetEnableDisp(bDisp);
}

//==========================================================================
// チェインの確認
//==========================================================================
void CCharmText::CheckChain()
{

}

//==========================================================================
// チェイン設定
//==========================================================================
void CCharmText::SetChain(int nChainIdx)
{

}
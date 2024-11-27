//==========================================================================
// 
//  モテ文字処理 [charmText_Right.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "charmText_Right.h"
#include "player.h"
#include "object2D.h"
#include "charmManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_FACE = "data\\TEXTURE\\faceicon\\000.png";			// 顔アイコンのテクスチャ
	const std::string TEXTURE_TEXT = "data\\TEXTURE\\charmtext\\window_full.png";	// テキストのテクスチャ

}

namespace Position
{
	const MyLib::Vector3 START(SCREEN_WIDTH + 100.0f, 600.0f, 0.0f);	// 開始位置
	const MyLib::Vector3 FADEIN(SCREEN_WIDTH - 60.0f, 600.0f, 0.0f);	// フェードイン位置
	const MyLib::Vector3 FADEOUT(0.0f, -80.0f, 0.0f);					// 終了位置
}

//==========================================================================
// 静的メンバ変数定義
//==========================================================================
CListManager<CCharmText_Right> CCharmText_Right::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CCharmText_Right::CCharmText_Right() : CCharmText()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCharmText_Right::~CCharmText_Right()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCharmText_Right::Init()
{
	// 初期化処理
	CCharmText::Init();

	// 顔の位置
	if (m_pFace != nullptr)
	{
		m_pFace->SetPosition(Position::START);
	}

	// テキストの位置
	if (m_pText != nullptr)
	{
		m_pText->SetAnchorType(CObject2D::AnchorPoint::RIGHT);
		m_pText->SetPosition(Position::START - m_pFace->GetSize().x);
	}

	// 既に存在しているものを上げる
	CListManager<CCharmText_Right>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ループ
		(*itr)->AddCountUP(1);
	}

	// リスト追加
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCharmText_Right::Uninit()
{
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CCharmText::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCharmText_Right::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 更新処理
	CCharmText::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 終了で抜ける
	if (IsDeath()) return;

	// 位置設定
	MyLib::Vector3 pos = GetPosition();

	// 間隔分上にあげる
	pos.y -= DISTANCE_UP * m_nCntUp;

	m_pFace->SetPosition(pos);
	m_pText->SetPosition(pos - MyLib::Vector3(m_pFace->GetSize().x, 0.0f, 0.0f));
}

//==========================================================================
// フェードイン
//==========================================================================
void CCharmText_Right::StateFadeIn()
{
	// 割合
	float ratio = UtilFunc::Correction::EaseInExpo(0.0f, 1.0f, 0.0f, STATETIME_FADEIN, m_fStateTime);

	// 位置更新
	MyLib::Vector3 pos = Position::START + (Position::FADEIN - Position::START) * ratio;
	SetPosition(pos);

	// 不透明度更新
	m_pFace->SetAlpha(ratio);
	m_pText->SetAlpha(ratio);

	// フェードイン
	CCharmText::StateFadeIn();
}

//==========================================================================
// 待機
//==========================================================================
void CCharmText_Right::StateWait()
{
	// 待機
	CCharmText::StateWait();
}

//==========================================================================
// フェードアウト
//==========================================================================
void CCharmText_Right::StateFadeOut()
{
	// 割合
	float ratio = UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, 0.0f, STATETIME_FADEOUT, m_fStateTime);

	// 位置更新
	MyLib::Vector3 pos = Position::FADEIN;
	pos.y += Position::FADEOUT.y * ratio;
	SetPosition(pos);

	// 不透明度更新
	m_pFace->SetAlpha(1.0f - ratio);
	m_pText->SetAlpha(1.0f - ratio);

	// フェードアウト
	CCharmText::StateFadeOut();
}

//==========================================================================
// チェインの確認
//==========================================================================
void CCharmText_Right::CheckChain()
{
	std::vector<int> chainIdx;	// チェインするインデックス

	// 既に存在しているものから確認
	CListManager<CCharmText_Right>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ループ

		// ポインタ変換
		CCharmText_Right* pText = (*itr);

		// チェイン可能フラグ取得
		bool bPossibleChain = pText->IsPossibleChain();

		if (bPossibleChain)
		{// チェイン可能なやつと、そいつがもっている全てとチェインする

			// 既にチェインしているやつとそいつのインデックス取得
			chainIdx = pText->GetChainIdx();
			int pairIdx = m_List.FindIdx(pText);

			// インデックス追加
			chainIdx.push_back(pairIdx);

			// もうチェイン出来なくする
			pText->SetEnablePossibleChain(false);
			break;
		}
	}

	// チェインインデックス割り当て
	m_nVecChainIdx = chainIdx;
}

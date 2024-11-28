//==========================================================================
// 
//  モテ文字処理 [charmText_Left.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "charmText_Left.h"
#include "player.h"
#include "object2D.h"
#include "charmManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

namespace Position
{
	const MyLib::Vector3 START(-400.0f, 100.0f, 700.0f);						// 開始位置
	const MyLib::Vector3 FADEIN = START + MyLib::Vector3(-150.0f, 150.0f, 0.0f);	// フェードイン位置
	const MyLib::Vector3 FADEOUT = FADEIN + MyLib::Vector3(-100.0f, 100.0f, 0.0f);						// 終了位置
	const MyLib::Vector3 OFFSET_ICON = MyLib::Vector3(40.0f, 0.0f, 0.0f);	// テキストのオフセット位置
}

//==========================================================================
// 静的メンバ変数定義
//==========================================================================
CListManager<CCharmText_Left> CCharmText_Left::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CCharmText_Left::CCharmText_Left() : CCharmText()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCharmText_Left::~CCharmText_Left()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCharmText_Left::Init()
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
		m_pText->SetPosition(Position::START + MyLib::Vector3(m_pFace->GetSize().x, 0.0f, 0.0f));
		m_pText->SetAnchorType(CObjectBillboard::EAnchorPoint::LEFT);
	}


	// 自分のチェインインデックス
	m_nMyChainIdx = 0;

	// 既に存在しているものを上げる
	CListManager<CCharmText_Left>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ループ
		(*itr)->AddCountUP(1);

		// チェインインデックスも加算
		m_nMyChainIdx++;
	}

	// チェイン確認
	CheckChain();

	// リスト追加
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCharmText_Left::Uninit()
{
	// リストから削除
	m_List.Delete(this);

	// 既に存在しているものを上げる
	CListManager<CCharmText_Left>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ループ

		CCharmText_Left* pText = (*itr);

		std::vector<int> vecIdx = pText->GetChainIdx();		// チェインしているインデックス番号取得
		std::vector<int>::iterator itrIdx = std::find(vecIdx.begin(), vecIdx.end(), m_nMyChainIdx);

		if (itrIdx != vecIdx.end())
		{// 今回分消す
			vecIdx.erase(itrIdx);
		}
		pText->SetChainIdx(vecIdx);

		// 自分のインデックスも減らす
		int myChainIdx = pText->GetMyChainIdx();
		myChainIdx--;
		pText->SetMyChainIdx(myChainIdx);
	}


	itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ループ

		CCharmText_Left* pText = (*itr);

		// 格納している全てを減算
		std::vector<int> vecIdx = pText->GetChainIdx();		// チェインしているインデックス番号取得
		for (auto& idx : vecIdx)
		{
			idx--;
		}
		pText->SetChainIdx(vecIdx);
	}

	// 終了処理
	CCharmText::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCharmText_Left::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 更新処理
	CCharmText::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 終了で抜ける
	if (IsDeath()) return;

	// 位置設定
	MyLib::Vector3 pos = GetPosition();

	// 間隔分ずらす
	pos.x -= DISTANCE_XZ * m_nCntUp;
	pos.z += DISTANCE_XZ * m_nCntUp;
	pos.y += DISTANCE_UP * m_nCntUp;

	MyLib::Vector2 size = m_pText->GetSize() * 0.5f;
	m_pFace->SetPosition(pos + MyLib::Vector3(size.x, -size.y, -50.0f));
	m_pText->SetPosition(pos);
}

//==========================================================================
// フェードイン
//==========================================================================
void CCharmText_Left::StateFadeIn()
{
	// 割合
	float ratio = UtilFunc::Correction::EasingQuintIn(0.0f, 1.0f, 0.0f, STATETIME_FADEIN, m_fStateTime);

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
void CCharmText_Left::StateWait()
{
	// 待機
	CCharmText::StateWait();
}

//==========================================================================
// フェードアウト
//==========================================================================
void CCharmText_Left::StateFadeOut()
{
	// 割合
	float ratio = UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, 0.0f, STATETIME_FADEOUT, m_fStateTime);

	// 位置更新
	MyLib::Vector3 pos = Position::FADEIN + (Position::FADEOUT - Position::FADEIN) * ratio;
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
void CCharmText_Left::CheckChain()
{
	std::vector<int> chainIdx;	// チェインするインデックス

	// 既に存在しているものから確認
	CListManager<CCharmText_Left>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ループ

		// ポインタ変換
		CCharmText_Left* pText = (*itr);

		// チェイン可能フラグ取得
		bool bPossibleChain = pText->IsPossibleChain();

		if (bPossibleChain)
		{// チェイン可能なやつと、そいつがもっている全てとチェインする

			// 既にチェインしているやつとそいつのインデックス取得
			chainIdx = pText->GetChainIdx();
			int pairIdx = pText->GetMyChainIdx();
			
			// インデックス追加
			chainIdx.push_back(pairIdx);

			// もうチェイン出来なくする
			pText->SetEnablePossibleChain(false);

			// TODO : チェイン演出

			break;
		}
	}

	// チェインインデックス割り当て
	m_nVecChainIdx = chainIdx;
}
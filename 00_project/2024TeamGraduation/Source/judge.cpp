//=============================================================================
// 
// 判定 [judge.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "judge.h"
#include "baggage.h"
#include "calculation.h"
#include <algorithm>

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//**************************************************************************
// 判定クラス
//**************************************************************************
//==========================================================================
// コンストラクタ
//==========================================================================
CJudge::CJudge()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudge::~CJudge()
{
	
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudge::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudge::Uninit()
{
	delete m_pJudgeConditional;
	m_pJudgeConditional = nullptr;

	delete this;
}

//==========================================================================
// 生成処理
//==========================================================================
CJudge* CJudge::Create(CJudgeConditional_None* pConditional)
{
	// メモリの確保
	CJudge* pObj = DEBUG_NEW CJudge;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
		pObj->m_pJudgeConditional = pConditional;
	}

	return pObj;
}

//==========================================================================
// 確認処理
//==========================================================================
void CJudge::Check()
{
	if (m_pJudgeConditional != nullptr)
	{
		m_pJudgeConditional->ConditionCheck();
	}
}

//==========================================================================
// 結果処理
//==========================================================================
CJudge::JUDGE CJudge::Judge()
{
	CJudge::JUDGE judge = CJudge::JUDGE::JUDGE_AAA;
	if (m_pJudgeConditional != nullptr)
	{
		judge = m_pJudgeConditional->Result();
	}
	return judge;
}


//**************************************************************************
// 条件（無条件）クラス
//**************************************************************************
//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeConditional_None::CJudgeConditional_None(std::map<CJudge::JUDGE, int> resultborder)
{
	int maxNum = -1;
	for (auto itr = resultborder.begin(); itr != resultborder.end(); itr++)
	{
		if ((*itr).second > maxNum)
		{
			m_judge = (*itr).first;
			maxNum = (*itr).second;
		}
	}
}


//**************************************************************************
// 条件（衝突回数あり）クラス
//**************************************************************************
//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeConditional_HitNum::CJudgeConditional_HitNum(std::map<CJudge::JUDGE, int> resultborder) : CJudgeConditional_None(resultborder)
{
	m_resultBorder = resultborder;
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeConditional_HitNum::~CJudgeConditional_HitNum()
{
}

//==========================================================================
// 条件確認
//==========================================================================
void CJudgeConditional_HitNum::ConditionCheck()
{
	// 荷物の当たり判定取得
	CListManager<CBaggage> baggageList = CBaggage::GetListObj();
	CListManager<CBaggage>::Iterator itr = baggageList.GetEnd();
	while (baggageList.ListLoop(itr))
	{
		if ((*itr)->IsHit())
		{//当たった
			m_nHitNum = UtilFunc::Transformation::Clamp(m_nHitNum + 1, 0, INT_MAX);
			break;
		}
	}
}

//==========================================================================
// 結果確認
//==========================================================================
CJudge::JUDGE CJudgeConditional_HitNum::Result()
{
	CJudge::JUDGE judge = CJudge::JUDGE::JUDGE_DDD;
	for (auto itr = m_resultBorder.begin(); itr != m_resultBorder.end(); itr++)
	{
		if (m_nHitNum <= itr->second && itr->second < m_resultBorder[judge])
		{
			judge = itr->first;
		}
	}

	return judge;
}

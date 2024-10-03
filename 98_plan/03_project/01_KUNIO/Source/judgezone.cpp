//=============================================================================
// 
// 判定ゾーン [judgezone.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "judgezone.h"
#include "judge.h"
#include "baggage.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}
CListManager<CJudgeZone> CJudgeZone::m_List = {};

//**************************************************************************
// 
// 判定ゾーンクラス
// 
//**************************************************************************
//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeZone::CJudgeZone()
{
	m_isEnable = true;
	m_pJudge = nullptr;
	m_zone = CJudgeZone::SJudgeZone(0.0f, 0.0f, 0.0f);
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeZone::~CJudgeZone()
{
	
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudgeZone::Init()
{
	m_List.Regist(this);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeZone::Uninit()
{

	if (m_pJudge != nullptr)
	{
		m_pJudge->Uninit();
		m_pJudge = nullptr;
	}

	m_isEnable = false;

	m_List.Delete(this);

	delete this;
}

//==========================================================================
// 生成処理
//==========================================================================
CJudgeZone* CJudgeZone::Create(const float start, const float end, const float borderHeight)
{
	// メモリの確保
	CJudgeZone* pObj = DEBUG_NEW CJudgeZone;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
		pObj->SetZone(CJudgeZone::SJudgeZone(start, end, borderHeight));
	}

	return pObj;
}

//==========================================================================
// 確認処理
//==========================================================================
void CJudgeZone::Check()
{
	if (m_pJudge != nullptr)
	{// どっち行くか決まってる
		m_pJudge->Check();
	}
	else
	{// 決まってない
		// 荷物取得
		CListManager<CBaggage> baggageList = CBaggage::GetListObj();
		CBaggage* pBaggage = nullptr;
		baggageList.ListLoop(&pBaggage);

		// 上か下か決める
		CJudge::BORDER border = (pBaggage->GetPosition().y > m_zone.borderHeight) ? CJudge::BORDER::TOP : CJudge::BORDER::UNDER;

		// 生成
		if (m_aJudgeInfo[border].type == CJudge::CONDITIONTYPE::TYPE_NONE)
		{
			m_pJudge = CJudge::Create(DEBUG_NEW CJudgeConditional_None(m_aJudgeInfo[border].judgeParam));
		}
		else if (m_aJudgeInfo[border].type == CJudge::CONDITIONTYPE::TYPE_HITNUM)
		{
			m_pJudge = CJudge::Create(DEBUG_NEW CJudgeConditional_HitNum(m_aJudgeInfo[border].judgeParam));
		}
		
		m_pJudge->Check();
	}
}

//==========================================================================
// 結果処理
//==========================================================================
CJudge::JUDGE CJudgeZone::Judge()
{
	CJudge::JUDGE judge = CJudge::JUDGE::JUDGE_AAA;
	if (m_pJudge != nullptr)
	{
		judge = m_pJudge->Judge();
	}

	return judge;
}


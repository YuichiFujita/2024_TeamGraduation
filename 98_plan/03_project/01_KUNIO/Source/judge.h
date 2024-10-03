//=============================================================================
// 
//  判定ヘッダー [judge.h]
//  Author : 石原颯馬
// 
//=============================================================================
#ifndef _JUDGE_H_
#define _JUDGE_H_		// 二重インクルード防止
#include "listmanager.h"
#include <map>

//前方宣言
class CJudgeConditional_None;

//==========================================================================
// クラス定義
//==========================================================================
//**************************************************************************
// 判定
//**************************************************************************
class CJudge
{
public:
	// 判定種類列挙
	enum CONDITIONTYPE
	{
		TYPE_NONE = 0,
		TYPE_HITNUM,
		TYPE_MAX,
	};

	// 判定列挙
	enum JUDGE
	{
		JUDGE_AAA = 0,
		JUDGE_BBB,
		JUDGE_CCC,
		JUDGE_DDD,
		JUDGE_MAX
	};

	// 上下
	enum BORDER
	{
		TOP = 0,
		UNDER,
		MAX
	};

	// 判定情報構造体
	struct SJudgeCondition
	{
		CONDITIONTYPE type;
		std::map<CJudge::JUDGE, int> judgeParam;

		SJudgeCondition() : type(CONDITIONTYPE::TYPE_NONE) {}
		SJudgeCondition(CONDITIONTYPE _type) : type(_type){}
	};

	CJudge();
	virtual ~CJudge();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// メンバ関数
	//=============================
	virtual void Check();
	virtual JUDGE Judge();

	//=============================
	// 静的関数
	//=============================
	static CJudge* Create(CJudgeConditional_None* pConditional = nullptr);	// 生成
private:
	JUDGE m_judge;
	CJudgeConditional_None* m_pJudgeConditional;
};

//**************************************************************************
// 条件（無条件）
//**************************************************************************
//**************************************************************************
class CJudgeConditional_None
{
public:
	CJudgeConditional_None(std::map<CJudge::JUDGE, int> resultborder);
	virtual ~CJudgeConditional_None() {}

	virtual void ConditionCheck() {}
	virtual CJudge::JUDGE Result() { return m_judge; }
private:
	CJudge::JUDGE m_judge;
};

//**************************************************************************
// 条件（衝突回数）
//**************************************************************************
class CJudgeConditional_HitNum : public CJudgeConditional_None
{
public:
	CJudgeConditional_HitNum(std::map<CJudge::JUDGE, int> resultborder);
	virtual ~CJudgeConditional_HitNum();

	virtual void ConditionCheck();
	virtual CJudge::JUDGE Result();
private:
	int m_nHitNum;			//何回当たったか
	std::map<CJudge::JUDGE, int> m_resultBorder;
};

#endif
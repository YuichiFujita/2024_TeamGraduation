//=============================================================================
// 
//  ����w�b�_�[ [judge.h]
//  Author : �Ό��D�n
// 
//=============================================================================
#ifndef _JUDGE_H_
#define _JUDGE_H_		// ��d�C���N���[�h�h�~
#include "listmanager.h"
#include <map>

//�O���錾
class CJudgeConditional_None;

//==========================================================================
// �N���X��`
//==========================================================================
//**************************************************************************
// ����
//**************************************************************************
class CJudge
{
public:
	// �����ޗ�
	enum CONDITIONTYPE
	{
		TYPE_NONE = 0,
		TYPE_HITNUM,
		TYPE_MAX,
	};

	// �����
	enum JUDGE
	{
		JUDGE_AAA = 0,
		JUDGE_BBB,
		JUDGE_CCC,
		JUDGE_DDD,
		JUDGE_MAX
	};

	// �㉺
	enum BORDER
	{
		TOP = 0,
		UNDER,
		MAX
	};

	// ������\����
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
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// �����o�֐�
	//=============================
	virtual void Check();
	virtual JUDGE Judge();

	//=============================
	// �ÓI�֐�
	//=============================
	static CJudge* Create(CJudgeConditional_None* pConditional = nullptr);	// ����
private:
	JUDGE m_judge;
	CJudgeConditional_None* m_pJudgeConditional;
};

//**************************************************************************
// �����i�������j
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
// �����i�Փˉ񐔁j
//**************************************************************************
class CJudgeConditional_HitNum : public CJudgeConditional_None
{
public:
	CJudgeConditional_HitNum(std::map<CJudge::JUDGE, int> resultborder);
	virtual ~CJudgeConditional_HitNum();

	virtual void ConditionCheck();
	virtual CJudge::JUDGE Result();
private:
	int m_nHitNum;			//���񓖂�������
	std::map<CJudge::JUDGE, int> m_resultBorder;
};

#endif
//=============================================================================
// 
// ���� [judge.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "judge.h"
#include "baggage.h"
#include "calculation.h"
#include <algorithm>

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//**************************************************************************
// ����N���X
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudge::CJudge()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudge::~CJudge()
{
	
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CJudge::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CJudge::Uninit()
{
	delete m_pJudgeConditional;
	m_pJudgeConditional = nullptr;

	delete this;
}

//==========================================================================
// ��������
//==========================================================================
CJudge* CJudge::Create(CJudgeConditional_None* pConditional)
{
	// �������̊m��
	CJudge* pObj = DEBUG_NEW CJudge;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
		pObj->m_pJudgeConditional = pConditional;
	}

	return pObj;
}

//==========================================================================
// �m�F����
//==========================================================================
void CJudge::Check()
{
	if (m_pJudgeConditional != nullptr)
	{
		m_pJudgeConditional->ConditionCheck();
	}
}

//==========================================================================
// ���ʏ���
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
// �����i�������j�N���X
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
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
// �����i�Փˉ񐔂���j�N���X
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeConditional_HitNum::CJudgeConditional_HitNum(std::map<CJudge::JUDGE, int> resultborder) : CJudgeConditional_None(resultborder)
{
	m_resultBorder = resultborder;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudgeConditional_HitNum::~CJudgeConditional_HitNum()
{
}

//==========================================================================
// �����m�F
//==========================================================================
void CJudgeConditional_HitNum::ConditionCheck()
{
	// �ו��̓����蔻��擾
	CListManager<CBaggage> baggageList = CBaggage::GetListObj();
	CListManager<CBaggage>::Iterator itr = baggageList.GetEnd();
	while (baggageList.ListLoop(itr))
	{
		if ((*itr)->IsHit())
		{//��������
			m_nHitNum = UtilFunc::Transformation::Clamp(m_nHitNum + 1, 0, INT_MAX);
			break;
		}
	}
}

//==========================================================================
// ���ʊm�F
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

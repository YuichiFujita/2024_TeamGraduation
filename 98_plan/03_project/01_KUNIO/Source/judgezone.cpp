//=============================================================================
// 
// ����]�[�� [judgezone.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "judgezone.h"
#include "judge.h"
#include "baggage.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}
CListManager<CJudgeZone> CJudgeZone::m_List = {};

//**************************************************************************
// 
// ����]�[���N���X
// 
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeZone::CJudgeZone()
{
	m_isEnable = true;
	m_pJudge = nullptr;
	m_zone = CJudgeZone::SJudgeZone(0.0f, 0.0f, 0.0f);
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudgeZone::~CJudgeZone()
{
	
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CJudgeZone::Init()
{
	m_List.Regist(this);
	return S_OK;
}

//==========================================================================
// �I������
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
// ��������
//==========================================================================
CJudgeZone* CJudgeZone::Create(const float start, const float end, const float borderHeight)
{
	// �������̊m��
	CJudgeZone* pObj = DEBUG_NEW CJudgeZone;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
		pObj->SetZone(CJudgeZone::SJudgeZone(start, end, borderHeight));
	}

	return pObj;
}

//==========================================================================
// �m�F����
//==========================================================================
void CJudgeZone::Check()
{
	if (m_pJudge != nullptr)
	{// �ǂ����s�������܂��Ă�
		m_pJudge->Check();
	}
	else
	{// ���܂��ĂȂ�
		// �ו��擾
		CListManager<CBaggage> baggageList = CBaggage::GetListObj();
		CBaggage* pBaggage = nullptr;
		baggageList.ListLoop(&pBaggage);

		// �ォ�������߂�
		CJudge::BORDER border = (pBaggage->GetPosition().y > m_zone.borderHeight) ? CJudge::BORDER::TOP : CJudge::BORDER::UNDER;

		// ����
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
// ���ʏ���
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


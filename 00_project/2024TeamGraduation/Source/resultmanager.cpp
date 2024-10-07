//=============================================================================
// 
//  ���U���g�}�l�[�W������ [resultmanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "debugproc.h"
#include "resultmanager.h"
#include "calculation.h"

#include "timer_result.h"
#include "clearrank.h"
#include "toatalrank.h"
#include "scroll.h"
#include "sound.h"
#include "ranking.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CResultManager* CResultManager::m_pThisPtr = nullptr;	// ���g�̃|�C���^
float CResultManager::m_fClearTime = 0.0f;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResultManager::CResultManager()
{
	// �l�̃N���A
	m_state = State::STATE_SCORE;				// ���
	m_JudgeRank = CJudge::JUDGE::JUDGE_DDD;	// �ŏI�]��
	m_pScroll = nullptr;		// �������̃I�u�W�F�N�g
	m_pTimer = nullptr;			// �^�C�}�[�̃I�u�W�F�N�g
	m_pClearRank = nullptr;		// �N���A���̃����N
	m_pToatalRank = nullptr;	// �����]��

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResultManager::~CResultManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CResultManager *CResultManager::Create()
{
	if (m_pThisPtr != nullptr)
	{
		return m_pThisPtr;
	}

	// �������̊m��
	m_pThisPtr = DEBUG_NEW CResultManager;
	if (m_pThisPtr == nullptr)
	{
		return nullptr;
	}

	// ����������
	HRESULT hr = m_pThisPtr->Init();
	if (FAILED(hr))
	{// ���s���Ă�����
		return nullptr;
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResultManager::Init()
{
	return S_OK;
}

//==========================================================================
// ���U���g��ʐ���
//==========================================================================
void CResultManager::CreateResultScreen()
{
	// ������
	m_pScroll = CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 1.5f, 350.0f, 1000.0f, false, true, 1);

	// �^�C�}�[
	m_pTimer = static_cast<CTimer_Result*>(CTimer::Create(CTimer::Type::TYPE_RESULT, 4));
	m_pTimer->SetTime(m_fClearTime);

	// �N���A�����N
	m_pClearRank = CClearRank::Create(m_JudgeRank);

	// �g�[�^�������N
	m_pToatalRank = CToatalRank::Create(m_JudgeRank, m_fClearTime);

	// �����L���O�f�[�^�ɕۑ�
	CRanking::SRankdata nowdata = CRanking::SRankdata();
	nowdata.allrank = m_pToatalRank->GetRank();
	nowdata.minutes = static_cast<int>(m_fClearTime / 60);
	nowdata.seconds = static_cast<int>(m_fClearTime) % 60;
	nowdata.milliSeconds = static_cast<int>((m_fClearTime - static_cast<int>(m_fClearTime)) * 1000);
	nowdata.milliSeconds /= 10;
	CRanking::SetNowData(nowdata);
}

//==========================================================================
// �I������
//==========================================================================
void CResultManager::Uninit()
{
	// �^�C�}�[�̔j��
	if (m_pTimer != nullptr)
	{
		// �I������
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	delete m_pThisPtr;
	m_pThisPtr = nullptr;

	m_fClearTime = 0.0f;
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CResultManager::Reset()
{
	// �^�C�}�[�̔j��
	if (m_pTimer != nullptr)
	{
		// �I������
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	m_state = State::STATE_SCORE;			// ���
	m_JudgeRank = CJudge::JUDGE::JUDGE_DDD;	// �ŏI�]��
	m_fClearTime = 0.0f;					// �N���A����

	m_pScroll = nullptr;		// �������̃I�u�W�F�N�g
	m_pTimer = nullptr;			// �^�C�}�[�̃I�u�W�F�N�g
	m_pClearRank = nullptr;		// �N���A���̃����N
	m_pToatalRank = nullptr;	// �����]��
}

//==========================================================================
// �X�V����
//==========================================================================
void CResultManager::Update()
{

	// �N���A�̃����N
	if (m_pClearRank == nullptr) return;

	if (m_pScroll->GetState() == CScroll::STATE::STATE_WAIT &&
		m_pClearRank->GetState() == CClearRank::State::STATE_NONE &&
		!m_pClearRank->IsFinish())
	{// �������J����������
		m_pClearRank->SetState(CClearRank::State::STATE_SCROLL_TEXT);

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING);
	}

	if (m_pClearRank->GetState() == CClearRank::State::STATE_FINISH &&
		m_pTimer->GetState() == CTimer_Result::State::STATE_NONE &&
		!m_pTimer->IsFinish())
	{// �N���A�����N�I����

		m_pTimer->CTimer_Result::SetState(CTimer_Result::State::STATE_SCROLL_TEXT);

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING);
	}

	if (m_pTimer->GetState() == CTimer_Result::State::STATE_FINISH &&
		m_pToatalRank->GetState() == CToatalRank::State::STATE_NONE &&
		!m_pToatalRank->IsFinish())
	{// �N���A�^�C���I����

		m_pToatalRank->SetState(CToatalRank::State::STATE_SCROLL_TEXT);

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING);
	}


	if (m_pToatalRank->IsFinish())
	{// �����]�����o��

		// �����҂�
		m_state = State::STATE_PRESSENTER;
	}

	// �^�C�}�[�X�V
	if (m_pTimer != nullptr)
	{
		m_pTimer->Update();
		m_pTimer->SetTime(m_fClearTime);
	}
}

//=============================================================================
// 
//  �L���b�`�X�y�V�������� [catchSpecial.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "catchSpecial.h"
#include "player.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace StateTime
{
	const float KAMEHAME_SUCC(5.0f);	// ���߂͂ߔg�H�L���b�`����
	const float KAMEHAME_FAIL(5.0f);	// ���߂͂ߔg�H�L���b�`����
}

// ���߂͂ߔg
namespace Kamehameha
{
	const float FAN_ROT(5.0f);		// ��������̐�̊p�x(�x���@)
	const float RAD_JUST(0.5f);		// ��������̂̋��e����(�W���X�g��)
	const float RAD_NORMAL(0.2f);	// ��������̂̋��e����(���펞)
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCatchSpecial::STATE_FUNC CCatchSpecial::m_StateFunc[] =	// �X�y�V�����L���b�`��Ԋ֐�
{
	&CCatchSpecial::StateNone,				// �Ȃ�
	&CCatchSpecial::State_Kamehame_Succ,	// ���߂͂ߔg�H�L���b�`����
	&CCatchSpecial::State_Kamehame_Fail,	// ���߂͂ߔg�H�L���b�`���s
};

// �֐����X�g
std::vector<std::function<CCatchSpecial::EState(const CPlayer*, const bool)>> CCatchSpecial::s_CheckFunc =
{
	nullptr,								// NONE
	CCatchSpecial::Check_Kamehameha,						// ���߂͂ߔg
	nullptr,								// MAX
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCatchSpecial::CCatchSpecial(CPlayer* pPlayer, EState state) :
	m_pPlayer(pPlayer), m_state(state)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;	// ��Ԏ���
	m_bSuccess = false;		// �����t���O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCatchSpecial::~CCatchSpecial()
{

}

//==========================================================================
// ��������
//==========================================================================
CCatchSpecial *CCatchSpecial::Create(CPlayer* pPlayer, EState state)
{
	// �������̊m��
	CCatchSpecial* pCatchSP = DEBUG_NEW CCatchSpecial(pPlayer, state);
	if (pCatchSP != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pCatchSP->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pCatchSP);
			return nullptr;
		}
	}

	return pCatchSP;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCatchSpecial::Init()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCatchSpecial::Uninit()
{
	m_pPlayer = nullptr;			// �v���C���[
}

//==========================================================================
// �X�V����
//==========================================================================
void CCatchSpecial::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime += fDeltaTime * fSlowRate;

	// ��ԍX�V
	if (m_StateFunc[m_state] != nullptr)
	{
		(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �Ȃ����
//==========================================================================
void CCatchSpecial::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I��
	m_pPlayer->SetCatchSpecial(nullptr);
	Uninit();
}

//==========================================================================
// ���߂͂ߔg�H�L���b�`����
//==========================================================================
void CCatchSpecial::State_Kamehame_Succ(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime > StateTime::KAMEHAME_SUCC)
	{// �I��������
		
		// ��������
		Success();

		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ���߂͂ߔg�H�L���b�`���s
//==========================================================================
void CCatchSpecial::State_Kamehame_Fail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime > StateTime::KAMEHAME_SUCC)
	{// �I��������

		// ���s����
		Failure();

		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CCatchSpecial::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// ����������
//==========================================================================
void CCatchSpecial::Success()
{
	CGameManager* pGameMgr = CGameManager::GetInstance();
	CGameManager::ETeamSide team = m_pPlayer->GetTeam();
	CGameManager::ETeamSide rivalTeam = pGameMgr->RivalTeam(team);
	
	// �����t���O
	m_bSuccess = true;

	// ���w
	// ���e���Z
	pGameMgr->AddCharmValue(team, CCharmValueManager::ETypeAdd::ADD_CATCH_SPECIAL);
	// �X�y�V�������Z
	pGameMgr->AddSpecialValue(team, CSpecialValueManager::ETypeAdd::ADD_CATCH_SPECIAL);

	// �G�w
	pGameMgr->SubCharmValue(rivalTeam, CCharmValueManager::ETypeSub::SUB_SPECIAL_CATCH);
}

//==========================================================================
// ���s������
//==========================================================================
void CCatchSpecial::Failure()
{
	// �����t���O
	m_bSuccess = false;
}

//==========================================================================
// �`�F�b�N
//==========================================================================
CCatchSpecial::EState CCatchSpecial::Check_Kamehameha(const CPlayer* pPlayer, const bool bJust)
{
	// �E�W���X�g�Ȃ�R�[�g�̑O50��
	// �E�m�[�}���Ȃ�R�[�g�̑O20��
	// �E�X�y�V���������N�̊p�x��(��)
	CGameManager* pGameMgr = CGameManager::GetInstance();
	CGameManager::ETeamSide team = pPlayer->GetTeam();
	MyLib::Vector3 pos = pPlayer->GetPosition();

	// �K�芄��
	float fRad = 0.0f;
	bJust ? fRad = Kamehameha::RAD_JUST : fRad = Kamehameha::RAD_NORMAL;

	// �R�[�g�������擾
	float fRate = pGameMgr->GetCourtPosPercentage(team, pos);

	if (fRate <= fRad)
	{// (�K���)�Ȃ琬��

		return EState::STATE_BEAM_SUCC;
	}

	return EState::STATE_BEAM_FAIL;
}

//==========================================================================
// �`�F�b�N
//==========================================================================
CCatchSpecial::EState CCatchSpecial::Check(CPlayer* pPlayer, bool bJust, CBall::ESpecial state)
{
	CCatchSpecial::EState catchState = EState::STATE_NONE;

	// �ÓI�֐��|�C���^
	if (s_CheckFunc[state] != nullptr)
	{
		catchState = CCatchSpecial::s_CheckFunc[state](pPlayer, bJust);
	}

	return catchState;
}

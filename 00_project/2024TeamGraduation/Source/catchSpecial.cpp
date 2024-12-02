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
// ���߂͂ߔg
namespace Kamehameha
{
	const float FAN_ROT(5.0f);		// ��������̐�̊p�x(�x���@)
	const float RAD_JUST(0.5f);		// ��������̂̋��e����(�W���X�g��)
	const float RAD_NORMAL(0.2f);	// ��������̂̋��e����(���펞)
	
	const float MOMENTUM_TIME[] =
	{
		0.5f,		// �Ȃ�
		1.0f,		// �Y�U�U�[
		2.0f,		// �ς���
		1.0f,		// ����(����)(���s)
		0.0f,
	};

	// ��ԓ���Ԓ�`(����)
	enum EMomentumState
	{
		STATE_NONE = 0,			// �Ȃ�
		STATE_SLIDE,			// �Y�U�U�[
		STATE_BRAKE,			// �ς���
		STATE_RESULT,			// ����(����)(���s)
		STATE_MAX
	};
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCatchSpecial::STATE_FUNC CCatchSpecial::m_StateFunc[] =	// �X�y�V�����L���b�`��Ԋ֐�
{
	&CCatchSpecial::StateNone,				// �Ȃ�
	&CCatchSpecial::StateKamehameSucc,		// ���߂͂ߔg�H�L���b�`����
	&CCatchSpecial::StateKamehameFail,		// ���߂͂ߔg�H�L���b�`���s
};

CCatchSpecial::MOMENTUM_FUNC CCatchSpecial::m_MomentumFunc[] =	// ������Ԋ֐�
{
	&CCatchSpecial::MomentumStateNone,		// �Ȃ�
	&CCatchSpecial::MomentumStateSlide,		// �Y�U�U
	&CCatchSpecial::MomentumStateBrake,		// �ς���
	&CCatchSpecial::MomentumStateResult,	// ����
	&CCatchSpecial::MomentumStateEnd,		// �I��
};

CCatchSpecial::START_FUNC CCatchSpecial::m_StartFunc[] =	// ������ԊJ�n�֐�
{
	nullptr,								// �Ȃ�
	&CCatchSpecial::MomentumStartSlide,		// �Y�U�U
	nullptr,								// �ς���
	nullptr,								// ����
	nullptr,								// �I��
};

// �֐����X�g
std::vector<std::function<CCatchSpecial::EState(const CPlayer*, const bool)>> CCatchSpecial::s_CheckFunc =
{
	nullptr,								// NONE
	CCatchSpecial::Check_Kamehameha,		// ���߂͂ߔg
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
	m_fStateTime = 0.0f;							// ��Ԏ���
	m_momentumState = EMomentumState::MOMENTUM_NONE;	// ��ԓ����
	m_fMomentumStateTime = 0.0f;					// ��ԓ���Ԏ���
	m_bSuccess = false;								// �����t���O
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
	MyAssert::CustomAssert(m_pPlayer != nullptr, "catchSpecial: �v���C���[�ǂ��s������H");
	m_pPlayer->SetEnableMove(false);
	m_pPlayer->SetEnableAction(false);

	m_fStateTime += fDeltaTime * fSlowRate;
	m_fMomentumStateTime += fDeltaTime * fSlowRate;

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

	// �s���\��
	m_pPlayer->SetEnableMove(true);
	m_pPlayer->SetEnableAction(true);
	m_pPlayer->SetCatchSpecial(nullptr);
	Uninit();
}

//==========================================================================
// ���߂͂ߔg�H�L���b�`����
//==========================================================================
void CCatchSpecial::StateKamehameSucc(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����t���O
	m_bSuccess = true;

	// �����X�V
	if (m_MomentumFunc[m_momentumState] != nullptr)
	{
		(this->*(m_MomentumFunc[m_momentumState]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// ���[�V�����ݒ�
	m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_CATCHSPECIAL_SUCC);

	if (m_momentumState == EMomentumState::MOMENTUM_END)
	{// �I��������(���ʎ��Ԃ��I�����)
		
		// ��������
		Success();

		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ���߂͂ߔg�H�L���b�`���s
//==========================================================================
void CCatchSpecial::StateKamehameFail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����t���O
	m_bSuccess = false;

	// �����X�V
	if (m_MomentumFunc[m_momentumState] != nullptr)
	{
		(this->*(m_MomentumFunc[m_momentumState]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// ���[�V�����ݒ�
	m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_CATCHSPECIAL_FAIL);

	if (m_momentumState == EMomentumState::MOMENTUM_END)
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
// [����] �Ȃ�
//==========================================================================
void CCatchSpecial::MomentumStateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �X���[������

	if (m_fMomentumStateTime > Kamehameha::MOMENTUM_TIME[m_momentumState])
	{// �I��

		// �X���[�߂�


		SetMomentumState(EMomentumState::MOMENTUM_SLIDE);
	}
}

//==========================================================================
// [����] �Y�U�U�[
//==========================================================================
void CCatchSpecial::MomentumStateSlide(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �Y�U�U�ŃR�[�g���܂ōs��
	MyLib::Vector3 pos = MyLib::Vector3();
	float fBaseTime = Kamehameha::MOMENTUM_TIME[m_momentumState];
	CPlayer::SKnockbackInfo knockback = m_pPlayer->GetKnockBackInfo();

	// �ړ��ʐ���
	MyLib::Vector3 move = knockback.posStart - knockback.posEnd;
	float fAngle = atan2f(move.x, move.z) + D3DX_PI;		//�ڕW�̌���
	UtilFunc::Transformation::RotNormalize(fAngle);

	move = m_pPlayer->GetMove();
	move.x = sinf(fAngle) * 1.0f;
	move.z = cosf(fAngle) * 1.0f;

	pos += move;

	m_pPlayer->SetPosition(pos);

	if (m_fMomentumStateTime > fBaseTime)
	{// �I��

		SetMomentumState(EMomentumState::MOMENTUM_BRAKE);
	}
}

//==========================================================================
// [����] �ς���
//==========================================================================
void CCatchSpecial::MomentumStateBrake(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fMomentumStateTime > Kamehameha::MOMENTUM_TIME[m_momentumState])
	{// �I��

		SetMomentumState(EMomentumState::MOMENTUM_RESULT);
	}
}

//==========================================================================
// [����] ����(����)(���s)
//==========================================================================
void CCatchSpecial::MomentumStateResult(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fMomentumStateTime > Kamehameha::MOMENTUM_TIME[m_momentumState])
	{// �I��

		SetMomentumState(EMomentumState::MOMENTUM_END);
	}
}

//==========================================================================
// [����] �I��
//==========================================================================
void CCatchSpecial::MomentumStateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fMomentumStateTime = 0.0f;
}

//==========================================================================
// ������Ԑݒ�
//==========================================================================
void CCatchSpecial::SetMomentumState(EMomentumState state)
{
	m_momentumState = state;
	m_fMomentumStateTime = 0.0f;

	// �����X�V
	if (m_StartFunc[m_momentumState] != nullptr)
	{
		(this->*(m_StartFunc[m_momentumState]))();
	}
}

//==========================================================================
// �Y�U�U�J�n
//==========================================================================
void CCatchSpecial::MomentumStartSlide()
{
	CPlayer::SKnockbackInfo knockback = m_pPlayer->GetKnockBackInfo();

	// �J�n�ʒu
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	MyLib::Vector3 rot = m_pPlayer->GetRotation();
	knockback.posStart = pos;

	// �R�[�g�[�Ŕ������Ă��̍ۂ�ݒ�
	CGameManager* pGmMgr = CGameManager::GetInstance();
	
	while(!pGmMgr->SetPosLimit(pos, m_pPlayer->GetRadius()))
	{// �[�ɓ������Ă��Ȃ�������
	
		pos.x += sinf(rot.y) * 10.0f;
		pos.z += cosf(rot.y) * 10.0f;
	}
	
	// �I���ʒu
	knockback.posEnd = pos;

	// �ݒ�
	m_pPlayer->SetKnockBackInfo(knockback);
}

//==========================================================================
// ����������
//==========================================================================
void CCatchSpecial::Success()
{
	CGameManager* pGameMgr = CGameManager::GetInstance();
	CGameManager::ETeamSide team = m_pPlayer->GetTeam();
	CGameManager::ETeamSide rivalTeam = pGameMgr->RivalTeam(team);

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
	//�X�y�V���������C���z������
	MyLib::Vector3 pos = m_pPlayer->GetPosition();

	if (m_state != CPlayer::EState::STATE_OUTCOURT &&
		m_state != CPlayer::EState::STATE_OUTCOURT_RETURN)
	{
		m_pPlayer->OutCourtSetting();
	}
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

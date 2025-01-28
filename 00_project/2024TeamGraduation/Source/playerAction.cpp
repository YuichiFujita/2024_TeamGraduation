//=============================================================================
// 
//  �v���C���[�R���g���[������ [playerAction.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "playerAction.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "ball.h"
#include "playerStatus.h"

namespace
{
	const float JUMPTHROW_DOWN_COR = 0.5f;	//���~���W�����v�����ړ��ʕ␳�l
}

namespace DodgeSlow
{
	const float END = 0.1f;			// �ő�l
	const float START = 1.0f;		// �ŏ��l
}

namespace ActionTime
{
	const float BLINK = 0.2f;		// �u�����N����
	const float DODGE = 0.5f;		// �������
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAction::ACTION_FUNC CPlayerAction::m_ActionFunc[] =	// �s���֐�
{
	&CPlayerAction::ActionNone,			// �Ȃ�
	&CPlayerAction::ActionUnstable,		// �����Ƃ���
	&CPlayerAction::ActionBlink,		// �u�����N
	&CPlayerAction::ActionDodge,		// ���
	&CPlayerAction::ActionRun,			// ����
	&CPlayerAction::ActionJump,			// �W�����v
	&CPlayerAction::ActionCatch,		// �L���b�`
	&CPlayerAction::ActionCatchJump,	// �L���b�`(�W�����v)
	&CPlayerAction::ActionThrow,		// ����
	&CPlayerAction::ActionThrowJump,	// ����(�W�����v)
	&CPlayerAction::ActionSpecial,		// �X�y�V����
};

CPlayerAction::START_FUNC CPlayerAction::m_StartFunc[] =	// �s���֐�
{
	nullptr,								// �Ȃ�
	nullptr,								// �����Ƃ���
	nullptr,								// �u�����N
	&CPlayerAction::StartDodge,				// ���
	nullptr,								// ����
	nullptr,								// �W�����v
	nullptr,								// �L���b�`
	&CPlayerAction::StartCatchJump,			// �L���b�`(�W�����v)
	nullptr,								// ����
	&CPlayerAction::StartThrowJump,			// ����(�W�����v)
	nullptr,								// �X�y�V����
};

CPlayerAction::END_FUNC CPlayerAction::m_EndFunc[] =	// �s���֐�
{
	nullptr,								// �Ȃ�
	&CPlayerAction::EndUnstable,			// �����Ƃ���
	nullptr,								// �u�����N
	&CPlayerAction::EndDodge,				// ���
	nullptr,								// ����
	nullptr,								// �W�����v
	nullptr,								// �L���b�`
	nullptr,								// �L���b�`(�W�����v)
	nullptr,								// ����
	nullptr,								// ����(�W�����v)
	nullptr,								// �X�y�V����
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAction::CPlayerAction(CPlayer* player)
{
	m_bCharm = false;
	m_Action = CPlayer::EAction::ACTION_NONE;	// �A�N�V����
	m_fActionTime = 0.0f;						// �A�N�V��������
	m_pPlayer = player;							// �v���C���[�̃|�C���^

	m_fSlowStart = DodgeSlow::START;		// �X���[�{��(�ŏ�)
	m_fSlowEnd = DodgeSlow::END;			// �X���[�{��(�ő�)

	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_ActionFunc) == CPlayer::EAction::ACTION_MAX, "ERROR : Action Count Mismatch");
	static_assert(NUM_ARRAY(m_StartFunc) == CPlayer::EAction::ACTION_MAX, "ERROR : Action Count Mismatch");
	static_assert(NUM_ARRAY(m_EndFunc)  == CPlayer::EAction::ACTION_MAX, "ERROR : Action Count Mismatch");
}

//==========================================================================
// �X�V
//==========================================================================
void CPlayerAction::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �A�N�V��������
	m_fActionTime += fDeltaTime * fDeltaRate * fSlowRate;

	// �s���X�V
	(this->*(m_ActionFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG //�f�o�b�O

	Debug();

#endif //�f�o�b�O
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CPlayerAction::ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �����Ƃ���
//==========================================================================
void CPlayerAction::ActionUnstable(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �u�����N
//==========================================================================
void CPlayerAction::ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fActionTime >= ActionTime::BLINK)
	{// �u�����N�o��
		SetAction(CPlayer::EAction::ACTION_NONE);
	}

	CMotion* pMotion = m_pPlayer->GetMotion();

	// �����{�[�������Ă�
	if (m_pPlayer->GetBall() != nullptr) return;

	//��𔻒�
	CBall* pObj = CGameManager::GetInstance()->GetBall();
	CGameManager::ETeamSide teamPlayer = m_pPlayer->GetTeam();

	// �U����ԂłȂ�or���w
	if (!pObj->IsAttack() || pObj->GetTypeTeam() == teamPlayer) return;

	// �U���t���[��(���P�\)����Ȃ��Ȃ�͂���
	if (!pMotion->IsAttacking()) return;

	// ����
	if (UtilFunc::Collision::CollisionCircleCylinder(
		pObj->GetPosition(), m_pPlayer->GetPosition(),		// �ʒu
		pObj->GetRadius(), m_pPlayer->GetRadius() * 2.0f,	// ���a
		m_pPlayer->GetParameter().fHeight * 1.3f))			// ����
	{
		// �A�N�V�����ݒ�
		SetAction(CPlayer::EAction::ACTION_DODGE);

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_DODGE);
	}
}

//==========================================================================
// ���
//==========================================================================
void CPlayerAction::ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���G���ԕt�^
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.1f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	// �X���[������
	float fRate = 1.0f;	// ����
	float fFrame = m_pPlayer->GetMotion()->GetAllCount();
	float fFrameMax = m_pPlayer->GetMotion()->GetMaxAllCount();

	if (fFrame <= fFrameMax * 0.5f)
	{
		fRate = UtilFunc::Correction::EasingEaseIn(m_fSlowStart, m_fSlowEnd, 0.0f, fFrameMax * 0.5f, fFrame);
	}
	else
	{// �����𒴂�����
		fRate = UtilFunc::Correction::EasingEaseOut(m_fSlowEnd, m_fSlowStart, fFrameMax * 0.5f, fFrameMax, fFrame);
	}

	UtilFunc::Transformation::ValueNormalize(fRate, 1.0f, 0.0f);
	GET_MANAGER->SetSlowRate(fRate);

	if (m_pPlayer->GetMotion()->IsFinish())
	{// �I��

		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAction::ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerAction::ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_pPlayer->IsJump())
	{// ���n�ŏI��
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerAction::ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// ���[�V�����Ɠ����ɏI��
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// �W�����v�L���b�`
//==========================================================================
void CPlayerAction::ActionCatchJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//�t���b��
	UniqueJumpUpdate(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pPlayer->GetMotion()->IsFinish())
	{// ���[�V�����Ɠ����ɏI��
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAction::ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMotion* pMotion = m_pPlayer->GetMotion();

	if (pMotion->IsFinish())
	{// �I��
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// �W�����v����
//==========================================================================
void CPlayerAction::ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//�t���b��
	UniqueJumpUpdate(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pPlayer->GetMotion()->IsFinish())
	{// �W�����v�ڍs
		SetAction(CPlayer::EAction::ACTION_JUMP);
	}
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerAction::ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// [�J�n] �W�����v�L���b�`
//==========================================================================
void CPlayerAction::StartCatchJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ӂ����
	UniqueJumpStart();
}

//==========================================================================
// [�J�n] �W�����v����
//==========================================================================
void CPlayerAction::StartThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �ӂ����
	UniqueJumpStart();
}

//==========================================================================
// [�J�n] ���
//==========================================================================
void CPlayerAction::StartDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager::ETeamSide teamPlayer = m_pPlayer->GetTeam();

	//�_���[�W��t���Ȃ����Ԑݒ�
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.5f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	// �����ԂɈڍs
	// ����s�\���
	m_pPlayer->SetEnableMove(false);
	m_pPlayer->SetState(CPlayer::STATE_DODGE);
	m_pPlayer->SetMotion(CPlayer::MOTION_DODGE);

	// �Q�[���}�l�[�W���擾
	CGameManager* pGameMgr = CGameManager::GetInstance();
	if (pGameMgr == nullptr) return;

	// ���e���Z
	pGameMgr->AddCharmValue(teamPlayer, CCharmValueManager::ETypeAdd::ADD_DODGE);

	// �X�y�V�������Z
	pGameMgr->AddSpecialValue(teamPlayer, CSpecialValueManager::ETypeAdd::ADD_DODGE);
}

//==========================================================================
// [�I��] ���
//==========================================================================
void CPlayerAction::EndDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���G���ԉ���
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.0f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	// �X���[����
	float fRate = 1.0f;
	m_pPlayer->SetEnableMove(true);
	UtilFunc::Transformation::ValueNormalize(fRate, 1.0f, 0.0f);
	GET_MANAGER->SetSlowRate(fRate);

	// ��ԉ���
	m_pPlayer->SetState(CPlayer::EState::STATE_NONE);
}

//==========================================================================
// [�I��] �����Ƃ���
//==========================================================================
void CPlayerAction::EndUnstable(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_pPlayer->SetEnableBrake(false);
	m_pPlayer->SetEnableMove(true);
}

//==========================================================================
// �A�N�V�����ݒ�
//==========================================================================
void CPlayerAction::SetAction(CPlayer::EAction action)
{
	float fDeltaTime = CManager::GetInstance()->GetDeltaTime();		// 1�t���[���Ԃ̌o�ߎ���
	float fDeltaRate = CManager::GetInstance()->GetDeltaRate();		// �o�ߎ��Ԃ̊���
	float fSlowRate = CManager::GetInstance()->GetSlowRate();		// ���x�ቺ�̊���

	// �s���I��
	if (m_Action != action &&
		m_EndFunc[m_Action] != nullptr)
	{
		(this->*(m_EndFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	m_Action = action;
	m_fActionTime = 0.0f;		// �A�N�V��������

	// �s���J�n
	if (m_StartFunc[m_Action] != nullptr)
	{
		(this->*(m_StartFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �W�����v�n�A�N�V�����X�^�[�g
//==========================================================================
void CPlayerAction::UniqueJumpStart()
{
	//�t���b��
	CCharacterStatus::CharParameter param = m_pPlayer->GetParameter();
	MyLib::Vector3 move = m_pPlayer->GetMove();
	float fMoveY = param.fJumpStartMove;

	//���~��
	if (move.y < 0.0f)
	{
		fMoveY *= JUMPTHROW_DOWN_COR;
	}

	move.y = fMoveY;
	m_pPlayer->SetMove(move);
}

//==========================================================================
// �W�����v�n�X�V
//==========================================================================
void CPlayerAction::UniqueJumpUpdate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//�t���b��
	CCharacterStatus::CharParameter param = m_pPlayer->GetParameter();
	MyLib::Vector3 move = m_pPlayer->GetMove();
	float fMoveY = param.fJumpUpdateMove;

	//���~��
	if (move.y < 0.0f)
	{
		fMoveY *= JUMPTHROW_DOWN_COR;
	}

	fMoveY *= fDeltaRate * fSlowRate;

	move.y += fMoveY;
	m_pPlayer->SetMove(move);
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CPlayerAction::Debug()
{
	if (ImGui::Button("Dodge"))
	{
		// �A�N�V�����ݒ�
		SetAction(CPlayer::EAction::ACTION_DODGE);

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_DODGE);
	}
}

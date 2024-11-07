//=============================================================================
// 
//  AI�v���C���[���� [playerAI.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "teamStatus.h"


// �g�p�N���X
#include "playerStatus.h"
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace timing
{
	// �^�C�~���O
	const float TIMING_NONE = 0.0f;	// �ʏ�
	const float TIMING_NORMAL = 1.0f;	// �ʏ�
	const float TIMING_QUICK = 0.2f;	// ����
	const float TIMING_DELAY = 2.0f;	// �x��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAI::MODE_FUNC CPlayerAI::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAI::ModeNone,				// �Ȃ�
	&CPlayerAI::ModeThrowManager,		// �����}�l�[�W���[
	&CPlayerAI::ModeCatchManager,		// �L���b�`�}�l�[�W���[
};

CPlayerAI::TYPE_FUNC CPlayerAI::m_ThrowTypeFunc[] =	// ������ފ֐�
{
	&CPlayerAI::TypeNone,				// �Ȃ�
	&CPlayerAI::TypeThrow,				// �ʏ퓊��
	&CPlayerAI::TypeJumpThrow,			// �W�����v����
	&CPlayerAI::TypeSpecialThrow,		// �X�y�V��������
};

CPlayerAI::MOVE_FUNC CPlayerAI::m_ThrowMoveFunc[] =	// ������Ԋ֐�
{
	&CPlayerAI::MoveNormal,			// �ʏ�(���̏�)
	&CPlayerAI::MoveWalk,				// ����
	&CPlayerAI::MoveDash,				// ����
};

CPlayerAI::TIMING_FUNC CPlayerAI::m_ThrowTimingFunc[] =	// �^�C�~���O�֐�
{
	&CPlayerAI::TimingNormal,			// �ʏ�
	&CPlayerAI::TimingQuick,			// ����
	&CPlayerAI::TimingDelay,			// �x��
	& CPlayerAI::TimingFeint,			// �t�F�C���g
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAI::CPlayerAI()
{
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	m_fTimingCount = 0.0f;
	m_bTiming = false;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CPlayerAI *CPlayerAI::Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos)
{
	// �������̊m��
	CPlayerAI* pPlayer = DEBUG_NEW CPlayerAI;
	if (pPlayer != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pPlayer);
			return nullptr;
		}

		// �`�[���T�C�h��ݒ�
		pPlayer->GetStatus()->SetTeam(team);

		// �ʒu��ݒ�
		pPlayer->SetPosition(rPos);
	}

	return pPlayer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerAI::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// �e�N���X�̏�����
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// ����֘A
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	m_fTimingCount = timing::TIMING_QUICK;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAI::Uninit()
{
	// �e�N���X�̏I��
	CPlayer::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CPlayerAI::Kill()
{
	// �e�N���X�̏I��
	CPlayer::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԍX�V
	UpdateMode(fDeltaTime, fDeltaRate, fSlowRate);

	// ����
	MyLib::Vector3 rot = GetRotation();

	float fRange = D3DX_PI * 0.25f;

	float fRangeMinMax[] =
	{
		D3DX_PI * 1.0f + fRange,	// ��
		D3DX_PI * 1.0f - fRange,	// ��
		D3DX_PI * 0.0f + fRange,	// ��
		D3DX_PI * 0.0f - fRange,	// ��
		D3DX_PI * -0.5f + fRange,	// �E
		D3DX_PI * -0.5f - fRange,	// �E
		D3DX_PI * 0.5f + fRange,	// ��
		D3DX_PI * 0.5f - fRange,	// ��
	};

	for (int i = 0; i < sizeof(fRangeMinMax) / sizeof(fRangeMinMax[0]); i++)
	{
		UtilFunc::Transformation::RotNormalize(fRangeMinMax[i]);

		MyLib::Vector3 move(
			sinf(fRangeMinMax[i] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[i] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}

	{
		MyLib::Vector3 move(
			sinf(fRangeMinMax[0] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[0] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}

	{
		MyLib::Vector3 move(
			sinf(fRangeMinMax[1] + D3DX_PI) * 15.0f,
			0.0f,
			cosf(fRangeMinMax[1] + D3DX_PI) * 15.0f
		);

		CEffect3D::Create(
			GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f),
			move,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			20.0f, 0.5f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayerAI::Draw()
{
	// �e�N���X�̕`��
	CPlayer::Draw();
}

//==========================================================================
// �q�b�g����
//==========================================================================
CPlayerAI::SHitInfo CPlayerAI::Hit(CBall* pBall)
{
	CPlayer::SHitInfo hitInfo = CPlayer::Hit(pBall);	// �q�b�g���

	if (hitInfo.eHit == EHit::HIT_NONE){
		return hitInfo;
	}

	if (hitInfo.eHit == EHit::HIT_CATCH)
	{// �L���b�`���

		// �������[�h
		m_eMode = EMode::MODE_THROW;
	}

	return hitInfo;
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAI::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager* pGameManager = CGame::GetInstance()->GetGameManager();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(this->GetCharStatus()->GetTeam());

	if (pTeamStatus->IsMaxSpecial())
	{// �X�y�V����
		m_eThrowType = EThrowType::TYPE_SPECIAL;
	}
	else
	{
		int n = rand() % 2;

		/*switch (n)
		{
		case 0:
			m_eThrowType = EThrowType::TYPE_NORMAL;
			break;

		case 1:*/
			m_eThrowType = EThrowType::TYPE_JUMP;
			/*break;

		default:
			break;
		}*/

		// �������ލX�V
		(this->*(m_ThrowTypeFunc[m_eThrowType]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAI::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �ʏ퓊������
//==========================================================================
void CPlayerAI::TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐ݒ� : ����(���̏�)
	m_eThrowMove = EThrowMove::MOVE_NORMAL;

	// �����铮���X�V
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �W�����v��������
//==========================================================================
void CPlayerAI::TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐ݒ� : ����(���̏�)
	m_eThrowMove = EThrowMove::MOVE_NORMAL;

	// �����铮���X�V
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CPlayerAI::TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);

	// ���ꂼ��̏�Ԃ�߂�
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
}

//==========================================================================
// �ʏ퓊��(���̏�)
//==========================================================================
void CPlayerAI::MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�~���O�Ǘ�
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �����铮���X�V
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��������
//==========================================================================
void CPlayerAI::MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAI::MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �^�C�~���O�Ǘ�
//==========================================================================
void CPlayerAI::TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bTiming)
	{
		// �^�C�~���O�̐ݒ�
		int n = rand() % 3;

		switch (n)
		{
		case 0:
			m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
			m_fTimingCount = timing::TIMING_NORMAL;
			m_bTiming = true;
			break;

		case 1:
			m_eThrowTiming = EThrowTiming::TIMING_QUICK;
			m_fTimingCount = timing::TIMING_QUICK;
			m_bTiming = true;
			break;

		case 2:
			m_eThrowTiming = EThrowTiming::TIMING_DELAY;
			m_fTimingCount = timing::TIMING_DELAY;
			m_bTiming = true;
			break;
		default:
			break;
		}
	}
}

//==========================================================================
// �ʏ�^�C�~���O
//==========================================================================
void CPlayerAI::TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_eThrowType == EThrowType::TYPE_JUMP)
	{
		// ������
		pControlAIAction->SetIsJump(true);
		m_eThrowType = EThrowType::TYPE_NONE;
	}

	if (m_fTimingCount > 0)
	{// �J�E���g�_�E��
		m_fTimingCount -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// ������
	pControlAIAction->SetIsThrow(true);

	// ���ꂼ��̏�Ԃ̃��Z�b�g
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// �^�C�~���O���Z�b�g
	m_fTimingCount = timing::TIMING_NONE;
	m_bTiming = false;

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// �����^�C�~���O
//==========================================================================
void CPlayerAI::TimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_eThrowType == EThrowType::TYPE_JUMP)
	{
		// ������
		pControlAIAction->SetIsJump(true);
		m_eThrowType = EThrowType::TYPE_NONE;
	}

	if (m_fTimingCount > 0)
	{// �J�E���g�_�E��
		m_fTimingCount -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// ������
	pControlAIAction->SetIsThrow(true);

	// ���ꂼ��̏�Ԃ̃��Z�b�g
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// �J�E���g�̃��Z�b�g
	m_fTimingCount = timing::TIMING_NONE;
	m_bTiming = false;

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// �x���^�C�~���O
//==========================================================================
void CPlayerAI::TimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_eThrowType == EThrowType::TYPE_JUMP)
	{
		// ������
		pControlAIAction->SetIsJump(true);
		m_eThrowType = EThrowType::TYPE_NONE;
	}

	if (m_fTimingCount > 0)
	{// �J�E���g�_�E��
		m_fTimingCount -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// ������
	pControlAIAction->SetIsThrow(true);

	// ���ꂼ��̏�Ԃ̃��Z�b�g
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// �J�E���g�̃��Z�b�g
	m_fTimingCount = timing::TIMING_NONE;
	m_bTiming = false;

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// �t�F�C���g
//==========================================================================
void CPlayerAI::TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CPlayerAI::UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���쏈��
//==========================================================================
void CPlayerAI::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// �ړ�����	// TODO�FAI�ނ��`
	//m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	delete pControlMove;
	pControlMove = control;

	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	delete pControlAction;
	pControlAction = control;

	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayerAI::DeleteControl()
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	if (pControlMove != nullptr)
	{// �ړ�����
		delete pControlMove;
		pControlMove = nullptr;
	}

	if (pControlAction != nullptr)
	{// �A�N�V��������
		delete pControlAction;
		pControlAction = nullptr;
	}

	// ����֘A�폜
	CPlayer::DeleteControl();

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}


//==========================================================================
// �f�o�b�O����
//==========================================================================
void CPlayerAI::Debug()
{
#if _DEBUG
	// �f�o�b�O����
	CPlayer::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*> (GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// ���������t���O�ݒ�
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{ 
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}
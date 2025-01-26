//============================================================
//
//	�Q�[���I�����o�}�l�[�W���[���� [gameEndManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameEndManager.h"
#include "manager.h"
#include "camera.h"
#include "lightManager.h"
#include "player.h"
#include "gamemanager.h"
#include "lightPoint.h"
#include "gamesetUI.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY	= 4;		// �D�揇��
	const float	LIGHT_RANGE	= 600.0f;	// �����͈�
	const float	WAIT_TIME	= 2.0f;		// �ҋ@����
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGameEndManager::AFuncUpdateState CGameEndManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	&CGameEndManager::UpdateNone,		// �������Ȃ��X�V
	&CGameEndManager::UpdateLookPlayer,	// �v���C���[�J�����X�V
	&CGameEndManager::UpdateDeathWait,	// �v���C���[���S�ҋ@�X�V
	&CGameEndManager::UpdateGameSet,	// �Q�[���Z�b�g�o���X�V
	&CGameEndManager::UpdateWait,		// �ҋ@�X�V
	&CGameEndManager::UpdateEnd,		// �I���X�V
};
CGameEndManager* CGameEndManager::m_pInstance = nullptr;	// ���g�̃C���X�^���X

//************************************************************
//	�e�N���X [CGameEndManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameEndManager::CGameEndManager(CPlayer* pDeathPlayer) : CObject(PRIORITY),
	m_pDeathPlayer	(pDeathPlayer),	// ���S�v���C���[
	m_pGameSetUI	(nullptr),		// �Q�[���Z�b�gUI
	m_state			(STATE_NONE),	// ���
	m_fCurTime		(0.0f)			// ���݂̑ҋ@����
{
	// �����o�ϐ����N���A
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ���C�g�����J��Ԃ�

		m_apLight[i] = nullptr;	// ���C�g���
	}

	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CGameEndManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGameEndManager::~CGameEndManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGameEndManager::Init(void)
{
	CGameManager* pGameManager = CGameManager::GetInstance();	// �Q�[���}�l�[�W���[
	CManager* pManager = GET_MANAGER;	// �}�l�[�W���[

	// ��ނ��}�l�[�W���[�ɂ���
	SetType(CObject::TYPE::TYPE_MANAGER);

	// ���E��~���ɓ�����悤�ɂ���
	SetEnablePosibleMove_WorldPause(true);

	// �^�C�}�[���~
	pGameManager->SetEnableTimerStop(true);

	// �Q�[�����I�����o�V�[���ɕύX
	pGameManager->SetSceneType(CGameManager::ESceneType::SCENE_END_STAG);

	if (m_pDeathPlayer != nullptr)
	{ // �v���C���[���S�ŏI������ꍇ

		// ������Ԃ�ݒ�
		m_state = STATE_LOOK_PLAYER;

		for (int i = 0; i < NUM_LIGHT; i++)
		{ // ���C�g�����J��Ԃ�

			// ���C�g�̐���
			m_apLight[i] = CLightPoint::Create();
			if (m_apLight[i] == nullptr)
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// �g�U����ݒ�
			m_apLight[i]->SetDiffuse(MyLib::color::White());

			// �����͈͂�ݒ�
			m_apLight[i]->SetRange(LIGHT_RANGE);
		}

		// �X���[�{����ݒ�
		pManager->SetSlowRate(0.25f);

		// �̈�ق��Â�����
		pManager->GetLight()->SetEnableBright(false, 1.5f);

		// �Q�[���I���J�����̐ݒ�
		CCamera* pCamera = pManager->GetCamera();	// �J�������
		pCamera->SetState(CCamera::STATE_GAME_END, false);

		// �����_�Ώۂ����S�v���C���[�ɂ���
		pCamera->SetGameEndInfo(m_pDeathPlayer, MyLib::Vector3(0.0f, 80.0f, 0.0f), 1.0f);

		// ���C�g�ʒu�̐ݒ�
		SetLightPosition();
	}
	else
	{ // �^�C�}�[�Ȃǂ̑��v���ŏI������ꍇ

		// �Q�[���Z�b�gUI�̐���
		m_pGameSetUI = CGameSetUI::Create();
		if (m_pGameSetUI == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// ���E�̎����~�߂�
		pManager->SetEnableWorldPaused(true);

		// ������Ԃ�ݒ�
		m_state = STATE_GAME_SET;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameEndManager::Uninit(void)
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ���C�g�����J��Ԃ�

		// ���C�g�̏I��
		SAFE_UNINIT(m_apLight[i]);
	}

	// �I�u�W�F�N�g��j��
	Release();

	// ���g�̕ۑ����Ă����C���X�^���X��������
	m_pInstance = nullptr;
}

//============================================================
//	�폜����
//============================================================
void CGameEndManager::Kill()
{
	// ���g�̏I��
	Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGameEndManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	assert(m_state > -1 && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CGameEndManager::Draw()
{

}

//============================================================
//	��������
//============================================================
CGameEndManager *CGameEndManager::Create(CPlayer* pDeathPlayer)
{
	if (m_pInstance != nullptr)
	{ // ���N���X�̑��C���X�^���X������ꍇ

		// �C���X�^���X��j��
		SAFE_UNINIT(m_pInstance);
	}

	// �Q�[���I�����o�}�l�[�W���[�̐���
	CGameEndManager *pGameEndManager = DEBUG_NEW CGameEndManager(pDeathPlayer);
	if (pGameEndManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[���I�����o�}�l�[�W���[�̏�����
		if (FAILED(pGameEndManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[���I�����o�}�l�[�W���[�̔j��
			SAFE_DELETE(pGameEndManager);
			return nullptr;
		}

		// �C���X�^���X��ۑ�
		assert(m_pInstance == nullptr);
		m_pInstance = pGameEndManager;

		// �m�ۂ����A�h���X��Ԃ�
		return pGameEndManager;
	}
}

//============================================================
//	�������Ȃ��̍X�V����
//============================================================
void CGameEndManager::UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//============================================================
//	�v���C���[�J�����̍X�V����
//============================================================
void CGameEndManager::UpdateLookPlayer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CManager* pManager = GET_MANAGER;			// �}�l�[�W���[
	CCamera* pCamera = pManager->GetCamera();	// �J�������
	if (pCamera->IsGameEnd())
	{ // �Q�[���I���J�����̈ړ����I������ꍇ

		// �X���[�{����������
		pManager->SetSlowRate(1.0f);

		// �v���C���[���S�ҋ@��Ԃɂ���
		m_state = STATE_DEATH_WAIT;
	}

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�v���C���[���S�ҋ@�̍X�V����
//============================================================
void CGameEndManager::UpdateDeathWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CManager* pManager = GET_MANAGER;			// �}�l�[�W���[
	CCamera* pCamera = pManager->GetCamera();	// �J�������
	if (m_pDeathPlayer->GetState() == CPlayer::EState::STATE_DEAD_CARRY)
	{ // �^�����ɔR���s�����ꍇ

		// �Q�[���Z�b�gUI�̐���
		m_pGameSetUI = CGameSetUI::Create();

		// �Œ�J�����̐ݒ�
		pCamera->SetState(CCamera::STATE_NONE, false);

		// �Q�[���Z�b�g�o����Ԃɂ���
		m_state = STATE_GAME_SET;
	}

	// ���C�g�ʒu�̐ݒ�
	SetLightPosition();
}

//============================================================
//	�Q�[���Z�b�g�o���̍X�V����
//============================================================
void CGameEndManager::UpdateGameSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pGameSetUI->IsEnd())
	{ // UI�̉��o���I�������ꍇ

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CGameEndManager::UpdateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= WAIT_TIME)
	{ // �ҋ@�I�������ꍇ

		// �ҋ@���Ԃ̏�����
		m_fCurTime = 0.0f;

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CGameEndManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �Q�[�����I���V�[���ɕύX
	CGameManager::GetInstance()->SetSceneType(CGameManager::ESceneType::SCENE_END);

	// ���g�̏I��
	Uninit();
}

//============================================================
//	���C�g�ʒu�̐ݒ菈��
//============================================================
void CGameEndManager::SetLightPosition()
{
	const MyLib::Vector3 posPlayer = m_pDeathPlayer->GetPosition();	// �v���C���[�ʒu
	float fRot = 0.0f;	// �I�t�Z�b�g����
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ���C�g�����J��Ԃ�

		if (m_apLight[i] != nullptr)
		{
			// ���C�g�ʒu�̐ݒ�
			MyLib::Vector3 posOffset = MyLib::Vector3(sinf(fRot) * 80.0f, 160.0f, cosf(fRot) * 80.0f);	// �I�t�Z�b�g
			m_apLight[i]->SetPosition(posPlayer + posOffset);
		}

		// �I�t�Z�b�g��������]
		fRot += HALF_PI;
	}
}

//============================================================
//
//	�v���C���[�o�ꉉ�o�}�l�[�W���[���� [playerSpawnManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerSpawnManager.h"
#include "specialEffect.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"

#include "game.h"
#include "gameManager.h"
#include "player.h"
#include "playerStatus.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 4;	// �D�揇��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CPlayerSpawnManager::AFuncUpdateState CPlayerSpawnManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,							// �������Ȃ��X�V
	&CPlayerSpawnManager::UpdateCutIn,	// �J�b�g�C���X�V
	&CPlayerSpawnManager::UpdateEnd,	// �I���X�V
};
CPlayerSpawnManager* CPlayerSpawnManager::m_pThisClass = nullptr;	// ���g�̃C���X�^���X

//************************************************************
//	�e�N���X [CPlayerSpawnManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerSpawnManager::CPlayerSpawnManager() : CObject(PRIORITY),
	m_state		(STATE_NONE),	// ���
	m_fCurTime	(0.0f)			// ���݂̑ҋ@����
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CPlayerSpawnManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerSpawnManager::~CPlayerSpawnManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerSpawnManager::Init(void)
{
	// FUJITA�F�G���g���[�ŊO���ɕۑ����ꂽ�v���C���[�����擾���v���C���[�𐶐�

	// �����o�ϐ���������
	m_state		= STATE_CUTIN;	// ���
	m_fCurTime	= 0.0f;			// ���݂̑ҋ@����

	// ��ނ��}�l�[�W���[�ɂ���
	SetType(CObject::TYPE::TYPE_MANAGER);

	// ���`�[���v���C���[����
	for (int i = 0; i < 5; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(-200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pLeftPlayer = CPlayer::Create
		(
			pos + offset, 					// �ʒu
			CGameManager::SIDE_LEFT,		// �`�[���T�C�h
			CPlayer::EFieldArea::FIELD_IN,	// �|�W�V����
			CPlayer::EBaseType::TYPE_USER,	// �x�[�X�^�C�v
			CPlayer::EBody::BODY_GARI,		// �̌n
			CPlayer::EHandedness::HAND_L	// ������
		);
		if (pLeftPlayer == nullptr)
		{
			return E_FAIL;
		}
	}

	// �E�`�[���v���C���[����
	for (int i = 0; i < 5; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pRightPlayer = CPlayer::Create
		(
			pos + offset, 					// �ʒu
			CGameManager::SIDE_RIGHT,		// �`�[���T�C�h
			CPlayer::EFieldArea::FIELD_IN,	// �|�W�V����
			CPlayer::EBaseType::TYPE_USER,	// �x�[�X�^�C�v
			CPlayer::EBody::BODY_DEBU,		// �̌n
			CPlayer::EHandedness::HAND_R	// ������
		);
		if (pRightPlayer == nullptr)
		{
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerSpawnManager::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	Release();

	// ���g�̕ۑ����Ă����C���X�^���X��������
	m_pThisClass = nullptr;
}

//============================================================
//	�폜����
//============================================================
void CPlayerSpawnManager::Kill()
{
	// ���g�̏I��
	Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerSpawnManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerSpawnManager::Draw()
{

}

//============================================================
//	��������
//============================================================
CPlayerSpawnManager *CPlayerSpawnManager::Create()
{
	if (m_pThisClass != nullptr)
	{ // ���N���X�̑��C���X�^���X������ꍇ

		// �C���X�^���X��j��
		SAFE_UNINIT(m_pThisClass);
	}

	// �v���C���[�o�ꉉ�o�}�l�[�W���[�̐���
	CPlayerSpawnManager *pPlayerSpawnManager = DEBUG_NEW CPlayerSpawnManager;
	if (pPlayerSpawnManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�o�ꉉ�o�}�l�[�W���[�̏�����
		if (FAILED(pPlayerSpawnManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�o�ꉉ�o�}�l�[�W���[�̔j��
			SAFE_DELETE(pPlayerSpawnManager);
			return nullptr;
		}

		// �C���X�^���X��ۑ�
		assert(m_pThisClass == nullptr);
		m_pThisClass = pPlayerSpawnManager;

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayerSpawnManager;
	}
}

//============================================================
//	�J�b�g�C���̍X�V����
//============================================================
void CPlayerSpawnManager::UpdateCutIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//if ()
	{ // 

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CPlayerSpawnManager::UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���g�̏I��
	Uninit();
}

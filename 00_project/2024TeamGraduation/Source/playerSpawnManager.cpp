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
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "player.h"

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
	nullptr,							// �I���X�V
};
CPlayerSpawnManager* CPlayerSpawnManager::m_pInstance = nullptr;	// ���g�̃C���X�^���X

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

	//// ���`�[���v���C���[����
	//for (int i = 0; i < 5; i++)
	//{
	//	MyLib::Vector3 pos = MyLib::Vector3(-200.0f, 0.0f, -100.0f);
	//	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
	//	CPlayer* pLeftPlayer = CPlayer::Create
	//	(
	//		pos + offset, 					// �ʒu
	//		CGameManager::SIDE_LEFT,		// �`�[���T�C�h
	//		CPlayer::EHuman::HUMAN_SPAWN,	// �l
	//		CPlayer::EBody::BODY_GARI,		// �̌n
	//		CPlayer::EHandedness::HAND_L	// ������
	//	);
	//	if (pLeftPlayer == nullptr)
	//	{
	//		return E_FAIL;
	//	}
	//}

	//// �E�`�[���v���C���[����
	//for (int i = 0; i < 5; i++)
	//{
	//	MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);
	//	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
	//	CPlayer* pRightPlayer = CPlayer::Create
	//	(
	//		pos + offset, 					// �ʒu
	//		CGameManager::SIDE_RIGHT,		// �`�[���T�C�h
	//		CPlayer::EHuman::HUMAN_SPAWN,	// �l
	//		CPlayer::EBody::BODY_DEBU,		// �̌n
	//		CPlayer::EHandedness::HAND_R	// ������
	//	);
	//	if (pRightPlayer == nullptr)
	//	{
	//		return E_FAIL;
	//	}
	//}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerSpawnManager::Uninit(void)
{
	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// �v���C���[�̏I��
		SAFE_UNINIT(pPlayer);
	}

	// �I�u�W�F�N�g��j��
	Release();

	// ���g�̕ۑ����Ă����C���X�^���X��������
	m_pInstance = nullptr;
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

	// �I����Ԃɂ���
	m_state = STATE_END;
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
	// �C���X�^���X�����ς�
	assert(m_pInstance == nullptr);

	// �v���C���[�o�ꉉ�o�}�l�[�W���[�̐���
	m_pInstance = DEBUG_NEW CPlayerSpawnManager;
	if (m_pInstance == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�o�ꉉ�o�}�l�[�W���[�̏�����
		if (FAILED(m_pInstance->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�o�ꉉ�o�}�l�[�W���[�̔j��
			SAFE_DELETE(m_pInstance);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return m_pInstance;
	}
}

//============================================================
//	�C���X�^���X�擾����
//============================================================
CPlayerSpawnManager* CPlayerSpawnManager::GetInstance()
{
	// �C���X�^���X������
	assert(m_pInstance != nullptr);

	// ���g�̃C���X�^���X��Ԃ�
	return m_pInstance;
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

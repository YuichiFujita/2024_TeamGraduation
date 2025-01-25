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
#include "playerManager.h"
#include "gymWallManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 4;			// �D�揇��
	const float	ZLINE_OFFSET = 200.0f;	// Z���I�t�Z�b�g
	const MyLib::Vector3 POS_LEFT = MyLib::Vector3(-972.85f, 0.0f, 1817.35f);	// ���擪�v���C���[�����ʒu
	const MyLib::Vector3 POS_RIGHT = MyLib::Vector3(972.85f, 0.0f, 1817.35f);	// �E�擪�v���C���[�����ʒu
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CPlayerSpawnManager::AFuncUpdateState CPlayerSpawnManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,								// �������Ȃ����
	&CPlayerSpawnManager::UpdateOpenDoor,	// �h�A�J�����
	&CPlayerSpawnManager::UpdateWalkAxisZ,	// Z���ړ����
	&CPlayerSpawnManager::UpdateRotate,		// ��]���
	&CPlayerSpawnManager::UpdateWalkAxisX,	// X���ړ����
	&CPlayerSpawnManager::UpdateBow,		// �����V���
	nullptr,								// �I���X�V
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
	// �v���C���[�}�l�[�W���[
	CPlayerManager::Create(CPlayerManager::EType::TYPE_SPAWN);

	// �����o�ϐ���������
	m_state		= STATE_OPEN_DOOR;	// ���
	m_fCurTime	= 0.0f;				// ���݂̑ҋ@����

	// ��ނ��}�l�[�W���[�ɂ���
	SetType(CObject::TYPE::TYPE_MANAGER);

	CCamera* pCamera = GET_MANAGER->GetCamera();				// �J�������
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������

	// �J�����ʒu�����_�ɂ���
	pCameraMotion->SetPosition(VEC3_ZERO);

	// �o�ꉉ�o���[�V������ݒ�
	pCameraMotion->SetMotion(CCameraMotion::MOTION_SPAWN);

	// BGM�Đ�
	CSound* pSound = CSound::GetInstance();
	pSound->StopSound();
	pSound->PlaySound(CSound::ELabel::LABEL_BGM_SPAWN);

	// �̈�ق̕ǐ���
	CGymWallManager* pWall = CGameManager::GetInstance()->GetGymWallManager();
	pWall->SetIsWall(true);

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
		if (pPlayer != nullptr)
		{
			pPlayer->Kill();
			pPlayer = nullptr;
		}
	}

	// �v���C���[�}�l�[�W���[
	CPlayerManager* pManager = CPlayerManager::GetInstance();
	SAFE_UNINIT(pManager);

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
	// �̈�ق̕Ǎ폜
	CGymWallManager* pWall = CGameManager::GetInstance()->GetGymWallManager();
	pWall->SetIsWall(false);

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
// �v���C���[�o�^����
//============================================================
void CPlayerSpawnManager::RegistPlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // �`�[���T�C�h���Ƃ̏���
	case CGameManager::ETeamSide::SIDE_LEFT:

		// �����X�g�ɓo�^
		m_listLeft.Regist(pPlayer);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// �E���X�g�ɓo�^
		m_listRight.Regist(pPlayer);
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
// �v���C���[�폜����
//============================================================
void CPlayerSpawnManager::DeletePlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // �`�[���T�C�h���Ƃ̏���
	case CGameManager::ETeamSide::SIDE_LEFT:

		// �����X�g����폜
		m_listLeft.Delete(pPlayer);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// �E���X�g����폜
		m_listRight.Delete(pPlayer);
		break;

	default:
		assert(false);
		break;
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
//	�h�A�J����Ԃ̍X�V����
//============================================================
void CPlayerSpawnManager::UpdateOpenDoor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const float TIME_END = 1.35f;	// �I������

	CCamera* pCamera = GET_MANAGER->GetCamera();				// �J�������
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������
	if (pCameraMotion->GetNowTriggerIdx() <= 0) { return; }		// �g���K�[�����ɍs���܂őҋ@

	// ���[�V�����̃h�A�J����u��
	CMotion* pMotion = m_listLeft.GetData(0)->GetMotion();
	if (pMotion->GetType() == CPlayer::EMotion::MOTION_DOOR &&
		pMotion->IsImpactFrame(pMotion->GetInfo()))
	{
		// �h�A���J������
		CGameManager* pManager = CGameManager::GetInstance();	// �Q�[���}�l�[�W���[
		pManager->SetEnableOpen(true, 0.2f);
	}

	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // �o�߂��������ꍇ

		// �ҋ@���Ԃ�������
		m_fCurTime = 0.0f;

		// Z���ړ���Ԃɂ���
		m_state = EState::STATE_WALK_Z;

		// ��������
		m_listLeft.GetData(0)->SetMotion(CPlayer::EMotion::MOTION_WALK_SPAWN);
		m_listRight.GetData(0)->SetMotion(CPlayer::EMotion::MOTION_WALK_SPAWN);
	}
}

//============================================================
//	Z���ړ���Ԃ̍X�V����
//============================================================
void CPlayerSpawnManager::UpdateWalkAxisZ(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const MyLib::Vector3 LEFT_POSEND = MyLib::Vector3(POS_LEFT.x, 0.0f, -300.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const MyLib::Vector3 RIGHT_POSEND = MyLib::Vector3(POS_RIGHT.x, 0.0f, -300.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const float TIME_END = 7.5f;	// �I������

	bool bEndState = false;	// ��ԏI���t���O

	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // �o�߂��������ꍇ

		// �o�ߎ��Ԃ�␳
		m_fCurTime = TIME_END;

		// ��Ԃ̏I����ۑ�
		bEndState = true;
	}

	CGameManager* pManager = CGameManager::GetInstance();	// �Q�[���}�l�[�W���[
	if (pManager->IsOpen() && m_fCurTime >= 1.6f)
	{ // �h�A�����Ă��Ȃ��ꍇ

		// �h�A���������߂�
		pManager->SetEnableOpen(false, 1.6f);
	}

	std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
	int nCntLeft = 0;	// ���`�[�����[�v��
	while (m_listLeft.ListLoop(itrLeft))
	{ // ���`�[�����X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itrLeft);	// �v���C���[���

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntLeft);	// Z���I�t�Z�b�g

		// ���`��Ԃňړ�
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(POS_LEFT + offset, LEFT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ���[�v���̉��Z
		nCntLeft++;
	}

	std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// �E�`�[���̍Ō���C�e���[�^�[
	int nCntRight = 0;	// �E�`�[�����[�v��
	while (m_listRight.ListLoop(itrRight))
	{ // �E�`�[�����X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itrRight);	// �v���C���[���

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntRight);	// Z���I�t�Z�b�g

		// ���`��Ԃňړ�
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(POS_RIGHT + offset, RIGHT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ���[�v���̉��Z
		nCntRight++;
	}

	if (bEndState)
	{ // ��Ԃ��I������ꍇ

		// �ҋ@���Ԃ�������
		m_fCurTime = 0.0f;

		// ��]��Ԃɂ���
		m_state = EState::STATE_ROTATE;
	}
}

//============================================================
//	��]��Ԃ̍X�V����
//============================================================
void CPlayerSpawnManager::UpdateRotate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const MyLib::Vector3 LEFT_ROTEND = MyLib::Vector3(0.0f, -HALF_PI, 0.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const MyLib::Vector3 RIGHT_ROTEND = MyLib::Vector3(0.0f, HALF_PI, 0.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const float TIME_END = 0.5f;	// �I������

	bool bEndState = false;	// ��ԏI���t���O

	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // �o�߂��������ꍇ

		// �o�ߎ��Ԃ�␳
		m_fCurTime = TIME_END;

		// ��Ԃ̏I����ۑ�
		bEndState = true;
	}

	std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
	while (m_listLeft.ListLoop(itrLeft))
	{ // ���`�[�����X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itrLeft);	// �v���C���[���

		// ���`��Ԃŉ�]
		pItrPlayer->SetRotation(UtilFunc::Correction::EasingLinear(VEC3_ZERO, LEFT_ROTEND, 0.0f, TIME_END, m_fCurTime));
	}

	std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// �E�`�[���̍Ō���C�e���[�^�[
	while (m_listRight.ListLoop(itrRight))
	{ // �E�`�[�����X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itrRight);	// �v���C���[���

		// ���`��Ԃŉ�]
		pItrPlayer->SetRotation(UtilFunc::Correction::EasingLinear(VEC3_ZERO, RIGHT_ROTEND, 0.0f, TIME_END, m_fCurTime));
	}

	if (bEndState)
	{ // ��Ԃ��I������ꍇ

		// �ҋ@���Ԃ�������
		m_fCurTime = 0.0f;

		// X���ړ���Ԃɂ���
		m_state = EState::STATE_WALK_X;


		//-----------------------------
		// �S���܂��܂�����
		//-----------------------------
		std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
		while (m_listLeft.ListLoop(itrLeft))
		{ // ���`�[�����X�g���̗v�f�����J��Ԃ�

			CPlayer* pItrPlayer = (*itrLeft);	// �v���C���[���

			// �������[�V�����ݒ�
			CMotionManager::Info info = pItrPlayer->GetMotion()->GetInfo(CPlayer::EMotion::MOTION_WALK);	// ���[�V�������
			int nKey = rand() % info.nNumKey;														// �J�n�L�[
			float fFrame = (float)(rand() % info.aKey[nKey].nFrame);								// �J�n�t���[��
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_WALK, nKey, true, fFrame);		// ���[�V�����ݒ�
		}

		std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// �E�`�[���̍Ō���C�e���[�^�[
		while (m_listRight.ListLoop(itrRight))
		{ // �E�`�[�����X�g���̗v�f�����J��Ԃ�

			CPlayer* pItrPlayer = (*itrRight);	// �v���C���[���

			// �������[�V�����ݒ�
			CMotionManager::Info info = pItrPlayer->GetMotion()->GetInfo(CPlayer::EMotion::MOTION_WALK);	// ���[�V�������
			int nKey = rand() % info.nNumKey;														// �J�n�L�[
			float fFrame = (float)(rand() % info.aKey[nKey].nFrame);								// �J�n�t���[��
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_WALK, nKey, true, fFrame);		// ���[�V�����ݒ�
		}
	}
}

//============================================================
//	X���ړ���Ԃ̍X�V����
//============================================================
void CPlayerSpawnManager::UpdateWalkAxisX(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	const MyLib::Vector3 LEFT_OLD_POSEND = MyLib::Vector3(POS_LEFT.x, 0.0f, -300.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const MyLib::Vector3 RIGHT_OLD_POSEND = MyLib::Vector3(POS_RIGHT.x, 0.0f, -300.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const MyLib::Vector3 LEFT_POSEND = MyLib::Vector3(-100.0f, 0.0f, -300.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const MyLib::Vector3 RIGHT_POSEND = MyLib::Vector3(100.0f, 0.0f, -300.0f);	// �I���n�_	// TODO�F�܂Ƃ��ȏI���n�_��
	const float TIME_END = 2.5f;	// �I������

	bool bEndState = false;	// ��ԏI���t���O

	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END)
	{ // �o�߂��������ꍇ

		// �o�ߎ��Ԃ�␳
		m_fCurTime = TIME_END;

		// ��Ԃ̏I����ۑ�
		bEndState = true;
	}

	std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
	int nCntLeft = 0;	// ���`�[�����[�v��
	while (m_listLeft.ListLoop(itrLeft))
	{ // ���`�[�����X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itrLeft);	// �v���C���[���

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntLeft);	// Z���I�t�Z�b�g

		// ���`��Ԃňړ�
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(LEFT_OLD_POSEND + offset, LEFT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ���[�v���̉��Z
		nCntLeft++;
	}

	std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// �E�`�[���̍Ō���C�e���[�^�[
	int nCntRight = 0;	// �E�`�[�����[�v��
	while (m_listRight.ListLoop(itrRight))
	{ // �E�`�[�����X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itrRight);	// �v���C���[���

		MyLib::Vector3 offset = (MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET) * (float)nCntRight);	// Z���I�t�Z�b�g

		// ���`��Ԃňړ�
		pItrPlayer->SetPosition(UtilFunc::Correction::EasingLinear(RIGHT_OLD_POSEND + offset, RIGHT_POSEND + offset, 0.0f, TIME_END, m_fCurTime));

		// ���[�v���̉��Z
		nCntRight++;
	}

	if (bEndState)
	{ // ��Ԃ��I������ꍇ

		// �ҋ@���Ԃ�������
		m_fCurTime = 0.0f;

		// �����V��Ԃɂ���
		m_state = EState::STATE_BOW;

		//-----------------------------
		// �S����
		//-----------------------------
		std::list<CPlayer*>::iterator itrLeft = m_listLeft.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
		while (m_listLeft.ListLoop(itrLeft))
		{ // ���`�[�����X�g���̗v�f�����J��Ԃ�

			CPlayer* pItrPlayer = (*itrLeft);	// �v���C���[���

			// �烂�[�V����
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_BOW);
		}

		std::list<CPlayer*>::iterator itrRight = m_listRight.GetEnd();	// �E�`�[���̍Ō���C�e���[�^�[
		while (m_listRight.ListLoop(itrRight))
		{ // �E�`�[�����X�g���̗v�f�����J��Ԃ�

			CPlayer* pItrPlayer = (*itrRight);	// �v���C���[���

			// �烂�[�V����
			pItrPlayer->GetMotion()->Set(CPlayer::EMotion::MOTION_BOW);
		}
	}
}

//============================================================
//	�����V��Ԃ̍X�V����
//============================================================
void CPlayerSpawnManager::UpdateBow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

#if 0
	const float TIME_END = 1.0f;	// �I������

	// TODO�F�����͂����V���[�V��������J�ڃ^�C�~���O�����߂�

	CCamera* pCamera = GET_MANAGER->GetCamera();				// �J�������
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// �J�������[�V�������

	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	if (m_fCurTime >= TIME_END
	&&  pCameraMotion->IsFinish())
	{ // �o�߂��������ꍇ

		// �ҋ@���Ԃ�������
		m_fCurTime = 0.0f;

		// �I����Ԃɂ���
		m_state = EState::STATE_END;
	}
#else

	// �擪�擾
	CPlayer* pFirstPlayer = m_listRight.GetData(0);
	CMotion* pMotion = pFirstPlayer->GetMotion();

	if (pMotion->GetType() == CPlayer::EMotion::MOTION_BOW &&
		pMotion->IsFinish())
	{// �� && �I��

		// �ҋ@���Ԃ�������
		m_fCurTime = 0.0f;

		// �I����Ԃɂ���
		m_state = EState::STATE_END;
	}

#endif
}

//==========================================================================
// 
//  �v���C���[�}�l�[�W���[���� [playerManager.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerManager.h"
#include "playerUserOut.h"
#include "bindKeyFront.h"
#include "bindKeyBack.h"
#include "bindKeyLeft.h"
#include "bindKeyRight.h"

// �h���N���X
#include "playerManager_game.h"
#include "playerManager_result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TOP_LINE = "#==============================================================================";	// �e�L�X�g�̃��C��
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// �e�L�X�g�̃��C��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// �O����֐�
CPlayerManager::INFO_FUNC CPlayerManager::m_InfoFuncList[] =
{
	&CPlayerManager::GetInfoLeftFar,	// �����̊O����
	&CPlayerManager::GetInfoLeft,		// ���̊O����
	&CPlayerManager::GetInfoLeftNear,	// ����O�̊O����
	&CPlayerManager::GetInfoRightFar,	// �E���̊O����
	&CPlayerManager::GetInfoRight,		// �E�̊O����
	&CPlayerManager::GetInfoRightNear,	// �E��O�̊O����
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CPlayerManager* CPlayerManager::m_pInstance = nullptr;	// ���g�̃C���X�^���X
std::vector<CPlayerManager::LoadInfo> CPlayerManager::m_vecLoadInfo[CGameManager::ETeamSide::SIDE_MAX] = {};	// �ǂݍ��ݏ��

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerManager::CPlayerManager() : m_bUserChange(false)	// ���[�U�[�ύX����t���O
{
	// �����o�ϐ����N���A
	memset(&m_apOut[0], 0, sizeof(m_apOut));	// �O��v���C���[

	for (int i= 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		m_vecLoadInfo[i].clear();
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerManager::~CPlayerManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CPlayerManager* CPlayerManager::Create(CScene::MODE mode)
{
	// ���ɐ����ς݂̏ꍇ������
	if (m_pInstance != nullptr) { return m_pInstance; }

	// �C���X�^���X�̐���
	switch (mode)
	{
	case CScene::MODE_GAME:
		m_pInstance = DEBUG_NEW CPlayerManager_Game;
		break;

	case CScene::MODE_RESULT:
		m_pInstance = DEBUG_NEW CPlayerManager_Result;
		break;

	default:
		m_pInstance = DEBUG_NEW CPlayerManager;
		break;
	}

	if (m_pInstance != nullptr)
	{ // �������̊m�ۂ��o�����ꍇ

		// �C���X�^���X�̏�����
		if (FAILED(m_pInstance->Init()))
		{ // �������Ɏ��s�����ꍇ

			return nullptr;
		}
	}

	return m_pInstance;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerManager::Init()
{
#ifdef ENTRYSTART

	// FUJITA�F�G���g���[�ŊO���ɕۑ����ꂽ�v���C���[�����擾���v���C���[�𐶐�
	// �ǂݍ��ݏ���
	Load();

	// �ǂݍ��񂾏������ƂɃv���C���[����
	int nLeft = 0, nRight = 0;
	for (int j = 0; j < CGameManager::MAX_SIDEPLAYER; j++)
	{
		// ���̃v���C���[����
		if (j < static_cast<int>(m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT].size()))
		{
			CreateLeftPlayer(nLeft, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT][j]);
			nLeft++;
		}

		// �E�̃v���C���[����
		if (j < static_cast<int>(m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT].size()))
		{
			CreateRightPlayer(nRight, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT][j]);
			nRight++;
		}
	}

#else

	//----------------------------------------------------------------------
	// �v���C���[���쐶��
	//----------------------------------------------------------------------
	// �v���C���[User�l�l����(���E)
#if 0
	// �v���C���[User��l����(�E)
	for (int i = 0; i < 2; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pUser = CPlayer::Create
		(
			pos + offset, 					// �ʒu
			CGameManager::SIDE_RIGHT,		// �`�[���T�C�h
			CPlayer::EFieldArea::FIELD_IN,	// �|�W�V����
			(CPlayer::EBaseType)i,			// �x�[�X�^�C�v
			CPlayer::EBody::BODY_DEBU,		// �̌n
			CPlayer::EHandedness::HAND_R	// ������
		);
		if (pUser == nullptr)
		{
			return E_FAIL;
		}
		pUser->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
		pUser->SetRotDest(HALF_PI);
	}

	// �v���C���[User��l����(��)
	for (int i = 0; i < 2; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(-200.0f, 0.0f, -100.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 200.0f * (float)i);
		CPlayer* pUser = CPlayer::Create
		(
			pos + offset, 					// �ʒu
			CGameManager::SIDE_LEFT,		// �`�[���T�C�h
			CPlayer::EFieldArea::FIELD_IN,	// �|�W�V����
			(CPlayer::EBaseType)i,			// �x�[�X�^�C�v
			CPlayer::EBody::BODY_GARI,		// �̌n
			CPlayer::EHandedness::HAND_L	// ������
		);
		if (pUser == nullptr)
		{
			return E_FAIL;
		}
		pUser->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
		pUser->SetRotDest(-HALF_PI);
	}
#endif

	// �v���C���[User����(��)
#if 1
	CPlayer* pUser = CPlayer::Create(MyLib::Vector3(-200.0f, 0.0f, 0.0f), CGameManager::SIDE_LEFT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_USER);
	if (pUser == nullptr)
	{
		return E_FAIL;
	}
	pUser->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
	pUser->SetRotDest(-HALF_PI);
#endif

	// �v���C���[User�񐢐���(�E)
#if 0
	CPlayer* pUser2 = CPlayer::Create(MyLib::Vector3(200.0f, 0.0f, 0.0f), CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_USER);
	if (pUser2 == nullptr)
	{
		return E_FAIL;
	}
	pUser2->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
	pUser2->SetRotDest(HALF_PI);
#endif

	// �v���C���[AI��l����(�E)
#if 1
	CPlayer* pAI = CPlayer::Create(MyLib::Vector3(200.0f, 0.0f, 0.0f), CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
	if (pAI == nullptr)
	{
		return E_FAIL;
	}
	pAI->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
	pAI->SetRotDest(HALF_PI);
#endif

	// �v���C���[AI��l����(��)
#if 0
	CPlayer* pAI2 = CPlayer::Create(MyLib::Vector3(-200.0f, 0.0f, 200.0f), CGameManager::SIDE_LEFT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
	if (pAI2 == nullptr)
	{
		return E_FAIL;
	}
	pAI2->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
	pAI2->SetRotDest(-HALF_PI);
#endif

	// �v���C���[AI�l�l����(�E)
#if 0
	for (int i = 0; i < 3; i++)
	{
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, 0.0f) + MyLib::Vector3(0.0f, 0.0f, -150.0f);
		MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, 100.0f * (float)i);
		CPlayer* pAI = CPlayer::Create(pos + offset, CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
		if (pAI == nullptr)
		{
			return E_FAIL;
		}
		pAI->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
		pAI->SetRotDest(HALF_PI);
	}
#endif

	//----------------------------------------------------------------------
	// �v���C���[�O�쐶��
	//----------------------------------------------------------------------
	int nHalfMax = EOutPos::OUT_MAX / 2;	// �`�[�����Ƃ̊O�쑍��

	// �v���C���[�O�쐶�� (�E�T�C�h)
#if 1
	for (int i = 0; i < nHalfMax; i++)
	{ // �`�[�����Ƃ̊O��l�����J��Ԃ�

		// ���`�[���̊O��v���C���[����
		CPlayer* pOutLeft = CPlayer::Create
		(
			VEC3_ZERO,						// �ʒu
			CGameManager::SIDE_RIGHT,		// �`�[���T�C�h
			CPlayer::EFieldArea::FIELD_OUT,	// �|�W�V����
			CPlayer::EBaseType::TYPE_AI		// �x�[�X�^�C�v
		);
		if (pOutLeft == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}
	}
#endif

	// �v���C���[�O�쐶�� (���T�C�h)
#if 1
	for (int i = 0; i < nHalfMax; i++)
	{ // �`�[�����Ƃ̊O��l�����J��Ԃ�

		// ���`�[���̊O��v���C���[����
		CPlayer* pOutLeft = CPlayer::Create
		(
			VEC3_ZERO,						// �ʒu
			CGameManager::SIDE_LEFT,		// �`�[���T�C�h
			CPlayer::EFieldArea::FIELD_OUT,	// �|�W�V����
			CPlayer::EBaseType::TYPE_AI		// �x�[�X�^�C�v
		);
		if (pOutLeft == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}
	}
#endif

#endif // ENTRY


	return S_OK;
}

//==========================================================================
// ���̃v���C���[����
//==========================================================================
HRESULT CPlayerManager::CreateLeftPlayer(int i, const LoadInfo& info)
{
	MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);

	CPlayer::EBaseType baseType = (info.nControllIdx >= 0) ? CPlayer::EBaseType::TYPE_USER : CPlayer::EBaseType::TYPE_AI;	// �x�[�X�^�C�v

	CPlayer* pPlayer = CPlayer::Create
	(
		pos, 								// �ʒu
		CGameManager::ETeamSide::SIDE_LEFT,	// �`�[���T�C�h
		CPlayer::EFieldArea::FIELD_IN,		// �|�W�V����
		baseType,							// �x�[�X�^�C�v
		info.eBody,							// �̌n
		info.eHanded						// ������
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}
	pPlayer->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
	pPlayer->SetRotDest(HALF_PI);
	
	// �C���f�b�N�X���f
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// �h���X�A�b�v���f
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// �E�̃v���C���[����
//==========================================================================
HRESULT CPlayerManager::CreateRightPlayer(int i, const LoadInfo& info)
{
	MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, -100.0f);

	CPlayer::EBaseType baseType = (info.nControllIdx >= 0) ? CPlayer::EBaseType::TYPE_USER : CPlayer::EBaseType::TYPE_AI;	// �x�[�X�^�C�v

	CPlayer* pPlayer = CPlayer::Create
	(
		pos, 								// �ʒu
		CGameManager::ETeamSide::SIDE_RIGHT,	// �`�[���T�C�h
		CPlayer::EFieldArea::FIELD_IN,		// �|�W�V����
		baseType,							// �x�[�X�^�C�v
		info.eBody,							// �̌n
		info.eHanded						// ������
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}
	pPlayer->SetRotation(MyLib::Vector3(0.0f, -HALF_PI, 0.0f));
	pPlayer->SetRotDest(-HALF_PI);

	// �C���f�b�N�X���f
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// �h���X�A�b�v���f
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerManager::Uninit()
{
	// ���g�̃C���X�^���X�̔j��
	SAFE_DELETE(m_pInstance);
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�U�[�ύX�����������
	m_bUserChange = false;
}

//==========================================================================
// �v���C���[�o�^����
//==========================================================================
int CPlayerManager::RegistPlayer(CPlayer* pPlayer, const int nPosIdx)
{
	switch (pPlayer->GetAreaType())
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::EFieldArea::FIELD_IN:

		// ���X�g�}�l�[�W���[�ɓo�^
		return RegistInPlayer(pPlayer);

	case CPlayer::EFieldArea::FIELD_OUT:

		// �z��ɓo�^
		if (nPosIdx <= -1)	{ return RegistOutPlayer(pPlayer); }			// �|�W�V�����w�肪�Ȃ��ꍇ�󂫔z��ɓo�^
		else				{ return RegistOutPlayer(pPlayer, nPosIdx); }	// �|�W�V�����w�肪����ꍇ�w��z��ɓo�^

	default:
		assert(false);
		break;
	}

	return -1;
}

//==========================================================================
// �v���C���[�폜����
//==========================================================================
void CPlayerManager::DeletePlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetAreaType())
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::EFieldArea::FIELD_IN:

		// ���X�g�}�l�[�W���[����폜
		DeleteInPlayer(pPlayer);
		break;

	case CPlayer::EFieldArea::FIELD_OUT:

		// �z�񂩂�폜
		DeleteOutPlayer(pPlayer);
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// ����v���C���[���X�g�̎擾����
//==========================================================================
CListManager<CPlayer> CPlayerManager::GetInList(const CGameManager::ETeamSide team)
{
	switch (team)
	{ // �`�[���T�C�h���Ƃ̏���
	case CGameManager::ETeamSide::SIDE_LEFT:
		return m_listInLeft;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		return m_listInRight;

	default:
		assert(false);
		break;
	}

	assert(false);
	return {};
}

//==========================================================================
// �O��v���C���[�̎擾����
//==========================================================================
CPlayer* CPlayerManager::GetOutPlayer(const EOutPos out)
{
	// �O��v���C���[����Ԃ�
	return m_apOut[out];
}

//==========================================================================
// �O��|�W�V�����̎擾����
//==========================================================================
CPlayerManager::EOutPos CPlayerManager::GetOutPosition(const CPlayer* pPlayer)
{
	for (int i = 0; i < EOutPos::OUT_MAX; i++)
	{ // �O��̑������J��Ԃ�

		// �����v���C���[�Ɠ����A�h���X�̏ꍇ���̃|�W�V������Ԃ�
		if (m_apOut[i] == pPlayer) { return (EOutPos)i; }
	}

	return (EOutPos)-1;
}

//==========================================================================
// �O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetOutInfo(const EOutPos out)
{
	// �O����̎擾
	return (this->*(m_InfoFuncList[out]))();
}

//==========================================================================
// ���[�U�[�x�[�X��AI�ύX
//--------------------------------------------------------------------------
// �� �ǂ̃|�W�V�������ύX�\
//==========================================================================
void CPlayerManager::ChangeAIToUser(CPlayer* pPlayer)
{
	// ���g�����[�U�[�̏ꍇ�͔�����
	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) { return; }

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	CPlayer* pChange = nullptr;	// �ύX���郆�[�U�[�v���C���[
	float fMinDis = 0.0f;		// ���[�U�[�Ƃ̍ŏ�����

	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itr);	// �v���C���[���
		
		// ���g�ƕʃ`�[���̃v���C���[�̏ꍇ����
		if (pItrPlayer->GetTeam() != pPlayer->GetTeam()) { continue; }

		// ���[�U�[�ł͂Ȃ��ꍇ����
		if (pItrPlayer->GetBaseType() != CPlayer::EBaseType::TYPE_USER) { continue; }

		// ���[�U�[�Ƃ̋������v�Z
		float fCurDis = pItrPlayer->GetPosition().DistanceXZ(pPlayer->GetPosition());
		if (pChange == nullptr || fCurDis < fMinDis)
		{ // �܂����[�U�[���ݒ肳��Ă��Ȃ��A�܂��̓��[�U�[�Ԃ̋��������Z�������ꍇ

			// �ύX���[�U�[��ۑ�
			pChange = pItrPlayer;

			// ���Z�����[�U�[�Ԃ̋�����ۑ�
			fMinDis = fCurDis;
		}
	}

	if (pChange != nullptr)
	{ // �ύX�\�ȃ��[�U�[�������ꍇ

		// ���̃��[�U�[�Ǝ��g�̃x�[�X������
		assert(pChange->GetBaseType() == CPlayer::EBaseType::TYPE_USER);
		pChange->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_AI);	// ���̃��[�U�[��AI�ɕύX
		pPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_USER);	// ���g�����[�U�[�x�[�X�ɕύX

		// ���̃��[�U�[�̑��쌠�C���f�b�N�X��ۑ�
		int nOldUserIdx = pChange->GetMyPlayerIdx();
		assert(nOldUserIdx != -1);

		// ���̃��[�U�[�Ǝ��g�̑��쌠������
		pChange->SetMyPlayerIdx(pPlayer->GetMyPlayerIdx());	// ���̃��[�U�[�̑��쌠�����g�̑��쌠�ɕύX
		pPlayer->SetMyPlayerIdx(nOldUserIdx);				// ���g�̑��쌠�����̃��[�U�[�̑��쌠�ɕύX
	}
}

//==========================================================================
// AI�x�[�X�̃��[�U�[�ύX
//--------------------------------------------------------------------------
// �� �O��̓��[�U�[�ɕύX�s��
//==========================================================================
void CPlayerManager::ChangeUserToAI(CPlayer* pPlayer)
{
	// ���g��AI�̏ꍇ�͔�����
	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_AI) { return; }

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	CPlayer* pChange = nullptr;	// �ύX����AI�v���C���[
	float fMinDis = 0.0f;		// AI�Ƃ̍ŏ�����

	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pItrPlayer = (*itr);	// �v���C���[���
		
		// ���g�ƕʃ`�[���̃v���C���[�̏ꍇ����
		if (pItrPlayer->GetTeam() != pPlayer->GetTeam()) { continue; }

		// ����ł͂Ȃ��ꍇ����
		if (pItrPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN) { continue; }

		// AI�ł͂Ȃ��ꍇ����
		if (pItrPlayer->GetBaseType() != CPlayer::EBaseType::TYPE_AI) { continue; }

		// AI�Ƃ̋������v�Z
		float fCurDis = pItrPlayer->GetPosition().DistanceXZ(pPlayer->GetPosition());
		if (pChange == nullptr || fCurDis < fMinDis)
		{ // �܂�AI���ݒ肳��Ă��Ȃ��A�܂���AI�Ԃ̋��������Z�������ꍇ

			// �ύXAI��ۑ�
			pChange = pItrPlayer;

			// ���Z��AI�Ԃ̋�����ۑ�
			fMinDis = fCurDis;
		}
	}

	if (pChange != nullptr)
	{ // �ύX�\��AI�������ꍇ

		// ����AI�Ǝ��g�̃x�[�X������
		assert(pChange->GetBaseType() == CPlayer::EBaseType::TYPE_AI);
		pChange->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_USER);	// ����AI�����[�U�[�ɕύX
		pPlayer->GetBase()->SetNewBase(CPlayer::EBaseType::TYPE_AI);	// ���g��AI�x�[�X�ɕύX

		// ����AI�̑��쌠�C���f�b�N�X��ۑ�
		int nOldUserIdx = pChange->GetMyPlayerIdx();
		assert(nOldUserIdx == -1);

		// ����AI�Ǝ��g�̑��쌠������
		pChange->SetMyPlayerIdx(pPlayer->GetMyPlayerIdx());	// ����AI�̑��쌠�����g�̑��쌠�ɕύX
		pPlayer->SetMyPlayerIdx(nOldUserIdx);				// ���g�̑��쌠������AI�̑��쌠�ɕύX
	}
}

//==========================================================================
// �x�[�X���֏���
//==========================================================================
void CPlayerManager::SwapBase(CPlayer* pPlayer, CPlayer* pTarget)
{
	// ��l�̃v���C���[�̃x�[�X�������ꍇ�͔�����
	if (pPlayer->GetBaseType() == pTarget->GetBaseType()) { return; }

	// �^�[�Q�b�g�̃x�[�X��ۑ�
	CPlayer::EBaseType oldBase = pTarget->GetBaseType();

	// �^�[�Q�b�g�Ǝ��g�̃x�[�X������
	pTarget->GetBase()->SetNewBase(pPlayer->GetBaseType());
	pPlayer->GetBase()->SetNewBase(oldBase);

	// �^�[�Q�b�g�̑��쌠�C���f�b�N�X��ۑ�
	int nOldUserIdx = pTarget->GetMyPlayerIdx();

	// �^�[�Q�b�g�Ǝ��g�̑��쌠������
	pTarget->SetMyPlayerIdx(pPlayer->GetMyPlayerIdx());
	pPlayer->SetMyPlayerIdx(nOldUserIdx);
}

//==========================================================================
// �O��v���C���[�o�^���� (�|�W�V�����w��)
//==========================================================================
int CPlayerManager::RegistOutPlayer(CPlayer* pPlayer, const int nPosIdx)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();		// �`�[��
	int nHalfMax = EOutPos::OUT_MAX / 2;					// �`�[�����Ƃ̊O�쑍��
	int nStart = nHalfMax * std::abs(team - 1);				// �z��擪
	int nEnd = nHalfMax + (nHalfMax * std::abs(team - 1));	// �z��Ō��

	// �|�W�V�����w��C���f�b�N�X���͈͊O�̏ꍇ�G���[
	if (nPosIdx <= -1 || nPosIdx >= EOutPos::OUT_MAX) { assert(false); return -1; }

	// �w��̃|�W�V�����ɊO�삪�o�^�ς݂̏ꍇ�G���[
	if (m_apOut[nPosIdx] != nullptr) { assert(false); return -1; }

	// �w��̃|�W�V�������ʃ`�[���������ꍇ�G���[
	if ((int)team != ((nPosIdx / nHalfMax) - 1) * -1) { assert(false); return -1; }

	// �O���o�^
	m_apOut[nPosIdx] = pPlayer;

	// ���݃C���f�b�N�X�̊O������擾
	SOutInfo info = GetOutInfo((EOutPos)nPosIdx);

	// ���E�ʒu��ݒ�
	CPlayerOut* pBase = pPlayer->GetBase()->GetPlayerOut();	// �O��v���C���[���
	pBase->SetPosLeft(info.posLeft);
	pBase->SetPosRight(info.posRight);

	if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER)
	{ // �x�[�X�����[�U�[�̏ꍇ

		// ���E����̊���
		CPlayerUserOut* pBaseUser = pPlayer->GetBase()->GetPlayerUserOut();	// ���[�U�[�O��v���C���[���
		pBaseUser->BindLeftKey(info.pKeyLeft);
		pBaseUser->BindRightKey(info.pKeyRight);
	}
	else
	{ // �x�[�X�����[�U�[�ł͂Ȃ��ꍇ

		// ���E����̔j��
		SAFE_DELETE(info.pKeyLeft);
		SAFE_DELETE(info.pKeyRight);
	}

	return nPosIdx;
}

//==========================================================================
// �O��v���C���[�o�^����
//==========================================================================
int CPlayerManager::RegistOutPlayer(CPlayer* pPlayer)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();		// �`�[��
	int nHalfMax = EOutPos::OUT_MAX / 2;					// �`�[�����Ƃ̊O�쑍��
	int nStart = nHalfMax * std::abs(team - 1);				// �z��擪
	int nEnd = nHalfMax + (nHalfMax * std::abs(team - 1));	// �z��Ō��

	for (int i = nStart; i < nEnd; i++)
	{ // ���`�[���̊O��|�W�V�����̑������J��Ԃ�

		if (m_apOut[i] == nullptr)
		{ // �O�삪���ݒ�̏ꍇ

			// �O���o�^
			m_apOut[i] = pPlayer;

			// ���݃C���f�b�N�X�̊O������擾
			SOutInfo info = GetOutInfo((EOutPos)i);

			// ���E�ʒu��ݒ�
			CPlayerOut* pBase = pPlayer->GetBase()->GetPlayerOut();	// �O��v���C���[���
			pBase->SetPosLeft(info.posLeft);
			pBase->SetPosRight(info.posRight);

			if (pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER)
			{ // �x�[�X�����[�U�[�̏ꍇ

				// ���E����̊���
				CPlayerUserOut* pBaseUser = pPlayer->GetBase()->GetPlayerUserOut();	// ���[�U�[�O��v���C���[���
				pBaseUser->BindLeftKey(info.pKeyLeft);
				pBaseUser->BindRightKey(info.pKeyRight);
			}
			else
			{ // �x�[�X�����[�U�[�ł͂Ȃ��ꍇ

				// ���E����̔j��
				SAFE_DELETE(info.pKeyLeft);
				SAFE_DELETE(info.pKeyRight);
			}

			return i;
		}
	}

	return -1;
}

//==========================================================================
// �O��v���C���[�폜����
//==========================================================================
void CPlayerManager::DeleteOutPlayer(CPlayer* pPlayer)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();		// �`�[��
	int nHalfMax = EOutPos::OUT_MAX / 2;					// �`�[�����Ƃ̊O�쑍��
	int nStart = nHalfMax * std::abs(team - 1);				// �z��擪
	int nEnd = nHalfMax + (nHalfMax * std::abs(team - 1));	// �z��Ō��

	for (int i = nStart; i < nEnd; i++)
	{ // ���`�[���̊O��|�W�V�����̑������J��Ԃ�

		if (m_apOut[i] == pPlayer)
		{ // �O�삪�ݒ�ς݂̏ꍇ

			// �O����폜��������
			m_apOut[i] = nullptr;
			return;
		}
	}
}

//==========================================================================
// ����v���C���[�o�^����
//==========================================================================
int CPlayerManager::RegistInPlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // �`�[���T�C�h���Ƃ̏���
	case CGameManager::ETeamSide::SIDE_LEFT:

		// ���썶�v���C���[���X�g�ɓo�^
		m_listInLeft.Regist(pPlayer);
		return m_listInLeft.GetNumAll() - 1;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// ����E�v���C���[���X�g�ɓo�^
		m_listInRight.Regist(pPlayer);
		return m_listInRight.GetNumAll() - 1;

	default:
		assert(false);
		break;
	}

	return -1;
}

//==========================================================================
// ����v���C���[�폜����
//==========================================================================
void CPlayerManager::DeleteInPlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetTeam())
	{ // �`�[���T�C�h���Ƃ̏���
	case CGameManager::ETeamSide::SIDE_LEFT:

		// ���썶�v���C���[���X�g����폜
		m_listInLeft.Delete(pPlayer);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// ����E�v���C���[���X�g����폜
		m_listInRight.Delete(pPlayer);
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// �����̊O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeftFar()
{
	SOutInfo info;	// �O����
	info.posLeft	= MyLib::Vector3(-900.0f, 0.0f, 650.0f);	// �ړ��\���ʒu
	info.posRight	= MyLib::Vector3(-50.0f, 0.0f, 650.0f);		// �ړ��\�E�ʒu
	info.pKeyLeft	= DEBUG_NEW CBindKeyLeft;	// ���ړ��L�[
	info.pKeyRight	= DEBUG_NEW CBindKeyRight;	// �E�ړ��L�[
	return info;
}

//==========================================================================
// ���̊O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeft()
{
	SOutInfo info;	// �O����
	info.posLeft	= MyLib::Vector3(-1040.0f, 0.0f, -650.0f);	// �ړ��\���ʒu
	info.posRight	= MyLib::Vector3(-1040.0f, 0.0f, 650.0f);	// �ړ��\�E�ʒu
	info.pKeyLeft	= DEBUG_NEW CBindKeyBack;	// ���ړ��L�[
	info.pKeyRight	= DEBUG_NEW CBindKeyFront;	// �E�ړ��L�[
	return info;
}

//==========================================================================
// ����O�̊O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoLeftNear()
{
	SOutInfo info;	// �O����
	info.posLeft	= MyLib::Vector3(-50.0f, 0.0f, -650.0f);	// �ړ��\���ʒu
	info.posRight	= MyLib::Vector3(-900.0f, 0.0f, -650.0f);	// �ړ��\�E�ʒu
	info.pKeyLeft	= DEBUG_NEW CBindKeyRight;	// ���ړ��L�[
	info.pKeyRight	= DEBUG_NEW CBindKeyLeft;	// �E�ړ��L�[
	return info;
}

//==========================================================================
// �E���̊O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoRightFar()
{
	SOutInfo info;	// �O����
	info.posLeft	= MyLib::Vector3(50.0f, 0.0f, 650.0f);		// �ړ��\���ʒu
	info.posRight	= MyLib::Vector3(900.0f, 0.0f, 650.0f);		// �ړ��\�E�ʒu
	info.pKeyLeft	= DEBUG_NEW CBindKeyLeft;	// ���ړ��L�[
	info.pKeyRight	= DEBUG_NEW CBindKeyRight;	// �E�ړ��L�[
	return info;
}

//==========================================================================
// �E�̊O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoRight()
{
	SOutInfo info;	// �O����
	info.posLeft	= MyLib::Vector3(1040.0f, 0.0f, 650.0f);	// �ړ��\���ʒu
	info.posRight	= MyLib::Vector3(1040.0f, 0.0f, -650.0f);	// �ړ��\�E�ʒu
	info.pKeyLeft	= DEBUG_NEW CBindKeyFront;	// ���ړ��L�[
	info.pKeyRight	= DEBUG_NEW CBindKeyBack;	// �E�ړ��L�[
	return info;
}

//==========================================================================
// �E��O�̊O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetInfoRightNear()
{
	SOutInfo info;	// �O����
	info.posLeft	= MyLib::Vector3(900.0f, 0.0f, -650.0f);	// �ړ��\���ʒu
	info.posRight	= MyLib::Vector3(50.0f, 0.0f, -650.0f);		// �ړ��\�E�ʒu
	info.pKeyLeft	= DEBUG_NEW CBindKeyRight;	// ���ړ��L�[
	info.pKeyRight	= DEBUG_NEW CBindKeyLeft;	// �E�ړ��L�[
	return info;
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CPlayerManager::Save(const std::vector<LoadInfo>& LeftInfo, const std::vector<LoadInfo>& RightInfo)
{
	// �t�@�C�����J��
	std::ofstream File(PlayerManager::TEXT_PLAYERINFO);
	if (!File.is_open()) return;

	// �������n��
	m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT] = LeftInfo;
	m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT] = RightInfo;

	// �e�L�X�g�t�@�C�����ڎ�
	File << TOP_LINE << std::endl;
	File << "# �`�[�����" << std::endl;
	File << TOP_LINE << std::endl;

	//--------------------------
	// ���`�[��
	//--------------------------
	File << TEXT_LINE << std::endl;
	File << "# �`�[��" << CGameManager::ETeamSide::SIDE_LEFT << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETTEAM" << std::endl;

	// �v���C���[���Z�[�u
	SavePlayerInfo(&File, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_LEFT]);
	File << "END_SETTEAM" << std::endl;

	//--------------------------
	// �E�`�[��
	//--------------------------
	File << TEXT_LINE << std::endl;
	File << "# �`�[��" << CGameManager::ETeamSide::SIDE_RIGHT << std::endl;
	File << TEXT_LINE << std::endl;
	File << "SETTEAM" << std::endl;

	// �v���C���[���Z�[�u
	SavePlayerInfo(&File, m_vecLoadInfo[CGameManager::ETeamSide::SIDE_RIGHT]);

	File << "END_SETTEAM" << std::endl;

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �v���C���[���Z�[�u
//==========================================================================
void CPlayerManager::SavePlayerInfo(std::ofstream* File, const std::vector<LoadInfo>& Info)
{
	for (const auto& info : Info)
	{
		(*File) << "\tSETPLAYER" << std::endl;

		(*File) << "\t\tID_CONTROLL = " << info.nControllIdx << std::endl;	// ���삷��C���f�b�N�X�ԍ�
		(*File) << "\t\tHAIR = " << info.nHair << std::endl;				// �������ւ�
		(*File) << "\t\tACCESSORY = " << info.nAccessory << std::endl;		// �A�N�Z�����ւ�
		(*File) << "\t\tFACE = " << info.nFace << std::endl;				// �璅���ւ�
		(*File) << "\t\tBODY = " << info.eBody << std::endl;				// �̌^
		(*File) << "\t\tHANDED = " << info.eHanded << std::endl;			// ������

		(*File) << "\tEND_SETPLAYER" << std::endl;
		(*File) << std::endl;
	}
}

//==========================================================================
// �ǂݍ���
//==========================================================================
void CPlayerManager::Load()
{
	// �t�@�C�����J��
	std::ifstream File(PlayerManager::TEXT_PLAYERINFO);
	if (!File.is_open()) return;

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;

	// �`�[���̍ő吔
	int nTeam = 0;

	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("SETTEAM") != std::string::npos)
		{// �`�[�����

			// �ǂݍ��ݏ��N���A
			m_vecLoadInfo[nTeam].clear();

			int nPlayerNum = 0;	// �v���C���[��

			while (line.find("END_SETTEAM") == std::string::npos)
			{// END_SETTEAM������܂ŌJ��Ԃ�

				// �m�F����
				std::getline(File, line);

				if (line.find("SETPLAYER") != std::string::npos)
				{// �v���C���[���ǂݍ���

					// �v���C���[���ǂݍ���
					LoadPlayerInfo(&File, nTeam, nPlayerNum);

					// �v���C���[�����Z
					nPlayerNum++;
				}
			}

			// �`�[�������Z
			nTeam++;
		}

		if (nTeam >= CGameManager::ETeamSide::SIDE_MAX)
		{// �`�[���̍ő吔�܂œǂݍ���ł�����I��
			break;
		}
	}
}

//==========================================================================
// �v���C���[���ǂݍ���
//==========================================================================
void CPlayerManager::LoadPlayerInfo(std::ifstream* File, int nTeam, int nIdxPlayer)
{
	// �R�����g�p
	std::string hoge;

	// �`�[��
	CGameManager::ETeamSide team = static_cast<CGameManager::ETeamSide>(nTeam);

	// �ǂݍ��ݗp�ϐ�
	LoadInfo info;	// �ǂݍ��ݏ��

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(*File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("END_SETPLAYER") != std::string::npos)
		{// �I��
			break;
		}

		if (line.find("ID_CONTROLL") != std::string::npos)
		{// ���삷��C���f�b�N�X�ԍ�

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>			// ��
				info.nControllIdx;	// ���삷��C���f�b�N�X�ԍ�
		}

		if (line.find("HAIR") != std::string::npos)
		{// ��

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				info.nHair;		// ��
		}

		if (line.find("ACCESSORY") != std::string::npos)
		{// �A�N�Z�T���[

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				info.nAccessory;		// �A�N�Z�T���[
		}

		if (line.find("FACE") != std::string::npos)
		{// ��

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				info.nFace;		// ��
		}

		if (line.find("BODY") != std::string::npos)
		{// �̌^

			int body = -1;

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				body;		// �̌^

			info.eBody = static_cast<CPlayer::EBody>(body);
		}

		if (line.find("HANDED") != std::string::npos)
		{// ������

			int hand = -1;

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				hand;		// ������

			info.eHanded = static_cast<CPlayer::EHandedness>(hand);
		}
	}

	// �ǂݍ��񂾏����i�[
	m_vecLoadInfo[nTeam].push_back(info);
}
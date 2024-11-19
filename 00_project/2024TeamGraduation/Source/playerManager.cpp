//==========================================================================
// 
//  �v���C���[�}�l�[�W���[���� [playerManager.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerManager.h"
#include "player.h"
#include "playerStatus.h"
#include "bindKeyFront.h"
#include "bindKeyBack.h"
#include "bindKeyLeft.h"
#include "bindKeyRight.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

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

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerManager::CPlayerManager()
{

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
CPlayerManager* CPlayerManager::Create()
{
	// ���ɐ����ς݂̏ꍇ������
	if (m_pInstance != nullptr) { return m_pInstance; }

	// �C���X�^���X�̐���
	m_pInstance = DEBUG_NEW CPlayerManager;
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

}

//==========================================================================
// �v���C���[�o�^����
//==========================================================================
int CPlayerManager::RegistPlayer(CPlayer* pPlayer)
{
	switch (pPlayer->GetAreaType())
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::EFieldArea::FIELD_IN:

		// ���X�g�}�l�[�W���[�ɓo�^
		m_listIn.Regist(pPlayer);
		return m_listIn.GetNumAll() - 1;

	case CPlayer::EFieldArea::FIELD_OUT:

		// �z��ɓo�^
		return RegistOutPlayer(pPlayer);

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
		m_listIn.Delete(pPlayer);
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
// �O����̎擾����
//==========================================================================
CPlayerManager::SOutInfo CPlayerManager::GetOutInfo(const EOutPos out)
{
	// �O����̎擾
	return (this->*(m_InfoFuncList[out]))();
}

//==========================================================================
// �O��v���C���[�o�^����
//==========================================================================
int CPlayerManager::RegistOutPlayer(CPlayer* pPlayer)
{
	CGameManager::ETeamSide team = pPlayer->GetTeam();	// �`�[��
	int nHalfMax = (int)(EOutPos::OUT_MAX * 0.5f);		// �`�[�����Ƃ̊O�쑍��
	int nStart = nHalfMax * (int)team;					// �z��擪
	int nEnd = nHalfMax + (nHalfMax * (int)team);		// �z��Ō��

	for (int i = nStart; i < nEnd; i++)
	{ // ���`�[���̊O��|�W�V�����̑������J��Ԃ�

		if (m_apOut[i] == nullptr)
		{ // �O�삪���ݒ�̏ꍇ

			// �O���o�^��������
			m_apOut[i] = pPlayer;
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
	CGameManager::ETeamSide team = pPlayer->GetTeam();	// �`�[��
	int nHalfMax = (int)(EOutPos::OUT_MAX * 0.5f);		// �`�[�����Ƃ̊O�쑍��
	int nStart = nHalfMax * (int)team;					// �z��擪
	int nEnd = nHalfMax + (nHalfMax * (int)team);		// �z��Ō��

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

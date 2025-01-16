//==========================================================================
// 
//  �v���C���[�}�l�[�W���[���� [playerManager_spawn.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "playerManager_spawn.h"
#include "resultmanager.h"
#include "playerResult.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float	ZLINE_OFFSET = 200.0f;	// Z���I�t�Z�b�g
	const MyLib::Vector3 POS_LEFT = MyLib::Vector3(-972.85f, 0.0f, 1817.35f);	// ���擪�v���C���[�����ʒu
	const MyLib::Vector3 POS_RIGHT = MyLib::Vector3(972.85f, 0.0f, 1817.35f);	// �E�擪�v���C���[�����ʒu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerManager_Spawn::CPlayerManager_Spawn() : CPlayerManager()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerManager_Spawn::~CPlayerManager_Spawn()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerManager_Spawn::Init()
{
	// �e�N���X�̏���������
	CPlayerManager::Init();

	return S_OK;
}

//==========================================================================
// ���̃v���C���[����
//==========================================================================
HRESULT CPlayerManager_Spawn::CreateLeftPlayer(int i, const LoadInfo& info)
{
	
	// �v���C���[����
	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET * (float)i);
	CPlayer* pPlayer = CPlayer::Create
	(
		POS_LEFT + offset, 				// �ʒu
		CGameManager::SIDE_LEFT,		// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_SPAWN,	// �l
		info.eBody,		// �̌n
		info.eHanded	// ������
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	// �C���f�b�N�X���f
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// �h���X�A�b�v���f
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	// �擪�����h�A�J��
	if (i == 0)
	{
		pPlayer->SetMotion(CPlayer::EMotion::MOTION_DOOR);
	}

	return S_OK;
}

//==========================================================================
// �E�̃v���C���[����
//==========================================================================
HRESULT CPlayerManager_Spawn::CreateRightPlayer(int i, const LoadInfo& info)
{
	// �v���C���[����
	MyLib::Vector3 offset = MyLib::Vector3(0.0f, 0.0f, ZLINE_OFFSET * (float)i);
	CPlayer* pPlayer = CPlayer::Create
	(
		POS_RIGHT + offset, 			// �ʒu
		CGameManager::SIDE_RIGHT,		// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_SPAWN,	// �l
		info.eBody,		// �̌n
		info.eHanded	// ������
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	// �C���f�b�N�X���f
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// �h���X�A�b�v���f
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	// �擪�����h�A�J��
	if (i == 0)
	{
		pPlayer->SetMotion(CPlayer::EMotion::MOTION_DOOR);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerManager_Spawn::Uninit()
{
	// ���g�̃C���X�^���X�̔j��
	SAFE_DELETE(m_pInstance);
}

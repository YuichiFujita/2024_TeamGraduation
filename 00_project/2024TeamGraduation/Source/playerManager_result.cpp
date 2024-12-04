//==========================================================================
// 
//  �v���C���[�}�l�[�W���[���� [playerManager_result.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "playerManager_result.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

// �v���C���[
namespace Player
{
	const float INIT_IN_X(100.0f);	// �����ʒu(����)
	const float INIT_IN_Z(100.0f);	// �����ʒu(����)
	const float INIT_OUT_X(100.0f);	// �����ʒu(�O��)
	const float INIT_OUT_Z(300.0f);	// �����ʒu(�O��)
	const float SHIFT(50.0f);	// ���炵�l

	const MyLib::Vector3 POS_IN[CGameManager::ETeamSide::SIDE_MAX] =
	{// ����ʒu
		MyLib::Vector3(-INIT_IN_X, 0.0f, +INIT_IN_Z),
		MyLib::Vector3(+INIT_IN_X, 0.0f, +INIT_IN_Z),
	};

	const MyLib::Vector3 POS_OUT[CGameManager::ETeamSide::SIDE_MAX] =
	{// �O��ʒu
		MyLib::Vector3(-INIT_OUT_X, 0.0f, +INIT_OUT_Z),
		MyLib::Vector3(+INIT_OUT_X, 0.0f, +INIT_OUT_Z),
	};

	const MyLib::Vector3 POS_SHIFT[CGameManager::ETeamSide::SIDE_MAX] =
	{// ���炵�l
		MyLib::Vector3(-SHIFT, 0.0f, -SHIFT),
		MyLib::Vector3(-SHIFT, 0.0f, -SHIFT),
	};
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerManager_Result::CPlayerManager_Result() : CPlayerManager()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerManager_Result::~CPlayerManager_Result()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerManager_Result::Init()
{
#ifdef ENTRYSTART
	// ����������
	CPlayerManager::Init();
#else

#endif

	return S_OK;
}

//==========================================================================
// ���̃v���C���[����
//==========================================================================
HRESULT CPlayerManager_Result::CreateLeftPlayer(int i, const LoadInfo& info)
{
	// �ʒu
	MyLib::Vector3 pos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_LEFT];
	pos += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_LEFT] * i);

	// �v���C���[����
	CPlayer* pPlayer = CPlayer::Create(
		pos,
		CGameManager::ETeamSide::SIDE_LEFT,
		CPlayer::EHuman::HUMAN_RESULT,
		info.eBody,
		info.eHanded);

	// �C���f�b�N�X���f
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// �h���X�A�b�v���f
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// �E�̃v���C���[����
//==========================================================================
HRESULT CPlayerManager_Result::CreateRightPlayer(int i, const LoadInfo& info)
{
	// �ʒu
	MyLib::Vector3 pos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_RIGHT];
	pos += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_RIGHT] * i);

	// �v���C���[����
	CPlayer* pPlayer = CPlayer::Create(
		pos,
		CGameManager::ETeamSide::SIDE_RIGHT,
		CPlayer::EHuman::HUMAN_RESULT,
		info.eBody,
		info.eHanded);

	// �C���f�b�N�X���f
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// �h���X�A�b�v���f
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerManager_Result::Uninit()
{
	// ���g�̃C���X�^���X�̔j��
	SAFE_DELETE(m_pInstance);
}

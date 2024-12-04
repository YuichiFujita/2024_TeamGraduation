//==========================================================================
// 
//  �v���C���[�}�l�[�W���[���� [playerManager_game.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "playerManager_game.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerManager_Game::CPlayerManager_Game() : CPlayerManager()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerManager_Game::~CPlayerManager_Game()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerManager_Game::Init()
{
	// ����������
	CPlayerManager::Init();

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

	// �R������
	CPlayer::Create
	(
		MyLib::Vector3(0.0f, 0.0f, CGameManager::GetInstance()->GetCourtSize().z),			// �ʒu
		CGameManager::ETeamSide::SIDE_NONE,			// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_REFEREE,				// �l
		CPlayer::EBody::BODY_NORMAL,				// �̌^
		CPlayer::EHandedness::HAND_R				// ������
	);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerManager_Game::Uninit()
{
	// ���g�̃C���X�^���X�̔j��
	SAFE_DELETE(m_pInstance);
}

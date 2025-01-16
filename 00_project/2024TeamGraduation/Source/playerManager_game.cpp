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
	namespace player
	{
		namespace left
		{
			const MyLib::Vector3 POS = MyLib::Vector3(-300.0f, 0.0f, 0.0f);	// �����S�ʒu
			const float OFFSET_Z = 300.0f;	// Z���W�I�t�Z�b�g
		}

		namespace right
		{
			const MyLib::Vector3 POS = MyLib::Vector3(300.0f, 0.0f, 0.0f);	// �E���S�ʒu
			const float OFFSET_Z = 300.0f;	// Z���W�I�t�Z�b�g
		}
	}
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

	// �R������
	CPlayer::Create
	(
		MyLib::Vector3(0.0f, 0.0f, CGameManager::GetInstance()->GetCourtSize().z),			// �ʒu
		CGameManager::ETeamSide::SIDE_NONE,			// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_REFEREE,				// �l
		CPlayer::EBody::BODY_NORMAL,				// �̌^
		CPlayer::EHandedness::HAND_R				// ������
	);

#if 0
	for (int i = 0; i < 10; i++)
	{
		CPlayer* pAI = CPlayer::Create(MyLib::Vector3(200.0f, 0.0f, 0.0f), CGameManager::SIDE_RIGHT, CPlayer::EFieldArea::FIELD_IN, CPlayer::EBaseType::TYPE_AI);
		if (pAI == nullptr)
		{
			return E_FAIL;
		}
		pAI->SetRotation(MyLib::Vector3(0.0f, HALF_PI, 0.0f));
		pAI->SetRotDest(HALF_PI);
	}
#endif
	return S_OK;
}

//==========================================================================
// �v���C���[����
//==========================================================================
HRESULT CPlayerManager_Game::CreatePlayer()
{
	// �ǂݍ��ݏ��
	std::vector<LoadInfo> leftLoadInfo = CPlayerManager::GetLoadInfo(CGameManager::ETeamSide::SIDE_LEFT);
	std::vector<LoadInfo> rightLoadInfo = CPlayerManager::GetLoadInfo(CGameManager::ETeamSide::SIDE_RIGHT);

	// �ǂݍ��񂾏������ƂɃv���C���[����
	int nMaxLeft = static_cast<int>(leftLoadInfo.size());		// ���`�[���l��
	int nMaxRight = static_cast<int>(rightLoadInfo.size());	// �E�`�[���l��
	int nCntLeft = 0, nCntRight = 0;	// �l���J�E���g
	for (int j = 0; j < CGameManager::MAX_SIDEPLAYER; j++)
	{
		// ���̃v���C���[����
		if (j < nMaxLeft)
		{
			MyLib::Vector3 pos = player::left::POS;	// �����ʒu

			// Z���W�𓮂���
			pos.z = player::left::POS.z - (player::left::OFFSET_Z * (float)(nMaxLeft - 1)) * 0.5f + (player::left::OFFSET_Z * (float)nCntLeft);

			// ���`�[���v���C���[����
			CreateLeftPlayer(nCntLeft, leftLoadInfo[j], pos);

			// ���l�����Z
			nCntLeft++;
		}

		// �E�̃v���C���[����
		if (j < nMaxRight)
		{
			MyLib::Vector3 pos = player::right::POS;	// �����ʒu

			// Z���W�𓮂���
			pos.z = player::right::POS.z - (player::right::OFFSET_Z * (float)(nMaxRight - 1)) * 0.5f + (player::right::OFFSET_Z * (float)nCntRight);

			// �E�`�[���v���C���[����
			CreateRightPlayer(nCntRight, rightLoadInfo[j], pos);

			// �E�l�����Z
			nCntRight++;
		}
	}

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

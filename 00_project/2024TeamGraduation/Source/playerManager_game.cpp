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
		MyLib::Vector3(0.0f, 0.0f, CGameManager::GetInstance()->GetCourtSize().z),	// �ʒu
		CGameManager::ETeamSide::SIDE_NONE,	// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_REFEREE,		// �l
		CPlayer::EBody::BODY_NORMAL,		// �̌^
		CPlayer::EHandedness::HAND_R		// ������
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
	//----------------------------------------------------------------------
	// �v���C���[���쐶��
	//----------------------------------------------------------------------
	std::vector<LoadInfo> vecInLeftInfo		= GetLoadInInfo(CGameManager::ETeamSide::SIDE_LEFT);	// ��������
	std::vector<LoadInfo> vecInRightInfo	= GetLoadInInfo(CGameManager::ETeamSide::SIDE_RIGHT);	// �E������

	// �ǂݍ��񂾏������ƂɃv���C���[����
	int nInMaxLeft = static_cast<int>(vecInLeftInfo.size());	// ���`�[���l��
	int nInMaxRight = static_cast<int>(vecInRightInfo.size());	// �E�`�[���l��
	int nCntLeft = 0, nCntRight = 0;	// �l���J�E���g
	for (int j = 0; j < CGameManager::MAX_SIDEPLAYER; j++)
	{
		if (j < nInMaxLeft)
		{
			MyLib::Vector3 pos = player::left::POS;	// �����ʒu

			// Z���W�𓮂���
			pos.z = player::left::POS.z - (player::left::OFFSET_Z * (float)(nInMaxLeft - 1)) * 0.5f + (player::left::OFFSET_Z * (float)nCntLeft);

			// ���`�[���v���C���[����
			if (FAILED(CreateLeftPlayer(nCntLeft, vecInLeftInfo[j], pos)))
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// ���l�����Z
			nCntLeft++;
		}

		if (j < nInMaxRight)
		{
			MyLib::Vector3 pos = player::right::POS;	// �����ʒu

			// Z���W�𓮂���
			pos.z = player::right::POS.z - (player::right::OFFSET_Z * (float)(nInMaxRight - 1)) * 0.5f + (player::right::OFFSET_Z * (float)nCntRight);

			// �E�`�[���v���C���[����
			if (FAILED(CreateRightPlayer(nCntRight, vecInRightInfo[j], pos)))
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// �E�l�����Z
			nCntRight++;
		}
	}

	//----------------------------------------------------------------------
	// �v���C���[�O�쐶��
	//----------------------------------------------------------------------
	std::vector<LoadInfo> vecOutLeftInfo	= GetLoadOutInfo(CGameManager::ETeamSide::SIDE_LEFT);	// ���O����
	std::vector<LoadInfo> vecOutRightInfo	= GetLoadOutInfo(CGameManager::ETeamSide::SIDE_RIGHT);	// �E�O����

	// �v���C���[�O�쐶�� (���T�C�h)
	for (const auto& rOutLeft : vecOutLeftInfo)
	{ // ���O��l�����J��Ԃ�

		// ���`�[���O��v���C���[����
		if (FAILED(CreateOutPlayer(CGameManager::ETeamSide::SIDE_LEFT, rOutLeft)))
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}
	}

	// �v���C���[�O�쐶�� (�E�T�C�h)
	for (const auto& rOutRight : vecOutRightInfo)
	{ // �E�O��l�����J��Ԃ�

		// �E�`�[���O��v���C���[����
		if (FAILED(CreateOutPlayer(CGameManager::ETeamSide::SIDE_RIGHT, rOutRight)))
		{ // �����Ɏ��s�����ꍇ

			return E_FAIL;
		}
	}

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

//==========================================================================
// �O��v���C���[����
//==========================================================================
HRESULT CPlayerManager_Game::CreateOutPlayer(CGameManager::ETeamSide team, const LoadInfo& info)
{
	// �v���C���[����
	CPlayer* pPlayer = CPlayer::Create
	(
		MyLib::Vector3(), 				// �ʒu
		team,							// �`�[���T�C�h
		CPlayer::EFieldArea::FIELD_OUT,	// �|�W�V����
		CPlayer::EBaseType::TYPE_AI,	// �x�[�X�^�C�v
		info.eBody,						// �̌n
		info.eHanded					// ������
	);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	// �C���f�b�N�X���f
	pPlayer->SetMyPlayerIdx(info.nControllIdx);

	// �h���X�A�b�v���f
	pPlayer->BindDressUp(info.nHair, info.nAccessory, info.nFace);

	return S_OK;
}

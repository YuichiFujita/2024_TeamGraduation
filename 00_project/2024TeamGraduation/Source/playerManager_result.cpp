//==========================================================================
// 
//  �v���C���[�}�l�[�W���[���� [playerManager_result.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "playerManager_result.h"
#include "resultmanager.h"
#include "playerResult.h"

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
	// �e�N���X�̏���������
	CPlayerManager::Init();

	//�v���C���[������
	InitPlayer();

	return S_OK;
}

//==========================================================================
// ���̃v���C���[����
//==========================================================================
HRESULT CPlayerManager_Result::CreateLeftPlayer(int i, const LoadInfo& info)
{
	// �ʒu
	MyLib::Vector3 posCreate = CResultManager::GetInstance()->GetPosMid(CGameManager::ETeamSide::SIDE_LEFT);
	posCreate += Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT];
	posCreate += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_LEFT] * static_cast<float>(i));

	// �v���C���[����
	CPlayer* pPlayer = CPlayer::Create(
		posCreate,
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
	MyLib::Vector3 posCreate = CResultManager::GetInstance()->GetPosMid(CGameManager::ETeamSide::SIDE_RIGHT);
	posCreate += Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT];
	posCreate += (Player::POS_SHIFT[CGameManager::ETeamSide::SIDE_RIGHT] * static_cast<float>(i));

	// �v���C���[����
	CPlayer* pPlayer = CPlayer::Create(
		posCreate,
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
// �v���C���[������
//==========================================================================
void CPlayerManager_Result::InitPlayer()
{
	//----------------------------------------------------------------------
	// �v���C���[�O�쐶��
	//----------------------------------------------------------------------
	int nHalfMax = EOutPos::OUT_MAX / 2;	// �`�[�����Ƃ̊O�쑍��

	// �v���C���[�O�쐶�� (�E�T�C�h)
#if 1
	for (int i = 0; i < nHalfMax; i++)
	{ // �`�[�����Ƃ̊O��l�����J��Ԃ�

		// �E�`�[���̊O��v���C���[����
		CPlayer* pOutRight= CPlayer::Create
		(
			VEC3_ZERO,
			CGameManager::ETeamSide::SIDE_RIGHT,
			CPlayer::EHuman::HUMAN_RESULT,
			CPlayer::BODY_NORMAL,
			CPlayer::HAND_R,
			CPlayer::EFieldArea::FIELD_OUT
		);
		if (pOutRight == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
		}
	}

	// �v���C���[�O�쐶�� (���T�C�h)
	for (int i = 0; i < nHalfMax; i++)
	{ // �`�[�����Ƃ̊O��l�����J��Ԃ�

		// ���`�[���̊O��v���C���[����
		CPlayer* pOutLeft = CPlayer::Create
		(
			VEC3_ZERO,
			CGameManager::ETeamSide::SIDE_LEFT,
			CPlayer::EHuman::HUMAN_RESULT,
			CPlayer::BODY_NORMAL,
			CPlayer::HAND_R,
			CPlayer::EFieldArea::FIELD_OUT
		);
		if (pOutLeft == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
		}
	}
#endif

	// �ʒu
	MyLib::Vector3 pos = VEC3_ZERO;

	// �ʒu�ݒ�
	int nNumRight = 0, nNumLeft = 0;
	CListManager<CPlayerResult> sampleList = CPlayerResult::GetList();
	std::list<CPlayerResult*>::iterator itr = sampleList.GetEnd();
	CPlayerResult* pObj = nullptr;

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);
		// pObj��(*itr)���g���ď���
	
		// ����r��
		if (pObj->GetAreaType() == CPlayer::EFieldArea::FIELD_IN) continue;

		CGameManager::ETeamSide team = pObj->GetTeam();
		pos = CResultManager::GetInstance()->GetPosMid(team);
		pos += Player::POS_OUT[team];

		// �J�E���^�[
		switch (team)
		{
		case CGameManager::ETeamSide::SIDE_LEFT:

			pos += (Player::POS_SHIFT[team] * static_cast<float>(nNumLeft));
			nNumLeft++;
			break;
		
		case CGameManager::ETeamSide::SIDE_RIGHT:

			pos += (Player::POS_SHIFT[team] * static_cast<float>(nNumRight));
			nNumRight++;
			break;
	
		default:
			MyAssert::CustomAssert(false, "playerManager_result: �ǂ����`�[������");
			break;
		}

		pObj->SetPosition(pos);
	}
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerManager_Result::Uninit()
{
	// ���g�̃C���X�^���X�̔j��
	SAFE_DELETE(m_pInstance);
}

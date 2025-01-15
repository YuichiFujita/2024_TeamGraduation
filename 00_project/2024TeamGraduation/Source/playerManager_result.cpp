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
	const float INIT_IN_X(900.0f);	// �����ʒu(����)
	const float INIT_IN_Z(-400.0f);	// �����ʒu(����)
	const float INIT_OUT_X(900.0f);	// �����ʒu(�O��)
	const float INIT_OUT_Z(100.0f);	// �����ʒu(�O��)
	const float SHIFT(50.0f);	// ���炵�l

	const float RANGE = D3DX_PI * 0.6f;	// �����͈�
	const float RANGE_OUT = D3DX_PI * 1.0f;	// �����͈�
	const float DISTANCE = 400.0f;	// �Ԋu

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
	CListManager<CPlayerResult> sampleList = CPlayerResult::GetList();
	std::list<CPlayerResult*>::iterator itr = sampleList.GetEnd();
	CPlayerResult* pObj = nullptr;

	// ���E�̃��X�g
	std::vector<CPlayer*> leftList, rightList;
	std::vector<CPlayer*> outLeftList, outRightList;

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);
		// pObj��(*itr)���g���ď���
	
		// �`�[���擾
		CGameManager::ETeamSide team = pObj->GetTeam();

		// �J�E���^�[
		switch (team)
		{
		case CGameManager::ETeamSide::SIDE_LEFT:

			// ����
			if (pObj->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
			{
				// ���X�g�ǉ�
				leftList.push_back(pObj);
			}
			else
			{
				// ���X�g�ǉ�
				outLeftList.push_back(pObj);
			}
			break;
		
		case CGameManager::ETeamSide::SIDE_RIGHT:

			// ����
			if (pObj->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
			{
				// ���X�g�ǉ�
				rightList.push_back(pObj);
			}
			else
			{
				// ���X�g�ǉ�
				outRightList.push_back(pObj);
			}
			break;
	
		default:
			MyAssert::CustomAssert(false, "playerManager_result: �ǂ����`�[������");
			break;
		}

		pObj->SetPosition(pos);
	}




	// �v���C���[��
	float division = Player::RANGE / leftList.size();	// 1����������̌���
	float fRot = Player::RANGE / (float)(leftList.size() + 1);

	MyLib::Vector3 setpos, setrot;

	// ��
	for (int i = 0; i < leftList.size(); i++)
	{
		// �ʒu�v�Z
		setpos = Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;

		// �ʒu�ݒ�
		leftList[i]->SetPosition(setpos);

		// �����ݒ�
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT]);
		leftList[i]->SetRotation(setrot);
	}

	// ���O��
	division = Player::RANGE_OUT / outLeftList.size();	// 1����������̌���
	fRot = Player::RANGE_OUT / (float)(outLeftList.size() + 1);
	for (int i = 0; i < outLeftList.size(); i++)
	{
		// �ʒu�v�Z
		setpos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_LEFT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;

		// �J�[�u���ɂ₩�ɂ���
		setpos.z -= cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE * 0.75f;

		// �ʒu�ݒ�
		outLeftList[i]->SetPosition(setpos);

		// �����ݒ�
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_LEFT]);
		outLeftList[i]->SetRotation(setrot);
	}

	// �E
	division = Player::RANGE / rightList.size();	// 1����������̌���
	fRot = Player::RANGE / (float)(rightList.size() + 1);
	for (int i = 0; i < rightList.size(); i++)
	{
		// �ʒu�v�Z
		setpos = Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE * 0.5f) * Player::DISTANCE;

		// �ʒu�ݒ�
		rightList[i]->SetPosition(setpos);

		// �����ݒ�
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT]);
		rightList[i]->SetRotation(setrot);
	}

	// �E�O��
	division = Player::RANGE_OUT / outRightList.size();	// 1����������̌���
	fRot = Player::RANGE_OUT / (float)(outRightList.size() + 1);
	for (int i = 0; i < outRightList.size(); i++)
	{
		// �ʒu�v�Z
		setpos = Player::POS_OUT[CGameManager::ETeamSide::SIDE_RIGHT];
		setpos.x += sinf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;
		setpos.z += Player::DISTANCE * 0.25f + cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE;

		// �J�[�u���ɂ₩�ɂ���
		setpos.z -= cosf(fRot * (i + 1) - Player::RANGE_OUT * 0.5f) * Player::DISTANCE * 0.75f;

		// �ʒu�ݒ�
		outRightList[i]->SetPosition(setpos);

		// �����ݒ�
		setrot.y = setpos.AngleXZ(Player::POS_IN[CGameManager::ETeamSide::SIDE_RIGHT]);
		outRightList[i]->SetRotation(setrot);
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

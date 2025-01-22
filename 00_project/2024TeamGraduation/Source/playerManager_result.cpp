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
// �v���C���[����
//==========================================================================
HRESULT CPlayerManager_Result::CreatePlayer()
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
			// ���`�[���v���C���[����
			if (FAILED(CreateLeftPlayer(nCntLeft, vecInLeftInfo[j])))
			{ // �����Ɏ��s�����ꍇ

				return E_FAIL;
			}

			// ���l�����Z
			nCntLeft++;
		}

		if (j < nInMaxRight)
		{
			// �E�`�[���v���C���[����
			if (FAILED(CreateRightPlayer(nCntRight, vecInRightInfo[j])))
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
// �O��v���C���[����
//==========================================================================
HRESULT CPlayerManager_Result::CreateOutPlayer(CGameManager::ETeamSide team, const LoadInfo& info)
{
	// �v���C���[����
	CPlayer* pPlayer = CPlayer::Create
	(
		MyLib::Vector3(), 				// �ʒu
		team,							// �`�[���T�C�h
		CPlayer::EHuman::HUMAN_RESULT,	// �l
		info.eBody,						// �̌^
		info.eHanded,					// ������
		CPlayer::EFieldArea::FIELD_OUT	// �|�W�V����
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

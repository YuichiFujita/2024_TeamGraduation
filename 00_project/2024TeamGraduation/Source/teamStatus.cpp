//=============================================================================
// 
//  �`�[���X�e�[�^�X���� [teamStatus.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "teamStatus.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace Charm
{
	const float VALUE_MAX = 100.0f;	// �ő�l
}

namespace Special
{
	const float VALUE_MAX = 100.0f;												// �ő�l
	const MyLib::Vector2 GAUGE_SIZE = MyLib::Vector2(50.0f, 200.0f);			// �Q�[�W�T�C�Y
	const MyLib::Vector3 GAUGE_POS = MyLib::Vector3(50.0f, 100.0f, 0.0f);		// �Q�[�W�ʒu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTeamStatus::CTeamStatus() :
	m_typeTeam(CGameManager::SIDE_NONE),	// �`�[���T�C�h
	m_sCharmInfo(SCharmInfo()),				// ���e���
	m_sSpecialInfo(SSpecialInfo())			// �X�y�V�������
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTeamStatus::~CTeamStatus()
{

}

//==========================================================================
// ����
//==========================================================================
CTeamStatus* CTeamStatus::Create()
{
	// �������̊m��
	CTeamStatus* pStatus = DEBUG_NEW CTeamStatus;
	if (pStatus != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pStatus->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pStatus);
			return nullptr;
		}
	}

	return pStatus;
}

//==========================================================================
// ������
//==========================================================================
HRESULT CTeamStatus::Init()
{
	//���e���
	InitCharmInfo();

	//�X�y�V�������
	InitSpecialInfo();

	return S_OK;
}

//==========================================================================
// �I��
//==========================================================================
void CTeamStatus::Uninit()
{
	m_sSpecialInfo.pGauge = nullptr;
}

//==========================================================================
// �`�[���ݒ�ɂ��
//==========================================================================
void CTeamStatus::TeamSetting(const CGameManager::TeamSide team)
{
	CObject2D::AnchorPoint anchor = CObject2D::AnchorPoint::CENTER;
	MyLib::Vector3 pos = MyLib::Vector3();
	MyLib::Vector3 dest = Special::GAUGE_POS;

	SetTeam(team);

	switch (team)
	{
	case CGameManager::SIDE_LEFT:

		anchor = CObject2D::LEFT;
		pos += MyLib::Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);	// ����
		pos += MyLib::Vector3(-dest.x, -dest.y, 0.0f);
		break;

	case CGameManager::SIDE_RIGHT:

		anchor = CObject2D::RIGHT;
		pos += MyLib::Vector3(0.0f, SCREEN_HEIGHT, 0.0f);	// �E��
		pos += MyLib::Vector3(dest.x, -dest.y, 0.0f);
		break;

	default:
		break;
	}

	m_sSpecialInfo.pGauge->SetAnchorType(anchor);
	m_sSpecialInfo.pGauge->SetPosition(pos);
}

//==========================================================================
// �X�y�V������񏉊���
//==========================================================================
void CTeamStatus::InitCharmInfo()
{
	m_sCharmInfo.fValueMax = Charm::VALUE_MAX;
}

//==========================================================================
// ���e��񏉊���
//==========================================================================
void CTeamStatus::InitSpecialInfo()
{
	if (m_sSpecialInfo.pGauge != nullptr)
	{
		m_sSpecialInfo.pGauge->Uninit();
		m_sSpecialInfo.pGauge = nullptr;
	}

	m_sSpecialInfo.pGauge = CObject2D::Create();
	m_sSpecialInfo.pGauge->SetSize(Special::GAUGE_SIZE);
	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;
}

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
	const float VALUE_MAX = 100.0f;										// �ő�l
	const MyLib::Vector2 GAUGE_SIZE = MyLib::Vector2(50.0f, 200.0f);	// �Q�[�W�T�C�Y
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
	return nullptr;
}

//==========================================================================
// ������
//==========================================================================
void CTeamStatus::Init()
{
	//���e���
	InitCharmInfo();

	//�X�y�V�������
	InitSpecialInfo();
}

//==========================================================================
// �I��
//==========================================================================
void CTeamStatus::Uninit()
{
	if (m_sSpecialInfo.pGauge != nullptr)
	{
		m_sSpecialInfo.pGauge->Uninit();
		m_sSpecialInfo.pGauge = nullptr;
	}
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
	m_sSpecialInfo.pGauge->SetAnchorType(CObject2D::LEFT);
	m_sSpecialInfo.pGauge->SetSize(Special::GAUGE_SIZE);
	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;

}

//=============================================================================
// 
//  �`�[���X�e�[�^�X���� [teamStatus.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "teamStatus.h"
#include "player.h"
#include "playerStatus.h"
#include "gauge2D.h"

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
	const float VALUE_FRAMERATE = 100.0f;										// �l�̐L�т鑬�x
	const MyLib::Vector2 GAUGE_SIZE = MyLib::Vector2(250.0f, 30.0f);			// �Q�[�W�T�C�Y
	const MyLib::Vector3 GAUGE_POS = MyLib::Vector3(30.0f, 50.0f, 0.0f);		// �Q�[�W�ʒu
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

	delete this;
}

//==========================================================================
// �`�[���ݒ�ɂ��
//==========================================================================
void CTeamStatus::TeamSetting(const CGameManager::ETeamSide team)
{
	CObject2D::AnchorPoint anchor = CObject2D::AnchorPoint::CENTER;
	MyLib::Vector3 pos = MyLib::Vector3();
	MyLib::Vector3 dest = Special::GAUGE_POS;
	//std::vector<D3DXVECTOR2> uv =
	//{// �e�N�X�`�����W
	//	uv[0] = D3DXVECTOR2(0.0f, 0.0f),
	//	uv[1] = D3DXVECTOR2(1.0f, 0.0f),
	//	uv[2] = D3DXVECTOR2(0.0f, 1.0f),
	//	uv[3] = D3DXVECTOR2(1.0f, 1.0f),
	//};

	// �`�[���T�C�h�ݒ�
	SetTeam(team);

	switch (team)
	{
	case CGameManager::ETeamSide::SIDE_LEFT:

		anchor = CObject2D::LEFT;
		pos += MyLib::Vector3(0.0f, SCREEN_HEIGHT, 0.0f);	// �E��
		pos += MyLib::Vector3(dest.x, -dest.y, 0.0f);
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		anchor = CObject2D::RIGHT;
		pos += MyLib::Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);	// ����
		pos += MyLib::Vector3(-dest.x, -dest.y, 0.0f);
		break;

	default:
		break;
	}

	m_sSpecialInfo.pGauge->SetAnchorType(anchor);
	m_sSpecialInfo.pGauge->SetPosition(pos);
	//m_sSpecialInfo.pGauge->SetTexUV(uv);
}

//==========================================================================
// �S�łŏI��
//==========================================================================
bool CTeamStatus::CheckAllDead()
{
	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// �����`�[���������Ă���ꍇ = �S�ł��Ă��Ȃ�
		if (pPlayer->GetTeam() == m_typeTeam &&
			pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN &&	// ����
			pPlayer->GetState() != CPlayer::EState::STATE_DEAD_AFTER)
		{
			return false;
		}
	}

	// �����I��
	CGameManager::GetInstance()->SetSceneType(CGameManager::ESceneType::SCENE_END);
	return true;
}

//==========================================================================
// ���e��񏉊���
//==========================================================================
void CTeamStatus::InitCharmInfo()
{
	m_sCharmInfo.fValueMax = Charm::VALUE_MAX;
}

//==========================================================================
// ���e�Q�[�W�l����
//==========================================================================
void CTeamStatus::AddCharmValue(float fValue)
{
	m_sCharmInfo.fValue = UtilFunc::Transformation::Clamp(m_sCharmInfo.fValue + fValue, 0.0f, m_sCharmInfo.fValueMax);
}
//==========================================================================
// ���e�Q�[�W�l����
//==========================================================================
void CTeamStatus::SubCharmValue(float fValue)
{
	m_sCharmInfo.fValue = UtilFunc::Transformation::Clamp(m_sCharmInfo.fValue - fValue, 0.0f, m_sCharmInfo.fValueMax);
}

//==========================================================================
// �X�y�V������񏉊���
//==========================================================================
void CTeamStatus::InitSpecialInfo()
{
	if (m_sSpecialInfo.pGauge != nullptr)
	{
		m_sSpecialInfo.pGauge->Uninit();
		m_sSpecialInfo.pGauge = nullptr;
	}

	//�Q�[�W����
	m_sSpecialInfo.pGauge = CGauge2D::Create(Special::VALUE_MAX, 0.1f, Special::GAUGE_POS, Special::GAUGE_SIZE);
	m_sSpecialInfo.pGauge->SetColorFront(MyLib::color::White());
	m_sSpecialInfo.pGauge->SetColorBack(MyLib::color::Black());

	// �l�̏�����
	ZeroSpecialValue();

	// ����ݒ�
	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;
}

//==========================================================================
// �X�y�V�����Q�[�W����
//==========================================================================
void CTeamStatus::SetSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue =	UtilFunc::Transformation::Clamp(fValue, 0.0f, m_sSpecialInfo.fValueMax);

	if (m_sSpecialInfo.pGauge != nullptr)
	{// �Q�[�W�ɒl�ݒ�
		m_sSpecialInfo.pGauge->SetNum(m_sSpecialInfo.fValue);
	}

	if (m_sSpecialInfo.pGauge != nullptr)
	{
		MyLib::Vector2 size = Special::GAUGE_SIZE;
		float fRad = m_sSpecialInfo.fValue / m_sSpecialInfo.fValueMax;

		// �T�C�Y�ݒ�
		m_sSpecialInfo.pGauge->SetSizeGaugeRadius(fRad);
	}
}

//==========================================================================
// �X�y�V�����Q�[�W����
//==========================================================================
void CTeamStatus::AddSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue += fValue;
	SetSpecialValue(m_sSpecialInfo.fValue);
}

//==========================================================================
// �X�y�V�����Q�[�W����
//==========================================================================
void CTeamStatus::SubSpecialValue(float fValue)
{
	m_sSpecialInfo.fValue -= fValue;
	SetSpecialValue(m_sSpecialInfo.fValue);
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CTeamStatus::Debug()
{
	if (ImGui::TreeNode("TeamStatus"))
	{
		if (m_sSpecialInfo.pGauge != nullptr)
		{// �Q�[�W��max����

			D3DXCOLOR col = m_sSpecialInfo.pGauge->GetBar()->GetColor();

			ImGui::Text("BrightTime: [%.2f]", m_sSpecialInfo.pGauge->GetBrightTime());
			ImGui::Text("BrightTime: [r: %.2f] [g: %.2f] [b: %.2f] [a: %.2f]", col.r, col.g, col.b, col.a);
		}

		ImGui::TreePop();
	}
}

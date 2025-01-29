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
#include "gameEndManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace Charm
{
	const float VALUE_MAX = 100.0f;	// �ő�l
}

namespace Special
{
	const float VALUE_MAX = 100.0f;			// �ő�l
	const float VALUE_FRAMERATE = 100.0f;	// �l�̐L�т鑬�x
	const MyLib::Vector2 GAUGE_SIZE = MyLib::Vector2(250.0f, 14.0f);		// �Q�[�W�T�C�Y
	const MyLib::Vector3 GAUGE_POS = MyLib::Vector3(-2.0f, 34.0f, 0.0f);	// �Q�[�W�ʒu
	const float FRAME_WIDTH = GAUGE_SIZE.x + 13.0f;	// �t���[������
	const float ASSIST_WIDTH = 48.0f;				// �A�V�X�g����
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
	std::vector<D3DXVECTOR2> uv =
	{// �e�N�X�`�����W
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(1.0f, 1.0f),
	};

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

		// �e�N�X�`�����]
		UtilFunc::Calculation::SwapValue(uv[0], uv[1]);
		UtilFunc::Calculation::SwapValue(uv[2], uv[3]);
		break;

	default:
		break;
	}

	m_sSpecialInfo.pGauge->SetTeam(team);
	m_sSpecialInfo.pGauge->SetAnchorType(anchor);
	m_sSpecialInfo.pGauge->SetPosition(pos);
	m_sSpecialInfo.pGauge->SetTexUV(uv);

	const MyLib::Vector3 OFFSET_FRAME[] =
	{
		MyLib::Vector3(-13.0f, -13.0f, 0.0f),
		MyLib::Vector3(12.5f, -13.0f, 0.0f),
	};
	m_sSpecialInfo.pGauge->SetOffsetFrame(OFFSET_FRAME[team]);

	const MyLib::Vector3 OFFSET_ASSIST[] =
	{
		MyLib::Vector3(375.0f, -31.0f, 0.0f),
		MyLib::Vector3(-375.0f, -31.0f, 0.0f),
	};
	m_sSpecialInfo.pGauge->SetOffsetAssist(OFFSET_ASSIST[team]);

	const MyLib::Vector3 OFFSET_NAME[] =
	{
		MyLib::Vector3(12.0f, -32.0f, 0.0f),
		MyLib::Vector3(-12.0f, -32.0f, 0.0f),
	};
	m_sSpecialInfo.pGauge->SetOffsetTeamName(OFFSET_NAME[team]);

	const EAlignX ALIGN_X[] =
	{
		XALIGN_LEFT,
		XALIGN_RIGHT,
	};
	m_sSpecialInfo.pGauge->SetAlignX(ALIGN_X[team]);

	// �O���t�@�C������ݒ肳�ꂽ�`�[������Ǎ�
	std::string sTeamName;	// �`�[����
	CEntryRuleManager::LoadName(&sTeamName, m_typeTeam);	// �`�[�����Ǎ�
	m_sSpecialInfo.pGauge->SetString(sTeamName);
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
		CGameManager::ETeamSide team = pPlayer->GetTeam();
		CPlayer::EFieldArea area = pPlayer->GetAreaType();
		CPlayer::EState state = pPlayer->GetState();

		// �����`�[���������Ă���ꍇ = �S�ł��Ă��Ȃ�
		if (team == m_typeTeam &&
			area == CPlayer::EFieldArea::FIELD_IN &&	// ����
			(state != CPlayer::EState::STATE_DEAD_AFTER &&
			state != CPlayer::EState::STATE_DEAD_CARRY))
		{
			return false;
		}
	}
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

	// �Q�[�W�̐���
	CTexture* pTexture = CTexture::GetInstance();
	m_sSpecialInfo.pGauge = CGauge2D::Create
	( // ����
		Special::VALUE_MAX,		// �ő�\���l
		10.1f,					// �\���l�ϓ��t���[��
		"",						// �`�[����
		Special::GAUGE_POS,		// �ʒu
		VEC3_ZERO,				// �t���[���I�t�Z�b�g
		VEC3_ZERO,				// �A�V�X�g�I�t�Z�b�g
		VEC3_ZERO,				// �`�[�����I�t�Z�b�g
		Special::GAUGE_SIZE,	// �Q�[�W�傫��
		VEC2_ZERO,				// �t���[���傫��
		VEC2_ZERO,				// �A�V�X�g�傫��
		MyLib::color::Blue()	// �Q�[�W�F
	);

	// �t���[���e�N�X�`������
	const int nTexFrameID = pTexture->Regist("data\\TEXTURE\\gaugeframe000.png");
	m_sSpecialInfo.pGauge->BindTextureFrame(nTexFrameID);

	// �t���[�����������ɃT�C�Y��ݒ�
	MyLib::Vector2 sizeFrame = pTexture->GetImageSize(nTexFrameID);
	sizeFrame = UtilFunc::Transformation::AdjustSizeByWidth(sizeFrame, Special::FRAME_WIDTH);
	m_sSpecialInfo.pGauge->SetSizeFrame(sizeFrame);

	// �A�V�X�g�e�N�X�`������
	const int nTexAssistID = pTexture->Regist("data\\TEXTURE\\xbox_lb.png");
	m_sSpecialInfo.pGauge->BindTextureAssist(nTexAssistID);

	// �A�V�X�g���������ɃT�C�Y��ݒ�
	MyLib::Vector2 sizeAssist = pTexture->GetImageSize(nTexAssistID);
	sizeAssist = UtilFunc::Transformation::AdjustSizeByWidth(sizeAssist, Special::ASSIST_WIDTH);
	m_sSpecialInfo.pGauge->SetSizeAssist(sizeAssist);

	// ����ݒ�
	m_sSpecialInfo.fValueMax = Special::VALUE_MAX;

	// �l�̏�����
	ZeroSpecialValue();
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
		float fRad = m_sSpecialInfo.fValue / m_sSpecialInfo.fValueMax;	// TODO�Ffloat�O����

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
// �Q�[�W�`��t���O�ݒ�
//==========================================================================
void CTeamStatus::SetEnableGaugeDisp(const bool bDisp)
{
	m_sSpecialInfo.pGauge->SetEnableDisp(bDisp);
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CTeamStatus::Debug()
{

}

//=============================================================================
// 
//  �v���C���[�X�e�[�^�X���� [playerStatus.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerStatus.h"
#include "player.h"
#include "object_circlegauge2D.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int MIN_LIFE = 0;	// �ŏ��̗�
}

namespace LifeGauge
{
	const int DIVISION = 32;	// ������
	const float SIZE = 40.0f;	// �T�C�Y
	const float DISTANCE = 100.0f;	// �Ԋu
	const MyLib::Vector3 DEFAULTPOS[CGameManager::ETeamSide::SIDE_MAX] =
	{
		MyLib::Vector3(60.0f, 80.0f, 0.0f),	// ���T�C�h
		MyLib::Vector3(SCREEN_WIDTH - 60.0f, 80.0f, 0.0f),	// �E�T�C�h
	};
}

//==========================================================================
// �ÓI�����o�ϐ���`
//==========================================================================
CListManager<CObjectCircleGauge2D> CPlayerStatus::m_LifeGaugeListLeft = {};		// ���̗̓Q�[�W�̃��X�g
CListManager<CObjectCircleGauge2D> CPlayerStatus::m_LifeGaugeListRight = {};	// �E�̗̓Q�[�W�̃��X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerStatus::CPlayerStatus(CPlayer* pPlayer) :
	m_pPlayer		(pPlayer),	// �v���C���[�̃|�C���^
	m_pLifeGauge	(nullptr)	// �̗̓Q�[�W
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerStatus::~CPlayerStatus()
{
	m_LifeGaugeListLeft.Delete(m_pLifeGauge);
	m_LifeGaugeListRight.Delete(m_pLifeGauge);
}

//==========================================================================
// ����������
//==========================================================================
void CPlayerStatus::Init()
{
	// �̗̓Q�[�W����
	if (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_IN)
	{// ����̂ݐ���

		// �̗̓Q�[�W����
		CreateLifeGuge(&m_pLifeGauge_Ground, false);
		CreateLifeGuge(&m_pLifeGauge, true);

		// ���n�^����
		m_pLifeGauge_Ground->SetColor(MyLib::color::Black());

		// �Q�[�W�t���[������
		CreateGaugeFrame();
	}
}

//==========================================================================
// �̗̓Q�[�W����
//==========================================================================
void CPlayerStatus::CreateLifeGuge(CObjectCircleGauge2D** pGauge, bool bAddList)
{
	// �̗̓Q�[�W����
	(*pGauge) = CObjectCircleGauge2D::Create(LifeGauge::DIVISION, LifeGauge::SIZE);
	(*pGauge)->SetType(CObject::TYPE::TYPE_UI);

	// �`�[���擾
	CGameManager::ETeamSide teamSide = m_pPlayer->GetTeam();

	// ���X�g�ǉ��E�ʒu�ݒ�
	int nNum = 0;
	switch (teamSide)
	{
	case CGameManager::ETeamSide::SIDE_NONE:
		return;

	case CGameManager::ETeamSide::SIDE_LEFT:

		// ���ɂ��镪�l�����Ă��炷
		nNum = m_LifeGaugeListLeft.GetNumAll();

		// �ʒu�ݒ�
		(*pGauge)->SetPosition(LifeGauge::DEFAULTPOS[teamSide] + MyLib::Vector3(nNum * LifeGauge::DISTANCE, 0.0f, 0.0f));

		// ���X�g�ǉ�
		if (bAddList)
		{
			m_LifeGaugeListLeft.Regist((*pGauge));
		}
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:

		// ���ɂ��镪�l�����Ă��炷
		nNum = m_LifeGaugeListRight.GetNumAll();

		// �ʒu�ݒ�
		(*pGauge)->SetPosition(LifeGauge::DEFAULTPOS[teamSide] - MyLib::Vector3(nNum * LifeGauge::DISTANCE, 0.0f, 0.0f));

		// ���X�g�ǉ�
		if (bAddList)
		{
			m_LifeGaugeListRight.Regist((*pGauge));
		}
		break;

	default:
		assert(false);
		break;
	}

	// �F�ݒ�
	(*pGauge)->SetColor(MyLib::color::Green());


	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist("data\\TEXTURE\\GRADATION\\black_04.jpg");
	(*pGauge)->BindTexture(nTexID);
}

//==========================================================================
// �Q�[�W�t���[������
//==========================================================================
void CPlayerStatus::CreateGaugeFrame()
{
	// �Q�[�W�t���[��
	m_pGaugeFrame = CObject2D::Create(m_pLifeGauge->GetPriority());

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist("data\\TEXTURE\\player\\gauge.png");
	m_pGaugeFrame->BindTexture(nTexID);

	// �ʒu�ݒ�
	m_pGaugeFrame->SetPosition(m_pLifeGauge->GetPosition());

	// �T�C�Y�ݒ�
	m_pGaugeFrame->SetSize(MyLib::Vector2(LifeGauge::SIZE * 1.25f));
}

//==========================================================================
// �폜����
//==========================================================================
void CPlayerStatus::Kill()
{
	// �폜
	if (m_pLifeGauge != nullptr)
	{
		m_LifeGaugeListLeft.Delete(m_pLifeGauge);
		m_LifeGaugeListRight.Delete(m_pLifeGauge);
		m_pLifeGauge->Kill();
		m_pLifeGauge = nullptr;
	}

	// �Q�[�W�폜
	SAFE_KILL(m_pGaugeFrame);
	SAFE_KILL(m_pLifeGauge_Ground);
}

//==========================================================================
//	�̗͌��Z
//==========================================================================
void CPlayerStatus::LifeDamage(const int nDmg)
{
	// �O��̏ꍇ������
	if (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) return;

	int nLife = m_pPlayer->GetLife();	// �̗�
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// �̗͂����Z
	nLife -= nDmg;

	// �̗͂�␳
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// �̗͂𔽉f
	m_pPlayer->SetLife(nLife);

	// �Q�[�W�̖ڕW�l�ݒ�
	float ratio = static_cast<float>(nLife) / static_cast<float>(nLifeOrigin);
	m_pLifeGauge->SetRateDest(ratio);
}

//==========================================================================
//	�̗͉��Z
//==========================================================================
void CPlayerStatus::LifeHeal(const int nHeal)
{
	// �O��̏ꍇ������
	if (m_pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) { return; }

	int nLife = m_pPlayer->GetLife();	// �̗�
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// �̗͂����Z
	nLife += nHeal;

	// �̗͂�␳
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// �̗͂𔽉f
	m_pPlayer->SetLife(nLife);

	// �Q�[�W�̖ڕW�l�ݒ�
	float ratio = static_cast<float>(nLife) / static_cast<float>(nLifeOrigin);
	m_pLifeGauge->SetRateDest(ratio);
}

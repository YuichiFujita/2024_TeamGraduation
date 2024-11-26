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
}

//==========================================================================
// �ÓI�����o�ϐ���`
//==========================================================================
//CListManager<CObjectCircleGauge2D> CPlayerStatus::m_LifeGaugeList = {};	// �̗̓Q�[�W�̃��X�g

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

}

//==========================================================================
// ����������
//==========================================================================
void CPlayerStatus::Init()
{
	// �̗̓Q�[�W����
	m_pLifeGauge = CObjectCircleGauge2D::Create(LifeGauge::DIVISION, LifeGauge::SIZE);
}

//==========================================================================
// �폜����
//==========================================================================
void CPlayerStatus::Kill()
{
	// �폜
	if (m_pLifeGauge != nullptr)
	{
		m_pLifeGauge->Kill();
		m_pLifeGauge = nullptr;
	}
}

//==========================================================================
//	�̗͌��Z
//==========================================================================
void CPlayerStatus::LifeDamage(const int nDmg)
{
	int nLife = m_pPlayer->GetLife();	// �̗�
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// �̗͂����Z
	nLife -= nDmg;

	// �̗͂�␳
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// �̗͂𔽉f
	m_pPlayer->SetLife(nLife);

	// �Q�[�W�̖ڕW�l�ݒ�
	m_pLifeGauge->SetRateDest(nLife / nLifeOrigin);
}

//==========================================================================
//	�̗͉��Z
//==========================================================================
void CPlayerStatus::LifeHeal(const int nHeal)
{
	int nLife = m_pPlayer->GetLife();	// �̗�
	int nLifeOrigin = m_pPlayer->GetLifeOrigin();

	// �̗͂����Z
	nLife += nHeal;

	// �̗͂�␳
	UtilFunc::Transformation::ValueNormalize(nLife, nLifeOrigin, MIN_LIFE);

	// �̗͂𔽉f
	m_pPlayer->SetLife(nLife);

	// �Q�[�W�̖ڕW�l�ݒ�
	m_pLifeGauge->SetRateDest(nLife / nLifeOrigin);
}

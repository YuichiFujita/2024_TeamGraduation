//=============================================================================
// 
//  �v���C���[�X�e�[�^�X���� [playerStatus.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerStatus.h"
#include "player.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int MAX_LIFE = 100;	// �ő�̗�
	const int MIN_LIFE = 0;		// �ŏ��̗�
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerStatus::CPlayerStatus(CPlayer* pPlayer) :
	m_pPlayer	(pPlayer),					// �v���C���[�̃|�C���^
	m_typeTeam	(CGameManager::SIDE_NONE)	// �`�[���T�C�h
{
	// �̗͂�������
	m_pPlayer->SetLife(MAX_LIFE);
	m_pPlayer->SetLifeOrigin(MAX_LIFE);
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerStatus::~CPlayerStatus()
{

}

//==========================================================================
//	�̗͌��Z
//==========================================================================
void CPlayerStatus::LifeDamage(const int nDmg)
{
	int nLife = m_pPlayer->GetLife();	// �̗�

	// �̗͂����Z
	nLife -= nDmg;

	// �̗͂�␳
	UtilFunc::Transformation::ValueNormalize(nLife, MAX_LIFE, MIN_LIFE);

	// �̗͂𔽉f
	m_pPlayer->SetLife(nLife);
}

//==========================================================================
//	�̗͉��Z
//==========================================================================
void CPlayerStatus::LifeHeal(const int nHeal)
{
	int nLife = m_pPlayer->GetLife();	// �̗�

	// �̗͂����Z
	nLife += nHeal;

	// �̗͂�␳
	UtilFunc::Transformation::ValueNormalize(nLife, MAX_LIFE, MIN_LIFE);

	// �̗͂𔽉f
	m_pPlayer->SetLife(nLife);
}

//=============================================================================
// 
// �X�y�V�������o(���߂͂ߔg)���� [specialEffect_kamehameha.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "specialEffect_kamehameha.h"
#include "player.h"
#include "sound.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float SCALE_CHARGESTART = 5.0f;	// �`���[�W�J�n
	const float SCALE_CHARGEEND = 50.0f;	// �`���[�W�I��
	const float SCALE_CONDENCEEND = 3.0f;	// �Ïk�I��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// �g���K�[��
CSpecialEffect_Kamehame::TRIGGER_FUNC CSpecialEffect_Kamehame::m_TriggerFunc[] =
{
	&CSpecialEffect_Kamehame::Trigger_Brust,		// ����
	&CSpecialEffect_Kamehame::Trigger_Stance,		// �\��
	&CSpecialEffect_Kamehame::Trigger_CreateEnergy,	// ���߂͂ߔg�̒��S������
	&CSpecialEffect_Kamehame::Trigger_ChargeStart,	// �`���[�W�J�n��
};

// �i�s��
CSpecialEffect_Kamehame::PROGRESS_FUNC CSpecialEffect_Kamehame::m_ProgressFunc[] =
{
	&CSpecialEffect_Kamehame::Progress_Brust,		// ����
	&CSpecialEffect_Kamehame::Progress_ChargeStart,	// �`���[�W�J�n
	&CSpecialEffect_Kamehame::Progress_Chargeing,	// �`���[�W��
	&CSpecialEffect_Kamehame::Progress_Condense,	// �Ïk
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSpecialEffect_Kamehame::CSpecialEffect_Kamehame() : CSpecialEffect()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSpecialEffect_Kamehame::~CSpecialEffect_Kamehame()
{

}


//==========================================================================
// ����
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Brust(CMotion::AttackInfo ATKInfo)
{
	// ���߂͂ߔg�̃G�l���M�[�����폜
	m_pEnergy->SetTrigger(0);
	m_pEnergy->Uninit();
	m_pEnergy = nullptr;

	// TODO : ����
}

//==========================================================================
// �\��
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Stance(CMotion::AttackInfo ATKInfo)
{
	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();

	// �Y����ԃI�[��
	m_pAtmosphere = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ATMOSPHERE,
		pPlayer->GetPosition(),
		MyLib::Vector3(),
		MyLib::Vector3(),
		40.0f, false);
}

//==========================================================================
// ���߂͂ߔg�̒��S������
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_CreateEnergy(CMotion::AttackInfo ATKInfo)
{
	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);

	// ���߂͂ߔg�̒��S(�J�n��)����
	m_pEnergyStart = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ENERGY,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		SCALE_CHARGESTART, false);
}

//==========================================================================
// �`���[�W�J�n��
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_ChargeStart(CMotion::AttackInfo ATKInfo)
{
	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);

	// ���߂͂ߔg�̒��S(�J�n��)�I��
	m_pEnergyStart->SetTrigger(0);
	SAFE_UNINIT(m_pEnergyStart);

	// ���߂͂ߔg�̃G�l���M�[��������
	m_pEnergy = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ENERGY,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		SCALE_CHARGESTART, false);

	// �J�n�̕�����
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_CHARGE,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		20.0f, true);

	// TODO : �t���b�V������

}




//==========================================================================
// �`���[�W�J�n
//==========================================================================
void CSpecialEffect_Kamehame::Progress_ChargeStart(CMotion::AttackInfo ATKInfo)
{
	if (m_pEnergy == nullptr) return;

	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// �`���[�W��
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Chargeing(CMotion::AttackInfo ATKInfo)
{
	if (m_pEnergy == nullptr) return;

	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �����ɉ����Ċg��
	m_pEnergy->SetScale(SCALE_CHARGESTART + (SCALE_CHARGEEND - SCALE_CHARGESTART) * ATKInfo.fCntRatio);

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// �Ïk
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Condense(CMotion::AttackInfo ATKInfo)
{
	if (m_pEnergy == nullptr) return;

	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �����ɉ����Ċg��
	m_pEnergy->SetScale(SCALE_CHARGEEND + (SCALE_CONDENCEEND - SCALE_CHARGEEND) * ATKInfo.fCntRatio);

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// ����
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Brust(CMotion::AttackInfo ATKInfo)
{
	
}
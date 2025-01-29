//=============================================================================
// 
// �X�y�V�������o(���߂͂ߔg)���� [specialEffect_kamehameha.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "specialEffect_kamehameha.h"
#include "player.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float SCALE_CHARGESTART = 5.0f;	// �`���[�W�J�n
	const float SCALE_CHARGEEND = 50.0f;	// �`���[�W�I��
	const float SCALE_CONDENCEEND = 3.0f;	// �Ïk�I��
}

namespace Wind	// ��
{
	const float INTERVAL = 0.3f;	// �Ԋu
	const int RANDRANGE_SCALE = 5;	// �X�P�[���̃����_���l
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
	&CSpecialEffect_Kamehame::Progress_TransBrust,	// ���˂֑J��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSpecialEffect_Kamehame::CSpecialEffect_Kamehame() : CSpecialEffect(),
	m_bWindCreate(false),				// �������t���O
	m_fIntervalWind(Wind::INTERVAL),	// �������C���^�[�o��
	m_fCreateWindTime(0.0f)				// �������^�C�}�[
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSpecialEffect_Kamehame::~CSpecialEffect_Kamehame()
{
	// �Y����ԃI�[��
	SAFE_UNINIT(m_pAtmosphere);

	// �`���[�W���̗�
	SAFE_UNINIT(m_pChargeThunder);

	// ���ꂫ
	SAFE_UNINIT(m_pBallast);
}

//==========================================================================
// �X�V
//==========================================================================
void CSpecialEffect_Kamehame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���X�V
	UpdateWind(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �I�����̐ݒ�
//==========================================================================
void CSpecialEffect_Kamehame::FinishSetting()
{
	// �Y����ԃI�[��
	m_pAtmosphere->SetTrigger(0);
	m_pAtmosphere->DeleteLater(1.0f);
	m_pAtmosphere = nullptr;

	// �`���[�W���̗�
	m_pChargeThunder->SetTrigger(0);
	m_pChargeThunder->DeleteLater(1.0f);
	m_pChargeThunder = nullptr;

	// ���ꂫ
	m_pBallast->SetTrigger(0);
	m_pBallast->DeleteLater(1.0f);
	m_pBallast = nullptr;

	// �T�E���h��~
	CSound::GetInstance()->StopSound(CSound::ELabel::LABEL_SE_KMHM_LOOP);
}

//==========================================================================
// ���X�V
//==========================================================================
void CSpecialEffect_Kamehame::UpdateWind(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���������Ȃ��ꍇ�͔�����
	if (!m_bWindCreate) return;

	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();

	// �������^�C�}�[���Z
	m_fCreateWindTime += fDeltaTime * fSlowRate;

	if (m_fIntervalWind <= m_fCreateWindTime)
	{// �C���^�[�o���o��

		// �^�C�}�[���Z�b�g
		m_fCreateWindTime = 0.0f;

		// �C���^�[�o��
		m_fIntervalWind = Wind::INTERVAL + UtilFunc::Transformation::Random(-30, 10) * 0.01f;

		// ������
		CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_CHARGE_WIND,
			pPlayer->GetPosition(),
			MyLib::Vector3(),
			MyLib::Vector3(),
			15.0f + UtilFunc::Transformation::Random(-Wind::RANDRANGE_SCALE, Wind::RANDRANGE_SCALE) * 0.1f, true);

	}
}

//==========================================================================
// ����
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Brust(CMotionManager::AttackInfo ATKInfo)
{
	// ���߂͂ߔg�̃G�l���M�[�����폜
	if (m_pEnergy != nullptr)
	{
		m_pEnergy->SetTrigger(0);
		m_pEnergy->Uninit();
		m_pEnergy = nullptr;
	}

	// TODO : ����
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);

	// ���߂͂ߔg�̃G�l���M�[��������
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_BRUST,
		posAtkInfo,
		MyLib::Vector3(0.0f, pPlayer->GetRotation().y, 0.0f),
		MyLib::Vector3(),
		20.0f, true);

	// �T�E���h�̍Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_BRUST);
}

//==========================================================================
// �\��
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_Stance(CMotionManager::AttackInfo ATKInfo)
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
// ���߂͂ߔg�̒��S(�J�n)������
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_CreateEnergy(CMotionManager::AttackInfo ATKInfo)
{
	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// ���߂͂ߔg�̒��S(�J�n��)����
	m_pEnergyStart = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_ENERGY,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		SCALE_CHARGESTART, false);

	// �`���[�W�̗�����
	m_pChargeThunder = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_CHARGE_THUNDER,
		playerPos,
		MyLib::Vector3(),
		MyLib::Vector3(),
		40.0f, false);

	// ���ꂫ����
	m_pBallast = CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_BALLAST,
		playerPos,
		MyLib::Vector3(),
		MyLib::Vector3(),
		20.0f, false);

	// ��������ON
	m_bWindCreate = true;

	// �T�E���h�̍Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_START);

	// �T�E���h�̍Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_CHARGE);
}

//==========================================================================
// �`���[�W�J�n��
//==========================================================================
void CSpecialEffect_Kamehame::Trigger_ChargeStart(CMotionManager::AttackInfo ATKInfo)
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

	// �t���b�V������
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_CAMEHAME_FLASH,
		posAtkInfo,
		MyLib::Vector3(),
		MyLib::Vector3(),
		40.0f, true);

	// �T�E���h��~
	CSound::GetInstance()->StopSound(CSound::ELabel::LABEL_SE_KMHM_CHARGE);

	// �T�E���h�̍Đ�
	PLAY_SOUND(CSound::ELabel::LABEL_SE_KMHM_LOOP);
}




//==========================================================================
// �`���[�W�J�n
//==========================================================================
void CSpecialEffect_Kamehame::Progress_ChargeStart(CMotionManager::AttackInfo ATKInfo)
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
void CSpecialEffect_Kamehame::Progress_Chargeing(CMotionManager::AttackInfo ATKInfo)
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
void CSpecialEffect_Kamehame::Progress_Condense(CMotionManager::AttackInfo ATKInfo)
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
// ���˂֑J��
//==========================================================================
void CSpecialEffect_Kamehame::Progress_TransBrust(CMotionManager::AttackInfo ATKInfo)
{
	// ��������~
	m_bWindCreate = false;

	if (m_pEnergy == nullptr) return;

	// �v���C���[�擾
	CPlayer* pPlayer = GetPlayer();
	CMotion* pMotion = pPlayer->GetMotion();

	// �U�����̈ʒu�֐ݒ�
	MyLib::Vector3 posAtkInfo = pMotion->GetAttackPosition(pPlayer->GetModel(), ATKInfo);
	m_pEnergy->SetPosition(posAtkInfo);
}

//==========================================================================
// ����
//==========================================================================
void CSpecialEffect_Kamehame::Progress_Brust(CMotionManager::AttackInfo ATKInfo)
{
	
}
//=============================================================================
//
// �X�y�V�������o(���߂͂ߔg)���� [specialEffect_kamehameha.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERATRIGGER_PASS_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERATRIGGER_PASS_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "motion.h"
#include "specialEffect.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����g���K�[_�p�X�N���X
class CSpecialEffect_Kamehame : public CSpecialEffect
{
public:

	CSpecialEffect_Kamehame();
	~CSpecialEffect_Kamehame();
	
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	// �g���K�[�̏u��
	virtual void TriggerMoment(CMotion::AttackInfo ATKInfo, int idx) override
	{
		(this->*(m_TriggerFunc[idx]))(ATKInfo);
	}

	// �i�s��
	virtual void ProgressMoment(CMotion::AttackInfo ATKInfo, int idx) override
	{
		(this->*(m_ProgressFunc[idx]))(ATKInfo);
	}

	virtual void FinishSetting() override;	// �I�����̐ݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	// �g���K�[���̏���
	typedef void(CSpecialEffect_Kamehame::*TRIGGER_FUNC)(CMotion::AttackInfo);
	static TRIGGER_FUNC m_TriggerFunc[];

	// �i�s���̏���
	typedef void(CSpecialEffect_Kamehame::*PROGRESS_FUNC)(CMotion::AttackInfo);
	static TRIGGER_FUNC m_ProgressFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// �g���K�[
	void Trigger_Brust(CMotion::AttackInfo ATKInfo);		// ����
	void Trigger_Stance(CMotion::AttackInfo ATKInfo);		// �\��
	void Trigger_CreateEnergy(CMotion::AttackInfo ATKInfo);	// ���߂͂ߔg�̒��S������
	void Trigger_ChargeStart(CMotion::AttackInfo ATKInfo);	// �`���[�W�J�n��

	// �i�s��
	void Progress_Brust(CMotion::AttackInfo ATKInfo);		// ����
	void Progress_ChargeStart(CMotion::AttackInfo ATKInfo);	// �`���[�W�J�n
	void Progress_Chargeing(CMotion::AttackInfo ATKInfo);	// �`���[�W��
	void Progress_Condense(CMotion::AttackInfo ATKInfo);	// �Ïk
	void Progress_TransBrust(CMotion::AttackInfo ATKInfo);	// ���˂֑J��

	// ���̑�
	void UpdateWind(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���X�V

	//=============================
	// �����o�ϐ�
	//=============================
	CEffekseerObj* m_pEnergyStart = nullptr;	// ���߂͂ߔg�̒��S(�J�n��)
	CEffekseerObj* m_pEnergy = nullptr;			// ���߂͂ߔg�̒��S
	CEffekseerObj* m_pAtmosphere = nullptr;		// �Y����ԃI�[��
	CEffekseerObj* m_pChargeThunder = nullptr;	// �`���[�W���̗�
	CEffekseerObj* m_pBallast = nullptr;		// ���ꂫ

	// ���p
	bool m_bWindCreate;			// �������t���O
	float m_fIntervalWind;		// �������C���^�[�o��
	float m_fCreateWindTime;	// �������^�C�}�[
};

#endif
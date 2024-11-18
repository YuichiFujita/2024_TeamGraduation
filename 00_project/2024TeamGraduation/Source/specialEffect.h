//=============================================================================
//
// �X�y�V�������o���� [specialEffect.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _SPECIALEFFECT_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _SPECIALEFFECT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "motion.h"
#include "EffekseerObj.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;

//==========================================================================
// �N���X��`
//==========================================================================
// �X�y�V�������o�N���X
class CSpecialEffect
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EType
	{
		TYPE_KAMEHAMEHA = 0,
		TYPE_MAX
	};

	CSpecialEffect();
	virtual ~CSpecialEffect();
	
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;			// �X�V
	virtual void TriggerMoment(CMotion::AttackInfo ATKInfo, int idx) = 0;	// �g���K�[�̏u��
	virtual void ProgressMoment(CMotion::AttackInfo ATKInfo, int idx) = 0;	// �i�s��

	CPlayer* GetPlayer() { return m_pPlayer; }	// �v���C���[�擾

	// ����
	static CSpecialEffect* Create(CPlayer* pPlayer, EType type)
	{
		// �����֐�
		CSpecialEffect* pEffect = m_CreateFunc[type]();

		if (pEffect != nullptr)
		{// �v���C���[���蓖��
			pEffect->m_pPlayer = pPlayer;
		}
		return pEffect;
	}

private:

	//=============================
	// �֐����X�g
	//=============================
	// �������X�g
	using CREATE_FUNC = std::function<CSpecialEffect*()>;
	static std::vector<CREATE_FUNC>m_CreateFunc;

	// �����o�֐�
	CPlayer* m_pPlayer;	// �v���C���[
};

#endif
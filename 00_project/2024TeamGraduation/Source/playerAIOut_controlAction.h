//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�A�N�V�����w�b�_�[ [playerAIcontrol_action.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_ACTION_H_
#define _PLAYERAI_CONTROL_ACTION_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playercontrol_action.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerUserControlAction;

//==========================================================================
// �v���C���[AI�R���g���[��_�A�N�V�����N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerAIOutControlAction : public CPlayerControlAction
{
private:
	//=============================
	// �\����
	//=============================
	struct SFlag
	{
		bool bThrow;		// �����邩
		bool bPass;			// �p�X
		bool bJump;			// �W�����v
		bool bSpecial;		// �X�y�V����
	};

public:

	// �R���X�g���N�^
	CPlayerAIOutControlAction();

	//=============================
	// ���z�֐�
	//=============================
	virtual CPlayerAIOutControlAction* GetAIOut() override { return this; }

	//=============================
	// �����o�֐�
	//=============================
	void SetIsThrow(bool bThrow) { m_sFlag.bThrow = bThrow; }			// �����邩�ݒ�
	void SetIsPass(bool bPass) { m_sFlag.bPass = bPass; }				// �p�X���邩
	void SetIsJump(bool bJump) { m_sFlag.bJump = bJump; }				// �W�����v���邩�ݒ�
	void SetIsSpecial(bool bSpecial) { m_sFlag.bSpecial = bSpecial; }	// �X�y�V���������ݒ�


#ifdef _DEBUG
	void ChangeAutoThrow() { m_bAutoThrow = !m_bAutoThrow; }
	void SetEnableAutoThrow(bool bAuto) { m_bAutoThrow = bAuto; }
	bool IsAutoThrow() { return m_bAutoThrow; }
#endif

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �L���b�`
	void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ����

	void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;			// �W�����v
	void JumpFloat(CPlayer* player) override;	// �W�����v�㏸
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �X�y�V����
	void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ���e

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static bool m_bAutoThrow;

	//=============================
	// �����o�ϐ�
	//=============================
	float fThrowTime = 0.0f;
	SFlag m_sFlag;

};

#endif

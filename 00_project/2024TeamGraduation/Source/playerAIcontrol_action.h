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
class CPlayerAIControlAction : public CPlayerControlAction
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
		bool bJumpFloat;	// �W�����v�z�[���h
		bool bSpecial;		// �X�y�V����
	};

public:

	// �R���X�g���N�^
	CPlayerAIControlAction();

	//=============================
	// ���z�֐�
	//=============================
	virtual CPlayerAIControlAction* GetAI() override { return this; };

	//=============================
	// �����o�֐�
	//=============================
	void SetIsThrow(bool bThrow) { m_sFlag.bThrow = bThrow; }				// �����邩�ݒ�
	void SetIsPass(bool bPass) { m_sFlag.bPass = bPass; }					// �p�X���邩�ݒ�
	void SetIsJump(bool bJump) { m_sFlag.bJump = bJump; }					// �W�����v���邩�ݒ�
	void SetIsJumpFloat(bool passFloat) { m_sFlag.bJumpFloat = passFloat; }	// �����Ղ��p�����邩�ݒ�
	void SetIsSpecial(bool bSpecial) { m_sFlag.bSpecial = bSpecial; }		// �X�y�V���������ݒ�
	void SetJumpRate(float rate) { m_fJumpRate = rate; }
	SFlag GetIsFlag() { return m_sFlag; }


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
	void JumpFloat(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;																			// �W�����v�㏸
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
	float m_fJumpTimer;
	float m_fJumpRate;
	SFlag m_sFlag;

};

#endif

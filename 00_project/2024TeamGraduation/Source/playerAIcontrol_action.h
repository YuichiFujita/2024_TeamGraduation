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
// �v���C���[AI�R���g���[��_�A�N�V�����N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerAIControlAction : public CPlayerControlAction
{
public:

	// �R���X�g���N�^
	CPlayerAIControlAction();

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

};

#endif

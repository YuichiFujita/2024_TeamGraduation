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
#include "player.h"

//==========================================================================
// �v���C���[AI�R���g���[��_�A�N�V�����N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerAIControlAction
{
public:

	// �R���X�g���N�^
	CPlayerAIControlAction();

	virtual void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`

#ifdef _DEBUG
	void ChangeAutoThrow() { m_bAutoThrow = !m_bAutoThrow; }
	void SetEnableAutoThrow(bool bAuto) { m_bAutoThrow = bAuto; }
	bool IsAutoThrow() { return m_bAutoThrow; }
#endif

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`
	void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ����
	void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �W�����v
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�y�V����
	void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���e

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
#ifdef _DEBUG
	static bool m_bAutoThrow;

	float fThrowTime = 0.0f;
#endif

	//=============================
	// �����o�ϐ�
	//=============================
};

#endif

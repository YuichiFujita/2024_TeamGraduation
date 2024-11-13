//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playerUsercontrol_action.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERUSER_CONTROL_ACTION_H_
#define _PLAYERUSER_CONTROL_ACTION_H_	// ��d�C���N���[�h�h�~

#include "playercontrol_action.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerUserControlAction;
class CPlayerAIControlAction;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerUserControlAction : public CPlayerControlAction
{
public:

	// �R���X�g���N�^
	CPlayerUserControlAction();

	//=============================
	// ���z�֐�
	//=============================
	virtual CPlayerUserControlAction* GetUser() override { return this; }

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
	// �����o�ϐ�
	//=============================
};

#endif
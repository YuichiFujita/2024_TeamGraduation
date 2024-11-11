//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_CONTROL_ACTION_H_
#define _PLAYER_CONTROL_ACTION_H_	// ��d�C���N���[�h�h�~

#include "playercontrol.h"

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
class CPlayerControlAction : public CPlayerControl
{
public:

	// �R���X�g���N�^
	CPlayerControlAction();

	void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����

	//=============================
	// �������z�֐�
	//=============================
	virtual CPlayerAIControlAction* GetAI() = 0;
	virtual CPlayerUserControlAction* GetUser() = 0;

protected:
	//=============================
	// �����o�֐�
	//=============================
	void ThrowSetting(CPlayer* player);
	void JumpSetting(CPlayer* player);
	void SpecialSetting(CPlayer* player, CBall* pBall, CTeamStatus* pTeamStatus);		// �X�y�V��������

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void ConditionalAction(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ��{�A�N�V��������

	virtual void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// �L���b�`
	virtual void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// ����
	virtual void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// �W�����v
	virtual void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �X�y�V����
	virtual void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// ���e

	//=============================
	// �����o�ϐ�
	//=============================
};


#endif
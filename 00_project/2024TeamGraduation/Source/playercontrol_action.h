//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_CONTROL_ACTION_H_
#define _PLAYER_CONTROL_ACTION_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playercontrol.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlAction;
class CPlayerAIOutControlAction;
class CPlayerUserControlAction;
class CPlayerUserOutControlAction;

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
	// ���z�֐�
	//=============================
	virtual CPlayerAIControlAction* GetAI()				{ return nullptr; }
	virtual CPlayerAIOutControlAction* GetAIOut()		{ return nullptr; }
	virtual CPlayerUserControlAction* GetUser()			{ return nullptr; }
	virtual CPlayerUserOutControlAction* GetUserOut()	{ return nullptr; }

	void SetThrowDrop();								// ����(�h���b�v�{�[��)�ݒ�
	inline bool IsThrowDrop() { return m_bThrowDrop; }	// ����(�h���b�v�{�[��)�\����擾

	void SetThrowJust();								// ����(�W���X�g�L���b�`)�ݒ�
	inline bool IsThrowJust() { return m_bThrowJust; }	// ����(�W���X�g�L���b�`)�\����擾

	// �W�����v�g���K�[
	void SetEnableJumpTrigger(bool bTrigger)	{ m_bJumpTrigger = bTrigger; }	// �W�����v�g���K�[
	bool IsJumpTrigger()						{ return m_bJumpTrigger; }		// �W�����v�g���K�[

protected:
	//=============================
	// �����o�֐�
	//=============================
	void PassSetting(CPlayer* player);
	void ThrowSetting(CPlayer* player);
	void JumpSetting(CPlayer* player);
	virtual void JumpFloat(CPlayer* playe, const float fDeltaTime, const float fDeltaRate, const float fSlowRater);	// �W�����v�㏸
	void SpecialSetting(CPlayer* player, CBall* pBall, CTeamStatus* pTeamStatus);	// �X�y�V��������

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void ConditionalAction(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��{�A�N�V��������
	void UpdateThrowDrop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����(�h���b�v�{�[��)�̗P�\
	void UpdateThrowJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����(�W���X�g�L���b�`)�̗P�\

	//=============================
	// �������z�֐�
	//=============================
	virtual void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)	= 0;	// �L���b�`
	virtual void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)	= 0;	// ����
	virtual void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)	= 0;	// �W�����v
	virtual void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)	= 0;	// ���e

	//=============================
	// ���z�֐�
	//=============================
	virtual void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)	{}	// �X�y�V����
	virtual void UserChange(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)	{}	// ���[�U�[�ύX

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fThrowDropTime;	// ����(�h���b�v�{�[��)�̗P�\
	bool m_bThrowDrop;		// ����(�h���b�v�{�[��)�\����
	float m_fThrowJustTime;	// ����(�W���X�g�L���b�`)�̗P�\
	bool m_bThrowJust;		// ����(�W���X�g�L���b�`)�\����
	bool m_bJumpTrigger;	// �W�����v�g���K�[
};


#endif
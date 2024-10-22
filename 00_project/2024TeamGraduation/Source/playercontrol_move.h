//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_move.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_CONTROL_MOVE_H_
#define _PLAYER_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

#define BRESSRANGE (0)

#include "playercontrol.h"

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerControlMove : public CPlayerControl
{
public:

	// �R���X�g���N�^
	CPlayerControlMove();

	virtual void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ړ�

	float GetTriggerInterval() { return m_fTriggerInterval; }									//�g���K�[�̃C���^�[�o���擾
	void SetBlink(bool bDash) { m_bBlink = bDash; }												// �u�����N�t���O�ݒ�
	bool IsBlink() { return m_bBlink; }															// �u�����N�t���O�擾

private:
	
	//=============================
	// �����o�֐�
	//=============================
	virtual void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �u�����N
	virtual void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����
	virtual void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �E�H�[�N
	virtual CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::DashAngle angle);	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nCntTrigger[CPlayer::DashAngle::ANGLE_MAX];	// �g���K�[�̃J�E���g
	CPlayer::DashAngle m_HoldDashAngle;					// �ێ����Ă�_�b�V���̈ړ�����
	float m_fInputInterval;								// ���͂̎󂯕t���P�\
	float m_fTriggerInterval;							// �g���K�[�̃C���^�[�o��
	bool m_bBlink;										// ����t���O
};

#endif
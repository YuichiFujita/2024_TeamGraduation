//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_move.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_MOVE_H_
#define _PLAYERCONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

#define BRESSRANGE (0)

#include "player.h"

class CSuffocation;
class CEffekseerObj;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerControlMove
{
public:

	// �R���X�g���N�^
	CPlayerControlMove();

	virtual void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ړ�

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �u�����N
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �u�����N
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �E�H�[�N
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::DashAngle angle);	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nCntTrigger[CPlayer::DashAngle::ANGLE_MAX];	// �g���K�[�̃J�E���g
	CPlayer::DashAngle m_HoldDashAngle;					// �ێ����Ă�_�b�V���̈ړ�����
	float m_fInputInterval;								// ���͂̎󂯕t���P�\
	float m_fTriggerInterval;							// �g���K�[�̃C���^�[�o��
	bool m_bDash;										// ����t���O
};


#endif
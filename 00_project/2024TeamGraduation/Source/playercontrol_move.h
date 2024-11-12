//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_move.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_CONTROL_MOVE_H_
#define _PLAYER_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

#include "playercontrol.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerUserControlMove;
class CPlayerAIControlMove;

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

	//=============================
	// �������z�֐�
	//=============================
	virtual CPlayerAIControlMove* GetAI() = 0;
	virtual CPlayerUserControlMove* GetUser() = 0;

	virtual void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ړ�

	void SetCntTrigger(int nIdx, int nCnt) { m_nCntTrigger[nIdx] = nCnt; }						// �g���K�[�̃J�E���g�ݒ�
	void SetCntTrigger(int* nTrigger);															// �g���K�[�̃J�E���g�ݒ�
	int GetCntTrigger(int nIdx) { return m_nCntTrigger[nIdx]; }									// �g���K�[�̃J�E���g�擾
	int* GetCntTrigger() { return &m_nCntTrigger[0]; }											// �g���K�[�̃J�E���g�擾
	void SetHoldDashAngle(CPlayer::EDashAngle eAngle) { m_HoldDashAngle = eAngle; }				// �_�b�V�������ݒ�
	CPlayer::EDashAngle GetHoldDashAngle() { return m_HoldDashAngle; }							// �_�b�V�������擾
	void SetInputInterval(float fInputInterval) { m_fInputInterval = fInputInterval; }			// ���͂̎󂯕t���P�\�ݒ�
	float GetInputInterval() { return m_fInputInterval; }										// ���͂̎󂯕t���P�\�擾
	void SetTriggerInterval(float fTriggerInterval) { m_fTriggerInterval = fTriggerInterval; }	// �g���K�[�̃C���^�[�o���ݒ�
	float GetTriggerInterval() { return m_fTriggerInterval; }									// �g���K�[�̃C���^�[�o���擾
	void SetBlink(bool bDash) { m_bBlink = bDash; }												// �u�����N�t���O�ݒ�
	bool IsBlink() { return m_bBlink; }															// �u�����N�t���O�擾
	void SetInputAngle(CPlayer::EDashAngle* pInputAngle) { m_pInputAngle = pInputAngle; }		// ���݂̓��͕����ݒ�
	CPlayer::EDashAngle* GetInputAngle() { return m_pInputAngle; }								// ���݂̓��͕����擾
	void SetInputAngleCtr(float fInputAngleCtr) { m_fInputAngleCtr = fInputAngleCtr; }			// ���݂̓��͕����J�E���^�[�ݒ�
	float GetInputAngleCtr() { return m_fInputAngleCtr; }										// ���݂̓��͕����J�E���^�[�擾

	void CrabSetting(CPlayer* player);				// �J�j�������

private:
	
	//=============================
	// �����o�֐�
	//=============================
	virtual void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �u�����N
	virtual void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// ����
	virtual void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �E�H�[�N
	virtual CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) = 0;	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nCntTrigger[CPlayer::EDashAngle::ANGLE_MAX];	// �g���K�[�̃J�E���g
	CPlayer::EDashAngle m_HoldDashAngle;					// �ێ����Ă�_�b�V���̈ړ�����
	float m_fInputInterval;								// ���͂̎󂯕t���P�\
	float m_fTriggerInterval;							// �g���K�[�̃C���^�[�o��
	bool m_bBlink;										// ����t���O
	CPlayer::EDashAngle* m_pInputAngle;					// ���݂̓��͕���
	float m_fInputAngleCtr;								// ���݂̓��͕����̕ێ��J�E���^�[
};

#endif
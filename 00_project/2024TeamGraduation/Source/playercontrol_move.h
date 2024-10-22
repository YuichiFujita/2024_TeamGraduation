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

	void SetCntTrigger(int nIdx, int nCnt) { m_nCntTrigger[nIdx] = nCnt; }						//�g���K�[�̃J�E���g�ݒ�(��)
	void SetCntTrigger(int* nTrigger);															//�g���K�[�̃J�E���g�ݒ�(�S)
	int GetCntTrigger(int nIdx) { return m_nCntTrigger[nIdx]; }									//�g���K�[�̃J�E���g�擾(��)
	int* GetCntTrigger() { return &m_nCntTrigger[0]; }											//�g���K�[�̃J�E���g�擾(�S)
	void SetHoldDashAngle(CPlayer::DashAngle angle) { m_HoldDashAngle = angle; }				//�_�b�V�������ݒ�
	CPlayer::DashAngle GetHoldDashAngle() { return m_HoldDashAngle; }							//�_�b�V�������擾
	void SetInputInterval(float fInputInterval) { m_fInputInterval = fInputInterval; }			//���͂̎󂯕t���P�\�ݒ�
	float GetInputInterval() { return m_fInputInterval; }										//���͂̎󂯕t���P�\�擾
	void SetTriggerInterval(float fTriggerInterval) { m_fTriggerInterval = fTriggerInterval; }	//�g���K�[�̃C���^�[�o���ݒ�
	float GetTriggerInterval() { return m_fTriggerInterval; }									//�g���K�[�̃C���^�[�o���擾
	void SetDash(bool bDash) { m_bDash = bDash; }												//����t���O�ݒ�
	bool GetDash() { return m_bDash; }															//����t���O�擾

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
	bool m_bDash;										// ����t���O
};

#endif
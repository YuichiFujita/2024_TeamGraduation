//==========================================================================
// 
//  �O��v���C���[�R���g���[���w�b�_�[ [playerAIOut_controlMove.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYERAI_OUT_CONTROL_MOVE_H_
#define _PLAYERAI_OUT_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// �O��v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerAIOutControlMove : public CPlayerControlMove
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIOutControlMove();
	virtual ~CPlayerAIOutControlMove() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual CPlayerAIOutControlMove* GetAIOut() override { return this; }

	//=============================
	// �����o�֐�
	//=============================
	void SetIsBlink(bool bBlink) { m_bBlink = bBlink; }
	void SetIsDash(bool bDash) { m_bDash = bDash; }
	void SetIsWalk(bool bWalk) { m_bWalk = bWalk; }
	void SetIsClab(bool clab) { m_bClab = clab;}

	void SetClabDirection(float clabDirection) { m_fClabDirection = clabDirection; }
	float GetClabDirection() { return m_fClabDirection; }	// �J�j�i�s�����̎擾

	//=============================
	// �����o�ϐ�
	//=============================

private:

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �u�����N
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �_�b�V��
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �E�H�[�N
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
	bool m_bBlink;
	bool m_bDash;
	bool m_bWalk;
	bool m_bClab;
	float m_fClabDirection;	// �J�j�̂��񂱂�����
};

#endif
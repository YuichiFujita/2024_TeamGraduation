//==========================================================================
// 
//  �O��v���C���[�R���g���[���w�b�_�[ [playerUserOut_controlMove.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYERUSER_OUT_CONTROL_MOVE_H_
#define _PLAYERUSER_OUT_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerUserOutControlMove;
class CPlayerAIControlMove;
class CBindKey;

//==========================================================================
// �O��v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerUserOutControlMove : public CPlayerControlMove
{
public:

	// �R���X�g���N�^
	CPlayerUserOutControlMove();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual CPlayerUserOutControlMove* GetUserOut() override { return this; }

	//=============================
	// �����o�֐�
	//=============================
	void BindLeftKey(CBindKey* pKey)	{ m_pLeftKey = pKey; }	// ���ړ��L�[����
	void BindRightKey(CBindKey* pKey)	{ m_pRightKey = pKey; }	// �E�ړ��L�[����

private:

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �u�����N
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �_�b�V��
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �E�H�[�N
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�֐�
	//=============================
	void BilnkKey(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L�[���͂̃u�����N
	void BilnkStick(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�e�B�b�N���͂̃u�����N

	//=============================
	// �����o�ϐ�
	//=============================
	CBindKey* m_pLeftKey;	// ���ړ��L�[���
	CBindKey* m_pRightKey;	// �E�ړ��L�[���
};

#endif
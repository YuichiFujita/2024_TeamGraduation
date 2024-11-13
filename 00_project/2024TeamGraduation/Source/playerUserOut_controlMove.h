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
	virtual CPlayerAIControlMove* GetAI() override { return nullptr; }
	virtual CPlayerUserControlMove* GetUser() override { return nullptr; }

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �u�����N
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �_�b�V��
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �E�H�[�N
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_posLeft;	// �ړ��\���ʒu
	MyLib::Vector3 m_posRight;	// �ړ��\�E�ʒu
};

#endif
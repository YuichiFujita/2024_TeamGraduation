//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�ړ��w�b�_�[ [playerAIcontrol_move.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_MOVE_H_
#define _PLAYERAI_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

#define BRESSRANGE (0)

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// �O���錾
//==========================================================================
class CSuffocation;
class CEffekseerObj;

//==========================================================================
// �v���C���[AI�R���g���[��_�ړ��N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerAIControlMove : public CPlayerControlMove
{
public:

	// �R���X�g���N�^
	CPlayerAIControlMove();

	void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �ړ�

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �u�����N
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �u�����N
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �E�H�[�N
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::DashAngle angle) override;	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
};


#endif
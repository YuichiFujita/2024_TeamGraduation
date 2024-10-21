//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playerUsercontrol_move.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERUSER_CONTROL_MOVE_H_
#define _PLAYERUSER_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

#define BRESSRANGE (0)

#include "playercontrol_move.h"

class CSuffocation;
class CEffekseerObj;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerUserControlMove : public CPlayerControlMove
{
public:

	// �R���X�g���N�^
	CPlayerUserControlMove();

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
//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playerUsercontrol_move.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERUSER_CONTROL_MOVE_H_
#define _PLAYERUSER_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

#include "playercontrol_move.h"

//==========================================================================
// �O���錾
//==========================================================================
class CSuffocation;
class CEffekseerObj;
class CPlayerUserControlMove;
class CPlayerAIControlMove;

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

	//=============================
	// ���z�֐�
	//=============================
	virtual CPlayerUserControlMove* GetUser() override { return this; }

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

};

#endif
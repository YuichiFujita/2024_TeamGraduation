//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// ��d�C���N���[�h�h�~

#include "player.h"

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerControl
{
public:

	// �R���X�g���N�^
	CPlayerControl();

	virtual void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`
	void SetPattern(CPlayer* player, CPlayer::MOTION typeM, CPlayer::Action typeA);
	void SetJump(CPlayer* player, CPlayer::MOTION typeM, CPlayer::Action typeA);

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`
	void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ����
	void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �W�����v
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�y�V����
	void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���e

	//=============================
	// �����o�ϐ�
	//=============================
};


#endif
//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playerUsercontrol_action.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERUSER_CONTROL_ACTION_H_
#define _PLAYERUSER_CONTROL_ACTION_H_	// ��d�C���N���[�h�h�~

#include "playercontrol_action.h"

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerUserControlAction : public CPlayerControlAction
{
public:

	// �R���X�g���N�^
	CPlayerUserControlAction();

	virtual void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �L���b�`
	void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ����
	void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;			// �W�����v
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// �X�y�V����
	void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ���e

	//=============================
	// �����o�ϐ�
	//=============================
};

#endif
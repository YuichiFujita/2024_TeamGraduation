//==========================================================================
// 
//  �O��v���C���[�A�N�V�����R���g���[���w�b�_�[ [playerUserOut_controlAction.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYERUSER_OUT_CONTROL_ACTION_H_
#define _PLAYERUSER_OUT_CONTROL_ACTION_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerUsercontrol_action.h"

//==========================================================================
// �O��O��v���C���[�A�N�V�����R���g���[���N���X��`
//==========================================================================
//=============================
// �A�N�V����
//=============================
class CPlayerUserOutControlAction : public CPlayerUserControlAction
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerUserOutControlAction();
	virtual ~CPlayerUserOutControlAction() override;

	//=============================
	// ���z�֐�
	//=============================
	virtual CPlayerUserOutControlAction* GetUserOut() override { return this; }

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override {}	// �X�y�V����
};

#endif

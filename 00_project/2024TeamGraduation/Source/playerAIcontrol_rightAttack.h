//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_RIGHTATTACK_H_
#define _PLAYERAICONTROL_RIGHTATTACK_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "playerAIcontrol_attack.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayer;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControlRightAttack : public CPlayerAIControlAttack
{
public:

	enum  EThrowType			// �����^�C�v
	{
		THROWTYPE_NONE = 0,		// �Ȃ�
		THROWTYPE_NORMAL,		// �ʏ�
		THROWTYPE_JUMP,			// �W�����v
		THROWTYPE_SPECIAL,		// �X�y�V����
		//THROWTYPE_PASS,			// �p�X
		THROWTYPE_MAX
	};

public:

	// �R���X�g���N�^
	CPlayerAIControlRightAttack();
	
	// ����
	static CPlayerAIControlRightAttack* Create();

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

private:
	//=============================
	// �����o�֐�
	//=============================
	void PreparationLeave(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Preparation() override;

	//=============================
	// �����o�ϐ�
	//=============================

};

#endif

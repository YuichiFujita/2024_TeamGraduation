//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_LEFTATTACK_H_
#define _PLAYERAICONTROL_LEFTATTACK_H_	// ��d�C���N���[�h�h�~

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
class CPlayerAIControlLeftAttack : public CPlayerAIControlAttack
{
public:

	// �R���X�g���N�^
	CPlayerAIControlLeftAttack();

	static CPlayerAIControlLeftAttack* Create(CPlayer* player);

	HRESULT Init() ;
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

private:
	//=============================
	// �����o�֐�
	//=============================
	void PreparationLeave(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	//=============================
	// �����o�ϐ�
	//=============================
};

#endif

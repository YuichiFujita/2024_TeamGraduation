//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_LEFT_H_
#define _PLAYERAICONTROL_LEFT_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "playerAIcontrol.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayer;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControlLeft : public CPlayerAIControl
{
public:

	// �R���X�g���N�^
	CPlayerAIControlLeft();

	static CPlayerAIControlLeft* Create(CPlayer* player);

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

private:
	//=============================
	// �����o�֐�
	//=============================

	// �I�[�o�[���C�h
	bool IsLineOverBall() override;				// ����������(�{�[��)
	bool IsLineOverPlayer() override;			// ���z������(�v���C���[)
	void AttackDash(CPlayer* pTarget) override;	// ���蓊��
	void ForciblyReturn() override;				// 
	virtual void MoveRetreat() override;		// 

	//=============================
	// �����o�ϐ�
	//=============================
};

#endif

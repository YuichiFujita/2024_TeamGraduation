//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_LEFTDEFENSE_H_
#define _PLAYERAICONTROL_LEFTDEFENSE_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "playerAIcontrol_defense.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayer;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControlLeftDefense : public CPlayerAIControlDefense
{
public:

	// �R���X�g���N�^
	CPlayerAIControlLeftDefense();

	static CPlayerAIControlLeftDefense* Create();


	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

private:
	//=============================
	// ���z�E�����֐�
	//=============================
	void MoveRetreat() override;				// ���
	void MoveRandom() override;					// �����_��
	bool IsLineOverBall() override;				// ����������(�{�[��)
	bool IsLineOverPlayer() override;			// ���z������(�v���C���[)
	//void BallSteal() override;
	//void BallChaseRebound() override;

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
};

#endif

//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_RIGHTDEFENSE_H_
#define _PLAYERAICONTROL_RIGHTDEFENSE_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "playerAIcontrol_defense.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayerAIControl;
class CPlayer;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControlRightDefense : public CPlayerAIControlDefense
{
public:

	// �R���X�g���N�^
	CPlayerAIControlRightDefense();

	static CPlayerAIControlRightDefense* Create(CPlayer* player);


	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

private:
	//=============================
	// ���z�E�����֐�
	//=============================
	bool IsLineOverBall() override;				// ����������(�{�[��)
	bool IsLineOverPlayer() override;			// ���z������(�v���C���[)
	void MoveRetreat() override;				// ���
	void MoveRandom() override;					// �����_��
	void BallSteal() override;

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
};

#endif

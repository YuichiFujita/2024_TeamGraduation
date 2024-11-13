//==========================================================================
// 
//  AI�v���C���[�w�b�_�[ [playerAI.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)

//==========================================================================
// �N���X��`
//==========================================================================
// AI�v���C���[�N���X
class CPlayerAI : public CPlayerBase
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAI(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAI() = 0;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerControlAction* control);	// �A�N�V�����̑���ύX
};

#endif

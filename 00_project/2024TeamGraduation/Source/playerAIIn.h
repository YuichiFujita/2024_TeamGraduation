//==========================================================================
// 
//  AI����v���C���[�w�b�_�[ [playerAIIn.h]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================

#ifndef _PLAYER_AI_IN_
#define _PLAYER_AI_IN_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerIn.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControl;	// �R���g���[��(AI)

//==========================================================================
// �N���X��`
//==========================================================================
// AI����v���C���[�N���X
class CPlayerAIIn : public CPlayerIn
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAIIn() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual void MotionCrab(int nStartKey) override;		// �J�j�����ω�
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// �q�b�g
	virtual void Debug() override;							// �f�o�b�O
	virtual CPlayerAIIn* GetPlayerAIIn() override { return nullptr; }	// AI����v���C���[�擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayerAIControl* m_pAIControl;
};

#endif

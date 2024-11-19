//==========================================================================
// 
//  AI�O��v���C���[�w�b�_�[ [playerAIOut.h]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================

#ifndef _PLAYER_AI_OUT_
#define _PLAYER_AI_OUT_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerOut.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIOutControl;	// �R���g���[��(AI�O��)

//==========================================================================
// �N���X��`
//==========================================================================
// AI�O��v���C���[�N���X
class CPlayerAIOut : public CPlayerOut
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAIOut() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// �q�b�g
	virtual void Debug() override;	// �f�o�b�O
	virtual CPlayerAIOut* GetPlayerAIOut() override { return this; }	// AI�O��v���C���[�擾

private:
	//=============================
	// �����o�ϐ�
	//=============================
	CPlayerAIOutControl* m_pAIOutControl;	// AI�R���g���[��(�O��)
};

#endif

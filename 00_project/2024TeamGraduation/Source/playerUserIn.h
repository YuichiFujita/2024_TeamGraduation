//==========================================================================
// 
//  ���[�U�[����v���C���[�w�b�_�[ [playerUserIn.h]
//  Author : ���c�E��
//  Adder  : ���c����
// 
//==========================================================================

#ifndef _PLAYER_USER_IN_
#define _PLAYER_USER_IN_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerIn.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�U�[����v���C���[�N���X
class CPlayerUserIn : public CPlayerIn
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerUserIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerUserIn() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void MotionCrab(int nStartKey) override;	// �J�j�����ω�
	virtual CPlayerUserIn* GetPlayerUserIn() override { return nullptr; }	// ���[�U�[����v���C���[�擾
};

#endif

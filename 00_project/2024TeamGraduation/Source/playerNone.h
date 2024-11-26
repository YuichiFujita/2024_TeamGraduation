//==========================================================================
// 
//  �w��Ȃ��v���C���[�w�b�_�[ [playerNone.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_NONE_
#define _PLAYER_NONE_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �w��Ȃ��v���C���[�N���X
class CPlayerNone : public CPlayerBase
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerNone(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerNone() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override { return {}; }	// �q�b�g
	virtual void MotionCrab(int nStartKey) override {}					// �J�j�����ω�
};

#endif

//==========================================================================
// 
//  �o��p�v���C���[�w�b�_�[ [playerSpawn.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_SPAWN_
#define _PLAYER_SPAWN_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �o��p�v���C���[�N���X
class CPlayerSpawn : public CPlayerBase
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerSpawn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerSpawn() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// �q�b�g
	virtual void MotionCrab(int nStartKey) override {}		// �J�j�����ω�

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerControlAction* control);	// �A�N�V�����̑���ύX
};

#endif

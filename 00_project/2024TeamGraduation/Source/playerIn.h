//==========================================================================
// 
//  ����v���C���[�w�b�_�[ [playerIn.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_IN_
#define _PLAYER_IN_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ����v���C���[�N���X
class CPlayerIn : public CPlayerBase
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerIn() override = 0;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;		// �q�b�g
	virtual CPlayerIn* GetPlayerIn() override { return this; }	// �O��v���C���[�擾

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerControlAction* control);	// �A�N�V�����̑���ύX
};

#endif

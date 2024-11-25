//==========================================================================
// 
//  �G���g���[�p�v���C���[�w�b�_�[ [playerEntry.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _PLAYER_ENTRY_
#define _PLAYER_ENTRY_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �G���g���[�p�v���C���[�N���X
class CPlayerEntry : public CPlayerBase
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerEntry(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerEntry() override;

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

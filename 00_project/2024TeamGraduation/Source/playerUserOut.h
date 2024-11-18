//==========================================================================
// 
//  ���[�U�[�O��v���C���[�w�b�_�[ [playerUserOut.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_USER_OUT_
#define _PLAYER_USER_OUT_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerOut.h"
#include "playerUserOut_controlMove.h"

//==========================================================================
// �O���錾
//==========================================================================
class CBindKey;

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�U�[�O��v���C���[�N���X
class CPlayerUserOut : public CPlayerOut
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerUserOut() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual CPlayerUserOut* GetPlayerUserOut() override { return this; }	// ���[�U�[�O��v���C���[�擾

	//=============================
	// �����o�֐�
	//=============================
	void BindLeftKey(CBindKey* pKey) { GetPlayerControlMove()->GetUserOut()->BindLeftKey(pKey); }	// ���ړ��L�[����
	void BindRightKey(CBindKey* pKey) { GetPlayerControlMove()->GetUserOut()->BindRightKey(pKey); }	// �E�ړ��L�[����
};

#endif

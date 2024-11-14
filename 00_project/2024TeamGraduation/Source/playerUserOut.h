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
#include "playerUser.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�U�[�O��v���C���[�N���X
class CPlayerUserOut : public CPlayerUser
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
	virtual bool IsCrab() override { return false; }	// �J�j��������

	//=============================
	// �����o�֐�
	//=============================
	MyLib::Vector3 GetPosLeft()	 { return m_posLeft; }	// �ړ��\�ȍ��ʒu�̎擾
	MyLib::Vector3 GetPosRight() { return m_posRight; }	// �ړ��\�ȉE�ʒu�̎擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_posLeft;	// �ړ��\���ʒu
	MyLib::Vector3 m_posRight;	// �ړ��\�E�ʒu
};

#endif

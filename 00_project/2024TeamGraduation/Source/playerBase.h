//==========================================================================
// 
//  �v���C���[�x�[�X�w�b�_�[ [playerBase.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_BASE_H_
#define _PLAYER_BASE_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerControlMove;
class CPlayerControlAction;
class CBall;

//==========================================================================
// �v���C���[�x�[�X��`
//==========================================================================
class CPlayerBase
{
public:
	// �R���X�g���N�^
	CPlayerBase(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea);

	// �f�X�g���N�^
	virtual ~CPlayerBase() = 0;

	// �������z�֐�
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �X�V

	// ���z�֐�
	virtual CPlayer::SHitInfo Hit(CBall* pBall);	// �q�b�g
	virtual void Debug();	// �f�o�b�O

	// �����o�֐�
	void DeleteControl();	// ����j��
	void SetPlayerControlMove(CPlayerControlMove* pControlMove);		// ����ݒ� (�ړ�)
	void SetPlayerControlAction(CPlayerControlAction* pControlAction);	// ����ݒ� (�A�N�V����)
	CPlayerControlMove* GetPlayerControlMove()		{ return m_pControlMove; }		// ����擾 (�ړ�)
	CPlayerControlAction* GetPlayerControlAction()	{ return m_pControlAction; }	// ����擾 (�A�N�V����)

protected:
	// �����o�֐�
	CPlayer* GetPlayer() { return m_pPlayer; }	// �v���C���[���擾

private:
	// �����o�ϐ�
	CPlayer* m_pPlayer;	// �v���C���[���
	CPlayerControlMove* m_pControlMove;		// �ړ�����
	CPlayerControlAction* m_pControlAction;	// �A�N�V��������
};

#endif

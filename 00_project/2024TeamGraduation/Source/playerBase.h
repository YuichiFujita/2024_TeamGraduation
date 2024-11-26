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
class CPlayerIn;
class CPlayerOut;
class CPlayerUserIn;
class CPlayerUserOut;
class CPlayerAIIn;
class CPlayerAIOut;
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
	CPlayerBase(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);

	// �f�X�g���N�^
	virtual ~CPlayerBase() = 0;

	// �������z�֐�
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �X�V
	virtual void MotionCrab(int nStartKey) = 0;	// �J�j�����ω�

	// ���z�֐�
	virtual CPlayer::SHitInfo Hit(CBall* pBall);			// �q�b�g
	virtual void InitPosition(const MyLib::Vector3& rPos);	// �ʒu������
	virtual bool IsCrab();	// �J�j��������
	virtual void Debug();	// �f�o�b�O
	virtual CPlayerIn* GetPlayerIn()			{ return nullptr; }	// ����v���C���[�擾
	virtual CPlayerOut* GetPlayerOut()			{ return nullptr; }	// �O��v���C���[�擾
	virtual CPlayerUserIn* GetPlayerUserIn()	{ return nullptr; }	// ���[�U�[����v���C���[�擾
	virtual CPlayerUserOut* GetPlayerUserOut()	{ return nullptr; }	// ���[�U�[�O��v���C���[�擾
	virtual CPlayerAIIn* GetPlayerAIIn()		{ return nullptr; }	// AI����v���C���[�擾
	virtual CPlayerAIOut* GetPlayerAIOut()		{ return nullptr; }	// AI�O��v���C���[�擾

	// �����o�֐�
	void UpdateChangeBase();	// �x�[�X�ύX�X�V
	void DeleteControl();		// ����j��
	void ChangeMoveControl(CPlayerControlMove* control);				// �ړ��̑���ύX
	void ChangeActionControl(CPlayerControlAction* control);			// �A�N�V�����̑���ύX
	void SetPlayerControlMove(CPlayerControlMove* pControlMove);		// ����ݒ� (�ړ�)
	void SetPlayerControlAction(CPlayerControlAction* pControlAction);	// ����ݒ� (�A�N�V����)
	void SetNewBase(const CPlayer::EBaseType typeBase)	{ m_newTypeBase = typeBase; }	// �V�x�[�X�ݒ�
	CPlayerControlMove* GetPlayerControlMove()			{ return m_pControlMove; }		// ����擾 (�ړ�)
	CPlayerControlAction* GetPlayerControlAction()		{ return m_pControlAction; }	// ����擾 (�A�N�V����)

protected:
	// �����o�֐�
	CPlayer* GetPlayer() { return m_pPlayer; }	// �v���C���[���擾

private:
	// �����o�ϐ�
	const CPlayer::EBaseType m_curTypeBase;	// ���݂̃x�[�X�^�C�v
	CPlayer::EBaseType m_newTypeBase;		// �V�����x�[�X�^�C�v
	CPlayer* m_pPlayer;						// �v���C���[���
	CPlayerControlMove* m_pControlMove;		// �ړ�����
	CPlayerControlAction* m_pControlAction;	// �A�N�V��������
};

#endif

//=============================================================================
// 
//  �v���C���[�A�N�V�����w�b�_�[ [playerAction.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERACTION_H_
#define _PLAYERACTION_H_	// ��d�C���N���[�h�h�~

#include "player.h"

//==========================================================================
// �v���C���[�A�N�V�����N���X
//==========================================================================
class CPlayerAction
{
public:

	// �R���X�g���N�^
	CPlayerAction(CPlayer* player);

	//=============================
	// �����o�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V
	void SetAction(CPlayer::Action action);						// �A�N�V�����ݒ�
	CPlayer::Action GetAction() { return m_Action; }			// �A�N�V�����擾
	void SetEnableCharm(bool bCharm) { m_bCharm = bCharm; }			// ���e�{�^����Ԑݒ�
	bool IsCharm() { return m_bCharm; }							// ���e�{�^����Ԏ擾
	void BindPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }	// �v���C���[���蓖��
private:
	
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAction::* ACTION_FUNC)(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	static ACTION_FUNC m_ActionFunc[];	// �s���֐�

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �Ȃ�
	void ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �u�����N
	void ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���
	void ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ����
	void ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �W�����v
	void ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`
	void ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ����
	void ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����(�W�����v)
	void ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �X�y�V����

	//=============================
	// �����o�ϐ�
	//=============================
	bool m_bCharm;				// ���e�{�^�����
	CPlayer::Action m_Action;	// �A�N�V����
	float m_fActionTime;		// �A�N�V��������
	CPlayer* m_pPlayer;			// �v���C���[�̃|�C���^
};


#endif
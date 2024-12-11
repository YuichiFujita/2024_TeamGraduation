//=============================================================================
// 
//  �v���C���[�A�N�V�����w�b�_�[ [playerAction.h]
//  Author : Kai Takada
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
	void SetAction(CPlayer::EAction action);					// �A�N�V�����ݒ�
	CPlayer::EAction GetAction() { return m_Action; }			// �A�N�V�����擾
	void SetEnableCharm(bool bCharm) { m_bCharm = bCharm; }		// ���e�{�^����Ԑݒ�
	bool IsCharm() { return m_bCharm; }							// ���e�{�^����Ԏ擾
	void BindPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }	// �v���C���[���蓖��

	void SetSlowStart(float rate) { m_fSlowStart = rate; }		// �X���[�{��(�J�n)�ݒ�
	float GetSlowStart() { return m_fSlowStart; }				// �X���[�{��(�J�n)�擾
	void SetSlowEnd(float rate) { m_fSlowEnd = rate; }			// �X���[�{��(�I��)�ݒ�
	float GetSlowEnd() { return m_fSlowEnd; }					// �X���[�{��(�I��)�擾

private:
	
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAction::* ACTION_FUNC)(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	static ACTION_FUNC m_ActionFunc[];	// �s���֐�

	typedef void(CPlayerAction::* START_FUNC)(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	static START_FUNC m_StartFunc[];	// �s���֐�

	typedef void(CPlayerAction::* END_FUNC)(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	static END_FUNC m_EndFunc[];		// �s���֐�

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �Ȃ�
	void ActionUnstable(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����Ƃ���
	void ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �u�����N
	void ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���
	void ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ����
	void ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �W�����v
	void ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`
	void ActionCatchJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`(�W�����v)
	void ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ����
	void ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����(�W�����v)
	void ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �X�y�V����

	// ��ԃX�^�[�g�n
	void StartCatchJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`(�W�����v)
	void StartThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ����(�W�����v)
	void StartDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ���

	// ��ԃG���h�n
	void EndDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ���
	void EndUnstable(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����Ƃ���

	// ���̑�
	void Debug();
	void UniqueJumpStart();		// �W�����v�n�A�N�V�����X�^�[�g
	void UniqueJumpUpdate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �W�����v�n�X�V

	//=============================
	// �����o�ϐ�
	//=============================
	bool m_bCharm;				// ���e�{�^�����
	CPlayer::EAction m_Action;	// �A�N�V����
	float m_fActionTime;		// �A�N�V��������
	CPlayer* m_pPlayer;			// �v���C���[�̃|�C���^

#if 1
	float m_fSlowStart;			// �X���[�{��(�ŏ�)
	float m_fSlowEnd;			// �X���[�{��(�ő�)
#endif	// �l�����p
};


#endif
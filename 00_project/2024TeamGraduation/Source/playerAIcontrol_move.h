//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�ړ��w�b�_�[ [playerAIcontrol_move.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_MOVE_H_
#define _PLAYERAI_CONTROL_MOVE_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// �O���錾
//==========================================================================
class CSuffocation;
class CEffekseerObj;
class CPlayerUserControlMove;

//==========================================================================
// �v���C���[AI�R���g���[��_�ړ��N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerAIControlMove : public CPlayerControlMove
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIControlMove();
	~CPlayerAIControlMove();

	void SetIsBlink(bool bBlink) { m_bBlink = bBlink; }
	void SetIsDash(bool bDash) { m_bDash = bDash; }
	void SetIsWalk(bool bWalk) { m_bWalk = bWalk; }

	void SetClabDirection(float clabDirection) { m_fClabDirection = clabDirection; }
	float GetClabDirection() { return m_fClabDirection; }	// �J�j�i�s�����̎擾

private:

	//=============================
	// ���z�֐�
	//=============================
	virtual CPlayerAIControlMove* GetAI() override { return this; }
	
	//=============================
	// �����o�֐�
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �u�����N
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �u�����N
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �E�H�[�N
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// �_�b�V�����̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
	bool m_bBlink;
	bool m_bDash;
	bool m_bWalk;
	float m_fClabDirection;	// �J�j�̂��񂱂�����
};


#endif
//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�ړ��w�b�_�[ [playerAIcontrol_move.h]
//  Author :Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_MODE_H_
#define _PLAYERAI_CONTROL_MODE_H_	// ��d�C���N���[�h�h�~

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
class CPlayerAIControlMode
{
public:

	enum EMode					// ���[�h
	{
		MODE_START = 0,			// ����
		MODE_IDLE,				// �ҋ@
		MODE_ATTACK,			// �U��
		MODE_DEFENCE,			// ���
		MODE_MAX
	};

	// �t���O�֘A
	enum EMoveFlag				// �s���t���O
	{
		MOVEFLAG_IDLE = 0,		// �~�܂�
		MOVEFLAG_WALK,			// ����
		MOVEFLAG_BLINK,			// �u�����N
		MOVEFLAG_DASH,			// ����
		MOVEFLAG_MAX
	};

	enum EActionFlag			// �A�N�V�����t���O
	{
		ACTION_NONE = 0,		// �Ȃ�
		ACTION_JUMP,			// �W�����v
		ACTION_MAX
	};

private:
	struct SParameter
	{
		float fDistanse;		// ����
		float fRadius;			// ���a

		float fJumpLength;
		float fLineLength;
		float fJumpEnd;			// �W�����v�I���ʒu
	};

public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIControlMode();
	~CPlayerAIControlMode();

	static CPlayerAIControlMode* Create(CPlayer* pPlayer, EMode mode);

	virtual HRESULT Init();
	void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// �ݒ�/�擾�֐�
	//=============================
	void SetPlayer(CPlayer* player) { m_pAI = player; }						// �����̃|�C���^�ݒ�
	CPlayer* GetPlayer() { return m_pAI; }									// �擾

	void SetMode(EMode mode) { m_eMode = mode; }							// ���[�h�̐ݒ�
	EMode GetMode() { return m_eMode; }										// �擾
	void SetMoveFlag(EMoveFlag move) { m_eMoveFlag = move; }				// �s���ݒ�
	EMoveFlag GetMoveFlag() { return m_eMoveFlag; }							// �擾
	void SetActionFlag(EActionFlag action) { m_eActionFlag = action; }		// �A�N�V�����t���O�ݒ�
	EActionFlag GetActionFlag() { return m_eActionFlag; }					// �擾

	void SetParameter(SParameter parameter) { m_sParameter = parameter; }	// �p�����[�^�ݒ�
	SParameter GetParameter() { return m_sParameter; }						// �擾

	void SetMaxjumpRate(float rate);										// �W�����v�͊����̐ݒ�
	float GetMaxjumpRate() { return m_fMaxJumpRate; }						// �擾
	float GetJumpRate();													// �W�����v�͊����̎擾

private:
	//=============================
	// �֐����X�g
	//=============================
	// �t���O�֘A�̊֐�
	typedef void(CPlayerAIControlMode::* ACTIONFLAG_FUNC)();
	static ACTIONFLAG_FUNC m_ActionFlagFunc[];				// �A�N�V�����t���O�֐�

	typedef void(CPlayerAIControlMode::* MOVEFLAG_FUNC)();
	static MOVEFLAG_FUNC m_MoveFlagFunc[];					// �s���t���O�֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	// �s���t���O
	void MoveFlagStop();			// �Ȃ�
	void MoveFlagWalk();			// ����
	void MoveFlagBlink();			// �u�����N
	void MoveFlagDash();			// ����

	// �A�N�V�����t���O
	void ActionFlagNone();			// �Ȃ�
	void ActionFlagJump();			// �W�����v

	//=============================
	// �����o�֐�
	//=============================
	void UpdateMoveFlag();			// �s���t���O�X�V
	void UpdateActionFlag();		// �A�N�V�����t���O�X�V

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAI;					// �����̃|�C���^

	float m_fMaxJumpRate;

	// ��
	EMode m_eMode;					// ���[�h�̗�
	EMoveFlag m_eMoveFlag;			// �s���̗�
	EActionFlag m_eActionFlag;		// �A�N�V�����̗�

	// �\����
	SParameter m_sParameter;
};

#endif
//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_H_
#define _PLAYERAICONTROL_H_	// ��d�C���N���[�h�h�~

#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayer;

//==========================================================================
// �萔��`
//==========================================================================
namespace playerAIcontrol
{
	// �����֘A
	const float THROW_JUMP_END = 130.0f;			// �W�����v�����̍ő�ʒu(�W�����v��MAX�F150)

	// ���z������(���S(x)����̋���)
	const float LINE_DISTANCE_OVER = 10.0f;		// ����������̋���
	const float RETURN_POS = 300.0f;			// �߂�ʒu
	const float OK_LENGTH = 50.0f;				// ����͈̔�(�ړI�Ƃ̋���)
}

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControl
{
public:
	//=============================
	// �񋓌^��`
	//=============================
	enum EHeartMain				// �S
	{
		HEART_MAIN_NORMAL = 0,	// �ʏ�
		HEART_MAIN_STRONG,		// ���C
		HEART_MAIN_TIMID,		// ��C
		HEART_MAIN_MAX
	};

	enum EMode					// ���[�h
	{
		MODE_IDLE = 0,			// �ҋ@
		MODE_ATTACK,			// �U��
		MODE_DEFENSE,			// ���
		MODE_MAX
	};

	enum EMoveForcibly			// �����s��
	{
		FORCIBLY_NONE = 0,		// �Ȃ�
		FORCIBLY_STOP,			// �~�܂�
		FORCIBLY_RETURN,		// �߂�
		FORCIBLY_START,			// ����
		FORCIBLY_MAX,
	};

	enum  EThrowType			// �����^�C�v
	{
		THROWTYPE_NONE = 0,		// �Ȃ�
		THROWTYPE_NORMAL,		// �ʏ�
		THROWTYPE_JUMP,			// �W�����v
		THROWTYPE_SPECIAL,		// �X�y�V����
		//THROWTYPE_PASS,			// �p�X
		THROWTYPE_MAX
	};

	enum EAction				// �A�N�V����
	{
		IDLE,					// �������Ȃ�
		DODGE,					// ����s��
		SUPPORT,				// �������T�|�[�g
		CHASE_BALL,				// �{�[����ǂ�������
		RETREAT,				// ��ށi���S�n�тɈړ��j
		RNDOM,					// �����_��
		MAX
	};

	enum ESupport
	{
		SUPPORT_NONE = 0,		// 
		SUPPORT_REBOUND,		// 
		SUPPORT_COVER,			// 
		SUPPORT_
	};

	// �t���O�֘A
	enum EThrowFlag				// �����t���O
	{
		THROW_NONE = 0,			// �Ȃ�
		THROW_NORMAL,			// ����
		THROW_PASS,				// �p�X
		THROW_SPECIAL,			// �X�y�V����
	};

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

	//=============================
	// �\���̒�`
	//=============================
	struct SThrow {				// �����֘A
		float fTiming;			// �^�C�~���O�J�E���g
		float fTimingRate;		// �^�C�~���O�̊���
		float fJumpEnd;			// �W�����v�̏I���ʒu
		bool bTiming;			// �^�C�~���O�t���O
		bool bFoldJump;			// �W�����v�̐܂�Ԃ��t���O
	};

	struct SMove {
		CPlayer* pDefenseTarget;	// ���Ώ�
		MyLib::Vector3 pos;			// �ʒu
		float fTimer;				// �^�C�}�[
		bool bSet;					// �ݒ�t���O
		bool bCancel;				// �L�����Z��
	};

	struct SParameter {			// �p�����[�^
		EHeartMain eHeartMain;	// �S(���C��)
		EHeartMain eHearDSub;	// �S(�T�u)
		int nMotivation;		// ���`�x�[�V����
		float fMove;			// �s��
		bool bSet;				// �ݒ�ON/OFF
	};

public:

	// �R���X�g���N�^
	CPlayerAIControl();

	// ����
	static CPlayerAIControl* Create(CPlayer* player);

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// �ݒ�,�擾
	//-----------------------------
	void SetPlayer(CPlayer* player) { m_pAI = player; }						// �����̐ݒ�
	CPlayer* GetPlayer() { return m_pAI; }									// �擾
	void SetAction(EAction action) { m_eAction = action; }
	EAction GetAction() { return m_eAction; }

	void SetMode(EMode mode) { m_eMode = mode; }							// ���[�h�ݒ�
	EMode GetMode() { return m_eMode; }										// �擾
	void SetForcibly(EMoveForcibly forcibly) { m_eForcibly = forcibly; }	// �����s���ݒ�
	EMoveForcibly GetForcibly() { return m_eForcibly; }						// �擾

	void SetMoveFlag(EMoveFlag move) { m_eMoveFlag = move; }				// �s���ݒ�
	EMoveFlag GetMoveFlag() { return m_eMoveFlag; }							// �擾
	void SetActionFlag(EActionFlag action) { m_eActionFlag = action; }		// �A�N�V�����ݒ�
	EActionFlag GetActionFlag() { return m_eActionFlag; }					// �擾
	void SetThrowFlag(EThrowFlag Throw) { m_eThrowFlag = Throw; }			// �����ݒ�
	EThrowFlag GetThrowFlag() { return m_eThrowFlag; }						// �擾

protected:
	//=============================
	// ���z�E�����֐�
	//=============================
	virtual bool IsLineOverBall() = 0;				// ����������(�{�[��)
	virtual bool IsLineOverPlayer() = 0;			// ���z������(�v���C���[)
	virtual void AttackDash(CPlayer* pTarget) = 0;	// ���蓊��

	//=============================
	// �����o�֐�
	//=============================
	bool Leave(MyLib::Vector3 targetPos, float distance);		// �����
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// �߂Â�

	// �s��
	void MoveIdle();		// �������Ȃ�
	void MoveUp();			// ��ړ�
	void MoveDown();		// ���ړ�
	void MoveLeft();		// ���ړ�
	void MoveRight();		// �E�ړ�


	//-----------------------------
	// �ݒ�֐�
	//-----------------------------
	CPlayer* GetThrowTarget();				// ������^�[�Q�b�g
	CPlayer* GetBallOwner();				// �{�[��������
	float GetDistance() { return m_fDistance; }

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];					// ���[�h�֐�

	typedef void(CPlayerAIControl::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];	// �����s���֐�

	typedef void(CPlayerAIControl::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];		// ������^�C�v�֐�

	typedef void(CPlayerAIControl::* MOVETYPE_FUNC)(const float, const float, const float);
	static MOVETYPE_FUNC m_MoveTypeFunc[];			// �s���֐�

	typedef void(CPlayerAIControl::* MOVESTATE_FUNC)(const float, const float, const float);
	static MOVESTATE_FUNC m_MoveStateFunc[];				// �L���b�`�֐�



	typedef void(CPlayerAIControl::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// �A�N�V�����֐�




	// �t���O�֘A�̊֐�
	typedef void(CPlayerAIControl::* ACTIONFLAG_FUNC)();
	static ACTIONFLAG_FUNC m_ActionFlagFunc[];				// �A�N�V�����t���O�֐�

	typedef void(CPlayerAIControl::* THROWFLAG_FUNC)();
	static THROWFLAG_FUNC m_ThrowFlagFunc[];				// �����t���O�֐�

	typedef void(CPlayerAIControl::* MOVEFLAG_FUNC)();
	static MOVEFLAG_FUNC m_MoveFlagFunc[];				// �s���t���O�֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	// ���[�h
	void ModeIdle(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �ҋ@
	void ModeAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �U��
	void ModeDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���

	// �����s��
	void ForciblyNone() {};				// �Ȃ�
	void ForciblyStop();				// �~�܂�
	virtual void ForciblyReturn() = 0;	// �߂�
	void ForciblyStart();				// ����

	// �����^�C�v
	void ThrowTypeNone() {};		// �Ȃ�
	void ThrowTypeNormal();			// �ʏ�
	void ThrowTypeJump();			// �W�����v
	void ThrowTypeSpecial();		// �X�y�V����

	// �A�N�V����
	void MoveDodge();				// ���
	void MoveSupport();				// �T�|�[�g
	void MoveChaseBall();			// �{�[����ǂ�������
	virtual void MoveRetreat() = 0;				// ���
	void MoveRandom();				// �����_��

	// �s���t���O
	void MoveFlagStop();			// �Ȃ�
	void MoveFlagWalk();			// ����
	void MoveFlagBlink();			// �u�����N
	void MoveFlagDash();			// ����

	// �A�N�V�����t���O
	void ActionFlagNone();			// �Ȃ�
	void ActionFlagJump();			// �W�����v

	// �����t���O
	void ThrowFlagNone() {};		// �Ȃ�
	void ThrowFlag();				// ����
	void ThrowFlagPass();			// �p�X
	void ThrowFlagSpecial();		// �X�y�V����

	//=============================
	// �����o�֐�
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���[�h�Ǘ�
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ���[�h�X�V
	void UpdateForcibly();																			// �����s��
	void UpdateThrowType();																			// �������
	void UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`

	void MoveCover(CPlayer* pPlayer);

	// �t���O�X�V
	void UpdateMoveFlag();			// �s��
	void UpdateActionFlag();		// �A�N�V����
	void UpdateThrowFlag();			// �����t���O

	void BallSteal();		// �{�[����D��
	void BallChase();		// �ڂ��ǂ�

	void SeeTarget(MyLib::Vector3 pos);		// �^�[�Q�b�g���݂�
	void SeeBall();							// �{�[��������

	void InitHeart();		// �S�̏�����
	void UpdateParameter();	// �p�����[�^

	//-----------------------------
	// �ݒ�,�擾
	//-----------------------------
	void SetMoveTimer(int nMin, int nMax);
	void UpdateMoveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	float GetDistanceBall();				// �{�[���Ƃ̋���
	float GetDistanceEnemy();				// �G�Ƃ̋���
	float GetDistanceBallowner();			// �{�[��������Ƃ̋���

	//-----------------------------
	// ����
	//-----------------------------
	bool IsDistanceBall();					// �����F�{�[��
	bool IsPassTarget();					// �p�X���鑊�肪���邩����
	bool IsWhoPicksUpTheBall();				// �{�[�����E�����f

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAI;					// �������
	float m_fDistance;				// ����Ƃ̋���

	//-----------------------------
	// ��
	//-----------------------------
	EMode m_eMode;					// ���[�h
	EMoveForcibly m_eForcibly;		// �����s��
	EThrowType m_eThrowType;		// �������
	EAction m_eAction;				// �A�N�V����

	EMoveFlag m_eMoveFlag;			// �s���t���O
	EActionFlag m_eActionFlag;		// �A�N�V�����t���O
	EThrowFlag m_eThrowFlag;		// �����t���O

	//-----------------------------
	// �\����
	//-----------------------------
	SThrow m_sThrow;				// ����
	SMove m_sMove;					// �s��
	SParameter m_sParameter;		// �p�����[�^	
};

#endif

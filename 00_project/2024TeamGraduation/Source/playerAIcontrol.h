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
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControl
{
public:
	//=============================
	// �񋓌^��`
	//=============================
	enum EHeart					// �S
	{
		HEART_NONE = 0,			// �����k�l
		HEART_NORMAL,			// �ʏ�
		HEART_STRONG,			// ���C
		HEART_TIMID,			// ��C
		HEART_MAX
	};

	enum EMode					// ���[�h
	{
		MODE_NONE = 0,			// �Ȃ�
		MODE_THROW,				// ����
		MODE_CATCH,				// �L���b�`
		MODE_MAX
	};

	enum EThrowTiming				// �^�C�~���O
	{
		TIMING_NONE = 0,
		TIMING_NORMAL,			// �ʏ�
		TIMING_QUICK,			// ��
		TIMING_DELAY,			// �x
		TIMING_JUMP_NORMAL,		// �W�����v�ʏ�
		TIMING_JUMP_QUICK,		// �W�����v��
		TIMING_JUMP_DELAY,		// �W�����v�x
		TIMING_MAX
	};

	enum  EThrowType			// �����^�C�v
	{
		THROWTYPE_NONE = 0,		// �Ȃ�
		THROWTYPE_NORMAL,		// �ʏ�
		THROWTYPE_JUMP,			// �W�����v
		THROWTYPE_SPECIAL,		// �X�y�V����
		THROWTYPE_PASS,			// �p�X
		THROWTYPE_MAX
	};

	enum EThrow					// �������
	{
		THROW_NONE = 0,			// �Ȃ�
		THROW_NORMAL,			// ����
		THROW_PASS,				// �p�X
		THROW_SPECIAL,			// �X�y�V����
	};

	enum ECatchType				// �L���b�`���
	{
		CATCH_TYPE_NONE = 0,	// �Ȃ�
		CATCH_TYPE_NORMAL,		// �ʏ�
		CATCH_TYPE_JUST,		// �W���X�g
		CATCH_TYPE_DASH,		// �_�b�V��
		CATCH_TYPE_PASS_GIVE,	// �p�X��Ⴄ
		CATCH_TYPE_PASS_STEAL,	// �p�X��D��
		CATCH_TYPE_FIND,		// ���ɍs��
		CATCH_TYPE_MAX
	};

	enum EMoveForcibly			// �����s��
	{
		FORCIBLY_NONE = 0,		// �Ȃ�
		FORCIBLY_STOP,			// �~�܂�
		FORCIBLY_RETURN,		// �߂�
		FORCIBLY_START,			// ����
		FORCIBLY_MAX,
	};

	enum EMoveType				// �s�����
	{
		MOVETYPE_STOP = 0,		// �~�܂�
		MOVETYPE_WALK,			// ����
		MOVETYPE_DASH,			// ����
		MOVETYPE_MAX
	};

	enum EAction				// �A�N�V����
	{
		ACTION_NONE = 0,		// �Ȃ�
		ACTION_JUMP,			// �W�����v
		ACTION_MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	struct SThrow
	{
		float fTiming;		// �^�C�~���O�J�E���g
		float fTimingRate;	// �^�C�~���O�̊���
		float fJumpEnd;		// �W�����v�̏I���ʒu
		bool bTiming;		// �^�C�~���O�t���O
		bool bFoldJump;		// �W�����v�̐܂�Ԃ��t���O
	};

public:

	// �R���X�g���N�^
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	virtual HRESULT Init() = 0;
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

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

	//-----------------------------
	// �ݒ�B�擾
	//-----------------------------
	CPlayer* GetThrowTarget();	// ������^�[�Q�b�g
	CPlayer* GetCatchTarget();	// �L���b�`�^�[�Q�b�g

	void SetMode(EMode mode) { m_eMode = mode; }	// ���[�h�ݒ�
	EMode GetMode() { return m_eMode; }
	void SetForcibly(EMoveForcibly forcibly) { m_eForcibly = forcibly; }
	EMoveForcibly GetForcibly() { return m_eForcibly; }
	void SetMove(EMoveType move) { m_eMove = move; }
	EMoveType GetMove() { return m_eMove; }
	void SetAction(EAction action) { m_eAction = action; }
	void SetThrow(EThrow a) { m_eThrow = a; }

	void SetPlayer(CPlayer* player) { m_pAI = player; }
	CPlayer* GetPlayer() { return m_pAI; }

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// ���[�h�֐�

	typedef void(CPlayerAIControl::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];	// �����s���֐�

	typedef void(CPlayerAIControl::* MOVE_FUNC)();
	static MOVE_FUNC m_MoveFunc[];					// �s���֐�

	typedef void(CPlayerAIControl::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// �A�N�V�����֐�

	typedef void(CPlayerAIControl::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];			// ������^�C�v�֐�

	typedef void(CPlayerAIControl::* THROWMOVE_FUNC)();
	static THROWMOVE_FUNC m_ThrowMoveFunc[];		// ������܂ł̍s���֐�

	typedef void(CPlayerAIControl::* THROW_FUNC)();
	static THROW_FUNC m_ThrowFunc[];				// �����^�C�~���O�֐�

	typedef void(CPlayerAIControl::* THROWTIMING_FUNC)(const float, const float, const float);
	static THROWTIMING_FUNC m_ThrowTimingFunc[];	// �����^�C�~���O�֐�

	typedef void(CPlayerAIControl::* CATCH_FUNC)();
	static CATCH_FUNC m_CatchFunc[];				// �L���b�`�֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	// ���[�h
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`����

	// �����s��
	void ForciblyNone() {};			// �Ȃ�
	void ForciblyStop();			// �~�܂�
	virtual void ForciblyReturn() = 0;			// �߂�
	void ForciblyStart();			// ����

	// �s��
	void MoveStop();			// �Ȃ�
	void MoveWalk();			// ����
	void MoveDash();			// ����

	// �A�N�V����
	void ActionNone();
	void ActionJump();

	// �����^�C�v
	void ThrowTypeNone() {};
	void ThrowTypeNormal();
	void ThrowTypeJump();
	void ThrowTypeSpecial();

	// ����
	void ThrowNone() {};
	void Throw();
	void ThrowPass();
	void ThrowSpecial();

	// ������܂ł̍s��
	void ThrowMoveNone();
	void ThrowMoveWalk();
	void ThrowMoveDash();

	// �����^�C�~���O
	void ThrowTimingNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �L���b�`
	void CatchNone() {};
	void CatchNormal();
	void CatchJust();
	void CatchDash();
	void CatchPassGive();
	void CatchPassSteal();
	void CatchFindBall();

	//=============================
	// �����o�֐�
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void UpdateMode();			// ���[�h
	void UpdateForcibly();		// �����s��
	void UpdateMove();			// �s��
	void UpdateAction();		// �A�N�V����
	void UpdateThrowType();		// �������
	void UpdateThrowMove();		// �����s��
	void UpdateThrow();			// ����
	void UpdateThrowTiming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����^�C�~���O
	void UpdateCatch();			// �L���b�`

	void UpdateSee();

	void PlanThrow();
	void PlanHeart();		// �S�̃v����

	void CatchDistance(CPlayer* pTarget);	// �����F�L���b�`���
	bool IsPassTarget();					// �p�X���鑊�肪���邩����
	bool IsWhoPicksUpTheBall();				// �{�[�����E�����f

	void SeeTarget(MyLib::Vector3 pos);		// �^�[�Q�b�g���݂�
	void SeeBall();							// �{�[��������

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAI;					// �������

	//-----------------------------
	// ��
	//-----------------------------
	EMode m_eMode;					// ���[�h
	EMoveForcibly m_eForcibly;		// �����s��
	EMoveType m_eMove;				// �s���^�C�v
	EHeart m_eHeart;				// �S
	EAction m_eAction;				// �A�N�V����
	EThrowType m_eThrowType;		// �������
	EThrow m_eThrow;				// ����
	EThrowTiming m_eThrowTiming;	// �����^�C�~���O
	ECatchType m_eCatchType;		// �L���b�`

	//-----------------------------
	// �\����
	//-----------------------------
	SThrow m_sThrow;				// ����
};

#endif

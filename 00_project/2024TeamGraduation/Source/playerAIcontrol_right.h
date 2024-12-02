//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_RIGHT_H_
#define _PLAYERAICONTROL_RIGHT_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "playerAIcontrol.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayer;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControlRight : public CPlayerAIControl
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EMode	// ���[�h
	{
		MODE_NONE = 0,	// �Ȃ�
		MODE_THROW,		// ����
		MODE_CATCH,		// �L���b�`
		MODE_MAX
	};

private:
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

	enum ETiming				// �^�C�~���O
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
		THROWTYPE_MAX
	};

	enum ECatchType				// �L���b�`���
	{
		CATCH_TYPE_NONE = 0,	// �Ȃ�
		CATCH_TYPE_NORMAL,		// �ʏ�
		CATCH_TYPE_JUST,		// �W���X�g
		CATCH_TYPE_DASH,		// �_�b�V��
		CATCH_TYPE_PASS,		// �p�X
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

	enum EThrow				// �������
	{
		THROW_NONE = 0,		// �Ȃ�
		THROW_NORMAL,		// ����
		THROW_PASS,			// �p�X
		THROW_SPECIAL,		// �X�y�V����
	};

	enum EAction	// �A�N�V����
	{
		ACTION_NONE = 0,
		ACTION_JUMP,
		ACTION_MAX
	};

	enum ELine
	{
		LINE_IN = 0,	// ����
		LINE_OVER,		// ���O
	};

	//=============================
	// �\���̒�`
	//=============================
	struct SThrowInfo	// �������
	{
		EThrowType eType;	// �^�C�v
		ETiming eTiming;	// �^�C�~���O

		float fTiming;		// �^�C�~���O�J�E���g
		float fTimingRate;	// �^�C�~���O�̊���
		float fJumpEnd;		// �W�����v�̏I���ʒu
		bool bTiming;		// �^�C�~���O�t���O
		bool bFoldJump;		// �W�����v�̐܂�Ԃ��t���O
		bool bThrow;		// �����Ă悵�I
	};

	struct SInfo
	{
		SThrowInfo sThrowInfo;		// �������
		EMode eMode;				// ���[�h
		ECatchType eCatchType;		// �L���b�`���
	};

	struct STarget	// �w�K
	{
		float fDistanceIN;		// ����Ƃ̋���
		float fDistanceOUT;		// �O��Ƃ̋���
	};

public:

	// �R���X�g���N�^
	CPlayerAIControlRight();

	static CPlayerAIControlRight* Create(CPlayer* player);

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	void SetMode(EMode mode) { m_sInfo.eMode = mode; }	// ���[�h�ݒ�
	void SetPlayerInfo(CPlayer* player) { m_pAI = player; }

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControlRight::* AREA_FUNC)();
	static AREA_FUNC m_AreaFunc[];			// ���[�h�֐�

	typedef void(CPlayerAIControlRight::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// ���[�h�֐�

	typedef void(CPlayerAIControlRight::* THROWTYPE_FUNC)(CPlayer*, const float, const float, const float);
	static THROWTYPE_FUNC m_ThrowTypeFunc[];			// ������^�C�v�֐�

	typedef void(CPlayerAIControlRight::* THROWMOVE_FUNC)(CPlayer*, const float, const float, const float);
	static THROWMOVE_FUNC m_ThrowMoveFunc[];			// ������܂ł̍s���֐�

	typedef void(CPlayerAIControlRight::* THROWTIMING_FUNC)(CPlayer*, const float, const float, const float);
	static THROWTIMING_FUNC m_ThrowTimingFunc[];			// �����^�C�~���O�֐�

	typedef void(CPlayerAIControlRight::* THROW_FUNC)();
	static THROW_FUNC m_ThrowFunc[];			// �����^�C�~���O�֐�

	typedef void(CPlayerAIControlRight::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];			// �L���b�`�֐�

	typedef void(CPlayerAIControlRight::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];			// �s���֐�

	typedef void(CPlayerAIControlRight::* MOVE_FUNC)();
	static MOVE_FUNC m_MoveFunc[];			// �s���֐�

	typedef void(CPlayerAIControlRight::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];			// �s���֐�

	//=============================
	// �����o�֐�
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���[�h�Ǘ�
	void MoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �s���Ǘ�
	void ActionManager();
	void ThrowManager();

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void AreaNone();
	void AreaLeft();
	void AreaRight();

	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`����

	// �����^�C�v
	void ThrowTypeNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// ������܂ł̍s��
	void ThrowMoveNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �����^�C�~���O
	void ThrowTimingNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �L���b�`
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void ThrowNone() {};
	void Throw();
	void ThrowPass();
	void ThrowSpecial();
	//void ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �����s��
	void ForciblyNone() {};			// �Ȃ�
	void ForciblyStop();			// �~�܂�
	void ForciblyReturn();			// �߂�
	void ForciblyStart();			// ����

	// �s��
	void MoveStop();			// �Ȃ�
	void MoveWalk();			// ����
	void MoveDash();			// ����

	// �A�N�V����
	void ActionNone();
	void ActionJump();

	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	CPlayer* GetThrowTarget();		// ������^�[�Q�b�g
	CPlayer* GetCatchTarget();		// �L���b�`�^�[�Q�b�g

	void PlanHeart();		// �S�̃v����
	void PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ������v����

	void CatchDistance(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����F�L���b�`���
	void CatchOutDistance();				// �����F�O��
	void CatchLineLeftDistance();			// �p�X���鑊��
	void PlanIsJump(CPlayer* pTarget);		// ���Ԃ��ǂ���
	void JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Timing(CPlayer* pTarget);			// �^�C�~���O�̎v�l

	bool IsPassTarget();					// �p�X���鑊�肪���邩����
	bool IsWhoPicksUpTheBall();				// �{�[�����E�����f
	bool IsLineOverBall();					// ����������(�{�[��)
	bool IsLineOverPlayer();				// ���z������(�v���C���[)

	bool Leave(MyLib::Vector3 targetPos, float distance);		// �����
	void LeaveOut(float distance);								// �����
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// �߂Â�

	void AttackDash();

	void IsJumpCatch();

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAI;				// �������
	CPlayer* m_pTarget;			// �^�[�Q�b�g���

	SInfo m_sInfo;				// ���[�h���
	STarget m_sTarget;			// �^�[�Q�b�g���
	EMoveForcibly m_eForcibly;	// �����s��
	EMoveType m_eMove;			// �s���^�C�v
	ELine m_eLine;				// ��
	EHeart m_eHeart;			// �S
	EAction m_eAction;
	EThrow m_eThrow;

	bool m_bStart;
	bool m_bEnd;
};

#endif

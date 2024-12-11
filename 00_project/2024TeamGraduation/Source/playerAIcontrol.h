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
	enum EHeart					// �S
	{
		HEART_NONE = 0,			// �����k�l
		HEART_NORMAL,			// �ʏ�
		HEART_STRONG,			// ���C
		HEART_TIMID,			// ��C
		HEART_CRAYZY,			// ���l
		HEART_MAX
	};

	enum EMode					// ���[�h
	{
		MODE_NONE = 0,			// �Ȃ�
		MODE_THROW,				// ����
		MODE_CATCH,				// �L���b�`
		MODE_MAX
	};

	enum EThrowTiming			// �^�C�~���O
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

	enum EMoveTypeChatch
	{
		MOVETYPE_NONE = 0,		// �Ȃ�
		MOVETYPE_DISTANCE,		// ���������
		MOVETYPE_RANDOM,		// �����_��
		MOVETYPE_LEFT,			// ��
		MOVETYPE_RIGHT,			// �E
		MOVETYPE_FORWARD,		// �O��
		MOVETYPE_BACK,			// ���
		MOVETYPE_MAX
	};

	enum EMoveForcibly			// �����s��
	{
		FORCIBLY_NONE = 0,		// �Ȃ�
		FORCIBLY_STOP,			// �~�܂�
		FORCIBLY_RETURN,		// �߂�
		FORCIBLY_START,			// ����
		FORCIBLY_MAX,
	};

	enum ESee
	{
		SEE_NONE = 0,
		SEE_BALL,
		SEE_TARGET,
		SEE_MAX
	};

	enum EThrowFlag				// �����t���O
	{
		THROW_NONE = 0,			// �Ȃ�
		THROW_NORMAL,			// ����
		THROW_PASS,				// �p�X
		THROW_SPECIAL,			// �X�y�V����
	};

	enum EMoveFlag				// �s���t���O
	{
		MOVEFLAG_STOP = 0,		// �~�܂�
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

	//=============================
	// �\���̒�`
	//=============================
	struct SThrow			// �����֘A
	{
		float fTiming;		// �^�C�~���O�J�E���g
		float fTimingRate;	// �^�C�~���O�̊���
		float fJumpEnd;		// �W�����v�̏I���ʒu
		bool bTiming;		// �^�C�~���O�t���O
		bool bFoldJump;		// �W�����v�̐܂�Ԃ��t���O
	};

	struct SMove
	{
		float fRot;		// �����̃����h
		float fTimer;		// �s���^�C�}�[
		bool bSet;			// �ݒ芮�����Ă邩
	};

	struct SDistance		// ����
	{
		float fInPair;		// ����F����
		float fInAlly;		// ����F����
		float fOut;			// �O��
		float fTarget;		// �^�[�Q�b�g
	};

public:

	// �R���X�g���N�^
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// �ݒ�,�擾
	//-----------------------------
	CPlayer* GetThrowTarget();												// ������^�[�Q�b�g�擾
	CPlayer* GetBallOwner();												// �{�[��������擾
	void SetMode(EMode mode) { m_eMode = mode; }							// ���[�h�ݒ�
	EMode GetMode() { return m_eMode; }										// �擾
	void SetForcibly(EMoveForcibly forcibly) { m_eForcibly = forcibly; }	// �����s���ݒ�
	EMoveForcibly GetForcibly() { return m_eForcibly; }						// �擾
	void SetMove(EMoveFlag move) { m_eMoveFlag = move; }					// �s���ݒ�
	EMoveFlag GetMove() { return m_eMoveFlag; }								// �擾
	void SetAction(EActionFlag action) { m_eActionFlag = action; }			// �A�N�V�����ݒ�
	EActionFlag GetAction() { return m_eActionFlag; }						// �擾
	void SetThrow(EThrowFlag Throw) { m_eThrow = Throw; }					// �����ݒ�
	EThrowFlag GetThrow() { return m_eThrow; }								// �擾
	void SetPlayer(CPlayer* player) { m_pAI = player; }						// �����̐ݒ�
	CPlayer* GetPlayer() { return m_pAI; }									// �擾


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

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];					// ���[�h�֐�

	typedef void(CPlayerAIControl::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];	// �����s���֐�

	typedef void(CPlayerAIControl::* MOVEFLAG_FUNC)();
	static MOVEFLAG_FUNC m_MoveFlagFunc[];				// �s���֐�

	typedef void(CPlayerAIControl::* MOVETYPE_FUNC)(const float, const float, const float);
	static MOVETYPE_FUNC m_MoveTypeFunc[];				// �s���֐�

	typedef void(CPlayerAIControl::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// �A�N�V�����֐�

	typedef void(CPlayerAIControl::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];		// ������^�C�v�֐�

	typedef void(CPlayerAIControl::* THROWMOVE_FUNC)();
	static THROWMOVE_FUNC m_ThrowMoveFunc[];		// ������܂ł̍s���֐�

	typedef void(CPlayerAIControl::* THROW_FUNC)();
	static THROW_FUNC m_ThrowFunc[];				// �����^�C�~���O�֐�

	typedef void(CPlayerAIControl::* THROWTIMING_FUNC)(const float, const float, const float);
	static THROWTIMING_FUNC m_ThrowTimingFunc[];	// �����^�C�~���O�֐�

	typedef void(CPlayerAIControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];				// �L���b�`�֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	// ���[�h
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`����

	// �����s��
	void ForciblyNone() {};				// �Ȃ�
	void ForciblyStop();				// �~�܂�
	virtual void ForciblyReturn() = 0;	// �߂�
	void ForciblyStart();				// ����

	// �s��
	void MoveFlagStop();			// �Ȃ�
	void MoveFlagWalk();			// ����
	void MoveFlagBlink();			// �u�����N
	void MoveFlagDash();			// ����

	// �s���^�C�v
	void MoveTypeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};	// �Ȃ�
	void MoveTypeDistance(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���������
	void MoveTypeAtyakotya(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �������Ⴑ������
	void MoveTypeLeft(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���E
	void MoveTypeRight(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���E
	void MoveTypeUp(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �㉺
	void MoveTypeDown(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �㉺

	// �A�N�V����
	void ActionNone();			// �Ȃ�
	void ActionJump();			// �W�����v

	// �����^�C�v
	void ThrowTypeNone() {};	// �Ȃ�
	void ThrowTypeNormal();		// �ʏ�
	void ThrowTypeJump();		// �W�����v
	void ThrowTypeSpecial();	// �X�y�V����

	// ����
	void ThrowNone() {};		// �Ȃ�
	void Throw();				// ����
	void ThrowPass();			// �p�X
	void ThrowSpecial();		// �X�y�V����

	// ������܂ł̍s��
	void ThrowMoveNone();		// �Ȃ�
	void ThrowMoveWalk();		// ����
	void ThrowMoveDash();		// ����

	// �����^�C�~���O
	void ThrowTimingNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �L���b�`
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchPassGive(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchPassSteal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// �����o�֐�
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ���[�h
	void UpdateForcibly();		// �����s��
	void UpdateMoveFlag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �s��
	void UpdateMoveType(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �s��
	void UpdateActionFlag();		// �A�N�V����
	void UpdateThrowType();		// �������
	void UpdateThrowMove();		// �����s��
	void UpdateThrowFlag();			// ����
	void UpdateThrowTiming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����^�C�~���O
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`
	void UpdateSee();			// ����

	void PlanThrow();			// �����̃v����
	void PlanHeart();			// �S�̃v����

	bool IsDistanceBall();					// �����F�{�[��
	bool IsPassTarget();					// �p�X���鑊�肪���邩����
	bool IsWhoPicksUpTheBall();				// �{�[�����E�����f
	void SeeTarget(MyLib::Vector3 pos);		// �^�[�Q�b�g���݂�
	void SeeBall();							// �{�[��������
	float GetDistance(CPlayer::EFieldArea area, CGameManager::ETeamSide teamMy, CGameManager::ETeamSide teamPair);
	float GetDistanceBallowner();

	void SetMoveTimer(float timer);

	void Parameter();

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pAI;					// �������

	//-----------------------------
	// ��
	//-----------------------------
	EMode m_eMode;					// ���[�h
	EMoveForcibly m_eForcibly;		// �����s��
	EMoveFlag m_eMoveFlag;			// �s���t���O
	EMoveTypeChatch m_eMoveType;			// �s���^�C�v
	EHeart m_eHeart;				// �S
	EActionFlag m_eActionFlag;		// �A�N�V�����t���O
	EThrowType m_eThrowType;		// �������
	EThrowFlag m_eThrow;			// ����
	EThrowTiming m_eThrowTiming;	// �����^�C�~���O
	ECatchType m_eCatchType;		// �L���b�`
	ESee m_eSee;					// ����

	//-----------------------------
	// �\����
	//-----------------------------
	SThrow m_sThrow;				// ����
	SMove m_sMove;					// �s��
	SDistance m_sDistance;			// ����
};

#endif

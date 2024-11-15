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
	enum EHeart	// �S
	{
		HEART_NONE = 0,	// �����k�l
		HEART_NORMAL,	// �ʏ�
		HEART_STRONG,	// ���C
		HEART_TIMID,	// ��C
		HEART_MAX
	};

	enum ETiming	// �^�C�~���O
	{
		TIMING_NORMAL = 0,	// �ʏ�
		TIMING_QUICK,		// ��
		TIMING_DELAY,		// �x
		TIMING_MAX
	};

	enum  EThrowType	// �����^�C�v
	{
		THROWTYPE_NORMAL = 0,
		THROWTYPE_JUMP,
		THROWTYPE_SPECIAL,
		THROWTYPE_MAX
	};

	enum ECatchType	// �L���b�`���
	{
		CATCH_TYPE_NONE = 0,	// �Ȃ�
		CATCH_TYPE_NORMAL,		// �ʏ�
		CATCH_TYPE_JUST,		// �W���X�g
		CATCH_TYPE_DASH,		// �_�b�V��
		CATCH_TYPE_FIND,		// ���ɍs��
		CATCH_TYPE_MAX
	};

	enum EMoveType	// �s�����
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_WALK,			// ����
		MOVETYPE_DASH,			// ����
		MOVETYPE_LEAVE,			// �����
		MOVETYPE_APPROATCH,		// �߂Â�
		MOVETYPE_MAX
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

	struct SCatchInfo	// �L���b�`���
	{
		ECatchType eCatchType;		// �L���b�`���
	};

	struct SMoveInfo	// �s��
	{
		EMoveType eType;	// �^�C�v
		bool bJump;			// �W�����v�t���O
	};

	struct SMode	// ���[�h
	{
		EMode eMode;				// ���[�h
	};

	struct SInfo
	{
		SMode sMode;				// ���[�h
		EHeart eHeart;				// �S
		SThrowInfo sThrowInfo;		// �������
		SCatchInfo sCatchInfo;		// �L���b�`���
		SMoveInfo sMoveInfo;		// �s�����
	};

public:

	// �R���X�g���N�^
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetMode(EMode mode) { m_sInfo.sMode.eMode = mode; }	// ���[�h�ݒ�
	void SetPlayerInfo(CPlayer* player) { m_pAI = player; }

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// ���[�h�֐�

	typedef void(CPlayerAIControl::* THOWTYPE_FUNC)(CPlayer*, const float, const float, const float);
	static THOWTYPE_FUNC m_ThrowTypeFunc[];			// ������^�C�v�֐�

	typedef void(CPlayerAIControl::* THOWMOVE_FUNC)(CPlayer*, const float, const float, const float);
	static THOWMOVE_FUNC m_ThrowMoveFunc[];			// ������܂ł̍s���֐�

	typedef void(CPlayerAIControl::* THOWTIMING_FUNC)(CPlayer*, const float, const float, const float);
	static THOWTIMING_FUNC m_ThrowTimingFunc[];			// �����^�C�v�֐�

	typedef void(CPlayerAIControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];		// �L���b�`�֐�

	//=============================
	// �����o�֐�
	//=============================
	void ModeManager();
	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`����

	// �����^�C�v
	void ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	// ������܂ł̍s��
	void ThrowMoveStop(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �����^�C�~���O
	void ThrowJumpTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowJumpTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowJumpTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �L���b�`
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void SetThrowTarget(CPlayer** ppTarget = nullptr);		// ������^�[�Q�b�g
	void DistanceLeaveCatch();		// �L���b�`����(�����)
	void DistanceApproachCatch();	// �L���b�`����(�߂Â�)

	void PlanThrowFlow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����闬��
	void PlanHeart();		// �S�̃v����
	void PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ������v����
	void PlanThrowDistance(CPlayer* pTarget);	// �����鋗���v����
	void PlanIsJump(CPlayer* pTarget);	// ���Ԃ��ǂ���
	void PlanMove(CPlayer* pTarget);	// �s���v����

	bool IsLineOverBall();	// ����������(�{�[��)
	void JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void StrategyTiming(CPlayer* pTarget);	// �^�C�~���O�̎v�l
	void StrategyLine(CPlayer* pTarget);
	void LineLeftTeam(CPlayer* pTarget);
	void LineRightTeam(CPlayer* pTarget);

	void Reset();			// �ϐ����Z�b�g

	bool IsWhoPicksUpTheBall();

	void RunUp(CPlayer* pTarget);

	void Debug();

	//=============================
	// �����o�ϐ�
	//=============================
	SInfo m_sInfo;		// ���[�h���
	CPlayer* m_pAI;			// �������g

	float m_fDistance;			// �^�[�Q�b�g�Ƃ̋���
	bool m_bStart;
};

#endif

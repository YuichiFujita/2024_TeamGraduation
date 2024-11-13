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
	enum EHeart
	{
		HEART_NONE = 0,	// �����k�l
		HEART_NORMAL,	// �ʏ�
		HEART_STRONG,	// ���C
		HEART_TIMID,	// ��C
		HEART_MAX
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
		MOVETYPE_DISTANCE,
		MOVETYPE_MAX
	};

public:

	// �R���X�g���N�^
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetMode(EMode mode) { m_eMode = mode; }	// ���[�h�ݒ�
	void SetPlayerInfo(CPlayer* player) { m_pAI = player; }

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// ���[�h�֐�

	typedef void(CPlayerAIControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];		// �����^�C�~���O�֐�

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

	// �L���b�`
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void Reset();			// �ϐ����Z�b�g
	void SetThrowTarget(CPlayer** ppTarget = nullptr);		// ������^�[�Q�b�g
	void DistanceThrow();	// �����鋗��
	void DistanceCatch();	// �L���b�`����

	void PlanThrowFlow();	// �����闬��
	void PlanHeart();		// �S�̃v����
	void PlanThrow(CPlayer* pTarget);		// ������v����
	void PlanThrowDistance(CPlayer* pTarget);	// �����鋗���v����
	void PlanIsJump(CPlayer* pTarget);	// ���Ԃ��ǂ���
	void PlanMove(CPlayer* pTarget);	// �s���v����

	void LineDistance();	// ���Ƃ̋���
	void LineOverPlayer();	// ������
	bool IsLineOverBall();	// ����������(�{�[��)


	//=============================
	// �����o�ϐ�
	//=============================
	EMode m_eMode;					// ���[�h
	EHeart m_eHeart;				// �S�̎��
	ECatchType m_eCatchType;		// �L���b�`���
	EMoveType m_eMoveType;			// �s�����
	float m_fTiming;		// �^�C�~���O�J�E���g
	float m_fTimingRate;	// �^�C�~���O�̊���
	bool m_bTiming;			// �^�C�~���O�t���O
	bool m_bFoldJump;		// �W�����v�̐܂�Ԃ��t���O
	float m_fJumpEnd;		// �W�����v�̏I���ʒu

	CPlayer* m_pAI;

	bool m_bGOThrow;		// �����Ă悵�I
};

#endif

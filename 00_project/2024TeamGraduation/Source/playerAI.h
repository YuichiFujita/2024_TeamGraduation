//==========================================================================
// 
//  AI�v���C���[�w�b�_�[ [playerAI.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_		// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)

//==========================================================================
// �N���X��`
//==========================================================================
// AI�v���C���[�N���X
class CPlayerAI : public CPlayerBase
{
private:

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

	enum EThrowType	// �����̎��
	{
		TYPE_NONE = 0,	// �Ȃ�
		TYPE_NORMAL,	// �ʏ퓊��
		TYPE_JUMP,		// �W�����v����
		TYPE_SPECIAL,	// �X�y�V��������
		TYPE_MAX
	};

	enum EThrowMove	// �����̏��
	{
		MOVE_NORMAL = 0,	// �ʏ�
		MOVE_WALK,			// ����
		MOVE_DASH,			// ����
		MOVE_MAX
	};

	enum EThrowTiming	// �����̃^�C�~���O
	{
		TIMING_NORMAL = 0,	// �ʏ�
		TIMING_FEINT,		// �t�F�C���g
		TIMING_JUMP_NORMAL,	// �ʏ�(�W�����v)
		TIMING_JUMP_QUICK,	// ����(�W�����v)
		TIMING_JUMP_DELAY,	// �x��(�W�����v)
		TIMING_JUMP_FEINT,	// �t�F�C���g(�W�����v)
		TIMING_MAX
	};

	enum ECatchType
	{
		CATCH_TYPE_NONE = 0,	// �Ȃ�
		CATCH_TYPE_NORMAL,		// �ʏ�
		CATCH_TYPE_JUST,		// �W���X�g
		CATCH_TYPE_DASH,		// �_�b�V��
		CATCH_TYPE_FIND,		// ���ɍs��
		CATCH_TYPE_MAX
	};

public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAI(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea);
	~CPlayerAI();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// �q�b�g
	virtual void Debug() override;	// �f�o�b�O

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerAIControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerAIControlAction* control);	// �A�N�V�����̑���ύX

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAI::* MODE_FUNC)(const float , const float, const float);
	static MODE_FUNC m_ModeFunc[];			// ���[�h�֐�

	typedef void(CPlayerAI::* TYPE_FUNC)(const float, const float, const float);
	static TYPE_FUNC m_ThrowTypeFunc[];		// ������ފ֐�

	typedef void(CPlayerAI::* MOVE_FUNC)(const float, const float, const float);
	static MOVE_FUNC m_ThrowMoveFunc[];		// ������Ԋ֐�

	typedef void(CPlayerAI::* TIMING_FUNC)(const float, const float, const float);
	static TIMING_FUNC m_ThrowTimingFunc[];	// �����^�C�~���O�֐�

	typedef void(CPlayerAI::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];		// �����^�C�~���O�֐�

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �L���b�`����

	// �����̎�ފ֐�
	void TypeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �ʏ퓊��
	void TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �W�����v����
	void TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�y�V��������
	
	// �ړ��̏��
	void MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���̏꓊��
	void MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����ē���
	void MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����ē���

	// �����̃^�C�~���O
	void TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 
	void TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 
	void TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 

	void TimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingJumpFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// �L���b�`
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void Reset();	// �񋓃��Z�b�g
	void Target();	// �^�[�Q�b�g
	bool IsWait();	// �҂�

	//=============================
	// �����o�ϐ�
	//=============================
	EMode m_eMode;					// ���[�h
	EThrowType m_eThrowType;		// �������
	EThrowMove m_eThrowMove;		// �����s��
	EThrowTiming m_eThrowTiming;	// �����^�C�~���O
	ECatchType m_eCatchType;		// �L���b�`���
	float m_fTiming;		// �^�C�~���O�J�E���g
	float m_fTimingRate;	// �^�C�~���O�̊���
	bool m_bTiming;			// �^�C�~���O�t���O
	bool m_bFoldJump;		// �W�����v�̐܂�Ԃ��t���O
	float m_fJumpEnd;		// �W�����v�̏I���ʒu
};

#endif

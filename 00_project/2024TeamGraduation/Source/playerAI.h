//=============================================================================
// 
//  AI�v���C���[�w�b�_�[ [playerAI.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_		// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)

//==========================================================================
// �N���X��`
//==========================================================================
// AI�v���C���[�N���X
class CPlayerAI : public CPlayer
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
		MOVE_NORMAL = 0,		// �ʏ�
		MOVE_WALK,			// ����
		MOVE_DASH,			// ����
		MOVE_MAX
	};

	enum EThrowTiming	// �����̃^�C�~���O
	{
		TIMING_NORMAL = 0,	// �ʏ�
		TIMING_QUICK,		// ����
		TIMING_DELAY,		// �x��
		TIMING_FEINT,		// �t�F�C���g
		TIMING_MAX
	};

	//=============================
	// �\���̒�`
	//=============================

public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAI();
	~CPlayerAI();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();	// �폜

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerAIControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerAIControlAction* control);	// �A�N�V�����̑���ύX

	//=============================
	// �ÓI�֐�
	//=============================
	/*
		@brief		��������
		@details	�v���C���[�C���f�b�N�X
		@details	�`�[���T�C�h
	*/
	static CPlayerAI *Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos);

protected:

	virtual void Debug() override;	// �f�o�b�O����

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAI::* MODE_FUNC)(const float , const float, const float);
	static MODE_FUNC m_ModeFunc[];	// ���[�h�֐�

	typedef void(CPlayerAI::* TYPE_FUNC)(const float, const float, const float);
	static TYPE_FUNC m_ThrowTypeFunc[];	// ������ފ֐�

	typedef void(CPlayerAI::* MOVE_FUNC)(const float, const float, const float);
	static MOVE_FUNC m_ThrowMoveFunc[];	// ������Ԋ֐�

	typedef void(CPlayerAI::* TIMING_FUNC)(const float, const float, const float);
	static TIMING_FUNC m_ThrowTimingFunc[];	// �����^�C�~���O�֐�


	//=============================
	// �����o�֐�
	//=============================
	SHitInfo Hit(CBall* pBall) override;

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// ��ԍX�V
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};			// �Ȃ�
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ��������
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`����

	// �����̎�ފ֐�
	void TypeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// �Ȃ�
	void TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �ʏ퓊��
	void TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �W�����v����
	void TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�y�V��������
	
	// �����̏��
	void MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����ē�����
	void MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����ē�����

	// �����̃^�C�~���O
	void TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ����
	void DeleteControl() override;	// ����폜

	//=============================
	// �����o�ϐ�
	//=============================
	EMode m_eMode;
	EThrowType m_eThrowType;
	EThrowMove m_eThrowMove;
	EThrowTiming m_eThrowTiming;

	float m_fTimingCount;	// �^�C�~���O�J�E���g
	bool m_bTiming;			// �^�C�~���O�t���O
};

#endif

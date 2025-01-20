//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_DEFENSE_H_
#define _PLAYERAICONTROL_DEFENSE_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "playerAIcontrol_mode.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerAIControlMove;		// �ړ�(AI)
class CPlayerAIControlAction;	// �A�N�V����(AI)
class CPlayer;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
class CPlayerAIControlDefense : public CPlayerAIControlMode
{
public: 

	enum EActionStatus
	{
		ACTIONSTATUS_IDLE = 0,
		ACTIONSTATUS_ACTION,
		ACTIONSTATUS_COOLDOWN,
		ACTIONSTATUS_MAX
	};

	enum EAction				// �A�N�V����
	{
		IDLE = 0,
		DODGE,					// ����s��
		SUPPORT,				// �������T�|�[�g
		CHASE_BALL,				// �{�[����ǂ�������
		RETREAT,				// ��ށi���S�n�тɈړ��j
		RNDOM,					// �����_��
		LEAVE,					// �����
		MAX
	};

	struct SAction {
		MyLib::Vector3 pos;			// �ʒu
		float fTimer;				// �^�C�}�[
		bool bSet;					// �ݒ�t���O
		bool bCooldown;				// �N�[���_�E��
		bool bCancel;				// �L�����Z��
	};

public:

	// �R���X�g���N�^
	CPlayerAIControlDefense();

	static CPlayerAIControlDefense* Create(CPlayer* player);


	HRESULT Init() override;
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	// �ݒ�E�擾
	void SetActionStatus(EActionStatus status) { m_eActionStatus = status; }
	EActionStatus GetActionStatus() { return m_eActionStatus; }
	void SetAction(EAction action) { m_eAction = action; }
	EAction GetAction() { return m_eAction; }
	void SetActionInfo(SAction action) { m_sAction = action; }
	SAction GetActionInfo() { return m_sAction; }

protected:
	//=============================
	// ���z�E�����֐�
	//=============================
	virtual bool IsLineOverBall() { return false; }				// ����������(�{�[��)
	virtual bool IsLineOverPlayer() { return false; };			// ���z������(�v���C���[)

	//=============================
	// �����o�֐�
	//=============================
	bool Leave(MyLib::Vector3 targetPos, float distance);		// �����
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// �߂Â�
	void MoveUp();			// ��ړ�
	void MoveDown();		// ���ړ�
	void MoveLeft();		// ���ړ�
	void MoveRight();		// �E�ړ�

	void UpdateSee();						// �X�V
	void SeeTarget(MyLib::Vector3 pos);		// �^�[�Q�b�g���݂�
	void SeeBall();							// �{�[��������

	//-----------------------------
	// �ݒ�֐�
	//-----------------------------
	CPlayer* GetBallOwner();				// �{�[��������
	float GetDistance() { return m_fDistanse; }

	float GetDistanceBall();				// �{�[���Ƃ̋���
	float GetDistanceEnemy();				// �G�Ƃ̋���
	float GetDistanceBallowner();			// �{�[��������Ƃ̋���

private:

	//-----------------------------
	// ����
	//-----------------------------
	bool IsDistanceBall();					// �����F�{�[��
	bool IsPassTarget();					// �p�X���鑊�肪���邩����
	bool IsPicksUpBall();					// �{�[�����E�����f

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControlDefense::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// �A�N�V�����֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void MoveIdle();
	void MoveDodge();				// ���
	void MoveSupport();				// �T�|�[�g
	void MoveChaseBall();			// �{�[����ǂ�������
	virtual void MoveRetreat() {};	// ���
	virtual void MoveRandom() {};	// �����_��
	void MoveLeave();				// �����

	//=============================
	// �����o�֐�
	//=============================
	void UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���
	void Action0();			// �A�N�V����
	void Action1();			// �A�N�V����

	void BallSteal();		// �{�[����D��
	void BallChase();		// �{�[����ǂ�

	void UpdateActionTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void SetActionTimer(int nMin, int nMax);	// �s���^�C�}�[

	void Cancel();

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fDistanse;

	//-----------------------------
	// �񋓌^
	//-----------------------------
	EAction m_eAction;				// �A�N�V����
	EActionStatus m_eActionStatus;	// �A�N�V�������

	//-----------------------------
	// �\����
	//-----------------------------
	SAction m_sAction;				// �A�N�V����
};

#endif

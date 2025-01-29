//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_ATTACK_H_
#define _PLAYERAICONTROL_ATTACK_H_	// ��d�C���N���[�h�h�~

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
class CPlayerAIControlAttack : public CPlayerAIControlMode
{
public:

	enum EAttackMode			// �U�����[�h
	{
		ATTACKMODE_PREPARATION = 0,		// ����
		ATTACKMODE_ATTACK,				// �U��
		ATTACKMODE_MAX
	};

	enum EAttackPrepatarion		// ����
	{
		ATTACKPREPATARION_NONE = 0,		// �Ȃ�
		ATTACKPREPATARION_WAIT,			// �҂�
		ATTACKPREPATARION_GO,			// �s��
		ATTACKPREPATARION_LEAVE,		// �����
		ATTACKPREPATARION_MAX
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

	enum EAttackNormal
	{
		ATTACK_NONE = 0,
		ATTACK_THROW,
		ATTACK_DASH,
		ATTACK_MAX
	};

	enum EThrowFlag				// �����t���O
	{
		THROW_NONE = 0,			// �Ȃ�
		THROW_NORMAL,			// ����
		THROW_PASS,				// �p�X
		THROW_SPECIAL,			// �X�y�V����
	};

private:

	struct STiming
	{
		float fTimer;
		float fPosY;
		bool bSet;
	};

public:

	// �R���X�g���N�^
	CPlayerAIControlAttack();

	static CPlayerAIControlAttack* Create(CPlayer* player);

	HRESULT Init() override;
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	// �ݒ�E�擾
	void SetThrowFlag(EThrowFlag Throw) { m_eThrowFlag = Throw; }			// �����ݒ�
	EThrowFlag GetThrowFlag() { return m_eThrowFlag; }						// �擾
	void SetAttackMode(EAttackMode mode) { m_eAttackMode = mode; }			// �U�����[�h�ݒ�
	EAttackMode GetAttackMode() { return m_eAttackMode; }					// �擾

protected:
	//=============================
	// ���z�E�����֐�
	//=============================
	virtual void Preparation() = 0;

	//=============================
	// �����o�֐�
	//=============================
	bool Leave(MyLib::Vector3 targetPos, float distance);		// �����
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// �߂Â�
	void SeeTarget(MyLib::Vector3 pos);							// �^�[�Q�b�g���݂�

	//=============================
	// �ݒ�E�擾�֐�
	//=============================
	CPlayer* GetBallOwner();				// �{�[��������
	CPlayer* GetThrowTarget();				// ������^�[�Q�b�g

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControlAttack::* ATTACKMODE_FUNC)();
	static ATTACKMODE_FUNC m_AttackModeFunc[];		// �U�����[�h

	typedef void(CPlayerAIControlAttack::* PREPARATION_FUNC)();
	static PREPARATION_FUNC m_PreparationFunc[];		// ����

	typedef void(CPlayerAIControlAttack::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];		// ������^�C�v�֐�

	typedef void(CPlayerAIControlAttack::* THROWFLAG_FUNC)();
	static THROWFLAG_FUNC m_ThrowFlagFunc[];		// �����t���O�֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	// �U�����[�h
	void AttackModePreparation();
	void AttackModeAttack();

	// ����
	void PreparationNone() {};
	void PreparationWait();
	void PreparationGo();
	virtual void PreparationLeave() = 0;

	// �����^�C�v
	void ThrowTypeNone() {};		// �Ȃ�
	void ThrowTypeNormal();			// �ʏ�
	void ThrowTypeJump();			// �W�����v
	void ThrowTypeSpecial();		// �X�y�V����
	void ThrowTypePass();			// �p�X
	
	// �����t���O
	void ThrowFlagNone() {};		// �Ȃ�
	void ThrowFlag();				// ����
	void ThrowFlagPass();			// �p�X
	void ThrowFlagSpecial();		// �X�y�V����

	//=============================
	// �����o�֐�
	//=============================
	void UpdateAttack();						// �U�ߍX�V
	void UpdateThrow();							// �����X�V
	bool IsCancelJumpAttack();					// �L�����Z��

	// ����
	void AttackNormal(CPlayer* pTarget);		// �ʏ퓊��
	void AttackDash(CPlayer* pTarget, float fTargetDis, float fLineDis);			// ���蓊��

	// �W�����v
	void AttackJump(CPlayer* pTarget);			// �W�����v����
	void AttackDashJump(CPlayer* pTarget, float fTargetDistanse, float LineDistanse);		// ����W�����v����

	void AttackFeint();							// �t�F�C���g

	void UpdateAttackTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void SetAttackTimer(int nMin, int nMax);	// �s���^�C�}�[

	bool IsStop();			// �~�߂锻�f

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pTarget;
	bool m_bSet;
	int m_nLevel;

	//-----------------------------
	// ��
	//-----------------------------
	EAttackMode m_eAttackMode;			// �U�����[�h
	EAttackPrepatarion m_ePreparation;	// ����
	EThrowType m_eThrowType;			// �������
	EThrowFlag m_eThrowFlag;			// �����t���O
	EAttackNormal m_eAttackNormal;

	//-----------------------------
	// �\����
	//-----------------------------
	STiming m_sTimig;
};

#endif

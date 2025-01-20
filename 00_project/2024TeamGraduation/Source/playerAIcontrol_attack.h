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

	enum  EThrowType			// �����^�C�v
	{
		THROWTYPE_NONE = 0,		// �Ȃ�
		THROWTYPE_NORMAL,		// �ʏ�
		THROWTYPE_JUMP,			// �W�����v
		THROWTYPE_SPECIAL,		// �X�y�V����
		//THROWTYPE_PASS,			// �p�X
		THROWTYPE_MAX
	};

	enum EThrowFlag				// �����t���O
	{
		THROW_NONE = 0,			// �Ȃ�
		THROW_NORMAL,			// ����
		THROW_PASS,				// �p�X
		THROW_SPECIAL,			// �X�y�V����
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

protected:
	//=============================
	// ���z�E�����֐�
	//=============================
	virtual void AttackDash(CPlayer* pTarget) {};	// ���蓊��

	//=============================
	// �����o�֐�
	//=============================
	bool Leave(MyLib::Vector3 targetPos, float distance);		// �����
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// �߂Â�
	void SeeTarget(MyLib::Vector3 pos);		// �^�[�Q�b�g���݂�

	//=============================
	// �ݒ�E�擾�֐�
	//=============================
	CPlayer* GetBallOwner();				// �{�[��������
	CPlayer* GetThrowTarget();				// ������^�[�Q�b�g

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPlayerAIControlAttack::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];		// ������^�C�v�֐�

	typedef void(CPlayerAIControlAttack::* THROWFLAG_FUNC)();
	static THROWFLAG_FUNC m_ThrowFlagFunc[];		// �����t���O�֐�

	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	// �����^�C�v
	void ThrowTypeNone() {};		// �Ȃ�
	void ThrowTypeNormal();			// �ʏ�
	void ThrowTypeJump();			// �W�����v
	void ThrowTypeSpecial();		// �X�y�V����
	
	// �����t���O
	void ThrowFlagNone() {};		// �Ȃ�
	void ThrowFlag();				// ����
	void ThrowFlagPass();			// �p�X
	void ThrowFlagSpecial();		// �X�y�V����

	//=============================
	// �����o�֐�
	//=============================
	void UpdateThrowFlag();			// �����t���O
	void UpdateThrowType();			// �������

	//=============================
	// �����o�ϐ�
	//=============================

	//-----------------------------
	// ��
	//-----------------------------
	EThrowType m_eThrowType;		// �������
	EThrowFlag m_eThrowFlag;		// �����t���O

	//-----------------------------
	// �\����
	//-----------------------------

};

#endif

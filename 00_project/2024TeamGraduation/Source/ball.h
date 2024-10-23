//==========================================================================
// 
//  �{�[���w�b�_�[ [ball.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _BALL_H_
#define _BALL_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "objectX.h"
#include "gamemanager.h"
#include "listmanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;

//==========================================================================
// �N���X��`
//==========================================================================
// �{�[���N���X
class CBall : public CObjectX
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	// ���
	enum EState
	{
		STATE_SPAWN = 0,	// ������� (�J�n���̃t���[�{�[��)
		STATE_CATCH,		// �L���b�`��� (�v���C���[���ێ�)
		STATE_HOM_NOR,		// �ʏ�z�[�~���O��� (�^�[�Q�b�g�Ǐ])
		STATE_HOM_JUMP,		// �W�����v�z�[�~���O��� (�^�[�Q�b�g�Ǐ])
		STATE_MOVE,			// �ړ���� (�����ړ�)
		STATE_REBOUND,		// ���o�E���h��� (�Ԃ��������̗���)
		STATE_LAND,			// ���n��� (�n�ʂɓ]�����Ă���)
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �U�����
	enum EAttack
	{
		ATK_NONE = 0,	// �U�����薳��
		ATK_NORMAL,		// �ʏ�U��
		ATK_JUMP,		// �W�����v�U��
		ATK_SPECIAL,	// �X�y�V�����X�g�U��
		ATK_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CBall(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBall();

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
	void CatchAttack(CPlayer* pPlayer);		// �U���L���b�`
	void CatchLand(CPlayer* pPlayer);		// ���n�L���b�`
	void ThrowNormal(CPlayer* pPlayer);		// �ʏ퓊��
	void ThrowJump(CPlayer* pPlayer);		// �W�����v����
	void ThrowSpecial(CPlayer* pPlayer);	// �X�y�V��������

	CGameManager::TeamSide GetTypeTeam() const { return m_typeTeam; }	// �`�[���T�C�h�擾
	EAttack GetTypeAtk() const	{ return m_typeAtk; }	// �U����ގ擾
	EState GetState() const		{ return m_state; }		// ��Ԏ擾
	float GetRadius() const;	// ���a�擾
	bool IsAttack() const;		// �U���t���O�擾

	//=============================
	// �ÓI�֐�
	//=============================
	/*
		@brief		��������
		@details	�ʒu
	*/
	static CBall *Create(const MyLib::Vector3& rPos);
	static CListManager<CBall> GetListObj() { return m_list; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CBall::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// �I�[�o�[���C�h�֐�
	void CalWorldMtx();	// ���[���h�}�g���b�N�X�̌v�Z

	// ��Ԋ֐�
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ������Ԃ̍X�V
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`��Ԃ̍X�V
	void UpdateHomingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ʏ�z�[�~���O��Ԃ̍X�V
	void UpdateHomingJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �W�����v�z�[�~���O��Ԃ̍X�V
	void UpdateMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �ړ���Ԃ̍X�V
	void UpdateReBound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���o�E���h��Ԃ̍X�V
	void UpdateLand(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ���n��Ԃ̍X�V

	void UpdateGravity(const float fDeltaRate, const float fSlowRate);	// �d�͉��Z
	void UpdateDecay(const float fDeltaRate, const float fSlowRate);	// �ړ��ʌ���
	void UpdateGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ʒu�ɏd�͔��f
	void UpdateMovePosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ʒu�Ɉړ��ʔ��f
	void UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ړ�
	bool UpdateLanding(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �n�ʒ��n

	CPlayer* CollisionPlayer(MyLib::Vector3* pPos);	// �v���C���[�Ƃ̓����蔻��
	CPlayer* CollisionThrow(void);		// �z�[�~���O�ΏۂƂ̓����蔻��
	void SetState(const EState state);	// ��Ԑݒ�
	void Catch(CPlayer* pPlayer);		// �L���b�`
	void Throw(CPlayer* pPlayer);		// ����
	void Landing(void);					// ���n
	void ReBound(CPlayer* pHitPlayer, MyLib::Vector3* pMove);	// ���o�E���h

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CListManager<CBall> m_list;	// ���X�g

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pPlayer;	// �v���C���[���
	CPlayer* m_pTarget;	// �z�[�~���O�^�[�Q�b�g���
	CPlayer* m_pCover;	// �J�o�[�Ώۃv���C���[���
	float m_fMoveSpeed;	// �ړ����x
	float m_fGravity;	// �d��
	MyLib::Vector3 m_oldOverLine;	// �z�[�~���O�I�����C��

	CGameManager::TeamSide m_typeTeam;	// �`�[���T�C�h
	EAttack m_typeAtk;	// �U�����
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃJ�E���^�[
};

#endif
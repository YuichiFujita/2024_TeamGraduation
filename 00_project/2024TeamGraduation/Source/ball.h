//=============================================================================
// 
//  �{�[���w�b�_�[ [ball.h]
//  Author : ���c�E��
// 
//=============================================================================

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
		STATE_CATCH,		// ������� (�v���C���[���ێ����Ă���)
		STATE_THROW,		// �U����� (�U������������Ă���)
		STATE_FALL,			// ������� (�n�ʂɓ]�����Ă���)
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
	void Catch(CPlayer* pPlayer);			// �L���b�`
	void ThrowNormal(CPlayer* pPlayer);		// �ʏ퓊��
	void ThrowJump(CPlayer* pPlayer);		// �W�����v����
	void ThrowSpecial(CPlayer* pPlayer);	// �X�y�V��������

	CGameManager::TeamSide GetTypeTeam() const { return m_typeTeam; }	// �`�[���T�C�h�擾
	EAttack GetTypeAtk() const	{ return m_typeAtk; }	// �U����ގ擾
	EState GetState() const		{ return m_state; }		// ��Ԏ擾

	//=============================
	// �ÓI�֐�
	//=============================
	/*
		@brief		��������
		@details	�K�v������Έ����ǉ�
	*/
	static CBall *Create(const MyLib::Vector3& rPos);
	static CListManager<CBall> GetListObj() { return m_list; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CBall::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_SampleFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ������Ԃ̍X�V
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ������Ԃ̍X�V
	void UpdateThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �U����Ԃ̍X�V
	void UpdateFall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ������Ԃ̍X�V

	void UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ړ�
	bool UpdateLanding(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �n�ʒ��n

	bool CollisionPlayer(MyLib::Vector3* pPos);	// �v���C���[�Ƃ̓����蔻��
	void Throw(CPlayer* pPlayer);	// ����
	void Fall(void);	// ����

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CListManager<CBall> m_list;	// ���X�g

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pPlayer;	// �v���C���[���

	CGameManager::TeamSide m_typeTeam;	// �`�[���T�C�h
	EAttack m_typeAtk;	// �U�����
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃJ�E���^�[
};

#endif
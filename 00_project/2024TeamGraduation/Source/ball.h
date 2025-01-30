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
class CShadow;
class CPlayer;
class CBallHolderMarker;
class CBallLandingMarker;
class CEffekseerObj;

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
		STATE_SPAWN = 0,		// ������� (�J�n���̃t���[�{�[��)
		STATE_CATCH,			// �L���b�`��� (�v���C���[���ێ�)
		STATE_HOM_NOR,			// �ʏ�z�[�~���O���
		STATE_HOM_JUMP,			// �W�����v�z�[�~���O���
		STATE_MOVE,				// �ړ����
		STATE_SPECIAL_STAG,		// �X�y�V�������o���
		STATE_SPECIAL_THROW,	// �X�y�V�����������
		STATE_HOM_PASS,			// �p�X�z�[�~���O���
		STATE_PASS,				// �p�X���
		STATE_CENTER_RETURN,	// �����������
		STATE_REBOUND,			// ���o�E���h��� (�Ԃ��������̗���)
		STATE_FREE,				// �t���[��� (�G�ꂽ�����Ŏ���)
		STATE_LAND,				// ���n��� (�n�ʂɓ]�����Ă���)
		STATE_MAX				// ���̗񋓌^�̑���
	};

	// �X�y�V�������		//Caution: �ǉ�������catchSpecial��CHECK_FUNC���ǉ�����
	enum ESpecial
	{
		SPECIAL_NONE = 0,	// �X�y�V��������
		SPECIAL_KAMEHAMEHA,	// ���߂͂ߔg
		SPECIAL_MAX			// ���̗񋓌^�̑���
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
	virtual void Kill() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void CatchAttack(CPlayer* pPlayer);			// �U���L���b�`
	void CatchLand(CPlayer* pPlayer);			// ���n�L���b�`
	void Spawn(CPlayer* pPlayer);				// �X�|�[��
	void ThrowNormal(CPlayer* pPlayer);			// �ʏ퓊��
	void ThrowJump(CPlayer* pPlayer);			// �W�����v����
	void Special(CPlayer* pPlayer);				// �X�y�V��������
	bool Pass(CPlayer* pPlayer);				// �p�X
	void CenterReturn(CPlayer* pPlayer);		// ��������
	void Toss(CPlayer* pPlayer);				// �g�X
	void SpecialEndSetting();					// �X�y�V�����I��
	MyLib::Vector3 GetPosPassStart() const		{ return m_posPassStart; }	// �p�X�J�n�ʒu�擾
	MyLib::Vector3 GetPosPassEnd() const		{ return m_posPassEnd; }	// �p�X�I���ʒu�擾
	CGameManager::ETeamSide GetTypeTeam() const	{ return m_typeTeam; }		// �`�[���T�C�h�擾
	ESpecial GetTypeSpecial() const	{ return m_typeSpecial; }	// �X�y�V������ގ擾
	EAttack GetTypeAtk() const		{ return m_typeAtk; }		// �U����ގ擾
	EState GetState() const			{ return m_state; }			// ��Ԏ擾
	CPlayer* GetPlayer() const		{ return m_pPlayer; }		// �����v���C���[�擾
	CPlayer* GetTarget() const		{ return m_pTarget; }		// �^�[�Q�b�g�擾
	CPlayer* GetCover() const		{ return m_pCover; }		// �J�o�[�Ώۃv���C���[�擾
	int GetDamage() const			{ return m_nDamage; }		// �_���[�W�擾
	int GetCoverHeal() const		{ return m_nCoverHeal; }	// �J�o�[�񕜎擾
	float GetKnockback() const		{ return m_fKnockback; }	// �m�b�N�o�b�N�擾
	bool IsLanding() const			{ return m_bLanding; }		// ���n�t���O�擾
	float GetRadius() const;		// ���a�擾
	bool IsAttack() const;			// �U���t���O�擾
	bool IsPass() const;			// �p�X�t���O�擾
	bool IsSpecial() const;			// �X�y�V�����t���O�擾

	//=============================
	// �ÓI�����o�֐�
	//=============================
	/*
		@brief		��������
		@details	�ʒu
	*/
	static CBall *Create(const MyLib::Vector3& rPos);
	static CListManager<CBall> GetListObj() { return m_list; }	// ���X�g�擾

protected:
	//=============================
	// �����o�֐�
	//=============================
	void Throw(CPlayer* pPlayer);	// ����

private:

	//=============================
	// �\���̒�`
	//=============================
	// �q�b�g�^�C�~���O���
	struct SHitTimingInfo
	{
		int nHitFrame;	// �q�b�g�܂łɂ�����t���[����
		float fHitTime;	// �q�b�g�܂łɂ����鎞��
		bool bHit;		// �q�b�g������
	};

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(CBall::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	// �X�y�V�����֐�
	typedef void(CBall::*SPECIAL_FUNC)(const float, const float, const float);
	static SPECIAL_FUNC m_SpecialFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// �I�[�o�[���C�h�֐�
	void CalWorldMtx() override;	// ���[���h�}�g���b�N�X�̌v�Z

	//--------------------------
	// ��Ԋ֐�
	//--------------------------
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ������Ԃ̍X�V
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �L���b�`��Ԃ̍X�V
	void UpdateHomingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ʏ�z�[�~���O��Ԃ̍X�V
	void UpdateHomingJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �W�����v�z�[�~���O��Ԃ̍X�V
	void UpdateMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �ړ���Ԃ̍X�V
	void UpdateSpecialStag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�y�V�������o��Ԃ̍X�V
	void UpdateSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�y�V����������Ԃ̍X�V
	void UpdateHomingPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �p�X�z�[�~���O��Ԃ̍X�V
	void UpdatePass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �p�X��Ԃ̍X�V
	void UpdateCenterReturn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����������Ԃ̍X�V
	void UpdateReBound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���o�E���h��Ԃ̍X�V
	void UpdateFree(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �t���[��Ԃ̍X�V
	void UpdateLand(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ���n��Ԃ̍X�V

	//--------------------------
	// �X�y�V�����֐�
	//--------------------------
	void UpdateKamehameha(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���߂͂ߔg�̍X�V

	//--------------------------
	// �ėp�֐�
	//--------------------------
	void UpdateGravity(const float fDeltaRate, const float fSlowRate);	// �d�͉��Z
	void UpdateDecay(const float fDeltaRate, const float fSlowRate);	// �ړ��ʌ���
	void UpdateUpOnlyGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ʒu�ɏd�͔��f (�㏸�̂�)
	void UpdateGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ʒu�ɏd�͔��f
	void UpdateMovePosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ʒu�Ɉړ��ʔ��f
	void UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �ړ�
	bool UpdateLanding(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// �n�ʒ��n
	CPlayer* CollisionPlayer(MyLib::Vector3* pPos);				// �v���C���[�Ƃ̓����蔻��
	CPlayer* CollisionThrowTarget(const bool bAbsLock = false);	// ������ΏۂƂ̓����蔻��
	CPlayer* CollisionPassTarget();		// �p�X����ΏۂƂ̓����蔻��
	float GetCollRadius();				// ���蔼�a�擾
	void SetState(const EState state);	// ��Ԑݒ�
	void Catch(CPlayer* pPlayer);		// �L���b�`
	void ThrowSpecial();				// �X�y�V��������
	void Landing();						// ���n
	void UpdateTypeTeam();				// �`�[���T�C�h�X�V
	void UpdateTypeAtk();				// �U����ލX�V
	void UpdateTypeSpecial();			// �X�y�V������ލX�V
	void OutcomeThrowJust();			// �W���X�g��������
	void OutcomeThrowDrop();			// �h���b�v��������
	void CalcSetInitialSpeed(const float fMove);	// �����v�Z
	void ReBound(CPlayer* pHitPlayer, MyLib::Vector3* pMove);	// ���o�E���h

	MyLib::Vector3 CalcVecMove(CPlayer* pTarget, CPlayer* pPlayer);	// �ړ��x�N�g���v�Z
	SHitTimingInfo CalcHitSpeedTime	// �{�[�����G�ɓ��B����܂ł̎��Ԍv�Z
	( // ����
		const MyLib::Vector3& rPosTarget,	// �^�[�Q�b�g�ʒu
		const MyLib::Vector3& rPosBall,		// �{�[���ʒu
		const float fRadiusTarget,			// �^�[�Q�b�g���a
		const float fInitSpeed,				// ����
		const float fMoveSpeed				// �ړ���
	);

	//--------------------------
	// �G�t�F�N�g
	//--------------------------
	void UpdateThrowLine();	// ���������̍X�V

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CListManager<CBall> m_list;	// ���X�g

	//=============================
	// �����o�ϐ�
	//=============================
	CBallHolderMarker* m_pHoldMarker;	// �{�[�������}�[�J�[���
	CBallLandingMarker* m_pLandMarker;	// �{�[�����n�_�}�[�J�[���
	CShadow* m_pShadow;		// �e���
	CPlayer* m_pPlayer;		// �v���C���[���
	CPlayer* m_pTarget;		// �z�[�~���O�^�[�Q�b�g���
	CPlayer* m_pCover;		// �J�o�[�Ώۃv���C���[���
	float m_fMoveSpeed;		// �ړ����x
	float m_fInitialSpeed;	// ����
	float m_fGravity;		// �d��
	float m_fBouncy;		// ����
	float m_fHomingTime;	// �z�[�~���O����
	bool m_bLanding;		// ���n�t���O
	MyLib::Vector3 m_posPassStart;	// �p�X�J�n�ʒu
	MyLib::Vector3 m_posPassEnd;	// �p�X�I���ʒu

	CGameManager::ETeamSide m_typeTeam;	// �`�[���T�C�h
	ESpecial m_typeSpecial;	// �X�y�V�������
	EAttack m_typeAtk;		// �U�����
	EState m_state;			// ���
	float m_fStateTime;		// ��ԃJ�E���^�[
	int m_nDamage;			// �_���[�W
	int m_nCoverHeal;		// �J�o�[��
	float m_fKnockback;		// �m�b�N�o�b�N
	float m_fBallAngle;		// ��]�p�x
	CEffekseerObj* m_pThrowLine;	// �����̃��C��
	CEffekseerObj* m_pAura;			// �I�[��
	CEffekseerObj* m_pSPBallet;		// �X�y�V�����̒e
};

#endif
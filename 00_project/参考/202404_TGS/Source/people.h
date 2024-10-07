//=============================================================================
// 
//  �l�w�b�_�[ [people.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// �O���錾
class CShadow;

//==========================================================================
// �N���X��`
//==========================================================================
// �l�N���X
class CPeople : public CObjectChara
{
public:
	
	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_FADEIN,		// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	enum MOTION
	{
		MOTION_DEF = 0,			// �j���[�g����
		MOTION_WALK,			// �ړ�
		MOTION_MAX
	};

	enum TYPE
	{
		TYPE_KITE = 8,	// ��
		TYPE_KITE2,	// ��
		TYPE_KITE3,	// ��
	};

	CPeople(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CPeople();


	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// �폜

	void SetState(STATE state);				// ��Ԑݒ�
	STATE GetState() { return m_state; }	// ��Ԏ擾
	void SetStateTime(float time) { m_fStateTime = time; }	// ��Ԏ��Ԑݒ�

	// ���[�V����
	void SetMotion(int motionIdx);	// ���[�V�����̐ݒ�

	HRESULT LoadText(const char *pFileName);

	static CListManager<CPeople> GetListObj() { return m_List; }	// ���X�g�擾
	static CPeople* Create(const std::string& filename, const MyLib::Vector3& pos, const TYPE& type);

protected:

	//=============================
	// �\���̒�`
	//=============================
	// ���[�V�����̔���
	struct SMotionFrag
	{
		bool bJump;			// �W�����v�����ǂ���
		bool bATK;			// �U�������ǂ���
		bool bKnockback;	// �m�b�N�o�b�N�����ǂ���
		bool bMove;			// �ړ������ǂ���
		bool bCharge;		// �`���[�W�����ǂ���
		SMotionFrag() : bJump(false), bATK(false), bKnockback(false), bMove(false), bCharge(false) {}
	};

	//=============================
	// �����o�֐�
	//=============================
	// ��ԍX�V�n
	virtual void StateNone();		// �����Ȃ����
	virtual void StateFadeIn();		// �t�F�[�h�C��
	virtual void StateFadeOut();	// �t�F�[�h�A�E�g

	// ���̑��֐�
	virtual void ProcessLanding();	// ���n������
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// �U��������
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// �U�����蒆����

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;							// ���
	STATE m_Oldstate;						// �O��̏��
	float m_fStateTime;						// ��ԃJ�E���^�[
	SMotionFrag m_sMotionFrag;				// ���[�V�����̃t���O
	D3DXCOLOR m_mMatcol;					// �}�e���A���̐F
	MyLib::Vector3 m_TargetPosition;		// �ڕW�̈ʒu
	int m_flame;

private:
	
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPeople::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐����X�g

	void UpdateState();					// ��ԍX�V����
	void Collision();					// �����蔻��
	void LimitArea(); // ��l�̕ǔ���

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fMoveVelocity;	// �ړ����x
	CShadow *m_pShadow;			// �e�̏��
	static CListManager<CPeople> m_List;	// ���X�g
};



#endif
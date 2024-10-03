//=============================================================================
// 
//  �͂���w�b�_�[ [receiver_people.h]
//  Author : IbukiOkusada
// 
//=============================================================================

#ifndef _RECEIVER_PEOPLE_H_
#define _RECEIVER_PEOPLE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// �O���錾
class CShadow;
class CBaggage;

//==========================================================================
// �N���X��`
//==========================================================================
// �˗��l�N���X
class CReceiverPeople : public CObjectChara
{
private:

	// ���������
	enum DISTANCE
	{
		DISTANCE_NEAR,	// �߂�
		DISTANCE_FAR,	// ����
		DISTANCE_MAX
	};

public:
	
	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_FADEIN,		// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_WAIT,			// �ҋ@���
		STATE_GET,			// �L���b�`���
		STATE_RETURN,		// ���V�[�u���
		STATE_YABAI,		// ���o�C���
		STATE_DIVE,			// �_�C�u���
		STATE_DROWN,		// �M����
		STATE_BYEBYE,		// �o�C�o�C
		STATE_WALK,			// ����
		STATE_MAX
	};

	enum MOTION
	{
		MOTION_DEF = 0,	// �j���[�g����
		MOTION_WALK,	// �ړ�
		MOTION_PASS,	// �n��
		MOTION_BYEBYE,	// �o�C�o�C
		MOTION_GET,		// �L���b�`
		MOTION_RETURN,	// �����ɔ�΂������ăA�E�g
		MOTION_DROWN,	// �M���
		MOTION_DIVE,	// �_�C�u
		MOTION_YABAI,	// ��޲�I
		MOTION_MAX
	};


	CReceiverPeople(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CReceiverPeople();


	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// �폜

	// ��Ԍn
	void SetState(STATE state);		// ��Ԑݒ�
	STATE GetState() { return m_state; }
	void SetStateTime(float time) { m_fStateTime = time; }	// ��Ԏ��Ԑݒ�

	// ���[�V����
	void SetMotion(int motionIdx);	// ���[�V�����̐ݒ�

	HRESULT LoadText(const std::string& pFileName);

	static CListManager<CReceiverPeople> GetListObj() { return m_List; }	// ���X�g�擾
	static CReceiverPeople* Create(const MyLib::Vector3& pos);

protected:

	
	//=============================
	// �����o�֐�
	//=============================
	// ��ԍX�V�n
	virtual void StateNone();		// �����Ȃ����
	virtual void StateFadeIn();		// �t�F�[�h�C��
	virtual void StateFadeOut();	// �t�F�[�h�A�E�g
	virtual void StateWait();		// �ҋ@
	virtual void StatePass();		// �p�X
	virtual void StateGet();		// �L���b�`
	virtual void StateReturn();		// ���V�[�u
	virtual void StateYabai();		// ��΂�
	virtual void StateDive();		// �_�C�u
	virtual void StateDrown();		// �M���
	virtual void StateByeBye();		// �o�C�o�C
	virtual void StateWalk();		// ����

	// ���̑��֐�
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// �U��������
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// �U�����蒆����

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;			// ���
	STATE m_Oldstate;		// �O��̏��
	float m_fStateTime;		// ��ԃJ�E���^�[
	D3DXCOLOR m_mMatcol;	// �}�e���A���̐F


private:
	
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CReceiverPeople::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐����X�g

	void UpdateState();		// ��ԍX�V����
	void Collision();		// �����蔻��

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_StartPos;	// �J�n�n�_
	MyLib::Vector3 m_StartRot;	// ����
	CShadow *m_pShadow;			// �e�̏��
	float m_fMoveTimer;			// �ړ�����
	bool m_bEnd;				// �I��
	DISTANCE m_Distance;		// �������
	static CListManager<CReceiverPeople> m_List;	// ���X�g
};



#endif
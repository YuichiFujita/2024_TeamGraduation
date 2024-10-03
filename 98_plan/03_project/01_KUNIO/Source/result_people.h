//=============================================================================
// 
//  �˗��l�w�b�_�[ [result_people.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RESULT_PEOPLE_H_
#define _RESULT_PEOPLE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// �O���錾
class CShadow;

//==========================================================================
// �N���X��`
//==========================================================================
// �˗��l�N���X
class CResultPeople : public CObjectChara
{
public:
	
	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_FADEIN,		// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_WAIT,			// �ҋ@���
		STATE_PASS,			// �p�X���
		STATE_BYEBYE,		// �o�C�o�C
		STATE_MAX
	};

	enum MOTION
	{
		MOTION_DEF = 0,	// �j���[�g����
		MOTION_WALK,	// �ړ�
		MOTION_PASS,	// �n��
		MOTION_BYEBYE,	// �o�C�o�C
		MOTION_RESULT = 9,	// ���U���g
		MOTION_MAX
	};


	CResultPeople(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CResultPeople();


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

	static CListManager<CResultPeople> GetListObj() { return m_List; }	// ���X�g�擾
	static CResultPeople* Create(const MyLib::Vector3& pos);

protected:

	
	//=============================
	// �����o�֐�
	//=============================
	// ��ԍX�V�n
	virtual void StateNone();		// �����Ȃ����
	virtual void StateFadeIn();		// �t�F�[�h�C��
	virtual void StateFadeOut();	// �t�F�[�h�A�E�g
	virtual void StateWait();		// �ҋ@
	virtual void StateResult();		// �p�X
	virtual void StateByeBye();		// �o�C�o�C

	// ���̑��֐�
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// �U��������
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// �U�����蒆����
	void CreateScroll();	// ����������

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
	typedef void(CResultPeople::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	void UpdateState();		// ��ԍX�V����
	void Collision();		// �����蔻��
	void UpdateScroll();	// �������̍X�V����

	//=============================
	// �����o�ϐ�
	//=============================
	CShadow *m_pShadow;				// �e�̏��
	CObjectX* m_pScroll;			// ������
	MyLib::Vector3 m_posDestScroll;	// �������̌��̈ʒu
	float m_fScrollThrowTimer;		// �������^�C�}�[

	static CListManager<CResultPeople> m_List;	// ���X�g
};



#endif
//=============================================================================
// 
//  �����Ԋ֐��w�b�_�[ [judgestatefunc.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _JUDGE_STATE_FUNC_H_
#define _JUDGE_STATE_FUNC_H_	// ��d�C���N���[�h�h�~

//�O���錾
class CJudgeObj;

//==========================================================================
// �N���X��`
//==========================================================================
// ���
class CJudgeStateFunc
{
public:
	//��ԗ�
	enum STATE
	{
		STATE_APPEAR = 0,
		STATE_WAIT,
		STATE_FADEOUT,
		STATE_MAX
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CJudgeStateFunc(CJudgeObj* pObj);
	~CJudgeStateFunc() {}

	// ��Ԋ֐�
	void ActionState(){ (this->*(m_StateFuncList[m_state]))(); }
	virtual void StateAppear() = 0;		// �o��
	virtual void StateWait() = 0;		// �҂�
	virtual void StateFadeOut() = 0;	// �t�F�[�h�A�E�g

	// �擾�E�ݒ�
	CJudgeObj* GetObj() { return m_pObj; }
	STATE GetState() { return m_state; }
	void SetState(const STATE state) { m_state = state; }
	float GetStateTime() { return m_fStateTime; }
	void SetStateTime(const float time) { m_fStateTime = time; }
	void AddStateTime(const float time) { m_fStateTime += time; }
private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CJudgeStateFunc::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];// ��Ԋ֐��̃��X�g

	//=============================
	// �ϐ�
	//=============================
	CJudgeObj* m_pObj;	//���삷��I�u�W�F�N�g
	STATE m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[
};

// AAA
class CJudgeStateFunc_AAA : public CJudgeStateFunc
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CJudgeStateFunc_AAA(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_AAA(){}

	// ��Ԋ֐�
	void StateAppear();		// �o��
	void StateWait();		// �҂�
	void StateFadeOut();	// �t�F�[�h�A�E�g
};

// BBB
class CJudgeStateFunc_BBB : public CJudgeStateFunc
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CJudgeStateFunc_BBB(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_BBB(){}

	// ��Ԋ֐�
	void StateAppear();		// �o��
	void StateWait();		// �҂�
	void StateFadeOut();	// �t�F�[�h�A�E�g
};

// CCC
class CJudgeStateFunc_CCC : public CJudgeStateFunc
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CJudgeStateFunc_CCC(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_CCC(){}

	// ��Ԋ֐�
	void StateAppear();		// �o��
	void StateWait();		// �҂�
	void StateFadeOut();	// �t�F�[�h�A�E�g
};

// DDD
class CJudgeStateFunc_DDD : public CJudgeStateFunc
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CJudgeStateFunc_DDD(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_DDD(){}

	// ��Ԋ֐�
	void StateAppear();		// �o��
	void StateWait();		// �҂�
	void StateFadeOut();	// �t�F�[�h�A�E�g
};


#endif
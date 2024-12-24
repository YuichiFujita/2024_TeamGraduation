//==========================================================================
// 
//  ���U���g�������� [resultCrown.cpp]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _RESULTCROWN_H_
#define _RESULTCROWN_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g�����S�N���X
class CResultCrown : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_SPAWN,	// �o��
		STATE_LOOP,		// ���[�v
		STATE_START,	// �J�n
		STATE_WAIT,		// �ҋ@
		STATE_MAX
	};

	enum EResult
	{
		RESULT_WIN = 0,		// ����
		RESULT_DRAW,		// ��������
		RESULT_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CResultCrown(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CResultCrown();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual HRESULT Init(EResult result);
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// �폜
	virtual void SetEnableDisp(bool bDisp);	// �`��󋵐ݒ�

	//=============================
	// �����o�֐�
	//=============================
	void SetState(EState state);			// ��Ԑݒ�
	EState GetState() { return m_state; }	// ��Ԏ擾
	void SetResult(EResult result) { m_result = result; }	// ���ʐݒ�
	EResult GetResult() { return m_result; }				// ���ʎ擾
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	void BindXData(std::string filepass);

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CResultCrown* Create(EResult result = EResult::RESULT_DRAW);	// ����

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(CResultCrown::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}	// �Ȃ�
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �o��
	void StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���[�v
	void StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �J�n
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ҋ@
	
	// ����
	HRESULT CreateMain();	// �吶��

	// ���̑�
	void UpdateMain();		// ��X�V

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[

	// ��]
	float m_fRotationTime;	// ��]�^�C�}�[
	float m_fIntervalRotate;	// ��]�܂ł̊Ԋu
	float m_fRotationY;			// Y����]��

	// ���̑�
	CObjectX* m_pMain;		// ���C�����S
	float m_fTime;			// �^�C�}�[
	EResult m_result;		// ����
};

#endif

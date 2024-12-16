//==========================================================================
// 
//  �^�C�g�����S�w�b�_�[ [titleLogo.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_	// ��d�C���N���[�h�h�~

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
class CTitleLogo : public CObject
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
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CTitleLogo(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CTitleLogo();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
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

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CTitleLogo* Create();	// ����

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(CTitleLogo::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �o��
	void StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���[�v
	void StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �J�n
	
	// ����
	HRESULT CreateMain();	// �吶��
	HRESULT CreateSub();	// ������

	// ���̑�
	void UpdateMain();		// ��X�V
	void UpdateSub();		// ���X�V

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[

	// ���̑�
	CObjectX* m_pMain;		// ���C�����S
	CObject2D* m_pSub;		// ��
	float m_fTime;			// �^�C�}�[
};

#endif

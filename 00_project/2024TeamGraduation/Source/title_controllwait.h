//=============================================================================
//
// ����ҋ@���� [title_controllwait.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _TITLE_CONTROLLWAIT_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _TITLE_CONTROLLWAIT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "titlescene.h"
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CDressup;
class CTitleStudent;	// �^�C�g���̐��k

//==========================================================================
// �N���X��`
//==========================================================================
// ����ҋ@�N���X
class CTitle_ControllWait : public CTitleScene
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EState
	{
		STATE_WAIT = 0,	// �ҋ@
		STATE_CONTROLL,	// ����
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CTitle_ControllWait();
	~CTitle_ControllWait();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init() override;	// ������
	virtual void Uninit() override;		// �I��
	virtual void Kill() override;		// �폜
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CTitle_ControllWait::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	//--------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ҋ@
	void StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����


	//=============================
	// �����o�ϐ�
	//=============================
	//--------------------------
	// ���
	//--------------------------
	EState m_state;			// ���
	float m_fStateTime;		// ��Ԏ���

	//--------------------------
	// ���̑�
	//--------------------------
	CTitleLogo* m_pLogo;	// ���S
	std::vector<CTitleStudent*> m_vecTitleStudent;	// �^�C�g���̐��k
};

#endif
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
class COption_BBS;

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
		STATE_TRANSITION_MORE_1st,	// �F�X�ɑJ��
		STATE_TRANSITION_WAIT,		// �J�ڑҋ@
		STATE_TRANSITION_MORE_2nd,	// �F�X�ɑJ��
		STATE_TRANSITION_MORE_LAST,	// �F�X�ɑJ��(���X�g)
		STATE_TRANSITION_BACK,		// �߂�
		STATE_MAX
	};

	enum EStateBG	// �w�i�̏��
	{
		STATEBG_SPAWN = 0,	// �o��
		STATEBG_LOOP,		// ���[�v
		STATEBG_MAX
	};

	enum ESelect
	{
		SELECT_BATTLE = 0,	// �J�n
		SELECT_MORE,		// �F�X
		SELECT_MAX
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

	void SetState(EState state);		// ��Ԑݒ�
	void SetStateBG(EStateBG state);		// ��Ԑݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CTitle_ControllWait::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	typedef void(CTitle_ControllWait::* STATEBG_FUNC)(const float, const float, const float);
	static STATEBG_FUNC m_StateBGFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	//--------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ҋ@
	void StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ����
	void StateTransitionMoreFirst(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �F�X�ɑJ��
	void StateTransitionWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �J�ڑҋ@
	void StateTransitionMoreSecond(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �F�X�ɑJ��
	void StateTransitionMoreLast(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �F�X�ɑJ��(���X�g)
	void StateTansitionBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �߂�

	// �w�i���
	void StateBGSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �o��
	void StateBGLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���[�v

	//--------------------------
	// �X�V
	//--------------------------
	void UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �I�����X�V
	void UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �F�X�V

	//--------------------------
	// ����
	//--------------------------
	HRESULT CreateSelect();	// �I��������
	HRESULT CreateBG();		// �w�i����

	// ���̑�
	void Decide();	// ����

	//=============================
	// �����o�ϐ�
	//=============================
	//--------------------------
	// ���
	//--------------------------
	EState m_state;			// ���
	float m_fStateTime;		// ��Ԏ���

	// �w�i���
	EStateBG m_stateBG;		// ���
	float m_fStateTimeBG;	// ��Ԏ���

	// �I����
	ESelect m_select;		// �I����
	CObject2D* m_pSelectUI[ESelect::SELECT_MAX];	// �I������UI
	CObject2D* m_pBG[ESelect::SELECT_MAX];			// �w�i

	//--------------------------
	// ���̑�
	//--------------------------
	CTitleLogo* m_pLogo;		// ���S
	COption_BBS* m_pOptionBBS;	// �f����
	float m_fMarkerTime;		// �}�[�J�[�̃^�C�}�[
	std::vector<CTitleStudent*> m_vecTitleStudent;	// �^�C�g���̐��k
};

#endif
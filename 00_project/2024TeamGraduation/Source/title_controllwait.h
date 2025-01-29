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
class CTutorialCheck;
class CTutorialScreen;

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
		STATE_WAIT = 0,				// �ҋ@
		STATE_CONTROLL,				// ����
		STATE_TRANSITION_MORE_1st,	// �F�X�ɑJ��
		STATE_TRANSITION_WAIT,		// �J�ڑҋ@
		STATE_TRANSITION_MORE_2nd,	// �F�X�ɑJ��
		STATE_TRANSITION_MORE_LAST,	// �F�X�ɑJ��(���X�g)
		STATE_TRANSITION_BACK,		// �߂�
		STATE_TUTORIALCHECK,		// �`���[�g���A���m�F
		STATE_TUTORIAL,				// �`���[�g���A��
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
	EState GetState() { return m_state; }	// ��Ԏ擾
	void SetStateBG(EStateBG state);		// ��Ԑݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CTitle_ControllWait::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	typedef void(CTitle_ControllWait::* STATESTART_FUNC)();
	static STATESTART_FUNC m_StateStartFunc[];	// ��ԊJ�n�֐�

	typedef void(CTitle_ControllWait::* STATEBG_FUNC)(const float, const float, const float);
	static STATEBG_FUNC m_StateBGFunc[];	// �w�i��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	//--------------------------
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
	void StateTutorialCheck(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �`���[�g���A���m�F
	void StateTutorial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// �`���[�g���A��

	//--------------------------
	// ��ԊJ�n�֐�
	//--------------------------
	void StateStartTutorialCheck();		// �`���[�g���A���m�F
	void StateStartTutorial();			// �`���[�g���A��

	// �w�i���
	void StateBGSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �o��
	void StateBGLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���[�v

	//--------------------------
	// �X�V
	//--------------------------
	void UpdateSandSmoke(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����X�V
	void UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �I�����X�V
	void UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �F�X�V
	void UpdateSelectSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �I�����̃T�C�Y�X�V

	//--------------------------
	// ����
	//--------------------------
	HRESULT CreateSelect();	// �I��������
	HRESULT CreateBG();		// �w�i����

	// ���̑�
	void Decide(int decideIdx);					// ����
	void ReCreateTutorialCheck(int decideIdx);	// �`���[�g���A���m�F�Đ���

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
	CTitleLogo* m_pLogo;				// ���S
	COption_BBS* m_pOptionBBS;			// �f����
	CTutorialCheck* m_pTutorialCheck;	// �`���[�g���A���`�F�b�N
	CTutorialScreen* m_pTutorialScreen;	// �`���[�g���A�����
	float m_fTimeMarker;				// �}�[�J�[�̃^�C�}�[
	float m_fTimeSandSmoke;				// �����̃^�C�}�[
	float m_fIntervalSandSmoke;			// �����̃C���^�[�o��
	std::vector<CTitleStudent*> m_vecTitleStudent;	// �^�C�g���̐��k
};

#endif
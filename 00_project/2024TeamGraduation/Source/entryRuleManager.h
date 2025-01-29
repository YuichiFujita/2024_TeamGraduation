//============================================================
//
//	�G���g���[���[���}�l�[�W���[�w�b�_�[ [entryRuleManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENTRYRULE_MANAGER_H_
#define _ENTRYRULE_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gamemanager.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_RULE_ARROW	(2)	// ���̑���

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CObject2D_Anim;	// �I�u�W�F�N�g2D�A�j���N���X
class CTimeUI;			// �^�C��UI�N���X
class CEntry_Dressup;	// �h���X�A�b�v�ݒ�N���X
class CArrowUI;			// ���UI

//************************************************************
//	�N���X��`
//************************************************************
// �G���g���[���[���}�l�[�W���[�N���X
class CEntryRuleManager
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_RULE_TITLE = 0,	// ���[���^�C�g���e�N�X�`��
		TEXTURE_LIFE,			// �̗̓e�N�X�`��
		TEXTURE_START,			// �J�n�{�^���e�N�X�`��
		TEXTURE_CONTROL,		// ����\���e�N�X�`��
		TEXTURE_ARROW,			// ���e�N�X�`��
		TEXTURE_SELECT,			// �I���e�N�X�`��
		TEXTURE_TIME_INF,		// ���ԁ��e�N�X�`��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_INIT = 0,		// ���������
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_RULESET,		// ���[���ݒ���
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_ENTRYBACK,	// �G���g���[�߂����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// ���[����
	enum ERule
	{
		RULE_TIME = 0,	// ����
		RULE_LIFE,		// �̗�
		RULE_MAX		// ���̗񋓌^�̑���
	};

	// �̗͗�
	enum ELife
	{
		LIFE_SMALL = 0,	// ���Ȃ�
		LIFE_NORMAL,	// �ʏ�
		LIFE_BIG,		// ����
		LIFE_MAX		// ���̗񋓌^�̑���
	};

	// ���[���\����
	struct SRule
	{
		float fTime;	// ����
		ELife life;		// �̗�
	};

	// �R���X�g���N�^
	CEntryRuleManager(CEntry_Dressup* pParent);

	// �f�X�g���N�^
	~CEntryRuleManager();

	// �����o�֐�
	HRESULT Init();		// ������
	HRESULT Uninit();	// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V
	SRule GetRule() const;	// ���[���擾

	// �ÓI�����o�֐�
	static CEntryRuleManager* Create(CEntry_Dressup* pParent);		// ����
	static void Release(CEntryRuleManager*& prEntryRuleManager);	// �j��
	static HRESULT SaveSetting(const SRule& rRule, const std::string& rNameLeft, const std::string& rNameRight);	// �Q�[���ݒ�ۑ�
	static HRESULT LoadRule(SRule* pRule);										// �Q�[���ݒ�Ǎ�
	static HRESULT LoadName(std::string* pName, CGameManager::ETeamSide team);	// �`�[�����Ǎ�

private:
	// �����o�֐�
	void UpdateFadeIn();	// �t�F�[�h�C���X�V
	void UpdateFadeOut();	// �t�F�[�h�A�E�g�X�V
	void UpdateControl();	// ����X�V
	void UpdateArrow();		// ���X�V
	void UpdateTimeUI();	// ���ԕ\���X�V
	void Select();			// �I�𑀍�
	void Decide();			// ���葀��
	void ChangeRule();		// ���[���ύX����
	CObject* GetTimeUI();	// ���ԕ\��UI�擾
	void AddTime(const float fAddTime);		// ���ԉ��Z
	void SetEnableDispUI(const bool bDisp);	// UI�`��ݒ�
	void SetAlphaUI(const float fAlpha, const bool bFadeOut);	// UI�����x�ݒ�

	// �����o�ϐ�
	CObject2D_Anim*	m_apRuleTitle[RULE_MAX];	// ���[���^�C�g���̏��
	CArrowUI*	m_apArrow[MAX_RULE_ARROW];		// ���̏��
	CObject2D_Anim*	m_pLife;	// �̗͂̏��
	CTimeUI*	m_pTime;		// ���Ԃ̏��
	CObject2D*	m_pTimeInf;		// ���ԁ��̏��
	CObject2D*	m_pSelect;		// �I���̏��
	CObject2D*	m_pStart;		// �J�n�{�^���̏��
	CObject2D*	m_pControl;		// ����\���̏��
	CObject2D*	m_pBG;			// �w�i�̏��
	CObject2D*	m_pFade;		// �t�F�[�h�̏��
	CEntry_Dressup* m_pParent;	// �e�̏��
	EState	m_state;			// ���
	int		m_nSelect;			// ���݂̑I��
	int		m_nOldSelect;		// �O��̑I��
	float	m_fSinControlAlpha;	// ����\���̓�������
	float	m_fSinArrowAlpha;	// ���\���̓�������
};

#endif	// _ENTRYRULE_MANAGER_H_

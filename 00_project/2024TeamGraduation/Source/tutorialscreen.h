//=============================================================================
//
//  �`���[�g���A����ʃw�b�_�[ [tutorialscreen.h]
//  Author�F���n�Ή�
//
//=============================================================================
//==========================================================================
// ��d�C���N���[�h�h�~
//==========================================================================
#ifndef _TUTORIAL_SCREEN_H_
#define _TUTORIAL_SCREEN_H_

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �`���[�g���A����ʃN���X
class CTutorialScreen : public CObject2D
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// �����̎��
	enum EManualType
	{
		TYPE_MOVE = 0,		// �ړ�
		TYPE_BLINK,			// �u�����N
		TYPE_JUMP,			// �W�����v
		TYPE_THROW,			// ����
		TYPE_PASS,			// �p�X
		TYPE_CATCH,			// �L���b�`
		TYPE_SP,			// �X�y�V����
		TYPE_JUSTCATCH,		// �W���X�g�L���b�`
		TYPE_MAX
	};

	// ���
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_SPAWN,	// �o��
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	// �X�N���[������
	enum EScrollDir
	{
		SCROLL_R = 0,
		SCROLL_L,
		SCROLL_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CTutorialScreen(int nPriority = mylib_const::PRIORITY_DEF2D);
	~CTutorialScreen() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	// �����o�֐�
	void SetState(EState state);	// ��Ԑݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CTutorialScreen* Create();	// ����

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTutorialScreen::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����Ȃ�
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �o��
	void StateFadeIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �t�F�[�h�C��
	void StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �t�F�[�h�A�E�g

	// ����
	HRESULT CreateBG();			// �w�i����
	HRESULT CreateFade();		// �t�F�[�h����
	HRESULT CreateManual();		// ��������
	HRESULT CreateManualText();	// �����e�L�X�g����

	// �X�V
	void UpdateBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �w�i�X�V
	void ChangeManual();	// �����̕ύX����

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[

	// �I�u�W�F�N�g
	CObject2D* m_pFade;			// �t�F�[�h�̏��
	CObject2D* m_pManual;		// �����̏��
	CObject2D* m_pManualText;	// �����e�L�X�g�̏��

	// ���̑�
	EManualType m_ManualType;	// �����̎��
	EScrollDir m_scrollDir;		// �X�N���[������

};

#endif	// _ARROW_UI_H_

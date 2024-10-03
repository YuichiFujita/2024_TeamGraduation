//=============================================================================
// 
//  �^�C�g���̃I�v�V�����I�����w�b�_�[ [title_optionselect.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TITLE_OPTIONSELECT_H_
#define _TITLE_OPTIONSELECT_H_	// ��d�C���N���[�h�h�~

#include "object.h"

class CObject2D;
class CScroll;
class COptionMenu;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g���̃I�v�V�����I�����N���X
class CTitle_OptionSelect : public CObject
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_SCROLLWAIT,	// �������҂�
		STATE_SCROLLWAIT_CLOSE,	// �������҂�(��)
		STATE_SELECT,	// �I��
		STATE_EDIT,		// �ҏW
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	enum Select
	{
		SELECT_KEYKONFIG = 0,	// �L�[�R���t�B�O
		SELECT_SOUND,			// �T�E���h
		SELECT_OTHER,			// ���̑�
		SELECT_MAX
	};

	CTitle_OptionSelect(int nPriority = 7);
	~CTitle_OptionSelect();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();						// �폜
	void SetState(const STATE& state);		// ��Ԑݒ�
	STATE GetState() { return m_state; }	// ��Ԏ擾

	CScroll* GetScroll() { return m_pScroll; }	// �������擾

	void SetBackSelect();	// �I�����ɖ߂�ݒ�

	//=============================
	// �ÓI�֐�
	//=============================
	static CTitle_OptionSelect* Create();	// ��������

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CTitle_OptionSelect::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();	// ��ԍX�V
	void StateNone();		// �Ȃɂ��Ȃ�
	void StateScrollWait();	// �������҂�
	void StateScrollWait_Close();	// �������҂�
	void StateSelect();		// �I��
	void StateEdit();		// �ҏW
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeOut();	// �t�F�[�h�A�E�g

	// ���̑�
	void CreateSelect();		// �I��������
	void ChangeOptionMenu();	// �I�v�V�������j���[�؂�ւ�
	void ResetSelect();			// �I�������Z�b�g

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	STATE m_state;			// ���
	float m_fStateTimer;	// ��ԃ^�C�}�[

	// ���̑�
	Select m_select;		// �I����
	Select m_Oldselect;		// �O��̑I����
	CObject2D* m_pSelect[Select::SELECT_MAX];	// �I�����̃I�u�W�F�N�g
	CScroll* m_pScroll;		// ������
	COptionMenu* m_pOptionMenu;	// �I�v�V�������j���[
};


#endif
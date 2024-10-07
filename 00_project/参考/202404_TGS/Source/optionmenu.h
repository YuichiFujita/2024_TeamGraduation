//=============================================================================
// 
//  �I�v�V�������j���[�w�b�_�[ [optionmenu.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OPTIONMENU_H_
#define _OPTIONMENU_H_	// ��d�C���N���[�h�h�~

#include "object.h"
#include "title_optionselect.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�v�V�������j���[�N���X
class COptionMenu : public CObject
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_EDIT,		// �ҏW
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	COptionMenu(int nPriority = 7);
	~COptionMenu();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();						// �폜
	void SetState(const STATE& state);			// ��Ԑݒ�
	STATE GetState() { return m_state; }		// ��Ԏ擾
	bool IsNowChange() { return m_bNowChange; }	// �ύX�����ۂ�

	//=============================
	// �ÓI�֐�
	//=============================
	static COptionMenu* Create(CTitle_OptionSelect::Select selectType);	// ��������

protected:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(COptionMenu::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();	// ��ԍX�V
	void StateNone();		// �Ȃɂ��Ȃ�
	virtual void StateEdit();		// �ҏW
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeOut();	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	STATE m_state;			// ���
	float m_fStateTimer;	// ��ԃ^�C�}�[
	bool m_bNowChange;		// �ύX�����ۂ�
};


#endif
//=============================================================================
// 
//  ���̑����j���[�w�b�_�[ [optionmenu_other.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OPTIONMENU_OTHER_H_
#define _OPTIONMENU_OTHER_H_	// ��d�C���N���[�h�h�~

#include "optionmenu.h"

class CMultiNumber;
class CSelectDraw;

//==========================================================================
// �N���X��`
//==========================================================================
// ���̑����j���[�N���X
class COptionMenu_Other : public COptionMenu
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	enum Select
	{
		SELECT_WINDOW = 0,	// �E�B���h�E���[�h
		SELECT_VIB,			// �o�C�u���[�V����
		SELECT_MAX
	};

	enum Active	// �A�N�e�B�u���
	{
		ON = 0,
		OFF,
		MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	struct SwitchInfo
	{
		Active active;		// �A�N�e�B�u���
		CObject2D* pSwitch;	// �؂�ւ�

		SwitchInfo() : active(Active::ON), pSwitch(nullptr) {}
	};

	COptionMenu_Other(int nPriority = 7);
	~COptionMenu_Other();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Kill() override;	// �폜

private:

	//=============================
	// �����o�֐�
	//=============================
	virtual void StateEdit() override;	// �ҏW
	void CreateText();					// �e�L�X�g����
	void CreateSwitch();				// �؂�ւ�����
	void ActiveByTexture(Select _select, Active _active);	// �A�N�e�B�u��ԕʃe�N�X�`��
	void ResetSelect();				// �I�������Z�b�g

	//=============================
	// �����o�ϐ�
	//=============================
	Select m_selectType;					// �I�𒆂̎��
	CObject2D* m_pText[Select::SELECT_MAX];	// �e�L�X�g
	SwitchInfo m_switchInfo[Select::SELECT_MAX];	// �؂�ւ����
	CSelectDraw* m_pDrawing;				// �I�����M
};


#endif
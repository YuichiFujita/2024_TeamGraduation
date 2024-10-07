//=============================================================================
// 
//  �T�E���h���j���[�w�b�_�[ [optionmenu_sound.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OPTIONMENU_SOUND_H_
#define _OPTIONMENU_SOUND_H_	// ��d�C���N���[�h�h�~

#include "optionmenu.h"

class CMultiNumber;
class CSelectDraw;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�v�V�������j���[�N���X
class COptionMenu_Sound : public COptionMenu
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	enum Select
	{
		SELECT_MASTER = 0,
		SELECT_SE,
		SELECT_BGM,
		SELECT_MAX
	};

	COptionMenu_Sound(int nPriority = 7);
	~COptionMenu_Sound();

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
	virtual void StateEdit() override;		// �ҏW
	void CreateText();	// �e�L�X�g����
	void ResetSelect();	// �I�������Z�b�g

	//=============================
	// �����o�ϐ�
	//=============================
	Select m_selectType;					// �I�𒆂̎��
	CMultiNumber* m_pNumber_Master;			// �}�X�^�[�{�����[���̐���
	CMultiNumber* m_pNumber_SE;				// SE�̐���
	CMultiNumber* m_pNumber_BGM;			// BGM�̐���
	CObject2D* m_pText[Select::SELECT_MAX];	// �e�L�X�g
	CSelectDraw* m_pDrawing;				// �I�����M
};


#endif
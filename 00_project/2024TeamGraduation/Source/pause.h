//=============================================================================
// 
//  �|�[�Y�w�b�_�[ [pause.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PAUSE_H_
#define _PAUSE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"
#include "manager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �|�[�Y�N���X��`
class CPause
{
public:

	CPause();
	~CPause();

	// �����o�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void Kill();	// �폜
	void SetPause();	// �g�p�󋵕ύX
	bool IsPause();		// �g�p�󋵎擾

	static CPause *Create(CScene::MODE mode);	// ����

protected:

	// �񋓌^��`
	enum VTX
	{
		VTX_FADE = 0,		// ������
		VTX_WINDOW,			// ���j���[�E�B���h�E
		VTX_CONTINUE,		// �Q�[���ɖ߂�
		VTX_RETRY,			// ���g���C
		VTX_QUIT,			// �^�C�g���ɖ߂�	
		VTX_MAX
	};

	// ���j���[
	enum MENU
	{
		MENU_RETURNGAME = 0,	// �|�[�Y�̉���
		MENU_RETRY,				// ���g���C
		MENU_RETURNTITLE,		// �^�C�g���ɖ߂�	
		MENU_MAX
	};

	void UpdateSelect();	// �I�����̍X�V
	void UpdateColor();		// �F�X�V
	void Decide();			// ���菈��
	virtual void DecideByMode() = 0;	// ���[�h�ʌ��菈��

	CObject2D* m_aObject2D[VTX_MAX];				// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	bool m_bPause;									// �|�[�Y�̔���
	int m_nSelect;									// �I����
	float m_fFlashTime;								// �_�Ŏ���
};

#endif
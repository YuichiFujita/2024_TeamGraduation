//=============================================================================
// 
//  �^�C�g���w�b�_�[ [tutorial.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_	// ��d�C���N���[�h�h�~

#include "manager.h"

class CTutorialLogo;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g���N���X
class CTutorial : public CScene
{
public:

	CTutorial();
	~CTutorial();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// �ÓI�֐�
	static CTutorial* GetInstance();	// �C���X�^���X�擾
	static CTutorial* Create();		// ��������

private:

	CTutorialLogo* m_pLogo;		// ���S�̃|�C���^
	static CTutorial* m_pThisPtr;	// ���g�̃|�C���^
};



#endif
//=============================================================================
// 
// �^�C�g�����S���� [tutoriallogo.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TUTORIALLOGO_H_
#define _TUTORIALLOGO_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"


//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g�����S
class CTutorialLogo : public CObject2D
{
public:

	CTutorialLogo(int nPriority = 8);
	~CTutorialLogo();
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CTutorialLogo* Create();	// ��������

private:


};

#endif
//=============================================================================
// 
//  �`���[�g���A�����|�[�Y�w�b�_�[ [pause_tutorial.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PAUSE_TUTORIAL_H_
#define _PAUSE_TUTORIAL_H_	// ��d�C���N���[�h�h�~

#include "pause.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �`���[�g���A�����|�[�Y�N���X
class CPause_Tutorial : public CPause
{
public:

	CPause_Tutorial();
	~CPause_Tutorial();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;

private:
	
	void DecideByMode() override;	// ���[�h�ʌ��菈��

};

#endif
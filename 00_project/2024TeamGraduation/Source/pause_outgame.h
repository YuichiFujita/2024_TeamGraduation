//=============================================================================
// 
//  �A�E�g�Q�[�����|�[�Y�w�b�_�[ [pause_outgame.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PAUSE_OUTGAME_H_
#define _PAUSE_OUTGAME_H_	// ��d�C���N���[�h�h�~

#include "pause.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �A�E�g�Q�[�����|�[�Y�N���X
class CPause_OutGame : public CPause
{
public:

	CPause_OutGame();
	~CPause_OutGame();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

private:
	
	void DecideByMode() override;	// ���[�h�ʌ��菈��

};

#endif
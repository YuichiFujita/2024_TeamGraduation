//=============================================================================
// 
// �L���b�`���ʏ��� [catchresult_OK.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CATCHRESULT_OK_H_
#define _CATCHRESULT_OK_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "catchresult.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �L���b�`����
class CCatchResult_OK : public CCatchResult
{
public:

	CCatchResult_OK(int nPriority = 8);
	~CCatchResult_OK();

	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;

private:

	//=============================
	// �����o�֐�
	//=============================
	void StateExpansion() override;	// �g��
	void StateExpNone() override;	// �g��㉽�����Ȃ�
	void StateFadeOut() override;	// �t�F�[�h�A�E�g���

};

#endif
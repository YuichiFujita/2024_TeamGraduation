//=============================================================================
// 
// �L���b�`���ʏ��� [catchresult_FAIL.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CATCHRESULT_FAIL_H_
#define _CATCHRESULT_FAIL_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "catchresult.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �L���b�`����
class CCatchResult_FAIL : public CCatchResult
{
public:

	CCatchResult_FAIL(int nPriority = 8);
	~CCatchResult_FAIL();

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
//=============================================================================
// 
//  �R�[�X�w�b�_�[ [course_title.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _COURSE_RESULT_H_
#define _COURSE_RESULT_H_	// ��d�C���N���[�h�h�~

#include "course.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �R�[�X�N���X
class CCourse_Title : public CCourse
{

public:

	CCourse_Title(int nPriority = 2, const LAYER layer = LAYER::LAYER_MAP);
	~CCourse_Title();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();

	
};



#endif
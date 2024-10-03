//=============================================================================
// 
// �A�T�[�g�w�b�_�[ [myassert.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ASSERT_H_
#define _ASSERT_H_		// ��d�C���N���[�h�h�~

#include "calculation.h"

/**
@brief	�A�T�[�g
*/
namespace MyAssert
{
	/**
	@brief	�G���[�\��
	@param	frag	[in]	�t���O
	@param	message	[in]	�G���[���b�Z�[�W
	@return	void
	*/
	inline void CustomAssert(bool frag, const std::string message) 
	{
		if (!frag)
		{
			MessageBoxA(NULL, message.c_str(), "Assertion Failed", MB_OK | MB_ICONERROR);

			 exit(EXIT_FAILURE);
			__debugbreak();  // �f�o�b�O���ɒ�~����
		}
	}

}

#endif
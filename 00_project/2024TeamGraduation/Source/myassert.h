//=============================================================================
// 
// アサートヘッダー [myassert.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ASSERT_H_
#define _ASSERT_H_		// 二重インクルード防止

#include "calculation.h"

/**
@brief	アサート
*/
namespace MyAssert
{
	/**
	@brief	エラー表示
	@param	frag	[in]	フラグ
	@param	message	[in]	エラーメッセージ
	@return	void
	*/
	inline void CustomAssert(bool frag, const std::string message) 
	{
		if (!frag)
		{
			MessageBoxA(NULL, message.c_str(), "Assertion Failed", MB_OK | MB_ICONERROR);

			 exit(EXIT_FAILURE);
			__debugbreak();  // デバッグ中に停止する
		}
	}

}

#endif
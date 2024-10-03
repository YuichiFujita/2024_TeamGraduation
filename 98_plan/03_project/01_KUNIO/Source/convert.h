//=============================================================================
// 
// 変換ヘッダー [convert.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CONVERT_H_
#define _CONVERT_H_		// 二重インクルード防止

#include "calculation.h"
#include "course.h"

/**
@brief	変換
*/
namespace MyConvert
{
	/**
		@brief	VtxInfoをVector3に変換
		@return	Vector3
	*/
	MyLib::Vector3 VtxInfoToVector3(const CCourse::VtxInfo& v);

	/**
		@brief	Vector3をVtxInfoに変換
		@return	VtxInfo
	*/
	CCourse::VtxInfo Vector3ToVtxInfo(const MyLib::Vector3& v);

}

#endif
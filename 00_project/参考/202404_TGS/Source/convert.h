//=============================================================================
// 
// �ϊ��w�b�_�[ [convert.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CONVERT_H_
#define _CONVERT_H_		// ��d�C���N���[�h�h�~

#include "calculation.h"
#include "course.h"

/**
@brief	�ϊ�
*/
namespace MyConvert
{
	/**
		@brief	VtxInfo��Vector3�ɕϊ�
		@return	Vector3
	*/
	MyLib::Vector3 VtxInfoToVector3(const CCourse::VtxInfo& v);

	/**
		@brief	Vector3��VtxInfo�ɕϊ�
		@return	VtxInfo
	*/
	CCourse::VtxInfo Vector3ToVtxInfo(const MyLib::Vector3& v);

}

#endif
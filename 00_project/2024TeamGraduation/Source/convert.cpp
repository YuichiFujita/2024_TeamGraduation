//==========================================================================
// 
// �ϊ����� [convert.cpp]
// Author : ���n�Ή�
// 
//==========================================================================
#include "convert.h"

/**
	@brief	VtxInfo��Vector3�ɕϊ�
	@return	Vector3
*/
MyLib::Vector3 VtxInfoToVector3(const CCourse::VtxInfo& v)
{
	return v.pos;
}


/**
	@brief	Vector3��VtxInfo�ɕϊ�
	@return	MyStruct
*/
CCourse::VtxInfo MyConvert::Vector3ToVtxInfo(const MyLib::Vector3& v)
{
	return CCourse::VtxInfo(v, 0.0f);
}

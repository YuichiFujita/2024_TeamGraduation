//==========================================================================
// 
// •ÏŠ·ˆ— [convert.cpp]
// Author : ‘Š”nèÎ‰ë
// 
//==========================================================================
#include "convert.h"

/**
	@brief	VtxInfo‚ğVector3‚É•ÏŠ·
	@return	Vector3
*/
MyLib::Vector3 VtxInfoToVector3(const CCourse::VtxInfo& v)
{
	return v.pos;
}


/**
	@brief	Vector3‚ğVtxInfo‚É•ÏŠ·
	@return	MyStruct
*/
CCourse::VtxInfo MyConvert::Vector3ToVtxInfo(const MyLib::Vector3& v)
{
	return CCourse::VtxInfo(v, 0.0f);
}

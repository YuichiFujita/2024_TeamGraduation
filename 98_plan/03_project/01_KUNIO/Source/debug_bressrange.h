//=============================================================================
// 
// デバッグ息範囲処理 [debug_bressrange.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _DEBUG_BRESSRANGE_H_
#define _DEBUG_BRESSRANGE_H_		// 二重インクルード防止のマクロを定義する

#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// デバッグ息範囲クラス
class CDebugBressRange : public CObjectBillboard
{
public:

	CDebugBressRange(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CDebugBressRange();

	static CDebugBressRange *Create();

	//  オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	void SetRange(const MyLib::Vector3& leftup, const MyLib::Vector3& rightup, const MyLib::Vector3& leftdw, const MyLib::Vector3& rightdw);

private:

	// メンバ関数
	

	// メンバ変数
	MyLib::Vector3 m_LeftUP;
	MyLib::Vector3 m_RightUP;
	MyLib::Vector3 m_LeftDW;
	MyLib::Vector3 m_RightDW;
};

#endif
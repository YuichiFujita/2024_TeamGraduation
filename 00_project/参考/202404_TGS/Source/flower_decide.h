//=============================================================================
// 
//  完成花ヘッダー [flower_decide.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _FLOWER_DECIDE_H_
#define _FLOWER_DECIDE_H_	// 二重インクルード防止

#include "objectX.h"

//==========================================================================
// クラス定義
//==========================================================================
// 完成花クラス定義
class CDecideFlower : public CObjectX
{
public:

	CDecideFlower(int nPriority = 6);
	~CDecideFlower();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	static CDecideFlower* Create(const MyLib::Vector3& pos, const MyLib::Vector3& move);	// 生成

private:

	//=============================
	// 関数リスト
	//=============================

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
};


#endif
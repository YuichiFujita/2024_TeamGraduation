//=============================================================================
// 
// 拡縮ハンドル処理 [handle_Scale.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _HANDLE_SCALE_H_
#define _HANDLE_SCALE_H_		// 二重インクルード防止のマクロを定義する

#include "edithandle.h"

//==========================================================================
// クラス定義
//==========================================================================
// 拡縮ハンドルクラス
class CHandle_Scale : public CHandle
{
public:

	CHandle_Scale(int nPriority = 8);
	~CHandle_Scale();

	//  オーバーライド関数
	HRESULT Init() override;

	SEditHandleInfo GetDiff(HandleAngle angle) override;	// 移動差分取得
	void Grab() override;	// 掴み中処理


private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_DiffMove;	// 移動の差分
	MyLib::Vector3 m_DiffScale;	// スケールの差分
};
#endif
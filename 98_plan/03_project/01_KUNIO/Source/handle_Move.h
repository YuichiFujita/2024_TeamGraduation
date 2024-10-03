//=============================================================================
// 
// 移動ハンドル処理 [handle_Move.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _HANDLE_MOVE_H_
#define _HANDLE_MOVE_H_		// 二重インクルード防止のマクロを定義する

#include "objectX.h"
#include "edithandle.h"
class CObjectLine;

//==========================================================================
// クラス定義
//==========================================================================
// 移動ハンドルクラス
class CHandle_Move : public CHandle
{
public:

	CHandle_Move(int nPriority = 8);
	~CHandle_Move();

	//  オーバーライド関数
	HRESULT Init() override;

	SEditHandleInfo GetDiff(HandleAngle angle);	// 差分取得
	void Grab() override;	// 掴み中処理

private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_DiffMove;				// 移動の差分
};
#endif
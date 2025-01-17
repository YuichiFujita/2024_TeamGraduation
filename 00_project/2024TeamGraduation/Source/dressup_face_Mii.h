//=============================================================================
//
// 着せ替え(顔)処理 [dressup_face_Mii.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _DRESSUP_FACE_MII_H_		// このマクロ定義がされていなかったら
#define _DRESSUP_FACE_MII_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "dressup_face.h"

//==========================================================================
// クラス定義
//==========================================================================
// 着せ替え(顔)クラス
class CDressup_Face_Mii : public CDressup_Face
{
public:

	CDressup_Face_Mii();
	~CDressup_Face_Mii();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了

private:

};

#endif
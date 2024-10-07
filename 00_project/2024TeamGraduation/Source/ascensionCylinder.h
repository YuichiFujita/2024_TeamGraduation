//=============================================================================
// 
//  目標地点ヘッダー [ascensionCylinder.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SSCENSIONCYLINDER_H_
#define _SSCENSIONCYLINDER_H_	// 二重インクルード防止

#include "meshcylinder.h"

//==========================================================================
// クラス定義
//==========================================================================
// 目標地点クラス定義
class CAscensionCylinder : public CMeshCylinder
{
public:
	CAscensionCylinder(int nPriority = 6);
	~CAscensionCylinder();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CAscensionCylinder *Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen);

private:

};



#endif
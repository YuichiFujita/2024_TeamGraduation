//=============================================================================
// 
//  キーコンフィグに応じたボタン表示 [controlkeydisp.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _KEYDISP_H_
#define _KEYDISP_H_	// 二重インクルード防止

#include "object2D.h"
#include "input_gamepad.h"
#include <map>

//==========================================================================
// クラス定義
//==========================================================================
class CControlKeyDisp : public CObject2D
{
public:

	CControlKeyDisp(int nPriority = 11);
	~CControlKeyDisp();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Kill();	// 削除
	static CControlKeyDisp* Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float width, float height, int type);	// 生成処理

	void SetType(int type);
	static void Load(CInputGamepad::BUTTON button, int nIdx) { m_buttonTextures[button] = nIdx; }

private:

	//=============================
	// メンバ変数
	//=============================
	static std::map<CInputGamepad::BUTTON, int> m_buttonTextures;
};


#endif
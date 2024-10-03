//=============================================================================
// 
//  戦闘開始ヘッダー [battlestart_effect.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _BATTLESTART_EFFECT_H_
#define _BATTLESTART_EFFECT_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 戦闘開始クラス
class CBattleStart_Effect : public CObject2D
{
public:

	CBattleStart_Effect(int nPriority = 10);
	~CBattleStart_Effect();


	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBattleStart_Effect* Create(const MyLib::Vector3& pos);

private:

	//=============================
	// メンバ変数
	//=============================
	float m_fLife;	// 寿命
};


#endif
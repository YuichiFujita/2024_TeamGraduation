//=============================================================================
// 
//  戦闘準備ヘッダー [beforebattle.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _BEFOREBATTLE_H_
#define _BEFOREBATTLE_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 戦闘準備クラス
class CBeforeBattle : public CObject2D
{
public:

	CBeforeBattle(int nPriority = 7);
	~CBeforeBattle();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBeforeBattle* Create(const MyLib::Vector3& pos);

private:

	//=============================
	// メンバ変数
	//=============================
	CObject2D* m_pAnyButton;	// 「キー押して」のテキスト
};


#endif
//=============================================================================
// 
//  スコアヘッダー [resultscore.h]
//  Author : 髙田佳依
// 
//=============================================================================

#ifndef _BATTLERESULT_H_
#define _BATTLERESULT_H_	// 二重インクルード防止

#include "object2d.h"
#include "multinumber.h"

//==========================================================================
// クラス定義
//==========================================================================
// 戦果クラス
class CResultScore : public CObject
{
public:

	CResultScore(int nPriority = 11);
	~CResultScore();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// 削除
	static CResultScore* Create(int score);	// 生成処理

private:

	//=============================
	// メンバ関数
	//=============================
	void CreateScore();

	//=============================
	// メンバ変数
	//=============================
	int m_nScore;
	float m_fStateTime;
	CObject2D* m_pObj2D;
	CMultiNumber* m_pScore;			// 生成数スコア

};


#endif
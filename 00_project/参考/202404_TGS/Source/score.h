//=============================================================================
// 
//  スコアヘッダー [score.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_	// 二重インクルード防止

#include "main.h"
#include "resultmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CMultiNumber;

//==========================================================================
// クラス定義
//==========================================================================
// 背景クラス定義
class CScore
{
public:

	CScore(int nPriority = 6);
	~CScore();

	static CScore *Create();
	static CScore *Create(MyLib::Vector3 pos);
	void SetPosition(const MyLib::Vector3 pos);
	MyLib::Vector3 GetPosition() const;

	// オーバーライドされた関数
	HRESULT Init();
	HRESULT Init(MyLib::Vector3 pos);
	void Uninit();
	void Update();
	void Draw();

	void Add(int nValue);

private:
	
	void SetValue();

	int m_nNum;								// 値
	CMultiNumber *m_apNumber;			// 数字のオブジェクト
	int m_nTexIdx;					// テクスチャのインデックス番号
};



#endif
//=============================================================================
// 
//  リザルトヘッダー [result.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_	// 二重インクルード防止

#include "main.h"
#include "manager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CResultScore;
class CPeopleManager;

//==========================================================================
// クラス定義
//==========================================================================
// リザルトクラス定義
class CResult : public CScene
{
public:

	CResult();
	~CResult();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CResultScore *GetResultScore();

private:

	// メンバ変数
	bool m_clear; // クリア判定
	CPeopleManager* m_pPeopleManager;			// 人マネージャ
	static CResultScore *m_pResultScore;
};



#endif
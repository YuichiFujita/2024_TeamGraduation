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
class CResultManager;

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
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

private:

	// メンバ変数
	CResultManager* m_pResultManager;			// リザルトマネージャのオブジェクト
};

#endif
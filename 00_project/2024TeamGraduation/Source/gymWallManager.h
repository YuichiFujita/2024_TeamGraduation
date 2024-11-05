//=============================================================================
// 
//  サンプル_オブジェクトXヘッダー [sample_objX.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SAMPLE_OBJX_H_
#define _SAMPLE_OBJX_H_		// 二重インクルード防止

#include "object.h"

// 前方宣言
class CGymWall;

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクトXクラス
class CGymWallManager : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum Wall
	{
		WALL_OFF = 0,
		WALL_ON,
		WALL_MAX
	};

	CGymWallManager(int nPriority = 6);
	~CGymWallManager();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	void Kill();		// 削除
	void SetIsWall(bool set) { m_bWall = set; }

	//=============================
	// 静的関数
	//=============================
	/**
	@brief		生成処理
	@details	必要があれば引数追加
	*/
	static CGymWallManager *Create();

private:

	//=============================
	// メンバ関数
	//=============================
	void State();

	//=============================
	// メンバ変数
	//=============================
	CGymWall* m_pGymWall;			// 壁ポインタ
	bool m_bWall;
};


#endif
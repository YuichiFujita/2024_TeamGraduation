//=============================================================================
// 
//  ジムウォールヘッダー [sample_objX.h]
//  Author : Takeru
// 
//=============================================================================

#ifndef _GYM_WALL_H_
#define _GYM_WALL_H_		// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクトXクラス
class CGymWall : public CObjectX
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	enum Sample
	{
		SAMPLE_WAO = 0,
		SAMPLE_WHO,
		SAMPLE_MAX
	};

	CGymWall(int nPriority = 6);
	~CGymWall();

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

	//=============================
	// 静的関数
	//=============================
	/**
	@brief		生成処理
	@details	必要があれば引数追加
	*/
	static CGymWall *Create();
	static CListManager<CGymWall> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CGymWall::*SAMPLE_FUNC)();
	static SAMPLE_FUNC m_SampleFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void SampleWho();	// フー
	void SampleWao();	// ワオ

	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;				// 状態カウンター
	Sample m_state;					// 状態
	static CListManager<CGymWall> m_List;	// リスト

};


#endif
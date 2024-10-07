//=============================================================================
// 
//  サンプル_オブジェクトXヘッダー [sample_objX.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SAMPLE_OBJX_H_
#define _SAMPLE_OBJX_H_		// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクトXクラス
class CSample_ObjX : public CObjectX
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

	CSample_ObjX(int nPriority = 6);
	~CSample_ObjX();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
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
	static CSample_ObjX *Create();
	static CListManager<CSample_ObjX> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CSample_ObjX::*SAMPLE_FUNC)();
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
	static CListManager<CSample_ObjX> m_List;	// リスト

};


#endif
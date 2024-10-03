//=============================================================================
// 
//  サンプル_オブジェクトXヘッダー [sample_objX.h]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================

#ifndef _GOALFLAG_H_
#define _GOALFLAG_H_		// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクトXクラス
class CGoalflagX : public CObjectX
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

	CGoalflagX(int nPriority = 6);
	~CGoalflagX();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// メンバ関数
	void Kill();		// 削除

	//=============================
	// 静的関数
	//=============================
	static CGoalflagX* Create(float fLength);
	static CListManager<CGoalflagX> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CGoalflagX::* GOALFLAG_FUNC)();
	static GOALFLAG_FUNC m_GoalflagFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void SampleWho();	// フー
	void SampleWao();	// ワオ

	// その他
	void Swing();	// 揺れ

	//=============================
	// メンバ変数
	//=============================
	bool m_bClear;					// クリア
	float m_fStateTime;				// 状態カウンター
	Sample m_state;					// 状態
	MyLib::Vector3 m_DestRot_Old;	// 目標の向き
	MyLib::Vector3 m_DestRot;		// 目標の向き
	static CListManager<CGoalflagX> m_List;	// リスト

};


#endif
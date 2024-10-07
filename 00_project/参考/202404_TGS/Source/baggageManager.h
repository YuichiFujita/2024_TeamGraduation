//=============================================================================
// 
//  荷物マネージャヘッダー [baggageManager.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _BAGGAGE_MANAGER_H_
#define _BAGGAGE_MANAGER_H_		// 二重インクルード防止

#include "listmanager.h"
#include "baggage.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物マネージャクラス
class CBaggageManager
{
public:

	CBaggageManager();
	~CBaggageManager();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	//=============================
	// メンバ関数
	//=============================
	void Kill();	// 削除

	// 情報系
	void SetBaggageInfo(const std::vector<CBaggage::SBaggageInfo>& info) { m_BaggageInfo = info; }	// 荷物情報設定
	void SetBaggageInfo(const CBaggage::SBaggageInfo& info, int idx) { m_BaggageInfo[idx] = info; }	// 荷物情報設定
	std::vector<CBaggage::SBaggageInfo> GetBaggageInfo() { return m_BaggageInfo; }					// 荷物情報取得
	CBaggage::SBaggageInfo GetBaggageInfo(int idx) { return m_BaggageInfo[idx]; }						// 荷物情報取得

	// 生成
	CBaggage* CreateBaggage(CBaggage::TYPE idx);

	// 入出力
	void Save();	// セーブ
	void Load();	// ロード

	//=============================
	// 静的関数
	//=============================
	static CBaggageManager* Create();	// 生成
	static CBaggageManager* GetInstance() { return m_ThisPtr; }	// インスタンス取得
private:

	//=============================
	// メンバ変数
	//=============================
	std::vector<CBaggage::SBaggageInfo> m_BaggageInfo;	// 障害物情報
	static CBaggageManager* m_ThisPtr;		// 自身のポインタ
};


#endif
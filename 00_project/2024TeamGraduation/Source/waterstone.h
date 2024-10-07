//=============================================================================
// 
//  水中石ヘッダー [waterstone.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _WATERSTONE_H_
#define _WATERSTONE_H_		// 二重インクルード防止

#include "objectX.h"
#include "waterstoneManager.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水中石クラス
class CWaterStone : public CObjectX
{
public:
	
	CWaterStone(int nPriority = 6);
	~CWaterStone();

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
	void SetStoneInfo(const CWaterStone_Manager::SStoneInfo& info) { m_StoneInfo = info; }	// 石情報設定
	CWaterStone_Manager::SStoneInfo GetStoneInfo() { return m_StoneInfo; }	// 石情報取得
	void ChangeModelType(int type);	// モデルの種類変更

	//=============================
	// 静的関数
	//=============================
	/**
	@brief		生成処理
	@details	必要があれば引数追加
	*/
	static CWaterStone* Create(const CWaterStone_Manager::SStoneInfo& info);
	static CListManager<CWaterStone> GetListObj() { return m_List; }				// リスト取得
	static int GetModelNum() { return static_cast<int>(m_vecModelFile.size()); }	// モデルの種類数取得
private:

	//=============================
	// メンバ関数
	//=============================


	//=============================
	// メンバ変数
	//=============================
	float m_fSplashTimer;		// しぶきタイマー
	float m_fIntervalSplash;	// しぶきまでのインターバル
	CWaterStone_Manager::SStoneInfo m_StoneInfo;	// 石情報
	static const std::vector<std::string> m_vecModelFile;	// モデルファイルのコンテナ
	static CListManager<CWaterStone> m_List;		// リスト
};


#endif
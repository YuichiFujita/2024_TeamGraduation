//=============================================================================
// 
//  ランキング2D_TOP3ヘッダー[rankinfItem_top3.h]
//  Author : 澗口将太郎
// 
//=============================================================================

#ifndef _RANKINGITEM_TOP3_H_
#define _RANKINGITEM_TOP3_H_	// 二重インクルード防止

#include "rankingItem.h"

//==========================================================================
// クラス定義
//==========================================================================
// 戦果クラス
class CRankingItem_top3 : public CRankingItem
{
public:

	//列挙型
	typedef enum Items_TOP3
	{
		ITEM_NUM_RANK = 0,
		ITEM_ALLRANK,
		ITEM_TIME,
		ITEM_DATE,
		ITEM_ALLRANK_DATA,
		ITEM_YOU,
		ITEM_MAX
	};

	CRankingItem_top3(int nPriority = 11);
	~CRankingItem_top3();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// 削除
	MyLib::Vector3 GetPos() override;
	void SetMove(MyLib::Vector3 move) override;	//ランキングアイテムの移動値セット
	static CRankingItem_top3* Create(int nNumRank, int nMin_Time, int nSec_Time, int nMilliSec_Time,int nAllRank,int nDate[3], MyLib::Vector3 posAll,float fSize, bool bNewRecord);	// 生成処理

private:
	//構造体
	typedef struct Time
	{
		int nMinutes;
		int nSeconds;
		int nMilliSeconds;
	};
	
	/*void SetScoreData()*/
	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;
	CObject2D* m_pItem[ITEM_MAX];	//項目数
	CObject2D* m_pScoreItem[8];		//スコアの桁毎obj
	CObject2D* m_pDateItem[10];		//日付データobj
	int m_nNumRank;					//順位
	Time m_nTime;					//タイム

	int m_nDate[3];					//日付データ
	
	int m_nAllRank;					//総評の値
	static int m_nObjNum;			//当オブジェクトの生成数

	MyLib::Vector3 m_posAll;		//全項目の基準位置
	MyLib::Vector3 m_posItem[ITEM_MAX];//各項目の位置
	MyLib::Vector3 m_moveAll;		//移動値
	float m_fSize;					//拡大率

	bool m_bUpdate;					//更新されたスコアか？
};


#endif
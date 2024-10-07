//=============================================================================
// 
//  ランキング2Dヘッダー[rankingItem.h]
//  Author : 澗口将太郎
// 
//=============================================================================

#ifndef _RANKINGITEM_H_
#define _RANKINGITEM_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// 戦果クラス
class CRankingItem : public CObject
{
public:

	//列挙型
	typedef enum Items
	{
		ITEM_NUM_RANK = 0,
		ITEM_ALLRANK,
		ITEM_TIME,
		ITEM_ALLRANK_DATA,
		ITEM_YOU,
		ITEM_MAX
	};
	

	CRankingItem(int nPriority = 11);
	~CRankingItem();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual MyLib::Vector3 GetPos();
	virtual void SetMove(MyLib::Vector3 move);	//ランキングアイテムの移動値セット
	void Kill();	// 削除
	static CRankingItem* Create(int nNumRank,int nMin_Time,int nSec_Time,int nMilliSec_Time,int nAllRank, MyLib::Vector3 posAll,float fSize,bool bNewRecord);	// 生成処理

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
	CObject2D* m_pScoreItem[6];		//スコアの桁毎obj
	int m_nNumRank;					//順位
	Time m_nTime;					//スコア値
	int m_nAllRank;					//総評の値
	static int m_nObjNum;			//当オブジェクトの生成数

	MyLib::Vector3 m_posAll;		//全項目の基準位置
	MyLib::Vector3 m_posItem[ITEM_MAX];//各項目の位置
	MyLib::Vector3 m_moveAll;		//移動値
	float m_fSize;					//拡大率

	bool m_bUpdate;					//更新されたスコアか？
};


#endif
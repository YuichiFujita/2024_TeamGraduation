//=============================================================================
// 
//  ジャッジアイテム [judgeitem.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _JUDGEITEM_H_
#define _JUDGEITEM_H_		// 二重インクルード防止

#include "objectX.h"
class CJudgeItemManager;

//==========================================================================
// クラス定義
//==========================================================================
// ジャッジアイテムクラス
class CJudgeItem : public CObjectX
{
public:

	CJudgeItem(int nPriority = 6);
	~CJudgeItem();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	// メンバ関数
	void Kill();		// 削除
	void Get();			// 取得
	float GetLength() { return m_fLength; }	// 距離取得
	void SetLength(const float length);		// 距離設定


	//=============================
	// 静的関数
	//=============================
	static CJudgeItem* Create(CJudgeItemManager* pMgr, int mapBlock, const float length, const float height);
	static CListManager<CJudgeItem> GetList() { return m_List; }	// リスト取得
	static CListManager<CJudgeItem> GetListBlock(int mapblock) { return m_ListBlock[mapblock]; }	// リスト取得

private:

	//=============================
	// メンバ変数
	//=============================
	float m_fLength;	// 距離
	float m_fHeight;	// 高さ
	int m_nMyBlockIdx;		// ブロックインデックス
	CJudgeItemManager* m_pMgr;	// マネージャ
	static CListManager<CJudgeItem> m_List;
	static std::map<int, CListManager<CJudgeItem>> m_ListBlock;
};


#endif
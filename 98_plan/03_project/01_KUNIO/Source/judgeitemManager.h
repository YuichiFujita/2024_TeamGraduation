//=============================================================================
// 
//  ジャッジアイテムマネージャヘッダー [judgeitemManager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _JUDGEITEMMANAGER_H_
#define _JUDGEITEMMANAGER_H_	// 二重インクルード防止

#include "object.h"
#include "map_block.h"

class CJudgeItem;

//==========================================================================
// クラス定義
//==========================================================================
// ジャッジアイテムマネージャクラス
class CJudgeItemManager : public CObject
{
public:

	CJudgeItemManager();
	virtual ~CJudgeItemManager() {}

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Reset();	// リセット
	void DeleteJudgeItem(CJudgeItem* pItem);	// ジャッジアイテムの削除

	std::vector<CMapBlockInfo::SJudgeInfo> GetJudgeInfo() { return m_vecJudgeInfo; }	// ジャッジ情報取得
	std::vector<CJudgeItem*> GetJudgeItem() { return m_vecJudgeItem; }					// ジャッジアイテム
	bool IsCheckCollision() { return m_bCheckCollision; }	// コリジョンチェック判定取得

	static CJudgeItemManager* Create(const std::vector<CMapBlockInfo::SJudgeInfo>& vecJudge, float startLen, int mapBlock);	// 生成
	static CListManager<CJudgeItemManager> GetList() { return m_List; }	// リスト取得
	static CListManager<CJudgeItemManager> GetListBlock(int mapblock) { return m_ListBlock[mapblock]; };	// リスト

protected:

	//=============================
	// メンバ関数
	//=============================
	bool CheckCollisionRange();	// コリジョン範囲確認
	void BindPoint();	// ポイント反映

	//=============================
	// メンバ変数
	//=============================
	float m_fStartLength;	// 初期地点長さ
	MyLib::Vector3 m_Top;		// 先頭
	MyLib::Vector3 m_Cur;		// 終端
	bool m_bCheckCollision;	// コリジョンチェック判定
	bool m_bBindPoint;		// ポイント反映
	int m_nMyBlockIdx;		// ブロックインデックス
	std::vector<CMapBlockInfo::SJudgeInfo> m_vecJudgeInfo;	// ジャッジ情報
	std::vector<CJudgeItem*> m_vecJudgeItem;		// ジャッジアイテム
	static CListManager<CJudgeItemManager> m_List;	// リスト
	static std::map<int, CListManager<CJudgeItemManager>> m_ListBlock;	// リスト


};

#endif
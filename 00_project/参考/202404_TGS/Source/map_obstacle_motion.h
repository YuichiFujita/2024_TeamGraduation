//=============================================================================
// 
//  マップのモーションする障害物ヘッダー [map_obstacle_motion.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_MOTION_H_
#define _MAP_OBSTACLE_MOTION_H_		// 二重インクルード防止

#include "objectChara.h"
#include "map_obstacle.h"
#include "listmanager.h"
#include "map_obstacleManager.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物クラス
class CMap_Obstacle_Motion : public CMap_Obstacle
{
private:

	//=============================
	// 列挙型定義
	//=============================
	enum T
	{
		SAKANA = 12,	// 魚
		BIRD = 13,		// 鳥
	};

	enum MOTION
	{
		MOTION_DEF = 0,
		MOTION_HIT,
		MOTION_MAX
	};

public:

	CMap_Obstacle_Motion(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CMap_Obstacle_Motion();

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
	virtual void Kill() override;	// 削除
	bool GetSave() { return m_bSave; }
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// 障害物情報取得
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// 障害物情報設定
	virtual MyLib::Matrix GetWorldMtx() override { return m_pChara->GetWorldMtx(); }
	virtual void SetPosition(const MyLib::Vector3& pos) override;		// 位置設定
	virtual void SetRotation(const MyLib::Vector3& rot) override;		// 向き設定
	virtual void SetScale(const MyLib::Vector3& scale)  override;		// サイズ設定
	virtual void CalWorldMtx() override { m_pChara->CalWorldMtx(); }
	virtual MyLib::Vector3 GetScale() override;

	virtual void Hit() override;	// ヒット処理

	//=============================
	// 静的関数
	//=============================
	static CMap_Obstacle_Motion *Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange = true, const bool bSave = true);	// 生成処理
	static CListManager<CMap_Obstacle_Motion> GetListObj() { return m_List; }				// リスト取得

private:

	
	//=============================
	// メンバ関数
	//=============================
	// その他

	//=============================
	// メンバ変数
	//=============================
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// 障害物情報
	CMap_ObstacleManager::SObstacleInfo m_OriginObstacleInfo;	// 障害物情報
	CObjectChara* m_pChara;
	static CListManager<CMap_Obstacle_Motion> m_List;	// リスト
	bool m_bSave;			// 保存するかどうか

};


#endif
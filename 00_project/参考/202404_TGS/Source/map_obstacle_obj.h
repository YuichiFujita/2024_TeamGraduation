//=============================================================================
// 
//  マップのオブジェクトの障害物ヘッダー [map_obstacle_obj.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_OBJ_H_
#define _MAP_OBSTACLE_OBJ_H_		// 二重インクルード防止

#include "objectX.h"
#include "map_obstacle.h"
#include "listmanager.h"
#include "map_obstacleManager.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物クラス
class CMap_Obstacle_Object : public CMap_Obstacle
{
public:

	CMap_Obstacle_Object(int nPriority = 5, 
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CMap_Obstacle_Object();

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
	virtual void Kill();	// 削除
	bool GetSave() { return m_bSave; }
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// 障害物情報取得
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// 障害物情報設定
	virtual MyLib::Matrix GetWorldMtx() override { return m_pObject->GetWorldMtx(); }
	virtual MyLib::Vector3 GetScale() override { return m_pObject->GetScale(); }
	virtual void CalWorldMtx() override { m_pObject->CalWorldMtx(); }
	virtual void SetPosition(const MyLib::Vector3& pos) override;		// 位置設定
	virtual void SetRotation(const MyLib::Vector3& rot) override;		// 向き設定
	virtual void SetScale(const MyLib::Vector3& scale) override;			// サイズ設定
	virtual MyLib::Vector3 GetVtxMin() override { return m_pObject->GetVtxMin(); };			// 頂点の最小値取得
	virtual MyLib::Vector3 GetVtxMax() override { return m_pObject->GetVtxMax(); };			// 頂点の最大値取得
	virtual void SetState(CObjectX::STATE state) override { m_pObject->SetState(state); }

	//=============================
	// 静的関数
	//=============================
	static CMap_Obstacle_Object *Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange = true, const bool bSave = true);	// 生成処理
	static CListManager<CMap_Obstacle_Object> GetListObj() { return m_List; }				// リスト取得

private:
	
	
	//=============================
	// メンバ関数
	//=============================
	// その他

	//=============================
	// メンバ変数
	//=============================
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// 障害物情報
	CObjectX* m_pObject;	// 表示するオブジェクト
	static CListManager<CMap_Obstacle_Object> m_List;	// リスト
	bool m_bSave;			// 保存するかどうか

};


#endif
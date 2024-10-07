//=============================================================================
// 
//  マップの障害物マネージャヘッダー [map_obstacleManager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MAP_OBSTACLEMANAGER_H_
#define _MAP_OBSTACLEMANAGER_H_		// 二重インクルード防止

#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物マネージャクラス
class CMap_ObstacleManager
{
public:

	//=============================
	// 障害物情報
	//=============================
	// セットアップ情報
	struct SObstacleSetup
	{
		int isAir;		// 空気貫通フラグ
		int isMove;		// 移動フラグ

		SObstacleSetup() : isAir(0), isMove(0) {}
	};

	// 障害物情報
	struct SObstacleInfo
	{
		SObstacleSetup setup;		// セットアップ情報
		std::string textFile;		// テキストファイル
		std::string modelFile;		// モデルファイル
		std::string colliderFile;	// コライダーファイル
		std::vector<MyLib::Collider_BOX> boxcolliders;	// BOXコライダー
		int type;					// 種類
		
		SObstacleInfo() : setup(), modelFile(), colliderFile(), boxcolliders(), type(0) {}

		// JSONからの読み込み
		void from_json(const json& j)
		{
			for (const auto& colliderData : j.at("colliders"))
			{
				MyLib::Collider_BOX collider;
				collider.from_json(colliderData);
				boxcolliders.push_back(collider);
			}
		}

		// JSONへの書き込み
		void to_json(json& j) const
		{
			j["colliders"] = json::array(); // 空の配列を作成

			for (const auto& collider : boxcolliders)
			{
				json colliderJson;
				collider.to_json(colliderJson);

				j["colliders"].emplace_back(colliderJson);
			}
		}
	};



	CMap_ObstacleManager();
	~CMap_ObstacleManager();

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
	void SetObstacleInfo(const std::vector<SObstacleInfo>& info) { m_ObstacleInfo = info; }		// 障害物情報設定
	void SetObstacleInfo(const SObstacleInfo& info, int idx) { m_ObstacleInfo[idx] = info; }	// 障害物情報設定
	std::vector<SObstacleInfo> GetObstacleInfo() { return m_ObstacleInfo; }						// 障害物情報取得
	SObstacleInfo GetObstacleInfo(int idx) { return m_ObstacleInfo[idx]; }						// 障害物情報取得
	void AddCollider(int idx);	// コライダー追加
	void SubCollider(int idx);	// コライダー削除

	// 入出力
	void Save();	// セーブ
	void SaveInfo();	// 情報セーブ
	void Load();	// ロード
	void LoadInfo(const std::string& file);	// 情報ロード

	//=============================
	// 静的関数
	//=============================
	static CMap_ObstacleManager* Create();	// 生成
	static CMap_ObstacleManager* GetInstance() { return m_ThisPtr; }	// インスタンス取得
private:

	//=============================
	// メンバ変数
	//=============================
	std::vector<SObstacleInfo> m_ObstacleInfo;	// 障害物情報
	static CMap_ObstacleManager* m_ThisPtr;		// 自身のポインタ
};


#endif
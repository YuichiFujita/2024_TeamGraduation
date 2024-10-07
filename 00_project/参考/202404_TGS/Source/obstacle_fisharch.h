//=============================================================================
// 
//  アーチ状の魚障害物ヘッダー [obstacle_fisharch.h]
//  Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _OBSTACLE_FISHARCH_H_
#define _OBSTACLE_FISHARCH_H_		// 二重インクルード防止

#include "map_obstacle.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物クラス
class CObstacle_FishArch : public CMap_Obstacle
{
private:

	// 魚情報構造体
	struct FishInfo
	{
		MyLib::Vector3 offset;	// 設定位置
		CMap_Obstacle* pFish;	// 障害物
		float fLength;			// 距離
		int nIdx;

		// コンストラクタ
		FishInfo() : offset(), pFish(nullptr), fLength(0.0f), nIdx(0) {}
	};

	// 本体の可変情報
	struct Info
	{
		float fDefHeight;	// デフォルト高さ
		float fPlusHeight;	// 追加で伸びる高さ
		float fRotSpeed;	// 回転速度
		float fNowHeight;	// 現在の高さ

		// コンストラクタ
		Info() : fDefHeight(0.0f), fPlusHeight(0.0f), fRotSpeed(0.0f), fNowHeight(0.0f) {}
	};

public:

	CObstacle_FishArch(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CObstacle_FishArch();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Kill() override;
	virtual MyLib::Matrix GetWorldMtx() override { return m_mtxWorld; }

	//=============================
	// メンバ関数
	//=============================
	void SetDefHeight(const float fHeight) { m_Info.fDefHeight = fHeight; }
	void SetPlusHeight(const float fHeight) { m_Info.fPlusHeight = fHeight; }
	void SetRotSpeed(const float fRotSpeed) { m_Info.fRotSpeed = fRotSpeed; }
	void SetScale(const MyLib::Vector3& scale) { m_scale = scale; }
	MyLib::Vector3 GetScale() { return m_scale; }

	/**
	@brief	可変情報の設定
	@param	fDefHeight	[in]	基本の高さ
	@param	fPlusHeight	[in]	変化する高さ
	@param	fRotSpd		[in]	回転速度
	@return	void
	*/
	void SetInfo(const float fDefHeight, const float fPlusHeight, const float fRotSpd);	

	//=============================
	// 静的関数
	//=============================
	static CObstacle_FishArch* Create(const CMap_ObstacleManager::SObstacleInfo& info);

private:


	//=============================
	// メンバ関数
	//=============================
	// その他
	void MainControll();	// メイン操作
	void ControllFish();	// 管理している魚の設定
	void SetFishOffSet(FishInfo& info);	// 魚のオフセット設定
	void SetNowHeight();	// 距離設定
	void CalWorldMtx();

	//=============================
	// メンバ変数
	//=============================
	std::vector<FishInfo> m_FishList;	// 魚のリスト
	Info m_Info;	// 可変情報
	float m_fRot;	// 回転向き
	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	MyLib::Vector3 m_scale;
};


#endif

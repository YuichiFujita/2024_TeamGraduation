//=============================================================================
// 
//  円形で回転する鳥障害物ヘッダー [obstacle_birdcircle.h]
//  Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _OBSTACLE_BIRDCIRCLE_H_
#define _OBSTACLE_BIRDCIRCLE_H_		// 二重インクルード防止

#include "map_obstacle.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物クラス
class CObstacle_BirdCircle : public CMap_Obstacle
{
private:

	// 魚情報構造体
	struct BirdInfo
	{
		MyLib::Vector3 offset;	// 設定位置
		CMap_Obstacle* pBird;	// 障害物
		float fLength;			// 距離
		int nIdx;

		// コンストラクタ
		BirdInfo() : offset(), pBird(nullptr), fLength(0.0f), nIdx(0) {}
	};

	// 本体の可変情報
	struct Info
	{
		float fDefLength;	// デフォルト距離
		float fPlusLength;	// 追加で伸びる距離
		float fRotSpeed;	// 回転速度
		float fNowLength;	// 現在の距離

		// コンストラクタ
		Info() : fDefLength(0.0f), fPlusLength(0.0f), fRotSpeed(0.0f), fNowLength(0.0f) {}
	};

public:

	CObstacle_BirdCircle(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CObstacle_BirdCircle();

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
	void SetDefLength(const float fLength) { m_Info.fDefLength = fLength; }
	void SetPlusLength(const float fLength) { m_Info.fPlusLength = fLength; }
	void SetRotSpeed(const float fRotSpeed) { m_Info.fRotSpeed = fRotSpeed; }
	void SetScale(const MyLib::Vector3& scale) { m_scale = scale; }
	MyLib::Vector3 GetScale() { return m_scale; }

	/**
	@brief	可変情報の設定
	@param	fDefLen		[in]	基本の距離
	@param	fPlusLen	[in]	変化距離
	@param	fRotSpd		[in]	回転速度
	@return	void
	*/
	void SetInfo(const float fDefLen, const float fPlusLen, const float fRotSpd);

	//=============================
	// 静的関数
	//=============================
	static CObstacle_BirdCircle* Create(const CMap_ObstacleManager::SObstacleInfo& info);

private:


	//=============================
	// メンバ関数
	//=============================
	// その他
	void MainControll();	// メイン操作
	void ControllBird();	// 管理している魚の設定
	void SetBirdOffSet(BirdInfo& info);	// 魚のオフセット設定
	void SetNowLength();	// 距離設定
	void CalWorldMtx();

	//=============================
	// メンバ変数
	//=============================
	std::vector<BirdInfo> m_BirdList;	// 鳥のリスト
	MyLib::Vector3 m_rot;	// 向き
	Info m_Info;	// 情報
	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	MyLib::Vector3 m_scale;		// スケール
	float m_fIntervalWing;		// 羽ばたきの間隔
};


#endif

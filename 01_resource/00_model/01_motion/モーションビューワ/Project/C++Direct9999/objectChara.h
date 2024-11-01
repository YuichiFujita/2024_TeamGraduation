//=============================================================================
// 
//  オブジェクトキャラクターヘッダー [objectChara.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"
#include "meshsphere.h"

class CModel;

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトキャラクタークラス定義
class CObjectChara : public CObject
{
public:

	// 構造体定義
	struct LoadData
	{
		std::string pModelFile;	// モデルファイル名
		int nType;				// モデル種類
		int nParent;			// 親のインデックス
		int nStart;				// 最初からあるかどうか
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
	};

	struct Load
	{
		std::string sTextFile;		// テキストファイル名
		int nCenterIdx;				// 中心にするパーツインデックス
		D3DXVECTOR3 centerOffSet;	// 中心位置のオフセット
		int nNumModel;				// モデル数
		float fVelocity;			// 移動速度
		float fRadius;				// 半径
		D3DXVECTOR3 posOrigin;		// 最初の位置
		LoadData LoadData[mylib_const::MAX_MODEL];
	};

	/**
	@brief	スフィアコライダー
	*/
	struct SphereCollider
	{
		MyLib::Vector3 center;		// 中心座標
		float radius;				// 半径
		int nParentPartsIdx;		// 親のパーツインデックス番号
		MyLib::Vector3 offset;		// オフセット位置

		// デフォルトコンストラクタ
		SphereCollider() : center(), radius(0.0f), nParentPartsIdx(0), offset() {}

		// パラメータつきコンストラクタ
		SphereCollider(const MyLib::Vector3& center, float radius, int nIdx, const MyLib::Vector3& offset)
			: center(center), radius(radius), nParentPartsIdx(nIdx), offset(offset) {}

		// JSONからの読み込み
		void from_json(const json& j)
		{
			j.at("center").get_to(center);
			j.at("radius").get_to(radius);
			j.at("parent").get_to(nParentPartsIdx);
			j.at("offset").get_to(offset);
		}

		// JSONへの書き込み
		void to_json(json& j) const
		{
			j = json
			{
				{"center", center},
				{"radius", radius},
				{"parent", nParentPartsIdx},
				{"offset", offset},
			};
		}
	};

	CObjectChara(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CObjectChara();

	// オーバーライドされた関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(D3DXCOLOR col);
	void Draw(int nIdx, float fAlpha);

	void SetmtxWorld(const D3DXMATRIX mtxWorld);
	D3DXMATRIX GetmtxWorld(void) const;			// ワールドマトリックス取得
	void SetPosition(const D3DXVECTOR3 pos);	// 位置設定
	D3DXVECTOR3 GetPosition(void) const;		// 位置取得
	void SetOriginPosition(const D3DXVECTOR3 pos);	// 最初の位置設定
	D3DXVECTOR3 GetOriginPosition(void) const;		// 最初の位置取得
	void SetOldPosition(const D3DXVECTOR3 pos);	// 前回の位置設定
	D3DXVECTOR3 GetOldPosition(void) const;		// 前回の位置取得
	void SetRotation(const D3DXVECTOR3 rot);	// 向き設定
	D3DXVECTOR3 GetRotation(void) const;		// 向き取得
	void SetMove(const D3DXVECTOR3 move);		// 移動量設定
	D3DXVECTOR3 GetMove(void) const;			// 移動量取得

	// キャラ情報
	int GetCenterPartsIdx() { return m_nCenterPartsIdx; }
	void SetCenterPartsIdx(int i) { m_nCenterPartsIdx = i; }
	D3DXVECTOR3 GetCenterOffset() { return m_CenterOffset; }	// 中心のオフセット
	void SetCenterOffset(D3DXVECTOR3 p) { m_CenterOffset = p; }
	void SetVelocity(const float fVelocity);	// 移動速度設定
	float GetVelocity(void) const;				// 移動速度取得
	void SetRadius(const float fRadius);		// 半径設定
	float GetRadius(void) const;				// 半径取得
	void SetRotDest(const float fRotDest);		// 目標の向き設定
	float GetRotDest(void) const;				// 目標の向き取得


	HRESULT ReadText(const std::string pTextFile);	// 外部ファイル読み込み処理
	HRESULT SetCharacter(const std::string pTextFile);

	void ChangeObject(int nDeleteParts, int nNewParts);
	void SetObject(int nNewParts);

	CObjectChara *GetObjectChara(void);
	int GetNumModel(void);		// モデル数取得
	void SetNumModel(int nNumModel);		// モデル数設定
	int GetIdxFile(void);		// ファイルのインデックス番号取得
	Load GetLoadData(int nIdx);	// 読み込み情報取得
	CModel **GetModel(void);

	SphereCollider GetNowSphereCollider(int nIdx)
	{
		if (nIdx >= 0 && nIdx < (int)m_SphereColliders.size())
		{
			return m_SphereColliders[nIdx];
		}
		return SphereCollider();
	}

	CMeshSphere* GetMeshSphere(int nIdx) { return m_MeshSphere[nIdx]; }

	void SetNowSphereCollider(int nIdx, const SphereCollider& collider)
	{
		if (nIdx < 0 || nIdx >= (int)m_SphereColliders.size())
		{
			return;
		}
		m_SphereColliders[nIdx] = collider;
		m_MeshSphere[nIdx]->SetWidthLen(collider.radius);
		m_MeshSphere[nIdx]->SetHeightLen(collider.radius);
		m_MeshSphere[nIdx]->SetPosition(collider.center);
	}

	void AddCollider(const SphereCollider& collider); // コライダー追加

	void SubCollider(void);	// コライダー削除

	int GetNumCollider()
	{
		return m_SphereColliders.size();
	}


	// JSONからの読み込み
	void from_json(const json& j)
	{
		// 基底クラスの読み込み
		//CObjectHierarchy::from_json(j);

		for (const auto& colliderData : j.at("colliders"))
		{
			SphereCollider collider;
			collider.from_json(colliderData);
			m_SphereColliders.push_back(collider);
		}
	}

	// CObjectCharaのJSONへの書き込み
	void to_json(json& j) const
	{
		// 基底クラスの書き込み
		//CObjectHierarchy::to_json(j);

		j["colliders"] = json::array(); // 空の配列を作成

		for (const auto& collider : m_SphereColliders)
		{
			json colliderJson;
			collider.to_json(colliderJson);

			j["colliders"].emplace_back(colliderJson);
		}
	}

	void LoadSphereColliders(const std::string textfile);
	void SaveSphereColliders();
private:


	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_posOld;		// 前回の位置
	D3DXVECTOR3 m_posOrigin;	// 最初の位置
	D3DXVECTOR3 m_posCenter;	// 中心位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_move;			// 移動量

	// キャラ情報
	int m_nCenterPartsIdx;		// 中心にするパーツのインデックス
	D3DXVECTOR3 m_CenterOffset;	// 中心のオフセット
	float m_fVelocity;			// 移動速度
	float m_fRadius;			// 半径
	float m_fRotDest;			// 目標の向き
	int m_nNumModel;			// モデルの数
	int m_nIdxFile;				// ファイルのインデックス番号
	CModel *m_apModel[mylib_const::MAX_MODEL];	// モデル(パーツ)のポインタ
	std::vector<SphereCollider> m_SphereColliders;	// スフィアコライダー
	std::vector<CMeshSphere*> m_MeshSphere;			// メッシュスフィア

	std::string m_SetUpFilename;
	static int m_nNumLoad;	// 読み込んだ数
	static Load m_aLoadData[mylib_const::MAX_MODEL];
};



#endif
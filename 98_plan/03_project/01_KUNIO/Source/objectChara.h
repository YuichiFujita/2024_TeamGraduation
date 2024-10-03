//=============================================================================
// 
//  オブジェクトキャラクターヘッダー [objectChara.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_	// 二重インクルード防止

#include "objectHierarchy.h"
#include "motion.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトキャラクタークラス定義
class CObjectChara : public CObjectHierarchy
{
public:

	//=============================
	// 構造体定義
	//=============================
	/**
	@brief	スフィアコライダー
	*/
	struct SphereCollider
	{
		MyLib::Vector3 center;	// 中心座標
		float radius;			// 半径
		int nParentPartsIdx;	// 親のパーツインデックス番号
		MyLib::Vector3 offset;	// オフセット位置

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

	/**
	@brief	コライダー情報
	*/
	struct ColliderData
	{
		std::vector<SphereCollider> colliders;	// スフィアコライダー
		std::string filename;		// ファイル名
	};

	CObjectChara(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectChara();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Draw(D3DXCOLOR col) override;
	void Draw(float fAlpha) override;

	void SetHeight(const float fHeight);		// 身長設定
	float GetHeight();						// 身長取得
	void SetVelocity(const float fVelocity);	// 移動速度設定
	float GetVelocity() const;				// 移動速度取得
	void SetRotDest(const float fRotDest);		// 目標の向き設定
	float GetRotDest() const;				// 目標の向き取得
	void SetLife(const int nLife);				// 体力設定
	int GetLife() const;					// 体力取得
	void SetLifeOrigin(const int nLife);		// 元の体力設定
	int GetLifeOrigin() const;				// 元の体力取得
	int GetMotionStartIdx() const;			// モーション開始のインデックス番号取得
	int GetAddScoreValue() const;			// スコア加算量取得

	// コライダー関連
	int GetSphereColliderNum();						// スフィアコライダーの数取得
	SphereCollider GetNowSphereCollider(int nIdx);	// コライダー取得
	std::vector<SphereCollider> GetSphereColliders();	// スフィアコライダー取得

	HRESULT SetCharacter(const std::string& file) override;	// キャラクター設定
	CObjectChara *GetObjectChara();
	CMotion* GetMotion();	// モーションオブジェクト取得
	static CObjectChara *Create(const std::string pTextFile);	// 生成処理


	//=============================
	// JSONからの読み込み
	//=============================
	void from_json(const json& j)
	{
		for (const auto& colliderData : j.at("colliders")) 
		{
			SphereCollider collider;
			collider.from_json(colliderData);
			m_SphereColliders.push_back(collider);
		}
	}

	//=============================
	// CObjectCharaのJSONへの書き込み
	//=============================
	void to_json(json& j) const
	{
		j["colliders"] = json::array(); // 空の配列を作成

		for (const auto& collider : m_SphereColliders)
		{
			json colliderJson;
			collider.to_json(colliderJson);

			j["colliders"].emplace_back(colliderJson);
		}
	}

protected:

	void LoadObjectData(FILE* pFile, const std::string& file) override;	// オブジェクト毎のデータ読み込み
	void LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts) override;		// パーツ毎のデータ読み込み
	void BindObjectData(int nCntData) override;							// オブジェクト毎のデータ割り当て
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK);	// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK);			// 攻撃判定中処理
	void ChangeMotion(const char* pMotionFile);	// モーションファイル変更

	bool m_bInDicision;	// 攻撃判定中フラグ
private:

	//=============================
	// メンバ関数
	//=============================
	void LoadSphereColliders(const std::string& textfile);
	void SaveSphereColliders();
	void MotionInProgressAction();	// モーション中の行動処理

	//=============================
	// メンバ変数
	//=============================
	float m_fHeight;			// 身長
	float m_fVelocity;			// 移動速度
	float m_fRotDest;			// 目標の向き
	int m_nLife;				// 体力
	int m_nLifeOrigin;			// 元の体力
	int m_nMotionStartIdx;		// モーション開始のインデックス番号
	int m_nAddScore;			// スコア加算量
	CMotion *m_pMotion;			// モーションの情報
	std::vector<SphereCollider> m_SphereColliders;	// スフィアコライダー
	static std::vector<ColliderData> m_LoadColliderData;	// コライダー情報

};



#endif
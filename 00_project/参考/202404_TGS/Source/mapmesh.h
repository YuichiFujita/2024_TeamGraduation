//=============================================================================
// 
//  マップメッシュヘッダー [mapmesh.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MAPMESH_H_
#define _MAPMESH_H_	// 二重インクルード防止

#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップメッシュクラス
class CMapMesh : public CObject3DMesh
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum MeshType
	{
		TYPE_STONEWALL_LEFT = 0,	// 石垣左
		TYPE_TOWNFIELD_SINUOUS,		// 街フィールド(うねり)
		TYPE_TOWNFIELD_SINUOUS_FRONT,		// 街フィールド(うねり)(手前)
		TYPE_TOWNFIELD_FIXEDPLANE,	// 街フィールド(固定平面)
		TYPE_TOWNFIELD_FIXEDPLANE_RESULT,	// 街フィールド(固定平面/リザルト)
		TYPE_TOWNFIELD_FIXEDPLANE_RANKING,	// 街フィールド(固定平面/ランキング)
		TYPE_MAX
	};

	CMapMesh(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CMapMesh();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	void Reset();	// リセット
	void CalVtxPosition();	// 各頂点計算
	virtual void BindVtxPosition() = 0;	// 頂点座標割り当て

	// 情報取得・設定
	std::vector<MyLib::Vector3> GetVecPosition() { return m_vecSegmentPosition; }	// 基点の位置取得
	MyLib::Vector3 GetVecPosition(int idx);											// 基点の位置取得
	void SetVecPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecSegmentPosition = vecpos; }
	void SetVecPosition(int idx, const MyLib::Vector3& pos);						// 基点の位置設定
	std::vector<MyLib::Vector3> GetVecVtxPosition() { return m_vecVtxPosition; }	// 各頂点の位置取得
	MyLib::Vector3 GetVecVtxPosition(int idx);										// 各頂点の位置取得
	void SetVecVtxPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecVtxPosition = vecpos; }
	void SetVecVtxPosition(int idx, const MyLib::Vector3& pos);						// 各頂点の位置設定

	static CMapMesh *Create(MeshType type);	// 生成

protected:

	//=============================
	// メンバ変数
	//=============================
	std::vector<MyLib::Vector3> m_vecSegmentPosition;	// 基点の位置
	std::vector<MyLib::Vector3> m_vecVtxPosition;		// 各頂点の位置
	float m_courceLength;

private:

	virtual void SetVtxTexUV() = 0;	// 各頂点UV座標設定
	 
	
};



#endif
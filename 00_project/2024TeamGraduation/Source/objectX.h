//=============================================================================
// 
//  オブジェクトXヘッダー [objectX.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_	// 二重インクルード防止

#include "object.h"
#include "Xload.h"
#include "listmanager.h"

// 前方宣言
class CShadow;
class CCollisionLine_Box;

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトXクラス定義
class CObjectX : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// なにもない
		STATE_EDIT,		// エディット
		STATE_MAX
	};

	CObjectX(int nPriority = mylib_const::PRIORITY_DEFAULT,	CObject::LAYER layer = CObject::LAYER::LAYER_DEFAULT);
	~CObjectX();

	// オーバーライドされた関数
	HRESULT Init() override;
	HRESULT Init(const std::string& file);
	HRESULT Init(int nIdxXFile);
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Draw(const D3DXCOLOR& col);
	void Draw(float fAlpha);

	//--------------------------
	// 操作
	//--------------------------
	void Kill();		// 削除
	void DrawOnly();	// 描画のみ

	//--------------------------
	// 要素
	//--------------------------
	void BindTexture(int *nIdx);								// テクスチャ割り当て
	void BindXData(int nIdxXFile) { m_nIdxXFile = nIdxXFile; }	// Xファイルデータ割り当て
	int GetIdxXFile() const { return m_nIdxXFile; }				// Xファイルのインデックス取得
	std::string GetFileName() const { return m_fileName; }		// Xファイルのファイル名取得

	//--------------------------
	// パラメーター
	//--------------------------
	void SetWorldMtx(const MyLib::Matrix& mtx) { m_mtxWorld = mtx; }	// マトリックス設定
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// マトリックス取得
	void SetScale(const MyLib::Vector3& scale) { m_scale = scale; }		// スケール設定
	MyLib::Vector3 GetScale() const { return m_scale; }					// スケール取得
	void SetColor(const D3DXCOLOR& col) { m_col = col; }				// 色設定
	D3DXCOLOR GetColor() const { return m_col; }						// 色取得
	void SetAABB(const MyLib::AABB& aabb) { m_AABB = aabb; }			// AABB設定
	MyLib::AABB GetAABB() const { return m_AABB; }						// AABB情報取得
	MyLib::Vector3 GetVtxMax() const { return m_AABB.vtxMax; }			// 頂点の最大値取得
	MyLib::Vector3 GetVtxMin() const { return m_AABB.vtxMin; }			// 頂点の最小値取得
	
	//--------------------------
	// 状態
	//--------------------------
	void SetState(STATE state) { m_state = state; }		// 状態設定
	STATE GetState() { return m_state; }				// 状態取得

	//--------------------------
	// 当たり判定ボックス
	//--------------------------
	void CreateCollisionBox();	// 当たり判定ボックス生成
	CCollisionLine_Box* GetCollisionLineBox() { return m_pCollisionLineBox; }	// 当たり判定ボックス取得
	static CListManager<CObjectX> GetListObj() { return m_List; }				// リスト取得

	//--------------------------
	// 生成
	//--------------------------
	static CObjectX *Create();
	static CObjectX *Create(const std::string& file);
	static CObjectX *Create(const std::string& file, const MyLib::Vector3& pos, const MyLib::Vector3& rot = 0.0f, bool bShadow = false);
	static CObjectX* Create(
		int nIdxXFile,
		const MyLib::Vector3& pos,
		const MyLib::Vector3& rot = 0.0f,
		bool bShadow = false,
		int nPriority = mylib_const::PRIORITY_DEFAULT,
		CObject::LAYER layer = CObject::LAYER::LAYER_DEFAULT);

	//--------------------------
	// その他
	//--------------------------
	float GetHeight(const MyLib::Vector3& pos, bool& bLand);	// 高さ取得
	bool IsUseShadow() const { return m_bShadow; }				// 影を使っているかどうか
	CObjectX* GetObjectX() { return this; }						// オブジェクトXの要素取得

protected:
	// 仮想関数
	virtual void CalWorldMtx();	// ワールドマトリックスの計算処理

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CObjectX::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数リスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void StateNone();	// なにもなし
	void StateEdit();	// エディット

	// その他
	void SetCollisionBoxData();	// 当たり判定ボックスデータ設定

	//=============================
	// メンバ変数
	//=============================
	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	MyLib::Vector3 m_scale;		// スケール
	D3DXCOLOR m_col;			// 色
	MyLib::Vector3 m_fSize;		// サイズ
	MyLib::AABB m_AABB;			// AABB情報
	MyLib::AABB m_OriginAABB;	// 元のAABB情報
	STATE m_state;				// 状態
	int m_nIdxTexure;			// テクスチャのインデックス番号
	int m_nIdxXFile;			// Xファイルのインデックス番号
	std::string m_fileName;		// ファイル名
	CShadow *m_pShadow;			// 影の情報
	bool m_bShadow;				// 影を使うかどうか
	LPD3DXMESH m_pMesh;			// メッシュ(頂点情報)へのポインタ
	CCollisionLine_Box* m_pCollisionLineBox;	// 当たり判定ボックス
	static CListManager<CObjectX> m_List;		// リスト


};



#endif
//=============================================================================
// 
//  オブジェクトXヘッダー [objectX.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"
#include "Xload.h"
#include "listmanager.h"

// 前方宣言
class CShadow;
class CCollisionLine_Box;
class CHandle_Move;

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

	// マクロ定義
#define MAX_MAT				(512)		// マテリアルサイズ
#define MAX_TX				(96)		// テクスチャサイズ

	CObjectX(int nPriority = mylib_const::PRIORITY_DEFAULT,
		CObject::LAYER layer = CObject::LAYER::LAYER_DEFAULT);
	~CObjectX();

	// オーバーライドされた関数
	HRESULT Init();
	HRESULT Init(const std::string& file);
	HRESULT Init(int nIdxXFile);
	void Uninit();
	void Update();
	void Draw();
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);

	void DrawOnly();	// 描画のみ

	void BindTexture(int *nIdx);
	void BindXData(int nIdxXFile);

	void CalWorldMtx();							// ワールドマトリックスの計算処理
	void SetWorldMtx(const MyLib::Matrix mtx);	// マトリックス設定
	MyLib::Matrix GetWorldMtx() const;			// マトリックス取得
	void SetScale(const MyLib::Vector3 scale);	// スケール設定
	MyLib::Vector3 GetScale() const;			// スケール取得
	void SetColor(const D3DXCOLOR col);			// 色設定
	D3DXCOLOR GetColor() const;					// 色取得
	void SetSize(const MyLib::Vector3 size);	// サイズの設定
	void SetAABB(const MyLib::AABB& aabb) { m_AABB = aabb; }
	MyLib::Vector3 GetSize() const;				// サイズの取得
	MyLib::Vector3 GetVtxMax() const;			// 頂点の最大値取得
	MyLib::Vector3 GetVtxMin() const;			// 頂点の最小値取得
	MyLib::AABB GetAABB() const;				// AABB情報取得
	int GetIdxXFile() const;					// Xファイルのインデックス取得
	std::string GetFileName() const { return m_fileName; }					// Xファイルのファイル名取得
	bool GetUseShadow() const;					// 影を使っているかどうか

	void SetState(STATE state) { m_state = state; }		// 状態設定
	STATE GetState() { return m_state; }				// 状態取得

	float GetHeight(MyLib::Vector3 pos, bool &bLand);	// 高さ取得

	void Kill();
	void CreateCollisionBox();	// 当たり判定ボックス生成
	CCollisionLine_Box* GetCollisionLineBox() { return m_pCollisionLineBox; }	// 当たり判定ボックス取得
	static CListManager<CObjectX> GetListObj() { return m_List; }				// リスト取得

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
	CObjectX *GetObjectX();
	static int GetNumAll();

private:


	struct VertexFormat
	{
		D3DXVECTOR3 position; // 頂点の位置
		D3DXVECTOR3 normal;   // 頂点の法線
		D3DXVECTOR2 texCoord; // テクスチャ座標
	};

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
	bool m_bShadow;				// 影を使うかどうか
	int m_nIdxTexure;			// テクスチャのインデックス番号
	int m_nIdxXFile;			// Xファイルのインデックス番号
	std::string m_fileName;
	static int m_nNumAll;		// 総数
	CShadow *m_pShadow;			// 影の情報
	CCollisionLine_Box* m_pCollisionLineBox;	// 当たり判定ボックス
	static CListManager<CObjectX> m_List;		// リスト

	LPD3DXMESH m_pMesh;			// メッシュ(頂点情報)へのポインタ

};



#endif
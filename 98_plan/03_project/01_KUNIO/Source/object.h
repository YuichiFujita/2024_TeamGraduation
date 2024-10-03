//=============================================================================
// 
//  オブジェクトヘッダー [object.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"
#include "renderer.h"
#include "texture.h"

class CObject2D;
class CObject3D;
class CObject3DMesh;
class CObjectX;
class CObjectChara;
class CEffect3D;

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトクラス定義
class CObject
{
public:

	// 列挙型定義
	enum TYPE
	{
		TYPE_NONE = 0,			// 無し
		TYPE_PLAYER,			// プレイヤー
		TYPE_ENEMY,				// 敵
		TYPE_EDIT,				// エディット
		TYPE_XFILE,				// Xファイル
		TYPE_STAGE,				// ステージ(床)
		TYPE_OBJECT2D,			// オブジェクト2D
		TYPE_OBJECT3D,			// オブジェクト3D
		TYPE_OBJECTMESH,		// オブジェクト3D
		TYPE_OBJECTX,			// オブジェクトX
		TYPE_OBJECTBILLBOARD,	// オブジェクトビルボード
		TYPE_MESHFIELD,			// メッシュフィールド
		TYPE_MESHWALL,			// メッシュウォール
		TYPE_MESHCYLINDER,		// メッシュシリンダー
		TYPE_MESHDOME,			// メッシュドーム
		TYPE_MESHDONUTS,		// メッシュドーナツ
		TYPE_MESHSPHERE,		// メッシュスフィア
		TYPE_NUMBER,			// 数字
		TYPE_UI,				// UI
		TYPE_MAX
	};

	enum LAYER
	{
		LAYER_MAP = 0,
		LAYER_LINE,
		LAYER_LEAF,
		LAYER_DEFAULT,
		LAYER_2D,
		LAYER_MAX
	};

	CObject(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CObject();

	// 派生クラスでオーバーライドされた関数が絶対必要(純粋仮想関数)
	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// パラメータ設定
	virtual void SetVtx();
	virtual void SetPosition(const MyLib::Vector3& pos);		// 位置設定
	virtual MyLib::Vector3 GetPosition() const;					// 位置取得
	virtual void SetOldPosition(const MyLib::Vector3& posOld);	// 過去の位置設定
	virtual MyLib::Vector3 GetOldPosition() const;				// 過去の位置取得
	virtual void SetOriginPosition(const MyLib::Vector3& pos);	// 元の位置設定
	virtual MyLib::Vector3 GetOriginPosition() const;			// 元の位置取得
	virtual void SetMove(const MyLib::Vector3& move);			// 移動量設定
	virtual MyLib::Vector3 GetMove() const;						// 移動量取得
	virtual void SetRotation(const MyLib::Vector3& rot);		// 向き設定
	virtual MyLib::Vector3 GetRotation() const;					// 向き取得
	virtual void SetOldRotation(const MyLib::Vector3& rot) { m_rotOld = rot; }		// 前回の向き設定
	virtual MyLib::Vector3 GetOldRotation() const { return m_rotOld; }				// 前回の向き取得
	virtual void SetOriginRotation(const MyLib::Vector3& rot);	// 元の向き設定
	virtual MyLib::Vector3 GetOriginRotation() const;			// 元の向き取得

	// 変更処理
	virtual void AddPosition(const MyLib::Vector3& pos);		// 位置加算
	virtual void AddMove(const MyLib::Vector3& move);			// 移動量加算
	virtual void AddRotation(const MyLib::Vector3& rot);		// 向き加算

	// 描画・スクリーン関連
	float ScreenZ() const;												// スクリーン座標
	static bool ZSort(const CObject* obj1, const CObject* obj2);		// Zソートの比較関数
	static bool ZSortInverse(const CObject *obj1, const CObject *obj2);	// 逆Zソートの比較関数
	int GetPriority() { return m_nPriority; }							// 優先順位取得
	void SetEnableDisp(bool bDisp);										// 描画設定
	bool IsDisp() { return m_bDisp; }									// 描画状況取得
	void SetEnableHitstopMove() { m_bHitstopMove = true; }				// ヒットストップ中に動くフラグ有効

	// エフェクト関連
	int SetEffectParent(CEffect3D *pEffect3D);	// エフェクト設定
	CEffect3D *GetEffectParent(int nIdx);		// 親設定したエフェクト取得
	int GetEffectParentNum();					// 親設定したエフェクトの数取得
	void ReleaseEffect(int nIdx);				// エフェクトの解放
	void UninitEffect();						// エフェクトの解放

	static void ReleaseAll();	// 全開放
	static void UpdateAll();	// 全更新
	static void DrawAll();		// 全描画
	static const int GetNumAll() { return m_nNumAll; }
	static const int GetNumPriorityAll(int nPriority) { return m_nNumPriorityAll[nPriority]; }
	static std::map<LAYER, std::map<int, std::vector<CObject*>>> GetTop() { return m_pObj; }	// 先頭取得

	void SetEnableDeath(bool bDeath) { m_bDeath = bDeath; }
	bool IsDeath();						// 死亡の判定
	void SetType(const TYPE type);		// 種類設定
	TYPE GetType() const;				// 種類取得
	virtual CObject *GetObject();
	virtual CObject2D *GetObject2D();
	virtual CObject3D *GetObject3D();
	virtual CObject3DMesh *GetObject3DMesh();
	virtual CObjectX *GetObjectX();
	virtual CObjectChara *GetObjectChara();

protected:

	void Release();	// 開放処理
	//void Death();	// 死亡処理

private:

	//=============================
	// メンバ関数
	//=============================
	static void DrawNone(const LAYER layer, int nPriority);	// 通常描画
	static void DrawZSort(const LAYER layer, int nPriority);	// Zソート描画

	//=============================
	// メンバ変数
	//=============================
	// 共通するパラメータ
	MyLib::Vector3 m_pos;			// 位置
	MyLib::Vector3 m_posOld;		// 前回の位置
	MyLib::Vector3 m_posOrigin;		// 元の位置
	MyLib::Vector3 m_rot;			// 向き
	MyLib::Vector3 m_rotOld;		// 前回の向き
	MyLib::Vector3 m_rotOrigin;		// 元の向き
	MyLib::Vector3 m_move;			// 移動量

	// オブジェクト管理用
	int m_nPriority;		// 優先順位
	LAYER m_Layer;			// レイヤー名
	TYPE m_type;			// 種類
	static std::map<LAYER, std::map<int, std::vector<CObject*>>> m_pObj;	// オブジェクト格納用

	// その他
	bool m_bDeath;			// 死亡フラグ
	bool m_bDisp;			// 描画フラグ
	CEffect3D *m_pEffect[mylib_const::MAX_OBJ];	// エフェクトのポインタ
	int m_nNumEffectParent;		// エフェクトの親設定した数
	bool m_bHitstopMove;		// ヒットストップ時に動くかのフラグ
	static int m_nNumAll;		// オブジェクトの総数
	static int m_nNumPriorityAll[mylib_const::PRIORITY_NUM];

};

#endif
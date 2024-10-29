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

	//=============================
	// 列挙型定義
	//=============================
	// 種類
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
		TYPE_MANAGER,			// マネージャー
		TYPE_NUMBER,			// 数字
		TYPE_LIGHT,				// ライト
		TYPE_TIMER,				// タイマー
		TYPE_UI,				// UI
		TYPE_MAX
	};

	// レイヤー
	enum LAYER
	{
		LAYER_TIMER = 0,
		LAYER_MAP,
		LAYER_LINE,
		LAYER_DEFAULT,
		LAYER_2D,
		LAYER_MAX
	};

	CObject(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CObject();

	//--------------------------
	// 純粋仮想関数
	//--------------------------
	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;
	virtual void Draw() = 0;
	virtual void Kill() = 0;	// 動的削除処理

	//--------------------------
	// 仮想関数
	//--------------------------
	virtual void Release();	// 開放処理
	// TODO：ここにKill
	virtual inline void SetVtx() { assert(false); }	// 頂点情報設定

	//--------------------------
	// 機能
	//--------------------------
	static void ReleaseAll();						// 全開放
	static void UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 全更新
	static void DrawAll();							// 全描画

	//--------------------------
	// 共通するパラメータ
	//--------------------------
	virtual inline void SetPosition(const MyLib::Vector3& pos)			{ m_pos = pos; }		// 位置設定
	virtual inline MyLib::Vector3 GetPosition() const					{ return m_pos; }		// 位置取得
	virtual inline void SetOldPosition(const MyLib::Vector3& posOld)	{ m_posOld = posOld; }	// 過去の位置設定
	virtual inline MyLib::Vector3 GetOldPosition() const				{ return m_posOld; }	// 過去の位置取得
	virtual inline void SetOriginPosition(const MyLib::Vector3& pos)	{ m_posOrigin = pos; }	// 元の位置設定
	virtual inline MyLib::Vector3 GetOriginPosition() const				{ return m_posOrigin; }	// 元の位置取得
	virtual inline void SetMove(const MyLib::Vector3& move)				{ m_move = move; }		// 移動量設定
	virtual inline MyLib::Vector3 GetMove() const						{ return m_move; }		// 移動量取得
	virtual inline void SetRotation(const MyLib::Vector3& rot)			{ m_rot = rot; }		// 向き設定
	virtual inline MyLib::Vector3 GetRotation() const					{ return m_rot; }		// 向き取得
	virtual inline void SetOldRotation(const MyLib::Vector3& rot)		{ m_rotOld = rot; }		// 前回の向き設定
	virtual inline MyLib::Vector3 GetOldRotation() const				{ return m_rotOld; }	// 前回の向き取得
	virtual inline void SetOriginRotation(const MyLib::Vector3& rot)	{ m_rotOrigin = rot; }	// 元の向き設定
	virtual inline MyLib::Vector3 GetOriginRotation() const				{ return m_rotOrigin; }	// 元の向き取得

	// 変更処理
	virtual inline void AddPosition(const MyLib::Vector3& pos)	{ m_pos += pos; }	// 位置加算
	virtual inline void AddMove(const MyLib::Vector3& move)		{ m_move += move; }	// 移動量加算
	virtual inline void AddRotation(const MyLib::Vector3& rot)	{ m_rot += rot; }	// 向き加算

	//--------------------------
	// 描画・スクリーン関連
	//--------------------------
	inline int GetPriority() { return m_nPriority; }					// 優先順位取得
	static bool ZSort(const CObject* obj1, const CObject* obj2);		// Zソートの比較関数
	static bool ZSortInverse(const CObject *obj1, const CObject *obj2);	// 逆Zソートの比較関数
	virtual inline void SetEnableDisp(bool bDisp) { m_bDisp = bDisp; }	// 描画状況設定
	bool IsDisp() { return m_bDisp; }									// 描画状況取得
	void SetEnablePosibleMove_WorldPause(bool bMove) { m_bPosibleMove_WorldPause = bMove; }	// 世界停止中に動けるフラグ
	bool IsPosibleMove_WorldPause() { return m_bPosibleMove_WorldPause; }					// 世界停止中に動けるフラグ

	//--------------------------
	// その他
	//--------------------------
	void SetEnableDeath(bool bDeath) { m_bDeath = bDeath; }				// 死亡フラグ設定
	bool IsDeath() { return m_bDeath; }									// 死亡フラグ取得
	TYPE GetType() const { return m_type; }								// 種類取得
	virtual inline void SetType(const TYPE type) { m_type = type; }		// 種類設定
	static const int GetNumAll() { return m_nNumAll; }					// 総数取得
	static std::map<LAYER, std::map<int, std::vector<CObject*>>> GetTop() { return m_pObj; }	// 先頭取得

private:

	//=============================
	// メンバ関数
	//=============================
	static void DrawNone(const LAYER layer, int nPriority);		// 通常描画
	static void DrawZSort(const LAYER layer, int nPriority);	// Zソート描画
	float ScreenZ() const;										// スクリーンのZ座標取得

	//=============================
	// メンバ変数
	//=============================
	//--------------------------
	// 共通するパラメータ
	//--------------------------
	MyLib::Vector3 m_pos;			// 位置
	MyLib::Vector3 m_posOld;		// 前回の位置
	MyLib::Vector3 m_posOrigin;		// 元の位置
	MyLib::Vector3 m_rot;			// 向き
	MyLib::Vector3 m_rotOld;		// 前回の向き
	MyLib::Vector3 m_rotOrigin;		// 元の向き
	MyLib::Vector3 m_move;			// 移動量

	//--------------------------
	// オブジェクト管理用
	//--------------------------
	int m_nPriority;		// 優先順位
	LAYER m_Layer;			// レイヤー名
	TYPE m_type;			// 種類
	static std::map<LAYER, std::map<int, std::vector<CObject*>>> m_pObj;	// オブジェクト格納用

	//--------------------------
	// その他
	//--------------------------
	bool m_bDeath;				// 死亡フラグ
	bool m_bDisp;				// 描画フラグ
	bool m_bPosibleMove_WorldPause;		// 世界停止中に動けるフラグ
	static int m_nNumAll;		// オブジェクトの総数

};

#endif
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

class CObject2D;
class CObject3D;
class CObject3DMesh;
class CObjectX;
class CObjectChara;

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトクラス定義
class CObject
{
public:

	// 列挙型定義
	typedef enum
	{
		TYPE_NONE = 0,		// 無し
		TYPE_PLAYER,		// プレイヤー
		TYPE_ENEMY,			// 敵
		TYPE_BULLET,		// 弾
		TYPE_EXPLOSION,		// 爆発
		TYPE_BG,			// 背景
		TYPE_EFFECT2D,		// 2Dエフェクト
		TYPE_EFFECT3D,		// 3Dエフェクト
		TYPE_SCORE,			// スコア
		TYPE_TIMER,			// タイマー
		TYPE_BLOCK,			// ブロック
		TYPE_ITEM,			// アイテム
		TYPE_FIELD,			// フィールド
		TYPE_HPGAUGE,		// HPゲージ
		TYPE_MAGIC_CIRCLE,	// 魔法陣
		TYPE_CAR,			// 車
		TYPE_SHADOW,		// 影
		TYPE_MAP,			// マップ
		TYPE_XFILE,			// Xファイル
		TYPE_MODEL,			// モデル
		TYPE_EDIT,			// エディット
		TYPE_MESHFIELD,		// メッシュフィールド
		TYPE_ELEVATION,		// 起伏フィールド
		TYPE_MESHWALL,		// メッシュウォール
		TYPE_MESHCYLINDER,	// メッシュシリンダー
		TYPE_MESHDOME,		// メッシュドーム
		TYPE_MESHDONUTS,	// メッシュドーナツ
		TYPE_BALLAST,		// 瓦礫
		TYPE_MAGIC_ICE,		// 氷魔法
		TYPE_TARGETPOINT,	// 目標の地点
		TYPE_SPEED_RUSH,	// 高速ラッシュ
	}TYPE;

	CObject(int nPriority = mylib_const::DEFAULT_PRIORITY);
	virtual ~CObject();

	// 派生クラスでオーバーライドされた関数が絶対必要(純粋仮想関数)
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetVtx(void);
	virtual void SetPosition(const D3DXVECTOR3 pos);	// 位置設定
	virtual D3DXVECTOR3 GetPosition(void) const;		// 位置取得
	virtual void SetMove(const D3DXVECTOR3 rot);	// 移動量設定
	virtual D3DXVECTOR3 GetMove(void) const;		// 移動量取得
	virtual void SetRotation(const D3DXVECTOR3 rot);	// 向き設定
	virtual D3DXVECTOR3 GetRotation(void) const;		// 向き取得
	virtual bool Hit(const int nValue);					// 嘆きのバーチャルヒット処理

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static const int GetNumAll(void) { return m_nNumAll; }
	static const int GetNumPriorityAll(int nPriority) { return m_nNumPriorityAll[nPriority]; }

	void SetType(const TYPE type);
	TYPE GetType(void) const;
	int GetID(void) { return m_nID; }
	static CObject *GetObject(int nPriority, int nIdx);
	CObject *GetObject(void);
	virtual CObject2D *GetObject2D(void);
	virtual CObject3D *GetObject3D(void);
	virtual CObject3DMesh *GetObject3DMesh(void);
	virtual CObjectX *GetObjectX(void);
	virtual CObjectChara *GetObjectChara(void);

protected:

	void Release(void);

private:
	int m_nID;				// 自分自身のID
	int m_nPriority;		// 優先順位
	static int m_nNumAll;	// オブジェクトの総数
	static int m_nNumPriorityAll[mylib_const::PRIORITY_NUM];
	TYPE m_type;			// 種類
	static CObject *m_apObject[mylib_const::PRIORITY_NUM][mylib_const::MAX_OBJ];	// オブジェクトのオブジェクト
};

#endif
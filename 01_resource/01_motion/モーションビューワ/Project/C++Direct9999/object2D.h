//=============================================================================
// 
//  オブジェクト2Dヘッダー [object2D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"

class CEnemy;

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト2Dクラス定義
class CObject2D : public CObject
{
public:

	CObject2D(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CObject2D();

	// オーバーライドされた関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetVtx(void);
	void BindTexture(int nIdx);

	void SetPosition(const D3DXVECTOR3 pos);	// 位置設定
	D3DXVECTOR3 GetPosition(void) const;		// 位置取得
	void SetOldPosition(const D3DXVECTOR3 posOld);	// 前回の位置設定
	D3DXVECTOR3 GetOldPosition(void) const;		// 前回の位置取得
	void SetRotation(const D3DXVECTOR3 rot);	// 向き設定
	D3DXVECTOR3 GetRotation(void) const;		// 向き取得
	void SetMove(const D3DXVECTOR3 move);		// 移動量設定
	D3DXVECTOR3 GetMove(void) const;			// 移動量取得
	void SetColor(const D3DXCOLOR col);			// 色設定
	D3DXCOLOR GetColor(void) const;				// 色取得
	void SetSize(const D3DXVECTOR2 size);		// サイズの設定
	D3DXVECTOR2 GetSize(void) const;			// サイズの取得
	void SetLength(const float fLength);		// 対角線の長さ設定
	float GetLength(void) const;				// 対角線の長さ取得
	void SetAngle(const float fAngle);			// 対角線の向き設定
	float GetAngle(void) const;					// 対角線の向き取得
	
	template<class T>T *GetTemplate(void);

	static CObject2D *Create();
	static CObject2D *Create(int nPriority);
	CObject2D *GetObject2D(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const;
protected:

private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	D3DXCOLOR m_col;		// 色
	D3DXVECTOR2 m_fSize;	// サイズ
	float m_fLength;		// 対角線の長さ
	float m_fAngle;			// 対角線の向き
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	int m_nTexIdx;					// テクスチャのインデックス番号
};

//template<class T>
//
//T *GetTemplate(void);

//template<class T> T *CObject2D::GetTemplate<T>(void)
//{
//	//T *pTemp = this;
//	return this;
//}



#endif
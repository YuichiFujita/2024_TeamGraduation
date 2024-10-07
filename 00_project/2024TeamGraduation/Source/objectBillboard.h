//=============================================================================
// 
//  ビルボードヘッダー [objectBillboard.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"
#include "constans.h"

//==========================================================================
// クラス定義
//==========================================================================
// ビルボードクラス定義
class CObjectBillboard : public CObject
{
public:

	CObjectBillboard(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CObjectBillboard();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	virtual void SetVtx();
	void BindTexture(int nIdx);
	virtual void SetRotation(const MyLib::Vector3& rot) override;		// 向き設定

	virtual void SetWorldMtx(const MyLib::Matrix mtx);	// マトリックス設定
	virtual MyLib::Matrix GetWorldMtx() const;			// マトリックス取得
	virtual void SetColor(const D3DXCOLOR col);			// 色設定
	virtual D3DXCOLOR GetColor() const;					// 色取得
	virtual void SetAlpha(const float col);				// 不透明度設定
	virtual float GetAlpha() const;						// 不透明度取得
	virtual void SetSize(const D3DXVECTOR2 size);		// サイズの設定
	virtual D3DXVECTOR2 GetSize() const;				// サイズの取得
	virtual void SetSizeOrigin(const D3DXVECTOR2 size);	// 元のサイズの設定
	virtual D3DXVECTOR2 GetSizeOrigin() const;			// 元のサイズの取得
	virtual void SetTex(D3DXVECTOR2 *tex);				// テクスチャ座標の設定
	virtual D3DXVECTOR2 *GetTex();						// テクスチャ座標の取得

	static CObjectBillboard *Create(int nPriority = mylib_const::PRIORITY_ZSORT);
	static CObjectBillboard *Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot);
	CObjectBillboard *GetObjectBillBoard();
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() const;

protected:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	float m_fLength;			// 対角線の長さ
	float m_fAngle;				// 対角線の向き

private:
	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	D3DXCOLOR m_col;			// 色
	D3DXVECTOR2 m_fSize;		// サイズ
	D3DXVECTOR2 m_sizeOrigin;	// 元のサイズ
	D3DXVECTOR2 m_fTex[32];	// テクスチャ座標
	int m_nTexIdx;			// テクスチャのインデックス番号
};



#endif
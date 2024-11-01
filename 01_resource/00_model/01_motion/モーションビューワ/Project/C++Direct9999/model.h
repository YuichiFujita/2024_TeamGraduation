//=============================================================================
// 
//  モデルヘッダー [model.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"

class CObjectX;

//==========================================================================
// クラス定義
//==========================================================================
// モデルクラス定義
class CModel
{
public:

	CModel(int nPriority = mylib_const::DEFAULT_PRIORITY);
	~CModel();

	HRESULT Init(const char *pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);
	void BindTexture(int *nIdx);
	void BindXData(int nIdxXFile);

	void SetWorldMtx(const D3DXMATRIX mtx);		// マトリックス設定
	D3DXMATRIX GetWorldMtx(void) const;			// マトリックス取得
	void SetPosition(const D3DXVECTOR3 pos);	// 位置設定
	D3DXVECTOR3 GetPosition(void) const;		// 位置取得
	void SetOldPosition(const D3DXVECTOR3 posOld);	// 前回の位置設定
	D3DXVECTOR3 GetOldPosition(void) const;		// 前回の位置取得
	void SetOriginPosition(const D3DXVECTOR3 pos);	// 元の位置設定
	D3DXVECTOR3 GetOriginPosition(void) const;		// 元の位置取得
	void SetRotation(const D3DXVECTOR3 rot);	// 向き設定
	D3DXVECTOR3 GetRotation(void) const;		// 向き取得
	void SetOriginRotation(const D3DXVECTOR3 rot);	// 元の向き設定
	D3DXVECTOR3 GetOriginRotation(void) const;		// 元の向き取得
	void SetOriginPotision(const D3DXVECTOR3 pos);	// 元の位置設定
	D3DXVECTOR3 GetOriginPotision(void) const;		// 元の位置取得
	void SetScale(const D3DXVECTOR3 scale);		// スケール設定
	D3DXVECTOR3 GetScale(void) const;			// スケール取得
	D3DXVECTOR3 GetVtxMax(void) const;			// 頂点の最大値取得
	D3DXVECTOR3 GetVtxMin(void) const;			// 頂点の最小値取得
	void SetParent(CModel *pModel, int nIdx);				// 親のポインタ設定
	int GetParentIdx(void) { return m_nIdxParent; }
	std::string GetModelName(void) { return m_modelname; }

	static CModel *Create(const char *pFileName, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CModel *GetModel(void);
	static int GetNumAll(void);
protected:

private:

	// メンバ関数
	void CalWorldMtx(void);	// ワールドマトリックス計算

	// メンバ変数
	D3DXMATRIX	m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_posOrigin;// 元の位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_rotOrigin;// 向き
	D3DXVECTOR3 m_scale;	// スケール
	int *m_nIdxTexure;		// テクスチャのインデックス番号
	int m_nIdxXFile;		// Xファイルのインデックス番号
	int m_nIdxParent;		// 親のインデックス番号
	CModel *m_pParent;		// 親モデルのポインタ
	CModel *m_pModel;		// 自分のポインタ
	std::string m_modelname;	// モデルの名前
	static int m_nNumAll;	// 総数
};



#endif
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

	CModel(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CModel();

	HRESULT Init(const std::string& file);
	void Uninit();
	void Update();
	void Draw();
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);
	void BindXData(int nIdxXFile);

	int *GetIdxTexture() {return m_nIdxTexture; }
	int GetIdxTexture(int nIdx);	// テクスチャインデックス番号取得
	void SetIdxTexture(int i, int nIdx);	// テクスチャのインデックス割り当て
	int GetIdxXFile() const { return m_nIdxXFile; }
	void SetWorldMtx(const MyLib::Matrix mtx);		// マトリックス設定
	MyLib::Matrix GetWorldMtx() ;			// マトリックス取得
	MyLib::Matrix*GetPtrWorldMtx();			// ポインタマトリックス取得
	void SetPosition(const MyLib::Vector3 pos);	// 位置設定
	MyLib::Vector3 GetPosition() const;		// 位置取得
	void SetOldPosition(const MyLib::Vector3 posOld);	// 前回の位置設定
	MyLib::Vector3 GetOldPosition() const;		// 前回の位置取得
	void SetOriginPosition(const MyLib::Vector3 pos);	// 元の位置設定
	MyLib::Vector3 GetOriginPosition() const;		// 元の位置取得
	void SetRotation(const MyLib::Vector3 rot);	// 向き設定
	MyLib::Vector3 GetRotation() const;		// 向き取得
	void SetOriginRotation(const MyLib::Vector3 rot);	// 元の向き設定
	MyLib::Vector3 GetOriginRotation() const;		// 元の向き取得
	void SetScale(const MyLib::Vector3 scale);		// スケール設定
	MyLib::Vector3 GetOriginScale() const { return m_scaleOrigin; }				// 元のスケール取得
	void SetOriginScale(const MyLib::Vector3& scale) { m_scaleOrigin = scale; }	// 元のスケール設定
	MyLib::Vector3 GetScale() const;			// スケール取得
	MyLib::Vector3 GetVtxMax() const;			// 頂点の最大値取得
	MyLib::Vector3 GetVtxMin() const;			// 頂点の最小値取得
	void SetParent(CModel *pModel);				// 親のポインタ設定
	CModel* GetParent(){ return m_pParent; }		// 親のポインタ取得

	void SetMtxParent(MyLib::Matrix* pMtx);

	static CModel *Create(const std::string& file, MyLib::Vector3 pos = MyLib::Vector3(0.0f, 0.0f, 0.0f), MyLib::Vector3 rot = MyLib::Vector3(0.0f, 0.0f, 0.0f));

	CModel *GetModel();
	static int GetNumAll();
protected:

private:

	void BindTexture();	// テクスチャ割り当て
	void CalWorldMtx();	// ワールドマトリックスの計算処理
	void DrawShadowMtx();

	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	MyLib::Matrix *m_mtxParent;	// 親マトリックスのポインタ
	MyLib::Vector3 m_pos;		// 位置
	MyLib::Vector3 m_posOld;	// 前回の位置
	MyLib::Vector3 m_posOrigin;	// 元の位置
	MyLib::Vector3 m_rot;		// 向き
	MyLib::Vector3 m_rotOrigin;// 向き
	MyLib::Vector3 m_scale;		// スケール
	MyLib::Vector3 m_scaleOrigin;	// 元のスケール
	int m_nIdxXFile;			// Xファイルのインデックス番号
	int *m_nIdxTexture;			// テクスチャのインデックス番号
	CModel *m_pParent;			// 親モデルのポインタ
	static int m_nNumAll;		// 総数
};



#endif
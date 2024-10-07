//=============================================================================
// 
//  起伏ヘッダー [elevation.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ELEVATION_H_
#define _ELEVATION_H_	// 二重インクルード防止

#include "main.h"
#include "object3DMesh.h"

// 前方宣言
class CTargetPoint;

//==========================================================================
// クラス定義
//==========================================================================
// 起伏クラス定義
class CElevation : public CObject3DMesh
{
private:

	struct Info
	{
		std::string TextureFileName;	// テクスチャファイル名
		MyLib::Vector3 pos;		// 位置
		float fWidthLength;		// 横長さ
		float fHeightLength;	// 縦長さ
		int nWidthBlock;		// 横分割数
		int nHeightBlock;		// 縦分割数
		MyLib::Vector3 *pVtxPos;	// 頂点座標
	};

public:

	CElevation(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CElevation();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual HRESULT Init(const char *pText);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Release();
	bool IsEdit();	// エディットの判定
	void Save();	// セーブ
	HRESULT Load(const char *pText);	// ロード
	bool IsChange() { return m_bChange; }

	static CElevation *Create(const char *pText);
	CElevation *GetElevation();
	float GetHeight(const MyLib::Vector3& pos, bool *pLand);	// 高さ取得
	bool IsHit(const MyLib::Vector3& pos);	// 当たったかの判定

protected:

private:

	void UpdateState();				// 状態更新処理
	void UPVtxField(MyLib::Vector3 pos);	// 頂点上げ下げ(デバッグ)

	Info m_aInfo;	// 起伏の情報
	float m_fBrushStrength;							// ブラシ強さ
	float m_fBrushRange;							// 範囲
	bool m_bEdit;									// エディットON/OFF
	CTargetPoint *m_pTargetP;						// 目標の地点
	MyLib::Vector3 m_VtxPos[mylib_const::MAX_VTX];		// 頂点座標
	int m_nTexIdx;									// テクスチャのインデックス番号
	bool m_bChange;	// 変更のフラグ
	static bool m_bLoadInfo;						// 情報読み込み判定
};



#endif
//=============================================================================
// 
//  メッシュシリンダーヘッダー [impactwave.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _IMPACTWAVE_H_
#define _IMPACTWAVE_H_	// 二重インクルード防止

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// メッシュシリンダークラス定義
class CImpactWave : public CObject3DMesh
{
public:
	//列挙型定義
	typedef enum
	{
		TYPE_BLACK = 0,
		TYPE_BLACK2,
		TYPE_ORANGE,
		TYPE_ORANGE2,
		TYPE_BLUE,
		TYPE_BLUE2,
		TYPE_GREEN,
		TYPE_GREEN2,
		TYPE_PURPLE,
		TYPE_PURPLE2,
		TYPE_PURPLE3,
		TYPE_PURPLE4,
		TYPE_PINK,
		TYPE_SMOKE,
		TYPE_GIZA,
		TYPE_GIZA2,
		TYPE_GIZABLACK,
		TYPE_GIZAWHITE,
		TYPE_GIZAWHITE2,
		TYPE_GIZAGRADATION,
		TYPE_WAVEWHITE,
		TYPE_MAX
	}TYPE;

	CImpactWave(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CImpactWave();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	// 設定・取得
	void SetLife(const int nLife);				// 寿命設定
	int GetLife() const;					// 寿命取得
	void SetMaxLife(const int nLife);			// 最大寿命設定
	int GetMaxLife() const;					// 最大寿命取得
	void SetColOrigin(const D3DXCOLOR col);		// 元の色設定
	D3DXCOLOR GetColOrigin() const;			// 元の色取得
	void SetOutWidth(const float fOutWidth);	// 外周設定
	float GetOutWidth() const;				// 外周取得
	void SetInWidth(const float fInWidth);		// 内周設定
	float GetInWidth() const;				// 内周取得
	void SetRotWidth(const float fRot);			// 一分割あたりの角度設定
	float GetRotWidth() const;				// 一分割あたりの角度取得
	void SetMoveSize(const float fMoveSize);	// サイズの移動量設定
	float GetMoveSize() const;				// サイズの移動量取得
	void SetTexDivision(const int nDivision);	// テクスチャ分割数設定
	int GetTexDivision() const;				// テクスチャ分割数取得
	void SetIsAddBlend(bool bAddBlend);			// 加算合成の設定

	static const char *GetFileName(int nTexType);	// ファイル名取得
	static CImpactWave *Create(MyLib::Vector3 pos, MyLib::Vector3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fCenterDistance, int nLife, float fMove, int nTexType, bool bAddBlend);
	CImpactWave *GetMyObject();

private:
	int m_nTexIdx;	// テクスチャのインデックス番号
	int m_nLife;	// 寿命
	int m_nMaxLife;	// 最大寿命
	int m_nTexDivision;		// テクスチャ分割
	D3DXCOLOR m_colOrigin;	// 元の色
	float m_fOutWidth;		// 横幅(外)
	float m_fInWidth;		// 幅(内)
	float m_fRotWidth;		// 1分割数あたりの角度割合
	float m_fHeight;		// 高さ
	float m_fMove;			// 広がる速度
	bool m_bAddBlend;		// 加算合成の判定
	static const char *m_apFilename[TYPE_MAX];		//ファイル読み込み
};



#endif
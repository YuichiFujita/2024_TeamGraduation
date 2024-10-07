//=============================================================================
// 
// エフェクト処理 [3D_effect.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EFFECT_3D_H_
#define _EFFECT_3D_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// エフェクトクラス定義
class CEffect3D : public CObjectBillboard
{
public:

	// 列挙型定義
	typedef enum
	{
		MOVEEFFECT_ADD = 0,		// 加算
		MOVEEFFECT_SUB,			// 減算
		MOVEEFFECT_SUPERSUB,	// 超減算
		MOVEEFFECT_NONE,		// 変化なし
		MOVEEFFECT_GENSUI,		// 減衰
		MOVEEFFECT_MAX
	}MOVEEFFECT;

	typedef enum
	{
		TYPE_NORMAL = 0,	// 通常エフェクト
		TYPE_POINT,			// 点
		TYPE_SMOKE,			// 煙エフェクト
		TYPE_SMOKEBLACK,	// 黒煙
		TYPE_BLACK,			// 黒エフェクト
		TYPE_JUJI,			// 十字エフェクト
		TYPE_JUJI2,			// 十字エフェクト
		TYPE_THUNDER,		// 雷エフェクト
		TYPE_WATER,			// 水滴エフェクト
		TYPE_nullptr,		// nullptrエフェクト
		TYPE_MAX
	}TYPE;

	CEffect3D(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CEffect3D();

	static CEffect3D *Create();
	static CEffect3D *Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type, const float fAddSizeValue = 0.0f);
	static void LoadTexture();	// テクスチャ読み込み

	//  オーバーライドされた関数
	HRESULT Init();
	HRESULT Init(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type);
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	void SetDisableAddAlpha() { m_bAddAlpha = false; }	// 加算合成取り消し
	void SetPositionDest(MyLib::Vector3 pos);	// 目標の位置設定
	void SetEnableGravity() { m_bGravity = true; }	// 重力有効
	void SetDisableZSort() { m_bZSort = false; }	// Zソート無効
	void SetGravityValue(float fValue);					// 重力の値設定
	void SetMoveFactor(float factor) { m_fMoveFactor = factor; };		// 移動補正係数
	void SetUp(MyLib::Vector3 setup, D3DXMATRIX *pMtxParent, CObject *pObj, int nParentIdx);	// セットアップ
	void UpdatePosition(MyLib::Vector3 rot);	// 位置更新
	void UninitParent();	// 親の破棄
	static int GetNumAll();	// 総数取得

private:

	// メンバ関数
	void UpdateMove();	// 移動処理
	void SubSize();
	void SuperSubSize();
	void AddSize();
	void Gensui();

	// メンバ変数
	MyLib::Vector3 m_posOrigin;		// 原点
	MyLib::Vector3 m_updatePosition;	// 更新後の位置
	MyLib::Vector3 m_setupPosition;	// セットアップ位置
	MyLib::Vector3 m_setupRotation;	// セットアップ位置
	MyLib::Vector3 m_posDest;			// 目標の位置
	D3DXCOLOR m_colOrigin;		// 色
	D3DXMATRIX *m_pMtxParent;	// 親マトリックスのポインタ
	float m_fRadius;			// 半径
	float m_fMaxRadius;			// 最大半径
	float m_fAddSizeValue;		// サイズ変更量
	float m_fGravity;			// 重力
	float m_fMoveFactor;		// 移動補正係数
	int m_nLife;				// 寿命
	int m_nMaxLife;				// 最大寿命(固定)
	int m_moveType;				// 移動の種類
	int m_nParentIdx;			// 親のインデックス番号
	bool m_bAddAlpha;			// 加算合成の判定
	bool m_bZSort;				// Zソートのフラグ
	bool m_bGravity;			// 重力のフラグ
	bool m_bChaseDest;			// 目標の位置へ向かうフラグ
	TYPE m_nType;				// 種類
	CObject *m_pParent;			// 親のポインタ
	static const char *m_apTextureFile[];			// テクスチャのファイル
	static int m_nNumAll;	// 総数
	static int m_nTexIdx[TYPE_MAX];				// テクスチャのインデックス番号
};

#endif
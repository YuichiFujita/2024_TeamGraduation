//=============================================================================
// 
// エフェクト処理 [2D_effect.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EFFECT_2D_H_
#define _EFFECT_2D_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// 弾クラス定義
class CEffect2D : public CObject2D
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
		TYPE_SMOKE,			// 煙エフェクト
		TYPE_SMOKEBLACK,	// 黒煙
		TYPE_BLACK,			// 黒エフェクト
		TYPE_JUJI,			// 十字エフェクト
		TYPE_JUJI2,			// 十字エフェクト
		TYPE_JUJI3,			// 十字エフェクト
		TYPE_MAX
	}TYPE;

	CEffect2D(int nPriority = 10);
	~CEffect2D();

	static CEffect2D *Create();
	static CEffect2D *Create(const MyLib::Vector3 pos, const MyLib::Vector3 move, const D3DXCOLOR col, const float fRadius, const float fLife, const int moveType, const TYPE type);
	static CEffect2D *Create(const MyLib::Vector3 pos, const MyLib::Vector3 move, const MyLib::Vector3 rot, const D3DXCOLOR col, const float fRadius, const float fLife, const int moveType, const TYPE type);

	//  オーバーライドされた関数
	HRESULT Init();
	HRESULT Init(const MyLib::Vector3 pos, const MyLib::Vector3 move, const D3DXCOLOR col, const float fRadius, const float fLife, const int moveType, const TYPE type);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	void SetVtx();

	static int GetNumAll();

private:
	void SubSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void SuperSubSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void AddSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Gensui(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	D3DXCOLOR m_colOrigin;	// 色
	float m_fRadius;		// 半径
	float m_fMaxRadius;		// 最大半径
	float m_fLife;			// 寿命
	float m_fMaxLife;		// 最大寿命(固定)
	int m_moveType;			// 移動の種類
	bool m_bAddAlpha;		// 加算合成の判定
	TYPE m_nType;			// 種類
	static const char *m_apTextureFile[];	// テクスチャのファイル
	static int m_nNumAll;		// 総数
	int m_nTexIdx[TYPE_MAX];	// テクスチャのインデックス番号
};

#endif
//==========================================================================
// 
//  影ヘッダー [shadow.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================

#ifndef _SHADOW_H_
#define _SHADOW_H_	// 二重インクルード防止

#include "main.h"
#include "object3D.h"
#include "gamemanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// 影クラス定義
class CShadow : public CObject3D
{
public:

	// 定数
	static constexpr float MIN_ALPHA = 0.25f;	// α値の最小値
	static constexpr float MAX_ALPHA = 0.55f;	// α値の最大値

	// コンストラクタ/デストラクタ
	CShadow(int nPriority = 7);
	~CShadow();

	// 静的メンバ関数
	static CShadow *Create
	(
		CObject* pObject,				// 親オブジェクト
		float fRadius = 50.0f,			// 元の半径
		float fMinAlpha = MIN_ALPHA,	// 最小透明度
		float fMaxAlpha = MAX_ALPHA,	// 最大透明度
		float fLandY = CGameManager::FIELD_LIMIT	// 表示Y座標
	);

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	// メンバ関数
	void BindObject(CObject* pObject) { m_pObject = pObject; }	// 追従オブジェクト割当
	void SetPositionRelative();	// 相対位置設定

private:

	// メンバ変数
	CObject *m_pObject;		// オブジェクトのポインタ
	float m_fOriginRadius;	// 元の半径
	float m_fLandY;			// 表示Y座標
	float m_fMinAlpha;		// 透明度の最小値
	float m_fMaxAlpha;		// 透明度の最大値
	int m_nTexIdx;			// テクスチャのインデックス番号
};


#endif
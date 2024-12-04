//=============================================================================
// 
//  影ヘッダー [shadow.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//=============================================================================

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

	// コンストラクタ/デストラクタ
	CShadow(int nPriority = 7);
	~CShadow();

	// 静的メンバ関数
	static CShadow *Create(CObject* pObject, float fRadius = 50.0f, float fLandY = CGameManager::FIELD_LIMIT);

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
	CObject *m_pObject;	// オブジェクトのポインタ
	float m_fLandY;		// 表示Y座標
	int m_nTexIdx;		// テクスチャのインデックス番号
};


#endif
//============================================================
//
//	ライトマネージャーヘッダー [lightManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "lightDir.h"

//************************************************************
//	クラス定義
//************************************************************
// ライトマネージャークラス
class CLightManager
{
public:
	// 定数
	static constexpr int MAX_NUM = 3;	// 使用ライト数

	// コンストラクタ
	CLightManager();

	// デストラクタ
	~CLightManager();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新
	inline D3DLIGHT9 GetLight(const int nIdx) { return m_apLight[nIdx]->GetLight(); }	// ライト取得

	// 静的メンバ関数
	static CLightManager* Create();	// 生成
	static void Release(CLightManager*& prLightManager);	// 破棄

private:
	// メンバ変数
	CLightDir* m_apLight[MAX_NUM];	// ライト情報
};

#endif	// _LIGHT_MANAGER_H_

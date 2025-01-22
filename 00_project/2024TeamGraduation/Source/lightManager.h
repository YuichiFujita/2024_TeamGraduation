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
	void SetEnableBright(const bool bBright, const float fChangeTime = 0.0f);			// 明るさ設定
	inline D3DLIGHT9 GetLight(const int nIdx) { return m_apLight[nIdx]->GetLight(); }	// ライト取得

	// 静的メンバ関数
	static CLightManager* Create();	// 生成
	static void Release(CLightManager*& prLightManager);	// 破棄

private:
	// メンバ関数
	MyLib::Vector3 ConvertColorToVector3(const D3DXCOLOR& rCol) { return MyLib::Vector3(rCol.r, rCol.g, rCol.b); }							// 色のベクトル変換
	D3DXCOLOR ConvertVector3ToColor(const MyLib::Vector3& rVec, const float fAlpha) { return D3DXCOLOR(rVec.x, rVec.y, rVec.z, fAlpha); }	// ベクトルの色変換

	// メンバ変数
	CLightDir* m_apLight[MAX_NUM];	// ライト情報
	D3DXCOLOR m_aColStart[MAX_NUM];	// ライト開始色
	D3DXCOLOR m_aColEnd[MAX_NUM];	// ライト終了色
	D3DXCOLOR m_colCamStart;		// カメラライト開始色
	D3DXCOLOR m_colCamEnd;			// カメラライト終了色
	float m_fStartTime;		// 開始時間
	float m_fCurTime;		// 経過時間
	bool m_bChange;			// 変動フラグ
};

#endif	// _LIGHT_MANAGER_H_

//============================================================
//
//	スクリーンヘッダー [screen.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCREEN_H_
#define _SCREEN_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// スクリーンクラス
class CScreen : public CObject2D
{
public:
	// コンストラクタ
	CScreen();

	// デストラクタ
	~CScreen();

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画

	// 静的メンバ関数
	static CScreen* Create(const int nScreenTexIdx);	// 生成

	// メンバ関数
	void SetEnableShader(const bool bShader, const float fTime = 0.0f);	// シェーダー設定
	inline bool IsShader() const { return m_bShader; }	// シェーダー取得

private:
	// メンバ変数
	bool m_bShader;		// シェーダーフラグ
	float m_fCurTime;	// 経過時間
};

#endif	// _SCREEN_H_

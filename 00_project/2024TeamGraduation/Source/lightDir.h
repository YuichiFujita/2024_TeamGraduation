//============================================================
//
//	ディレクショナルライトヘッダー [lightDir.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LIGHT_DIR_H_
#define _LIGHT_DIR_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "light.h"

//************************************************************
//	クラス定義
//************************************************************
// ディレクショナルライトクラス
class CLightDir : public CLight
{
public:
	// コンストラクタ
	CLightDir();

	// デストラクタ
	~CLightDir() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画

	// 静的メンバ関数
	static CLightDir* Create();	// 生成
};

#endif	// _LIGHT_DIR_H_

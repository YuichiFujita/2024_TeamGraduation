//==========================================================================
// 
//  実況吹き出しヘッダー [thoughtBalloon.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _THOUGHT_BALLOON_H_
#define _THOUGHT_BALLOON_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "objectBillboard.h"
#include "scrollText2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// 実況吹き出しクラス
class CThoughtBalloon : public CObjectBillboard
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// なにもない
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CThoughtBalloon(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CThoughtBalloon();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;	// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Kill() override;		// 削除
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual void Draw() override;		// 描画

	//=============================
	// 静的メンバ関数
	//=============================
	// TODO : 引数に文字 or 種類を扱う列挙
	static CThoughtBalloon* Create();	// 生成

	//=============================
	// メンバ関数
	//=============================
	inline void SetEnableTextDisp(bool bDisp) { m_pText->SetEnableDisp(bDisp); }	// テキスト表示設定

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(CThoughtBalloon::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_stateFuncList[];	// 関数リスト

	//=============================
	// メンバ関数
	//=============================
	// 生成関連
	HRESULT CreateRenderTexture();	// レンダーテクスチャ生成
	HRESULT CreateTextureObject();	// テクスチャ用オブジェクト生成

	// テクスチャ関連
	void CreateTexture();	// テクスチャ作成

	// 状態関数


	// その他関数


	//=============================
	// メンバ変数
	//=============================
	// テクスチャ関連
	CRenderTexture* m_pRenderScene;	// シーンレンダーテクスチャ
	CScrollText2D* m_pText;			// テキスト情報

	// 状態系
	EState m_state;		// 状態
	float m_fStateTime;	// 状態カウンター

	// その他変数

};


#endif
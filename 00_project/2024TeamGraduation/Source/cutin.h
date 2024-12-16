//==========================================================================
// 
//  カットインヘッダー [cutin.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _CUT_IN_H_
#define _CUT_IN_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// カットインクラス
class CCutIn : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// なにもない状態
		STATE_TIME,		// 時間経過状態
		STATE_END,		// 終了状態
		STATE_MAX
	};

	CCutIn();
	~CCutIn();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Kill() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	//=============================
	// 静的メンバ関数
	//=============================
	static CCutIn* Create();

	//=============================
	// メンバ関数
	//=============================
	bool IsEnd() const;	// 終了フラグ取得

private:

	//=============================
	// メンバ関数
	//=============================
	// 生成関連
	HRESULT CreateRenderTexture();	// レンダーテクスチャ生成
	HRESULT CreateTextureObject();	// テクスチャ用オブジェクト生成
	HRESULT CreateObject2D();		// プレイヤーテクスチャ貼付ポリゴン/輝き生成

	// テクスチャ関連
	void CreateTexture();	// テクスチャ作成

	// その他関数
	void UpdateBlinkColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 背景色の更新

	//=============================
	// メンバ変数
	//=============================
	// テクスチャ関連
	CRenderTexture* m_pRenderScene;	// シーンレンダーテクスチャ

	// その他変数
	CObject2D* m_pBlink;	// 輝き情報
	CObject2D* m_pPlayer2D;	// プレイヤーテクスチャ情報
	float m_fBlinkTime;		// 背景輝きカウンター
};


#endif
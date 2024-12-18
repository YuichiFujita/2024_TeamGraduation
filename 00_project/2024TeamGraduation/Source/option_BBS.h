//==========================================================================
// 
//  オプションの掲示板ヘッダー [option_BBS.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _OPTION_BBS_H_
#define _OPTION_BBS_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "objectX.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CBalloonFrame;
class COptionMenu;

//==========================================================================
// クラス定義
//==========================================================================
// 実況吹き出しクラス
class COption_BBS : public CObjectX
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
	COption_BBS(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~COption_BBS();

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
	static COption_BBS* Create();	// 生成

	//=============================
	// メンバ関数
	//=============================

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(COption_BBS::*STATE_FUNC)(const float, const float, const float);
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

	// 状態系
	EState m_state;		// 状態
	float m_fStateTime;	// 状態カウンター

	// その他変数
	COptionMenu* m_pOptionMenu;	// オプションメニュー
};


#endif
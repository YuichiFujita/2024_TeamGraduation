//============================================================
//
//	ゲームセットUIヘッダー [gamesetUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAME_SET_UI_H_
#define _GAME_SET_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
// 前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// ゲームセットUIクラス
class CGameSetUI : public CObject
{
public:
	// コンストラクタ
	CGameSetUI();

	// デストラクタ
	~CGameSetUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetEnableDisp(const bool bDisp) override;	// 描画状況設定

	// 静的メンバ関数
	static CGameSetUI* Create();	// 生成

	// メンバ関数
	bool IsEnd();	// 演出終了確認

private:
	// メンバ関数
	HRESULT CreateUI();	// UI生成

	// メンバ変数
	CObject2D* m_pGameSet;	// ゲームセット情報
	float m_fCurTime;		// 経過時間
};

#endif	// _GAME_SET_UI_H_

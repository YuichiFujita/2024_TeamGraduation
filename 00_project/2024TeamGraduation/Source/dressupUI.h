//============================================================
//
//	ドレスアップUIヘッダー [dressupUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _DRESSUP_UI_H_
#define _DRESSUP_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// ドレスアップUIクラス
class CDressupUI : public CObject
{
public:
	// コンストラクタ
	CDressupUI();

	// デストラクタ
	~CDressupUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetType(const TYPE type) override;					// 種類設定
	void SetEnableDisp(const bool bDraw) override;			// 描画状況設定
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定
	void SetRotation(const MyLib::Vector3& rot) override;	// 向き設定

	// 静的メンバ関数
	static CDressupUI* Create	// 生成
	( // 引数
		const MyLib::Vector3& rPos	// 原点位置
	);

	// メンバ関数


private:
	// メンバ関数
	void SetPositionRelative();	// 相対位置設定

	// メンバ変数
	
};

#endif	// _DRESSUP_UI_H_

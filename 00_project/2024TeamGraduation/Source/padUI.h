//==========================================================================
//
//	パッドUIヘッダー [padUI.h]
//	Author：相馬靜雅
//
//==========================================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PAD_UI_H_
#define _PAD_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D_Anim.h"
#include "arrowUI.h"

//************************************************************
// 前方宣言
//************************************************************

//************************************************************
// クラス定義
//************************************************************
// パッドUIクラス
class CPadUI : public CObject2D_Anim
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPadUI(int nPriority);
	~CPadUI() override;

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定
	virtual void SetEnableDisp(bool bDisp) override;		// 描画状況設定

	//=============================
	// メンバ関数
	//=============================
	void Decide();		// 決定
	void Cancel();		// キャンセル
	void SetSizeByWidth(const float width);					// 横幅からサイズ設定
	void SetAction(CArrowUI::EDirection dir);				// アクション設定
	void SetEnableDispArrow(bool bDisp);					// 矢印の描画設定
	CArrowUI* GetArrowUI(int i) { return m_apArrow[i]; };	// 矢印UIの情報取得

	//=============================
	// 静的メンバ関数
	//=============================
	static CPadUI* Create(const MyLib::Vector3& pos, const float width, const int pattern, int nPriority = mylib_const::PRIORITY_DEF2D);	// 生成

private:

	//=============================
	// メンバ関数
	//=============================
	HRESULT CreateArrowUI();	// 矢印UIの生成

	//=============================
	// メンバ変数
	//=============================
	CArrowUI* m_apArrow[CArrowUI::EDirection::DIRECTION_MAX];	// 矢印の情報
};

#endif	// _ARROW_UI_H_

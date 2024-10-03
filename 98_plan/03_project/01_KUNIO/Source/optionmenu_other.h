//=============================================================================
// 
//  その他メニューヘッダー [optionmenu_other.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OPTIONMENU_OTHER_H_
#define _OPTIONMENU_OTHER_H_	// 二重インクルード防止

#include "optionmenu.h"

class CMultiNumber;
class CSelectDraw;

//==========================================================================
// クラス定義
//==========================================================================
// その他メニュークラス
class COptionMenu_Other : public COptionMenu
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	enum Select
	{
		SELECT_WINDOW = 0,	// ウィンドウモード
		SELECT_VIB,			// バイブレーション
		SELECT_MAX
	};

	enum Active	// アクティブ状態
	{
		ON = 0,
		OFF,
		MAX
	};

	//=============================
	// 構造体定義
	//=============================
	struct SwitchInfo
	{
		Active active;		// アクティブ状態
		CObject2D* pSwitch;	// 切り替え

		SwitchInfo() : active(Active::ON), pSwitch(nullptr) {}
	};

	COptionMenu_Other(int nPriority = 7);
	~COptionMenu_Other();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Kill() override;	// 削除

private:

	//=============================
	// メンバ関数
	//=============================
	virtual void StateEdit() override;	// 編集
	void CreateText();					// テキスト生成
	void CreateSwitch();				// 切り替え生成
	void ActiveByTexture(Select _select, Active _active);	// アクティブ状態別テクスチャ
	void ResetSelect();				// 選択肢リセット

	//=============================
	// メンバ変数
	//=============================
	Select m_selectType;					// 選択中の種類
	CObject2D* m_pText[Select::SELECT_MAX];	// テキスト
	SwitchInfo m_switchInfo[Select::SELECT_MAX];	// 切り替え情報
	CSelectDraw* m_pDrawing;				// 選択肢筆
};


#endif
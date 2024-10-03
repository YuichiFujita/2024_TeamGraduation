//=============================================================================
// 
//  キーコンフィグメニューヘッダー [optionmenu_keyconfig.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OPTIONMENU_KEYCONFIG_H_
#define _OPTIONMENU_KEYCONFIG_H_	// 二重インクルード防止

#include "optionmenu.h"

class CMultiNumber;
class CSelectDraw;



#include "keyconfig.h"

// 前方宣言
class CControlKeyDisp;

//==========================================================================
// クラス定義
//==========================================================================
// キーコンフィグメニュークラス
class COptionMenu_Keyconfig : public COptionMenu
{
private:

	// キー情報
	struct KEYINFO
	{
		CObject2D* s_p2D;				// 説明文字
		CControlKeyDisp* s_pKeyDisp;	// キーコンフィグ表示

		// コンストラクタ
		KEYINFO() : s_p2D(nullptr), s_pKeyDisp(nullptr){}
	};

	// 切り替え情報
	struct CHENGEINFO
	{
		CObject2D* s_p2D;				// 説明文字
		CControlKeyDisp* s_pKeyDispOK;	// キーコンフィグ表示
		CControlKeyDisp* s_pKeyDispNO;	// キーコンフィグ表示
		CSelectDraw* pSelect;			// 選択肢筆

		// コンストラクタ
		CHENGEINFO() : pSelect(nullptr), s_p2D(nullptr), s_pKeyDispOK(nullptr), s_pKeyDispNO(nullptr) { }
	};

public:

	COptionMenu_Keyconfig(int nPriority = 7);
	~COptionMenu_Keyconfig();

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
	virtual void StateEdit() override;		// 編集
	void Chenge();
	void SetAlpha();
	void ResetSelect();		// 選択肢リセット

	//=============================
	// メンバ変数
	//=============================
	float m_Alpha;
	float m_fTime;
	int m_SelectKey;	// 選択中
	bool m_bDeath;		// 死亡フラグ
	KEYINFO m_aKeyConfig[INGAME::ACT_MAX];	// キーコンフィグ用
	CHENGEINFO m_checkconfig;	// 決定戻る変更用
	CObject2D* m_pTitle2D;		// 説明用ポリゴン
	CSelectDraw* m_pSelect;		// 選択肢筆
};

#endif
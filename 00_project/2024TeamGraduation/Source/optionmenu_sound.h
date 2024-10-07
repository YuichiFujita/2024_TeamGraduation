//=============================================================================
// 
//  サウンドメニューヘッダー [optionmenu_sound.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OPTIONMENU_SOUND_H_
#define _OPTIONMENU_SOUND_H_	// 二重インクルード防止

#include "optionmenu.h"

class CMultiNumber;
class CSelectDraw;

//==========================================================================
// クラス定義
//==========================================================================
// オプションメニュークラス
class COptionMenu_Sound : public COptionMenu
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	enum Select
	{
		SELECT_MASTER = 0,
		SELECT_SE,
		SELECT_BGM,
		SELECT_MAX
	};

	COptionMenu_Sound(int nPriority = 7);
	~COptionMenu_Sound();

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
	void CreateText();	// テキスト生成
	void ResetSelect();	// 選択肢リセット

	//=============================
	// メンバ変数
	//=============================
	Select m_selectType;					// 選択中の種類
	CMultiNumber* m_pNumber_Master;			// マスターボリュームの数字
	CMultiNumber* m_pNumber_SE;				// SEの数字
	CMultiNumber* m_pNumber_BGM;			// BGMの数字
	CObject2D* m_pText[Select::SELECT_MAX];	// テキスト
	CSelectDraw* m_pDrawing;				// 選択肢筆
};


#endif
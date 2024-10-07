//=============================================================================
// 
//  ゲームヘッダー [game.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// 二重インクルード防止

#include "scene.h"
#include "gamemanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CEdit;

//==========================================================================
// クラス定義
//==========================================================================
// ゲームクラス定義
class CGame : public CScene
{
public:

	// エディットタイプ
	enum EditType
	{
		EDITTYPE_OFF = 0,		// オフ
		EDITTYPE_MAP,			// マップ
		EDITTYPE_OBSTACLE,		// 障害物
		EDITTYPE_COURSE,		// コース
		EDITTYPE_WATERSTONE,	// 水中石
		EDITTYPE_JUDGEZONE,		// 判定ゾーン
		EDITTYPE_MAX
	};

	CGame();
	~CGame();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	EditType GetEditType() { return m_EditType; }
	CGameManager *GetGameManager();
	void SetEnableClear(bool bClear) { m_clear = bClear; }		// クリアのフラグ
	bool IsClear() { return m_clear; }						// クリアフラグの取得
	bool IsEdit() { return m_bEdit; }							// エディットの判定

	// 静的関数
	static CGame* GetInstance() { return m_pThisPtr; }	// インスタンス取得
	static CGame* Create(CScene::MODE mode);			// 生成処理

protected:

	virtual void InitByMode();	// モード別初期化

	//=============================
	// メンバ関数
	//=============================
	void EditReset();
	void ChangeEdit();	// エディット切り替え処理

	//=============================
	// メンバ変数
	//=============================
	CGameManager *m_pGameManager;			// ゲームマネージャのオブジェクト
	EditType m_EditType;					// エディットの種類
	CEdit* m_pEdit;							// エディター
	bool m_bEdit;				// エディットの判定
	bool m_clear; // クリア判定

	static CGame* m_pThisPtr;	// 自身のポインタ
};



#endif
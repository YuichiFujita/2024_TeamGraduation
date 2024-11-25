//=============================================================================
//
// モテマネージャ処理 [charmManager.h]
// Author : Kai Takada
//
//=============================================================================
#ifndef _CHARMMANAGER_H_		// このマクロ定義がされていなかったら
#define _CHARMMANAGER_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;

//==========================================================================
// クラス定義
//==========================================================================
// モテマネージャクラス
class CCharmManager
{
public:

	CCharmManager();
	~CCharmManager();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init();		// 初期化
	virtual void Uninit();		// 終了

	//--------------------------
	// その他
	//--------------------------

	// 静的関数
	static CCharmManager* Create();	// 生成処理
	static CCharmManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

	//-----------------------------
	// 非モテ関数
	//-----------------------------
	void UnCharm(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 非モテまとめ

private:

	//-----------------------------
	// メンバ関数
	//-----------------------------
	void LongHold(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 持ち続けてる
	void EdgeEscape(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 端に逃げ続ける
	
	bool CheckEdgeEscape(CPlayer* pPlayer);		// 端に逃げ続ける

	//=============================
	// メンバ変数
	//=============================

	static CCharmManager* m_pThisPtr;	// 自身のポインタ
};

#endif
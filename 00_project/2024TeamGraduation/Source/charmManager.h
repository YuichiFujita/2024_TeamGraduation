//=============================================================================
//
// モテマネージャ処理 [charmManager.h]
// Author : Kai Takada
//
//=============================================================================
#ifndef _CHARMMANAGER_H_		// このマクロ定義がされていなかったら
#define _CHARMMANAGER_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gameManager.h"
#include "charmValueManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;
class CCollisionLine_Box;

//==========================================================================
// クラス定義
//==========================================================================
// モテマネージャクラス
class CCharmManager
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr float TIME_CHAIN = 3.0f;	// チェイン可能な時間

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CCharmManager();
	~CCharmManager();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init();		// 初期化
	virtual void Uninit();		// 終了
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新

	//-----------------------------
	// モテ盛り上げ関数
	//-----------------------------
	float GetPrisetHypeTime(const CCharmValueManager::ETypeAdd preset);									// プリセット盛り上がり時間取得
	void SetHypeTime(const CGameManager::ETeamSide team, const CCharmValueManager::ETypeAdd preset);	// 盛り上がり時間設定 (プリセット)
	inline void SetHypeTime(const CGameManager::ETeamSide team, const float fTime)	{ m_fHypeTime[team] = fTime; }			// 盛り上がり時間設定 (時間指定)
	inline float GetHypeTime(const CGameManager::ETeamSide team) const				{ return m_fHypeTime[team]; }			// 盛り上がり時間取得
	inline bool IsHype(const CGameManager::ETeamSide team) const					{ return (m_fHypeTime[team] > 0.0f); }	// 盛り上がりフラグ取得

	//-----------------------------
	// 非モテ関数
	//-----------------------------
	void UnCharm(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 非モテまとめ

	//--------------------------
	// その他
	//--------------------------

	//=============================
	// 静的関数
	//=============================
	static CCharmManager* Create();		// 生成処理
	static CCharmManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:

	//=============================
	// メンバ関数
	//=============================
	void LongHold(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 持ち続けてる
	void EdgeEscape(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 端に逃げ続ける
	bool CheckEdgeEscape(CPlayer* pPlayer);	// 端に逃げ続ける

	//=============================
	// メンバ変数
	//=============================
#if _DEBUG
	CCollisionLine_Box* m_pCourtSizeBox[4];	// コートサイズのボックス
#endif
	static CCharmManager* m_pThisPtr;	// 自身のポインタ
	float m_fHypeTime[CGameManager::ETeamSide::SIDE_MAX];	// 盛り上がり時間
};

#endif
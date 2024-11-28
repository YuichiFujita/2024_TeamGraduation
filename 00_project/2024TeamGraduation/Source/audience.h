//==========================================================================
// 
//  観客ヘッダー [audience.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _AUDIENCE_H_
#define _AUDIENCE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"
#include "gamemanager.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// 観客クラス
class CAudience : public CObject
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr float MAX_LEFT_LINE = -2100.0f;	// 左の生成位置上限
	static constexpr float MAX_RIGHT_LINE = 2100.0f;	// 右の生成位置上限
	static constexpr float SPAWN_SIDE_LINE = 2500.0f;	// 生成する横位置
	static constexpr int MAX_WATCH = 500;				// 観戦する最大人数

	//=============================
	// 列挙型定義
	//=============================
	// オブジェクト種類
	enum EObjType
	{
		OBJTYPE_NONE = 0,	// なし
		OBJTYPE_ANIM,		// 3Dポリゴン
		OBJTYPE_LOWPOLY,	// ローポリキャラクター
		OBJTYPE_HIGHPOLY,	// ハイポリキャラクター
		OBJTYPE_MAX			// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_SPAWN = 0,	// 入場状態
		STATE_NORMAL,		// 通常状態
		STATE_JUMP,			// 盛り上がり状態
		STATE_SPECIAL,		// スペシャル状態
		STATE_DESPAWN,		// 退場状態
		STATE_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudience(EObjType type, CGameManager::ETeamSide team, int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CAudience();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// 削除

	//=============================
	// 純粋仮想関数
	//=============================
	virtual void SetPosition(const MyLib::Vector3& pos) override = 0;	// 位置設定
	virtual MyLib::Vector3 GetPosition() const override = 0;			// 位置取得
	virtual void SetMove(const MyLib::Vector3& move) override = 0;		// 移動量設定
	virtual MyLib::Vector3 GetMove() const override = 0;				// 移動量取得
	virtual void SetRotation(const MyLib::Vector3& rot) override = 0;	// 向き設定
	virtual MyLib::Vector3 GetRotation() const override = 0;			// 向き取得

	//=============================
	// 仮想関数
	//=============================


	//=============================
	// メンバ関数
	//=============================
	void SetEnableJump(const bool bJump);	// 盛り上がり設定
	void SetSpecial();						// スペシャル設定
	virtual bool SetDespawn(EObjType type = OBJTYPE_NONE);	// 退場設定

	//=============================
	// 静的メンバ関数
	//=============================
	static CAudience* Create(EObjType type, CGameManager::ETeamSide team);				// 生成
	static HRESULT SetNumWatch(const int nNumWatch, CGameManager::ETeamSide team);		// 観戦中の人数設定
	static int GetNumWatchAll(CGameManager::ETeamSide team);								// 全観戦中の人数取得
	static void SetEnableJumpAll(const bool bJump, CGameManager::ETeamSide team);		// 全盛り上がり設定
	static void SetSpecialAll(CGameManager::ETeamSide team);								// 全スペシャル設定
	static void SetDespawnAll(CGameManager::ETeamSide team, const int nNumDespawn = -1);	// 全退場設定

protected:

	//=============================
	// 純粋仮想関数
	//=============================
	// 状態関数
	virtual int UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// 入場状態の更新
	virtual int UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// 通常状態の更新
	virtual int UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// 盛り上がり状態の更新
	virtual int UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// スペシャル状態の更新
	virtual int UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// 退場状態の更新

	// ゲッター/セッター
	virtual void SetMotion(const int nMotion) = 0;	// モーション設定
	virtual void EndSettingSpawn() = 0;				// スポーン終了時の設定

	//=============================
	// メンバ関数
	//=============================
	// ゲッター/セッター
	inline void SetSpawnPosition(const MyLib::Vector3& rPos)	{ m_posSpawn = rPos;}	// 入場位置設定
	inline MyLib::Vector3 GetSpawnPosition() const				{ return m_posSpawn; }	// 入場位置取得
	inline void SetWatchPosition(const MyLib::Vector3& rPos)	{ m_posWatch = rPos;}	// 観戦位置設定
	inline MyLib::Vector3 GetWatchPosition() const				{ return m_posWatch; }	// 観戦位置取得
	inline CGameManager::ETeamSide GetTeam() const				{ return m_team; }		// 応援チーム取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef int(CAudience::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 汎用関数
	bool UpdateGravity(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaTime, const float fDeltaRate, const float fSlowRate, const float fGravityRate = 1.0f);	// 重力更新

	//=============================
	// 静的メンバ変数
	//=============================
	static CListManager<CAudience> m_list;	// リスト
	static int m_aNumWatchAll[CGameManager::ETeamSide::SIDE_MAX];			// 観戦中の人数

	//=============================
	// メンバ変数
	//=============================
	const CGameManager::ETeamSide m_team;	// 応援チーム
	const EObjType m_type;		// オブジェクト種類
	const float m_fJumpLevel;	// ジャンプ量
	MyLib::Vector3 m_posSpawn;	// 入場位置
	MyLib::Vector3 m_posWatch;	// 観戦位置
	MyLib::Vector3 m_posDespawnStart;	// 退場開始位置
	EState m_state;		// 状態
	float m_fTimeState;	// 状態管理時間
};

#endif

//=============================================================================
// 
//  プレイヤーヘッダー [player.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

#include "main.h"
#include "objectChara.h"
#include "motion.h"

// 前方宣言
class CShadow;
class CMagicCircle;
class CTargetPoint;
class CHP_Gauge;

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayer : public CObjectChara
{
public:

	// 列挙型定義
#if ACTION_MODE
	typedef enum
	{
		SHOTMODE_TRIGGER = 0,	// トリガー
		SHOTMODE_RELEASE,		// リリース
		SHOTMODE_PRESS,			// プレス
		SHOTMODE_REPEAT,		// リピート
		SHOTMODE_MAX
	}SHOTMODE;

#else
	typedef enum
	{
		SHOTMODE_NONE = 0,	// なし
		SHOTMODE_REPEAT,	// リピート
		SHOTMODE_MAX
	}SHOTMODE;

	typedef enum
	{
		ATK_DEBUG_POWER = 0,	// パワー
		ATK_DEBUG_CROWD,	// 群体
		ATK_DEBUG_SPEED,	// スピード
		ATK_DEBUG_MAX
	}ATK_DEBUG;
#endif

	CPlayer(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CPlayer();

	static CPlayer *Create(const std::string pTextFile);

	// オーバーライドされた関数
	HRESULT Init(void);
	HRESULT Init(const std::string pTextFile);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Hit(const int nValue);	// ヒット処理


	void SetPartsRot(void);		// 向き設定
	void SetSphereCollider(void);		// 球の当たり判定設定
	void SaveNowMotion(void);	// 現在のモーションセーブ

private:
	// 列挙型定義
	typedef enum
	{
		MOTION_DEF = 0,	// ニュートラルモーション
		MOTION_WALK,	// 移動モーション
		MOTION_SUPERATK,		// 超攻撃
		MOTION_MAGIC_ICE,		// 氷魔法
		MOTION_SPEED_RUSH,		// 高速ラッシュ
		MOTION_SPEED_RUSH_MOVE,		// 高速ラッシュ(移動)
		MOTION_MAGIC_WAIT,		// 魔法待機
		MOTION_MAX
	}MOTION;

	// 状態定義
	typedef enum
	{
		STATE_NONE = 0,	// なにもない
		STATE_DMG,		// ダメージ
		STATE_DEAD,		// 死
		STATE_ATTACK,	// 攻撃処理
		STATE_MAX
	}STATE;

	void UpdateState(void);	// 状態更新処理
	void Controll(void);	// 操作
	void MotionSet(void);	// モーションの設定
	void Collision(void);	// 当たり判定
	void Atack(void);		// 攻撃
	void SetAttackInfo(void);


	void SaveMotionInfo(void);	// 現在のモーションセーブ
	void SaveSetUpInfo(void);	// 現在のセットアップ情報セーブ

	STATE m_state;				// 状態
	D3DXCOLOR m_mMatcol;		// マテリアルの色
	int m_Shotmode;				// 射撃の種類
	bool m_bJump;				// ジャンプ中かどうか
	bool m_bMove;				// 移動中かどうか
	bool m_bATK;				// 攻撃中かどうか
	bool m_bMWait;				// 魔法待機中かどうか
	int m_nCntState;			// 状態遷移カウンター
	int m_nLife;				// 体力
	int m_nTexIdx;				// テクスチャのインデックス番号
	int m_nIdxXFile;			// Xファイルのインデックス番号
	CMotion *m_pMotion;			// モーションの情報
	CShadow *m_pShadow;			// 影の情報

	int m_nNowParts;			// 現在のパーツ
	int m_nNowMotion;			// 現在のモーション
	int m_nNowKey;				// 現在のキー
	int m_nNowAttackIdx;		// 現在の攻撃インデックス番号
	float m_fSetUpBuff;			// セットアップバフ

	int m_nNowColliderIdx;		// 現在のコライダーインデックス番号
	CMotion::Key m_CopyData;	// コピー用パーツデータ
	bool m_bStop;				// ストップするか
};



#endif
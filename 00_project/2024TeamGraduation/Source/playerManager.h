//==========================================================================
// 
//  プレイヤーマネージャーヘッダー [playerManager.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "listmanager.h"
#include "gamemanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;
class CBindKey;

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤーマネージャークラス
class CPlayerManager
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 外野ポジション列挙
	enum EOutPos
	{
		OUT_LEFT_FAR = 0,	// 左奥
		OUT_LEFT,			// 左
		OUT_LEFT_NEAR,		// 左手前
		OUT_RIGHT_FAR,		// 右奥
		OUT_RIGHT,			// 右
		OUT_RIGHT_NEAR,		// 右手前
		OUT_MAX				// この列挙型の総数
	};

	//=============================
	// 構造体定義
	//=============================
	// 外野情報構造体
	struct SOutInfo
	{
		MyLib::Vector3 posLeft;		// 移動可能左位置
		MyLib::Vector3 posRight;	// 移動可能右位置
		CBindKey* pKeyLeft;			// 左移動キー割当
		CBindKey* pKeyRight;		// 右移動キー割当
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerManager();
	~CPlayerManager();

	//=============================
	// 仮想関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// メンバ関数
	//=============================
	int RegistPlayer(CPlayer* pPlayer, const int nPosIdx = -1);	// プレイヤー登録
	void DeletePlayer(CPlayer* pPlayer);			// プレイヤー削除
	SOutInfo GetOutInfo(const EOutPos out);			// 外野情報取得
	CPlayer* GetOutPlayer(const EOutPos out);		// 外野プレイヤー取得
	EOutPos GetOutPosition(const CPlayer* pPlayer);	// 外野ポジション取得
	void ChangeUser(CPlayer* pPlayer);				// AIベースのユーザー変更
	void ChangeAI(CPlayer* pPlayer);				// ユーザーベースのAI変更

	//=============================
	// 静的メンバ関数
	//=============================
	static CPlayerManager* Create();	// 生成
	static CPlayerManager* GetInstance() { return m_pInstance; }	// インスタンス取得

private:

	//=============================
	// 関数リスト
	//=============================
	// 外野情報関数
	typedef SOutInfo(CPlayerManager::*INFO_FUNC)();
	static INFO_FUNC m_InfoFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	int RegistOutPlayer(CPlayer* pPlayer, const int nPosIdx);	// 外野プレイヤー登録 (ポジション指定)
	int RegistOutPlayer(CPlayer* pPlayer);	// 外野プレイヤー登録
	void DeleteOutPlayer(CPlayer* pPlayer);	// 外野プレイヤー削除
	int RegistInPlayer(CPlayer* pPlayer);	// 内野プレイヤー登録
	void DeleteInPlayer(CPlayer* pPlayer);	// 内野プレイヤー削除
	SOutInfo GetInfoLeftFar();		// 左奥の外野情報取得
	SOutInfo GetInfoLeft();			// 左の外野情報取得
	SOutInfo GetInfoLeftNear();		// 左手前の外野情報取得
	SOutInfo GetInfoRightFar();		// 右奥の外野情報取得
	SOutInfo GetInfoRight();		// 右の外野情報取得
	SOutInfo GetInfoRightNear();	// 右手前の外野情報取得

	//=============================
	// メンバ変数
	//=============================
	CListManager<CPlayer> m_listInLeft;		// 内野左プレイヤー
	CListManager<CPlayer> m_listInRight;	// 内野右プレイヤー
	CPlayer* m_apOut[OUT_MAX];				// 外野プレイヤー

	//=============================
	// 静的メンバ変数
	//=============================
	static CPlayerManager* m_pInstance;	// 自身のインスタンス
};

#endif

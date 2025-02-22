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
#include "manager.h"
#include "listmanager.h"
#include "gamemanager.h"
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;
class CBindKey;

//==========================================================================
// 名前空間
//==========================================================================
namespace PlayerManager
{
	const std::string TEXT_PLAYERINFO = "data\\TEXT\\playerManager\\playerInfo.txt";	// プレイヤー情報のテキストファイル
}

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

	// 生成する種類
	enum EType
	{
		TYPE_GAME = 0,	// ゲーム
		TYPE_SPAWN,		// 登場時
		TYPE_RESULT,	// リザルト
		TYPE_MAX
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

	struct LoadInfo		// 読み込み情報
	{
		LoadInfo() :
			nControllIdx	(-1),	// 操作するインデックス番号
			eHanded			(CPlayer::EHandedness::HAND_R),	// 利き手
			eBody			(CPlayer::EBody::BODY_NORMAL),	// 体型
			nHair			(0),	// 髪
			nAccessory		(0),	// アクセサリー
			nFace			(0)		// 顔
		{}

		int nControllIdx;				// 操作するインデックス番号
		CPlayer::EHandedness eHanded;	// 利き手
		CPlayer::EBody eBody;			// 体型
		int nHair;						// 髪
		int nAccessory;					// アクセサリー
		int nFace;						// 顔
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerManager();
	virtual ~CPlayerManager();

	//=============================
	// 仮想関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit() = 0;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// メンバ関数
	//=============================
	int RegistPlayer(CPlayer* pPlayer, const int nPosIdx = -1);	// プレイヤー登録
	void DeletePlayer(CPlayer* pPlayer);	// プレイヤー削除

	CListManager<CPlayer> GetInList(const CGameManager::ETeamSide team);	// 内野プレイヤーリスト取得
	CPlayer* GetOutPlayer(const EOutPos out);			// 外野プレイヤー取得
	EOutPos GetOutPosition(const CPlayer* pPlayer);		// 外野ポジション取得
	SOutInfo GetOutInfo(const EOutPos out);				// 外野情報取得
	void ChangeAIToUser(CPlayer* pPlayer);				// ユーザーベースのAI変更
	void ChangeUserToAI(CPlayer* pPlayer);				// AIベースのユーザー変更
	void SwapBase(CPlayer* pPlayer, CPlayer* pTarget);	// ベース入替

	void SetEnableUserChange(const bool bUserChange) { m_bUserChange = bUserChange; }	// ユーザー変更操作フラグ設定
	bool IsUserChange() const { return m_bUserChange; }	// ユーザー変更操作フラグ取得
	
	// ファイル関連
	static void Save	// セーブ処理
	( // 引数
		const std::vector<LoadInfo>& inLeftInfo,
		const std::vector<LoadInfo>& inRightInfo,
		const std::vector<LoadInfo>& outLeftInfo,
		const std::vector<LoadInfo>& outRightInfo
	);

	//=============================
	// その他関数
	//=============================
	void CautionInAll(const CGameManager::ETeamSide team);	// 警戒処理
	void CautionOutAll();	// 警戒処理

	//=============================
	// 静的メンバ関数
	//=============================
	static CPlayerManager* Create(EType type);	// 生成
	static CPlayerManager* GetInstance() { return m_pInstance; }	// インスタンス取得
	static std::vector<LoadInfo> GetLoadInInfo(CGameManager::ETeamSide side) { return m_vecInLoadInfo[side]; }		// 内野読み込み情報
	static std::vector<LoadInfo> GetLoadOutInfo(CGameManager::ETeamSide side) { return m_vecOutLoadInfo[side]; }	// 外野読み込み情報
	static void SavePlayerInfo(std::ofstream* File, const std::vector<LoadInfo>& Info);	// プレイヤー情報セーブ
	static void Load();
	static void LoadPlayerInfo(std::ifstream* File, int nTeam, int nIdxPlayer, std::vector<LoadInfo>* pInfo);	// プレイヤー情報読み込み

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
	// 内外野操作
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
	bool m_bUserChange;						// ユーザー変更操作フラグ

	// ファイル関連
	static std::vector<LoadInfo> m_vecInLoadInfo[CGameManager::ETeamSide::SIDE_MAX];	// 内野読み込み情報
	static std::vector<LoadInfo> m_vecOutLoadInfo[CGameManager::ETeamSide::SIDE_MAX];	// 外野読み込み情報

protected:

	//=============================
	// メンバ関数
	//=============================
	// 生成
	virtual HRESULT CreatePlayer() = 0;	// プレイヤー生成
	virtual HRESULT CreateOutPlayer(CGameManager::ETeamSide team, const LoadInfo& info);	// 外野プレイヤー生成
	virtual HRESULT CreateLeftPlayer(int i, const LoadInfo& info);							// 左のプレイヤー生成
	HRESULT CreateLeftPlayer(int i, const LoadInfo& info, const MyLib::Vector3& pos);		// 左のプレイヤー生成(位置あり)
	virtual HRESULT CreateRightPlayer(int i, const LoadInfo& info);							// 右のプレイヤー生成
	HRESULT CreateRightPlayer(int i, const LoadInfo& info, const MyLib::Vector3& pos);		// 右のプレイヤー生成(位置あり)

	//=============================
	// 静的メンバ変数
	//=============================
	static CPlayerManager* m_pInstance;	// 自身のインスタンス
};

#endif

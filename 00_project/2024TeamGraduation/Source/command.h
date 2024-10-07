//=============================================================================
//
// コマンドヘッダー [command.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _COMMAND_H_
#define _COMMAND_H_	// 二重インクルード防止

//==========================================================================
// クラス定義
//==========================================================================
// キーコンフィグ
class CCommand
{
public:

	// 種類
	enum TYPE
	{
		TYPE_ROTATION = 0,	// 高速回転
		TYPE_UP,			// 上昇
		TYPE_FLY,			// 飛ぶ
		TYPE_MAX
	};

protected:

	// 入力情報
	struct Info
	{
		int nButton;	// 入力№
		bool bActive;	// 入力されたかどうか

		// コンストラクタ
		Info() 
		{ 
			nButton = 1; 
			bActive = false; 
		}
	};

public:

	CCommand();
	virtual ~CCommand() {}	// デストラクタ

	// 基本処理
	void Uninit();	// 終了処理
	virtual bool GetCommand() = 0;	// 入力確認
	void Load(std::string& string);
	void SetId(const int nId) { m_nId = nId; }
	int GetId() { return m_nId; }
	void SetType(const int nType) { m_nType = nType; }
	int GetType() { return m_nType; }

protected:

	// メンバ関数
	void SetCommand(int nKey);	// コマンドキー設定
	void Reset();

	// 取得系メンバ関数
	int GetNowFlame() { return m_nNowFlame; }	// 現在のフレーム取得
	int GetMaxFlame() { return m_nMaxFlame; }	// 最大フレーム取得

	// 設定系メンバ関数
	void SetNowFlame(int nFlame);	// 現在のフレーム設定	
	void SetMaxFlame(int nFlame) { m_nMaxFlame = nFlame; }	// 最大フレーム設定

	// メンバ変数
	std::vector<Info> m_Info;	// 情報

private:

	// メンバ変数
	int m_nType;		// 種類
	int m_nId;			// 番号
	int m_nNowFlame;	// 現在のフレーム
	int m_nMaxFlame;	// 入力受付最大フレーム
};

//==========================================================================
// 派生クラス定義
//==========================================================================
// ゲームパッド
class CCommandPad : public CCommand
{
public:

	// 入力方向列挙
	enum DIRECTION
	{
		STICK_NONE = 0,		// 押してない
		STICK_UP,			// 上
		STICK_DOWN,			// 下
		STICK_LEFT,			// 左
		STICK_RIGHT,		// 右
		STICK_LEFTUP,		// 左上	
		STICK_RIGHTUP,		// 右上
		STICK_LEFTDOWN,		// 左下
		STICK_RIGHTDOWN,	// 右下
		STICK_MAX
	};

public:

	// コンストラクタ
	CCommandPad() {}
	~CCommandPad() override {}	// デストラクタ

	// メンバ関数
	bool GetCommand() override;	// 入力確認
	DIRECTION GetDirection();	// 入力方向取得

private:
};

#endif
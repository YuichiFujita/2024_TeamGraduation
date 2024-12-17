//==========================================================================
// 
//  ドレスアップ設定処理 [entry_dressup.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================

#ifndef _ENTRY_DRESSUP_H_
#define _ENTRY_DRESSUP_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "entryscene.h"
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CDressup;

//==========================================================================
// クラス定義
//==========================================================================
// ドレスアップ設定クラス
class CEntry_Dressup : public CEntryScene
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// エディットする種類
	enum EEditType
	{
		EDIT_PROCESS = 0,	// 実際の変更
		EDIT_CHANGETYPE,	// 変更する種類
		EDIT_MAX
	};

	// 変更する種類
	enum EChangeType
	{
		TYPE_HAIR = 0,		// 髪
		TYPE_ACCESSORY,		// アクセサリー
		TYPE_FACE,			// 顔
		TYPE_BODY,			// 体型
		TYPE_HANDEDNESS,	// 利き手
		TYPE_MAX
	};

	//=============================
	// 構造体定義
	//=============================
	struct SEntryDressup	// 着せ替え情報
	{
		CPlayer* pPlayer;		// プレイヤー
		CDressup* pHair;		// 髪着せ替え
		CDressup* pAccessory;	// アクセ着せ替え
		CDressup* pFace;		// 顔着せ替え
		EEditType editType;		// エディットする種類
		EChangeType changeType;	// 変更する種類

		// コンストラクタ
		SEntryDressup() : pPlayer(nullptr), pHair(nullptr), pAccessory(nullptr), pFace(nullptr), editType(EEditType::EDIT_PROCESS), changeType(EChangeType::TYPE_HAIR) {}
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CEntry_Dressup();
	~CEntry_Dressup();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init() override;	// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//-----------------------------
	// その他
	//-----------------------------
	virtual void Debug() override;

private:

	//=============================
	// メンバ関数
	//=============================
	void ChangeEditType(int nLoop, int nControllIdx);	// エディットする種類変更
	void ChangeChangeType(int nLoop, int nControllIdx);	// 変更する箇所の種類変更
	void ChangeBodyType(int nLoop, int nControllIdx);	// 体型変更
	void ChangeHandedness(int nLoop, int nControllIdx);	// 利き手変更
	void ReCreatePlayer(int i, CPlayer::EHandedness handedness, CPlayer::EBody body);	// プレイヤー再生成
	void Save();	// セーブ
	void Load();	// ロード

	//=============================
	// メンバ変数
	//=============================
	std::vector<SEntryDressup> m_vecDressupInfo;	// 着せ替え情報
};

#endif
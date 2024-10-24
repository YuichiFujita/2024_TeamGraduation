//============================================================
//
//	キャラクターアニメーションヘッダー [characterAnim.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CHARACTER_ANIM_H_
#define _CHARACTER_ANIM_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectCharaAnim.h"

//************************************************************
//	クラス定義
//************************************************************
// キャラクターアニメーションクラス
class CCharacterAnim
{
public:
	// コンストラクタ
	CCharacterAnim();

	// デストラクタ
	~CCharacterAnim();

	// モーション情報型エイリアス定義
	using AMotion = CObjectCharaAnim::SInfo;

	// メンバ関数
	HRESULT Init();		// キャラクター初期化
	void Uninit();		// キャラクター終了
	HRESULT LoadAll();	// キャラクター全読込
	AMotion Regist(const char* pCharaPath);	// キャラクター登録

	// 静的メンバ関数
	static CCharacterAnim* Create();	// 生成
	static void Release(CCharacterAnim*& prCharacterAnim);	// 破棄

private:
	// メンバ関数
	HRESULT SearchFolderAll(std::string sFolderPath);	// フォルダ全検索

	// メンバ関数
	HRESULT LoadSetup(AMotion* pInfoChara, const char* pCharaPath);			// キャラクター情報セットアップ
	HRESULT LoadMotionSetup(AMotion* pInfoChara, const char* pMotionPath);	// モーション情報セットアップ

	// メンバ変数
	std::map<std::string, AMotion> m_mapCharacter;	// キャラクターアニメーション連想配列
};

#endif	// _CHARACTER_ANIM_H_

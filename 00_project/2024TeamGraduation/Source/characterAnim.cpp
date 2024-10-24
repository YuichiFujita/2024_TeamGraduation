#if 0
//============================================================
//
//	キャラクターアニメーション処理 [characterAnim.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "characterAnim.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* LOAD_FOLDER = "data\\CHARACTER";	// キャラクターフォルダ相対パス
}

//************************************************************
//	親クラス [CCharacterAnim] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCharacterAnim::CCharacterAnim()
{
	// キャラクター連想配列をクリア
	m_mapCharacter.clear();
}

//============================================================
//	デストラクタ
//============================================================
CCharacterAnim::~CCharacterAnim()
{

}

//============================================================
//	キャラクター初期化処理
//============================================================
HRESULT CCharacterAnim::Init()
{
	// キャラクター連想配列を初期化
	m_mapCharacter.clear();

	// 成功を返す
	return S_OK;
}

//============================================================
//	キャラクター終了処理
//============================================================
void CCharacterAnim::Uninit()
{
	for (auto& rMap : m_mapCharacter)
	{ // キャラクター数分繰り返す

		// モーション情報をクリア
		rMap.second.vecMotion.clear();
	}

	// キャラクター連想配列をクリア
	m_mapCharacter.clear();
}

//============================================================
//	キャラクター全読込処理
//============================================================
HRESULT CCharacterAnim::LoadAll()
{
	// キャラクターの全読込
	if (FAILED(SearchFolderAll(LOAD_FOLDER)))
	{ // 読込に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	キャラクター登録
//============================================================
CCharacterAnim::AMotion CCharacterAnim::Regist(const char* pCharaPath)
{
	// 既に生成済みかを検索
	auto itr = m_mapCharacter.find(pCharaPath);	// 引数のキャラクター情報を検索
	if (itr != m_mapCharacter.end())
	{ // 生成済みの場合

		// 読込済みのキャラクター情報を返す
		return itr->second;
	}

	// キャラクター情報を読込
	AMotion tempCharaData;	// キャラクター情報
	if (FAILED(LoadSetup(&tempCharaData, pCharaPath)))
	{ // 読込に失敗した場合

		// 初期値を返す
		assert(false);
		return {};
	}

	// キャラクター情報を保存
	m_mapCharacter.insert(std::make_pair(pCharaPath, tempCharaData));

	// 生成したキャラクター情報を返す
	return tempCharaData;
}

//============================================================
//	生成処理
//============================================================
CCharacterAnim* CCharacterAnim::Create()
{
	// キャラクターアニメーションの生成
	CCharacterAnim* pCharacter = new CCharacterAnim;
	if (pCharacter == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// キャラクターアニメーションの初期化
		if (FAILED(pCharacter->Init()))
		{ // キャラクターアニメーション初期化に失敗した場合

			// キャラクターアニメーションの破棄
			SAFE_DELETE(pCharacter);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pCharacter;
	}
}

//============================================================
//	破棄処理
//============================================================
void CCharacterAnim::Release(CCharacterAnim*& prCharacter)
{
	// キャラクターアニメーションの終了
	assert(prCharacter != nullptr);
	prCharacter->Uninit();

	// メモリ開放
	SAFE_DELETE(prCharacter);
}

//============================================================
//	フォルダ全検索処理
//============================================================
HRESULT CCharacterAnim::SearchFolderAll(std::string sFolderPath)
{
	HANDLE hFile;	// 検索ハンドル
	WIN32_FIND_DATA findFileData;	// ファイル情報

	// 引数パスのディレクトリを取得
	std::string sAllLoadPath = sFolderPath + "\\*.*";	// 全読込パス
	hFile = FindFirstFile(sAllLoadPath.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE == hFile)
	{ // ハンドルが無効の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	do
	{ // ファイル内の情報全てを読み込む

		// 現在のディレクトリ、親ディレクトリの場合次のループに移行
		if (strcmp(findFileData.cFileName, ".") == 0)	{ continue; }
		if (strcmp(findFileData.cFileName, "..") == 0)	{ continue; }

		// ファイル名を相対パスに変換
		std::string sFullPath = sFolderPath;	// 現在の相対パスを設定
		sFullPath += "\\";						// パス区切り文字を追加
		sFullPath += findFileData.cFileName;	// ファイル名を追加

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{ // ディレクトリだった場合

			// 新たなディレクトリを全検索
			SearchFolderAll(sFullPath);
		}
		else
		{ // ファイルだった場合

			// テクスチャを登録
			Regist(sFullPath.c_str());
		}

	} while (FindNextFile(hFile, &findFileData));	// 次のファイルを検索

	// 検索ハンドルを閉じる
	FindClose(hFile);

	// 成功を返す
	return S_OK;
}

//============================================================
//	キャラクター情報セットアップ処理
//============================================================
HRESULT CCharacterAnim::LoadSetup(AMotion* pInfoChara, const char* pCharaPath)
{
	// ファイルを開く
	std::ifstream file(pCharaPath);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "キャラクターセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "MOTIONPASS")
		{
			file >> str;	// ＝を読込
			file >> str;	// モーションパスを読込

			// モーションパスを読み込む
			LoadMotionSetup(pInfoChara, str.c_str());
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
//	モーション情報セットアップ処理
//============================================================
HRESULT CCharacterAnim::LoadMotionSetup(AMotion* pInfoChara, const char* pMotionPath)
{
	// ファイルを開く
	std::ifstream file(pMotionPath);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "モーションセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 最後尾にモーション情報を追加
	int nCurMotionIdx = pInfoChara->GetNumMotion();	// 現在の最後尾インデックス
	pInfoChara->vecMotion.emplace_back();			// 空の要素を最後尾に追加

	// 変数をポインタ化し簡略化
	CObjectCharaAnim::SMotion* pMotion = &pInfoChara->vecMotion[nCurMotionIdx];			// モーション情報
	CObjectCharaAnim::SChara* pChara = &pInfoChara->vecMotion[nCurMotionIdx].infoChara;	// キャラクター情報
	CObjectCharaAnim::SColl* pColl = &pInfoChara->vecMotion[nCurMotionIdx].infoColl;		// 当たり判定情報

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(file, str);
		}
		else if (str == "TEXTURE_PASS")
		{
			file >> str;					// ＝を読込
			file >> pChara->sPathTexture;	// テクスチャパスを読込
		}
		else if (str == "TEX_PATTERN")
		{
			MyLib::PosGrid2 ptrnTexture;	// テクスチャ分割数
			file >> str;			// ＝を読込
			file >> ptrnTexture.x;	// テクスチャ横分割を読込
			file >> ptrnTexture.y;	// テクスチャ縦分割を読込

			// テクスチャ分割数・パターン総数を設定
			if (FAILED(pChara->SetTexPtrn(ptrnTexture)))
			{ // 設定に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
		else if (str == "OFFSET_ORIGIN")
		{
			file >> str;				// ＝を読込
			file >> pChara->offset.x;	// 原点オフセットXを読込
			file >> pChara->offset.y;	// 原点オフセットYを読込
			file >> pChara->offset.z;	// 原点オフセットZを読込
		}
		else if (str == "CHARA_SIZE")
		{
			file >> str;			// ＝を読込
			file >> pChara->size.x;	// キャラクター大きさXを読込
			file >> pChara->size.y;	// キャラクター大きさYを読込
			file >> pChara->size.z;	// キャラクター大きさZを読込
		}
		else if (str == "NEXT_TIME")
		{
			float fNextTime = 0.0f;	// パターン変更時間
			file >> str;			// ＝を読込
			file >> fNextTime;		// パターン変更時間を読込

			// パターン変更時間を設定
			if (FAILED(pChara->SetNextTime(fNextTime)))
			{ // 設定に失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
		else if (str == "LOOP")
		{
			int nCastBool = 0;	// bool型変換
			file >> str;		// ＝を読込
			file >> nCastBool;	// ループのON/OFFを読込

			// 読み込んだ値をbool型に変換
			pChara->bLoop = (nCastBool != 0);
		}
		else if (str == "CANCEL_TIME")
		{
			file >> str;					// ＝を読込
			file >> pMotion->fCancelTime;	// キャンセル可能時間を読込
		}
		else if (str == "COMBO_TIME")
		{
			file >> str;					// ＝を読込
			file >> pMotion->fComboTime;	// コンボ可能時間を読込
		}
		else if (str == "OFFSET_COLL")
		{
			file >> str;				// ＝を読込
			file >> pColl->offset.x;	// 判定原点オフセットXを読込
			file >> pColl->offset.y;	// 判定原点オフセットYを読込
			file >> pColl->offset.z;	// 判定原点オフセットZを読込
		}
		else if (str == "COLL_SIZE")
		{
			file >> str;			// ＝を読込
			file >> pColl->size.x;	// 判定大きさXを読込
			file >> pColl->size.y;	// 判定大きさYを読込
			file >> pColl->size.z;	// 判定大きさZを読込
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
#endif
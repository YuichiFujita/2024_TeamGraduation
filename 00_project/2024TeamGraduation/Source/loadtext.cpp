//============================================================
//
//	テキスト読込関数処理 [loadtext.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "loadtext.h"
#include "text2D.h"
#include "string2D.h"

//============================================================
//	文字列配列の読込処理
//============================================================
void loadtext::LoadVector(std::ifstream* pFile, std::vector<std::wstring>* pVecStr)
{
	// 動的配列ポインタがない場合抜ける
	if (pVecStr == nullptr) { assert(false); return; }

	// ファイルポインタがない場合抜ける
	if (pFile == nullptr)	{ assert(false); return; }

	// 開けてないファイルの場合抜ける
	if (!pFile->is_open())	{ assert(false); return; }

	std::string str;	// 読込文字列
	do { // END_TEXTを読み込むまでループ

		// 文字列を読み込む
		*pFile >> str;

		if (str.front() == '#')
		{ // コメントアウトされている場合

			// 一行全て読み込む
			std::getline(*pFile, str);
		}
		else if (str == "STR")
		{
			*pFile >> str;					// ＝を読込
			pFile->seekg(1, std::ios::cur);	// 読込位置を空白分ずらす
			std::getline(*pFile, str);		// 一行全て読み込む

			// 文字列を最後尾に追加
			pVecStr->push_back(useful::MultiByteToWide(str));	// ワイド文字列に変換
		}

	} while (str != "END_TEXT");	// END_TEXTを読み込むまでループ
}

//============================================================
//	テキストの読込処理 (文字列)
//============================================================
std::vector<std::wstring> loadtext::LoadText(const char* pFilePath, const char* pTextStr)
{
	std::vector<std::wstring> vecStr;	// 文字列の動的配列
	std::string sLoadStart;				// 読込開始の文字列

	// 読込の開始文字列を作成
	sLoadStart = "TEXT_";			// 認識用の先頭文字を設定
	sLoadStart.append(pTextStr);	// 引数の認識用文字列と連結

	// ファイルを開く
	std::ifstream file(pFilePath);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "テキストファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 初期値を返す
		return vecStr;
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
		else if (str == "TEXT_ELSE")
		{ // 汎用的な読込開始の文字列と一致した場合

			// 文字列の動的配列を読込
			LoadVector(&file, &vecStr);
		}
		else if (str == sLoadStart)
		{ // 読込開始の文字列と一致した場合

			// 文字列の動的配列を読込
			LoadVector(&file, &vecStr);

			break;	// ELSEを読み込まないよう繰り返しから抜ける
		}
	}

	// ファイルを閉じる
	file.close();

	// 文字列の動的配列を返す
	return vecStr;
}

//============================================================
//	テキストの読込処理 (インデックス)
//============================================================
std::vector<std::wstring> loadtext::LoadText(const char* pFilePath, const int nTextIdx)
{
	// テキストの読込
	return LoadText(pFilePath, std::to_string(nTextIdx).c_str());
}

//============================================================
//	テキストの割当処理 (マルチバイト文字列)
//============================================================
void loadtext::BindText(CText2D* pText2D, const std::vector<std::string>& rVecStr)
{
	for (int i = 0; i < (int)rVecStr.size(); i++)
	{ // 文字列数分繰り返す

		// 文字列を最後尾に追加
		pText2D->PushBackString(rVecStr[i]);
	}
}

//============================================================
//	テキストの割当処理 (ワイド文字列)
//============================================================
void loadtext::BindText(CText2D* pText2D, const std::vector<std::wstring>& rVecStr)
{
	for (int i = 0; i < (int)rVecStr.size(); i++)
	{ // 文字列数分繰り返す

		// 文字列を最後尾に追加
		pText2D->PushBackString(rVecStr[i]);
	}
}

//============================================================
//	文字列の割当処理 (マルチバイト文字列)
//============================================================
void loadtext::BindString(CString2D* pString2D, const std::vector<std::string>& rVecStr)
{
	// 文字列が一つではない場合エラー
	assert((int)rVecStr.size() == 1);

	// 文字列を設定
	pString2D->SetString(rVecStr[0]);
}

//============================================================
//	文字列の割当処理 (ワイド文字列)
//============================================================
void loadtext::BindString(CString2D* pString2D, const std::vector<std::wstring>& rVecStr)
{
	// 文字列が一つではない場合エラー
	assert((int)rVecStr.size() == 1);

	// 文字列を設定
	pString2D->SetString(rVecStr[0]);
}

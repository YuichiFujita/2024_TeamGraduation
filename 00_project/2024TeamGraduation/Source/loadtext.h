//============================================================
//
//	テキスト読込関数ヘッダー [loadtext.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LOAD_TEXT_H_
#define _LOAD_TEXT_H_

//************************************************************
//	前方宣言
//************************************************************
class CText2D;		// テキスト2Dクラス
class CString2D;	// 文字列2Dクラス

//************************************************************
//	名前空間宣言
//************************************************************
// テキスト読込空間
namespace loadtext
{
	void LoadVector(std::ifstream* pFile, std::vector<std::wstring>* pVecStr);			// 文字列配列の読込
	std::vector<std::wstring> LoadText(const char* pFilePath, const char* pTextStr);	// テキスト読込 (文字列)
	std::vector<std::wstring> LoadText(const char* pFilePath, const int nTextIdx);		// テキスト読込 (インデックス)
	void BindText(CText2D* pText2D, const std::vector<std::string>& rVecStr);			// テキスト割当 (マルチバイト文字列)
	void BindText(CText2D* pText2D, const std::vector<std::wstring>& rVecStr);			// テキスト割当 (ワイド文字列)
	void BindString(CString2D* pString2D, const std::vector<std::string>& rVecStr);		// 文字列割当 (マルチバイト文字列)
	void BindString(CString2D* pString2D, const std::vector<std::wstring>& rVecStr);	// 文字列割当 (ワイド文字列)
}

#endif	// _LOAD_TEXT_H_

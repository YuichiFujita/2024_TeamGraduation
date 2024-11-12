//=============================================================================
// 
//  Xファイルヘッダー [Xload.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _XLOAD_H_
#define _XLOAD_H_	// 二重インクルード防止

#include "object.h"

class CObjectX;

//==========================================================================
// クラス定義
//==========================================================================
// Xファイルクラス定義
class CXLoad
{
public:

	struct SXFile
	{
		MyLib::Vector3 vtxMin;					// モデルの最小値
		MyLib::Vector3	vtxMax;					// モデルの最大値
		BYTE *pVtxBuff;							// 頂点バッファのポインタ
		LPD3DXMESH pMesh;						// メッシュ(頂点情報)へのポインタ
		LPD3DXBUFFER pBuffMat;					// マテリアルへのポインタ
		DWORD dwNumMat;							// マテリアルの数
		std::vector<D3DXMATERIAL> pMatData;		// マテリアルのデータ
		int nVtxNum;							// 頂点数
		int nFaceNum;							// 面の数
		MyLib::Vector3 *pVtxPos;				// 頂点座標
		float fMaxVtxDistance;					// 頂点間の最大距離
		int *nIdxTexture;						// テクスチャのインデックス番号
		std::string filename;					// ファイル名
		int nFileNameLen;						// ファイル名の文字数
	};

	CXLoad();
	~CXLoad();

	HRESULT Init();
	void Uninit();
	int XLoad(const std::string& file);
	HRESULT LoadAll();	// 全てのモデル読み込み
	void Unload();

	SXFile *GetMyObject(int nIdx);
	int GetNumAll();		// 総数

	static CXLoad* Create();
	static CXLoad* GetInstance() { return m_pThisPtr; }
protected:

private:
	HRESULT Load(const std::string& file);
	void SearchAllXModel(const std::wstring& folderPath);	// 全検索

	std::vector<SXFile> m_XFileInfo;	// Xファイルの情報
	std::vector<std::string> m_ImageNames;		// 読み込み用文字列
	std::vector<std::string> m_FolderFilePath;	// フォルダー格納のファイルパス
	static CXLoad* m_pThisPtr;	// 自身のポインタ
};



#endif
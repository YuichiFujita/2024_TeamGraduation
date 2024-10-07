//=============================================================================
// 
//  マップ処理 [map.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "map.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "objectX.h"
#include "object3DMesh.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "shadow.h"

using namespace std;	// 名前空間を指定

namespace mapdate
{
	// 構造体定義
	struct SMap
	{
		int nType;						// 種類
		MyLib::Vector3 pos;				// 位置
		MyLib::Vector3 rot;				// 向き
		int nWidth, nHeight;			// 分割数
		float fWidthLen, fHeightLen;	// サイズ
		float fMove;					// 移動量
		int nShadow;					// 影使うかどうか
	};

	int nNumModelAll = 0;	// モデルの総数
	int nNumTexAll = 0;		// テクスチャの総数
	int nNumObjXAll = 0;		// オブジェクトXの総数
	int nNumObj3DMeshAll = 0;		// オブジェクト3Dメッシュの総数
	CObjectX *pObjX[mylib_const::MAX_OBJ];		// オブジェクトX
	CObject3DMesh *pObj3DMesh[mylib_const::MAX_OBJ];		// オブジェクト3Dメッシュ
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
vector<std::string> ModelFile;		// モデルファイル名
vector<std::string> TextureFile;	// テクスチャファイル名
vector<int> ModelIdx;	// モデルインデックス
vector<int> TextureIdx;	// テクスチャインデックス
mapdate::SMap g_Map;

//==========================================================================
// 生成処理
//==========================================================================
HRESULT MyMap::Create(const char *pTextFile)
{
	// 総数
	mapdate::nNumModelAll = 0;
	mapdate::nNumObjXAll = 0;
	mapdate::nNumObj3DMeshAll = 0;

	memset(mapdate::pObjX, 0, sizeof(mapdate::pObjX));
	memset(mapdate::pObj3DMesh, 0, sizeof(mapdate::pObj3DMesh));	// オブジェクト3Dメッシュ
	ModelIdx.clear();
	ModelFile.clear();		// モデルファイル名

	// 外部テキスト読み込み処理
	HRESULT hr = ReadText(pTextFile);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 解放処理
//==========================================================================
void MyMap::Release()
{
	for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
	{
		if (mapdate::pObjX[nCntObj] != nullptr)
		{
			mapdate::pObjX[nCntObj]->Kill();
			mapdate::pObjX[nCntObj] = nullptr;
		}

		if (mapdate::pObj3DMesh[nCntObj] != nullptr)
		{
			mapdate::pObj3DMesh[nCntObj]->Uninit();
			mapdate::pObj3DMesh[nCntObj] = nullptr;
		}
	}

	ModelFile.clear();		// モデルファイル名
	ModelIdx.clear();
	mapdate::nNumObjXAll = 0;
}

//==========================================================================
// マップ切り替え
//==========================================================================
void MyMap::ChangeMap(const char* pTextFile)
{
	// 開放処理
	Release();

	// 外部テキスト読み込み処理
	ReadText(pTextFile);
}

//==========================================================================
// 外部ファイル書き込み処理
//==========================================================================
HRESULT MyMap::SaveText()
{
#if TOPCUR
	FILE *pFile = nullptr;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen("data\\TEXT\\map\\save_info.txt", "w");

	if (pFile == nullptr)
	{// ファイルが開けなかった場合
		return E_FAIL;
	}

	// ファイルに書き出す
	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# テクスチャ数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_TEXTURE = %d\n\n", mapdate::nNumTexAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# テクスチャファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// テクスチャファイル名分
	for (int nCntFileNum = 0; nCntFileNum < mapdate::nNumTexAll; nCntFileNum++)
	{
		fprintf(pFile, "TEXTURE_FILENAME = %s\t\t# [%d]\n", &TextureFile[nCntFileNum][0], nCntFileNum);
	}

	// ファイルに書き出す
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# モデル数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", mapdate::nNumModelAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# モデルファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// モデルファイル名分
	for (int nCntFileNum = 0; nCntFileNum < mapdate::nNumModelAll; nCntFileNum++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &ModelFile[nCntFileNum][0], nCntFileNum);
	}

	// テクスチャ取得
	CTexture* pTexture = CTexture::GetInstance();

	//**********************************
	// 空配置
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# 空情報\n"
		"#==============================================================================\n");


	std::map<CObject::LAYER, std::map<int, std::vector<CObject*>>> objects = CObject::GetTop();
	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 種類の取得
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHDOME)
				{// メッシュドームだったら

					// Xファイルの情報取得
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類

					// テクスチャ名
					vector<string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					// 出力
					fprintf(pFile,
						"SKYSET\n"
						"\tTEXTYPE = %d\n"
						"\tMOVE = %f\n"
						"END_SKYSET\n\n",
						nType, g_Map.fMove);
				}
			}
		}
	}

	
	//**********************************
	// 山配置
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# 山情報\n"
		"#==============================================================================\n");

	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 種類の取得
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHCYLINDER)
				{// メッシュシリンダーだったら

					// Xファイルの情報取得
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類

					// テクスチャ名
					vector<string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					// 出力
					fprintf(pFile,
						"MOUNTAINSET\n"
						"\tTEXTYPE = %d\n"
						"END_MOUNTAINSET\n\n", nType);
				}
			}
		}
	}


	//**********************************
	// 地面配置
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# 地面情報\n"
		"#==============================================================================\n");


	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 種類の取得
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHFIELD)
				{// メッシュフィールドだったら

					// Xファイルの情報取得
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類

					// テクスチャ名
					vector<string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					MyLib::Vector3 pos = pObjMesh->GetPosition();		// 位置
					MyLib::Vector3 rot = pObjMesh->GetRotation();		// 向き
					int nWidth = pObjMesh->GetWidthBlock();			// 横分割数
					int nHeight = pObjMesh->GetHeightBlock();		// 縦分割数
					float fWidthLen = pObjMesh->GetWidthLen();		// 横長さ
					float fHeightLen = pObjMesh->GetHeightLen();	// 縦長さ

					// 出力
					fprintf(pFile,
						"FIELDSET\n"
						"\tTEXTYPE = %d\n"
						"\tPOS = %.0f %.0f %.0f\n"
						"\tROT = %.0f %.0f %.0f\n"
						"\tBLOCK = %d %d\n"
						"\tSIZE = %.0f %.0f\n"
						"END_FIELDSET\n\n",
						nType, pos.x, pos.y, pos.z,
						rot.x, rot.y, rot.z,
						nWidth, nHeight,
						fWidthLen, fHeightLen);
				}
			}
		}
	}
	

	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# モデルの配置\n"
		"#==============================================================================\n");

	int nnnn = 0;
	for (int i = 0; i < mylib_const::MAX_OBJ; i++)
	{
		// Xファイルの情報取得
		CObjectX* pObjX = mapdate::pObjX[i];

		if (pObjX == nullptr) {
			continue;
		}

		int nType = ModelIdx[nnnn];		// 種類
		nnnn++;
		MyLib::Vector3 pos = pObjX->GetPosition();	// 位置
		MyLib::Vector3 rot = pObjX->GetRotation();	// 向き
		int nShadow = 0;						// 影使うかどうか

		if (pObjX->GetUseShadow())
		{// 使っている場合
			nShadow = 1;
		}

		// 出力
		fprintf(pFile,
			"MODELSET\n"
			"\tTYPE = %d\n"
			"\tPOS = %.2f %.2f %.2f\n"
			"\tROT = %.2f %.2f %.2f\n"
			"\tSHADOW = %d\n"
			"END_MODELSET\n\n",
			nType, pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z, nShadow);
	}

	

	fprintf(pFile, "\nEND_SCRIPT		# この行は絶対消さないこと！");

	//ファイルを閉じる
	fclose(pFile);
#endif
	return S_OK;
}

//==========================================================================
// テクスチャ読み込み処理
//==========================================================================
HRESULT MyMap::ReadTexture()
{
	char aComment[MAX_COMMENT] = {};	// コメント用
	int nFileNum = 0;					// ファイルの数
	int nCntTexture = 0;				// テクスチャ読み込みカウント

	// ファイルポインタ
	FILE *pFile = nullptr;

	//ファイルを開く
	pFile = fopen("data\\TEXT\\map\\info.txt", "r");

	// 後でつかう
	/*switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_TUTORIAL:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_GAME:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RESULT:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	case CScene::MODE_RANKING:
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
		break;

	default:
		return E_FAIL;
		break;
	}*/

	if (pFile == nullptr)
	{//ファイルが開けた場合
		return E_FAIL;
	}

	TextureIdx.clear();

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// テクスチャ数の設定
		if (strcmp(&aComment[0], "NUM_TEXTURE") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &mapdate::nNumTexAll);	// テクスチャ数
		}

		while (nCntTexture != mapdate::nNumTexAll)
		{// テクスチャの数分読み込むまで繰り返し

			fscanf(pFile, "%s", &aComment[0]);	// =の分

			if (strcmp(&aComment[0], "TEXTURE_FILENAME") == 0)
			{// TEXTURE_FILENAMEがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 最後尾に追加
				TextureFile.emplace_back();
				TextureFile.back() = &aComment[0];

				TextureFile.back() = UtilFunc::Transformation::ReplaceBackslash(TextureFile.back());
				TextureFile.back() = UtilFunc::Transformation::ReplaceForwardSlashes(TextureFile.back());

				// テクスチャの割り当て
				CTexture::GetInstance()->Regist(&TextureFile[nCntTexture][0]);

				// テクスチャ情報追加
				TextureIdx.emplace_back();
				TextureIdx.back()= nCntTexture;

				nCntTexture++;	// テクスチャ数加算
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける

			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// モデル読み込み処理
//==========================================================================
HRESULT MyMap::ReadXFile(const char *pTextFile)
{
	// リセット
	mapdate::nNumModelAll = 0;


	char aComment[MAX_COMMENT] = {};	// コメント用
	int nFileNum = 0;					// ファイルの数
	int nCntTexture = 0;				// テクスチャ読み込みカウント

	// ファイルポインタ
	FILE *pFile = nullptr;

	//ファイルを開く
	pFile = fopen(pTextFile, "r");

	if (pFile == nullptr)
	{//ファイルが開けた場合
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル数の設定
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &nFileNum);	// モデル数
		}

		while (mapdate::nNumModelAll != nFileNum)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 最後尾に追加
				ModelFile.push_back(&aComment[0]);

				// Xファイルのデータ取得
				CXLoad *pXLoad = CXLoad::GetInstance();

				// ファイル読み込み
				pXLoad->XLoad(&ModelFile[mapdate::nNumModelAll][0]);

				mapdate::nNumModelAll++;	// モデル数加算
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける

			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// 外部テキスト読み込み処理
//==========================================================================
HRESULT MyMap::ReadText(const char *pTextFile)
{
	std::string FileName;	// ファイル名
	char aComment[MAX_COMMENT] = {};	//コメント用
	int nFileNum = 0;					// ファイルの数
	int nCntTexture = 0;				// テクスチャ読み込みカウント

	// ファイルポインタ
	FILE *pFile = nullptr;

	//ファイルを開く
	switch (CManager::GetInstance()->GetMode())
	{
	case CScene::MODE_TITLE:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

	case CScene::MODE_TUTORIAL:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

	case CScene::MODE_GAME:
	case CScene::MODE::MODE_GAMETUTORIAL:
		FileName = pTextFile;
		break;

	case CScene::MODE_RESULT:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

		//メモ：仮修正、本来のinfo_ranking.txtだと落ちる
	case CScene::MODE_RANKING:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;

	default:
		FileName = "data\\TEXT\\map\\info_title.txt";
		break;
	}

	// マップファイルを開く
	pFile = fopen(FileName.c_str(), "r");

	if (pFile == nullptr)
	{//ファイルが開けた場合
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

#if 1
		// メッシュフィールドの設定
		if (strcmp(&aComment[0], "FIELDSET") == 0)
		{// モデルの読み込みを開始

			while (strcmp(&aComment[0], "END_FIELDSET"))
			{// END_FIELDSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPEが来たら種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &g_Map.nType);		// モデル種類の列挙
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POSが来たら位置読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &g_Map.pos.x);		// X座標
					fscanf(pFile, "%f", &g_Map.pos.y);		// Y座標
					fscanf(pFile, "%f", &g_Map.pos.z);		// Z座標
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROTが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);		 // =の分
					fscanf(pFile, "%f", &g_Map.rot.x);		 // Xの向き
					fscanf(pFile, "%f", &g_Map.rot.y);		 // Yの向き
					fscanf(pFile, "%f", &g_Map.rot.z);		 // Zの向き
					g_Map.rot.x = D3DXToRadian(g_Map.rot.x); // 360度形式から変換
					g_Map.rot.y = D3DXToRadian(g_Map.rot.y); // 360度形式から変換
					g_Map.rot.z = D3DXToRadian(g_Map.rot.z); // 360度形式から変換
				}

				if (strcmp(&aComment[0], "BLOCK") == 0)
				{//BLOCKが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);		// =の分
					fscanf(pFile, "%d", &g_Map.nWidth);		// 横の分割数
					fscanf(pFile, "%d", &g_Map.nHeight);	// 縦の分割数
				}

				if (strcmp(&aComment[0], "SIZE") == 0)
				{//SIZEが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);		// =の分
					fscanf(pFile, "%f", &g_Map.fWidthLen);	// 横の長さ
					fscanf(pFile, "%f", &g_Map.fHeightLen);	// 縦の長さ
				}

			}// END_FIELDSETのかっこ

			 //**********************************
			 // 生成処理
			 //**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshField::Create(
				g_Map.pos, g_Map.rot,
				g_Map.fWidthLen, g_Map.fHeightLen,
				g_Map.nWidth, g_Map.nHeight,
				(CMeshField::TYPE)g_Map.nType, &TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == nullptr)
			{// nullptrだったら
				return E_FAIL;
			}

			// オブジェクトメッシュの総数加算
			mapdate::nNumObj3DMeshAll++;
		}

#endif
		// メッシュシリンダーの設定
		if (strcmp(&aComment[0], "MOUNTAINSET") == 0)
		{// モデルの読み込みを開始

			while (strcmp(&aComment[0], "END_MOUNTAINSET"))
			{// END_MOUNTAINSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPEが来たら種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &g_Map.nType);	// モデル種類の列挙
				}

			}// END_MOUNTAINSETのかっこ

			 //**********************************
			 // 生成処理
			 //**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshCylinder::Create(&TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == nullptr)
			{// nullptrだったら
				return E_FAIL;
			}

			// オブジェクトメッシュの総数加算
			mapdate::nNumObj3DMeshAll++;
		}

		// メッシュドームの設定
		if (strcmp(&aComment[0], "SKYSET") == 0)
		{// モデルの読み込みを開始

			while (strcmp(&aComment[0], "END_SKYSET"))
			{// END_MOUNTAINSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(&aComment[0], "TEXTYPE") == 0)
				{// TEXTYPEが来たら種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &g_Map.nType);		// モデル種類の列挙
				}

				if (strcmp(&aComment[0], "MOVE") == 0)
				{//MOVEが来たら移動読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &g_Map.fMove);	// 移動量
				}

			}// END_SKYSETのかっこ

			//**********************************
			// 生成処理
			//**********************************
			mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] = CMeshDome::Create(g_Map.fMove, &TextureFile[g_Map.nType][0]);

			if (mapdate::pObj3DMesh[mapdate::nNumObj3DMeshAll] == nullptr)
			{// nullptrだったら
				return E_FAIL;
			}

			// オブジェクトメッシュの総数加算
			mapdate::nNumObj3DMeshAll++;
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける

			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

void MyMap::Regist(int nType, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow)
{
	ModelIdx.push_back(nType);

	// タイプの物を生成
	mapdate::pObjX[mapdate::nNumObjXAll] = CObjectX::Create(ModelFile[nType].c_str(), pos, rot, bShadow);
	mapdate::pObjX[mapdate::nNumObjXAll]->SetType(CObject::TYPE_XFILE);
	mapdate::nNumObjXAll++;
}

void MyMap::Delete(CObjectX* obj)
{
	for (int i = 0; i < mylib_const::MAX_OBJ; i++)
	{
		if (mapdate::pObjX[i] == obj)
		{
			mapdate::pObjX[i]->Kill();
			mapdate::pObjX[i] = nullptr;

			mapdate::nNumObjXAll--;
			break;
		}
		
	}
}

std::string MyMap::GetModelFileName(int nIdx)
{
	return ModelFile[nIdx];
}

std::vector<std::string> MyMap::GetModelFileNameAll()
{
	return ModelFile;
}

std::vector<std::string> MyMap::GetTextureFileNameAll()
{
	return TextureFile;
}

//==========================================================================
// 総数取得
//==========================================================================
int MyMap::GetNumModelAll()
{
	return mapdate::nNumModelAll;
}

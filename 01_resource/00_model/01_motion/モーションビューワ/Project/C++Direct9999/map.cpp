//=============================================================================
// 
//  マップ処理 [MyMap.cpp]
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

namespace MyMapdate
{
	// 構造体定義
	struct SMap
	{
		int nType;						// 種類
		D3DXVECTOR3 pos;				// 位置
		D3DXVECTOR3 rot;				// 向き
		int nWidth, nHeight;			// 分割数
		float fWidthLen, fHeightLen;	// サイズ
		float fMove;					// 移動量
		int nShadow;					// 影使うかどうか
	};

	int nNumModelAll = 0;	// モデルの総数
	int nNumTexAll = 0;		// テクスチャの総数
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
vector<std::string> ModelFile;		// モデルファイル名
vector<std::string> TextureFile;	// テクスチャファイル名
MyMapdate::SMap g_Map;

//==========================================================================
// 生成処理
//==========================================================================
HRESULT MyMap::Create(void)
{
	// 総数
	MyMapdate::nNumModelAll = 0;

	// 外部テキスト読み込み処理
	HRESULT hr = ReadText();

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 外部ファイル書き込み処理
//==========================================================================
HRESULT MyMap::SaveText(void)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen("data\\TEXT\\edit_info.txt", "w");

	if (pFile == NULL)
	{// ファイルが開けなかった場合
		return E_FAIL;
	}

	// ファイルに書き出す
	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# テクスチャ数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_TEXTURE = %d\n\n", MyMapdate::nNumTexAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# テクスチャファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// テクスチャファイル名分
	for (int nCntFileNum = 0; nCntFileNum < MyMapdate::nNumTexAll; nCntFileNum++)
	{
		fprintf(pFile, "TEXTURE_FILENAME = %s\t\t# [%d]\n", &TextureFile[nCntFileNum][0], nCntFileNum);
	}

	// ファイルに書き出す
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# モデル数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", MyMapdate::nNumModelAll);

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# モデルファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// モデルファイル名分
	for (int nCntFileNum = 0; nCntFileNum < MyMapdate::nNumModelAll; nCntFileNum++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &ModelFile[nCntFileNum][0], nCntFileNum);
	}


	//**********************************
	// 空配置
	//**********************************
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# 空情報\n"
		"#==============================================================================\n");

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// オブジェクトの情報取得
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// 情報が入っていたら

				// 種類の取得
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_MESHDOME)
				{// メッシュドームだったら

					// Xファイルの情報取得
					CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類
					nType--;	// NULL分減らす

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

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// オブジェクトの情報取得
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// 情報が入っていたら

				// 種類の取得
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_MESHCYLINDER)
				{// メッシュシリンダーだったら

					// メッシュの情報取得
					CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類
					nType--;	// NULL分減らす

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

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// オブジェクトの情報取得
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// 情報が入っていたら

				// 種類の取得
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_MESHFIELD)
				{// メッシュフィールドだったら

					// メッシュの情報取得
					CObject3DMesh *pObjMesh = pObj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類
					nType--;	// NULL分減らす
					D3DXVECTOR3 pos = pObjMesh->GetPosition();		// 位置
					D3DXVECTOR3 rot = pObjMesh->GetRotation();		// 向き
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

	for (int nCntPriority = 0; nCntPriority < mylib_const::PRIORITY_NUM; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < mylib_const::MAX_OBJ; nCntObj++)
		{
			// オブジェクトの情報取得
			CObject *pObj = CObject::GetObject(nCntPriority, nCntObj);

			if (pObj != NULL)
			{// 情報が入っていたら

				// 種類の取得
				CObject::TYPE TargetType = pObj->GetType();

				if (TargetType == CObject::TYPE_XFILE)
				{// Xファイルのモデルだったら

					// Xファイルの情報取得
					CObjectX *pObjX = pObj->GetObjectX();

					int nType = pObjX->GetIdxXFile();		// 種類
					D3DXVECTOR3 pos = pObjX->GetPosition();	// 位置
					D3DXVECTOR3 rot = pObjX->GetRotation();	// 向き
					int nShadow = 0;						// 影使うかどうか

					if (pObjX->GetUseShadow() == true)
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
			}
		}
	}

	fprintf(pFile, "END_SCRIPT		# この行は絶対消さないこと！");

	//ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// 外部テキスト読み込み処理
//==========================================================================
HRESULT MyMap::ReadText(void)
{
	char aComment[MAX_COMMENT] = {};	//コメント用
	int nFileNum = 0;					// ファイルの数
	int nCntTexture = 0;				// テクスチャ読み込みカウント

	// オブジェクトXのオブジェクト
	CObjectX *pObjectX = NULL;

	// メッシュオブジェクトのオブジェクト
	CObject3DMesh *pObjMesh = NULL;

	//ファイルを開く
	FILE *pFile = fopen("data\\TEXT\\map.txt", "r");

	if (pFile == NULL)
	{//ファイルが開けた場合
		return E_FAIL;
	}

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// テクスチャ数の設定
		if (strcmp(&aComment[0], "NUM_TEXTURE") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &MyMapdate::nNumTexAll);	// テクスチャ数
		}

		while (nCntTexture != MyMapdate::nNumTexAll)
		{// テクスチャの数分読み込むまで繰り返し

			fscanf(pFile, "%s", &aComment[0]);	// =の分

			if (strcmp(&aComment[0], "TEXTURE_FILENAME") == 0)
			{// TEXTURE_FILENAMEがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 最後尾に追加
				TextureFile.push_back(&aComment[0]);

				// テクスチャの割り当て
				CManager::GetTexture()->Regist(&TextureFile[nCntTexture][0]);

				nCntTexture++;	// テクスチャ数加算
			}
		}
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
			pObjMesh = CMeshField::Create(
				g_Map.pos, g_Map.rot,
				g_Map.fWidthLen, g_Map.fHeightLen,
				g_Map.nWidth, g_Map.nHeight,
				(CMeshField::TYPE)g_Map.nType, &TextureFile[g_Map.nType][0]);

			if (pObjMesh == NULL)
			{// NULLだったら
				return E_FAIL;
			}
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
			pObjMesh = CMeshCylinder::Create(&TextureFile[g_Map.nType][0]);

			if (pObjMesh == NULL)
			{// NULLだったら
				return E_FAIL;
			}
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
			pObjMesh = CMeshDome::Create(g_Map.fMove, &TextureFile[g_Map.nType][0]);

			if (pObjMesh == NULL)
			{// NULLだったら
				return E_FAIL;
			}
		}


		// モデル数の設定
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &nFileNum);	// モデル数
		}

		while (MyMapdate::nNumModelAll != nFileNum)
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
				CXLoad *pXLoad = CManager::GetXLoad();

				// ファイル読み込み
				pXLoad->XLoad(&ModelFile[MyMapdate::nNumModelAll][0]);

				MyMapdate::nNumModelAll++;	// モデル数加算
			}
		}

#if 0
		// モデルの設定
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{// モデルの読み込みを開始

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPEが来たら種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &g_Map.nType);	// モデル種類の列挙
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POSが来たら位置読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &g_Map.pos.x);	// X座標
					fscanf(pFile, "%f", &g_Map.pos.y);	// Y座標
					fscanf(pFile, "%f", &g_Map.pos.z);	// Z座標
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROTが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &g_Map.rot.x);	// Xの向き
					fscanf(pFile, "%f", &g_Map.rot.y);	// Yの向き
					fscanf(pFile, "%f", &g_Map.rot.z);	// Zの向き
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOWが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);		// =の分
					fscanf(pFile, "%d", &g_Map.nShadow);	// 影を使うかどうか
				}

			}// END_MODELSETのかっこ

			if (g_Map.nShadow == 1)
			{
				// タイプの物を生成
				pObjectX = pObjectX->Create(&ModelFile[g_Map.nType][0], g_Map.pos, g_Map.rot, true);
			}
			else
			{
				// タイプの物を生成
				pObjectX = pObjectX->Create(&ModelFile[g_Map.nType][0], g_Map.pos, g_Map.rot, false);
			}

			if (pObjectX == NULL)
			{// 失敗していたら
				return E_FAIL;
			}

			// 種類設定
			pObjectX->SetType(CObject::TYPE_XFILE);
		}
#endif
		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける

			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;;
}

//==========================================================================
// 総数取得
//==========================================================================
int MyMap::GetNumModelAll(void)
{
	return MyMapdate::nNumModelAll;
}
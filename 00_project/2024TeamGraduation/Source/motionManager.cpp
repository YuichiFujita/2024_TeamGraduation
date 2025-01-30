//=============================================================================
// 
//  モーションマネージャー処理 [motionManager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "motionManager.h"
#include "calculation.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CMotionManager* CMotionManager::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CMotionManager::CMotionManager()
{
	// メモリクリア
	m_LoadInfo.clear();
}

//==========================================================================
// デストラクタ
//==========================================================================
CMotionManager::~CMotionManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMotionManager* CMotionManager::Create()
{
	// 存在する場合既存のインスタンス返し
	if (m_pThisPtr != nullptr) return m_pThisPtr;

	// インスタンス生成
	m_pThisPtr = DEBUG_NEW CMotionManager;

	if (m_pThisPtr != nullptr)
	{
		// 初期化処理
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMotionManager::Init()
{
	// メモリクリア
	m_LoadInfo.clear();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMotionManager::Uninit()
{
	// メモリクリア
	m_LoadInfo.clear();

	// 自身のポインタ削除
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// リセット
//==========================================================================
void CMotionManager::Reset()
{
	// 終了
	Uninit();

	// 生成
	Create();
}

//==========================================================================
// 読み込み処理
//==========================================================================
CMotionManager::SLoadInfo CMotionManager::Load(const std::string& setupfile)
{
	//--------------------------
	// 要素チェック
	//--------------------------
	// 構造体の中の文字列を探す
	auto CheckFile = [&setupfile](const CMotionManager::SLoadInfo& info)
	{
		return info.sTextFile == setupfile;
	};
	std::vector<CMotionManager::SLoadInfo>::iterator itr = std::find_if(m_LoadInfo.begin(), m_LoadInfo.end(), CheckFile);

	if (itr != m_LoadInfo.end())
	{// ファイル名が一致

		return (*itr);
	}

	//--------------------------
	// ファイル読み込み
	//--------------------------
	// ファイルを開く
	std::ifstream File(setupfile);
	if (!File.is_open())
	{//ファイルが開けなかった場合
		MyAssert::CustomAssert(false, "ファイル開けてないよ！");
		return SLoadInfo();
	}

	// 読み込み用変数
	std::vector<std::string> motionFile;	// モーションファイル
	SLoadInfo loadInfo;

	// ファイル名保存
	loadInfo.sTextFile = setupfile;

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;

	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("DEFAULT_ID") != std::string::npos)
		{// デフォルトモーション読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			lineStream >>	// 題名
				hoge >>		// ゴミ
				hoge;		// ＝

			// 情報渡す
			int value;
			while (lineStream >> value)
			{
				loadInfo.vecDefaultIdx.push_back(value);
			}
		}

		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// モーションファイル名読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>		// ゴミ
				hoge >>		// ＝
				hoge;		// モーションファイル名

			// ファイル名保存
			motionFile.push_back(hoge);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();


	//--------------------------
	// モーション読み込み
	//--------------------------
	// 要素分繰り返し
	for (int i = 0; i < static_cast<int>(motionFile.size()); i++)
	{
		// モーション読み込み
		loadInfo.vecLoadData.push_back(LoadMotion(motionFile[i], i));
	}

	// 要素追加
	m_LoadInfo.push_back(loadInfo);

	return loadInfo;
}

//==========================================================================
// モーション読み込み
//==========================================================================
CMotionManager::Info CMotionManager::LoadMotion(const std::string& file, int nIdxMotion)
{
	// ファイルを開く
	std::ifstream File(file);
	if (!File.is_open())
	{//ファイルが開けなかった場合
		MyAssert::CustomAssert(false, "ファイル開けてないよ！");
		return Info();
	}

	// 読み込み用データ
	Info loadInfo;

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;

	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("MOTIONSET") != std::string::npos)
		{// モーション情報の読み込みを開始

			while (line.find("END_MOTIONSET") == std::string::npos)
			{// END_MOTIONSETが来るまで繰り返し

				// 確認する
				std::getline(File, line);

				if (line.find("LOOP") != std::string::npos)
				{// LOOPが来たらループON/OFF読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>			// ゴミ
						hoge >>			// ＝
						loadInfo.nLoop;	// ループ0か1
				}
				else if (line.find("NUM_KEY") != std::string::npos)
				{// NUM_KEYが来たらキー数読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>				// ゴミ
						hoge >>				// ＝
						loadInfo.nNumKey;	// キー数
				}
				else if (line.find("MOVE") != std::string::npos)
				{// MOVEが来たら移動ON/OFF読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>				// ゴミ
						hoge >>				// ＝
						loadInfo.nMove;		// 移動0か1か
				}
				else if (line.find("SPECIAL") != std::string::npos)
				{// SPECIALが来たらON/OFF読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// スペシャル
					int nSpecial;

					// 情報渡す
					lineStream >>
						hoge >>		// ゴミ
						hoge >>		// ＝
						nSpecial;	// スペシャル判定

					// スペシャル判定
					loadInfo.bSpecial = (nSpecial == 1);
				}
				else if (line.find("CANCELABLE") != std::string::npos)
				{// CANCELABLEでキャンセル可能フレーム読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>		// ゴミ
						hoge >>		// ＝
						loadInfo.nCancelableFrame;	// キャンセル可能フレーム
				}
				else if (line.find("COMBOLABLE") != std::string::npos)
				{// COMBOLABLEでコンボ可能フレーム読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>		// ゴミ
						hoge >>		// ＝
						loadInfo.nCombolableFrame;	// コンボ可能フレーム
				}


				//--------------------------
				// 攻撃情報
				//--------------------------
				if (line.find("ATTACKINFO") != std::string::npos)
				{// ATTACKINFOが来たら

					// 読み込み用変数
					AttackInfo loadAttackInfo;

					while (line.find("END_ATTACKINFO") == std::string::npos)
					{// END_ATTACKINFOが来るまで繰り返し

						// 確認する
						std::getline(File, line);


						if (line.find("COLLISIONPARTS") != std::string::npos)
						{// COLLISIONPARTSが来たらパーツ番号読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nCollisionNum;	// 判定を取るパーツ番号
						}
						else if (line.find("ATTACKRANGE") != std::string::npos)
						{// ATTACKRANGEが来たら攻撃サイズ読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.fRangeSize;	// 判定のサイズ
						}
						else if (line.find("OFFSET") != std::string::npos)
						{// OFFSETが来たらオフセット読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.Offset.x >> loadAttackInfo.Offset.y >> loadAttackInfo.Offset.z;	// オフセット
						}
						else if (line.find("ATTACKCOUNT") != std::string::npos)
						{// ATTACKCOUNTが来たら攻撃カウント読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nMinCnt >>
								loadAttackInfo.nMaxCnt;	// 衝撃のカウント
						}
						else if (line.find("INPACTCOUNT") != std::string::npos)
						{// INPACTCOUNTが来たら衝撃カウント読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nInpactCnt;	// 衝撃のカウント
						}
						else if (line.find("DAMAGE") != std::string::npos)
						{// DAMAGEが来たら攻撃力読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.nDamage;	// 攻撃力
						}
						else if (line.find("ALIGNFRAME") != std::string::npos)
						{// ALIGNFRAMEが来たら足揃え

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadAttackInfo.AlignInfo.nFrame >>			// 衝撃のフレーム
								loadAttackInfo.AlignInfo.nExtensionFrame;	// 猶予フレーム
						}
					}

					// 攻撃の情報追加
					loadInfo.AttackInfo.push_back(loadAttackInfo);
				}


				//--------------------------
				// 各キーの設定
				//--------------------------
				if (line.find("KEYSET") != std::string::npos)
				{// KEYSETでキー情報の読み込み開始

					// 読み込み用データ
					Key loadKey;

					while (line.find("END_KEYSET") == std::string::npos)
					{// END_KEYSETが来るまで繰り返し

						// 確認する
						std::getline(File, line);


						if (line.find("FRAME") != std::string::npos)
						{// FRAMEが来たら再生フレーム数読み込み

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>			// ゴミ
								hoge >>			// ＝
								loadKey.nFrame;	// 再生フレーム
						}


						//--------------------------
						// パーツの設定
						//--------------------------
						if (line.find("PARTS") != std::string::npos)
						{// PARTSでパーツ情報の読み込み開始

							// 読み込み用データ
							Parts loadParts;

							while (line.find("END_PARTS") == std::string::npos)
							{// END_PARTSが来るまで繰り返し

								// 確認する
								std::getline(File, line);

								if (line.find("POS") != std::string::npos)
								{// POS

									// ストリーム作成
									std::istringstream lineStream(line);

									// 情報渡す
									lineStream >>
										hoge >>			// ゴミ
										hoge >>			// ＝
										loadParts.pos.x >> loadParts.pos.y >> loadParts.pos.z;	// 位置
								}
								else if (line.find("ROT") != std::string::npos)
								{// ROT

									// ストリーム作成
									std::istringstream lineStream(line);

									// 情報渡す
									lineStream >>
										hoge >>			// ゴミ
										hoge >>			// ＝
										loadParts.rot.x >> loadParts.rot.y >> loadParts.rot.z;	// 向き
								}
								else if (line.find("SCALE") != std::string::npos)
								{// SCALE

									// ストリーム作成
									std::istringstream lineStream(line);

									// 情報渡す
									lineStream >>
										hoge >>			// ゴミ
										hoge >>			// ＝
										loadParts.scale.x >> loadParts.scale.y >> loadParts.scale.z;	// スケール
								}

							}// END_PARTSのかっこ

							// パーツデータ追加
							loadKey.aParts.push_back(loadParts);
						}

					}// END_KEYSETのかっこ

					// キーデータ追加
					loadInfo.aKey.push_back(loadKey);
				}

			}// END_MOTIONSETのかっこ
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();


	//-----------------------------
	// 読み込み後の計算
	//-----------------------------
	// キーの数
	loadInfo.nNumKey = static_cast<int>(loadInfo.aKey.size());

	// 攻撃情報の数
	loadInfo.nNumAttackInfo = static_cast<int>(loadInfo.AttackInfo.size());

	for (int i = 0; i < loadInfo.nNumKey; i++)
	{
		// 1つ前のキーインデックス
		int nBeforeCnt = UtilFunc::Transformation::Clamp(i - 1, 0, loadInfo.nNumKey);

		// 1つ前の位置
		float PosX = loadInfo.aKey[nBeforeCnt].aParts[0].pos.x;
		float PosZ = loadInfo.aKey[nBeforeCnt].aParts[0].pos.z;

		// 向きを求める
		loadInfo.aKey[i].fRotMove = atan2f(
			(PosX - loadInfo.aKey[i].aParts[0].pos.x),
			(PosZ - loadInfo.aKey[i].aParts[0].pos.z));
	}

	// 読み込みデータ渡す
	return loadInfo;
}

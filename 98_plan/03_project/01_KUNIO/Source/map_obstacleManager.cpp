//=============================================================================
// 
//  マップの障害物マネージャ処理 [map_obstacleManager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "map_obstacleManager.h"
#include "map_obstacle.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXT = "data\\TEXT\\map\\obstacle.txt";	// 設置情報スクリプトファイル
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン
}
CMap_ObstacleManager* CMap_ObstacleManager::m_ThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CMap_ObstacleManager::CMap_ObstacleManager()
{
	m_ObstacleInfo.clear();	// 障害物情報

}

//==========================================================================
// デストラクタ
//==========================================================================
CMap_ObstacleManager::~CMap_ObstacleManager()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CMap_ObstacleManager *CMap_ObstacleManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CMap_ObstacleManager;

		if (m_ThisPtr != nullptr)
		{
			// 初期化処理
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMap_ObstacleManager::Init()
{
#if 0
	SObstacleInfo info;

	MyLib::Collider_BOX box;
	info.boxcolliders.push_back(box);

	// ファイルにキャラクターのデータを書き込む
	std::ofstream outFile("data/TEXT/mapobstacle/stone/collider.json");
	if (!outFile.is_open())
	{
		return E_FAIL;
	}

	nlohmann::json outputData;
	info.to_json(outputData);

	// ファイルにデータを書き込む
	outFile << std::setw(4) << outputData << std::endl;

	outFile.close();
#endif

	// 読み込み
	Load();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMap_ObstacleManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 削除
//==========================================================================
void CMap_ObstacleManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CMap_ObstacleManager::Update()
{
	

}

//==========================================================================
// セーブ
//==========================================================================
void CMap_ObstacleManager::Save()
{
	// リスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	// ファイルを開く
	std::ofstream File(TEXT);
	if (!File.is_open()) {
		return;
	}


	// テキストファイル名目次
	File << TEXT_LINE << std::endl;
	File << " テキストファイル名" << std::endl;
	File << TEXT_LINE << std::endl;

	int i = 0;
	for (const auto& info : m_ObstacleInfo)
	{
		// テキストファイル名
		File << "TEXT_FILENAME = " << info.textFile << "\t\t# [" << i << "]" << std::endl;
		i++;
	}
	File << "" << std::endl;

	File << TEXT_LINE << std::endl;
	File << " モデルの配置" << std::endl;
	File << TEXT_LINE << std::endl;
	while (list.ListLoop(itr))
	{
		pObj = (*itr);

		// 障害物情報取得
		MyLib::Vector3 pos = pObj->GetPosition(), rot = pObj->GetRotation(), scale = pObj->GetScale();
		SObstacleInfo info = pObj->GetObstacleInfo();
		std::string text = info.textFile;

		const auto& obItr = std::find_if(m_ObstacleInfo.begin(), m_ObstacleInfo.end(), [&text](const SObstacleInfo& string) {return string.textFile == text; });
		int type = (obItr - m_ObstacleInfo.begin());

		// モデル情報
		File << "MODELSET" << std::endl;
		File << "\tTYPE = " << type << std::endl;
		File << "\tPOS = " << std::fixed << std::setprecision(2) << pos.x << " " << pos.y << " " << pos.z << std::endl;
		File << "\tROT = " << std::fixed << std::setprecision(2) << rot.x << " " << rot.y << " " << rot.z << std::endl;
		File << "\tSCALE = " << std::fixed << std::setprecision(2) << scale.x << " " << scale.y << " " << scale.z << std::endl;
		File << "END_MODELSET" << std::endl;
		File << "" << std::endl;
	}

	// ファイルを閉じる
	File << "\nEND_SCRIPT\t\t# この行は絶対消さないこと！" << std::endl;
	File.close();

}

//==========================================================================
// 情報セーブ
//==========================================================================
void CMap_ObstacleManager::SaveInfo()
{
	for (const auto& info : m_ObstacleInfo)
	{
		// ファイルを開く
		std::ofstream File(info.textFile);
		if (!File.is_open()) {
			return;
		}

		File << "#====================================================================================================" << std::endl;
		File << "#" << std::endl;
		File << "# 障害物スクリプトファイル [setup.txt]" << std::endl;
		File << "# Author : 相馬 靜雅" << std::endl;
		File << "#" << std::endl;
		File << "#====================================================================================================" << std::endl;
		File << "SCRIPT		# この行は絶対消さないこと！\n" << std::endl;

		{
			// モデルファイル名目次
			File << TEXT_LINE << std::endl;
			File << " モデルファイル名" << std::endl;
			File << TEXT_LINE << std::endl;

			// モデルファイル名
			File << "MODEL_FILENAME = " << info.modelFile << std::endl;
			File << "" << std::endl;
		}

		{
			// モデル情報目次
			File << TEXT_LINE << std::endl;
			File << " モデル情報" << std::endl;
			File << TEXT_LINE << std::endl;

			// モデル情報
			File << "SETUP" << std::endl;
			File << "\tIS_AIR = " << info.setup.isAir << "\t\t# 空気を通すか[0:通さない / 1:通す]" << std::endl;
			File << "\tIS_MOVE = " << info.setup.isMove << "\t\t# 動きを持たせるか[0:動く / 1:動かない]" << std::endl;
			File << "END_SETUP" << std::endl;
			File << "" << std::endl;
		}

		{
			// コライダー目次
			File << TEXT_LINE << std::endl;
			File << " コライダー情報" << std::endl;
			File << TEXT_LINE << std::endl;

			// コライダーファイル名
			File << "COLLIDER_FILENAME = " << info.colliderFile << std::endl;
			File << "" << std::endl;
		}


		// ファイルを閉じる
		File << "\nEND_SCRIPT\t\t# この行は絶対消さないこと！" << std::endl;
		File.close();

		// ファイルにキャラクターのデータを書き込む
		std::ofstream outFile(info.colliderFile);
		if (!outFile.is_open())
		{
			return;
		}

		nlohmann::json outputData;
		info.to_json(outputData);

		// ファイルにデータを書き込む
		outFile << std::setw(4) << outputData << std::endl;
		outFile.close();
	}
}

//==========================================================================
// ロード
//==========================================================================
void CMap_ObstacleManager::Load()
{
	// ファイルを開く
	std::ifstream File(TEXT);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

	// ファイル名
	std::vector<std::string> filename;

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

		// ストリーム作成
		std::istringstream lineStream(line);

		if (line.find("TEXT_FILENAME") != std::string::npos)
		{// TEXT_FILENAMEでモデル名読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 追加
			filename.emplace_back();

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>			// ＝
				filename.back();// モデルファイル名

			// 情報読み込み
			LoadInfo(filename.back());
			continue;
		}

		if (line.find("MODELSET") != std::string::npos)
		{// MODELSETで配置情報読み込み

			// 読み込み情報
			int type;
			MyLib::Vector3 pos, rot, scale(1.0f);

			while (line.find("END_MODELSET") == std::string::npos)
			{
				std::getline(File, line);
				if (line.find("TYPE") != std::string::npos)
				{// TYPEで配置物の種類

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>	// ＝
						type;	// 配置物の種類
					continue;
				}

				if (line.find("POS") != std::string::npos)
				{// POSで位置

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>						// ＝
						pos.x >> pos.y >> pos.z;	// 位置
					continue;
				}

				if (line.find("ROT") != std::string::npos)
				{// ROTで向き

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>						// ＝
						rot.x >> rot.y >> rot.z;	// 向き
					continue;
				}

				if (line.find("SCALE") != std::string::npos)
				{// SCALEで拡大率

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>						// ＝
						scale.x >> scale.y >> scale.z;	// 拡大率
					continue;
				}
			}

			// 生成
			if (type < static_cast<int>(m_ObstacleInfo.size()))
			{
				CMap_Obstacle* pObj = CMap_Obstacle::Create(m_ObstacleInfo[type], pos);
				pObj->SetRotation(rot);
				pObj->SetScale(scale);
			}
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 情報ロード
//==========================================================================
void CMap_ObstacleManager::LoadInfo(const std::string& file)
{
	// ファイルを開く
	std::ifstream File(file);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

	// 障害物情報
	SObstacleInfo obstacleInfo;
	obstacleInfo.textFile = file;

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

		// ストリーム作成
		std::istringstream lineStream(line);


		if (line.find("MODEL_FILENAME") != std::string::npos)
		{// MODEL_FILENAMEでモデル名読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>		// ＝
				hoge >>		// ＝
				obstacleInfo.modelFile;	// モデルファイル名
			continue;
		}

		if (line.find("COLLIDER_FILENAME") != std::string::npos)
		{// COLLIDER_FILENAMEでコライダー読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>			// ＝
				obstacleInfo.colliderFile;	// コライダー名


			//=============================
			// コライダー読み込み
			//=============================
			{
				// ファイルからJSONを読み込む
				std::ifstream filejson(obstacleInfo.colliderFile);
				if (!filejson.is_open())
				{
					return;
				}

				nlohmann::json jsonData;
				filejson >> jsonData;	// jsonデータを与える

				// jsonデータから読み込む
				obstacleInfo.from_json(jsonData);


				int n = 0;
			}
			continue;
		}

		if (line.find("SETUP") != std::string::npos)
		{// SETUPで情報読み込み

			while (line.find("END_SETUP") == std::string::npos)
			{
				std::getline(File, line);
				if (line.find("IS_AIR") != std::string::npos)
				{// IS_AIRで空気貫通読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>	// ＝
						obstacleInfo.setup.isAir;	// 空気貫通
					continue;
				}

				if (line.find("IS_MOVE") != std::string::npos)
				{// IS_MOVEで動き読み込み

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>	// ＝
						obstacleInfo.setup.isMove;	// 動き
					continue;
				}
			}
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	if (obstacleInfo.boxcolliders.empty())
	{
		obstacleInfo.boxcolliders.push_back(MyLib::Collider_BOX());
	}

	// 情報追加
	obstacleInfo.type = m_ObstacleInfo.size();
	m_ObstacleInfo.push_back(obstacleInfo);

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// コライダー追加
//==========================================================================
void CMap_ObstacleManager::AddCollider(int idx)
{
	if (static_cast<int>(m_ObstacleInfo.size()) <= idx) {
		return;
	}

	m_ObstacleInfo[idx].boxcolliders.push_back(MyLib::Collider_BOX());
}

//==========================================================================
// コライダー削除
//==========================================================================
void CMap_ObstacleManager::SubCollider(int idx)
{
	if (static_cast<int>(m_ObstacleInfo[idx].boxcolliders.size()) >= 1)
	{
		m_ObstacleInfo[idx].boxcolliders.pop_back();
	}
}

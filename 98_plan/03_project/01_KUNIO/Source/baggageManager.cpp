//=============================================================================
// 
//  荷物マネージャ [baggageManager.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "baggageManager.h"
#include "baggage.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXT = "data\\TEXT\\baggage\\baggage_info.txt";	// 設置情報スクリプトファイル
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン
}
CBaggageManager* CBaggageManager::m_ThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CBaggageManager::CBaggageManager()
{
	m_BaggageInfo.clear();	// 障害物情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CBaggageManager::~CBaggageManager()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CBaggageManager *CBaggageManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CBaggageManager;

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
HRESULT CBaggageManager::Init()
{
	// 読み込み
	Load();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBaggageManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 削除
//==========================================================================
void CBaggageManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CBaggageManager::Update()
{
	

}

//==========================================================================
// 種類指定して生成
//==========================================================================
CBaggage* CBaggageManager::CreateBaggage(CBaggage::TYPE idx)
{
	return CBaggage::Create(idx, m_BaggageInfo[idx]);
}

//==========================================================================
// セーブ
//==========================================================================
void CBaggageManager::Save()
{
	// ファイルを開く
	std::ofstream File(TEXT);
	if (File.is_open()) 
	{
		File << "#====================================================================================================" << std::endl;
		File << "#" << std::endl;
		File << "# 荷物情報スクリプトファイル [baggage_info.txt]" << std::endl;
		File << "#" << std::endl;
		File << "#====================================================================================================" << std::endl;
		File << "SCRIPT		# この行は絶対消さないこと！\n" << std::endl;

		// 荷物情報目次
		File << TEXT_LINE << std::endl;
		File << "# 荷物情報" << std::endl;
		File << "# MODEL  : モデルのファイルパス" << std::endl;
		File << "# WEIGHT : 重さ" << std::endl;
		File << "# WIDTH : 振れ幅" << std::endl;
		File << "# CYCLE : 周期（ラジアン単位）" << std::endl;
		File << "# LIFE : 体力" << std::endl;
		File << TEXT_LINE << std::endl;

		// 情報設定
		for (const auto& info : m_BaggageInfo)
		{
			File << "BAGGAGESET" << std::endl;
			File << "MODEL = " << info.path << std::endl;
			File << "WEIGHT = " << info.weight << std::endl;
			File << "WIDTH = " << info.width << std::endl;
			File << "CYCLE = " << info.cycle << std::endl;
			File << "LIFE = " << info.life << std::endl;
			File << "END_BAGGAGESET" << std::endl;
			File << "\n" << std::endl;
		}

		// ファイルを閉じる
		File << "END_SCRIPT\t\t# この行は絶対消さないこと！" << std::endl;
		File.close();
	}
}

//==========================================================================
// ロード
//==========================================================================
void CBaggageManager::Load()
{
	// ファイルを開く
	std::ifstream File(TEXT);
	if (File.is_open()) {
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

			if (line.find("BAGGAGESET") != std::string::npos)
			{// MODELSETで配置情報読み込み

				// 読み込み情報
				CBaggage::SBaggageInfo info;

				while (line.find("END_BAGGAGESET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("MODEL") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.path;	// パス
						info.path = UtilFunc::Transformation::ReplaceBackslash(info.path);
					}
					else if (line.find("WEIGHT") != std::string::npos)
					{// POSで位置

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>			// ＝
							info.weight;	// 重さ
					}
					else if (line.find("WIDTH") != std::string::npos)
					{// ROTで向き

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.width;	// ぶれ幅
					}
					else if (line.find("CYCLE") != std::string::npos)
					{// ROTで向き

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.cycle;	// 周期
					}
					else if (line.find("LIFE") != std::string::npos)
					{// LIFEで体力

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.life;	// 周期
					}
				}

				// 取り込み
				m_BaggageInfo.push_back(info);
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// ファイルを閉じる
		File.close();
	}
}

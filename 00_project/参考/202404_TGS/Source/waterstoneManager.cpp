//=============================================================================
// 
//  マップの障害物マネージャ処理 [waterstoneManager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "waterstoneManager.h"
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXT = "data\\TEXT\\map\\waterstone.bin";	// 設置情報ファイル
}
CWaterStone_Manager* CWaterStone_Manager::m_ThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CWaterStone_Manager::CWaterStone_Manager()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CWaterStone_Manager::~CWaterStone_Manager()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CWaterStone_Manager *CWaterStone_Manager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CWaterStone_Manager;

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
HRESULT CWaterStone_Manager::Init()
{
	// 読み込み
	//Load();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CWaterStone_Manager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 削除
//==========================================================================
void CWaterStone_Manager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CWaterStone_Manager::Update()
{
	

}

//==========================================================================
// セーブ
//==========================================================================
void CWaterStone_Manager::Save()
{

	// 障害物のリスト取得
	CListManager<CWaterStone> list = CWaterStone::GetListObj();

	// 先頭を保存
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CWaterStone* pObj = nullptr;

	// 石情報のコンテナ
	std::vector<SStoneInfo> vecInfo;

	int i = 0;
	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;
		MyLib::Vector3 ObjPos = pObj->GetPosition();

		vecInfo.push_back(pObj->GetStoneInfo());
	}



	// ファイルを開く
	std::ofstream File(TEXT, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// データをバイナリファイルに書き出す
	File.write(reinterpret_cast<char*>(vecInfo.data()), vecInfo.size() * sizeof(SStoneInfo));

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// ロード
//==========================================================================
void CWaterStone_Manager::Load()
{
	// ファイルを開く
	std::ifstream File(TEXT, std::ios::binary);
	if (!File.is_open()) {

		// 例外処理
		CWaterStone::Create(SStoneInfo());
		Save();
		return;
	}

	// 構造体のサイズを取得
	std::streamsize structSize = sizeof(SStoneInfo);

	// ファイルの末尾までデータを読み込む
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);

	// データの個数を計算
	size_t numVectors = fileSize / structSize;


	// 石情報のコンテナ
	std::vector<SStoneInfo> vecInfo;
	vecInfo.resize(numVectors);

	// ファイルからデータを読み込む
	File.read(reinterpret_cast<char*>(vecInfo.data()), fileSize);

	// ファイルを閉じる
	File.close();


	// 石生成
	CreateStone(vecInfo);
	return;
}

//==========================================================================
// 石生成
//==========================================================================
void CWaterStone_Manager::CreateStone(const std::vector<SStoneInfo>& vecinfo)
{
	for (const auto& info : vecinfo)
	{
		CWaterStone::Create(info);
	}
}

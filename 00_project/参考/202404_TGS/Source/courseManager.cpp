//=============================================================================
// 
//  コースマネージャ処理 [courseManager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "courseManager.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"

#include "course.h"
#include "waterfield.h"
#include "stonewall.h"
#include "stonewall_front.h"
#include "map_block.h"
#include "spline.h"
#include "frontobj_manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\chunkdata.bin";
	const int NUM_CHUNK = 5;	// チャンクの数
	const int DECIDE_LEVEL[] =	// レベル
	{
		0,
		0,
		1,
		1,
		2
	};

	const std::vector<MyLib::Vector3> DEFAULT_SEGMENTPOS =
	{
		//{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 4000.0f, 0.0f, 0.0f },
		{ 8000.0f, 0.0f, 0.0f },
		{ 12000.0f, 0.0f, 0.0f },
		//{ 9000.0f, 0.0f, 0.0f },
	};
	const float DISTANCE_TO_CHUNCK = 1500.0f;	// チャンク同士の間隔
}
CCourseManager* CCourseManager::m_ThisPtr = nullptr;	// 自身のポインタ
const float CCourseManager::m_fBlockLength = 9000.0f;	// ブロックの長さ

//==========================================================================
// コンストラクタ
//==========================================================================
CCourseManager::CCourseManager()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCourseManager::~CCourseManager()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CCourseManager *CCourseManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CCourseManager;

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
HRESULT CCourseManager::Init()
{
	CFrontObjManager::Create();
	// 読み込み
	Load();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCourseManager::Uninit()
{
	CFrontObjManager::GetInstance()->Uninit();

	delete m_ThisPtr;
	m_ThisPtr = nullptr;

	CMapBlock::Kill();
}

//==========================================================================
// 削除
//==========================================================================
void CCourseManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CCourseManager::Update()
{
	

}

//==========================================================================
// セーブ
//==========================================================================
void CCourseManager::Save()
{
	// ファイルを開く
	std::ofstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	std::vector<std::vector<MyLib::Vector3>> savedata = m_vecAllSegmentPos;

	// 外側ベクトルのサイズを書き込む
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));

	// それぞれのブロックの最初と最後消す
	for (auto& data : savedata)
	{
		/*data.erase(data.begin());
		data.pop_back();*/

		// データの個数を計算
		size_t vecSize = data.size();

		// ベクトルのサイズをセーブ
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// ベクトル内の要素をセーブ
		File.write(reinterpret_cast<const char*>(data.data()), vecSize * sizeof(MyLib::Vector3));
	}

	// ファイルを閉じる
	File.close();

	// バイナリ保存
	CMapBlock::SaveBin();

	//// チェックポイント保存
	//CMapBlock::SaveBin_CheckPoint();

	//// 障害物保存
	//CMapBlock::SaveBin_Obstacle();

	//// 水中岩保存
	//CMapBlock::SaveBin_WaterStone();
}

//==========================================================================
// ロード
//==========================================================================
void CCourseManager::Load()
{
	
	//=============================
	// 全ブロック情報(コース)を読み込む。基点の位置
	//=============================
	// ファイルを開く
	std::ifstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		// 例外処理

		m_vecAllSegmentPos.emplace_back();

		m_vecAllSegmentPos[0] = DEFAULT_SEGMENTPOS;
		/*m_vecAllSegmentPos[0].insert(m_vecAllSegmentPos[0].begin(), MyLib::Vector3(0.0f, 0.0f, 0.0f));
		m_vecAllSegmentPos[0].push_back(DEFAULT_SEGMENTPOS.back());*/

		Save();
		return;
	}



	// まず、外側のベクトルのサイズを読み込む
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));


	std::vector<std::vector<MyLib::Vector3>> loaddata(outer_size);

	// 各内側のベクトルに対してデータを読み込む
	for (auto& inner_vector : loaddata)
	{
		// 内側のベクトルのサイズを読み込む
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// データ読み込み
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(MyLib::Vector3));
	}

	// ファイルを閉じる
	File.close();


	// ロード情報コピー
	m_vecAllSegmentPos = loaddata;


	// ランダム選出されたブロックに付随する、チェックポイント、障害物の生成
	// Blockの読み込み(障害物、チェックポイント)
	CMapBlock::Load();

	for(auto& vec : m_vecAllSegmentPos)
	{
		while (1)
		{
			if (vec[0] != MyLib::Vector3(0.0f, 0.0f, 0.0f))
			{
				vec.erase(vec.begin());
				continue;
			}
			break;
		}

		int i = 0;
		while (1)
		{
			if (vec.size() <= i)
			{
				break;
			}

			if (vec[i].x <= 12000.0f)
			{
				i++;
				continue;
			}

			if (vec[i].x > 12000.0f)
			{
				vec.erase(vec.begin() + i);

				if (vec.size() <= i)
				{
					break;
				}
			}
		}

	}

	// セーブ
	Save();


	//=============================
	// ランダム選出
	//=============================
	// 乱数の種を設定
	srand((unsigned int)time(0));
	int segmentSize = static_cast<int>(m_vecAllSegmentPos.size()) - 1;

	std::vector<int> randIdx;

	// 選出したインデックスのブロック情報
	CListManager<CMapBlockInfo> BlockInfoList = CMapBlock::GetInfoList();
	int nowLevel = 0;

	for (int i = 0; i < NUM_CHUNK; i++)
	{
		int idx = 0;
		while (1)
		{
			idx = UtilFunc::Transformation::Random(0, segmentSize);
			
			// 選出したインデックスのレベル
			nowLevel = BlockInfoList.GetData(idx)->GetLevel();

			// 前回と違う番号にする
			if ((i != 0 && segmentSize > 1))
			{
				if (idx == randIdx[i - 1]) continue;
			}

			// 該当レベルと違う場合
			if (DECIDE_LEVEL[i] != nowLevel)
			{
				continue;
			}

			break;
		}

		// 選出された番号を追加
		randIdx.push_back(idx);
	}


	//=============================
	// 一本のコースにする
	//=============================
	std::vector<MyLib::Vector3> segmentpos;	// 基点の位置
	std::vector<float> segmentlength;
	int size = 0;
	MyLib::Vector3 start;
	std::vector<MyLib::Vector3> vecstart;	// 基点の位置
	vecstart.push_back(MyLib::Vector3(2000.0f, 0.0f, 0.0f));
	start = vecstart.back() + MyLib::Vector3(DISTANCE_TO_CHUNCK, 0.0f, 0.0f);
	segmentpos.push_back(MyLib::Vector3(-3010.0f, 0.0f, 0.0f));
	segmentpos.push_back(MyLib::Vector3(-3000.0f, 0.0f, 0.0f));
	segmentpos.push_back(MyLib::Vector3(2000.0f, 0.0f, 0.0f));
	segmentlength.push_back(2000.0f);

	int i = 0;
	const float lastDistance = 20000.0f;
	for (const auto& idx : randIdx)
	{
		for (const auto& pos : m_vecAllSegmentPos[idx])
		{
			segmentpos.push_back(start + pos);
		}

		// 間隔追加
		if (i == randIdx.size() - 1)
		{// ラスト
			segmentpos.push_back(segmentpos.back() + MyLib::Vector3(lastDistance, 0.0f, 0.0f));
			start = segmentpos.back() + MyLib::Vector3(lastDistance, 0.0f, 0.0f);
		}
		else
		{
			float distance = DISTANCE_TO_CHUNCK / 8.0f;
			
			for (int push = 0; push < 8; push++)
			{
				segmentpos.push_back(segmentpos.back() + MyLib::Vector3(distance, 0.0f, 0.0f));
			}
			start = segmentpos.back() + MyLib::Vector3(DISTANCE_TO_CHUNCK, 0.0f, 0.0f);
		}
		vecstart.push_back(start);
		i++;

		// ここまでの長さ取得
		size += segmentpos.size();
		std::vector<float> calLength;
		calLength.resize(size);

		segmentlength.push_back(MySpline::CalSegmentLength_NonLoop(segmentpos, &calLength, 10.0f));
	}
	segmentpos.push_back(segmentpos.back() + MyLib::Vector3(10.0f, 0.0f, 0.0f));


	//=============================
	// コース作成
	//=============================
	CCourse* pCourse = CCourse::Create("data\\TEXT\\map\\course.bin");
	pCourse->SetVecPosition(segmentpos);
	pCourse->ReCreateVtx();
	CGame::GetInstance()->SetCource(pCourse);

	// 距離にあわせた配置を行う
	for (int i = 0; i < NUM_CHUNK; i++)
	{
		CMapBlock* pBlock = DEBUG_NEW CMapBlock;

		float f = 3000.0f;

		if (i != 0)
		{
			f = 1450.0f;
		}

		if (pBlock != nullptr)
		{
			pBlock->Set(randIdx[i], vecstart[i], segmentlength[i] + f);
		}
	}


	//この中で障害物、チェックポイント

	//=============================
	// 石垣(奥)
	//=============================
	m_pStoneWall = CStoneWall::Create();

	// 基点地点設定
	m_pStoneWall->SetVecPosition(pCourse->GetVecPosition());
	m_pStoneWall->Reset();

	std::vector<CCourse::VtxInfo> vtxInfo = pCourse->GetVecVtxinfo();
	std::vector<MyLib::Vector3> vecpos;

	MyLib::Vector3 setpos;
	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * -600.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * -600.0f;
		vecpos.push_back(setpos);
	}

	// 各頂点座標
	m_pStoneWall->SetVecVtxPosition(vecpos);
	m_pStoneWall->BindVtxPosition();


	//=============================
	// 石垣(手前)
	//=============================
	m_pStoneWall_Front = CStoneWall_Front::Create();

	// 基点地点設定
	m_pStoneWall_Front->SetVecPosition(pCourse->GetVecPosition());
	m_pStoneWall_Front->Reset();

	vtxInfo = pCourse->GetVecVtxinfo();
	vecpos.clear();

	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * 800.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * 800.0f;
		vecpos.push_back(setpos);
	}

	// 各頂点座標
	m_pStoneWall_Front->SetVecVtxPosition(vecpos);
	m_pStoneWall_Front->BindVtxPosition();


#if 1
	//=============================
	// うねりの街フィールド
	//=============================
	CMapMesh* pTownField = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS);
	pTownField->SetVecPosition(pCourse->GetVecPosition());
	pTownField->Reset();

	// 石垣の頂上に頂点をそろえる
	pTownField->SetVecVtxPosition(m_pStoneWall->GetVecTopPosition());
	pTownField->BindVtxPosition();


	//=============================
	// うねりの街フィールド(手前)
	//=============================
	CMapMesh* pTownFieldFront = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS_FRONT);
	pTownFieldFront->SetVecPosition(pCourse->GetVecPosition());
	pTownFieldFront->Reset();

	// 石垣の頂上に頂点をそろえる
	pTownFieldFront->SetVecVtxPosition(m_pStoneWall_Front->GetVecTopPosition());
	pTownFieldFront->BindVtxPosition();

	//=============================
	// 固定平面街フィールド
	//=============================
	CMapMesh* pTownFieldFixed = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_FIXEDPLANE);
#endif
}

//==========================================================================
// 基点追加
//==========================================================================
void CCourseManager::AddSegmentPos()
{
	m_vecAllSegmentPos.emplace_back();
	m_vecAllSegmentPos.back() = DEFAULT_SEGMENTPOS;
}

//==========================================================================
// 基点削除
//==========================================================================
void CCourseManager::SubSegmentPos()
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;
	m_vecAllSegmentPos.pop_back();
}

//==========================================================================
// 基点削除
//==========================================================================
void CCourseManager::SubSegmentPos(int idx)
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;

	if (static_cast<int>(m_vecAllSegmentPos.size()) <= idx) return;

	std::vector<std::vector<MyLib::Vector3>>::iterator itr = m_vecAllSegmentPos.begin();
	std::advance(itr, idx);
	m_vecAllSegmentPos.erase(itr);

}

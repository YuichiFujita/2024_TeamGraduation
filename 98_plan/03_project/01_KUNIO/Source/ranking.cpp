//=============================================================================
// 
//  ランキング処理 [ranking.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "rankingscore.h"
#include "keyconfig.h"

#include "rankingItem.h"
#include "rankingItem_top3.h"

// マップ用
#include "tree.h"
#include "waterfield.h"
#include "course.h"
#include "mapmesh.h"
#include "stonewall.h"
#include "peoplemanager.h"
#include "environment.h"

//=============================================================================
// 定数定義
//=============================================================================
namespace
{
	//ランキングのコンフィグ
	const float SCROLL_SPEED = 35.0f;	// スクロール速度
	const float SCROLL_AUTOSPEED = 8.0f;	// スクロール速度
	const float SCROLL_STOP_POS_Y_HIGH = 250.0f;	// 停止座標
	const float SCROLL_STOP_POS_Y = 600.0f;	// 停止座標
	const float TIME_NONESCROLL = 2.0f;		// スクロールなしの時間
	const int NUM_RANK = 10;	// ランキング数
	const int NUM_ALLRANK = 4;	// 総評ランク数
	const std::string FILE_BIN = "data\\TEXT\\ranking\\ranking.bin";	// ランキングデータ保存ファイル
	const float DEFAULT_INTERVAL_AIRSPAWN = 1.5f;	// デフォの空気生成間隔
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CRankingScore *CRanking::m_pRankingScore = nullptr;	// ランキングスコアのオブジェクト
bool CRanking::m_bAllArrival = false;		// 全て到着した判定
CRanking::SRankdata CRanking::m_NowData = CRanking::SRankdata(0, 0, 0, 0, 0, 0, NUM_ALLRANK);

//==========================================================================
// コンストラクタ
//==========================================================================
CRanking::CRanking()
{
	m_pRankingScore = nullptr;	// ランキングスコアのオブジェクト
	m_bAllArrival = false;	// 全て到着した判定
	m_pRankData = nullptr;
	
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_pRanking[nCnt] = nullptr;
	}
	m_pPeopleManager = nullptr;	// 人マネージャ
	m_pSpawn_Air = nullptr;		// 空気生成
	m_pSpawn_Leaf = nullptr;	// 降る葉生成
	m_fNoneScrollTimer = 0.0f;	// スクロールなしの時間
}

//==========================================================================
// デストラクタ
//==========================================================================
CRanking::~CRanking()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRanking::Init()
{

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// ランキングのスコア生成
	/*m_pRankingScore = CRankingScore::Create();*/

	// ランキングデータ格納用構造体を作る
	m_pRankData = DEBUG_NEW SRankdata[NUM_RANK];

	// ファイル読み込み
	Load();

	// ソート
	Sort();

	// ランクイン確認
	RankIn();

	// ファイル保存
	Save();

	// ランキング数分生成
	for (int i = 0; i < NUM_RANK; i++)
	{
		int nDate[3] = { m_pRankData[i].year,m_pRankData[i].month,m_pRankData[i].day };
		int minutes = m_pRankData[i].minutes;
		int seconds = m_pRankData[i].seconds;
		int milliseconds = m_pRankData[i].milliSeconds;
		int allrank = m_pRankData[i].allrank;
		bool rankin = m_pRankData[i].rankin;

		if (i < 3)
		{
			// TOP3のアイテム生成（順位、時間、総評、日付、基準位置、拡大率、ランキング更新）
			m_pRanking[i] = CRankingItem_top3::Create(i + 1, minutes, seconds, milliseconds, allrank, nDate, MyLib::Vector3(250.0f, 250.0f + i * 140.0f, 0.0f), 0.9f, rankin);
		}
		else
		{
			int j = i - 3;
			//下位のアイテム生成（順位、時間、総評、基準位置、拡大率、ランキング更新）
			m_pRanking[i] = CRankingItem::Create(i + 1, minutes, seconds, milliseconds, allrank, MyLib::Vector3(250.0f, 700.0f + j * 150.0f, 0.0f), 0.85f, rankin);
		}
	}
	
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RANKING);

	// マップ生成
	CreateMap();

	// 成功
	return S_OK;
}

//==========================================================================
// マップ生成
//==========================================================================
void CRanking::CreateMap()
{
	//=============================
	// 木
	//=============================
	MyLib::Vector3 treepos2(-8723.0f, 1500.0f, 12128.0f);
	for (int nCnt = 0; nCnt <= 19; nCnt++)
	{
		CTree::Create(treepos2);
		treepos2.x += 2000.0f;
	}

	MyLib::Vector3 treepos(-8223.0f, 900.0f, 10128.0f);
	for (int nCnt = 0; nCnt <= 20; nCnt++)
	{
		CTree::Create(treepos);
		treepos.x += 2000.0f;
	}

	//=============================
	// コース
	//=============================
	CCourse::Create("", CScene::MODE::MODE_TITLE);

	//=============================
	// 固定平面街フィールド
	//=============================
	CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_FIXEDPLANE_RANKING);

	//=============================
	// 石垣(奥)
	//=============================
	{
		CStoneWall* pStoneWall = CStoneWall::Create();

		// 基点地点設定
		std::vector<MyLib::Vector3> vecpos =
		{
			MyLib::Vector3(-1500.0f, 0.0f, 3000.0f),
			MyLib::Vector3(0.0f, 0.0f, 3000.0f),
			MyLib::Vector3(500.0f, 0.0f, 3000.0f),
			MyLib::Vector3(2500.0f, 0.0f, 3000.0f),
		};
		pStoneWall->SetVecPosition(vecpos);
		pStoneWall->Reset();

		// 各頂点座標
		std::vector<MyLib::Vector3> vecVtxpos =
		{
			MyLib::Vector3(-5000.0f, 0.0f, 2000.0f),
			MyLib::Vector3(-5010.0f, 0.0f, 2000.0f),
			MyLib::Vector3(5000.0f, 0.0f, 2000.0f),
			MyLib::Vector3(5010.0f, 0.0f, 2000.0f),
		};
		pStoneWall->SetVecVtxPosition(vecVtxpos);
		pStoneWall->BindVtxPosition();
	}

	//=============================
	// 水フィールド
	//=============================
	CWaterField::Create(CWaterField::TYPE::TYPE_RIGHT);
	CWaterField::Create(CWaterField::TYPE::TYPE_LEFT);

	//=============================
	// 人マネージャ
	//=============================
	m_pPeopleManager = CPeopleManager::Create(CPeopleManager::Type::TYPE_TITLE);


	// 空気の生成クラス生成
	m_pSpawn_Air = DEBUG_NEW CSpawn_Air_Title(0.0f, DEFAULT_INTERVAL_AIRSPAWN);

	// 降る葉生成クラス生成
	m_pSpawn_Leaf = DEBUG_NEW CSpawn_Leaf_Ranking(0.0f, 0.6f);
}

//==========================================================================
// 終了処理
//==========================================================================
void CRanking::Uninit()
{
	// ランキングスコアのオブジェクト
	m_pRankingScore = nullptr;

	// ランキング情報の削除
	if (m_pRankData != nullptr)
	{
		delete[] m_pRankData;
		m_pRankData = nullptr;
	}

	// 空気生成の削除
	if (m_pSpawn_Air != nullptr)
	{
		delete m_pSpawn_Air;
		m_pSpawn_Air = nullptr;
	}

	// 降る葉生成の削除
	if (m_pSpawn_Leaf != nullptr)
	{
		delete m_pSpawn_Leaf;
		m_pSpawn_Leaf = nullptr;
	}

	// リセット
	m_NowData = SRankdata();
	m_NowData.allrank = NUM_ALLRANK;

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CRanking::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// デルタタイム取得
	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// キーコンフィグ
	CKeyConfigManager* pConfigMgr = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKey = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	//=============================
	// 遷移
	//=============================
	if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) ||
		pPad->GetTrigger(OUTGAME::ACT_OK, 0))
	{
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}

	// スクロールなしの時間加算
	m_fNoneScrollTimer += deltaTime;

	// スクロールの操作
	ControllScroll(deltaTime);

	// スクロールが基底時間されなかった
	if (m_fNoneScrollTimer >= TIME_NONESCROLL)
	{
		m_fNoneScrollTimer = TIME_NONESCROLL;
		m_ScrollMove.y -= SCROLL_AUTOSPEED * deltaTime;
	}

	//=============================
	// スクロール停止判定
	//=============================
	// 下の停止
	MyLib::Vector3 latestpos = m_pRanking[9]->GetPos();
	if (latestpos.y <= SCROLL_STOP_POS_Y && 
		m_ScrollMove.y < 0.0f)
	{
		m_ScrollMove.y = 0.0f;
	}

	// 上の停止
	MyLib::Vector3 hightpos = m_pRanking[0]->GetPos();
	if (hightpos.y >= SCROLL_STOP_POS_Y_HIGH &&
		m_ScrollMove.y > 0.0f)
	{
		m_ScrollMove.y = 0.0f;
	}


	// スクロール処理
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_pRanking[nCnt]->SetMove(m_ScrollMove);
	}

	// スクロール更新処理
	for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
	{
		m_pRanking[nCnt]->Update();
	}


	// 空気生成の更新
	if (m_pSpawn_Air != nullptr)
	{
		m_pSpawn_Air->Update(deltaTime);
	}

	// 降る葉生成の更新
	if (m_pSpawn_Leaf != nullptr)
	{
		m_pSpawn_Leaf->Update(deltaTime);
	}
}

//==========================================================================
// スクロールの操作
//==========================================================================
void CRanking::ControllScroll(float deltaTime)
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// キーコンフィグ
	CKeyConfigManager* pConfigMgr = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKey = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	// スティックの向き取得
	float stickrot = fabsf(pInputGamepad->GetStickRotL(0));

	if (pInputKeyboard->GetPress(DIK_W) ||
		(pInputGamepad->IsTipStick() && (stickrot >= 0.0f && stickrot < D3DX_PI * 0.5f)))
	{// 上スクロール

		// スクロールの移動量加算
		float stickRatio = 1.0f - (stickrot / (D3DX_PI * 0.5f));
		m_ScrollMove.y += (SCROLL_SPEED * stickRatio) * deltaTime;

		// スクロールなしの時間リセット
		m_fNoneScrollTimer = 0.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_S) ||
		(pInputGamepad->IsTipStick() && stickrot > D3DX_PI * 0.5f))
	{// 下スクロール

		// スクロールの移動量減算
		float stickRatio = stickrot / D3DX_PI;
		m_ScrollMove.y -= (SCROLL_SPEED * stickRatio) * deltaTime;

		// スクロールなしの時間リセット
		m_fNoneScrollTimer = 0.0f;
	}

	// 慣性補正
	m_ScrollMove.y += (0.0f - m_ScrollMove.y) * 0.08f;
}

//==========================================================================
// 描画処理
//==========================================================================
void CRanking::Draw()
{

}

//==========================================================================
// 到着設定ON
//==========================================================================
void CRanking::SetEnableArrival()
{
	m_bAllArrival = true;
}

//==========================================================================
// ファイル読み込み
//==========================================================================
void CRanking::Load()
{
	// ファイルを開く
	std::ifstream File(FILE_BIN, std::ios::binary);
	if (!File.is_open()) {

		if (m_pRankData == nullptr)
		return;

		// 時刻を取得
		time_t Time = time(NULL);

		// 現在の時刻を現地時間に変換
		std::tm* now = std::localtime(&Time);

		// 年、月、日、時、分、秒をそれぞれintに変換
		int year = now->tm_year + 1900;  // 年は1900年からの経過年数
		int month = now->tm_mon + 1;     // 月は0から始まるので+1
		int day = now->tm_mday;          // 日

		// 初期値を入れる
		for (int i = 0; i < NUM_RANK; i++)
		{
			m_pRankData[i].year = year;
			m_pRankData[i].month = month;
			m_pRankData[i].day = day;
			m_pRankData[i].minutes = 8 + (NUM_RANK - i) * 1;
			m_pRankData[i].seconds = i % 6 * 10;
			m_pRankData[i].milliSeconds = 0;
			m_pRankData[i].allrank = i % 3 + 1;
			m_pRankData[i].rankin = false;
		}

		return;
	}


	// データ読み込み
	File.read(reinterpret_cast<char*>(m_pRankData), sizeof(SRankdata) * NUM_RANK);

	// ファイルを閉じる
	File.close();

	// フラグオフ
	for (int i = 0; i < NUM_RANK; i++)
	{
		m_pRankData[i].rankin = false;
	}
}

//==========================================================================
// 保存
//==========================================================================
void CRanking::Save()
{
	// ファイルを開く
	std::ofstream File(FILE_BIN, std::ios::binary);
	if (!File.is_open()) {

		return;
	}

	// データ保存
	File.write(reinterpret_cast<const char*>(m_pRankData), sizeof(SRankdata) * NUM_RANK);

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// ソート
//==========================================================================
void CRanking::Sort()
{
	// 昇順ソート(総評)
	for (int fst = 0; fst < NUM_RANK - 1; fst++)
	{
		int tempNum = fst;	// 仮の一番大きい番号

		for (int sec = fst + 1; sec < NUM_RANK; sec++)
		{
			SRankdata* pRank = &m_pRankData[sec];

			if (pRank->allrank < m_pRankData[tempNum].allrank)
			{// 値が小さい場合
				tempNum = sec;	// 小さい番号を変更
			}
		}

		if (tempNum != fst)
		{// 変更する場合
			SRankdata temp = m_pRankData[fst];
			m_pRankData[fst] = m_pRankData[tempNum];
			m_pRankData[tempNum] = temp;
		}
	}

	// 昇順ソート(タイム)
	for (int i = 0; i < NUM_ALLRANK; i++)
	{
		for (int fst = 0; fst < NUM_RANK - 1; fst++)
		{
			// 同ランク内でのみ確認する
			if (m_pRankData[fst].allrank != i) { continue; }

			int tempNum = fst;	// 仮の一番大きい番号
			int temptime = (m_pRankData[fst].minutes * 10000.0f) + 
				(m_pRankData[fst].seconds * 100.0f) + 
				m_pRankData[fst].milliSeconds;

			for (int sec = fst + 1; sec < NUM_RANK; sec++)
			{
				// 同ランク内でのみ比較
				if (m_pRankData[sec].allrank != i) { continue; }

				SRankdata* pRank = &m_pRankData[sec];
				int time = (pRank->minutes * 10000.0f) +
					(pRank->seconds * 100.0f) +
					pRank->milliSeconds;

				if (time < temptime)
				{// 値が小さい場合
					tempNum = sec;	// 小さい番号を変更
					temptime = time;
				}
			}

			if (tempNum != fst)
			{// 変更する場合
				SRankdata temp = m_pRankData[fst];
				m_pRankData[fst] = m_pRankData[tempNum];
				m_pRankData[tempNum] = temp;
			}
		}
	}
}

//==========================================================================
// ランクイン
//==========================================================================
void CRanking::RankIn()
{
	// 今回のタイムを格納
	int nowallrank = m_NowData.allrank;
	int nowtime = (m_NowData.minutes * 10000.0f) +
		(m_NowData.seconds * 100.0f) +
		m_NowData.milliSeconds;

	// 最下位のタイムを格納
	SRankdata* pRank = &m_pRankData[NUM_RANK - 1];
	int lowestallrank = pRank->allrank;
	int lowesttime = (pRank->minutes * 10000.0f) +
		(pRank->seconds * 100.0f) +
		pRank->milliSeconds;

	// 最下位よりランクが上もしくは同ランク以上かつタイムが速い
	if (nowallrank < lowestallrank ||
		(nowallrank <= lowestallrank && nowtime <= lowesttime))
	{
		// 時刻を取得
		time_t Time = time(NULL);

		// 現在の時刻を現地時間に変換
		std::tm* now = std::localtime(&Time);

		// 年、月、日、時、分、秒をそれぞれintに変換
		int year = now->tm_year + 1900;  // 年は1900年からの経過年数
		int month = now->tm_mon + 1;     // 月は0から始まるので+1
		int day = now->tm_mday;          // 日
		m_NowData.year = year;
		m_NowData.month = month;
		m_NowData.day = day;

		m_pRankData[NUM_RANK - 1] = m_NowData;
		m_pRankData[NUM_RANK - 1].rankin = true;

		// サイドソート
		Sort();
	}
}
//=============================================================================
// 
//  マップブロック管理ヘッダー [map_block.h]
//  Author : Ibuki Okusada
// 
//=============================================================================

#ifndef _MAPBLOCK_H_
#define _MAPBLOCK_H_		// 二重インクルード防止

#include "listmanager.h"
#include "waterstoneManager.h"

// 前方宣言
class CCheckpoint;
class CMap_Obstacle;

//==========================================================================
// クラス定義
//==========================================================================
// マップブロック配置情報まとめクラス
class CMapBlockInfo
{
public:
	//=============================
	// 構造体情報
	//=============================
	struct SObsacleInfo
	{
		MyLib::Vector3 pos;		// 座標
		MyLib::Vector3 rot;		// 向き
		MyLib::Vector3 scale;	// スケール
		int nType;				// モデル種類

		// コンストラクタ
		SObsacleInfo() : pos(0.0f), rot(0.0f), scale(1.0f), nType(0) {}
		SObsacleInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, const MyLib::Vector3& _scale, int _type) : 
			pos(_pos), rot(_rot), scale(_scale), nType(_type) {}
	};

	// ジャッジ
	struct SJudgeInfo
	{
		float length;	// 距離
		float height;	// 高さ

		// コンストラクタ
		SJudgeInfo() : length(0.0f), height(0.0f) {}
		SJudgeInfo(float _length, float _height) : length(_length), height(_height) {}
	};

	CMapBlockInfo();
	~CMapBlockInfo();

	HRESULT Init();
	void Uninit();
	void SaveBin();	// セーブ
	void Load(std::ifstream* pFile);	// ロード
	void ObstacleLoad(std::ifstream* pFile); // 障害物読み込み
	
	// リスト
	void SetObstacleInfo(const std::vector<SObsacleInfo>& info) { m_ObstacleList = info; }	// 障害物
	std::vector<SObsacleInfo> GetObstacleInfo() { return m_ObstacleList; }					// 障害物
	void SetMapInfo(const std::vector<SObsacleInfo>& info) { m_MapList = info; }			// マップ情報
	std::vector<SObsacleInfo> GetMapInfo() { return m_MapList; }							// マップ情報
	void SetCheckpointInfo(const std::vector<float>& list) { m_CheckpointList = list; }		// チェックポイント
	std::vector<float> GetCheckpointInfo() { return m_CheckpointList; }						// チェックポイント
	void SetWaterStoneInfo(const std::vector<CWaterStone_Manager::SStoneInfo>& list) { m_WaterStoneList = list; }	// 水中岩
	std::vector<CWaterStone_Manager::SStoneInfo> GetWaterStoneInfo() { return m_WaterStoneList; }					// 水中岩
	void SetJudgeInfo(const std::vector<std::vector<SJudgeInfo>>& info) { m_JudgeList = info; }	// ジャッジ
	std::vector<std::vector<SJudgeInfo>> GetJudgeInfo() { return m_JudgeList; }					// ジャッジ

	void SetLevel(int level) { m_nLevel = level; }	// レベル設定
	int GetLevel() { return m_nLevel; }				// レベル取得

private:

	// 配置情報リスト
	std::vector<SObsacleInfo> m_ObstacleList;		// 障害物
	std::vector<SObsacleInfo> m_MapList;			// マップ
	std::vector<float> m_CheckpointList;			// チェックポイント
	std::vector<std::vector<SJudgeInfo>> m_JudgeList;	// ジャッジ
	std::vector<CWaterStone_Manager::SStoneInfo> m_WaterStoneList;	// 水中岩
	int m_nLevel;	// 難易度
};

// マップブロック管理クラス
class CMapBlock
{
public:

	CMapBlock();	// コンストラクタ
	~CMapBlock();	// デストラクタ

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	// 入出力
	
	void Set(const int Idx, const MyLib::Vector3& startpos, float startlength);
	CListManager<CMap_Obstacle> GetObstacleList() { return m_ObstacleList; }
	CListManager<CCheckpoint> GetCheckpointList() { return m_CheckpointList; }

	void AddInfo();	// 情報追加
	void SubInfo();	// 情報削除

	//=============================
	// 静的関数
	//=============================
	static CListManager<CMapBlockInfo>* Load();	// 読み込み
	static void SaveBin();	// セーブ
	static void SaveBin_CheckPoint();	// チェックポイントセーブ
	static void SaveBin_Obstacle();		// 障害物セーブ
	static void SaveBin_Map();			// マップセーブ
	static void SaveBin_WaterStone();	// 水中岩セーブ
	static void SaveBin_Judge();		// ジャッジセーブ
	static void SaveBin_Level();		// レベルセーブ
	static void LoadBin();	// ロード
	static void Kill();	// 開放
	static CListManager<CMapBlock> GetList() { return m_List; }
	static void SetInfoList(const CListManager<CMapBlockInfo>& info) { m_InfoList = info; }	// 配置情報リスト取得
	static CListManager<CMapBlockInfo> GetInfoList() { return m_InfoList; }	// 配置情報リスト取得
	static CListManager<CMapBlockInfo>* GetInfoListPtr() { return &m_InfoList; }	// 配置情報リスト取得
	static void Delete();

private:

	//=============================
	// メンバ関数
	//=============================
	static std::vector<std::vector<float>> LoadBin_CheckPoint();								// チェックポイント読み込み
	static std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> LoadBin_Obstacle();			// 障害物読み込み
	static std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> LoadBin_Map();					// マップ読み込み
	static std::vector<std::vector<CWaterStone_Manager::SStoneInfo>> LoadBin_WaterStone();		// 水中岩読み込み
	static std::vector<std::vector<std::vector<CMapBlockInfo::SJudgeInfo>>> LoadBin_Judge();	// ジャッジ読み込み
	static std::vector<int> LoadBin_Level(size_t courseSize);									// レベル読み込み
	void DataDelete();
	void InfoDelete();

	//=============================
	// メンバ変数
	//=============================
	static CListManager<CMapBlock> m_List;	// リスト
	static CListManager<CMapBlockInfo> m_InfoList;	// 配置情報リスト

	// 配置情報リスト
	CListManager<CMap_Obstacle> m_ObstacleList = {};	// 障害物
	CListManager<CCheckpoint> m_CheckpointList = {};	// チェックポイント
};

#endif

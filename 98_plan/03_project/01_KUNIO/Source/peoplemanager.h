//=============================================================================
// 
//  人マネージャヘッダー [peoplemanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PEOPLEMANAGER_H_
#define _PEOPLEMANAGER_H_	// 二重インクルード防止

#include "judge.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPeople;

//==========================================================================
// クラス定義
//==========================================================================
// 人マネージャクラス
class CPeopleManager
{
public:

	//=============================
	// 構造体定義
	//=============================
	struct SPeopleData
	{
		int nType;			// キャラクター種類
		MyLib::Vector3 pos;	// 位置

		SPeopleData() : nType(0), pos(MyLib::Vector3()) {}
		SPeopleData(int _nType,MyLib::Vector3 _pos) : nType(_nType), pos(_pos) {}

		bool operator==(const SPeopleData& other) const
		{
			return (this->nType == other.nType && this->pos == other.pos) ? true : false;
		}
	};

	struct SPattern
	{
		int nNum;	// 総数
		int nRank;	// ランク
		std::vector<SPeopleData> data;
	};

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_NONE = 0,		// 何もない状態
		STATE_MAX
	};

	enum Type
	{
		TYPE_GAME = 0,
		TYPE_RESULT,
		TYPE_TITLE,
		TYPE_MAX
	};

	CPeopleManager();
	~CPeopleManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	HRESULT ReadText(const std::string& filename);	// 外部ファイル読み込み処理
	void SetPeople(const MyLib::Vector3& pos, const MyLib::Vector3& rot, int nPattern);	// 敵配置
	void SetByRank();	// ランクごとのセット処理
	void DespawnPeople();	// 範囲外の人を消す
	void LateSpawn();		// 後生成分の生成
	void SetRank(const CJudge::JUDGE& judge) { m_Rank = judge; }			// ランク設定
	void ResetLateSpawn() { m_lateSpawnPeople.clear(); }

	static CPeopleManager* GetInstance() { return m_ThisPtr; }				// 自身のポインタ
	static CPeopleManager* Create(Type type);


protected:

	//=============================
	// メンバ関数
	//=============================
	void SetEffect();	// エフェクト生成

	//=============================
	// メンバ変数
	//=============================
	CJudge::JUDGE m_Rank;							// 現在のランク
	CJudge::JUDGE m_OldRank;						// 前回のランク
	float m_fTimer_EffectSpawn;						// エフェクト生成のタイマー
	float m_fInterval_EffectSpawn;					// エフェクト生成の間隔
	std::vector<SPattern> m_vecPattern;				// 配置パターン
	std::vector<SPattern> m_PatternByRank[CJudge::JUDGE::JUDGE_MAX];			// 配置パターン
	std::list<SPeopleData> m_lateSpawnPeople;		// 後生成の人リスト
	std::vector<std::string> m_vecMotionFileName;	// モーションファイル名
	static CPeopleManager* m_ThisPtr;				// 自身のポインタ
};

// リザルトのマネージャ
class CPeopleManager_Result : public CPeopleManager
{
public:

	CPeopleManager_Result() : CPeopleManager() {}
	~CPeopleManager_Result() {}

	HRESULT Init() override;
	void Update() override;

private:

};


// タイトルのマネージャ
class CPeopleManager_Title : public CPeopleManager
{
public:

	CPeopleManager_Title() : CPeopleManager() {}
	~CPeopleManager_Title() {}

	HRESULT Init() override;
	void Update() override;

private:

};


#endif
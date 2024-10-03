//=============================================================================
// 
//  判定ゾーンマネージャヘッダー [judgezoneManager.h]
//  Author : 石原颯馬
// 
//=============================================================================
#ifndef _JUDGE_ZONE_MANAGER_H_
#define _JUDGE_ZONE_MANAGER_H_		// 二重インクルード防止
#include "listmanager.h"
#include "judgezone.h"
#include "judge.h"
#include <string>

//==========================================================================
// クラス定義
//==========================================================================
// 判定ゾーンマネージャクラス
class CJudgeZoneManager
{
public:
	CJudgeZoneManager();
	~CJudgeZoneManager();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// メンバ関数
	//=============================
	void Check(float progress);
	void Release();		//無効化した判定ゾーンを破棄
	void ReleaseAll();	//判定ゾーンをすべて破棄

	void Load(std::string path);
	void LoadZone(std::string path);
	CJudge::SJudgeCondition LoadCondition(std::string path);
	void Save(std::string path);
	void SaveZone(std::string path, CJudgeZone::SJudgeZone zoneinfo, CJudge::SJudgeCondition conditionUp, CJudge::SJudgeCondition conditionUnder);
	bool SaveCondition(std::string path, CJudge::SJudgeCondition condition);

	//=============================
	// 静的関数
	//=============================
	static CJudgeZoneManager* Create();
	static CJudgeZoneManager* GetInstance() { return m_ThisPtr; }	// インスタンス取得

private:
	static CJudgeZoneManager* m_ThisPtr;		// 自身のポインタ
};

#endif
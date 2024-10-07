//=============================================================================
// 
//  判定ゾーンエディタ―ヘッダー [edit_judgezone.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _EDIT_JUDGEZONE_H_
#define _EDIT_JUDGEZONE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"
#include "judgezone.h"
#include "judge.h"
#include <map>

//==========================================================================
// クラス定義
//==========================================================================
// コースエディタ―クラス
class CEdit_JudgeZone : public CEdit
{
public:

	CEdit_JudgeZone();
	~CEdit_JudgeZone();
	
	// オーバーライド関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:
	//=============================
	// 判定ゾーンデータ構造体定義
	//=============================
	struct SJudgeZoneData
	{
		CJudgeZone::SJudgeZone zone;	//判定ゾーン
		CJudge::SJudgeCondition conditionTop, conditionUnder;	//判定条件
	};

	// メンバ関数
	void SelectZone();		// ゾーン選択
	void FileControl();		// ファイル操作
	void SetJudgeZone();	// 判定ゾーン設定
	void SetCondition(CJudge::SJudgeCondition& condition,bool* pEnable, CJudge::CONDITIONTYPE& conditionType);
	OPENFILENAMEA CreateOFN(std::string relativeDir, char* filePass);

	CJudgeZone* m_pSelectZone;
	SJudgeZoneData m_JudgeZoneData;			// 判定ゾーン情報
	CJudge::CONDITIONTYPE m_aConditionType[CJudge::BORDER::MAX];	// 条件種類
	bool m_bHoverWindow;					// マウスのウィンドウホバー判定
	bool m_abEnable[CJudge::BORDER::MAX][CJudge::JUDGE::JUDGE_MAX];
};


#endif
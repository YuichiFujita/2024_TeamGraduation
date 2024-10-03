//=============================================================================
// 
//  判定ゾーンヘッダー [judgezone.h]
//  Author : 石原颯馬
// 
//=============================================================================
#ifndef _JUDGE_ZONE_H_
#define _JUDGE_ZONE_H_		// 二重インクルード防止
#include "listmanager.h"
#include "judge.h"
#include <map>

//==========================================================================
// クラス定義
//==========================================================================
//**************************************************************************
// 判定ゾーン全体
//**************************************************************************
class CJudgeZone
{
public:
	// 判定ゾーン構造体
	struct SJudgeZone
	{
		float start, end;	// 始点,終点
		float borderHeight;

		SJudgeZone() : start(0.0f), end(0.0f), borderHeight(0.0f) {}
		SJudgeZone(float _start, float _end, float _border) : start(_start), end(_end), borderHeight(_border) {}
	};

	CJudgeZone();
	virtual ~CJudgeZone();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// メンバ関数
	//=============================
	virtual void Check();
	virtual CJudge::JUDGE Judge();

	// 取得・設定
	SJudgeZone GetZone() { return m_zone; }
	CJudge::SJudgeCondition GetInfo(CJudge::BORDER border) { return m_aJudgeInfo[border]; }
	void SetZone(const SJudgeZone zone) { m_zone = zone; }
	void SetInfo(CJudge::BORDER border, CJudge::SJudgeCondition info) { m_aJudgeInfo[border] = info; }
	bool IsEnable() { return m_isEnable; }
	std::string GetPath() { return m_path; }
	void SetPath(std::string path) { m_path = path; }

	//=============================
	// 静的関数
	//=============================
	static CJudgeZone* Create(const float start, const float end, const float borderHeight);	// 生成
	static CListManager<CJudgeZone> GetListObj() { return m_List; }								// リスト取得

private:
	SJudgeZone m_zone;
	CJudge* m_pJudge;
	bool m_isEnable;
	CJudge::SJudgeCondition m_aJudgeInfo[2];
	std::string m_path;
	static CListManager<CJudgeZone> m_List;
};

#endif
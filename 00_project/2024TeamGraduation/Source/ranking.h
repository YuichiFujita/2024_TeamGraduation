//=============================================================================
// 
//  ランキングヘッダー [ranking.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_	// 二重インクルード防止

#include "main.h"
#include "manager.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// ランキングクラス定義
class CRanking : public CScene
{
public:

	struct SRankdata
	{
		int year;	// 年
		int month;	// 月
		int day;	// 日
		int minutes;		// 分
		int seconds;		// 秒
		int milliSeconds;	// ミリ秒
		int allrank;
		bool rankin;

		// コンストラクタ
		SRankdata() : year(0), month(0), day(0), minutes(0), seconds(0), milliSeconds(0), allrank(0), rankin(false) {}
		SRankdata(int _year, int _month, int _day, int _minnutes, int _seconds, int _milliseconds, int _allrank) : 
			year(_year), month(_month), day(_day), minutes(_minnutes), seconds(_seconds), milliSeconds(_milliseconds), allrank(_allrank), rankin(false) {}
	};

public:

	CRanking();
	~CRanking();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:

	void Load();
	void Save();
	void Sort();
	void CreateMap();	// マップ生成

};



#endif
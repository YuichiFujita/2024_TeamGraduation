//=============================================================================
// 
//  判定表示ヘッダー [judgeobj.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _JUDGE_OBJ_H_
#define _JUDGE_OBJ_H_	// 二重インクルード防止

#include "object2d.h"
#include "judge.h"

//前方宣言
class CJudgeStateFunc;

//==========================================================================
// クラス定義
//==========================================================================
// 戦闘開始クラス
class CJudgeObj : public CObject2D
{
public:

	CJudgeObj(int nPriority = 7);
	~CJudgeObj();


	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CJudgeObj* Create(const MyLib::Vector3& pos,const CJudge::JUDGE judge);
	void CreateState();

private:

	//=============================
	// 列挙型定義
	//=============================
	enum VTXTYPE
	{
		VTXTYPE_RIGHTUP = 0,	// 右上
		VTXTYPE_LEFTDW,		// 左下
		VTXTYPE_MAX
	};

	// 構造体定義
	struct SRockOnMarker
	{
		CObject2D* p2D;	// 2Dオブジェのポインタ
		float distance;		// 間隔
		int Angle;			// 向き
		float distancetimer;	// 距離時間

		SRockOnMarker() : p2D(nullptr), distance(0.0f), Angle(0), distancetimer(0.0f) {}
	};

	//=============================
	// メンバ変数
	//=============================
	SRockOnMarker m_RockOnInfo[VTXTYPE_MAX];	// マーカー情報
	CJudgeStateFunc* m_pStateFunc;	// 状態関数（判定によって変わる）
	int m_Judge;
};

#endif
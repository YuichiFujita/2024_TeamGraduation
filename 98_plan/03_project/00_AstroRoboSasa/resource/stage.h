//============================================================
//
//	ステージヘッダー [stage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CField;	// 地面クラス
class CWall;	// 壁クラス
class CScenery;	// 景色クラス
class CSky;		// 空クラス
class CLiquid;	// 液体クラス

//************************************************************
//	クラス定義
//************************************************************
// ステージクラス
class CStage
{
public:
	// 制限列挙
	enum ELimit
	{
		LIMIT_BOX = 0,	// 矩形範囲
		LIMIT_CIRCLE,	// 円範囲
		LIMIT_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CStage();

	// デストラクタ
	~CStage();

	// ステージ範囲構造体
	struct SStageLimit
	{
		ELimit mode;		// 制限モード
		D3DXVECTOR3 center;	// 中心座標
		float fNear;		// 制限位置 (前)
		float fFar;			// 制限位置 (後)
		float fRight;		// 制限位置 (右)
		float fLeft;		// 制限位置 (左)
		float fRadius;		// 制限位置 (半径)
		float fField;		// 制限位置 (地面)
		float fKillY;		// 制限位置 (削除)
	};

	// 地面構造体
	struct SField
	{
		CField **ppField;	// 地面の情報
		int nNum;			// 地面の総数
	};

	// 壁構造体
	struct SWall
	{
		CWall **ppWall;	// 壁の情報
		int nNum;		// 壁の総数
	};

	// 風景構造体
	struct SScenery
	{
		CScenery **ppScenery;	// 風景の情報
		int nNum;				// 風景の総数
	};

	// 空構造体
	struct SSky
	{
		CSky **ppSky;	// 空の情報
		int nNum;		// 空の総数
	};

	// 液体構造体
	struct SLiquid
	{
		CLiquid **ppLiquid;	// 液体の情報
		int nNum;			// 液体の総数
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);			// 更新
	void SetStageLimit(const SStageLimit& rLimit);	// ステージ範囲設定
	SStageLimit GetStageLimit(void) const;			// ステージ範囲取得

	bool LandLimitPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// 範囲外の着地判定
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);		// 位置補正
	bool CollisionKillY(const D3DXVECTOR3& rPos);					// キルY座標との当たり判定
	bool LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove);	// 地面着地 (回転考慮)
	bool IsFieldPositionRange(const D3DXVECTOR3& rPos);				// メッシュの範囲内取得 (回転考慮)
	float GetFieldPositionHeight(const D3DXVECTOR3& rPos);			// メッシュの着地位置取得 (回転考慮)

	// 静的メンバ関数
	static CStage *Create(const CScene::EMode mode);	// 生成
	static void Release(CStage *&prStage);				// 破棄

private:
	// 静的メンバ関数
	static HRESULT LoadSetup(const CScene::EMode mode, CStage *pStage);					// セットアップ
	static HRESULT LoadLimit(const char* pString, FILE *pFile, CStage *pStage);			// 範囲情報の読込
	static HRESULT LoadField(const char* pString, FILE *pFile, CStage *pStage);			// 地面情報の読込
	static HRESULT LoadWall(const char* pString, FILE *pFile, CStage *pStage);			// 壁情報の読込
	static HRESULT LoadScenery(const char* pString, FILE *pFile, CStage *pStage);		// 景色情報の読込
	static HRESULT LoadSky(const char* pString, FILE *pFile, CStage *pStage);			// 空情報の読込
	static HRESULT LoadLiquid(const char* pString, FILE *pFile, CStage *pStage);		// 液体情報の読込

	// メンバ変数
	SStageLimit	m_stageLimit;	// 範囲情報
	SField		m_field;		// 地面情報
	SWall		m_wall;			// 壁情報
	SScenery	m_scenery;		// 景色情報
	SSky		m_sky;			// 空情報
	SLiquid		m_liquid;		// 液体情報
};

#endif	// _STAGE_H_

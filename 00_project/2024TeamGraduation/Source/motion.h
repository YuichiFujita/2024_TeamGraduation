//=============================================================================
// 
//  モーションヘッダー [motion.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "motionManager.h"
#include "gamemanager.h"
#include "constans.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CModel;
class CObjectChara;

//==========================================================================
// クラス定義
//==========================================================================
// モーションクラス定義
class CMotion
{
public:

	//=============================
	// コンストラクタ, デストラクタ
	//=============================
	CMotion();
	~CMotion();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init(const std::string& file);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);


	//--------------------------
	// 再生中情報
	//--------------------------
	CMotionManager::Info GetInfo()			{ return m_vecInfo[m_nType]; }	// 情報取得
	CMotionManager::Info GetInfo(int type)	{ return m_vecInfo[type]; }		// 情報取得
	std::vector<CMotionManager::AttackInfo> GetAttackInfo() { return m_vecInfo[m_nType].AttackInfo; }	// 攻撃情報取得
	int GetType()			{ return m_nType; }				// 現在のモーションタイプ取得
	int GetOldType()		{ return m_nOldType; }			// 前回のモーションタイプ取得
	float GetAllCount()		{ return m_fAllFrame; }		// 全てのカウント取得
	float GetMaxAllCount()	{ return m_fMaxAllFrame; }	// 全てのカウントの最大値取得
	int GetMaxAllCount(int nType);	// 全てのカウント取得
	float GetFrameCount() { return m_fCntFrame; }	// フレームカウント取得
	int GetNumAll() { return m_nNumModel; }		// パーツ数取得
	int GetNumMotion() { return m_nNumMotion; }	// モーションの総数取得
	void SetNowPattern(int nPattern) { m_nPatternKey = nPattern; }		// 現在のパターン設定
	int GetNowPattern() { return m_nPatternKey; }	// 現在のパターン取得

	//--------------------------
	// フラグ
	//--------------------------
	inline int IsGetMove() { return (m_vecInfo[m_nType].nMove == 0) ? 0 : 1; }			// 移動の判定取得
	inline int IsGetMove(int nType) { return (m_vecInfo[nType].nMove == 0) ? 0 : 1; }	// 移動の判定取得
	inline bool IsFinish() { return m_bFinish; }						// 終了しているかの判定
	inline void ToggleFinish(bool bFinish) { m_bFinish = bFinish; }		// 終了しているかの判定強制切り替え
	inline bool IsGetCancelable() { return m_bCancelable; }				// キャンセル可能の判定取得
	inline bool IsGetCombiable() { return m_bCombiable; }				// コンボ可能の判定取得
	inline bool IsAttacking() { return m_bAttaking; }					// 攻撃判定中フラグ取得
	inline bool IsSpecial() { return m_vecInfo[m_nType].bSpecial; }		// スペシャル判定取得
	bool IsImpactFrame(const CMotionManager::Info& info);								// 指定した種類の情報が衝撃カウントか
	bool IsImpactFrame(int nCntAtk);									// 指定したインデックスの情報が衝撃カウントか
	bool IsAlignFrame(const CMotionManager::Info& info);								// 指定した情報がフレーム内

	//--------------------------
	// 設定
	//--------------------------
	void Set(int nType, int nStartKey = 0, bool bBlend = true, float fCntFrame = 0.0f);	// モーションの設定処理
	void ResetPose(int nType);	// ポーズのリセット

	MyLib::Vector3 GetAttackPosition(CModel** ppModel, CMotionManager::AttackInfo attackInfo);	// 攻撃の位置取得
	MyLib::Vector3 GetAttackPosition(CModel* pModel, CMotionManager::AttackInfo attackInfo);	// 攻撃の位置取得
	
	CMotionManager::Parts GetPartsOld(int nParts) { return m_pPartsOld[nParts]; }				// 過去のパーツ情報取得
	void SetPartsOld(int nParts, CMotionManager::Parts parts) { m_pPartsOld[nParts] = parts; }	// 過去のパーツ情報設定
	void SetModel(CModel** pModel, int nNumModel);	// モーションをするモデルの登録

	//=============================
	// 静的メンバ関数
	//=============================
	static CMotion* Create(const std::string& file, CObjectChara* pObjChara);

private:

	//=============================
	// 列挙型定義
	//=============================
	enum LOOP
	{
		LOOP_OFF = 0,	// ループ無し
		LOOP_ON,		// ループする
		LOOP_MAX
	};

	// メンバ関数
	void ReadText(const std::string& file);
	void LoadMotion(const std::string& file, int nMotion);
	void UpdateRotation(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio);	// 向きの更新
	void UpdateScale(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio);	// スケールの更新
	void UpdatePosition(int i, const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio, int nMaxFrame, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 位置更新
	void UpdateEntityPosition(int i, const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio, int nMaxFrame, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 本体ごと位置更新
	void UpdateVisualPosition(int i, const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio, int nMaxFrame, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 見た目のみ位置更新

	// メンバ変数
	std::vector<CMotionManager::Info> m_vecInfo;	// モーションの情報
	CMotionManager::Parts* m_pPartsOld;	// 過去の情報
	int m_nType;				// 現在のモーションの種類
	int m_nOldType;				// 前回のモーションの種類
	bool m_bLoop;				// ループするかどうか
	int m_nPatternKey;			// 何個目のキーか
	float m_fCntFrame;			// フレームのカウント
	float m_fAllFrame;			// 再生中の全てのカウント
	float m_fMaxAllFrame;		// 再生中の全てのカウントの最大値
	float m_fSlowFactor;		// 遅延係数
	bool m_bFinish;				// 終了したかどうか
	bool m_bCancelable;			// キャンセル可能か
	bool m_bCombiable;			// コンボ可能か
	bool m_bAttaking;			// 攻撃判定中フラグ

	CObjectChara* m_pObjChara;			// オブジェクトキャラクターのポインタ
	CModel** m_ppModel;					// モデルのポインタ
	int m_nNumModel;					// パーツの総数
	int m_nNumMotion;					// モーションの総数
	std::vector<int> m_vecDefaultIdx;	// デフォルトのインデックス
};

#endif
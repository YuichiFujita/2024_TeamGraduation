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
	// 構造体定義
	//=============================
	// 判定の構造体
	struct AttackInfo
	{
		int nCollisionNum;		// 当たり判定のパーツ番号
		float fRangeSize;		// 判定のサイズ
		MyLib::Vector3 Offset;	// 判定のオフセット
		int nMinCnt;			// 判定の最低カウント
		int nMaxCnt;			// 判定の最大カウント
		int nDamage;			// ダメージ
		int nInpactCnt;			// 衝撃のカウント
		bool bInpactAct;		// 衝撃カウントの行動をしたか
		bool bInpactActSet;		// 衝撃カウントの行動設定したか
		bool bEndAtk;			// 攻撃の終了フラグ
	};

	// パーツ情報
	struct Parts
	{
		MyLib::Vector3 rot;			// 向き
		MyLib::Vector3 rotDest;		// 目標の向き
		MyLib::Vector3 pos;			// 位置
		MyLib::Vector3 posDest;		// 目標の位置
		MyLib::Vector3 posOrigin;	// 位置の原点
		MyLib::Vector3 scale;		// スケール

		Parts() : scale(1.0f) {}
	};

	// キー情報
	struct Key
	{
		std::vector<Parts> aParts;	// パーツ情報
		int nFrame;					// 再生フレーム
		float fRotMove;				// 移動の向き
	};

	// 全体の情報
	struct Info
	{
		std::vector<Key> aKey;	// キー情報
		int nNumKey;			// キーの数
		int nLoop;				// ループ判定
		int nMove;				// 移動判定
		bool bSpecial;			// スペシャル判定
		int nNumAttackInfo;		// 攻撃情報の数
		int nCancelableFrame;	// キャンセル可能フレーム
		int nCombolableFrame;	// コンボ可能フレーム
		std::vector<AttackInfo> AttackInfo;	// 当たり判定用

		// コンストラクタ
		Info() : bSpecial(false), nCancelableFrame(-1), nCombolableFrame(-1) {}
	};

	CMotion();
	~CMotion();

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	static CMotion* Create(const std::string& file, CObjectChara* pObjChara);

	//--------------------------
	// 再生中情報
	//--------------------------
	Info GetInfo()			{ return m_vecInfo[m_nType]; }	// 情報取得
	Info GetInfo(int type)	{ return m_vecInfo[type]; }		// 情報取得
	std::vector<AttackInfo> GetAttackInfo() { return m_vecInfo[m_nType].AttackInfo; }	// 攻撃情報取得
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
	inline int IsGetMove() { return m_vecInfo[m_nType].nMove; }			// 移動の判定取得
	inline int IsGetMove(int nType) { return m_vecInfo[nType].nMove; }	// 移動の判定取得
	inline bool IsFinish() { return m_bFinish; }						// 終了しているかの判定
	inline void ToggleFinish(bool bFinish) { m_bFinish = bFinish; }		// 終了しているかの判定強制切り替え
	inline bool IsGetCancelable() { return m_bCancelable; }				// キャンセル可能の判定取得
	inline bool IsGetCombiable() { return m_bCombiable; }				// コンボ可能の判定取得
	inline bool IsAttacking() { return m_bAttaking; }					// 攻撃判定中フラグ取得
	inline bool IsSpecial() { return m_vecInfo[m_nType].bSpecial; }		// スペシャル判定取得
	bool IsImpactFrame(int nCntAtk);									// 指定したインデックスの情報が衝撃カウントか

	//--------------------------
	// 設定
	//--------------------------
	void Set(int nType, int nStartKey = 0, bool bBlend = true);		// モーションの設定処理
	void ResetPose(int nType);					// ポーズのリセット

	MyLib::Vector3 GetAttackPosition(CModel** ppModel, AttackInfo attackInfo);	// 攻撃の位置取得
	MyLib::Vector3 GetAttackPosition(CModel* pModel, AttackInfo attackInfo);	// 攻撃の位置取得
	
	Parts GetPartsOld(int nParts) { return m_pPartsOld[nParts]; }				// 過去のパーツ情報取得
	void SetPartsOld(int nParts, Parts parts) { m_pPartsOld[nParts] = parts; }	// 過去のパーツ情報設定
	void SetModel(CModel** pModel, int nNumModel);	// モーションをするモデルの登録

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
	void UpdateRotation(int i);	// 向きの更新

	// メンバ変数
	std::vector<Info> m_vecInfo;	// モーションの情報
	Parts* m_pPartsOld;	// 過去の情報
	int m_nType;				// 現在のモーションの種類
	int m_nOldType;				// 前回のモーションの種類
	bool m_bLoop;				// ループするかどうか
	int m_nPatternKey;			// 何個目のキーか
	float m_fCntFrame;			// フレームのカウント
	float m_fAllFrame;			// 全てのカウント
	float m_fMaxAllFrame;		// 全てのカウントの最大値
	float m_fSlowFactor;		// 遅延係数
	bool m_bFinish;				// 終了したかどうか
	bool m_bCancelable;			// キャンセル可能か
	bool m_bCombiable;			// コンボ可能か
	bool m_bAttaking;			// 攻撃判定中フラグ

	CObjectChara* m_pObjChara;	// オブジェクトキャラクターのポインタ
	CModel** m_ppModel;			// モデルのポインタ
	int m_nNumModel;			// パーツの総数
	int m_nNumMotion;			// モーションの総数
	static std::vector<std::string> m_sTextFile;	// テキストファイル名
	static int m_nNumLoad;	// 読み込んだ数
	static std::vector<int> m_nNumLoadData;				// モーション毎のデータ数
	static std::vector<std::vector<CMotion::Info>> m_vecLoadData;		// モーションの読み込み情報
};

#endif
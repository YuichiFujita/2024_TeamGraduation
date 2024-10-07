//=============================================================================
// 
//  モーションヘッダー [motion.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_	// 二重インクルード防止

#include "gamemanager.h"
#include "constans.h"

class CModel;
class CObjectChara;

#define MAX_MOTION	(28)	// モーションの最大数
#define MAX_KEY		(24)	// キーの最大数
#define MAX_PARTS	(32)	// パーツの最大数

//==========================================================================
// クラス定義
//==========================================================================
// モーションクラス定義
class CMotion
{
public:

	// 判定の構造体
	typedef struct
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
		bool bOnlyOneTime;		// 1度だけかの判定
		CGameManager::AttackType AtkType;	// 攻撃の種類
	}AttackInfo;

	// モーションの構造体
	typedef struct
	{
		MyLib::Vector3 rot;		// 向き
		MyLib::Vector3 rotDest;	// 目標の向き
		MyLib::Vector3 pos;		// 位置
		MyLib::Vector3 posDest;	// 目標の位置
		MyLib::Vector3 posOrigin;	// 位置の原点
		MyLib::Vector3 scale;		// スケール
	}Parts;

	typedef struct
	{
		Parts aParts[MAX_PARTS];
		int nFrame;				// 再生フレーム
		float fRotMove;			// 移動の向き
	}Key;

	typedef struct
	{
		Key aKey[MAX_KEY];
		int nNumKey;			// キーの数
		int nLoop;				// ループ判定
		int nMove;				// 移動判定
		int nNumAttackInfo;		// 攻撃情報の数
		int nCancelableFrame;	// キャンセル可能フレーム
		int nCombolableFrame;	// コンボ可能フレーム
		AttackInfo *AttackInfo[MAX_PARTS];	// 当たり判定用
	}Info;

	CMotion();
	~CMotion();

	HRESULT Init();
	void Uninit();
	void Update(float fBuff = 1.0f);
	static CMotion *Create(const std::string& file, CObjectChara* pObjChara);
	void Set(int nType, bool bBlend = true);	// モーションの設定処理
	int GetType();							// 現在のモーションタイプ取得
	int GetOldType();						// 前回のモーションタイプ取得
	bool IsFinish();						// 終了しているかの判定
	void ToggleFinish(bool bFinish);			// 終了しているかの判定切り替え
	bool IsGetCancelable() { return m_bCancelable; }	// キャンセルの判定取得
	bool IsGetCombiable() { return m_bCombiable; }	// キャンセルの判定取得
	bool IsBeforeInAttack() { return m_bBeforeInAction; }		// 攻撃前フラグ取得
	bool IsAttacking() { return m_bAttaking; }					// 攻撃判定中フラグ取得
	bool IsAllAfterAttack() { return m_bAllAfterAttack; }		// 全ての攻撃後フラグ取得
	void SetSlowFactor(float fFactor);			// 遅延係数の設定

	bool IsImpactFrame(AttackInfo attackInfo);	// 衝撃のフレームかどうか取得
	MyLib::Vector3 GetAttackPosition(CModel **ppModel, AttackInfo attackInfo);	// 攻撃の位置取得
	MyLib::Vector3 GetAttackPosition(CModel *pModel, AttackInfo attackInfo);	// 攻撃の位置取得
	float GetAllCount();	// 全てのカウント取得
	float GetMaxAllCount() { return m_fMaxAllFrame; }
	int GetMaxAllCount(int nType);	// 全てのカウント取得
	void SetFrameCount(float fCnt);					// フレームカウント設定
	float GetFrameCount() { return m_fCntFrame; }	// フレームカウント取得
	int GetNumAll() { return m_nNumModel; }		// パーツ数取得
	int GetNumMotion() { return m_nNumMotion; }	// モーションの総数取得
	void SetNowPattern(int nPattern) { m_nPatternKey = nPattern; }		// 現在のパターン設定
	int GetNowPattern() { return m_nPatternKey; }	// 現在のパターン取得
	void AddNumAttackInfo(int nType);	// 攻撃情報の総数加算
	void SubNumAttackInfo(int nType);	// 攻撃情報の総数減算

	Parts GetPartsOld(int nParts);
	void SetPartsOld(int nParts, Parts parts);
	void SetInfo(Info info);	// モーション情報の登録
	void SetInfoData(int nType, Info info);	// モーション情報の登録
	void SetAttackInfo(AttackInfo info);	// 攻撃情報の登録
	Info GetInfo(int nType);	// モーション情報の取得
	Info *GetInfoPtr(int nType);	// モーション情報の取得
	void SetInfoSave(int nType, int nKey, int nParts, MyLib::Vector3 rot);	// モーション情報の登録
	void ChangeKeyNum(int nType, int nNum);	// キー総数変更
	void ChangeKeyFrame(int nType, int nKey, int nNum);	// キーフレーム変更
	void ChangeAttackInfo(int nType, int nIdx, AttackInfo info);	// 攻撃情報変更
	int IsGetMove(int nType);	// 移動の判定取得
	void SetModel(CModel **pModel, int nNumModel);	// モーションをするモデルの登録
	void ResetPose(int nType);	// ポーズのリセット

private:
	// 列挙型定義
	typedef enum
	{
		LOOP_OFF = 0,	// ループ無し
		LOOP_ON,		// ループする
		LOOP_MAX
	}LOOP;

	// メンバ関数
	void ReadText(const std::string& file);
	void LoadMotion(const std::string& file, int nMotion);

	// メンバ変数
	Info* m_pInfo;	// モーションの情報
	Parts* m_pPartsOld;	// 過去の情報
	int m_nNumAll;				// モーションの総数
	int m_nType;				// 現在のモーションの種類
	int m_nOldType;				// 前回のモーションの種類
	bool m_bLoop;				// ループするかどうか
	int m_nPatternKey;			// 何個目のキーか
	float m_fCntFrame;			// フレームのカウント
	float m_fCntAllFrame;		// 全てのカウント
	float m_fMaxAllFrame;		// 全てのカウントの最大値
	float m_fSlowFactor;		// 遅延係数
	bool m_bFinish;				// 終了したかどうか
	bool m_bCancelable;			// キャンセル可能か
	bool m_bCombiable;			// コンボ可能か
	bool m_bBeforeInAction;		// 攻撃前フラグ
	bool m_bAttaking;			// 攻撃判定中フラグ
	bool m_bAllAfterAttack;		// 全ての攻撃後フラグ

	CObjectChara *m_pObjChara;	// オブジェクトキャラクターのポインタ
	CModel **m_ppModel;			// モデルのポインタ
	int m_nNumModel;			// パーツの総数
	int m_nNumMotion;			// モーションの総数
	static std::vector<std::string> m_sTextFile;	// テキストファイル名
	static int m_nNumLoad;	// 読み込んだ数
	static std::vector<int> m_nNumLoadData;				// モーション毎のデータ数
	static std::vector<CMotion::Info> m_aLoadData[MAX_MOTION];	// モーションの読み込み情報
	static std::vector<AttackInfo> m_aLoadAttackData[MAX_MOTION][MAX_PARTS];	// 攻撃の読み込み情報
};

#endif
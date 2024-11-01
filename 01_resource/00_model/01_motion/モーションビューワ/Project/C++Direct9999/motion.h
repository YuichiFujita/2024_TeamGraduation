//=============================================================================
// 
//  モーションヘッダー [motion.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"

class CModel;
class CObjectChara;

#define MAX_MOTION	(32)	// モーションの最大数
#define MAX_KEY		(24)	// キーの最大数
#define MAX_PARTS	(48)	// パーツの最大数

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
		D3DXVECTOR3 Offset;		// 判定のオフセット
		int nMinCnt;			// 判定の最低カウント
		int nMaxCnt;			// 判定の最大カウント
		int nDamage;			// ダメージ
		int nInpactCnt;			// 衝撃のカウント
	}AttackInfo;

	// モーションの構造体
	typedef struct
	{
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 rotDest;	// 目標の向き
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 posDest;	// 目標の位置
		D3DXVECTOR3 posOrigin;	// 位置の原点
		D3DXVECTOR3 scale;		// スケール
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
		std::string filename;	// ファイル名
		int nNumKey;			// キーの数
		int nLoop;				// ループ判定
		int nMove;				// 移動判定
		int nNumAttackInfo;		// 攻撃情報の数
		int nMaxAllFrame;		// 全てのカウント
		int nCancelableFrame;	// キャンセル可能フレーム
		int nCombolableFrame;	// コンボ可能フレーム
		AttackInfo *AttackInfo[MAX_PARTS];	// 当たり判定用
	}Info;

	CMotion();
	~CMotion();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CMotion *Create(void);
	void Set(int nType);	// モーションの設定処理
	int GetType(void);		// 現在のモーションタイプ取得
	bool IsFinish(void);	// 終了しているかの判定
	bool IsGetCancelable(void);	// キャンセルの判定取得

	int GetAllCount(void);	// 全てのカウント取得
	int GetFrameCount(void) { return m_nCntFrame; }	// フレームカウント取得
	int GetNumAll(void) { return m_nNumModel; }		// パーツ数取得
	int GetNumMotion(void) { return m_nNumMotion; }	// モーションの総数取得
	int GetNowPattern(void) { return m_nPatternKey; }	// 現在のパターン取得
	void AddNumAttackInfo(int nType);	// 攻撃情報の総数加算
	void SubNumAttackInfo(int nType);	// 攻撃情報の総数減算

	void ReadText(const char *pTextFile);
	void SetInfo(Info info);	// モーション情報の登録
	void SetAttackInfo(AttackInfo info);	// 攻撃情報の登録
	void ResetMoveData(int nType, int nKey);	// 移動情報のリセット
	Info GetInfo(int nType);	// モーション情報の取得
	void SetInfoSave(int nType, int nKey, int nParts, D3DXVECTOR3 rot);	// モーション情報の登録
	void SetInfoSavePos(int nType, int nKey, int nParts, D3DXVECTOR3 pos);	// モーション情報の登録
	void SetInfoSaveScale(int nType, int nKey, int nParts, D3DXVECTOR3 scale);	// モーション情報の登録
	void ChangeKeyNum(int nType, int nNum);	// キー総数変更
	void ChangeKeyFrame(int nType, int nKey, int nNum);	// キーフレーム変更
	void ChangeCancelableFrame(int nType, int nFrame);	// キャンセル可能フレーム変更
	void ChangeCombolableFrame(int nType, int nFrame);	// コンボ可能フレーム変更
	void ChangeAttackInfo(int nType, int nIdx, AttackInfo info);	// 攻撃情報変更
	int IsGetMove(int nType);		// 移動の判定取得
	void SetModel(CModel **pModel, int nNumModel, CObjectChara *pObjChara);	// モーションをするモデルの登録

	std::string GetMotionFilename(int nType) { return m_sMotionFile[nType]; }
	const std::vector<std::string>& GetMotionFilenameVector() { return m_sMotionFile; }
	std::string* GetMotionFilenameVectorPtr() { return m_sMotionFile.data(); }
private:
	// 列挙型定義
	typedef enum
	{
		LOOP_OFF = 0,	// ループ無し
		LOOP_ON,		// ループする
		LOOP_MAX
	}LOOP;

	void LoadMotion(const std::string text, int nMotion);

	Info m_aInfo[MAX_MOTION];	// モーションの情報
	Parts aPartsOld[MAX_PARTS];	// 過去の情報
	int m_nNumAll;				// モーションの総数
	int m_nType;				// 現在のモーションの種類
	int m_nOldType;				// 前回のモーションの種類
	bool m_bLoop;				// ループするかどうか
	int m_nPatternKey;			// 何個目のキーか
	int m_nCntFrame;			// フレームのカウント
	int m_nCntAllFrame;			// 全てのカウント
	bool m_bFinish;				// 終了したかどうか
	bool m_bCancelable;			// キャンセル可能か

	CObjectChara *m_pObjChara;	// オブジェクトキャラクターのポインタ
	CModel **m_ppModel;			// モデルのポインタ
	int m_nNumModel;			// パーツの総数
	int m_nNumMotion;			// モーションの総数
	std::vector<std::string> m_sMotionFile;	// モーションファイル名
	static int m_nNumLoad;	// 読み込んだ数
	static std::vector<std::string> m_sTextFile;	// テキストファイル名
	static std::vector<int> m_nNumLoadData;		// モーション毎のデータ数
	static std::vector<Info> m_aLoadData[MAX_MOTION];	// モーションの読み込み情報
	static std::vector<AttackInfo> m_aLoadAttackData[MAX_MOTION][MAX_PARTS];	// 攻撃の読み込み情報
};



#endif
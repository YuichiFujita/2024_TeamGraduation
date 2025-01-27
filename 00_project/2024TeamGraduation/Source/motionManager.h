//=============================================================================
// 
//  モーションマネージャーヘッダー [motionManager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MOTIONMANAGER_H_
#define _MOTIONMANAGER_H_	// 二重インクルード防止

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
// モーションマネージャークラス
class CMotionManager
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum LOOP
	{
		LOOP_OFF = 0,	// ループ無し
		LOOP_ON,		// ループする
		LOOP_MAX
	};

	//=============================
	// 構造体定義
	//=============================
	// 揃え情報
	struct AlignInfo
	{
		int nFrame;				// このフレーム
		int nExtensionFrame;	// 猶予フレーム
		bool bSet;				// 設定時

		// コンストラクタ
		AlignInfo() : nFrame(0), nExtensionFrame(0), bSet(false) {}
	};

	// 判定の構造体
	struct AttackInfo
	{
		int nCollisionNum;		// 当たり判定のパーツ番号
		float fRangeSize;		// 判定のサイズ
		MyLib::Vector3 Offset;	// 判定のオフセット
		int nMinCnt;			// 判定の最低カウント
		int nMaxCnt;			// 判定の最大カウント
		float fCntRatio;		// 判定カウントの割合
		int nDamage;			// ダメージ
		int nInpactCnt;			// 衝撃のカウント
		bool bInpactAct;		// 衝撃カウントの行動をしたか
		bool bInpactActSet;		// 衝撃カウントの行動設定したか
		bool bAtkking;			// 攻撃フラグ
		bool bEndAtk;			// 攻撃の終了フラグ
		AlignInfo AlignInfo;	// 揃え情報
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

	// 読み込み情報保存情報
	struct SLoadInfo
	{
		std::vector<int> vecDefaultIdx;					// デフォルトのインデックス
		std::string sTextFile;							// テキストファイル名
		int nNumLoadData;								// モーション毎のデータ数
		std::vector<CMotionManager::Info> vecLoadData;	// モーションの読み込み情報
	};


	//=============================
	// コンストラクタ, デストラクタ
	//=============================
	CMotionManager();
	~CMotionManager();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();	// 初期化処理
	void Uninit();	// 終了処理
	void Reset();	// リセット

	//--------------------------
	// 再生中情報
	//--------------------------
	SLoadInfo Load(const std::string& setupfile);	// 読み込み処理

	//=============================
	// 静的メンバ関数
	//=============================
	static CMotionManager* Create();	// 生成
	static CMotionManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:

	Info LoadMotion(const std::string& file, int nMotion);

	// メンバ変数
	std::vector<SLoadInfo> m_LoadInfo;	// 読み込みデータ
	static CMotionManager* m_pThisPtr;	// 自身のポインタ
};

#endif
//=============================================================================
//
// カメラモーション処理 [camera_motion.h]
// Author : 相馬靜雅
// Adder  : 藤田勇一
//
//=============================================================================
#ifndef _CAMERA_MOTION_H_		// このマクロ定義がされていなかったら
#define _CAMERA_MOTION_H_		// 二重インクルード防止のマクロを定義する

#include "scene.h"
#include "camera_motion_trigger.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラモーションクラス
class CCameraMotion
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum MOTION
	{
		MOTION_PASS = 0,		// パスモーション
		MOTION_SPAWN,			// 登場演出モーション
		MOTION_SPECIAL_HYPE,	// スペシャル盛り上げモーション
		MOTION_KAMEHAMEHA,		// かめはめ波モーション
		MOTION_MAX
	};

	// 補正種類
	enum EEasing
	{
		Linear = 0,
		EaseIn,
		EaseOut,
		EaseInOut,
		EaseInExpo,
		EaseOutExpo,
		EaseInOutExpo,
		MAX
	};

	//=============================
	// 構造体定義
	//=============================
	// キー情報
	struct MotionKey
	{
		MyLib::Vector3 posRDest;	// 目標の注視点
		MyLib::Vector3 rotDest;		// 目標の向き
		float distance;				// 距離
		float playTime;				// 再生フレーム
		EEasing easeType;			// イージングの種類

		MotionKey() : posRDest(0.0f), rotDest(0.0f), distance(0.0f), playTime(1.0f), easeType(EEasing::Linear) {}
	};

	// モーション情報
	struct MotionInfo
	{
		std::vector<MotionKey> Key;	// キー情報
		std::vector<float> trigger;	// トリガー

		MotionInfo() : Key() {}
	};

	// エディット情報
	struct EditInfo
	{
		int motionIdx;			// モーションインデックス
		int keyIdx;				// キーインデックス
		int triggerIdx;			// トリガーインデックス
		float playRatio;		// 再生割合
		bool bSlide;			// スライド中判定
		MyLib::Vector3 offset;	// オフセット
		MotionInfo motionInfo;	// モーション情報
		MotionKey Key_copyData;	// キーのコピーデータ

		EditInfo() : motionIdx(0), keyIdx(0), triggerIdx(0), playRatio(0.0f), bSlide(false), motionInfo(),
			Key_copyData(MotionKey()) {}
	};

	// トリガー情報
	struct TriggerInfo
	{
		bool bTrigger;	// トリガー判定
		int nIdx;		// インデックス

		TriggerInfo() : bTrigger(false), nIdx(0) {}
		TriggerInfo(bool _bTrigger, int _nIdx) : bTrigger(_bTrigger), nIdx(_nIdx) {}
	};

	CCameraMotion();
	~CCameraMotion();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();		// 初期化
	void LoadText();	// テキスト読み込み
	void Uninit();		// 終了
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新

	TriggerInfo GetTrigger();	// トリガー判定取得
	void SetMotion				// モーション設定
	(
		int nMotion,			// モーション種類
		bool bInverse = false,	// 反転フラグ
		bool bPos = true,		// 位置動作フラグ
		bool bRot = true,		// 向き動作フラグ
		bool bDis = true		// 距離動作フラグ
	);

	// 情報操作
	inline void SetPosition(const MyLib::Vector3& pos)	{ m_pos = pos; }	// 位置設定
	inline MyLib::Vector3 GetPosition()					{ return m_pos; }	// 位置取得
	inline int GetNowMotionIdx()			{ return m_nNowMotionIdx; }
	inline void SetFinish(bool bFinish)		{ m_bFinish = bFinish; }
	inline bool IsFinish()	{ return m_bFinish; }
	inline bool IsEdit()	{ return m_bEdit; }

	inline void SetEnablePause(bool bPause)			{ m_bPause = bPause; }			// ポーズ判定
	inline void SetEnableSystemPause(bool bPause)	{ m_bSystemPause = bPause; }	// システムポーズ判定
	inline bool IsPause()							{ return m_bPause; }			// ポーズ判定

	inline int GetNowTriggerIdx() { return m_nNowTriggerIdx; }
	inline int GetNowKeyIdx() { return m_nNowKeyIdx; }
	inline int GetNowKeyMax() { return m_vecMotionInfo[m_nNowMotionIdx].Key.size(); }
	inline int GetKeyMax(const MOTION motion) { return m_vecMotionInfo[motion].Key.size(); }

	MotionKey GetKeyData(const MOTION motion, const int nKeyIdx);	// キー情報取得
	float GetWholeCurTimer();			// 現在の全体時間取得
	float GetWholeMaxTimer();			// 最大の全体時間取得
	bool IsImpactFrame(int nCntAtk);	// 指定したインデックスの情報が衝撃カウントか

	// 静的関数
	static CCameraMotion* Create();	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	// モーションリスト
	typedef void(CCameraMotion::*MOTION_FUNC)();
	static MOTION_FUNC m_MotionFunc[];

#if 0
	// vec3線形補正リスト
	typedef MyLib::Vector3(*VEC3_EASING_FUNC)(const MyLib::Vector3&, const MyLib::Vector3&, float);
	static VEC3_EASING_FUNC m_Vec3EasingFunc[];

	// float線形補正リスト
	typedef float(*FLOAT_EASING_FUNC)(float, float, float);
	static FLOAT_EASING_FUNC m_FloatEasingFunc[];
#else
	// vec3線形補正リスト
	typedef MyLib::Vector3(*VEC3_EASING_FUNC)(const MyLib::Vector3&, const MyLib::Vector3&, float, float, float);
	static VEC3_EASING_FUNC m_Vec3EasingFunc[];

	// float線形補正リスト
	typedef float(*FLOAT_EASING_FUNC)(float, float, float, float, float);
	static FLOAT_EASING_FUNC m_FloatEasingFunc[];
#endif

	//=============================
	// メンバ関数
	//=============================
	// セーブ＆ロード
	void SaveMotion(const std::string& filename, const MotionInfo& info);
	void LoadMotion(const std::string& filename);	// モーション読み込み

	// モーション
	void MotionPass();			// パス
	void MotionSpawn();			// 登場演出
	void MotionSpecial();		// スペシャル盛り上げ
	void MotionKamehameha();	// かめはめ波

	// エディット用
	void UpdateEdit();		// エディット更新
	void SliderPlay();		// スライド再生
	void ChangeMotion();	// モーション切り替え
	void ChangeKey();		// キー切り替え
	void EditMotion();		// モーションエディット
	void EditKey();			// キーエディット
	void EditTrigger();		// トリガーエディット

	// その他
	void TriggerMoment();	// トリガー判定の瞬間

	//=============================
	// メンバ変数
	//=============================
	std::vector<MotionInfo> m_vecMotionInfo;	// モーション情報
	std::vector<std::string> m_MotionFileName;	// モーションファイル名
	std::string m_PathName;	// パス名
	MyLib::Vector3 m_pos;	// 位置
	int m_nNowMotionIdx;	// 現在のモーションインデックス
	int m_nNowKeyIdx;		// 現在のキーインデックス
	int m_nNowTriggerIdx;	// 現在のトリガーインデックス
	float m_fMotionTimer;	// モーションタイマー
	float m_fTriggerTimer;	// トリガータイマー
	bool m_bFinish;			// 終了判定
	bool m_bEdit;			// エディット使用中か
	bool m_bPause;			// ポーズ判定
	bool m_bSystemPause;	// システムポーズ判定
	bool m_bTrigger;		// トリガー判定
	bool m_bMovePos;		// 位置動作フラグ
	bool m_bMoveRot;		// 向き動作フラグ
	bool m_bMoveDis;		// 距離動作フラグ
	bool m_bInverse;		// 反転フラグ
	EditInfo m_EditInfo;	// エディット情報
	CCameraMotion_Trigger* m_pCameraMotion_Trigger[MOTION_MAX];
};

#endif
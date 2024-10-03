//=============================================================================
//
// カメラモーション処理 [camera_motion.h]
// Author : 相馬靜雅
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
		MOTION_PASS = 0,	// パスモーション
		MOTION_GOAL,		// ゴール
		MOTION_GOALBAG,		// ゴールフット橋
		MOTION_RESULT,		// リザルト
		MOTION_MAX
	};

	// 補正種類
	enum EASING
	{
		Linear = 0,
		EaseIn,
		EaseOut,
		EaseInOut,
		MAX
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
	void Update();		// 更新
	void SetMotion(int motion, EASING EasingType);	// モーション設定

	// 情報操作
	void SetPosition(const MyLib::Vector3& pos) { m_pos = pos; }	// 位置設定
	MyLib::Vector3 GetPosition() { return m_pos; }					// 位置取得
	int GetNowMotionIdx() { return m_nNowMotionIdx; }
	void SetFinish(bool bFinish) { m_bFinish = bFinish; }
	bool IsFinish() { return m_bFinish; }
	bool IsEdit() { return m_bEdit; }

	void SetEnablePause(bool bPause) { m_bPause = bPause; }	// ポーズ判定
	void SetEnableSystemPause(bool bPause) { m_bSystemPause = bPause; }	// システムポーズ判定
	bool IsPause() { return m_bPause; }		// ポーズ判定

	TriggerInfo GetTrigger();	// トリガー判定取得

	int GetNowKeyIdx() { return m_nNowKeyIdx; }
	int GetNowKeyMax() { return m_vecMotionInfo[m_nNowMotionIdx].Key.size(); }

	// 静的関数
	static CCameraMotion* Create();	// 生成処理

private:

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

		MotionKey() : posRDest(0.0f), rotDest(0.0f), distance(0.0f), playTime(1.0f) {}
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

	//=============================
	// 関数リスト
	//=============================
	// モーションリスト
	typedef void(CCameraMotion::*MOTION_FUNC)();
	static MOTION_FUNC m_MotionFunc[];

	//=============================
	// メンバ関数
	//=============================
	// セーブ＆ロード
	void SaveMotion(const std::string& filename, const MotionInfo& info);
	void LoadMotion(const std::string& filename);	// モーション読み込み

	// モーション
	void MotionPass();		// パス
	void MotionGoal();		// ゴール
	void MotionGoalBag();	// ゴールフット橋

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
	EASING m_EasingType;	// 補正の種類
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
	EditInfo m_EditInfo;	// エディット情報

	CCameraMotion_Trigger* m_pCameraMotion_Trigger[MOTION_MAX];
};

#endif
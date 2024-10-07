//=============================================================================
// 
//  チェックポイント[checkpoint.h]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================

#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_		// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"

class CObjectBillboard;
class CEffekseerObj;

//==========================================================================
// クラス定義
//==========================================================================
//チェックポイントクラス
class CCheckpoint : public CObjectX
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum State
	{
		SWING = 0,
		ROTATE,
		MAX
	};

	CCheckpoint(int nPriority = 6);
	~CCheckpoint();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// メンバ関数
	void Kill();		// 削除
	float GetLength() { return m_fLength; }	// 距離取得
	void SetLength(const float length);	// 距離設定
	float GetPassedTime() { return m_fPassedTime; }

	//=============================
	// 静的関数
	//=============================
	static CCheckpoint* Create(const float length);
	static void Load(const std::string filename);
	static int GetSaveID() { return m_nSaveID; }	// セーブID取得
	static void ResetSaveID();	// セーブIDリセット
	static CListManager<CCheckpoint> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCheckpoint::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void StateSwing();	// ゆらゆら
	void StateRotate();	// 回転

	// その他
	void CreateEffect();	// エフェクト生成
	void CreateWaterEffect(int max);	// 水エフェクト生成

	//=============================
	// メンバ変数
	//=============================
	State m_state;					// 状態
	float m_fStateTime;				// 状態カウンター
	float m_fLength;				// 距離
	float m_fPassedTime;			// 通過したときの時間
	bool m_bIsPassed;				// 通過したかどうか
	MyLib::Vector3 m_DestRot_Old;	// 目標の向き
	MyLib::Vector3 m_DestRot;		// 目標の向き
	CObjectBillboard* m_pEffect;	// エフェクトのポインタ
	CEffekseerObj* m_pEffekseerObj;	// エフェクシアのオブジェクト
	static CListManager<CCheckpoint> m_List;	// リスト

	static int m_nAll;		// 総数
	static int m_nSaveID;	// save id
	int m_MyIndex;			// 自分のID
};


#endif
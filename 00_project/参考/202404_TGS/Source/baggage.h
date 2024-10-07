//=============================================================================
// 
//  荷物ヘッダー [baggage.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _BAGGAGE_H_
#define _BAGGAGE_H_		// 二重インクルード防止

#include "objectQuaternion.h"
#include "listmanager.h"

class CObject3D;
class CEffekseerObj;

//==========================================================================
// クラス定義
//==========================================================================
// 荷物クラス
class CBaggage : public CObjectQuaternion
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	enum TYPE
	{
		TYPE_CLOTH = 0,	// 布
		TYPE_TMP_YMDHOUSE,
		TYPE_TMP_FLOWER,
		TYPE_MAX
	};

	// 状態
	enum STATE
	{
		STATE_NONE = 0,		// 通常
		STATE_DAMAGE,		// ダメージ
		STATE_DEAD,			// 死亡
		STATE_APPEARANCE_WAIT,	// 出現
		STATE_APPEARANCE,	// 出現
		STATE_PASS,			// パス
		STATE_GOAL,			// ゴール
		STATE_SEND,			// 届ける
		STATE_RETURN,		// 反射x
		STATE_RECEIVE,		// receive
		STATE_FALL,			// 矯正落下
		STATE_MAX
	};

	//=============================
	// 荷物情報
	//=============================
	struct SBaggageInfo
	{
		std::string path;	// モデルファイルパス
		float weight;		// 重さ
		float width;		// ぶれ幅
		float cycle;		// 周期
		int life;			// 体力

		SBaggageInfo() : path(""), weight(0.0f), width(0.0f), cycle(0.0f), life(0) {}
	};

	CBaggage(int nPriority = 4);
	~CBaggage();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	void Kill();		// 削除
	void UIDraw();
	void SetForce(const MyLib::Vector3& power) { m_force = power; }				// 力設定
	void AddForce(const MyLib::Vector3& power, const MyLib::Vector3& ActPos);	// 力追加
	void SetModelPass(const std::string path) { m_baggageInfo.path = path; }
	void SetCycle(const float& deviation) { m_baggageInfo.cycle = deviation; }
	void SetWidth(const float& deviWidth) { m_baggageInfo.width = deviWidth; }
	void SetWeight(const float& weight) { m_baggageInfo.weight = weight; }
	bool IsLand() { return m_bLand; }	// 着地判定
	bool IsHit() { return m_bHit; }		// Hit()での判定を返す
	bool IsEnd() { return m_bEnd; }		// 終了判定を返す
	bool IsFall() { return m_bfall; }	// 落下判定
	void SetIsFall(bool fall) { m_bfall = fall; }	// 落下判定

	MyLib::Vector3 GetVeloRot() { return m_velorot; }	// 回転速度
	void SetVeloRot(const MyLib::Vector3& velorot) { m_velorot = velorot; }
	void Reset();						// 判定リセット

	void SetAwayStartPosition(const MyLib::Vector3& pos) { m_posAwayStart = pos; }	// 吹っ飛びスタート地点
	MyLib::Vector3 GetAwayStartPosition() { return m_posAwayStart; }				// 吹っ飛びスタート地点


	int GetMapBlock() { return m_nMapBlock; }		// マップのブロック取得

	// 状態系
	void SetState(STATE state);				// 状態設定
	STATE GetState() { return m_state; }	// 状態取得

	//=============================
	// 静的関数
	//=============================
	static CBaggage* Create(TYPE type, SBaggageInfo info);
	static CListManager<CBaggage> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(CBaggage::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];
	
	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateDamage();		// ダメージ
	void StateDead();		// 死亡
	void StateAppearance_Wait();	// 出現待ち
	void StateAppearance();	// 出現
	void StatePass();		// パス
	void StateGoal();		// ゴール
	void StateSend();		// 届ける
	void StateReturn();		// 反射
	void StateReceive();	// 反射
	void StateFall();

	// その他
	void Controll();	// 操作
	bool Hit();			// 障害物判定
	void HitItem();		// アイテムとの判定
	void DebugTransform();	// デバッグ時変形

	//=============================
	// メンバ変数
	//=============================
	TYPE m_type;				// 種類
	STATE m_state;				// 状態
	float m_fStateTimer;		// 状態タイマー
	float m_fWeight;			// 重さ
	MyLib::Vector3 m_force;		// 力
	MyLib::Vector3 m_velorot;	// 回転速度
	MyLib::Vector3 m_posAwayStart;		// 吹っ飛びスタート地点
	bool m_bLand;				// 着地判定
	bool m_bHit;				// 障害物衝突判定
	bool m_bEnd;				// 終了判定
	SBaggageInfo m_baggageInfo;
	int m_nMapBlock;			// マップのブロック
	int m_nLife;				// 体力
	bool m_bfall;				// 落下判定
	float m_fDeviation;
	static CListManager<CBaggage> m_List;	// リスト
	CObject3D* m_p3D;
	CEffekseerObj* m_pEffekseerObj;		// エフェクトシアオブジェクト
};


#endif
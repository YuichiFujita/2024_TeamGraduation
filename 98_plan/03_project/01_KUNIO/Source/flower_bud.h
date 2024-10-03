//=============================================================================
// 
//  つぼみヘッダー [flower_bud.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _FLOWER_BUD_H_
#define _FLOWER_BUD_H_	// 二重インクルード防止

#include "objectX.h"
#include "multinumber.h"

//==========================================================================
// クラス定義
//==========================================================================
// つぼみクラス定義
class CFlowerBud : public CObjectX
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_WAIT = 0,		// 待機
		STATE_CHARGE,		// チャージ
		STATE_FLOWERING,	// 開花
		STATE_MAX
	};

	CFlowerBud(int nPriority = 6);
	~CFlowerBud();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	void SetSatate(STATE state) { m_state = state; }
	void SetCurrentPollen(int maxPollen, int pollen);
	int GetSpawnNum() { return m_nSpawnNum; }

	static CFlowerBud* Create(const MyLib::Vector3& pos, int maxPollen, int currentPollen);	// 生成
	static CFlowerBud* GetInstance() { return m_ThisPtr; }

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CFlowerBud::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 状態関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void StateWait() {}			// 待機
	void StateCharge();			// 溜め
	void StateFlowering();		// 開花

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;		// 状態
	float m_fStateTime;	// 状態タイマー
	float m_fRatio;	// 割合
	int m_nSpawnNum;	// 生成数

	static CFlowerBud* m_ThisPtr;
};


#endif
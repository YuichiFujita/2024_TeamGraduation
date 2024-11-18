//=============================================================================
//
// スペシャル演出処理 [specialEffect.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _SPECIALEFFECT_H_		// このマクロ定義がされていなかったら
#define _SPECIALEFFECT_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "motion.h"
#include "EffekseerObj.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;

//==========================================================================
// クラス定義
//==========================================================================
// スペシャル演出クラス
class CSpecialEffect
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EType
	{
		TYPE_KAMEHAMEHA = 0,
		TYPE_MAX
	};

	CSpecialEffect();
	virtual ~CSpecialEffect();
	
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;			// 更新
	virtual void TriggerMoment(CMotion::AttackInfo ATKInfo, int idx) = 0;	// トリガーの瞬間
	virtual void ProgressMoment(CMotion::AttackInfo ATKInfo, int idx) = 0;	// 進行中

	CPlayer* GetPlayer() { return m_pPlayer; }	// プレイヤー取得

	// 生成
	static CSpecialEffect* Create(CPlayer* pPlayer, EType type)
	{
		// 生成関数
		CSpecialEffect* pEffect = m_CreateFunc[type]();

		if (pEffect != nullptr)
		{// プレイヤー割り当て
			pEffect->m_pPlayer = pPlayer;
		}
		return pEffect;
	}

private:

	//=============================
	// 関数リスト
	//=============================
	// 生成リスト
	using CREATE_FUNC = std::function<CSpecialEffect*()>;
	static std::vector<CREATE_FUNC>m_CreateFunc;

	// メンバ関数
	CPlayer* m_pPlayer;	// プレイヤー
};

#endif
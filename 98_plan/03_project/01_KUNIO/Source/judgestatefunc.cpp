//=============================================================================
// 
//  判定状態関数 [judgestatefunc.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "judgestatefunc.h"
#include "judgeobj.h"
#include "manager.h"
#include "game.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	// 判定別処理時間
	namespace Judge_AAA
	{
		const float TIME_APPEAR = 1.0f;		// 出現時間
		const float TIME_WAIT = 0.4f;		// 待ち時間
		const float TIME_FADEOUT = 0.6f;	// フェードアウト時間
		const int LAP_NUM = 4;				// 回転する回数
		const float INCLINATION_ANGLE = D3DX_PI / 8;	// 傾ける角度
	}
	namespace Judge_BBB
	{
		const float TIME_APPEAR = 1.0f;		// 出現時間
		const float TIME_WAIT = 0.4f;		// 待ち時間
		const float TIME_FADEOUT = 0.6f;	// フェードアウト時間
	}
	namespace Judge_CCC
	{
		const float TIME_APPEAR = 0.85f;		// 出現時間
		const float TIME_WAIT = 0.3f;		// 待ち時間
		const float TIME_FADEOUT = 0.85f;	// フェードアウト時間
		const float APPEAR_SIZE = 0.8f;		// 出現時の拡大率
	}
	namespace Judge_DDD
	{
		const float TIME_APPEAR = 0.5f;		// 出現時間
		const float TIME_WAIT = 0.7f;		// 待ち時間
		const float TIME_FADEOUT = 0.8f;	// フェードアウト時間
		const float KOTE_ANGLE = D3DX_PI / 8;	//コテッとさせる角度（現在6分の1PI[rad]）
		const float KOTE_TIME = 0.16f;			//コテッとさせるのにかかる時間
	}
	const MyLib::Vector3 DESTROTATION_GETTOGETHER = MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.75f);
	const MyLib::Vector3 DESTPOSITION_GETTOGETHER = MyLib::Vector3(130.0f, 230.0f, 0.0f);
	const MyLib::Vector3 DESTPOSITION_STINGS = MyLib::Vector3(0.0f, 360.0f, 0.0f);
}
//==========================================================================
// 関数ポインタ
//==========================================================================
CJudgeStateFunc::STATE_FUNC CJudgeStateFunc::m_StateFuncList[] =
{
	&CJudgeStateFunc::StateAppear,
	&CJudgeStateFunc::StateWait,
	&CJudgeStateFunc::StateFadeOut,
};

//**************************************************************************
// 基底状態関数
//**************************************************************************
//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeStateFunc::CJudgeStateFunc(CJudgeObj* pObj)
{
	m_pObj = pObj;
	m_state = CJudgeStateFunc::STATE_APPEAR;
	m_fStateTime = 0.0f;
}

//**************************************************************************
// AAAランク別状態関数
//**************************************************************************
//==========================================================================
// 出現状態
//==========================================================================
void CJudgeStateFunc_AAA::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_AAA::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// サイズ設定
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / Judge_AAA::TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y, ratio);
	pObj->SetSize(size);

	// 回転
	float angle = (Judge_AAA::LAP_NUM * 2 * D3DX_PI) * ratio;
	UtilFunc::Transformation::RotNormalize(angle);
	pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, angle));
}

//==========================================================================
// 待機状態
//==========================================================================
void CJudgeStateFunc_AAA::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_AAA::TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
	CJudgeObj* pObj = GetObj();
	pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
}

//==========================================================================
// 消える状態
//==========================================================================
void CJudgeStateFunc_AAA::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_AAA::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// 戦闘開始に遷移
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// サイズ設定
	float ratio = stateTime / Judge_BBB::TIME_FADEOUT;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(sizeOrigin.x, 0.0f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(sizeOrigin.y, 0.0f, ratio);
	pObj->SetSize(size);
}

//**************************************************************************
// BBBランク別状態関数
//**************************************************************************
//==========================================================================
// 出現状態
//==========================================================================
void CJudgeStateFunc_BBB::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_BBB::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// サイズ設定
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / Judge_BBB::TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseOut(0.0f, sizeOrigin.x, ratio);
	size.y = UtilFunc::Correction::EasingEaseOut(0.0f, sizeOrigin.y, ratio);
	pObj->SetSize(size);
}

//==========================================================================
// 待機状態
//==========================================================================
void CJudgeStateFunc_BBB::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_BBB::TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// 消える状態
//==========================================================================
void CJudgeStateFunc_BBB::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_BBB::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// 戦闘開始に遷移
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// サイズ設定
	float ratio = stateTime / Judge_BBB::TIME_FADEOUT;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(sizeOrigin.x, 0.0f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(sizeOrigin.y, 0.0f, ratio);
	pObj->SetSize(size);
}

//**************************************************************************
// CCCランク別状態関数
//**************************************************************************
//==========================================================================
// 出現状態
//==========================================================================
void CJudgeStateFunc_CCC::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_CCC::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// サイズ設定
	CJudgeObj* pObj = GetObj();
	pObj->SetSize(pObj->GetSizeOrigin());
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, stateTime / Judge_DDD::TIME_APPEAR);
	pObj->SetAlpha(alpha);

	/*CJudgeObj* pObj = GetObj();
	float ratio = stateTime / Judge_CCC::TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y, ratio);
	pObj->SetSize(size);*/
}

//==========================================================================
// 待機状態
//==========================================================================
void CJudgeStateFunc_CCC::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_CCC::TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// 消える状態
//==========================================================================
void CJudgeStateFunc_CCC::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_CCC::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// 戦闘開始に遷移
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// サイズ設定
	float ratio = stateTime / Judge_CCC::TIME_FADEOUT;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.y = UtilFunc::Correction::EasingEaseIn(sizeOrigin.y, 0.0f, ratio);
	pObj->SetSize(size);

	// 不透明度設定
	/*float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, stateTime / Judge_CCC::TIME_FADEOUT);
	pObj->SetAlpha(alpha);*/
}

//**************************************************************************
// DDDランク別状態関数
//**************************************************************************
//==========================================================================
// 出現状態
//==========================================================================
void CJudgeStateFunc_DDD::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_DDD::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// 不透明度設定
	CJudgeObj* pObj = GetObj();
	pObj->SetSize(pObj->GetSizeOrigin());
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, stateTime / Judge_DDD::TIME_APPEAR);
	pObj->SetAlpha(alpha);
}

//==========================================================================
// 待機状態
//==========================================================================
void CJudgeStateFunc_DDD::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_DDD::TIME_WAIT)
	{// 次の状態
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
	else if (stateTime <= Judge_DDD::KOTE_TIME)
	{// コテッとさせる
		CJudgeObj* pObj = GetObj();
		float angle = Judge_DDD::KOTE_ANGLE * (stateTime / Judge_DDD::KOTE_TIME);
		pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, angle));
	}
}

//==========================================================================
// 消える状態
//==========================================================================
void CJudgeStateFunc_DDD::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_DDD::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// 戦闘開始に遷移
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// 不透明度設定
	float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, stateTime / Judge_DDD::TIME_FADEOUT);
	pObj->SetAlpha(alpha);
	//pObj->SetAlpha(1.0f - stateTime / Judge_DDD::TIME_FADEOUT);
}
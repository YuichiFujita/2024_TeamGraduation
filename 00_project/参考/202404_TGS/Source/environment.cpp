//=============================================================================
// 
//  自然物処理 [environment.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "environment.h"
#include "manager.h"
#include "calculation.h"

#include "player.h"
#include "leaf_flow.h"
#include "camera.h"

//==========================================================================
// 更新処理
//==========================================================================
void CSpawnEnvironment::Update(float deltaTime)
{
	// タイマー加算
	m_fSpawnTimer += deltaTime;

	if (m_fSpawnTimer >= m_fSpawnInterval)
	{
		// 生成時のトリガー
		TriggerSpawn();
	}
}

//==========================================================================
// 空気の生成時のトリガー
//==========================================================================
void CSpawn_Air::TriggerSpawn()
{
	// タイマー関連リセット
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-20, 20) * 0.01f;

	// プレイヤー取得
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// 基点の位置
	MyLib::Vector3 basepos = MyLib::Vector3(playerPos.x, 0.0f, playerPos.z) + MyLib::Vector3(-500.0f, 500.0f, 1000.0f);

	int rand = UtilFunc::Transformation::Random(0, 2);
	switch (rand)
	{
	case 0:
		// 風生成
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 1:
		// 右の風
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(40, 60) * 10.0f,
				UtilFunc::Transformation::Random(-5, 15) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 2:
		// 右上の風
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(80, 120) * 10.0f,
				UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	default:
		break;
	}

}


//==========================================================================
// 空気の生成時のトリガー
//==========================================================================
void CSpawn_Air_Title::TriggerSpawn()
{
	// タイマー関連リセット
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-20, 20) * 0.01f;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 posR = pCamera->GetPositionR();

	// 基点の位置
	MyLib::Vector3 basepos = MyLib::Vector3(posR.x, 0.0f, posR.z) + MyLib::Vector3(0.0f, 500.0f, 500.0f);

	int rand = UtilFunc::Transformation::Random(0, 2);
	switch (rand)
	{
	case 0:
		// 風生成
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 1:
		// 右の風
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(40, 60) * 10.0f,
				UtilFunc::Transformation::Random(-5, 15) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 2:
		// 右上の風
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(80, 120) * 10.0f,
				UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	default:
		break;
	}

}

//==========================================================================
// 流れる葉の生成時のトリガー
//==========================================================================
void CSpawn_FlowLeaf::TriggerSpawn()
{
	// タイマー関連リセット
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 20) * 0.01f;

	// プレイヤー取得
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	float playerMoveLen = pPlayer->GetMoveLength();

	// 流れる葉生成
	CLeafFlow* pLeaf = static_cast<CLeafFlow*>(CLeaf::Create(
		MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-6, 6) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FLOW));

	pLeaf->SetMoveLen(playerMoveLen - 1000.0f);
	pLeaf->SetOriginPosition(pLeaf->GetPosition());
}


//==========================================================================
// 降る葉の更新処理
//==========================================================================
void CSpawn_Leaf::Update(float deltaTime)
{
	// プレイヤー取得
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return;

	MyLib::Vector3 playerMove = pPlayer->GetMove();	// 移動量取得
	float fVelocity = pPlayer->GetVelocity();		// 移動速度取得

	// 移動速度割合
	float ratio = playerMove.x / (fVelocity);

	// 移動速度に応じて生成間隔短くする
	m_fSpawnTimer += deltaTime * ratio;

	// 親の更新処理
	CSpawnEnvironment::Update(deltaTime);
}

//==========================================================================
// 降る葉の生成時のトリガー
//==========================================================================
void CSpawn_Leaf::TriggerSpawn()
{
	// タイマー関連リセット
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 40) * 0.01f;

	// プレイヤー取得
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return;
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 基点の位置
	MyLib::Vector3 basepos = MyLib::Vector3(playerPos.x, 0.0f, playerPos.z) + MyLib::Vector3(0.0f, 700.0f + pCamera->GetPositionR().y, 0.0f);

	// 降る葉生成
	CLeaf* pLeaf = CLeaf::Create(
		basepos + MyLib::Vector3(
			UtilFunc::Transformation::Random(-5, 20) * 100.0f + UtilFunc::Transformation::Random(-90, 90),
			UtilFunc::Transformation::Random(0, 3) * 100.0f,
			UtilFunc::Transformation::Random(-5, 5) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FALL);
}


//==========================================================================
// 降る葉の更新処理
//==========================================================================
void CSpawn_Leaf_Title::Update(float deltaTime)
{
	// 親の更新処理
	CSpawnEnvironment::Update(deltaTime);
}

//==========================================================================
// 降る葉の生成時のトリガー
//==========================================================================
void CSpawn_Leaf_Title::TriggerSpawn()
{
	// タイマー関連リセット
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 40) * 0.01f;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 基点の位置
	MyLib::Vector3 basepos = pCamera->GetPositionR();
	basepos.z = 0.0f;

	basepos += MyLib::Vector3(-800.0f, 400.0f, 100.0f);

	// 降る葉生成
	CLeaf* pLeaf = CLeaf::Create(
		basepos + MyLib::Vector3(
			UtilFunc::Transformation::Random(-5, 20) * 100.0f + UtilFunc::Transformation::Random(-90, 90),
			UtilFunc::Transformation::Random(0, 3) * 100.0f,
			UtilFunc::Transformation::Random(-5, 5) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FALL);
}


//==========================================================================
// 降る葉(ランキング)の更新処理
//==========================================================================
void CSpawn_Leaf_Ranking::Update(float deltaTime)
{
	// 親の更新処理
	CSpawnEnvironment::Update(deltaTime);
}

//==========================================================================
// 降る葉(ランキング)の生成時のトリガー
//==========================================================================
void CSpawn_Leaf_Ranking::TriggerSpawn()
{
	// タイマー関連リセット
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 40) * 0.01f;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 基点の位置
	MyLib::Vector3 basepos = pCamera->GetPositionR();
	basepos += MyLib::Vector3(0.0f, 700.0f, 500.0f);

	// 降る葉生成
	CLeaf* pLeaf = CLeaf::Create(
		basepos + MyLib::Vector3(
			UtilFunc::Transformation::Random(-20, 20) * 100.0f + UtilFunc::Transformation::Random(-90, 90),
			UtilFunc::Transformation::Random(0, 3) * 100.0f,
			UtilFunc::Transformation::Random(-5, 5) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FALL);
}

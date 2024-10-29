//=============================================================================
// 
// カメラデバッグ処理 [camera_debug.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "camera_debug.h"
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float MOVE_SLIDEMOUSE = 2.5f;			// マウスのスライド移動量
	const float MOVE_ROTATIONMOUSE = 0.005f;	// マウスの回転移動量
	const float MOVE_WASD = 5.0f;				// WASDの移動量
	const float MOVE_UP_DOWN = 10.0f;			// 上下移動量
	const float MOVE_DISTANCE = 1.0f;			// 距離の移動量
	const float  MIN_ROT = -D3DX_PI * 0.49f;	// カメラ固定用
	const float  MAX_ROT = D3DX_PI * 0.49f;		// カメラ固定用
}

//==========================================================================
// 更新処理
//==========================================================================
void CCamera_Debug::Update()
{
	// 注視点取得
	MyLib::Vector3 posR = m_pCamera->GetPositionR();

	// 向き取得
	MyLib::Vector3 rot = m_pCamera->GetRotation();


	// キーボード情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// キーボード情報取得
	CInputMouse* pMouse = CInputMouse::GetInstance();


	if (pKey->GetTrigger(DIK_F7))
	{// F7が押された,追従切り替え

		// 反転
		bool bFollow = m_pCamera->IsFollow() ? false : true;
		m_pCamera->SetEnableFollow(bFollow);
	}

	if (pMouse->GetPress(CInputMouse::BUTTON_LEFT) &&
		pMouse->GetPress(CInputMouse::BUTTON_RIGHT))
	{// 左右同時押し

		posR.x +=
			(pMouse->GetMouseMove().x * sinf(-D3DX_PI * 0.5f + rot.y) * MOVE_SLIDEMOUSE) -
			(pMouse->GetMouseMove().y * cosf(-D3DX_PI * 0.5f + rot.y) * MOVE_SLIDEMOUSE);

		posR.z +=
			(pMouse->GetMouseMove().x * cosf(-D3DX_PI * 0.5f + rot.y) * MOVE_SLIDEMOUSE) +
			(pMouse->GetMouseMove().y * sinf(-D3DX_PI * 0.5f + rot.y) * MOVE_SLIDEMOUSE);
	}
	else if (
		pKey->GetPress(DIK_LALT) &&
		pMouse->GetPress(CInputMouse::BUTTON_LEFT))
	{// 左クリックしてるとき,視点回転

		m_moveRot.y += pMouse->GetMouseMove().x * MOVE_ROTATIONMOUSE;
		m_moveRot.z += pMouse->GetMouseMove().y * MOVE_ROTATIONMOUSE;

		// 角度の正規化
		UtilFunc::Transformation::RotNormalize(rot);

		// 値の正規化
		UtilFunc::Transformation::ValueNormalize(rot.z, MAX_ROT, MIN_ROT);
	}

	// 移動量分を加算
	rot += m_moveRot;

	// 向き設定
	m_pCamera->SetRotation(rot);

	// 移動量をリセット
	m_moveRot += (MyLib::Vector3() - m_moveRot) * 0.6f;

	// キーボード更新
	UpdateKeyboard();

	// 距離の更新
	UpdateDistance();

	// GUI操作
	if (ImGui::TreeNode("Camera Debug"))
	{
		UpdateGUI();
		ImGui::TreePop();
	}
}

//==========================================================================
// キーボード更新
//==========================================================================
void CCamera_Debug::UpdateKeyboard()
{
	// 注視点取得
	MyLib::Vector3 posR = m_pCamera->GetPositionR();
	MyLib::Vector3 rot = m_pCamera->GetRotation();

	// キーボード情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// マウス情報取得
	CInputMouse* pMouse = CInputMouse::GetInstance();

	if (pMouse->GetPress(CInputMouse::BUTTON_LEFT) &&
		!pKey->GetPress(DIK_LALT))
	{// 左クリック時移動

		//--------------------------
		// 移動
		//--------------------------
		if (pKey->GetPress(DIK_W))
		{
			if (pKey->GetPress(DIK_A))
			{
				m_move.x += sinf(D3DX_PI * -0.25f + rot.y) * MOVE_WASD;
				m_move.z += cosf(D3DX_PI * -0.25f + rot.y) * MOVE_WASD;
			}
			else if (pKey->GetPress(DIK_D))
			{
				m_move.x += sinf(D3DX_PI * 0.25f + rot.y) * MOVE_WASD;
				m_move.z += cosf(D3DX_PI * 0.25f + rot.y) * MOVE_WASD;
			}
			else
			{
				m_move.x += sinf(D3DX_PI * 0.0f + rot.y) * MOVE_WASD;
				m_move.z += cosf(D3DX_PI * 0.0f + rot.y) * MOVE_WASD;
			}
		}
		else if (pKey->GetPress(DIK_S))
		{
			if (pKey->GetPress(DIK_A))
			{
				m_move.x += sinf(D3DX_PI * -0.75f + rot.y) * MOVE_WASD;
				m_move.z += cosf(D3DX_PI * -0.75f + rot.y) * MOVE_WASD;
			}
			else if (pKey->GetPress(DIK_D))
			{
				m_move.x += sinf(D3DX_PI * 0.75f + rot.y) * MOVE_WASD;
				m_move.z += cosf(D3DX_PI * 0.75f + rot.y) * MOVE_WASD;
			}
			else
			{
				m_move.x += sinf(D3DX_PI * 1.0f + rot.y) * MOVE_WASD;
				m_move.z += cosf(D3DX_PI * 1.0f + rot.y) * MOVE_WASD;
			}
		}
		else if (pKey->GetPress(DIK_A))
		{
			m_move.x += sinf(D3DX_PI * -0.5f + rot.y) * MOVE_WASD;
			m_move.z += cosf(D3DX_PI * -0.5f + rot.y) * MOVE_WASD;
		}
		else if (pKey->GetPress(DIK_D))
		{
			m_move.x += sinf(D3DX_PI * 0.5f + rot.y) * MOVE_WASD;
			m_move.z += cosf(D3DX_PI * 0.5f + rot.y) * MOVE_WASD;
		}

		//--------------------------
		// 上下
		//--------------------------
		if (pKey->GetPress(DIK_E))
		{// 上
			m_move.y += MOVE_WASD;
		}
		else if (pKey->GetPress(DIK_Q))
		{// 下
			m_move.y -= MOVE_WASD;
		}
	}

	// 移動量分を加算
	posR += m_move;

	// 移動量をリセット
	m_move += (MyLib::Vector3() - m_move) * 0.15f;

	// 注視点設定
	m_pCamera->SetPositionR(posR);
	m_pCamera->SetPositionRDest(posR);
}

//==========================================================================
// 距離の更新
//==========================================================================
void CCamera_Debug::UpdateDistance()
{
	// 距離取得
	float distance = m_pCamera->GetDistance();
	float destDistance = m_pCamera->GetDistanceDest();
	float originDistance = m_pCamera->GetOriginDistance();

	// キーボード情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();

	float moceWheel = pMouse->GetMouseMove().z;

	if (pKey->GetPress(DIK_LALT))
	{
		// マウスホイールで距離調整
		distance += moceWheel * MOVE_DISTANCE;

		// 値の補正
		UtilFunc::Transformation::ValueNormalize(distance, 10000.0f, 1.0f);

		// 目標/元々も同一にする
		destDistance = originDistance = distance;
	}

	// 距離設定
	m_pCamera->SetDistance(distance);
	m_pCamera->SetDistanceDest(destDistance);
	m_pCamera->SetOriginDistance(originDistance);
	m_pCamera->WarpCamera(m_pCamera->GetPositionR());
}

//==========================================================================
// GUI操作
//==========================================================================
void CCamera_Debug::UpdateGUI()
{
	//--------------------------
	// 追従切り替え
	//--------------------------
	{
		bool bFollow = m_pCamera->IsFollow();
		ImGui::Checkbox("Follow", &bFollow);
		m_pCamera->SetEnableFollow(bFollow);
	}

	//--------------------------
	// 注視点操作
	//--------------------------
	{
		// 注視点取得
		MyLib::Vector3 posR = m_pCamera->GetPositionR();

		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		if (ImGui::Button("Reset"))
		{// リセット
			posR = m_pCamera->GetPositionROrigin();
		}
		ImGui::PopID();

		// 詰める
		ImGui::SameLine();

		ImGui::DragFloat3("posR", (float*)&posR, 1.0f, 0.0f, 0.0f, "%.2f");

		// 注視点設定
		m_pCamera->SetPositionR(posR);
		m_pCamera->SetPositionRDest(posR);
	}

	//--------------------------
	// 向き操作
	//--------------------------
	{
		// 向き取得
		MyLib::Vector3 rot = m_pCamera->GetRotation();

		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		if (ImGui::Button("Reset"))
		{// リセット
			rot = m_pCamera->GetOriginRotation();
		}
		ImGui::PopID();
		ImGui::SameLine();

		ImGui::DragFloat3("rot", (float*)&rot, 1.0f, 0.0f, 0.0f, "%.2f");

		// 向き設定
		m_pCamera->SetRotation(rot);
	}
}

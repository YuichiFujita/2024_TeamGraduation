//=============================================================================
// 
// �J�����f�o�b�O���� [camera_debug.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "camera_debug.h"
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float MOVE_SLIDEMOUSE = 2.5f;			// �}�E�X�̃X���C�h�ړ���
	const float MOVE_ROTATIONMOUSE = 0.005f;	// �}�E�X�̉�]�ړ���
	const float MOVE_WASD = 5.0f;				// WASD�̈ړ���
	const float MOVE_UP_DOWN = 10.0f;			// �㉺�ړ���
	const float MOVE_DISTANCE = 1.0f;			// �����̈ړ���
	const float  MIN_ROT = -D3DX_PI * 0.49f;	// �J�����Œ�p
	const float  MAX_ROT = D3DX_PI * 0.49f;		// �J�����Œ�p
}

//==========================================================================
// �X�V����
//==========================================================================
void CCamera_Debug::Update()
{
	// �����_�擾
	MyLib::Vector3 posR = m_pCamera->GetPositionR();

	// �����擾
	MyLib::Vector3 rot = m_pCamera->GetRotation();


	// �L�[�{�[�h���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// �L�[�{�[�h���擾
	CInputMouse* pMouse = CInputMouse::GetInstance();


	if (pKey->GetTrigger(DIK_F7))
	{// F7�������ꂽ,�Ǐ]�؂�ւ�

		// ���]
		bool bFollow = m_pCamera->IsFollow() ? false : true;
		m_pCamera->SetEnableFollow(bFollow);
	}

	if (pMouse->GetPress(CInputMouse::BUTTON_LEFT) &&
		pMouse->GetPress(CInputMouse::BUTTON_RIGHT))
	{// ���E��������

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
	{// ���N���b�N���Ă�Ƃ�,���_��]

		m_moveRot.y += pMouse->GetMouseMove().x * MOVE_ROTATIONMOUSE;
		m_moveRot.z += pMouse->GetMouseMove().y * MOVE_ROTATIONMOUSE;

		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(rot);

		// �l�̐��K��
		UtilFunc::Transformation::ValueNormalize(rot.z, MAX_ROT, MIN_ROT);
	}

	// �ړ��ʕ������Z
	rot += m_moveRot;

	// �����ݒ�
	m_pCamera->SetRotation(rot);

	// �ړ��ʂ����Z�b�g
	m_moveRot += (MyLib::Vector3() - m_moveRot) * 0.6f;

	// �L�[�{�[�h�X�V
	UpdateKeyboard();

	// �����̍X�V
	UpdateDistance();

	// GUI����
	if (ImGui::TreeNode("Camera Debug"))
	{
		UpdateGUI();
		ImGui::TreePop();
	}
}

//==========================================================================
// �L�[�{�[�h�X�V
//==========================================================================
void CCamera_Debug::UpdateKeyboard()
{
	// �����_�擾
	MyLib::Vector3 posR = m_pCamera->GetPositionR();
	MyLib::Vector3 rot = m_pCamera->GetRotation();

	// �L�[�{�[�h���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	// �}�E�X���擾
	CInputMouse* pMouse = CInputMouse::GetInstance();

	if (pMouse->GetPress(CInputMouse::BUTTON_LEFT) &&
		!pKey->GetPress(DIK_LALT))
	{// ���N���b�N���ړ�

		//--------------------------
		// �ړ�
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
		// �㉺
		//--------------------------
		if (pKey->GetPress(DIK_E))
		{// ��
			m_move.y += MOVE_WASD;
		}
		else if (pKey->GetPress(DIK_Q))
		{// ��
			m_move.y -= MOVE_WASD;
		}
	}

	// �ړ��ʕ������Z
	posR += m_move;

	// �ړ��ʂ����Z�b�g
	m_move += (MyLib::Vector3() - m_move) * 0.15f;

	// �����_�ݒ�
	m_pCamera->SetPositionR(posR);
	m_pCamera->SetPositionRDest(posR);
}

//==========================================================================
// �����̍X�V
//==========================================================================
void CCamera_Debug::UpdateDistance()
{
	// �����擾
	float distance = m_pCamera->GetDistance();
	float destDistance = m_pCamera->GetDistanceDest();
	float originDistance = m_pCamera->GetOriginDistance();

	// �L�[�{�[�h���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();

	float moceWheel = pMouse->GetMouseMove().z;

	if (pKey->GetPress(DIK_LALT))
	{
		// �}�E�X�z�C�[���ŋ�������
		distance += moceWheel * MOVE_DISTANCE;

		// �l�̕␳
		UtilFunc::Transformation::ValueNormalize(distance, 10000.0f, 1.0f);

		// �ڕW/���X������ɂ���
		destDistance = originDistance = distance;
	}

	// �����ݒ�
	m_pCamera->SetDistance(distance);
	m_pCamera->SetDistanceDest(destDistance);
	m_pCamera->SetOriginDistance(originDistance);
	m_pCamera->WarpCamera(m_pCamera->GetPositionR());
}

//==========================================================================
// GUI����
//==========================================================================
void CCamera_Debug::UpdateGUI()
{
	//--------------------------
	// �Ǐ]�؂�ւ�
	//--------------------------
	{
		bool bFollow = m_pCamera->IsFollow();
		ImGui::Checkbox("Follow", &bFollow);
		m_pCamera->SetEnableFollow(bFollow);
	}

	//--------------------------
	// �����_����
	//--------------------------
	{
		// �����_�擾
		MyLib::Vector3 posR = m_pCamera->GetPositionR();

		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		if (ImGui::Button("Reset"))
		{// ���Z�b�g
			posR = m_pCamera->GetPositionROrigin();
		}
		ImGui::PopID();

		// �l�߂�
		ImGui::SameLine();

		ImGui::DragFloat3("posR", (float*)&posR, 1.0f, 0.0f, 0.0f, "%.2f");

		// �����_�ݒ�
		m_pCamera->SetPositionR(posR);
		m_pCamera->SetPositionRDest(posR);
	}

	//--------------------------
	// ��������
	//--------------------------
	{
		// �����擾
		MyLib::Vector3 rot = m_pCamera->GetRotation();

		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		if (ImGui::Button("Reset"))
		{// ���Z�b�g
			rot = m_pCamera->GetOriginRotation();
		}
		ImGui::PopID();
		ImGui::SameLine();

		ImGui::DragFloat3("rot", (float*)&rot, 1.0f, 0.0f, 0.0f, "%.2f");

		// �����ݒ�
		m_pCamera->SetRotation(rot);
	}
}

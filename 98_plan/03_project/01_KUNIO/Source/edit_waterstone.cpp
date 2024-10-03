//=============================================================================
// 
//  �R�[�X�G�f�B�^�\���� [edit_waterstone.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_waterstone.h"
#include "waterstone.h"
#include "waterstoneManager.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"
#include "spline.h"
#include "collisionLine_Box.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::AABB AABB_LINE = MyLib::AABB(-25.0f, 25.0f);	// �ӂ�AABB

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_WaterStone::CEdit_WaterStone()
{
	// �l�̃N���A
	m_nEditIdx = 0;			// ���삷��C���f�b�N�X�ԍ�
	m_bEdit = false;		// ���쒆����
	m_bDrag = false;		// �͂ݔ���
	m_bHoverWindow = false;	// �}�E�X�̃E�B���h�E�z�o�[����
	m_bSetMode = false;		// �ݒ胂�[�h����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_WaterStone::~CEdit_WaterStone()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_WaterStone::Init()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_WaterStone::Uninit()
{
	// �I������
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_WaterStone::Update()
{
	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// �t�@�C������
	FileControl();

	// ���[�h�ύX
	ChangeMode();

	// ��_�ǉ�
	AddPoint();

	// ���C���I��
	SelectLine();

	// �͂ݒ�
	DragLine();

	// �g�����X�t�H�[��
	Transform();

}

//==========================================================================
// �t�@�C������
//==========================================================================
void CEdit_WaterStone::FileControl()
{
	// �����΃}�l�[�W���擾
	CWaterStone_Manager* pMgr = CWaterStone_Manager::GetInstance();
	if (pMgr == nullptr) return;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	float width = 150.0f;
	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save"))
	{
		//pMgr->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save_as"))
	{
		//pMgr->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{

	}
}

//==========================================================================
// ���[�h�ύX
//==========================================================================
void CEdit_WaterStone::ChangeMode()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (ImGui::Checkbox("Enable SetMode!!!", &m_bSetMode))
	{
		// ���씻�胊�Z�b�g
		m_bEdit = false;
	}
}

//==========================================================================
// ���C���I��
//==========================================================================
void CEdit_WaterStone::SelectLine()
{
	if (m_bSetMode) return;	// �Z�b�g���[�h�͏I���

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-25.0f, 25.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		!m_bDrag &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{// ���N���b�N��

		// ���씻�胊�Z�b�g
		m_bEdit = false;


		// ��Q���̃��X�g�擾
		CListManager<CWaterStone> list = CWaterStone::GetListObj();

		// �擪��ۑ�
		std::list<CWaterStone*>::iterator itr = list.GetEnd();
		CWaterStone* pObj = nullptr;

		int i = 0;
		while (list.ListLoop(itr))
		{
			CWaterStone* pObj = *itr;
			MyLib::Vector3 ObjPos = pObj->GetPosition();



			// �}�g���b�N�X������
			mtx.Identity();
			mtxTrans.Identity();

			// �ʒu��񔽉f
			mtxTrans.Translation(ObjPos);
			mtx.Multiply(mtx, mtxTrans);

			MyLib::Vector3 OBpos;

			MyLib::AABB aabb = pObj->GetAABB();
			if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
			{// �d�Ȃ� && ���N���b�N

				// ���삷��ӕۑ�
				m_nEditIdx = i;

				m_bEdit = true;
				break;
			}
			i++;
		}

	}
}

//==========================================================================
// ���C���͂�
//==========================================================================
void CEdit_WaterStone::DragLine()
{

	// �����΃}�l�[�W���擾
	CWaterStone_Manager* pMgr = CWaterStone_Manager::GetInstance();
	if (pMgr == nullptr) return;

	if (!m_bEdit) return;
	if (m_bSetMode) return;	// �Z�b�g���[�h�͏I���

	// ��Q���̃��X�g�擾
	CListManager<CWaterStone> list = CWaterStone::GetListObj();
	CWaterStone* pObj = list.GetData(m_nEditIdx);
	if (pObj == nullptr) return;

	MyLib::Vector3 ObjPos = pObj->GetPosition();

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = pObj->GetAABB();
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		pMouse->GetPress(CInputMouse::BUTTON::BUTTON_LEFT) && !CInputKeyboard::GetInstance()->GetPress(DIK_LALT))
	{// ���������ݎ�

		// �}�g���b�N�X������
		mtx.Identity();
		mtxTrans.Identity();

		// �ʒu��񔽉f
		mtxTrans.Translation(ObjPos);
		mtx.Multiply(mtx, mtxTrans);

		MyLib::Vector3 OBpos;

		if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
		{// �d�Ȃ�
			m_bDrag = true;
		}

	}

	if (pMouse->GetRelease(CInputMouse::BUTTON::BUTTON_LEFT))
	{
		m_bDrag = false;
	}

	ObjPos = pObj->GetPosition();

	// �h���b�O��
	if (m_bDrag)
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera == nullptr) return;

		// �Ĉړ���
		MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();
		ObjPos.x += diffpos.x;
		ObjPos.z += diffpos.z;
	}

	// ���_�f�[�^�ݒ�
	pObj->SetPosition(ObjPos);
}

//==========================================================================
// �g�����X�t�H�[��
//==========================================================================
void CEdit_WaterStone::Transform()
{
	// ��Q���̃��X�g�擾
	CListManager<CWaterStone> list = CWaterStone::GetListObj();
	CWaterStone* pSelectStone = list.GetData(m_nEditIdx);
	if (pSelectStone == nullptr) return;

	// �擪��ۑ�
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CWaterStone* pObj = nullptr;

	int i = 0;
	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;
		MyLib::Vector3 ObjPos = pObj->GetPosition();

		// �f�o�b�O���BOX�擾
		CCollisionLine_Box* pBox = pObj->GetCollisionLineBox();
		if (pBox == nullptr) continue;

		// �F���Z�b�g
		D3DXCOLOR col = (i == m_nEditIdx) ? D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pBox->SetColor(col);

		i++;
	}



	CWaterStone_Manager::SStoneInfo selectInfo = pSelectStone->GetStoneInfo();

	MyLib::Vector3 move;
	move.x = sinf(D3DX_PI + selectInfo.rot.y) * 30.0f;
	move.z = cosf(D3DX_PI + selectInfo.rot.y) * 30.0f;

	CEffect3D::Create(
		MyLib::Vector3(selectInfo.pos.x, selectInfo.pos.y + 100.0f, selectInfo.pos.z),
		move,
		mylib_const::DEFAULT_COLOR,
		40.0f,
		10,
		CEffect3D::MOVEEFFECT::MOVEEFFECT_SUB,
		CEffect3D::TYPE::TYPE_NORMAL);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Transform"))
	{
		float windowWidth = 100.0f;
		const float  ROT_MOVE = D3DX_PI * 0.01f;
		const float  POS_MOVE = 0.5f;

		if (!m_bEdit)
		{
			ImGui::Text(":None Obj:");
			ImGui::TreePop();
			return;
		}

		ImGui::Text("NowEdit : [ %d ]", m_nEditIdx);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// ���Z�b�g
		if (ImGui::Button("RESET")) {
			selectInfo.pos = 0.0f;
			selectInfo.rot = 0.0f;
		}
		

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		//=============================
		// �ʒu�ݒ�
		//=============================
		ImGui::Text("pos");
		ImGui::SameLine();

		// X
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &selectInfo.pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &selectInfo.pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &selectInfo.pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		//=============================
		// �����ݒ�
		//=============================
		ImGui::Text("rot");
		ImGui::SameLine();

		// X
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &selectInfo.rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &selectInfo.rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &selectInfo.rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		//=============================
		// ��ސݒ�
		//=============================
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::SliderInt("Type", &selectInfo.type, 0, CWaterStone::GetModelNum() - 1);

		ImGui::TreePop();
	}

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(selectInfo.rot);

	if (m_bDrag)
		return;

	// �Ώ��ݒ�
	pSelectStone->SetStoneInfo(selectInfo);
	pSelectStone->SetPosition(selectInfo.pos);
	pSelectStone->ChangeModelType(selectInfo.type);
}

//==========================================================================
// �n�_�ǉ�
//==========================================================================
void CEdit_WaterStone::AddPoint()
{
	if (!m_bSetMode) return;	// �Z�b�g���[�h�ȊO�͏I���

	ImGui::Text("Push Mouse Button!!!");

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	// �J�������
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ(
		pMouse->GetPosition(),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		pCamera->GetMtxView(),
		pCamera->GetMtxProjection());

	

	// �ݒu
	if (!m_bHoverWindow &&
		!CInputKeyboard::GetInstance()->GetPress(DIK_LALT) &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{
		pos.y = 0.0f;
		CWaterStone_Manager::SStoneInfo info(pos, 0.0f, 0);
		CWaterStone::Create(info);
	}
}

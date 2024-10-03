//=============================================================================
// 
//  ��Q���G�f�B�^�\���� [edit_obstacle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float DISTANCE_OBJ = 2000.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_Obstacle::CEdit_Obstacle()
{
	// �l�̃N���A
	m_editType = EditType::TYPE_ARRANGMENT;
	m_pEditControl = nullptr;	// ��ޑ���

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_Obstacle::~CEdit_Obstacle()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_Obstacle::Init()
{
	// ����ύX
	ChangeMode(m_editType);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_Obstacle::Uninit()
{
	// �I������
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Obstacle::Update()
{
	// �R���{�{�b�N�X
	static const char* items[] = { "Arrangement", "Collider" };
	static int select = 1;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::Combo(": Mode", &select, items, IM_ARRAYSIZE(items)))
	{
		ChangeMode(static_cast<EditType>(select));
	}


	// �G�f�B�b�g�X�V
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	m_pEditControl->Update();
}

//==========================================================================
// ���[�h�ύX
//==========================================================================
void CEdit_Obstacle::ChangeMode(EditType type)
{
	if (m_pEditControl != nullptr)
	{
		m_pEditControl->DeleteBoxLine();
		m_pEditControl->Uninit();
		m_pEditControl = nullptr;
	}

	switch (type)
	{
	case CEdit_Obstacle::TYPE_ARRANGMENT:
		m_pEditControl = DEBUG_NEW CEdit_Obstacle_Arrangment;
		break;

	case CEdit_Obstacle::TYPE_COLLIDER:
		m_pEditControl = DEBUG_NEW CEdit_Obstacle_Collider;
		break;
	}
	m_pEditControl->Init();
}

//==========================================================================
// ������
//==========================================================================
void CEdit_Obstacle_Arrangment::Init()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	m_ObstacleInfo = pObstacleMgr->GetObstacleInfo(0);

	// �����蔻��{�b�N�X����
	CreateBoxLine();

}

//==========================================================================
// �����蔻��{�b�N�X����
//==========================================================================
void CEdit_Obstacle_Arrangment::CreateBoxLine()
{
	// �����蔻��{�b�N�X�폜
	DeleteBoxLine();

	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		for (auto& boxcollider : pObj->GetObstacleInfo().boxcolliders)
		{
			// AABB�ݒ�
			MyLib::AABB aabb = MyLib::AABB(boxcollider.vtxMin, boxcollider.vtxMax);

			// ��������
			CCollisionLine_Box* pBox;
			pBox = CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			m_pCollisionLineBox.push_back(pBox);

			// �ʒu�ݒ�
			boxcollider.TransformOffset(pObj->GetWorldMtx());
			m_pCollisionLineBox.back()->SetPosition(boxcollider.GetMtx().GetWorldPosition());
		}
	}

}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Obstacle_Arrangment::Update()
{

	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	// �R���{�{�b�N�X
	static const char* savetext[] = { "Save", "Save_as", "Load" };
	static int saveselect = 0;
	float width = 150.0f;



	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);



	std::vector<std::string> items;
	for (const auto& info : vecInfo)
	{
		std::string file = UtilFunc::Transformation::RemoveFilePath(info.modelFile);
		items.push_back(file);
	}

	// char* �̔z��ɕϊ�
	std::vector<const char*> items_cstr;
	for (const auto& item : items)
	{
		items_cstr.push_back(item.c_str());
	}

	static int select = 0;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::Combo("SetType", &select, items_cstr.data(), items_cstr.size()))
	{
		// ��Q�����ݒ�
		m_ObstacleInfo = vecInfo[select];
	}


	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
	//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

	ImVec2 imageSize = ImVec2(150, 50);

	// �h���b�O�\�ȗv�f�̕`��
	ImGui::Button(": Drag Me! :", imageSize);

	// �h���b�O���e�L�X�g�`��
	if (ImGui::BeginDragDropSource(src_flags))
	{
		std::string file = UtilFunc::Transformation::RemoveFilePath(m_ObstacleInfo.modelFile);
		ImGui::Text(file.c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::IsItemHovered() &&
		ImGui::IsMouseDown(0))
	{// UI��ɃJ�[�\�� && �N���b�N
		m_bButtonDrag = true;
	}

	// �����ڂ̂ݔz�u
	if (m_bButtonDrag &&
		!m_bHoverWindow)
	{// �h���b�O�� && �E�B���h�E�O

		if (m_pDragObj != nullptr) {
			m_pDragObj->Kill();
			m_pDragObj = nullptr;
		}

		// ����
		/*m_pDragObj = CObjectX::Create(m_ObstacleInfo.modelFile);
		m_pDragObj->SetType(CObject::TYPE::TYPE_OBJECTX);*/
	}


	// �}�E�X���擾
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();

	// �h���b�O��
	if (m_bButtonDrag)
	{
		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;

		CEffect3D::Create(
			setpos,
			0.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			80.0f, 2, CEffect3D::MOVEEFFECT::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}


	if (m_pDragObj != nullptr) {

		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;

		m_pDragObj->SetPosition(setpos);
	}

	// �z�u
	if (m_bButtonDrag &&
		ImGui::IsMouseReleased(0))
	{// �͂ݒ� && �}�E�X�����[�X

		if (m_pDragObj != nullptr) {
			m_pDragObj->Kill();
			m_pDragObj = nullptr;
		}

		if (!m_bHoverWindow) {

			CInputMouse* pMouse = CInputMouse::GetInstance();
			MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();

			MyLib::Vector3 setpos = mouseWorldPos;
			setpos.y = 0.0f;

			CMap_Obstacle* pObj = CMap_Obstacle::Create(m_ObstacleInfo, setpos);
			pObj->CalWorldMtx();

			CreateBoxLine();
		}
		m_bButtonDrag = false;
	}


	
	// �n���h���؂�ւ�
	ChangeHandle();


	// �I�u�W�F�N�g�I��
	ObjectSelect();

	Transform();


	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	int i = 0;
	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		for (auto& boxcollider : pObj->GetObstacleInfo().boxcolliders)
		{
			// �ʒu�ݒ�
			boxcollider.TransformOffset(pObj->GetWorldMtx());

			if (m_pCollisionLineBox.size() > i)
			{
				m_pCollisionLineBox[i]->SetPosition(boxcollider.GetMtx().GetWorldPosition());
				m_pCollisionLineBox[i]->SetRotation(pObj->GetRotation());
			}
			i++;
		}
	}

}

//==========================================================================
// �I�u�W�F�N�g�I��
//==========================================================================
void CEdit_Obstacle_Arrangment::ObjectSelect()
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();
	MyLib::Vector3 mouseOldWorldPos = pMouse->GetOldWorldPosition();
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();

	if (!m_bHoverWindow &&
		!m_bGrabHandle &&
		(m_pHandle == nullptr || (m_pHandle != nullptr && !m_pHandle->IsHoverHandle())) &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseClicked(0))
	{// �N���b�N

		// �Ē͂ݔ���
		m_bReGrab = false;

		m_pGrabObj = nullptr;

		// �擪��ۑ�
		CMap_Obstacle* pObject = nullptr;

		// ���X�g�R�s�[
		std::vector<CMap_Obstacle*> pObjectSort;

		while (list.ListLoop(itr))
		{
			// �v�f�𖖔��ɒǉ�
			pObjectSort.push_back((*itr));
		}

		// Z�\�[�g
		std::sort(pObjectSort.begin(), pObjectSort.end(), CObject::ZSortInverse);

		bool bHit = false;
		for (const auto& obj : pObjectSort)
		{
			if (!bHit) 
			{
				MyLib::AABB aabb;
				aabb.vtxMin = obj->GetVtxMin();
				aabb.vtxMax = obj->GetVtxMax();

				MyLib::Matrix mtx = obj->GetWorldMtx();
				float time = 0.0f;
				MyLib::Vector3 OBpos;

				bHit = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos);

				if (bHit)
				{// ����Ă�
					obj->SetState(CObjectX::STATE::STATE_EDIT);

					// �͂݃I�u�W�F�N�g
					m_pGrabObj = obj;

					if (m_pHandle != nullptr) {
						m_pHandle->SetPosition(m_pGrabObj->GetPosition());
					}

					if (m_pHandle == nullptr) {
						m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());
					}

					// �Ē͂ݔ���
					m_bReGrab = true;
				}
				else {
					obj->SetState(CObjectX::STATE::STATE_NONE);
				}
			}
			else {
				obj->SetState(CObjectX::STATE::STATE_NONE);
			}
		}

		if (m_pHandle != nullptr &&
			!m_pHandle->IsHoverHandle() &&
			!bHit) 
		{
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}
	}




	if (m_pHandle != nullptr && 
		m_pHandle->IsHoverHandle() &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseClicked(0))
	{// �n���h���Ƀz�o�[���N���b�N

		// �ړ��̌���
		m_moveAngle = m_pHandle->GetHoverAngle();
		m_pHandle->SetState(CHandle::State::STATE_GRAB);
		m_bGrabHandle = true;
	}


	if (m_bGrabHandle &&
		m_pGrabObj != nullptr)
	{
		MyLib::Vector3 pos = m_pGrabObj->GetPosition();
		MyLib::Vector3 rot = m_pGrabObj->GetRotation();
		MyLib::Vector3 scale = m_pGrabObj->GetScale();

		// �����擾
		CHandle::SEditHandleInfo info = m_pHandle->GetDiff(m_moveAngle);
		pos += info.pos;
		rot += info.rot;
		UtilFunc::Transformation::RotNormalize(rot);

		scale += info.scale;
		if (scale.x <= 0.1f) scale.x = 0.1f;
		if (scale.y <= 0.1f) scale.y = 0.1f;
		if (scale.z <= 0.1f) scale.z = 0.1f;

		m_pGrabObj->SetPosition(pos);
		m_pGrabObj->SetRotation(rot);
		m_pGrabObj->SetScale(scale);

		if (m_pGrabObj->GetCollisionLineBox().size() > 0) {
			for (auto& box : m_pGrabObj->GetCollisionLineBox())
			{
				box->SetPosition(m_pGrabObj->GetPosition() + pos);
			}
		}

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}
	}


	if (m_pHandle != nullptr &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseReleased(0))
	{// �����[�X

		m_bGrabHandle = false;
		m_pHandle->SetState(CHandle::State::STATE_NONE);
	}

	if (pKeyboard->GetTrigger(DIK_DELETE))
	{
		if (m_pHandle != nullptr)
		{
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}

		if (m_pGrabObj != nullptr)
		{
			m_pGrabObj->Kill();
			m_pGrabObj = nullptr;
		}

		// �����蔻��{�b�N�X����
		CreateBoxLine();
	}

}

//==========================================================================
// �n���h���؂�ւ�
//==========================================================================
void CEdit_Obstacle_Arrangment::ChangeHandle()
{
	bool bChange = false;

	// �ړ��{�^��
	if (ImGui::Button("Move", ImVec2(50.0f, 50.0f)))
	{
		m_HandleType = CHandle::HandleType::TYPE_MOVE;
		bChange = true;
	}
	ImGui::SameLine();

	// �X�P�[���{�^��
	if (ImGui::Button("Scale", ImVec2(50.0f, 50.0f)))
	{
		m_HandleType = CHandle::HandleType::TYPE_SCALE;
		bChange = true;
	}
	ImGui::SameLine();

	if (bChange)
	{
		// �폜
		if (m_pHandle != nullptr) {
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}

		// �n���h������
		if (m_pGrabObj != nullptr)
		{
			m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());
		}
	}

}

//==========================================================================
// �g�����X�t�H�[������
//==========================================================================
void CEdit_Obstacle_Arrangment::Transform()
{

	//=============================
	// �����ݒ�
	//=============================
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Transform"))
	{
		float windowWidth = 100.0f;
		const float  ROT_MOVE = D3DX_PI * 0.01f;
		const float  POS_MOVE = 0.5f;

		if (m_pGrabObj == nullptr)
		{
			ImGui::Text(":None Obj:");
			ImGui::TreePop();
			return;
		}

		// ���Z�b�g
		if (ImGui::Button("ALL RESET")) {
			m_pGrabObj->SetPosition(0.0f);
			m_pGrabObj->SetRotation(0.0f);
			m_pGrabObj->SetScale(1.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("POS RESET")) {
			m_pGrabObj->SetPosition(0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("ROT RESET")) {
			m_pGrabObj->SetRotation(0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("SCALE RESET")) {
			m_pGrabObj->SetScale(1.0f);
		}

		//=============================
		// �ʒu�ݒ�
		//=============================
		MyLib::Vector3 pos = m_pGrabObj->GetPosition();
		ImGui::Text("pos");
		ImGui::SameLine();

		// X
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// �ʒu�ݒ�
		m_pGrabObj->SetPosition(pos);
		if (m_pGrabObj->GetCollisionLineBox().size() > 0) {
			for (auto& box : m_pGrabObj->GetCollisionLineBox())
			{
				box->SetPosition(m_pGrabObj->GetPosition() + pos);
			}
		}
		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}


		//=============================
		// �����ݒ�
		//=============================
		MyLib::Vector3 rot = m_pGrabObj->GetRotation();
		ImGui::Text("rot");
		ImGui::SameLine();

		// X
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// �����ݒ�
		UtilFunc::Transformation::RotNormalize(rot);
		m_pGrabObj->SetRotation(rot);


		//=============================
		// �g�k�ݒ�
		//=============================
		MyLib::Vector3 scale = m_pGrabObj->GetScale();
		ImGui::Text("scale");
		ImGui::SameLine();

		// X
		ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &scale.x, 0.01f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &scale.y, 0.01f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &scale.z, 0.01f, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// �g�k�ݒ�
		m_pGrabObj->SetScale(scale);

		ImGui::TreePop();
	}
}


//==========================================================================
// ������
//==========================================================================
void CEdit_Obstacle_Collider::Init()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 5000.0f, 0.0f);
	for (const auto& info : vecInfo)
	{
		CObjectX* pObj = CObjectX::Create(info.modelFile, pos);
		pObj->SetType(CObject::TYPE::TYPE_OBJECTX);

		m_pObjX.push_back(pObj);

		pos.x += DISTANCE_OBJ;
	}

	// �����蔻��BOX����
	CreateBoxLine();
}

//==========================================================================
// �����蔻��{�b�N�X����
//==========================================================================
void CEdit_Obstacle_Collider::CreateBoxLine()
{
	// �����蔻��{�b�N�X�폜
	DeleteBoxLine();

	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// �����蔻��{�b�N�X�擾
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	for (auto& boxcollider : info.boxcolliders)
	{
		// AABB�ݒ�
		MyLib::AABB aabb = MyLib::AABB(boxcollider.vtxMin, boxcollider.vtxMax);

		// ��������
		CCollisionLine_Box* pBox;
		pBox = CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_pCollisionLineBox.push_back(pBox);

		// �ʒu�ݒ�
		boxcollider.TransformOffset(m_pObjX[m_nEditIdx]->GetWorldMtx());
		m_pCollisionLineBox.back()->SetPosition(boxcollider.GetMtx().GetWorldPosition());
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Obstacle_Collider::Update()
{
	// �G�f�B�b�g���j���[
	if (ImGui::TreeNode("Obstacle Edit"))
	{
		MenuBar();

		//***********************
		// ���݂̃L�[�ύX
		//***********************
		// [�X���C�_�[]��������C���f�b�N�X
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		// ���T�C�Y
		if (ImGui::TreeNode("Target"))
		{
			ImGui::SeparatorText("ChangeTarget Info");

			ImGui::SetNextItemWidth(140.0f);
			if (ImGui::SliderInt("Edit Idx", &m_nEditIdx, 0, m_pObjX.size() - 1))
			{
				CreateBoxLine();
			}
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Collider"))
		{
			ImGui::SeparatorText("ChangeCollider Info");

			// ��Q���}�l�[�W���擾
			CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
			CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

			// �����ύX
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Change Coolider Num:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left))
			{
				pObstacleMgr->SubCollider(m_nEditIdx);
				CreateBoxLine();
			}
			ImGui::SameLine(0.0f);
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				pObstacleMgr->AddCollider(m_nEditIdx);
				CreateBoxLine();
			}
			ImGui::SameLine();
			ImGui::Text("%d", info.boxcolliders.size());

			Resize();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}



	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetTargetPosition(m_pObjX[m_nEditIdx]->GetPosition());
	pCamera->SetAutoMovingPosR(m_pObjX[m_nEditIdx]->GetPosition());

	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	for (int i = 0; i < static_cast<int>(info.boxcolliders.size()); i++)
	{
		// BOX�R���C�_�[
		MyLib::Collider_BOX collider = info.boxcolliders[i];

		collider.TransformOffset(m_pObjX[m_nEditIdx]->GetWorldMtx());
		m_pCollisionLineBox[i]->SetPosition(collider.GetMtx().GetWorldPosition());
	}
}

//==========================================================================
// ���j���[�o�[����
//==========================================================================
void CEdit_Obstacle_Collider::MenuBar()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	


	// �R���{�{�b�N�X
	static const char* savetext[] = { "Save", "Save_as", "Load" };
	static int saveselect = 0;
	float width = 150.0f;

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save"))
	{
		pObstacleMgr->SaveInfo();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save_as"))
	{
		pObstacleMgr->SaveInfo();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{

	}



#if 0
	// �R���{�{�b�N�X
	static const char* items[] = { "Save", "Save_as", "Load" };
	int select = 0;
	if (ImGui::Combo(": Save & Load", &select, items, IM_ARRAYSIZE(items)))
	{
		// �I�����ꂽ���ڂ��ύX���ꂽ�Ƃ��̏���
		switch (select)
		{
		case 0:	// Save
		{
			pObstacleMgr->SaveInfo();
		}
			break;

		case 1:	// Save_as
		{
			OPENFILENAMEA filename = {};
			char sFilePass[1024] = {};
			// �t�@�C���I���_�C�A���O�̐ݒ�
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "�e�L�X�g�t�@�C��\0*.txt\0�摜�t�@�C��\0*.bmp;.jpg\0���ׂẴt�@�C��\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// �J�����g�f�B���N�g�����擾����
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"�t�H���_�̐�΃p�X�����߂�
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\mapobstacle";

			// ���݂���ꍇ�́AlpstrInitialDir�Ɏw�肷��
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// �t�@�C���I���_�C�A���O��\��
			if (GetOpenFileNameA(&filename))
			{
				// �I�����ꂽ�t�@�C���̃p�X��\��
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//�Z�[�u
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}
			break;

		case 2:	// Load
			
			break;
		}
	}
#endif

#if 0
	// �����o��
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save"))
		{
			pObstacleMgr->SaveInfo();
		}

		if (ImGui::MenuItem("Save_as"))
		{
			OPENFILENAMEA filename = {};
			char sFilePass[1024] = {};
			// �t�@�C���I���_�C�A���O�̐ݒ�
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "�e�L�X�g�t�@�C��\0*.txt\0�摜�t�@�C��\0*.bmp;.jpg\0���ׂẴt�@�C��\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// �J�����g�f�B���N�g�����擾����
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"�t�H���_�̐�΃p�X�����߂�
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\mapobstacle";

			// ���݂���ꍇ�́AlpstrInitialDir�Ɏw�肷��
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// �t�@�C���I���_�C�A���O��\��
			if (GetOpenFileNameA(&filename))
			{
				// �I�����ꂽ�t�@�C���̃p�X��\��
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//�Z�[�u
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}

		// ���[�h
		if (ImGui::MenuItem("Load"))
		{

		}

		ImGui::EndMenu();
	}
#endif

}

//==========================================================================
// ���T�C�Y
//==========================================================================
void CEdit_Obstacle_Collider::Resize()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	// [�X���C�_�[]��������R���C�_�[�̃C���f�b�N�X
	ImGui::SetNextItemWidth(140.0f);
	if (ImGui::SliderInt("Collider Idx", &m_nColliderIdx, 0, info.boxcolliders.size() - 1))
	{

	}

	float windowWidth = 100.0f;
	const float  POS_MOVE = 0.5f;

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	// ���T�C�Y
	if (ImGui::TreeNode("Resize"))
	{
		ImGui::SeparatorText("ChangeAABB Info");

		// BOX�R���C�_�[
		MyLib::Collider_BOX collider = info.boxcolliders[m_nColliderIdx];

		// ���Z�b�g
		if (ImGui::Button("ALL RESET")) {
			collider.vtxMin = 0.0f;
			collider.vtxMax = 0.0f;
			collider.offset = 0.0f;
		}

		if (ImGui::Button("VtxMax RESET")) {
			collider.vtxMax = 0.0f;
		}
		ImGui::SameLine();

		if (ImGui::Button("VtxMin RESET")) {
			collider.vtxMin = 0.0f;
		}
		ImGui::SameLine();

		if (ImGui::Button("Offset RESET")) {
			collider.offset = 0.0f;
		}

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		{
			ImGui::Text("VtxMax");
			ImGui::SameLine();
			// MaxX
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.vtxMax.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MaxY
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.vtxMax.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MaxZ
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.vtxMax.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		{
			ImGui::Text("VtxMin");
			ImGui::SameLine();
			// MinX
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.vtxMin.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MinY
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.vtxMin.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MinZ
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.vtxMin.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		{

			ImGui::Text("Offset");
			ImGui::SameLine();
			// X
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.offset.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.offset.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.offset.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		// BOX�ݒ�
		MyLib::AABB aabb = MyLib::AABB(collider.vtxMin, collider.vtxMax);
		m_pCollisionLineBox[m_nColliderIdx]->SetAABB(aabb);


		// BOX�R���C�_�[�ݒ�
		info.boxcolliders[m_nColliderIdx] = collider;
		pObstacleMgr->SetObstacleInfo(info, m_nEditIdx);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::TreePop();
	}
}

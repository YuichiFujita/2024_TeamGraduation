//=============================================================================
// 
//  �}�b�v�u���b�N�ł̔z�u���G�f�B�^�\���� [edit_mapblock.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_mapblock.h"
#include "edit_map.h"
#include "manager.h"
#include "calculation.h"
#include "camera.h"
#include "frontobj_manager.h"

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
CEdit_MapBlock::CEdit_MapBlock()
{
	// �l�̃N���A
	m_pEditControl = nullptr;	// ��ޑ���

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_MapBlock::~CEdit_MapBlock()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_MapBlock::Init()
{
	m_pEditControl = DEBUG_NEW CEdit_MapBlock_Arrangment;
	m_pEditControl->Init();

	// ����ύX
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_MapBlock::Uninit()
{
	if (m_pEditControl != nullptr)
	{
		m_pEditControl->DeleteBoxLine();
		m_pEditControl->Uninit();
		m_pEditControl = nullptr;
	}

	// �I������
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_MapBlock::Update()
{
	// �G�f�B�b�g�X�V
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	m_pEditControl->Update();
}

//==========================================================================
// ������
//==========================================================================
void CEdit_MapBlock_Arrangment::Init()
{
	m_FileName = CEdit_Map::GetModelFileName();
	// �����蔻��{�b�N�X����
	CreateBoxLine();
}

//==========================================================================
// �����蔻��{�b�N�X����
//==========================================================================
void CEdit_MapBlock_Arrangment::CreateBoxLine()
{
	// �����蔻��{�b�N�X�폜
	DeleteBoxLine();

	// ��Q���̃��X�g�擾
	CListManager<CObjectX> list = CObjectX::GetListObj();

	// �擪��ۑ�
	std::list<CObjectX*>::iterator itr = list.GetEnd();
	CObjectX* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CObjectX* pObj = *itr;
		// AABB�ݒ�
		MyLib::AABB aabb = pObj->GetAABB();

		// ��������
		CCollisionLine_Box* pBox;
		pBox = CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_pCollisionLineBox.push_back(pBox);
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_MapBlock_Arrangment::Update()
{

	// �R���{�{�b�N�X
	static const char* savetext[] = { "Save", "Save_as", "Load" };
	static int saveselect = 0;
	float width = 150.0f;



	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);


	std::vector<std::string> items;

	// �`�F�b�N�|�C���g�̏I��
	for (int i = 0; i < static_cast<int>(m_FileName.size()); i++)
	{
		std::string file = m_FileName[i];
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
		m_MapInfo.fileName = items[select];
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
		std::string file = UtilFunc::Transformation::RemoveFilePath(m_MapInfo.fileName);
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

			CObjectX* pObj = CObjectX::Create(m_MapInfo.fileName, setpos);
			pObj->CalWorldMtx();
			pObj->SetType(CObject::TYPE_OBJECTX);
			CFrontObjManager::GetInstance()->Regist(pObj);

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
	CListManager<CObjectX> list = CObjectX::GetListObj();

	// �擪��ۑ�
	std::list<CObjectX*>::iterator itr = list.GetEnd();
	CObjectX* pObj = nullptr;

	int i = 0;
	while (list.ListLoop(itr))
	{
		CObjectX* pObj = *itr;

		//for (auto& boxcollider : pObj->GetObstacleInfo().boxcolliders)
		//{
		//	// �ʒu�ݒ�
		//	boxcollider.TransformOffset(pObj->GetWorldMtx());

		//	if (m_pCollisionLineBox.size() > i)
		//	{
		//		m_pCollisionLineBox[i]->SetPosition(boxcollider.GetMtx().GetWorldPosition());
		//		m_pCollisionLineBox[i]->SetRotation(pObj->GetRotation());
		//	}
		//	i++;
		//}
	}

}

//==========================================================================
// �I�u�W�F�N�g�I��
//==========================================================================
void CEdit_MapBlock_Arrangment::ObjectSelect()
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();
	MyLib::Vector3 mouseOldWorldPos = pMouse->GetOldWorldPosition();
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	// ��Q���̃��X�g�擾
	CListManager<CObjectX> list = CObjectX::GetListObj();

	// �擪��ۑ�
	std::list<CObjectX*>::iterator itr = list.GetEnd();

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
		CObjectX* pObject = nullptr;

		// ���X�g�R�s�[
		std::vector<CObjectX*> pObjectSort;

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
			CFrontObjManager::GetInstance()->Erase(m_pGrabObj);
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
void CEdit_MapBlock_Arrangment::ChangeHandle()
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
void CEdit_MapBlock_Arrangment::Transform()
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


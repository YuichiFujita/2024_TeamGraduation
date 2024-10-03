//==========================================================================
// 
// Imgui�̃}�l�[�W�� [Imguimanager.cpp]
// Author : ���n�Ή�
// 
//==========================================================================
#include "Imguimanager.h"
#include "main.h"
#include "manager.h"

using namespace ImguiMgr;

LPDIRECT3DTEXTURE9 texture;

// ImGui�Ńe�N�X�`����\��
ImVec2 imageSize;

int textureWidth;
int textureHeight;

HWND backupHWnd;
LPDIRECT3DDEVICE9 device;

//==========================================================================
// ����������
//==========================================================================
void ImguiMgr::Init(HWND hwnd, LPDIRECT3DDEVICE9 pd3dDevice)
{
	backupHWnd = hwnd;
	device = pd3dDevice;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	// �����̐ݒ�
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// �f�B�X�v���C�T�C�Y�̐ݒ�
	io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

#if 0
	// �X�^�C���̐ݒ�
	ImGui::StyleColorsDark();

	// �^�C�g���o�[�̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// �X���C�h�̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// �`�F�b�N�}�[�N�̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// �X�N���[���̐F�ݒ�
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
#endif
	// �v���b�g�t�H�[���̐ݒ�
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pd3dDevice);



	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// �摜�̃��[�h
	HRESULT hr = D3DXCreateTextureFromFileEx(pDevive, "data\\TEXTURE\\forest_01.png", 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
		NULL, NULL, &texture);

	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	textureWidth = desc.Width;
	textureHeight = desc.Height;

	imageSize = ImVec2(50, 50);

	//if (SUCCEEDED(hr))
	//{
	//	// ImGui�Ńe�N�X�`����\��
	//	ImVec2 imageSize(static_cast<float>(50), static_cast<float>(50));

	//	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), imageSize))
	//	{
	//		// �h���b�O����̊J�n
	//		ImGui::SetDragDropPayload("MY_TEXTURE_TYPE", &texture, sizeof(LPDIRECT3DTEXTURE9));
	//	}

	//	// �e�N�X�`���̉��
	//	texture->Release();
	//}
}

//==========================================================================
// �I������
//==========================================================================
void ImguiMgr::Uninit()
{
	// ImGui�̏I��
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();
}

//==========================================================================
// �X�V����
//==========================================================================
void ImguiMgr::Update()
{
	// ImGui�t���[���̊J�n
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();


#if 0
	// ������Imgui��GUI�v�f��ǉ�
	ImGui::Text("Hello, ImGui!");
	ImGui::Separator();

	static int myVariable = 0;

	// �X���C�_�[���g�����ϐ��̒���
	ImGui::SliderInt("My Variable", &myVariable, 0, 100);

	// �������̓e�L�X�g�{�b�N�X���g�����ϐ��̒���
	ImGui::InputInt("My Variable", &myVariable, 1, 10);

	if (ImGui::Button("Click Me"))
	{
		// �{�^�����N���b�N���ꂽ�Ƃ��̏���
		//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}

	// ���ɂ��l�X��Imgui�̃E�B�W�F�b�g�����p�\
	// �`�F�b�N�{�b�N�X
	static bool myCheckbox = false;
	ImGui::Checkbox("Enable Feature", &myCheckbox);

	// ���W�I�{�^��
	static int myRadioSelection = 0; // 0, 1, 2, ...
	ImGui::RadioButton("Option 1", &myRadioSelection, 0);
	ImGui::RadioButton("Option 2", &myRadioSelection, 1);

	// �R���{�{�b�N�X
	static const char* items[] = { "Option 1", "Option 2", "Option 3" };
	static int selectedItem = 0;

	// �R���{�{�b�N�X
	if (ImGui::Combo("Select Options", &selectedItem, items, IM_ARRAYSIZE(items)))
	{
		// �I�����ꂽ���ڂ��ύX���ꂽ�Ƃ��̏���
		switch (selectedItem)
		{
		case 0:
			// Option 1 �̏���
			break;
		case 1:
			// Option 2 �̏���
			break;
		case 2:
			// Option 3 �̏���
			break;
		}
	}

	// �e�L�X�g
	ImGui::Text("This is a static text.");

	// �J���[�G�f�B�b�g
	static ImVec4 myColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // RGBA
	ImGui::ColorEdit4("Pick a Color", &myColor.x);

	// �摜�f�[�^�̃��[�h��`����@�̓v���W�F�N�g�ɂ��قȂ�܂�
	ImGui::Image(0, ImVec2(100, 100));


	// �e�L�X�g����
	static char buffer[256] = "Type here...";
	ImGui::InputText("Text Input", buffer, IM_ARRAYSIZE(buffer));

	// �E�B���h�E
	ImGui::Begin("My Window2");
	ImGui::Text("This is my window content.");
	ImGui::End();


	// ���j���[�o�[
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			// New ���j���[���I�����ꂽ�Ƃ��̏���
		}
		if (ImGui::MenuItem("Open"))
		{
			// Open ���j���[���I�����ꂽ�Ƃ��̏���
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit"))
	{
		// Edit ���j���[�̍��ڂ�ǉ�����
		ImGui::EndMenu();
	}
	// ���̃��j���[��ǉ�����
	ImGui::EndMainMenuBar();


	struct MyDragData {
		float x, y;
	};

	// �h���b�O�\�ȗv�f�̕`��

	// ImGui::IsMouseDragging(0);	// �h���b�O��
	
	if (ImGui::Button("Drag me"))
	{
		//// �h���b�O����̊J�n
		//ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", NULL, 0);
	}

	bool bDrag = false;
	if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
	{
		int n = 0;
		bDrag = true;
	}

	

	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
	//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
	if (ImGui::BeginDragDropSource(src_flags))
	{
		static MyDragData dragData = { 10.0f, 20.0f }; // ��Ƃ��č��W�f�[�^��ݒ�
		ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(MyDragData));
		ImGui::Text("KOKOOOOOO");
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		// �h���b�v���󂯓����ꂽ�ꍇ�̏���
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_COORDINATE_TYPE"))
		{
			// �h���b�v���󂯓����ꂽ�Ƃ��̏���
			static const MyDragData* receivedData = reinterpret_cast<const MyDragData*>(payload->Data);
			// receivedData���g�p���ĉ�������̏������s��
		}
	}

	// �E�B���h�E������
	/*if (ImGui::IsWindowHovered())
	{
		ImGui::Text("WINDOWNAI");
		int n = 0;
	}*/
	

	// �h���b�O�\�ȗv�f�̕`��
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), imageSize))
	{

	}


	if (ImGui::BeginDragDropSource(src_flags))
	{
		static MyDragData dragData = { 10.0f, 20.0f }; // ��Ƃ��č��W�f�[�^��ݒ�
		ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(MyDragData));
		ImGui::Text("KOKOOOOOO");
		ImGui::EndDragDropSource();
	}

	/*ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImGui::SetNextWindowPos(mousePos);
	ImGui::Begin("Preview Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::Text("Preview");
	ImGui::End();*/

	static ImVec2 s_RectanglePos = ImVec2(50, 50); // ��`�̏����ʒu

	//// �h���b�O�\�ȋ�`��`��
	//ImGui::SetCursorScreenPos(s_RectanglePos);
	//ImGui::Button("Draggable Rectangle", ImVec2(100, 50));

	//// �h���b�O�A���h�h���b�v�C�x���g������
	//if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
	//	// �h���b�O���̏���
	//	ImVec2 delta = ImGui::GetIO().MouseDelta;
	//	s_RectanglePos.x += delta.x;
	//	s_RectanglePos.y += delta.y;
	//}

	//ImDrawList* drawList = ImGui::GetWindowDrawList();
	//ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // �����x���w��
	//drawList->AddRectFilled(ImVec2(1280.0f, 720.0f), ImVec2(50.0f + 1280.0f, 50.0f + 720.0f), IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255));


	// �O���[�v
	if (ImGui::CollapsingHeader("My Group"))
	{
		ImGui::Text("Content inside the group.");

		// �`�F�b�N�{�b�N�X
		static bool myCheckbox = false;
		ImGui::Checkbox("Enable Feature", &myCheckbox);
	}



	//if (ImGui::Button(names[selectedIndex]))
	//{
	//	ImGui::OpenPopup("OptionsPopup");
	//}

	//// �|�b�v�A�b�v
	//static const char* edittype[] = { "Edit", "Play", "SetUp" };
	//static int type = 0;
	//ImGui::Combo("a", &type, edittype, IM_ARRAYSIZE(edittype));
	//m_editType = static_cast<CManager::EDITTYPE>(type);

	//const char* names[] = { "Option 1", "Option 2", "Option 3" };
	//int selectedIndex = 0;

	//if (ImGui::Button(names[selectedIndex]))
	//{
	//	ImGui::OpenPopup("OptionsPopup");
	//}

	//if (ImGui::BeginPopup("OptionsPopup"))
	//{
	//	for (int i = 0; i < IM_ARRAYSIZE(names); ++i)
	//	{
	//		if (ImGui::Selectable(names[i]))
	//		{
	//			selectedIndex = i;
	//		}
	//	}
	//	ImGui::EndPopup();
	//	}





	// ���j���[
	if (ImGui::BeginMenu("File"))
	{
		// �ʏ�
		if (ImGui::MenuItem("Open"))
		{
			/* ���j���[�A�C�e�����I�����ꂽ�Ƃ��̏��� */

			// �R���{�{�b�N�X
			static const char* items[] = { "Option 1", "Option 2", "Option 3" };
			static int selectedItem = 0;
			ImGui::Combo("Select Option", &selectedItem, items, IM_ARRAYSIZE(items));
		}

		if (ImGui::MenuItem("Save")) { /* ���j���[�A�C�e�����I�����ꂽ�Ƃ��̏��� */ }

		// �`�F�b�N�}�[�N�t��
		static bool option2_checked = false;
		if (ImGui::MenuItem("Option 2", nullptr, &option2_checked))
		{
			// Option 2 �̃`�F�b�N���؂�ւ�����Ƃ��̏���
		}

		if (ImGui::MenuItem("Option 3", "Ctrl+S"))
		{
			// Option 2 �̃`�F�b�N���؂�ւ�����Ƃ��̏���
			// �{�^�����N���b�N���ꂽ�Ƃ��̏���
			//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}

		ImGui::EndMenu();
	}
#endif

}

//==========================================================================
// �`�揈��
//==========================================================================
void ImguiMgr::Draw()
{
#if _DEBUG

	if (!CManager::IsDisp_ImGui())
	{
		ImGui::ShowDemoWindow();
	}
#endif

	// ImGui�t���[���̏I��
	ImGui::EndFrame();

	// ImGui�̕`��
	if (!CManager::IsDisp_ImGui())
	{
		// ImGui�̕`��
		ImGui::Render();

		// �`��R�}���h�̏���
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

//========================
//�o�b�N�A�b�v
//========================
void ImguiMgr::Backup()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

//========================
//����
//========================
void ImguiMgr::Restore()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}
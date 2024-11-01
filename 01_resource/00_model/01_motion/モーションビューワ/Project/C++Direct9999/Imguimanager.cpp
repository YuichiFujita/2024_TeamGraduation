//==================================================================================
// 
// Imgui�̃}�l�[�W�� [Imguimanager.cpp]
// Author : ���n�Ή�
// 
//==================================================================================
#include "Imguimanager.h"
#include "main.h"

using namespace ImguiMgr;

//==========================================================================
// ����������
//==========================================================================
void ImguiMgr::Init(HWND hwnd, LPDIRECT3DDEVICE9 pd3dDevice)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	// �����̐ݒ�
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	/*static ImWchar const glyph_ranges[] = {
	0x0020, 0xfffd,
	0,
	};
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, glyph_ranges);*/

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
}

//==========================================================================
// �I������
//==========================================================================
void ImguiMgr::Uninit(void)
{
	// ImGui�̏I��
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//==========================================================================
// �X�V����
//==========================================================================
void ImguiMgr::Update(void)
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


	struct MyDragData {
		float x, y;
	};

	// �h���b�O��
	if (ImGui::BeginDragDropSource())
	{
		static MyDragData dragData = { 10.0f, 20.0f }; // ��Ƃ��č��W�f�[�^��ݒ�
		ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(MyDragData));
		ImGui::Text("Drag me!");
		ImGui::EndDragDropSource();
	}

	// �h���b�v��
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_COORDINATE_TYPE"))
		{
			// �h���b�v���󂯓����ꂽ�Ƃ��̏���
			static const MyDragData* receivedData = reinterpret_cast<const MyDragData*>(payload->Data);
			// receivedData���g�p���ĉ�������̏������s��
		}
		ImGui::EndDragDropTarget();
	}


	// �O���[�v
	if (ImGui::CollapsingHeader("My Group"))
	{
		ImGui::Text("Content inside the group.");

		// �`�F�b�N�{�b�N�X
		static bool myCheckbox = false;
		ImGui::Checkbox("Enable Feature", &myCheckbox);
	}



	if (ImGui::Button(names[selectedIndex]))
	{
		ImGui::OpenPopup("OptionsPopup");
	}

	// �|�b�v�A�b�v
	static const char* edittype[] = { "Edit", "Play", "SetUp" };
	static int type = 0;
	ImGui::Combo("a", &type, edittype, IM_ARRAYSIZE(edittype));
	m_editType = static_cast<CManager::EDITTYPE>(type);

	const char* names[] = { "Option 1", "Option 2", "Option 3" };
	int selectedIndex = 0;

	if (ImGui::Button(names[selectedIndex]))
	{
		ImGui::OpenPopup("OptionsPopup");
	}

	if (ImGui::BeginPopup("OptionsPopup"))
	{
		for (int i = 0; i < IM_ARRAYSIZE(names); ++i)
		{
			if (ImGui::Selectable(names[i]))
			{
				selectedIndex = i;
			}
		}
		ImGui::EndPopup();
		}





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
		if (ImGui::MenuItem("Option 2", NULL, &option2_checked))
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
void ImguiMgr::Draw(void)
{

	// ImGui�t���[���̏I��
	ImGui::EndFrame();
	ImGui::Render();

	// �`��R�}���h�̏���
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}


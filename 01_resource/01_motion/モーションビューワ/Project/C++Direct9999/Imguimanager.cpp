//==================================================================================
// 
// Imguiのマネージャ [Imguimanager.cpp]
// Author : 相馬靜雅
// 
//==================================================================================
#include "Imguimanager.h"
#include "main.h"

using namespace ImguiMgr;

//==========================================================================
// 初期化処理
//==========================================================================
void ImguiMgr::Init(HWND hwnd, LPDIRECT3DDEVICE9 pd3dDevice)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	// 文字の設定
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	/*static ImWchar const glyph_ranges[] = {
	0x0020, 0xfffd,
	0,
	};
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, glyph_ranges);*/

	// ディスプレイサイズの設定
	io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));

#if 0
	// スタイルの設定
	ImGui::StyleColorsDark();

	// タイトルバーの色設定
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// スライドの色設定
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// チェックマークの色設定
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.615f, 0.215f, 0.341f, 1.0f));

	// スクロールの色設定
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
#endif
	// プラットフォームの設定
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pd3dDevice);
}

//==========================================================================
// 終了処理
//==========================================================================
void ImguiMgr::Uninit(void)
{
	// ImGuiの終了
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//==========================================================================
// 更新処理
//==========================================================================
void ImguiMgr::Update(void)
{
	// ImGuiフレームの開始
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

#if 0
	// ここにImguiのGUI要素を追加
	ImGui::Text("Hello, ImGui!");
	ImGui::Separator();

	static int myVariable = 0;

	// スライダーを使った変数の調整
	ImGui::SliderInt("My Variable", &myVariable, 0, 100);

	// もしくはテキストボックスを使った変数の調整
	ImGui::InputInt("My Variable", &myVariable, 1, 10);

	if (ImGui::Button("Click Me"))
	{
		// ボタンがクリックされたときの処理
		//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
	}

	// 他にも様々なImguiのウィジェットが利用可能
	// チェックボックス
	static bool myCheckbox = false;
	ImGui::Checkbox("Enable Feature", &myCheckbox);

	// ラジオボタン
	static int myRadioSelection = 0; // 0, 1, 2, ...
	ImGui::RadioButton("Option 1", &myRadioSelection, 0);
	ImGui::RadioButton("Option 2", &myRadioSelection, 1);

	// コンボボックス
	static const char* items[] = { "Option 1", "Option 2", "Option 3" };
	static int selectedItem = 0;

	// コンボボックス
	if (ImGui::Combo("Select Options", &selectedItem, items, IM_ARRAYSIZE(items)))
	{
		// 選択された項目が変更されたときの処理
		switch (selectedItem)
		{
		case 0:
			// Option 1 の処理
			break;
		case 1:
			// Option 2 の処理
			break;
		case 2:
			// Option 3 の処理
			break;
		}
	}

	// テキスト
	ImGui::Text("This is a static text.");

	// カラーエディット
	static ImVec4 myColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // RGBA
	ImGui::ColorEdit4("Pick a Color", &myColor.x);

	// 画像データのロードや描画方法はプロジェクトにより異なります
	ImGui::Image(0, ImVec2(100, 100));


	// テキスト入力
	static char buffer[256] = "Type here...";
	ImGui::InputText("Text Input", buffer, IM_ARRAYSIZE(buffer));

	// ウィンドウ
	ImGui::Begin("My Window2");
	ImGui::Text("This is my window content.");
	ImGui::End();


	struct MyDragData {
		float x, y;
	};

	// ドラッグ元
	if (ImGui::BeginDragDropSource())
	{
		static MyDragData dragData = { 10.0f, 20.0f }; // 例として座標データを設定
		ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(MyDragData));
		ImGui::Text("Drag me!");
		ImGui::EndDragDropSource();
	}

	// ドロップ先
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_COORDINATE_TYPE"))
		{
			// ドロップが受け入れられたときの処理
			static const MyDragData* receivedData = reinterpret_cast<const MyDragData*>(payload->Data);
			// receivedDataを使用して何かしらの処理を行う
		}
		ImGui::EndDragDropTarget();
	}


	// グループ
	if (ImGui::CollapsingHeader("My Group"))
	{
		ImGui::Text("Content inside the group.");

		// チェックボックス
		static bool myCheckbox = false;
		ImGui::Checkbox("Enable Feature", &myCheckbox);
	}



	if (ImGui::Button(names[selectedIndex]))
	{
		ImGui::OpenPopup("OptionsPopup");
	}

	// ポップアップ
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





	// メニュー
	if (ImGui::BeginMenu("File"))
	{
		// 通常
		if (ImGui::MenuItem("Open"))
		{
			/* メニューアイテムが選択されたときの処理 */

			// コンボボックス
			static const char* items[] = { "Option 1", "Option 2", "Option 3" };
			static int selectedItem = 0;
			ImGui::Combo("Select Option", &selectedItem, items, IM_ARRAYSIZE(items));
		}

		if (ImGui::MenuItem("Save")) { /* メニューアイテムが選択されたときの処理 */ }

		// チェックマーク付き
		static bool option2_checked = false;
		if (ImGui::MenuItem("Option 2", NULL, &option2_checked))
		{
			// Option 2 のチェックが切り替わったときの処理
		}

		if (ImGui::MenuItem("Option 3", "Ctrl+S"))
		{
			// Option 2 のチェックが切り替わったときの処理
			// ボタンがクリックされたときの処理
			//CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}

		ImGui::EndMenu();
	}
#endif

}

//==========================================================================
// 描画処理
//==========================================================================
void ImguiMgr::Draw(void)
{

	// ImGuiフレームの終了
	ImGui::EndFrame();
	ImGui::Render();

	// 描画コマンドの処理
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}


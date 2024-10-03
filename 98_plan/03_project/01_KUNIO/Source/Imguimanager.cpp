//==========================================================================
// 
// Imguiのマネージャ [Imguimanager.cpp]
// Author : 相馬靜雅
// 
//==========================================================================
#include "Imguimanager.h"
#include "main.h"
#include "manager.h"

using namespace ImguiMgr;

LPDIRECT3DTEXTURE9 texture;

// ImGuiでテクスチャを表示
ImVec2 imageSize;

int textureWidth;
int textureHeight;

HWND backupHWnd;
LPDIRECT3DDEVICE9 device;

//==========================================================================
// 初期化処理
//==========================================================================
void ImguiMgr::Init(HWND hwnd, LPDIRECT3DDEVICE9 pd3dDevice)
{
	backupHWnd = hwnd;
	device = pd3dDevice;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	// 文字の設定
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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



	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// 画像のロード
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
	//	// ImGuiでテクスチャを表示
	//	ImVec2 imageSize(static_cast<float>(50), static_cast<float>(50));

	//	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), imageSize))
	//	{
	//		// ドラッグ操作の開始
	//		ImGui::SetDragDropPayload("MY_TEXTURE_TYPE", &texture, sizeof(LPDIRECT3DTEXTURE9));
	//	}

	//	// テクスチャの解放
	//	texture->Release();
	//}
}

//==========================================================================
// 終了処理
//==========================================================================
void ImguiMgr::Uninit()
{
	// ImGuiの終了
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();
}

//==========================================================================
// 更新処理
//==========================================================================
void ImguiMgr::Update()
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


	// メニューバー
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			// New メニューが選択されたときの処理
		}
		if (ImGui::MenuItem("Open"))
		{
			// Open メニューが選択されたときの処理
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit"))
	{
		// Edit メニューの項目を追加する
		ImGui::EndMenu();
	}
	// 他のメニューを追加する
	ImGui::EndMainMenuBar();


	struct MyDragData {
		float x, y;
	};

	// ドラッグ可能な要素の描画

	// ImGui::IsMouseDragging(0);	// ドラッグ中
	
	if (ImGui::Button("Drag me"))
	{
		//// ドラッグ操作の開始
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
		static MyDragData dragData = { 10.0f, 20.0f }; // 例として座標データを設定
		ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(MyDragData));
		ImGui::Text("KOKOOOOOO");
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		// ドロップが受け入れられた場合の処理
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_COORDINATE_TYPE"))
		{
			// ドロップが受け入れられたときの処理
			static const MyDragData* receivedData = reinterpret_cast<const MyDragData*>(payload->Data);
			// receivedDataを使用して何かしらの処理を行う
		}
	}

	// ウィンドウ内判定
	/*if (ImGui::IsWindowHovered())
	{
		ImGui::Text("WINDOWNAI");
		int n = 0;
	}*/
	

	// ドラッグ可能な要素の描画
	if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture), imageSize))
	{

	}


	if (ImGui::BeginDragDropSource(src_flags))
	{
		static MyDragData dragData = { 10.0f, 20.0f }; // 例として座標データを設定
		ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(MyDragData));
		ImGui::Text("KOKOOOOOO");
		ImGui::EndDragDropSource();
	}

	/*ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImGui::SetNextWindowPos(mousePos);
	ImGui::Begin("Preview Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::Text("Preview");
	ImGui::End();*/

	static ImVec2 s_RectanglePos = ImVec2(50, 50); // 矩形の初期位置

	//// ドラッグ可能な矩形を描画
	//ImGui::SetCursorScreenPos(s_RectanglePos);
	//ImGui::Button("Draggable Rectangle", ImVec2(100, 50));

	//// ドラッグアンドドロップイベントを処理
	//if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
	//	// ドラッグ中の処理
	//	ImVec2 delta = ImGui::GetIO().MouseDelta;
	//	s_RectanglePos.x += delta.x;
	//	s_RectanglePos.y += delta.y;
	//}

	//ImDrawList* drawList = ImGui::GetWindowDrawList();
	//ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 透明度を指定
	//drawList->AddRectFilled(ImVec2(1280.0f, 720.0f), ImVec2(50.0f + 1280.0f, 50.0f + 720.0f), IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255));


	// グループ
	if (ImGui::CollapsingHeader("My Group"))
	{
		ImGui::Text("Content inside the group.");

		// チェックボックス
		static bool myCheckbox = false;
		ImGui::Checkbox("Enable Feature", &myCheckbox);
	}



	//if (ImGui::Button(names[selectedIndex]))
	//{
	//	ImGui::OpenPopup("OptionsPopup");
	//}

	//// ポップアップ
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
		if (ImGui::MenuItem("Option 2", nullptr, &option2_checked))
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
void ImguiMgr::Draw()
{
#if _DEBUG

	if (!CManager::IsDisp_ImGui())
	{
		ImGui::ShowDemoWindow();
	}
#endif

	// ImGuiフレームの終了
	ImGui::EndFrame();

	// ImGuiの描画
	if (!CManager::IsDisp_ImGui())
	{
		// ImGuiの描画
		ImGui::Render();

		// 描画コマンドの処理
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}

//========================
//バックアップ
//========================
void ImguiMgr::Backup()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

//========================
//復元
//========================
void ImguiMgr::Restore()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}
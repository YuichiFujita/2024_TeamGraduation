//=============================================================================
// 
//  マップエディタ―処理 [edit_map.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_map.h"
#include "manager.h"
#include "calculation.h"
#include "collisionLine_Box.h"
#include "camera.h"
#include "map.h"
#include "object3DMesh.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string SAVETEXT = "data\\TEXT\\map\\save_info.txt";
	const std::string TEXTURE_HANDLE[] =							// テクスチャのファイル
	{
		"data\\TEXTURE\\handle\\move.png",
		"data\\TEXTURE\\handle\\scale.png",
		"data\\TEXTURE\\handle\\rotation.png",
	};
}
CListManager<CObjectX> CEdit_Map::m_List = {};				// リスト
std::vector<std::string> CEdit_Map::m_TextureFile = {};		// テクスチャファイル
std::vector<std::string> CEdit_Map::m_ModelFile = {};		// モデルファイル
bool CEdit_Map::m_bLoad = false;							// 読み込み判定
std::vector<CObjectX*> CEdit_Map::m_pObjX = {};			// オブジェクトXのポインタ


//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_Map::CEdit_Map()
{
	// 値のクリア
	m_nModelIdx.clear();	// モデルインデックス
	//m_pObjX.clear();		// オブジェクトXのポインタ
	m_bGrab = false;		// 掴み判定
	m_bReGrab = false;		// 再掴み判定
	m_pCopyObj = nullptr;	// コピーオブジェクト
	m_pGrabObj = nullptr;	// 掴みオブジェクト
	m_pHandle = nullptr;	// 移動ハンドル
	m_HandleType = CHandle::HandleType::TYPE_MOVE;	// ハンドルの種類
	m_moveAngle = CHandle::HandleAngle::ANGLE_X;	// 移動の向き
	memset(m_HandleTex, 0, sizeof(m_HandleTex));// テクスチャのポインタ
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_Map::~CEdit_Map()
{
	
}

//==========================================================================
// 生成
//==========================================================================
CEdit_Map* CEdit_Map::Create(const std::string& file, CManager::BuildMode mode)
{
	CEdit_Map* pEdit = nullptr;

	switch (mode)
	{
	case CManager::MODE_DEBUG:
		pEdit = DEBUG_NEW CEdit_Map;
		break;

	case CManager::MODE_RELEASE:
		pEdit = DEBUG_NEW CEdit_Map_Release;
		break;
	}

	if (pEdit != nullptr)
	{
		pEdit->m_Filename = file;
		pEdit->Init();
	}

	return pEdit;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_Map::Init()
{
	// ロード状況保存
	bool bLoad = m_bLoad;

#if _DEBUG
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 画像のロード
	for (const auto& file : m_ModelFile)
	{
		if (bLoad) {
			m_nModelIdx.push_back(0);

			// インデックス取得
			m_nModelIdx.back() = CXLoad::GetInstance()->XLoad(file);
		}
		
		m_pTexture.emplace_back();

		HRESULT hr = D3DXCreateTextureFromFileEx(pDevive, file.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
			NULL, NULL, &m_pTexture.back());
	}

	int max = static_cast<int>(CHandle::HandleType::TYPE_MAX);
	for (int i = 0; i < max; i++)
	{
		D3DXCreateTextureFromFileEx(pDevive, TEXTURE_HANDLE[i].c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
			NULL, NULL, &m_HandleTex[i]);
	}
#endif
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_Map::Uninit()
{
	for (const auto& texture : m_pTexture)
	{
		if (texture != nullptr) {
			texture->Release();
		}
	}
	m_pTexture.clear();

	int max = static_cast<int>(CHandle::HandleType::TYPE_MAX);
	for (int i = 0; i < max; i++)
	{
		if (m_HandleTex[i] == nullptr) {
			continue;
		}

		m_HandleTex[i]->Release();
		m_HandleTex[i] = nullptr;
	}

	//m_List.Uninit();

	std::string().swap(m_Filename);
	std::vector<int>().swap(m_nModelIdx); // モデルインデックス削除（vectorの問題でこうしないとメモリリークを起こすみたい）
	m_pObjX.clear();		// オブジェクトXのポインタ

	// 終了処理
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_Map::Update()
{
#if _DEBUG
	// エディットメニュー
	if (ImGui::CollapsingHeader("MapEdit"))
	{
		// メニューバー処理
		MenuBar();

		// オブジェクトボタン設定
		SetObjectButton();

		// ウィンドウのマウスホバー判定
		ImGuiHoveredFlags frag = 128;
		m_bHoverWindow = ImGui::IsWindowHovered(frag);
	}


	// ImGuiの操作
	EditImGui();

	// 掴み中処理
	Grab();

	// オブジェクト選択中
	SelectObject();

	// 再移動中
	Remove();

#endif
}

//==========================================================================
// メニューバー処理
//==========================================================================
void CEdit_Map::MenuBar()
{

	if (ImGui::TreeNode("File"))
	{
		float width = 150.0f;

		ImGui::SetNextItemWidth(width);
		if (ImGui::Button("Save"))
		{
			Save();
		}
		ImGui::SameLine();

		ImGui::SetNextItemWidth(width);
		if (ImGui::Button("Save_as"))
		{
			OPENFILENAMEA filename = {};
			char sFilePass[1024] = {};
			// ファイル選択ダイアログの設定
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "テキストファイル\0*.txt\0画像ファイル\0*.bmp;.jpg\0すべてのファイル\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// カレントディレクトリを取得する
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"フォルダの絶対パスを求める
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\map";

			// 存在する場合は、lpstrInitialDirに指定する
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// ファイル選択ダイアログを表示
			if (GetOpenFileNameA(&filename))
			{
				// 選択されたファイルのパスを表示
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//セーブ
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}
		ImGui::SameLine();

		ImGui::SetNextItemWidth(width);
		if (ImGui::Button("Load"))
		{

		}

		ImGui::TreePop();
	}
}

//==========================================================================
// オブジェクトボタン設定
//==========================================================================
void CEdit_Map::SetObjectButton()
{
	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
	//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

	ImVec2 imageSize = ImVec2(50, 50);
	for (int i = 0; i < static_cast<int>(m_nModelIdx.size()); i++)
	{
		ImGui::PushID(i); // ウィジェットごとに異なるIDを割り当てる
		{
			if (i % 5 != 0) {
				ImGui::SameLine();
			}

			// ドラッグ可能な要素の描画
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_pTexture[i]), imageSize);

			if (ImGui::BeginDragDropSource(src_flags))
			{
				SDragDropData dragData;
				dragData.nType = m_nModelIdx[i];
				m_DragData.nType = i;

				ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(SDragDropData));

				std::string file = UtilFunc::Transformation::RemoveFilePath(m_ModelFile[i]);
				ImGui::Text(file.c_str());

				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseDown(0))
			{// UI上にカーソル && クリック
				m_bGrab = true;
			}
		}
		ImGui::PopID();
	}
}

//==========================================================================
// ImGuiの操作
//==========================================================================
void CEdit_Map::EditImGui()
{
	ImVec2 imageSize = ImVec2(50, 50);

	//=============================
	// Imgui設定
	//=============================
	if (m_pGrabObj != nullptr) {

		ImGui::Begin("Transform");
		//ImGui::Begin("Transform", NULL, ImGuiWindowFlags_MenuBar);


		// 移動ボタン
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_HandleTex[CHandle::HandleType::TYPE_MOVE]), imageSize))
		{
			m_HandleType = CHandle::HandleType::TYPE_MOVE;
			ChangeHandle();
		}
		ImGui::SameLine();

		// スケールボタン
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_HandleTex[CHandle::HandleType::TYPE_SCALE]), imageSize))
		{
			m_HandleType = CHandle::HandleType::TYPE_SCALE;
			ChangeHandle();
		}
		ImGui::SameLine();


		if (!m_bHoverWindow) {
			ImGuiHoveredFlags frag = 128;
			m_bHoverWindow = ImGui::IsWindowHovered(frag);
		}

		//=============================
		// 向き設定
		//=============================
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		float windowWidth = 100.0f;
		const float  ROT_MOVE = D3DX_PI * 0.01f;
		const float  POS_MOVE = 0.5f;

		//if (ImGui::CollapsingHeader("Transform"))
		{
			//// チェックボックスを表示
			//unsigned int checkboxFlags = 0;
			//static bool readOnlyCheckboxState = false; // チェックボックスの読み取り専用状態

			////ImGui::CheckboxFlags("チェックボックス", &checkboxFlags, 1 << 0, ImGuiCheckboxFlags_No ImGuiCheckboxFlags_NoCheckOnClick);


			//// チェックボックスを表示し、読み取り専用状態であれば無効化する
			//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, readOnlyCheckboxState); // 読み取り専用状態ならチェックボックスを無効化
			//ImGui::Checkbox("読み取り専用チェックボックス", &readOnlyCheckboxState);
			//ImGui::PopItemFlag(); // チェックボックスの無効化を解除


			// リセット
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
			// 位置設定
			//=============================
			MyLib::Vector3 pos = m_pGrabObj->GetPosition();

			// X
			ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// 位置設定
			m_pGrabObj->SetPosition(pos);
			if (m_pGrabObj->GetCollisionLineBox() != nullptr) {
				m_pGrabObj->GetCollisionLineBox()->SetPosition(pos);
			}
			if (m_pHandle != nullptr) {
				m_pHandle->SetPosition(m_pGrabObj->GetPosition());
			}


			//=============================
			// 向き設定
			//=============================
			MyLib::Vector3 rot = m_pGrabObj->GetRotation();

			// X
			ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// 向き設定
			UtilFunc::Transformation::RotNormalize(rot);
			m_pGrabObj->SetRotation(rot);


			//=============================
			// 拡縮設定
			//=============================
			MyLib::Vector3 scale = m_pGrabObj->GetScale();

			// X
			ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &scale.x, 0.01f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &scale.y, 0.01f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &scale.z, 0.01f, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// 拡縮設定
			m_pGrabObj->SetScale(scale);
		}
		ImGui::End();
	}
}

//==========================================================================
// 掴み中処理
//==========================================================================
void CEdit_Map::Grab()
{
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();

	if (m_bGrab &&
		!m_bHoverWindow) {

		// ウィンドウ外 && オブジェクト無い状態で生成
		if (m_DragData.objX == nullptr)
		{
			m_DragData.objX = CObjectX::Create(m_nModelIdx[m_DragData.nType], mouseWorldPos, MyLib::Vector3(0.0f), true);
			m_DragData.objX->SetType(CObject::TYPE_XFILE);
			m_DragData.objX->CreateCollisionBox();
		}
	}
	else {

		// ウィンドウ内 && オブジェクトある状態で削除
		if (m_DragData.objX != nullptr) {
			m_DragData.objX->Kill();
			m_DragData.objX = nullptr;
		}
	}

	// ドラッグ中位置更新
	if (m_DragData.objX != nullptr) {
		m_DragData.objX->SetState(CObjectX::STATE::STATE_EDIT);

		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;
		m_DragData.objX->SetPosition(setpos);

		if (m_DragData.objX->GetCollisionLineBox() != nullptr) {
			m_DragData.objX->GetCollisionLineBox()->SetPosition(setpos);
		}
	}

	// 配置
	if (m_bGrab &&
		ImGui::IsMouseReleased(0))
	{// 掴み中 && マウスリリース

		if (!m_bHoverWindow) {

			MyLib::Vector3 setpos = mouseWorldPos;
			setpos.y = 0.0f;
			Regist(m_DragData.nType, setpos, 0.0f, 1.0f, true);
		}
		m_bGrab = false;
	}
}

//==========================================================================
// オブジェクト選択中
//==========================================================================
void CEdit_Map::SelectObject()
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();

	// オブジェクト選択中
	if (!m_bGrab &&
		m_pHandle != nullptr &&
		!m_bReGrab)
	{
		if ((pKeyboard->GetPress(DIK_LCONTROL) || pKeyboard->GetPress(DIK_RCONTROL)) &&
			pKeyboard->GetTrigger(DIK_C))
		{// コピー
			m_pCopyObj = m_pGrabObj;
		}

		if (m_pCopyObj != nullptr &&
			(pKeyboard->GetPress(DIK_LCONTROL) || pKeyboard->GetPress(DIK_RCONTROL)) &&
			pKeyboard->GetTrigger(DIK_V))
		{// ペースト

			// モデルインデックス検索
			std::vector<std::string>::iterator file = std::find(m_ModelFile.begin(), m_ModelFile.end(), m_pCopyObj->GetFileName());
			int nIdx = file - m_ModelFile.begin();

			Regist(nIdx, 0.0f, 0.0f, 1.0f, true);
		}

		if (m_pHandle->IsHoverHandle() &&
			!pKeyboard->GetPress(DIK_LALT) &&
			ImGui::IsMouseClicked(0))
		{// ホバー中クリック

			m_bReGrab = true;

			// 移動の向き
			m_moveAngle = m_pHandle->GetHoverAngle();
			m_pHandle->SetState(CHandle::State::STATE_GRAB);
			return;
		}

		if (pKeyboard->GetTrigger(DIK_DELETE))
		{// 削除
			Delete(m_pGrabObj);
			m_pGrabObj = nullptr;

			if (m_pHandle != nullptr) {
				m_pHandle->Kill();
				m_pHandle = nullptr;
			}
		}
	}
}

//==========================================================================
// 再移動
//==========================================================================
void CEdit_Map::Remove()
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();
	MyLib::Vector3 mouseOldWorldPos = pMouse->GetOldWorldPosition();
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	if (m_bReGrab &&
		m_pGrabObj != nullptr)
	{
		MyLib::Vector3 pos = m_pGrabObj->GetPosition();
		MyLib::Vector3 rot = m_pGrabObj->GetRotation();
		MyLib::Vector3 scale = m_pGrabObj->GetScale();

		// 差分取得
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

		if (m_pGrabObj->GetCollisionLineBox() != nullptr) {
			m_pGrabObj->GetCollisionLineBox()->SetPosition(pos);
		}

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}
	}


	if (m_pHandle != nullptr &&
		m_bReGrab &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseReleased(0))
	{// リリース
		m_bReGrab = false;
		m_pHandle->SetState(CHandle::State::STATE_NONE);
	}

	if (!m_bReGrab &&
		!pKeyboard->GetPress(DIK_LALT) &&
		!m_bHoverWindow &&
		ImGui::IsMouseClicked(0))
	{// クリック

		m_pGrabObj = nullptr;

		// 先頭を保存
		CObjectX* pObject = nullptr;

		// リストコピー
		std::vector<CObjectX*> pObjectSort;

		while (m_List.ListLoop(&pObject))
		{
			// 要素を末尾に追加
			pObjectSort.push_back(pObject);
		}

		// Zソート
		std::sort(pObjectSort.begin(), pObjectSort.end(), CObject::ZSortInverse);

		bool bHit = false;
		for (const auto& obj : pObjectSort)
		{
			if (!bHit) {

				MyLib::AABB aabb;
				aabb.vtxMin = obj->GetVtxMin();
				aabb.vtxMax = obj->GetVtxMax();

				MyLib::Matrix mtx = obj->GetWorldMtx();
				float time = 0.0f;
				MyLib::Vector3 OBpos;

				bHit = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos);

				if (bHit)
				{// 被ってる
					obj->SetState(CObjectX::STATE::STATE_EDIT);

					// 掴みオブジェクト
					m_pGrabObj = obj;

					if (m_pHandle != nullptr) {
						m_pHandle->SetPosition(m_pGrabObj->GetPosition());
					}

					if (m_pHandle == nullptr) {
						m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());
					}
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
			!bHit && m_pHandle != nullptr) {
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}
	}
}

//==========================================================================
// ハンドル切り替え
//==========================================================================
void CEdit_Map::ChangeHandle()
{
	// 削除
	if (m_pHandle != nullptr) {
		m_pHandle->Kill();
		m_pHandle = nullptr;
	}

	// ハンドル生成
	m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());

}

//==========================================================================
// セーブ
//==========================================================================
void CEdit_Map::Save()
{
	// ファイルを開く
	FILE* pFile = fopen(SAVETEXT.c_str(), "w");
	if (pFile == nullptr)
	{// ファイルが開けなかった場合
		return;
	}


	// テクスチャ取得
	CTexture* pTexture = CTexture::GetInstance();

	std::vector<std::string> TextureFile = MyMap::GetTextureFileNameAll();

	// ファイルに書き出す
	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# テクスチャ数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_TEXTURE = %d\n\n", static_cast<int>(TextureFile.size()));

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# テクスチャファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// テクスチャファイル名分
	for (int nCntFileNum = 0; nCntFileNum < static_cast<int>(TextureFile.size()); nCntFileNum++)
	{
		fprintf(pFile, "TEXTURE_FILENAME = %s\t\t# [%d]\n", &TextureFile[nCntFileNum][0], nCntFileNum);
	}

	// ファイルに書き出す
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# モデル数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", (int)m_nModelIdx.size());

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# モデルファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// モデルファイル名分
	for (int i = 0; i < (int)m_nModelIdx.size(); i++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", m_ModelFile[i].c_str(), i);
	}



	//=============================
	// 空配置
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# 空情報\n"
		"#==============================================================================\n");


	std::map<CObject::LAYER, std::map<int, std::vector<CObject*>>> objects = CObject::GetTop();
	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 種類の取得
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHDOME)
				{// メッシュドームだったら

					// Xファイルの情報取得
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類

					// テクスチャ名
					std::vector<std::string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					// 出力
					fprintf(pFile,
						"SKYSET\n"
						"\tTEXTYPE = %d\n"
						"\tMOVE = %f\n"
						"END_SKYSET\n\n",
						nType, 0.0002f);
				}
			}
		}
	}


	//=============================
	// 山配置
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# 山情報\n"
		"#==============================================================================\n");

	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 種類の取得
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHCYLINDER)
				{// メッシュシリンダーだったら

					// Xファイルの情報取得
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類

					// テクスチャ名
					std::vector<std::string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					// 出力
					fprintf(pFile,
						"MOUNTAINSET\n"
						"\tTEXTYPE = %d\n"
						"END_MOUNTAINSET\n\n", nType);
				}
			}
		}
	}


	//=============================
	// 地面配置
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# 地面情報\n"
		"#==============================================================================\n");


	for (const auto& layer : objects)
	{
		for (const auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// 種類の取得
				CObject::TYPE TargetType = obj->GetType();

				if (TargetType == CObject::TYPE_MESHFIELD)
				{// メッシュフィールドだったら

					// Xファイルの情報取得
					CObject3DMesh* pObjMesh = obj->GetObject3DMesh();

					// テクスチャのインデックス番号
					int nType = pObjMesh->GetIdxTex();	// 種類

					// テクスチャ名
					std::vector<std::string>::iterator itr = find(TextureFile.begin(), TextureFile.end(), pTexture->GetTextureInfo(nType).filename);
					nType = static_cast<int>(std::distance(TextureFile.begin(), itr));

					MyLib::Vector3 pos = pObjMesh->GetPosition();		// 位置
					MyLib::Vector3 rot = pObjMesh->GetRotation();		// 向き
					int nWidth = pObjMesh->GetWidthBlock();			// 横分割数
					int nHeight = pObjMesh->GetHeightBlock();		// 縦分割数
					float fWidthLen = pObjMesh->GetWidthLen();		// 横長さ
					float fHeightLen = pObjMesh->GetHeightLen();	// 縦長さ

					// 出力
					fprintf(pFile,
						"FIELDSET\n"
						"\tTEXTYPE = %d\n"
						"\tPOS = %.0f %.0f %.0f\n"
						"\tROT = %.0f %.0f %.0f\n"
						"\tBLOCK = %d %d\n"
						"\tSIZE = %.0f %.0f\n"
						"END_FIELDSET\n\n",
						nType, pos.x, pos.y, pos.z,
						rot.x, rot.y, rot.z,
						nWidth, nHeight,
						fWidthLen, fHeightLen);
				}
			}
		}
	}


	//=============================
	// モデル
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# モデルの配置\n"
		"#==============================================================================\n");

	// リストループ
	CObjectX* pObj = nullptr;
	while (m_List.ListLoop(&pObj))
	{
		MyLib::Vector3 pos = pObj->GetPosition();	// 位置
		MyLib::Vector3 rot = pObj->GetRotation();	// 向き
		MyLib::Vector3 scale = pObj->GetScale();	// スケール
		int nShadow = 0;							// 影使うかどうか
		int nType = 0;								// 種類

		if (pObj->GetUseShadow())
		{// 使っている場合
			nShadow = 1;
		}
		
		// モデルインデックス検索
		std::vector<std::string>::iterator file = std::find(m_ModelFile.begin(), m_ModelFile.end(), pObj->GetFileName());
		int nIdx = file - m_ModelFile.begin();
		// 出力
		fprintf(pFile,
			"MODELSET\n"
			"\tTYPE = %d\n"
			"\tPOS = %.2f %.2f %.2f\n"
			"\tROT = %.2f %.2f %.2f\n"
			"\tSCALE = %.2f\n"
			"\tSHADOW = %d\n"
			"END_MODELSET\n\n",
			nIdx, pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z,
			scale.x, nShadow);
	}

	fprintf(pFile, "\nEND_SCRIPT		# この行は絶対消さないこと！");

	//ファイルを閉じる
	fclose(pFile);
}

//==========================================================================
// ロード
//==========================================================================
void CEdit_Map::Load(const std::string& file)
{
	// 再読み込み回避
	//if (m_bLoad) return;

	char aComment[MAX_COMMENT] = {};	//コメント用
	int nFileNum = 0;					// ファイルの数
	int nCntTexture = 0;				// テクスチャ読み込みカウント

	// テクスチャファイル読み込み
	ReadTexture(file);

	// Xファイル読み込み
	ReadXFile(file);

	// マップファイルを開く
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{//ファイルが開けた場合
		return;
	}

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);
//
//#ifndef _DEBUG
//		// メッシュフィールドの設定
//		if (strcmp(&aComment[0], "FIELDSET") == 0)
//		{// モデルの読み込みを開始
//
//			MyLib::Vector3 pos, rot;
//			int width, height;
//			float widthlen, heightlen;
//			int type;
//			while (strcmp(&aComment[0], "END_FIELDSET"))
//			{// END_FIELDSETが来るまで繰り返し
//
//				fscanf(pFile, "%s", &aComment[0]);	// 確認する
//
//				if (strcmp(&aComment[0], "TEXTYPE") == 0)
//				{// TEXTYPEが来たら種類読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);	// =の分
//					fscanf(pFile, "%d", &type);		// モデル種類の列挙
//				}
//
//				if (strcmp(&aComment[0], "POS") == 0)
//				{// POSが来たら位置読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);	// =の分
//					fscanf(pFile, "%f", &pos.x);		// X座標
//					fscanf(pFile, "%f", &pos.y);		// Y座標
//					fscanf(pFile, "%f", &pos.z);		// Z座標
//				}
//
//				if (strcmp(&aComment[0], "ROT") == 0)
//				{// ROTが来たら向き読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);		 // =の分
//					fscanf(pFile, "%f", &rot.x);		 // Xの向き
//					fscanf(pFile, "%f", &rot.y);		 // Yの向き
//					fscanf(pFile, "%f", &rot.z);		 // Zの向き
//					rot.x = D3DXToRadian(rot.x); // 360度形式から変換
//					rot.y = D3DXToRadian(rot.y); // 360度形式から変換
//					rot.z = D3DXToRadian(rot.z); // 360度形式から変換
//				}
//
//				if (strcmp(&aComment[0], "BLOCK") == 0)
//				{//BLOCKが来たら向き読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);		// =の分
//					fscanf(pFile, "%d", &width);		// 横の分割数
//					fscanf(pFile, "%d", &height);	// 縦の分割数
//				}
//
//				if (strcmp(&aComment[0], "SIZE") == 0)
//				{//SIZEが来たら向き読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);		// =の分
//					fscanf(pFile, "%f", &widthlen);		// 横の長さ
//					fscanf(pFile, "%f", &heightlen);	// 縦の長さ
//				}
//
//			}// END_FIELDSETのかっこ
//
//			//**********************************
//			// 生成処理
//			//**********************************
//			CMeshField::Create(
//				pos, rot,
//				widthlen, heightlen,
//				width, height,
//				(CMeshField::TYPE)type, m_TextureFile[type].c_str());
//		}
//
//		// メッシュシリンダーの設定
//		if (strcmp(&aComment[0], "MOUNTAINSET") == 0)
//		{// モデルの読み込みを開始
//
//			int type;
//			while (strcmp(&aComment[0], "END_MOUNTAINSET"))
//			{// END_MOUNTAINSETが来るまで繰り返し
//
//				fscanf(pFile, "%s", &aComment[0]);	// 確認する
//
//				if (strcmp(&aComment[0], "TEXTYPE") == 0)
//				{// TEXTYPEが来たら種類読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);	// =の分
//					fscanf(pFile, "%d", &type);	// モデル種類の列挙
//				}
//
//			}// END_MOUNTAINSETのかっこ
//
//			//**********************************
//			// 生成処理
//			//**********************************
//			CMeshCylinder::Create(m_TextureFile[type].c_str());
//
//		}
//
//		// メッシュドームの設定
//		if (strcmp(&aComment[0], "SKYSET") == 0)
//		{// モデルの読み込みを開始
//
//			int type;
//			float move;
//			while (strcmp(&aComment[0], "END_SKYSET"))
//			{// END_MOUNTAINSETが来るまで繰り返し
//
//				fscanf(pFile, "%s", &aComment[0]);	// 確認する
//
//				if (strcmp(&aComment[0], "TEXTYPE") == 0)
//				{// TEXTYPEが来たら種類読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);	// =の分
//					fscanf(pFile, "%d", &type);		// モデル種類の列挙
//				}
//
//				if (strcmp(&aComment[0], "MOVE") == 0)
//				{//MOVEが来たら移動読み込み
//
//					fscanf(pFile, "%s", &aComment[0]);	// =の分
//					fscanf(pFile, "%f", &move);			// 移動量
//				}
//
//			}// END_SKYSETのかっこ
//
//			//**********************************
//			// 生成処理
//			//**********************************
//			CMeshDome::Create(move, m_TextureFile[type].c_str());
//
//		}
//
//		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
//		{// 終了文字でループを抜ける
//
//			break;
//		}
//#endif

		// モデルの設定
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{
			int nType = 0, nShadow = 0;
			MyLib::Vector3 pos, rot;
			float scale = 1.0f;

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPEが来たら種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &nType);		// モデル種類の列挙
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POSが来たら位置読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &pos.x);	// X座標
					fscanf(pFile, "%f", &pos.y);	// Y座標
					fscanf(pFile, "%f", &pos.z);	// Z座標
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROTが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &rot.x);	// Xの向き
					fscanf(pFile, "%f", &rot.y);	// Yの向き
					fscanf(pFile, "%f", &rot.z);	// Zの向き
				}

				if (strcmp(&aComment[0], "SCALE") == 0)
				{// SCALEが来たら拡大率読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &scale);		// スケール
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOWが来たら影使用

					fscanf(pFile, "%s", &aComment[0]);		// =の分
					fscanf(pFile, "%d", &nShadow);			// 影を使うかどうか
				}

			}// END_MODELSETのかっこ

			// 追加
			bool bShadow = (nShadow == 1);
			Regist(nType, pos, rot, scale, bShadow);
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	// 読み込み完了
	m_bLoad = true;
}


//==========================================================================
// テクスチャ読み込み処理
//==========================================================================
HRESULT CEdit_Map::ReadTexture(const std::string& file)
{
	char aComment[MAX_COMMENT] = {};	// コメント用
	int nFileNum = 0;					// ファイルの数

	// ファイルを開く
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{// ファイルが開けた場合
		return E_FAIL;
	}

	int textureNum = 0;
	m_TextureFile.clear();

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// テクスチャ数の設定
		if (strcmp(&aComment[0], "NUM_TEXTURE") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &nFileNum);		// テクスチャ数
		}

		while (textureNum != nFileNum)
		{// テクスチャの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// テクスチャ名の設定
			if (strcmp(&aComment[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 追加
				m_TextureFile.push_back(&aComment[0]);

				// \\変換
				m_TextureFile.back() = UtilFunc::Transformation::ReplaceBackslash(m_TextureFile.back());
				m_TextureFile.back() = UtilFunc::Transformation::ReplaceForwardSlashes(m_TextureFile.back());

				textureNum++;	// テクスチャ数加算
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// モデル読み込み処理
//==========================================================================
HRESULT CEdit_Map::ReadXFile(const std::string& file)
{
	char aComment[MAX_COMMENT] = {};	// コメント用
	int nFileNum = 0;					// ファイルの数

	// ファイルを開く
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{// ファイルが開けた場合
		return E_FAIL;
	}

	int modelNum = 0;
	m_nModelIdx.clear();
	m_ModelFile.clear();

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル数の設定
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &nFileNum);		// モデル数
		}

		while (modelNum != nFileNum)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 追加
				m_ModelFile.push_back(&aComment[0]);
				m_nModelIdx.push_back(0);

				// \\変換
				m_ModelFile.back() = UtilFunc::Transformation::ReplaceBackslash(m_ModelFile.back());
				m_ModelFile.back() = UtilFunc::Transformation::ReplaceForwardSlashes(m_ModelFile.back());

				// インデックス取得
				m_nModelIdx.back() = CXLoad::GetInstance()->XLoad(m_ModelFile.back());

				modelNum++;	// モデル数加算
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// 削除
//==========================================================================
void CEdit_Map::Delete(CObjectX* obj)
{
	// 終了する
	std::vector<CObjectX*>::iterator itr = std::find(m_pObjX.begin(), m_pObjX.end(), obj);

	// 要素が見つかった場合のみ削除
	if (itr != m_pObjX.end())
	{
		// リストから削除
		m_List.Delete((*itr));

		(*itr)->Kill();
		m_pObjX.erase(itr);
	}
}

//==========================================================================
// 割り当て
//==========================================================================
void CEdit_Map::Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, float scale, bool bShadow)
{

	m_pObjX.emplace_back();

	if (m_nModelIdx.size() <= idx) {
		int n = 0;
	}

	// タイプの物を生成
	m_pObjX.back() = CObjectX::Create(m_nModelIdx[idx], pos, rot, bShadow, mylib_const::PRIORITY_DEFAULT, CObject::LAYER::LAYER_MAP);
	m_pObjX.back()->SetScale(scale);
	m_pObjX.back()->SetType(CObject::TYPE_XFILE);
	m_pObjX.back()->CreateCollisionBox();

	// リストに追加
	m_List.Regist(m_pObjX.back());

	if (m_bGrab) {

		// リストコピー
		CObjectX* pObject = nullptr;
		while (m_List.ListLoop(&pObject))
		{
			pObject->SetState(CObjectX::STATE::STATE_NONE);
		}

		// 掴みオブジェクト
		m_pGrabObj = m_pObjX.back();

		m_pObjX.back()->SetState(CObjectX::STATE::STATE_EDIT);

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}

		if (m_pHandle == nullptr) {
			m_pHandle = CHandle::Create(CHandle::HandleType::TYPE_MOVE, m_pGrabObj->GetPosition());
		}
	}
}


//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_Map_Release::CEdit_Map_Release() : CEdit_Map()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_Map_Release::~CEdit_Map_Release()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_Map_Release::Init()
{
	
	// 読み込み処理
	Load(m_Filename);

	return S_OK;
}

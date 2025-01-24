//=============================================================================
// 
//  ゲーム処理 [game.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "fade.h"
#include "camera.h"
#include "lightManager.h"
#include "pause.h"
#include "texture.h"

#include "input.h"
#include "map.h"
#include "sound.h"

#include "particle.h"
#include "myeffekseer.h"
#include "edit_map.h"
#include "objectLine.h"

#include "2D_Effect.h"
#include "controlkeydisp.h"
#include "ball.h"
#include "audience.h"
#include "teamStatus.h"

// TODO：デバッグ、後で消しますいらないので
#include "specialManager.h"
#include "gymWallManager.h"
#include "charmText.h"
#include "objectBillboard.h"
#include "cutin.h"
#include "balloonFrame.h"

namespace
{
	const float RATIO_SETGOAL = 0.825f;	// ゴール設置の割合
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CGame* CGame::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CGame::CGame()
{
	// 値のクリア
	m_pGameManager = nullptr;		// ゲームマネージャのオブジェクト
	m_EditType = EDITTYPE_OFF;		// エディットの種類
	m_bEdit = false;				// エディットの判定
	m_clear = false;				// クリア判定
	m_pEdit = nullptr;				// エディター
}

//==========================================================================
// デストラクタ
//==========================================================================
CGame::~CGame()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGame* CGame::Create(CScene::MODE mode)
{
	if (m_pThisPtr == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		switch (mode)
		{
		case CScene::MODE_GAME:
			m_pThisPtr = DEBUG_NEW CGame;
			break;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGame::Init()
{
	// エディット判定OFF
	m_bEdit = false;

	// プレイヤーの数設定
	CManager::GetInstance()->SetNumPlayer(1);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	//**********************************
	// ゲームマネージャ
	//**********************************
	m_pGameManager = CGameManager::Create(GetMode());

	// モード別初期化処理
	InitByMode();

	// ボールの生成
	CBall::Create(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// 体育館を明るくする
	GET_MANAGER->GetLight()->SetEnableBright(true);

	// クリアの判定
	SetEnableClear(true);

	return S_OK;
}

//==========================================================================
// モード別初期化
//==========================================================================
void CGame::InitByMode()
{

}

//==========================================================================
// 終了処理
//==========================================================================
void CGame::Uninit()
{
	m_pThisPtr = nullptr;

	// エディットの廃棄
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		m_pEdit = nullptr;
	}

	if (m_pGameManager != nullptr)
	{
		// 終了処理
		m_pGameManager->Uninit();
		m_pGameManager = nullptr;
	}


	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ゲームマネージャ
	if (m_pGameManager != nullptr)
	{
		// 更新処理
		m_pGameManager->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

#if _DEBUG
	// エディット切り替え処理
	ChangeEdit();
#endif


#if _DEBUG

	// 生成
	if (ImGui::TreeNode("Create"))
	{
#if 1
		if (ImGui::Button("Audience : AnimLeft"))
		{
			// オーディエンス生成
			CAudience::Create(CAudience::EObjType::OBJTYPE_HIGHPOLY, CGameManager::ETeamSide::SIDE_LEFT);
		}

		if (ImGui::Button("Audience : AnimRight"))
		{
			// オーディエンス生成
			CAudience::Create(CAudience::EObjType::OBJTYPE_ANIM, CGameManager::ETeamSide::SIDE_RIGHT);
		}

		if (ImGui::Button("CharmText_Left"))
		{
			// オーディエンス生成
			CCharmText::Create(CGameManager::ETeamSide::SIDE_LEFT);
		}
		if (ImGui::Button("Double CharmText_Left"))
		{
			// オーディエンス生成
			CCharmText::Create(CGameManager::ETeamSide::SIDE_LEFT);
			CCharmText::Create(CGameManager::ETeamSide::SIDE_LEFT);
		}
		if (ImGui::Button("Tripple CharmText_Left"))
		{
			// オーディエンス生成
			CCharmText::Create(CGameManager::ETeamSide::SIDE_LEFT);
			CCharmText::Create(CGameManager::ETeamSide::SIDE_LEFT);
			CCharmText::Create(CGameManager::ETeamSide::SIDE_LEFT);
		}

		if (ImGui::Button("CharmText_Rihgt"))
		{
			// オーディエンス生成
			CCharmText::Create(CGameManager::ETeamSide::SIDE_RIGHT);
		}

#endif

		// ツリー終端
		ImGui::TreePop();
	}

	// 操作
	if (ImGui::TreeNode("Control"))
	{
		if (ImGui::Button("Shader : Change"))
		{
			// シェーダーフラグの反転
			GET_RENDERER->SetEnableShader(!GET_RENDERER->IsShader(), 2.0f);
		}

		if (ImGui::Button("Camera : Swing"))
		{
			// カメラを適当に揺らす
			GET_MANAGER->GetCamera()->SetSwing(CCamera::SSwing(10.0f, 2.0f, 0.6f));
		}

		if (ImGui::Button("Room : Bright"))
		{
			// 部屋を明るくする
			GET_MANAGER->GetLight()->SetEnableBright(true);
		}

		if (ImGui::Button("Room : Dark"))
		{
			// 部屋を明るくする
			GET_MANAGER->GetLight()->SetEnableBright(false);
		}

		if (ImGui::Button("Audience : NormalLeft"))
		{
			// オーディエンス全通常
			CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_LEFT);
		}
		if (ImGui::Button("Audience : JumpLeft"))
		{
			// オーディエンス全盛り上がり
			CAudience::SetEnableJumpAll(true, CGameManager::ETeamSide::SIDE_LEFT);
		}
		if (ImGui::Button("Audience : SpecialLeft"))
		{
			// オーディエンス全スペシャル
			CAudience::SetSpecialAll(CGameManager::ETeamSide::SIDE_LEFT);
		}

		if (ImGui::Button("Audience : NormalRight"))
		{
			// オーディエンス全通常
			CAudience::SetEnableJumpAll(false, CGameManager::ETeamSide::SIDE_RIGHT);
		}
		if (ImGui::Button("Audience : JumpRight"))
		{
			// オーディエンス全盛り上がり
			CAudience::SetEnableJumpAll(true, CGameManager::ETeamSide::SIDE_RIGHT);
		}
		if (ImGui::Button("Audience : SpecialRight"))
		{
			// オーディエンス全スペシャル
			CAudience::SetSpecialAll(CGameManager::ETeamSide::SIDE_RIGHT);
		}

#if 1
		// 左チームのモテ値増減
		CTeamStatus* pTeamLeft = CGameManager::GetInstance()->GetTeamStatus(CGameManager::ETeamSide::SIDE_LEFT);	// チーム情報
		CTeamStatus::SCharmInfo infoLeft = pTeamLeft->GetCharmInfo();	// モテ情報
		ImGui::DragFloat("MoteValue : Left", &infoLeft.fValue, 0.1f, 0.0f, infoLeft.fValueMax, "%.2f");	// モテ値の変動操作
		pTeamLeft->SetCharmInfo(infoLeft);	// モテ値割当

		// 右チームのモテ値増減
		CTeamStatus* pTeamRight = CGameManager::GetInstance()->GetTeamStatus(CGameManager::ETeamSide::SIDE_RIGHT);	// チーム情報
		CTeamStatus::SCharmInfo infoRight = pTeamRight->GetCharmInfo();	// モテ情報
		ImGui::DragFloat("MoteValue : Right", &infoRight.fValue, 0.1f, 0.0f, infoRight.fValueMax, "%.2f");	// モテ値の変動操作
		pTeamRight->SetCharmInfo(infoRight);	// モテ値割当

#else
		if (ImGui::Button("Audience : DespawnLeft"))
		{
			// オーディエンス全退場
			CAudience::SetDespawnAll(CGameManager::ETeamSide::SIDE_LEFT);
		}
		if (ImGui::Button("Audience : DespawnRight"))
		{
			// オーディエンス全退場
			CAudience::SetDespawnAll(CGameManager::ETeamSide::SIDE_RIGHT);
		}
#endif

		// ツリー終端
		ImGui::TreePop();
	}
#endif

	// シーンの更新
	CScene::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// エディット切り替え処理
//==========================================================================
void CGame::ChangeEdit()
{
	static bool no_titlebar = false;
	static bool no_scrollbar = true;
	static bool no_menu = true;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = true;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

	// エディットメニュー
	ImGui::Begin("Edit", 0, window_flags);
	{
		if (ImGui::CollapsingHeader("Window options"))
		{
			if (ImGui::BeginTable("split", 3))
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("No titlebar", &no_titlebar);
				ImGui::TableNextColumn(); ImGui::Checkbox("No scrollbar", &no_scrollbar);
				ImGui::TableNextColumn(); ImGui::Checkbox("No menu", &no_menu);
				ImGui::TableNextColumn(); ImGui::Checkbox("No move", &no_move);
				ImGui::TableNextColumn(); ImGui::Checkbox("No resize", &no_resize);
				ImGui::TableNextColumn(); ImGui::Checkbox("No collapse", &no_collapse);
				ImGui::TableNextColumn(); ImGui::Checkbox("No close", &no_close);
				ImGui::TableNextColumn(); ImGui::Checkbox("No nav", &no_nav);
				ImGui::TableNextColumn(); ImGui::Checkbox("No background", &no_background);
				ImGui::TableNextColumn(); ImGui::Checkbox("No bring to front", &no_bring_to_front);
				ImGui::TableNextColumn(); ImGui::Checkbox("Unsaved document", &unsaved_document);
				ImGui::EndTable();
			}
		}

		// テキスト
		static const char* items[] = { "OFF", "Map", "Obstacle", "Course", "WaterStone", "JudgeZone"};
		int selectedItem = m_EditType;

		// [グループ]エディット切り替え
		if (ImGui::CollapsingHeader("Change Edit Mode"))
		{
			// [ラジオボタン]モーション切り替え
			for (int i = 0; i < EditType::EDITTYPE_MAX; i++)
			{
				if (ImGui::RadioButton(items[i], &selectedItem, i))
				{
					if (i != 0)
					{
						// デバッグモード
						m_pGameManager->SetSceneType(CGameManager::ESceneType::SCENE_DEBUG);
					}
					else
					{
						// デバッグモード
						m_pGameManager->SetSceneType(m_pGameManager->GetOldType());
					}

					// エディット終了
					EditReset();
					m_EditType = static_cast<EditType>(selectedItem);

					// 生成
					if (m_pEdit != nullptr)
					{
						m_pEdit->Uninit();
						m_pEdit = nullptr;
					}

					m_pEdit = CEdit::Create(m_EditType);
				}
			}
		}

		if (m_pEdit != nullptr) {
			m_pEdit->Update();
		}

	}
	ImGui::End();
}

//==========================================================================
// 描画処理
//==========================================================================
void CGame::Draw()
{

}

//==========================================================================
// エディターリセット処理
//==========================================================================
void CGame::EditReset()
{
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		m_pEdit = nullptr;
	}
}

//=============================================================================
// 
// ドレスアップ設定処理 [entry_dressup.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "entry_dressup.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "entry_setupTeam.h"
#include "dressup.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTFILE = "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE = "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry_Dressup::CEntry_Dressup() : CEntryScene()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntry_Dressup::~CEntry_Dressup()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEntry_Dressup::Init()
{
	// 読み込み
	Load();

	// チーム等設定
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetEntryScene()->GetSetupTeam();
	if (pSetupTeam == nullptr) return E_FAIL;

	int nPlyerNum = pSetupTeam->GetAllPlayerNum();

	// 着せ替え情報リサイズ
	m_vecDressupInfo.resize(nPlyerNum);

	// 生成処理
	for (int i = 0; i < nPlyerNum; i++)
	{
		// プレイヤー生成
		MyLib::Vector3 pos = MyLib::Vector3(200.0f, 0.0f, 0.0f);
		MyLib::Vector3 offset = MyLib::Vector3(200.0f * i, 0.0f, 0.0f);
		m_vecDressupInfo[i].pPlayer = CPlayer::Create
		(
			pos + offset, 						// 位置
			CGameManager::ETeamSide::SIDE_NONE,	// チームサイド
			CPlayer::EFieldArea::FIELD_ENTRY,	// ポジション
			CPlayer::EBaseType::TYPE_USER,		// ベースタイプ
			CPlayer::EBody::BODY_NORMAL,		// 体系
			CPlayer::EHandedness::HAND_R		// 利き手
		);

		// エントリーした番号
		int entryIdx = pSetupTeam->GetEntryIdx(i);

		// 髪着せ替え生成
		m_vecDressupInfo[i].pHair = CDressup::Create(
			CDressup::EType::TYPE_HAIR,		// 着せ替えの種類
			m_vecDressupInfo[i].pPlayer,	// 変更するプレイヤー
			CPlayer::ID_HAIR);				// 変更箇所のインデックス
		
		// アクセ切り替え生成
		m_vecDressupInfo[i].pAccessory = CDressup::Create(
			CDressup::EType::TYPE_HAIR,		// 着せ替えの種類
			m_vecDressupInfo[i].pPlayer,	// 変更するプレイヤー
			CPlayer::ID_ACCESSORY);			// 変更箇所のインデックス

		// 操作するインデックス設定
		m_vecDressupInfo[i].pHair->SetControllIdx(entryIdx);
		m_vecDressupInfo[i].pAccessory->SetControllIdx(entryIdx);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_Dressup::Uninit()
{
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_Dressup::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 今回のサイズ
	int size = static_cast<int>(m_vecDressupInfo.size());

	for (int i = 0; i < size; i++)
	{
		// 着せ替えの更新
		m_vecDressupInfo[i].pHair->Update(fDeltaTime, fDeltaRate, fSlowRate);
		m_vecDressupInfo[i].pAccessory->Update(fDeltaTime, fDeltaRate, fSlowRate);

		// 体型変更
		ChangeBodyType(i);

		// 利き手変更
		ChangeHandedness(i);
	}

}

//==========================================================================
// 体型変更
//==========================================================================
void CEntry_Dressup::ChangeBodyType(int i)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 体型取得
	CPlayer::EBody body = m_vecDressupInfo[i].pPlayer->GetBodyType();
	CPlayer::EBody oldBody = body;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, i))
	{// ループ

		// 次へ変更
		int afterBody = (body + 1) % (CPlayer::EBody::BODY_MAX - 1);
		body = static_cast<CPlayer::EBody>(afterBody);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, i))
	{// 逆ループ

		// 前へ変更
		int deforeBody = (body + (CPlayer::EBody::BODY_MAX - 1)) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(deforeBody);
	}

	if (body != oldBody)
	{// 変更されていたら

		// プレイヤー再生成
		ReCreatePlayer(i, m_vecDressupInfo[i].pPlayer->GetHandedness(), body);
	}

}

//==========================================================================
// 利き手変更
//==========================================================================
void CEntry_Dressup::ChangeHandedness(int i)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 利き手取得
	CPlayer::EHandedness handedness = m_vecDressupInfo[i].pPlayer->GetHandedness();
	CPlayer::EHandedness oldHandedness = handedness;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, i))
	{// ループ

		// 次へ変更
		int afterHandedness = (handedness + 1) % (CPlayer::EHandedness::HAND_MAX - 1);
		handedness = static_cast<CPlayer::EHandedness>(afterHandedness);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, i))
	{// 逆ループ

		// 前へ変更
		int deforeHandedness = (handedness + (CPlayer::EHandedness::HAND_MAX - 1)) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(deforeHandedness);
	}

	if (handedness != oldHandedness)
	{// 変更されていたら

		// プレイヤー再生成
		ReCreatePlayer(i, handedness, m_vecDressupInfo[i].pPlayer->GetBodyType());
	}
}

//==========================================================================
// プレイヤー再生成
//==========================================================================
void CEntry_Dressup::ReCreatePlayer(int i, CPlayer::EHandedness handedness, CPlayer::EBody body)
{
	// 情報保存
	MyLib::Vector3 pos = m_vecDressupInfo[i].pPlayer->GetPosition();

	// 削除
	m_vecDressupInfo[i].pPlayer->Kill();

	// 再生成
	m_vecDressupInfo[i].pPlayer = CPlayer::Create
	(
		pos, 								// 位置
		CGameManager::ETeamSide::SIDE_NONE,	// チームサイド
		CPlayer::EFieldArea::FIELD_ENTRY,	// ポジション
		CPlayer::EBaseType::TYPE_USER,		// ベースタイプ
		body,								// 体系
		handedness							// 利き手
	);
}

//==========================================================================
// セーブ処理
//==========================================================================
void CEntry_Dressup::Save()
{
	// ファイルを開く
	std::ofstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// テキストファイル名目次
	File << TOP_LINE			<< std::endl;
	File << "# チーム等設定"	<< std::endl;
	File << TOP_LINE			<< std::endl;


	// ファイルを閉じる
	File.close();
}

//==========================================================================
// ロード
//==========================================================================
void CEntry_Dressup::Load()
{
	// ファイルを開く
	std::ifstream File(TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("PLAYERNUM") != std::string::npos)
		{// PLAYERNUMでチーム毎のプレイヤー数読み込み

			// ストリーム作成
			std::istringstream lineStream(line);
			
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CEntry_Dressup::Debug()
{
	if (ImGui::TreeNode("SetUpTeam"))
	{
		//=============================
		// セーブ
		//=============================s
		if (ImGui::Button("Save"))
		{
			Save();
		}

		

		ImGui::TreePop();
	}
}

//=============================================================================
// 
//  チーム等設定処理 [entry_setupTeam.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//=============================================================================
#include "entry_setupTeam.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objectChara.h"
#include "object2D_Anim.h"
#include "number.h"
#include "transUI.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\setupTeam.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// テキストのライン
	const int PRIORITY = 5;	// 優先順位

	namespace pad
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker001.png";	// コントローラーUIテクスチャ
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// テクスチャ分割数
		const float WIDTH = 25.0f;	// 横幅

		namespace none
		{
			const MyLib::Vector3 POS = VEC3_SCREEN_CENT;	// 中心位置
			const float OFFSET_Y = 60.0f;	// Y座標オフセット
		}

		namespace left
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 120.0f, 0.0f);	// 左上位置
			}

			const MyLib::Vector3 POS = MyLib::Vector3(up::POS.x, VEC3_SCREEN_CENT.y, 0.0f);	// 左中心位置
			const float OFFSET_X = 60.0f;	// X座標オフセット
		}

		namespace right
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 120.0f, 0.0f);	// 右上位置
			}

			const MyLib::Vector3 POS = MyLib::Vector3(up::POS.x, VEC3_SCREEN_CENT.y, 0.0f);	// 右中心位置
			const float OFFSET_X = 60.0f;	// X座標オフセット
		}
	}

	namespace num
	{
		const std::string TEXTURE = "data\\TEXTURE\\number\\number000.png";	// 数字テクスチャ
		const float WIDTH = 200.0f;	// 横幅
	}
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry_SetUpTeam::CEntry_SetUpTeam() : CEntryScene(),
	m_pTransUI	(nullptr)	// 遷移UI情報
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		m_vecAddIdx[i].clear();		// 追加されたインデックス
		m_apNumInTeam[i] = nullptr;	// チーム人数情報
		m_nMaxChangeIdx[i] = -1;	// 最大数変更するインデックス
		m_nPlayerNum[i] = 1;		// プレイヤーの数
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // チーム合計のプレイヤー総数分繰り返す

		m_TeamSide[i].team = CGameManager::ETeamSide::SIDE_NONE;	// チームサイド
		m_TeamSide[i].nPadIdx = -1;	// 操作権インデックス
		m_apPadUI[i] = nullptr;		// コントローラーUI情報
	}

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // パッド認識の最大数分繰り返す

		m_TeamSide[i].nPadIdx = i;	// 操作権インデックス
		m_nEntryIdx[i] = -1;		// エントリーのインデックス
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CEntry_SetUpTeam::~CEntry_SetUpTeam()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEntry_SetUpTeam::Init()
{
	// 前回のセットアップ読込 // TODO：初期情報の書き出しをCManager破棄時に呼び出し（このままだと前回の誰かの設定がそのままになる）
	Load();

	// チーム人数の生成
	if (FAILED(CreateNumInTeam()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// コントローラーUIの生成
	if (FAILED(CreatePadUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 遷移UIの生成
	if (FAILED(CreateTransUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 追加されたインデックスリセット // TODO：毎回リセットしちゃうから要検討
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		m_vecAddIdx[i].clear();
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEntry_SetUpTeam::Uninit()
{
	// 保存された自身のアドレスを初期化
	CEntry::GetInstance()->ResetSetupTeam();

	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEntry_SetUpTeam::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CEntryScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// チーム選択の更新
	bool bAllReady = SelectTeam();

	// プレイヤー最大数変更の更新
	ChangeMaxPlayer();

	// UIの位置補正
	PosAdjUI(bAllReady, fDeltaTime, fDeltaRate, fSlowRate);

	// 着せ替え遷移
	TransDressUp(bAllReady);
}

//==========================================================================
// チーム人数の生成処理
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateNumInTeam()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム人数の生成
		m_apNumInTeam[i] = CNumber::Create(CMultiNumber::EObjType::OBJTYPE_2D, PRIORITY);
		if (m_apNumInTeam[i] == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 位置の設定
		m_apNumInTeam[i]->SetPosition(MyLib::Vector3(320.0f + (640.0f * (float)i), 120.0f, 0.0f));	// TODO：初期生成位置の調整

		// テクスチャの割当
		CTexture* pTexture = CTexture::GetInstance();
		int nTexID = pTexture->Regist(num::TEXTURE);
		m_apNumInTeam[i]->BindTexture(nTexID);

		// 数字の設定
		m_apNumInTeam[i]->SetNum(m_nPlayerNum[i]);

		// 横幅を元にサイズを設定
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, num::WIDTH);
		size.x /= 10.0f;
		m_apNumInTeam[i]->SetSize(size);
		m_apNumInTeam[i]->SetSizeOrigin(m_apNumInTeam[i]->GetSize());
	}

	return S_OK;
}

//==========================================================================
// コントローラーUIの生成処理
//==========================================================================
HRESULT CEntry_SetUpTeam::CreatePadUI()
{
	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // チーム合計のプレイヤー総数分繰り返す

		// コントローラーUIの生成
		m_apPadUI[i] = CObject2D_Anim::Create
		( // 引数
			MyLib::Vector3(VEC3_SCREEN_CENT.x, 100.0f + (300.0f * (float)i), 0.0f),	// 位置	// TODO：初期生成位置の調整
			pad::PTRN.x,	// テクスチャ横分割数
			pad::PTRN.y,	// テクスチャ縦分割数
			0.0f,			// 再生時間
			false,			// 自動破棄
			PRIORITY		// 優先順位
		);
		if (m_apPadUI[i] == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 自動再生をOFFにする
		m_apPadUI[i]->SetEnableAutoPlay(false);
		
		// 自動描画をOFFにする
		m_apPadUI[i]->SetEnableDisp(false);
		
		// テクスチャの割当
		CTexture* pTexture = CTexture::GetInstance();
		int nTexID = pTexture->Regist(pad::TEXTURE);
		m_apPadUI[i]->BindTexture(nTexID);
		
		// テクスチャパターンの設定
		m_apPadUI[i]->SetPatternAnim(i);
		
		// 横幅を元にサイズを設定
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, pad::WIDTH);
		size.y *= (float)pad::PTRN.x;
		m_apPadUI[i]->SetSize(size);
		m_apPadUI[i]->SetSizeOrigin(m_apPadUI[i]->GetSize());
	}

	return S_OK;
}

//==========================================================================
// 遷移UIの生成処理
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateTransUI()
{
	// 遷移UIの生成
	m_pTransUI = CTransUI::Create();
	if (m_pTransUI == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 空きメンバーのAI埋め
//==========================================================================
void CEntry_SetUpTeam::FillAI()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		for (int j = 0; j < CGameManager::MAX_PLAYER; j++)
		{ // チーム合計のプレイヤー総数分繰り返す

			if (m_TeamSide[j].team == CGameManager::ETeamSide::SIDE_NONE)
			{ // チームが未設定のプレイヤーがいた場合

				const int nNumInTeam = (int)m_vecAddIdx[i].size();	// 内野人数
				if (nNumInTeam <= j && m_nPlayerNum[i] > nNumInTeam)
				{ // サイド毎のプレイヤー数より大きい且つ、必要チーム人数に到達していない場合

					// 現在のチームを保存
					m_TeamSide[j].team = (CGameManager::ETeamSide)i;

					// 操作権インデックスを初期化
					m_TeamSide[j].nPadIdx = -1;

					// AIインデックスのメンバーを追加
					m_vecAddIdx[i].push_back(-1);
				}
				else
				{ // 既にチーム参加プレイヤーが整っている場合

					// 操作権インデックスを初期化
					m_TeamSide[j].nPadIdx = -1;
				}
			}
		}
	}
}

//==========================================================================
// UIの削除
//==========================================================================
void CEntry_SetUpTeam::KillUI()
{
	// 遷移UIの削除
	SAFE_KILL(m_pTransUI);

	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム人数の削除
		SAFE_KILL(m_apNumInTeam[i]);
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // チーム合計のプレイヤー総数分繰り返す

		// コントローラーUIの削除
		SAFE_KILL(m_apPadUI[i]);
	}
}

//==========================================================================
// UIの位置補正
//==========================================================================
void CEntry_SetUpTeam::PosAdjUI(const bool bAllReady, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム人数の総数を反映
		m_apNumInTeam[i]->SetNum(m_nPlayerNum[i]);
	}

	// コントローラーUI位置補正
	PosAdjPadUI();

	// 遷移表示のON/OFF切り替え
	m_pTransUI->SetEnableDispUI(bAllReady);
}

//==========================================================================
// コントローラーUI位置補正
//==========================================================================
void CEntry_SetUpTeam::PosAdjPadUI()
{
	std::vector<CObject2D_Anim*> vecNone;	// チーム指定なし配列
	std::vector<CObject2D_Anim*> vecLeft;	// 左チーム配列
	std::vector<CObject2D_Anim*> vecRight;	// 右チーム配列
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // パッド認識の最大数分繰り返す

		// プレイヤーインデックスが指定されていない場合次へ
		if (m_nEntryIdx[i] <= -1) { continue; }

		// チーム指定ごとの配列に追加
		const int nCurIdx = m_nEntryIdx[i];	// エントリーインデックス
		switch (m_TeamSide[nCurIdx].team)
		{ // チームごとの処理
		case CGameManager::ETeamSide::SIDE_NONE:
			vecNone.push_back(m_apPadUI[nCurIdx]);
			break;

		case CGameManager::ETeamSide::SIDE_LEFT:
			if (m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_LEFT] != nCurIdx)
			{ vecLeft.push_back(m_apPadUI[nCurIdx]); }
			break;

		case CGameManager::ETeamSide::SIDE_RIGHT:
			if (m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_RIGHT] != nCurIdx)
			{ vecRight.push_back(m_apPadUI[nCurIdx]); }
			break;

		default:
			assert(false);
			break;
		}
	}

	// 配列内要素をソートする
	std::sort(vecNone.begin(),  vecNone.end(),  [](CObject2D_Anim* p1, CObject2D_Anim* p2) { return p1->GetPatternAnim() < p2->GetPatternAnim(); });
	std::sort(vecLeft.begin(),  vecLeft.end(),  [](CObject2D_Anim* p1, CObject2D_Anim* p2) { return p1->GetPatternAnim() < p2->GetPatternAnim(); });
	std::sort(vecRight.begin(), vecRight.end(), [](CObject2D_Anim* p1, CObject2D_Anim* p2) { return p1->GetPatternAnim() < p2->GetPatternAnim(); });

	// チーム指定なしの位置補正
	const int nNumNone = (int)vecNone.size();	// 要素数
	int nLoopNone = 0;	// ループ数
	for (auto& rNone : vecNone)
	{ // 指定なし選択中の要素数分繰り返す

		MyLib::Vector3 posNone = rNone->GetPosition();	// 位置

		// オフセットを与えた位置を計算
		posNone.x = pad::none::POS.x;
		posNone.y = pad::none::POS.y - (pad::none::OFFSET_Y * (float)(nNumNone - 1)) * 0.5f + (pad::none::OFFSET_Y * (float)nLoopNone);

		// 位置を反映
		rNone->SetPosition(posNone);

		// ループ回数を加算
		nLoopNone++;
	}

	// 左チームの位置補正
	const int nNumLeft = (int)vecLeft.size();	// 要素数
	int nLoopLeft = 0;	// ループ数
	for (auto& rLeft : vecLeft)
	{ // 指定なし選択中の要素数分繰り返す

		MyLib::Vector3 posLeft = rLeft->GetPosition();	// 位置

		// オフセットを与えた位置を計算
		posLeft.x = pad::left::POS.x - (pad::left::OFFSET_X * (float)(nNumLeft - 1)) * 0.5f + (pad::left::OFFSET_X * (float)nLoopLeft);
		posLeft.y = pad::left::POS.y;

		// 位置を反映
		rLeft->SetPosition(posLeft);

		// ループ回数を加算
		nLoopLeft++;
	}

	// 右チームの位置補正
	const int nNumRight = (int)vecRight.size();	// 要素数
	int nLoopRight = 0;	// ループ数
	for (auto& rRight : vecRight)
	{ // 指定なし選択中の要素数分繰り返す

		MyLib::Vector3 posRight = rRight->GetPosition();	// 位置

		// オフセットを与えた位置を計算
		posRight.x = pad::right::POS.x - (pad::right::OFFSET_X * (float)(nNumRight - 1)) * 0.5f + (pad::right::OFFSET_X * (float)nLoopRight);
		posRight.y = pad::right::POS.y;

		// 位置を反映
		rRight->SetPosition(posRight);

		// ループ回数を加算
		nLoopRight++;
	}

	// 左チーム人数調整の位置補正
	const int nLeftUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_LEFT];		// 左チーム人数調整パッドインデックス
	if (nLeftUpIdx > -1)
	{ // 操作するプレイヤーがいた場合

		// 位置を反映
		m_apPadUI[nLeftUpIdx]->SetPosition(pad::left::up::POS);
	}

	// 右チーム人数調整の位置補正
	const int nRightUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_RIGHT];	// 右チーム人数調整パッドインデックス
	if (nRightUpIdx > -1)
	{ // 操作するプレイヤーがいた場合

		// 位置を反映
		m_apPadUI[nRightUpIdx]->SetPosition(pad::right::up::POS);
	}
}

//==========================================================================
// チーム選択
//==========================================================================
bool CEntry_SetUpTeam::SelectTeam()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	int nNumEntry = 0;	// エントリー人数
	int nNumReady = 0;	// 準備完了人数

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // パッド認識の最大数分繰り返す

		//--------------------------
		// エントリー操作
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i))
		{ // エントリー操作が行われた場合

			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{ // パッド認識の最大数分繰り返す

				// 既にエントリーされている場合次へ
				if (m_nEntryIdx[j] >= 0 && m_nEntryIdx[j] != i) { continue; }

				// 今回のプレイヤーインデックスを追加
				m_nEntryIdx[j] = i;

				// コントローラーUIの自動描画をONにする
				m_apPadUI[i]->SetEnableDisp(true);
				break;
			}
		}

		// 未エントリーの場合次へ
		const int nUserIdx = m_nEntryIdx[i];	// ユーザーインデックス
		if (nUserIdx <= -1) { continue; }

		// エントリー人数を加算
		nNumEntry++;

		//--------------------------
		// 離脱操作
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, nUserIdx))
		{ // 離脱操作が行われた場合

			std::vector<int>::iterator itr;	// 準備完了配列イテレーター
			if (IsUserReady(nUserIdx, &itr))
			{ // 準備完了している場合

				// 準備完了配列からイテレーターを削除
				m_vecAddIdx[m_TeamSide[nUserIdx].team].erase(itr);
				break;
			}
			else
			{ // 準備完了していない場合

				// エントリーを解除
				DeleteEntry(&m_nEntryIdx[i]);
				break;
			}
		}

		// 既に準備完了済みの場合次へ
		if (IsUserReady(nUserIdx)) { nNumReady++; continue; }	// 準備完了人数を加算

		//--------------------------
		// チーム替え
		//--------------------------
		if (!IsUserMaxChange(nUserIdx))
		{ // 最大数変更の操作権を持っていない場合

			if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nUserIdx)
			&&  m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_LEFT)
			{ // 左端じゃない時に左移動操作が行われた場合

				// 左に移動
				m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_LEFT : CGameManager::ETeamSide::SIDE_NONE;
			}
			else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx)
				 &&  m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_RIGHT)
			{ // 右端じゃない時に右移動操作が行われた場合

				// 右に移動
				m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_RIGHT : CGameManager::ETeamSide::SIDE_NONE;
			}
		}

		// 選択チームを保存
		const int nSide = m_TeamSide[nUserIdx].team;

		//--------------------------
		// チーム内人数変え
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nUserIdx)
		&&  IsUserTeamSelect(nUserIdx)		// チーム選択中
		&&  m_nMaxChangeIdx[nSide] <= -1)	// 最大数変更の操作権を誰も持っていない
		{ // 上移動操作が行われた場合

			// 自分のサイドを変更する用インデックス保持
			m_nMaxChangeIdx[nSide] = nUserIdx;
		}

		//--------------------------
		// 準備完了操作
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, nUserIdx)
		&&  IsUserTeamSelect(nUserIdx)	// チーム選択中
		&&  !IsUserMaxChange(nUserIdx))	// 最大数変更を自分がしていない
		{ // 準備完了操作が行われた場合

			const int nCurInTeam = static_cast<int>(m_vecAddIdx[nSide].size());	// 現在のチーム人数
			if (!IsUserReady(nUserIdx) && m_nPlayerNum[nSide] > nCurInTeam)
			{ // 準備未完了且つ、最大チーム人数未満の場合

				// 準備完了配列に追加
				m_vecAddIdx[nSide].push_back(nUserIdx);
			}
		}
	}

	// チームセットアップが完了しているかを返す
	return (nNumEntry == nNumReady && nNumEntry > 0);	// 全員が準備完了済み且つエントリー人数が一人でもいる場合true
}

//==========================================================================
// プレイヤー最大数変更
//==========================================================================
void CEntry_SetUpTeam::ChangeMaxPlayer()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	for (int nSide = 0; nSide < CGameManager::ETeamSide::SIDE_MAX; nSide++)
	{ // チーム数分繰り返す

		// 操作権を持っていない場合次へ
		const int nUserIdx = m_nMaxChangeIdx[nSide];	// ユーザーインデックス
		if (nUserIdx <= -1) { continue; }

		//--------------------------
		// 最大数変更操作
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nUserIdx))
		{ // 減算操作が行われた場合

			// チーム人数を減算
			m_nPlayerNum[nSide]--;
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx))
		{ // 加算操作が行われた場合

			// チーム人数を加算
			m_nPlayerNum[nSide]++;
		}

		// チーム人数を補正
		m_nPlayerNum[nSide] = UtilFunc::Transformation::Clamp(m_nPlayerNum[nSide], 1, CGameManager::MAX_SIDEPLAYER);

		//--------------------------
		// 最大数変更解除
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nUserIdx))
		{ // 下移動操作が行われた場合

			// 変更操作権を初期化
			m_nMaxChangeIdx[nSide] = -1;
		}
	}
}

//==========================================================================
// 着せ替え遷移
//==========================================================================
void CEntry_SetUpTeam::TransDressUp(const bool bAllReady)
{
	if (m_pTransUI->UpdateDecide())
	{ // 全員の準備が完了している且つ、操作が行われた場合

		// 空きメンバーのAI埋め
		FillAI();

		// UIの削除
		KillUI();

		// 着せ替えシーンへ遷移
		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_DRESSUP);
	}
}

//==========================================================================
// エントリー解除
//==========================================================================
void CEntry_SetUpTeam::DeleteEntry(int* pEntryIdx)
{
	if (m_nMaxChangeIdx[m_TeamSide[*pEntryIdx].team] == *pEntryIdx)
	{ // 最大数変更の操作権が自身の場合

		// 最大数変更の操作権を初期化
		m_nMaxChangeIdx[m_TeamSide[*pEntryIdx].team] = -1;
	}

	// コントローラーUIの自動描画をOFFにする
	m_apPadUI[*pEntryIdx]->SetEnableDisp(false);

	// チームを初期化
	m_TeamSide[*pEntryIdx].team = CGameManager::ETeamSide::SIDE_NONE;

	// エントリーインデックスを初期化
	*pEntryIdx = -1;
}

//==========================================================================
// 準備完了フラグ取得
//==========================================================================
bool CEntry_SetUpTeam::IsUserReady(const int nUserIdx, std::vector<int>::iterator* pItr)
{
	// チームが未選択の場合抜ける
	const int nSide = m_TeamSide[nUserIdx].team;	// 選択チーム
	if (nSide == CGameManager::ETeamSide::SIDE_NONE) { return false; }

	// ユーザーインデックスを検索 (準備完了している場合は配列内にインデックスが格納されている)
	std::vector<int>::iterator itr = std::find(m_vecAddIdx[nSide].begin(), m_vecAddIdx[nSide].end(), nUserIdx);

	// 保存用ポインタが指定されている場合イテレーターを保存
	if (pItr != nullptr) { *pItr = itr; }

	// 準備完了状況を返す
	return (itr != m_vecAddIdx[nSide].end());
}

//==========================================================================
// チーム選択中フラグ取得
//==========================================================================
bool CEntry_SetUpTeam::IsUserTeamSelect(const int nUserIdx)
{
	// 選択中チームの指定があるかを返す
	return (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_NONE);
}

//==========================================================================
// 最大数変更フラグ取得
//==========================================================================
bool CEntry_SetUpTeam::IsUserMaxChange(const int nUserIdx)
{
	// チームが未選択の場合抜ける
	const int nSide = m_TeamSide[nUserIdx].team;	// 選択チーム
	if (nSide == CGameManager::ETeamSide::SIDE_NONE) { return false; }

	// 最大数変更の操作権インデックスが引数ユーザーかを返す
	return (m_nMaxChangeIdx[nSide] == nUserIdx);
}

//==========================================================================
// セーブ処理
//==========================================================================
void CEntry_SetUpTeam::Save()
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


	// サイド毎の人数
	File << TEXT_LINE			<< std::endl;
	File << "# プレイヤー数"	<< std::endl;
	File << TEXT_LINE			<< std::endl;
	File << "PLAYERNUM = "	<< 
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] << 
		" " <<
		m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT] << std::endl;
	
	File << "" << std::endl;


	// プレイヤーインデックス
	File << TEXT_LINE << std::endl;
	File << "# プレイヤーインデックス" << std::endl;
	File << TEXT_LINE << std::endl;
	
	// 左
	File << "LEFT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size()) - 1 >= i)
		{// サイズ内
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";

	}
	File << std::endl;

	// 右
	File << "RIGHT_ID = ";
	for (int i = 0; i < CGameManager::MAX_SIDEPLAYER; i++)
	{
		if (static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size()) - 1 >= i)
		{// サイズ内
			File << m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][i];
		}
		else
		{
			File << -1;
		}
		File << " ";
	}
	File << std::endl;


	// ファイルを閉じる
	File.close();
}

//==========================================================================
// ロード
//==========================================================================
void CEntry_SetUpTeam::Load()
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

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] >>
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT];	// プレイヤー数
		}

		if (line.find("LEFT_ID") != std::string::npos)
		{// LEFT_IDで左側チームのインデックス

			// ストリーム作成
			std::istringstream lineStream(line);

			// 破棄
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].clear();

			// リサイズ
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].resize(CGameManager::MAX_SIDEPLAYER);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT][2];	// プレイヤー数
		}

		if (line.find("RIGHT_ID") != std::string::npos)
		{// RIGHT_IDで右側チームのインデックス

			// ストリーム作成
			std::istringstream lineStream(line);

			// 破棄
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].clear();

			// リサイズ
			m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].resize(CGameManager::MAX_SIDEPLAYER);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][0] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][1] >>
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT][2];	// プレイヤー数
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
void CEntry_SetUpTeam::Debug()
{

	//--------------------------
	// エントリー操作
	//--------------------------
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN))
	{
		// 今回のプレイヤーインデックスを追加
		m_nEntryIdx[0] = 0;

		// コントローラーUIの自動描画をONにする
		m_apPadUI[0]->SetEnableDisp(true);

		// 左に移動
		m_TeamSide[0].team = CGameManager::ETeamSide::SIDE_LEFT;

		// 準備完了配列に追加
		m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].push_back(0);
	}

	//--------------------------
	// シーン遷移
	//--------------------------
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_X))
	{
		// 着せ替えシーンへ遷移
		CEntry::GetInstance()->ChangeEntryScene(CEntry::ESceneType::SCENETYPE_DRESSUP);
	}

	if (ImGui::TreeNode("SetUpTeam"))
	{
		//=============================
		// セーブ
		//=============================s
		if (ImGui::Button("Save"))
		{
			Save();
		}

		//=============================
		// 総数変更
		//=============================
		//--------------------------
		// 左
		//--------------------------
		{
			ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Entry Left:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() > 1)
			{// 削除
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].pop_back();
				m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT]--;		// プレイヤーの数
			}
			ImGui::SameLine(0.0f);

			if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size() < CGameManager::MAX_SIDEPLAYER)
			{// 追加
				int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size();
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].push_back(size);
			}
			ImGui::SameLine();
			ImGui::PopID();

			// サイズ
			int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT].size());
			ImGui::Text("%d", segmentSize);
		}

		//--------------------------
		// 右
		//--------------------------
		{
			ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Entry Right:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() > 1)
			{// 削除
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].pop_back();
			}
			ImGui::SameLine(0.0f);

			if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size() < CGameManager::MAX_SIDEPLAYER)
			{// 追加
				int size = m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size();
				m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].push_back(size);
			}
			ImGui::SameLine();
			ImGui::PopID();

			// サイズ
			int segmentSize = static_cast<int>(m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT].size());
			ImGui::Text("%d", segmentSize);
		}


		//=============================
		// 総数変更
		//=============================
		ImGui::Text("Side Player");
		ImGui::SliderInt("SIDE_LEFT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT], 1, CGameManager::MAX_SIDEPLAYER);
		ImGui::SliderInt("SIDE_RIGHT", &m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT], 1, CGameManager::MAX_SIDEPLAYER);

		// エントリーのインデックス
		for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
		{
			ImGui::Text("EntryIdx[%d] : %d", i, m_nEntryIdx[i]);
		}

		ImGui::TreePop();
	}
}

//==========================================================================
// パッドインデックスのエントリーインデックス変換
//==========================================================================
int CEntry_SetUpTeam::PadIdxToEntryIdx(int nPadIdx)
{
	if (nPadIdx >= mylib_const::MAX_PLAYER) return -1;
	return m_nEntryIdx[nPadIdx];
}

//==========================================================================
// プレイヤーインデックスのパッドインデックス変換
//==========================================================================
int CEntry_SetUpTeam::PlayerIdxToPadIdx(int nPlayerIdx)
{
	if (nPlayerIdx >= CGameManager::MAX_PLAYER) return -1;
	return m_TeamSide[nPlayerIdx].nPadIdx;
}

//==========================================================================
// チームサイド取得
//==========================================================================
CGameManager::ETeamSide CEntry_SetUpTeam::GetTeamSide(int nPlayerIdx)
{
	if (nPlayerIdx >= CGameManager::MAX_PLAYER) return CGameManager::ETeamSide::SIDE_NONE;
	return m_TeamSide[nPlayerIdx].team;
}

//==========================================================================
// チームサイド取得 (AI)
//==========================================================================
CGameManager::ETeamSide CEntry_SetUpTeam::GetTeamSideAI(int nOccurIdx)
{
	int nNumAI = 0;	// AI発見数
	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // チーム合計のプレイヤー総数分繰り返す

		// AIではない場合次へ
		if (m_TeamSide[i].nPadIdx > -1) { continue; }

		// 引数分AIを発見できていない場合次へ
		if (nNumAI < nOccurIdx) { nNumAI++; continue; }	// AI発見数を加算

		// 引数番目に発見したAIのチームサイドを返す
		return m_TeamSide[i].team;
	}

	// 引数分AIがいないよ
	assert(false);
	return CGameManager::ETeamSide::SIDE_NONE;
}

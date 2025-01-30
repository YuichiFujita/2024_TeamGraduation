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
#include "padUI.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTFILE	= "data\\TEXT\\entry\\team_setting.txt";
	const std::string TOP_LINE	= "#==============================================================================";	// テキストのライン
	const std::string TEXT_LINE	= "#------------------------------------------------------------------------------";	// テキストのライン
	const int PRIORITY = 5;	// 優先順位

	namespace pad
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\playerMarker001.png";	// コントローラーUIテクスチャ
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// テクスチャ分割数
		const float WIDTH = 45.0f;	// 横幅
		const MyLib::Vector3 ARROWSPACE_NUMBER = MyLib::Vector3(60.0f, 0.0f, 0.0f);	// 数字部分の空白
		const float ARROWWIDTH_NUMBER = 20.0f;	// 数字部分の矢印サイズ

		namespace none
		{
			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x, 550.0f, 0.0f);	// 中心位置
			const float OFFSET_Y = 50.0f;	// Y座標オフセット
		}

		namespace left
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 120.0f, 386.0f, 0.0f);	// 左上位置
			}

			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x - 320.0f, 550.0f, 0.0f);	// 左中心位置
			const float OFFSET_X = 70.0f;	// X座標オフセット
		}

		namespace right
		{
			namespace up
			{
				const MyLib::Vector3 POS = MyLib::Vector3(left::up::POS.x + 640.0f, 386.0f, 0.0f);	// 右上位置
			}

			const MyLib::Vector3 POS = MyLib::Vector3(VEC3_SCREEN_CENT.x + 320.0f, 550.0f, 0.0f);	// 右中心位置
			const float OFFSET_X = 70.0f;	// X座標オフセット
		}
		const MyLib::Vector3 OFFSET = MyLib::Vector3(70.0f, 70.0f, 0.0f);	// 中心位置
	}

	namespace num
	{
		const std::string TEXTURE = "data\\TEXTURE\\number\\school.png";	// 数字テクスチャ
		const float WIDTH = 200.0f;	// 横幅
		const float ALPHARATE_MAX = 1.0f;	// 不透明度割合の最大値
		const float ALPHARATE_MIN = 0.2f;	// 不透明度割合の最大値
		const float SCALERATE_MAX = 1.0f;	// 拡大割合の最大値
		const float SCALERATE_MIN = 0.95f;	// 拡大割合の最小値
	}

	namespace teamside
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\team.png";	// 数字テクスチャ
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(1, 2);	// テクスチャ分割数
		const MyLib::Vector3 POS = MyLib::Vector3(640.0f, 310.0f, 0.0f);	// 中心位置
		const float WIDTH = 120.0f;		// 横幅
	}

	namespace gameset
	{
		const std::string TEXTURE = "data\\TEXTURE\\entry\\board.png";	// 数字テクスチャ
		const MyLib::Vector3 POS = MyLib::Vector3(640.0f, 360.0f, 0.0f);	// 中心位置
		const float WIDTH = 640.0f;		// 横幅
	}
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEntry_SetUpTeam::CEntry_SetUpTeam() : CEntryScene(),
	m_pTransUI	(nullptr),	// 遷移UI情報
	m_pBG		(nullptr)	// 背景情報
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		m_apTeamSideUI[i] = nullptr;	// チームサイドUI情報
		m_vecAddIdx[i].clear();			// 追加されたインデックス
		m_apNumInTeam[i] = nullptr;		// チーム人数情報
		m_nMaxChangeIdx[i] = -1;		// 最大数変更するインデックス
		m_nPlayerNum[i] = 0;			// プレイヤーの数
		m_fTimeNumInTeam[i] = 0.0f;		// チーム人数のタイマー
	}

	for (int i = 0; i < CGameManager::MAX_PLAYER; i++)
	{ // チーム合計のプレイヤー総数分繰り返す

		m_TeamSide[i].team = CGameManager::ETeamSide::SIDE_NONE;	// チームサイド
		m_TeamSide[i].nPadIdx = -1;	// 操作権インデックス
	}

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // パッド認識の最大数分繰り返す

		m_apPadUI[i] = nullptr;			// コントローラーUI情報
		m_TeamSide[i].nPadIdx = i;		// 操作権インデックス
		m_nEntryIdx[i] = -1;			// エントリーのインデックス
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
	// 背景の生成
	if (FAILED(CreateBG()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

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

	// チームサイドUIの生成
	if (FAILED(CreateTeamSideUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 前回のセットアップ読込
	Load();

	// 追加されたインデックスリセット // TODO：毎回リセットしちゃうから要検討
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		m_vecAddIdx[i].clear();
	}

	return S_OK;
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
		m_apNumInTeam[i]->SetPosition(MyLib::Vector3(390.0f + (640.0f * (float)i), 386.0f, 0.0f));	// TODO：初期生成位置の調整

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
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // チーム合計のプレイヤー総数分繰り返す

		// コントローラーUIの生成
		m_apPadUI[i] = CPadUI::Create
		( // 引数
			MyLib::Vector3(VEC3_SCREEN_CENT.x, 100.0f + (50.0f * (float)i), 0.0f),	// 位置
			pad::WIDTH,		// 横幅
			i,				// テクスチャ縦分割数
			PRIORITY		// 優先順位
		);
		if (m_apPadUI[i] == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 自動描画をOFFにする
		m_apPadUI[i]->SetEnableDisp(false);

		// テクスチャパターンの設定
		m_apPadUI[i]->SetPatternAnim(i);
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
// 背景の生成
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateBG()
{
	// 遷移UIの生成
	m_pBG = CObject2D::Create(PRIORITY);
	if (m_pBG == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 位置設定
	m_pBG->SetPosition(gameset::POS);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = pTexture->Regist(gameset::TEXTURE);
	m_pBG->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	m_pBG->SetSize(size);
	m_pBG->SetSizeOrigin(m_pBG->GetSize());

	return S_OK;
}

//==========================================================================
// チームサイドUIの生成
//==========================================================================
HRESULT CEntry_SetUpTeam::CreateTeamSideUI()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// 位置の設定
		MyLib::Vector3 setpos = teamside::POS;
		setpos.x += ((CGameManager::ETeamSide)(i) == CGameManager::ETeamSide::SIDE_LEFT) ? -320.0f : 320.0f;

		// チーム人数の生成
		m_apTeamSideUI[i] = CObject2D_Anim::Create( // 引数
			setpos,				// 位置
			teamside::PTRN.x,	// テクスチャ横分割数
			teamside::PTRN.y,	// テクスチャ縦分割数
			0.0f,				// 再生時間
			false,				// 自動破棄
			PRIORITY			// 優先順位
		);
		if (m_apTeamSideUI[i] == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 自分サイドのパターンに変更
		m_apTeamSideUI[i]->SetPatternAnim(i);

		// 自動再生をOFFにする
		m_apTeamSideUI[i]->SetEnableAutoPlay(false);

		// テクスチャの割当
		CTexture* pTexture = CTexture::GetInstance();
		int nTexID = pTexture->Regist(teamside::TEXTURE);
		m_apTeamSideUI[i]->BindTexture(nTexID);

		// 横幅を元にサイズを設定
		MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, teamside::WIDTH);
		size.y /= (float)teamside::PTRN.y;	// 分割数で割る
		m_apTeamSideUI[i]->SetSize(size);
		m_apTeamSideUI[i]->SetSizeOrigin(m_apTeamSideUI[i]->GetSize());
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

	// チーム人数の更新
	UpdateNumInUI(fDeltaTime, fDeltaRate, fSlowRate);

	// パッドUIの更新
	UpdatePadUI(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// チーム人数の更新
//==========================================================================
void CEntry_SetUpTeam::UpdateNumInUI(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す
		
		// チーム人数のタイマー
		m_fTimeNumInTeam[i] += fDeltaTime * fSlowRate;

#if 0
		// 操作権を持っている場合最大値
		const int nUserIdx = m_nMaxChangeIdx[i];	// ユーザーインデックス
		if (nUserIdx > -1)
		{
			m_fTimeNumInTeam[i] = 2.0f;
		}
#endif

		// nullは次へ
		if (m_apNumInTeam[i] == nullptr) continue;

		// 色取得
		D3DXCOLOR col = m_apNumInTeam[i]->GetColor();
		MyLib::Vector2 size = m_apNumInTeam[i]->GetSize(), sizeOrigin = m_apNumInTeam[i]->GetSizeOrigin();

		// 不透明度とスケール変更
		if (m_fTimeNumInTeam[i] <= 1.0f)
		{
			col.a = UtilFunc::Correction::EasingEaseIn(num::ALPHARATE_MIN, num::ALPHARATE_MAX, 0.0f, 1.0f, m_fTimeNumInTeam[i]);
		}
		else
		{
			col.a = UtilFunc::Correction::EasingEaseOut(num::ALPHARATE_MAX, num::ALPHARATE_MIN, 1.0f, 2.0f, m_fTimeNumInTeam[i]);
		}
		m_apNumInTeam[i]->SetColor(col);

		// スケール設定
		float scaleRate = UtilFunc::Transformation::ValueToRate(col.a, num::ALPHARATE_MIN, num::ALPHARATE_MAX);
		scaleRate = num::SCALERATE_MIN + (num::SCALERATE_MAX - num::SCALERATE_MIN) * scaleRate;
		m_apNumInTeam[i]->SetSize(sizeOrigin * scaleRate);

		if (m_fTimeNumInTeam[i] >= 2.0f)
		{
			m_fTimeNumInTeam[i] = 0.f;
		}
	}
}

//==========================================================================
// パッドUIの更新
//==========================================================================
void CEntry_SetUpTeam::UpdatePadUI(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		// 未エントリーの場合次へ
		const int nUserIdx = m_nEntryIdx[i];	// ユーザーインデックス
		if (nUserIdx <= -1) { continue; }

		// 準備完了している場合は矢印非表示
		if (m_apPadUI[nUserIdx] != nullptr)
		m_apPadUI[nUserIdx]->SetEnableDispArrow(!IsUserReady(nUserIdx));
	}
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

	// 背景の削除
	SAFE_KILL(m_pBG);

	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{ // チーム数分繰り返す

		// チーム人数の削除
		SAFE_KILL(m_apNumInTeam[i]);
		SAFE_KILL(m_apTeamSideUI[i]);
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
	std::vector<CPadUI*> vecNone;	// チーム指定なし配列
	std::vector<CPadUI*> vecLeft;	// 左チーム配列
	std::vector<CPadUI*> vecRight;	// 右チーム配列
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

	// 比較関数
	auto func = [](CPadUI* p1, CPadUI* p2)
	{
		// プレイヤーインデックス取得
		int nPlayerIdx1 = p1->GetPatternAnim();
		if (nPlayerIdx1 >= mylib_const::MAX_PLAYER) { nPlayerIdx1 /= 2; }

		// プレイヤーインデックス取得
		int nPlayerIdx2 = p1->GetPatternAnim();
		if (nPlayerIdx2 >= mylib_const::MAX_PLAYER) { nPlayerIdx2 /= 2; }

		return nPlayerIdx1 < nPlayerIdx2;
	};

	// 配列内要素をソートする
	std::sort(vecNone.begin(),  vecNone.end(),  func);
	std::sort(vecLeft.begin(),  vecLeft.end(),  func);
	std::sort(vecRight.begin(), vecRight.end(), func);

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
	PosAdjPadUIToNumIn(vecLeft, CGameManager::ETeamSide::SIDE_LEFT);

	// 右チームの位置補正
	PosAdjPadUIToNumIn(vecRight, CGameManager::ETeamSide::SIDE_RIGHT);

	// 左チーム人数調整の位置補正
	const int nLeftUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_LEFT];		// 左チーム人数調整パッドインデックス
	if (nLeftUpIdx > -1)
	{ // 操作するプレイヤーがいた場合

		// 位置を反映
		m_apPadUI[nLeftUpIdx]->SetPosition(pad::left::up::POS);

		// 矢印の位置は数字にする
		for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
		{
			m_apPadUI[nLeftUpIdx]->GetArrowUI(i)->SetPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_LEFT]->GetPosition());
			m_apPadUI[nLeftUpIdx]->GetArrowUI(i)->SetOriginPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_LEFT]->GetPosition());
		}
	}

	// 右チーム人数調整の位置補正
	const int nRightUpIdx = m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_RIGHT];	// 右チーム人数調整パッドインデックス
	if (nRightUpIdx > -1)
	{ // 操作するプレイヤーがいた場合

		// 位置を反映
		m_apPadUI[nRightUpIdx]->SetPosition(pad::right::up::POS);

		// 矢印の位置は数字にする
		for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
		{
			m_apPadUI[nRightUpIdx]->GetArrowUI(i)->SetPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_RIGHT]->GetPosition());
			m_apPadUI[nRightUpIdx]->GetArrowUI(i)->SetOriginPosition(m_apNumInTeam[CGameManager::ETeamSide::SIDE_RIGHT]->GetPosition());
		}
	}
}

//==========================================================================
// 内側にいる人数をもとにコントローラーUI位置補正
//==========================================================================
void CEntry_SetUpTeam::PosAdjPadUIToNumIn(const std::vector<CPadUI*>& vecPadUI, CGameManager::ETeamSide side)
{
	// 左チームの位置補正
	const int nNumLeft = (int)vecPadUI.size();	// 要素数

	MyLib::Vector3 setpos;	// 設定位置
	switch (side)
	{
	case CGameManager::SIDE_LEFT:
		setpos = pad::left::POS;
		break;

	case CGameManager::SIDE_RIGHT:
		setpos = pad::right::POS;
		break;
	}

	if (nNumLeft == 1)
	{// 1のとき中心
		vecPadUI[0]->SetPosition(setpos);
	}
	else if (nNumLeft == 2)
	{// 2

		// 左にずらす
		setpos.x -= pad::OFFSET.x;
		for (int i = 0; i < nNumLeft; i++)
		{
			vecPadUI[i]->SetPosition(setpos);

			// 右にずらす
			setpos.x += pad::OFFSET.x * 2.0f;
		}

	}
	else if (nNumLeft == 3)
	{// 3

		// 左にずらす
		setpos.x -= pad::OFFSET.x;
		setpos.y -= pad::OFFSET.y * 0.5f;
		for (int i = 0; i < 2; i++)
		{
			vecPadUI[i]->SetPosition(setpos);

			// 右にずらす
			if (i == 0)
			{
				setpos.x += pad::OFFSET.x * 2.0f;
			}
		}

		// 最後の位置
		setpos.x -= pad::OFFSET.x * 1.0f;
		setpos.y += pad::OFFSET.y;
		vecPadUI[2]->SetPosition(setpos);

	}
	else if (nNumLeft == 4)
	{// 4

		// 左にずらす
		setpos.x -= pad::OFFSET.x;
		setpos.y -= pad::OFFSET.y * 0.5f;
		for (int i = 0; i < 4; i++)
		{
			vecPadUI[i]->SetPosition(setpos);

			if (i == 1)
			{// 右端
				setpos.x -= pad::OFFSET.x * 2.0f;
				setpos.y += pad::OFFSET.y;
			}
			else
			{
				// 設定位置ずらす
				setpos.x += pad::OFFSET.x * 2.0f;
			}
		}
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
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, i)
		||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, i))
		{ // エントリー操作が行われた場合

			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{ // パッド認識の最大数分繰り返す

				// 既にエントリーされている場合抜ける
				if (m_nEntryIdx[j] == i) { break; }

				// 既にエントリーされている場合次へ
				if (m_nEntryIdx[j] != -1) { continue; }

				// 今回のプレイヤーインデックスを追加
				m_nEntryIdx[j] = i;

				// コントローラーUIの自動描画をONにする
				m_apPadUI[i]->SetEnableDisp(true);

				// サウンドの再生
				PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_05);
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
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, nUserIdx)
		||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, nUserIdx))
		{ // 離脱操作が行われた場合

			// サウンドの再生
			PLAY_SOUND(CSound::ELabel::LABEL_SE_CANCEL);

			std::vector<int>::iterator itr;	// 準備完了配列イテレーター
			if (IsUserReady(nUserIdx, &itr))
			{ // 準備完了している場合

				// 準備完了配列からイテレーターを削除
				m_vecAddIdx[m_TeamSide[nUserIdx].team].erase(itr);

				// キャンセル
				m_apPadUI[nUserIdx]->Cancel();
				break;
			}
			else
			{ // 準備完了していない場合

				// エントリーを解除
				DeleteEntry(&m_nEntryIdx[i]);

				auto func = [](int n1, int n2)
				{
					if (n1 == -1) return false;
					if (n2 == -1) return true;

					return n1 < n2;
				};

				std::sort(&m_nEntryIdx[0], &m_nEntryIdx[0] + mylib_const::MAX_PLAYER, func);
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
			||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_LEFT))
			{
				if (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_LEFT)
				{ // 左端じゃない時に左移動操作が行われた場合

					// サウンドの再生
					PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

					// 左に移動
					m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_LEFT : CGameManager::ETeamSide::SIDE_NONE;
					m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_L);
				}
			}
			else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx)
				 ||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_RIGHT))
			{
				if (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_RIGHT)
				{ // 右端じゃない時に右移動操作が行われた場合

					// サウンドの再生
					PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

					// 右に移動
					m_TeamSide[nUserIdx].team = (m_TeamSide[nUserIdx].team == CGameManager::ETeamSide::SIDE_NONE) ? CGameManager::ETeamSide::SIDE_RIGHT : CGameManager::ETeamSide::SIDE_NONE;
					m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_R);
				}
			}
		}

		// 選択チームを保存
		const int nSide = (int)m_TeamSide[nUserIdx].team;

		//--------------------------
		// チーム内人数変え
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nUserIdx)
		||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_UP))
		{ // 上移動操作が行われた場合

			if (IsUserTeamSelect(nUserIdx))
			{ // チーム選択中

				assert(!(nSide <= CGameManager::ETeamSide::SIDE_NONE || nSide >= CGameManager::ETeamSide::SIDE_MAX));
				if (m_nMaxChangeIdx[nSide] <= -1)
				{ // 最大数変更の操作権を誰も持っていない

					// 自分のサイドを変更する用インデックス保持
					m_nMaxChangeIdx[nSide] = nUserIdx;

						// サウンドの再生
						PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

						// 矢印を数字用にする
						CArrowUI* pArrow = nullptr;
					for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
					{
						pArrow = m_apPadUI[nUserIdx]->GetArrowUI(i);
						pArrow->SetOffset(pad::ARROWSPACE_NUMBER);
						pArrow->SetSizeByWidth(pad::ARROWWIDTH_NUMBER);
					}
				}
			}
		}

		//--------------------------
		// 準備完了操作
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, nUserIdx)
		||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, nUserIdx))
		{ // 準備完了操作が行われた場合

			if (IsUserTeamSelect(nUserIdx)
			&&  !IsUserMaxChange(nUserIdx))
			{ // チーム選択中且つ、最大数変更を自分がしていない場合

				assert(!(nSide <= CGameManager::ETeamSide::SIDE_NONE || nSide >= CGameManager::ETeamSide::SIDE_MAX));
				const int nCurInTeam = static_cast<int>(m_vecAddIdx[nSide].size());	// 現在のチーム人数
				if (!IsUserReady(nUserIdx) && m_nPlayerNum[nSide] > nCurInTeam)
				{ // 準備未完了且つ、最大チーム人数未満の場合

					// 準備完了配列に追加
					m_vecAddIdx[nSide].push_back(nUserIdx);

					// 決定
					m_apPadUI[nUserIdx]->Decide();

					// サウンドの再生
					PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_00);
				}
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
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nUserIdx)
		||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
		{ // 減算操作が行われた場合

			// チーム人数を減算
			m_nPlayerNum[nSide]--;

			// 左に移動
			m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_L);

			// サウンドの再生
			if (m_nPlayerNum[nSide] >= 1)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
			}
			else
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nUserIdx)
			 ||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT))
		{ // 加算操作が行われた場合

			// チーム人数を加算
			m_nPlayerNum[nSide]++;

			// 左に移動
			m_apPadUI[nUserIdx]->SetAction(CArrowUI::EDirection::DIRECTION_R);

			// サウンドの再生
			if (m_nPlayerNum[nSide] <= CGameManager::MAX_SIDEPLAYER)
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
			}
			else
			{
				PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
			}
		}

		// チーム人数を補正
		m_nPlayerNum[nSide] = UtilFunc::Transformation::Clamp(m_nPlayerNum[nSide], 1, CGameManager::MAX_SIDEPLAYER);

		//--------------------------
		// 最大数変更解除
		//--------------------------
		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nUserIdx)
		||  pPad->GetLStickTrigger(nUserIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_DOWN))
		{ // 下移動操作が行われた場合

			// サウンドの再生
			PLAY_SOUND(CSound::ELabel::LABEL_SE_CONTROERMOVE);

			// 矢印の間隔をコントローラー用にする
			CArrowUI* pArrow = nullptr;
			for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
			{
				pArrow = m_apPadUI[nUserIdx]->GetArrowUI(i);
				pArrow->SetOffset(pArrow->GetOffsetOrigin());
				pArrow->SetSizeByWidth(pArrow->GetSizeWidthOrigin());
			}

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

		// セーブ
		Save(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT], m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT]);

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
bool CEntry_SetUpTeam::IsUserTeamSelect(const int nUserIdx) const
{
	// 選択中チームの指定があるかを返す
	return (m_TeamSide[nUserIdx].team != CGameManager::ETeamSide::SIDE_NONE);
}

//==========================================================================
// 最大数変更フラグ取得
//==========================================================================
bool CEntry_SetUpTeam::IsUserMaxChange(const int nUserIdx) const
{
	// チームが未選択の場合抜ける
	const int nSide = m_TeamSide[nUserIdx].team;	// 選択チーム
	if (nSide == CGameManager::ETeamSide::SIDE_NONE) { return false; }

	// 最大数変更の操作権インデックスが引数ユーザーかを返す
	return (m_nMaxChangeIdx[nSide] == nUserIdx);
}

//==========================================================================
// エントリー中プレイヤーインデックス取得
//==========================================================================
std::vector<int> CEntry_SetUpTeam::GetEntryPlayerIdx()
{
	std::vector<int> vecIdx;
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{ // パッド認識の最大数分繰り返す

		// 未エントリーの場合次へ
		const int nUserIdx = m_nEntryIdx[i];	// ユーザーインデックス
		if (nUserIdx <= -1) { continue; }

		// 準備が完了していない場合次へ
		if (!IsUserReady(nUserIdx)) { continue; }

		// インデックス追加
		vecIdx.push_back(nUserIdx);
	}

	return vecIdx;
}

//==========================================================================
// セーブ処理
//==========================================================================
void CEntry_SetUpTeam::Save(std::vector<int>& rVecLeft, std::vector<int>& rVecRight)
{
	// ファイルを開く
	std::ofstream file(TEXTFILE);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "チーム設定の書き出しに失敗！", "警告！", MB_ICONWARNING);
	}

	// 見出しの書き出し
	file << "#==============================================================================" << std::endl;
	file << "#" << std::endl;
	file << "#	チーム設定テキスト [team_setting.txt]" << std::endl;
	file << "#	Author : 藤田 勇一" << std::endl;
	file << "#" << std::endl;
	file << "#==============================================================================" << std::endl;

	// チーム人数の書き出し
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "#	チーム人数情報" << std::endl;
	file << "#------------------------------------------------------------------------------" << std::endl;
	file << "TEAMIDX" << std::endl;

	// 左チームの書き出し
	file << "	LEFT  = ";
	for (const int nPadIdx : rVecLeft)
	{
		file << nPadIdx << " ";
	}
	file << "END" << std::endl;

	// 右チームの書き出し
	file << "	RIGHT = ";
	for (const int nPadIdx : rVecRight)
	{
		file << nPadIdx << " ";
	}
	file << "END" << std::endl;

	file << "END_TEAMIDX" << std::endl;

	// ファイルを閉じる
	file.close();
}

//==========================================================================
// ロード
//==========================================================================
void CEntry_SetUpTeam::Load()
{
	// ファイルを開く
	std::ifstream file(TEXTFILE);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "チーム設定の読み込みに失敗！", "警告！", MB_ICONWARNING);
	}

	// ファイルを読込
	std::string str;	// 読込文字列
	while (file >> str)
	{ // ファイルの終端ではない場合ループ

		if (str.front() == '#') { std::getline(file, str); }	// コメントアウト
		else if (str == "TEAMIDX")
		{
			do { // END_TEAMIDXを読み込むまでループ

				// 文字列を読み込む
				file >> str;

				if (str.front() == '#') { std::getline(file, str); }	// コメントアウト
				else if (str == "LEFT")
				{
					// ロードインデックス
					LoadIdx(&file, CGameManager::ETeamSide::SIDE_LEFT);
				}
				else if (str == "RIGHT")
				{
					// ロードインデックス
					LoadIdx(&file, CGameManager::ETeamSide::SIDE_RIGHT);
				}
			} while (str != "END_TEAMIDX");	// END_TEAMIDXを読み込むまでループ
		}
	}

	for (int team = 0; team < CGameManager::ETeamSide::SIDE_MAX; team++)
	{ // チーム数分繰り返す

		// 最低値に補正
		if (m_nPlayerNum[team] == 0) { m_nPlayerNum[team]++; }
	}

	// ファイルを閉じる
	file.close();
}

//==========================================================================
// ロードインデックス
//==========================================================================
void CEntry_SetUpTeam::LoadIdx(std::ifstream* pFile, const CGameManager::ETeamSide team)
{
	// ファイルストリームが未設定の場合抜ける
	if (pFile == nullptr) { assert(false); return; }

	std::string str;	// 読込文字列
	*pFile >> str;		// ＝を読込
	while (1)
	{ // ENDを読み込むまでループ

		// 文字列を読み込む
		*pFile >> str;

		if (str == "END")
		{
			break;
		}
		else
		{
			int nPadIdx = std::stoi(str);
			m_nPlayerNum[team]++;
			if (nPadIdx <= -1) { continue; }

			for (int j = 0; j < mylib_const::MAX_PLAYER; j++)
			{ // パッド認識の最大数分繰り返す

				// 既にエントリーされている場合次へ
				if (m_nEntryIdx[j] >= 0 && m_nEntryIdx[j] != nPadIdx) { continue; }

				// 今回のプレイヤーインデックスを追加
				m_nEntryIdx[j] = nPadIdx;

				// コントローラーUIの自動描画をONにする
				m_apPadUI[nPadIdx]->SetEnableDisp(true);
				break;
			}

			// 選択チーム設定
			m_TeamSide[nPadIdx].team = team;
		}
	}
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
			Save(m_vecAddIdx[CGameManager::ETeamSide::SIDE_LEFT], m_vecAddIdx[CGameManager::ETeamSide::SIDE_RIGHT]);
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
// セーブ初期化
//==========================================================================
void CEntry_SetUpTeam::SaveInit()
{
	// セーブ処理
	std::vector<int> vec;
	Save(vec, vec);
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
	if (nPlayerIdx >= CGameManager::MAX_PLAYER || nPlayerIdx <= -1) return 9999999;
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

		// チーム指定がない次へ
		if (m_TeamSide[i].team == CGameManager::SIDE_NONE) { continue; }

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

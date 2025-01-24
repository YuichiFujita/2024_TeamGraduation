//=============================================================================
// 
// 操作待機処理 [title_controllwait.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "title_controllwait.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "titleLogo.h"
#include "titlestudent.h"
#include "fade.h"
#include "option_BBS.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int PLAYER_MIN = 10;	// プレイヤー最少人数
	const int PLAYER_MAX = 14;	// プレイヤー最大人数

	const std::string TEXTURE_SELECT[CTitle_ControllWait::ESelect::SELECT_MAX] =	// 選択肢のテクスチャ
	{
		"data\\TEXTURE\\title\\battle.png",	// 試合開始
		"data\\TEXTURE\\title\\more.png",	// 色々
	};
	const std::string TEXTURE_BG[CTitle_ControllWait::ESelect::SELECT_MAX] =	// 選択肢のテクスチャ
	{
		"data\\TEXTURE\\title\\battle_BG.png",	// 試合開始
		"data\\TEXTURE\\title\\more_BG.png",	// 色々
	};

	const float SIZEHEIGHT_SELECT[CTitle_ControllWait::ESelect::SELECT_MAX] =	// 選択肢のテクスチャ
	{
		100.0f,	// 試合開始
		140.0f,	// 色々
	};
	const float SIZEHEIGHT_BG = 200.0f;	// 背景のサイズ

	const MyLib::Vector3 POSITION_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// 選択肢の位置
	{
		MyLib::Vector3(340.0f, 500.0f, 0.0f),	// 試合開始
		MyLib::Vector3(940.0f, 500.0f, 0.0f),	// 色々
	};
	const D3DXCOLOR COLOR_NOTSELECT = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);	// 選択してない色
	const float MARKERTIME = 0.2f;	// 選択マーカーの時間
	const float SCALE_SELECT = 0.7f;	// 選択マーカーのスケール
}

namespace Timer
{
	namespace State
	{
		const float WAIT = 15.0f;	// 待機
		const float TRANSITION = 0.2f;	// 遷移
		const float TRANSITION_WAIT = 0.5f;		// 遷移待機
		const float TRANSITION_SECOND = 0.4f;	// 遷移
		const float TRANSITION_LAST = 0.5f;		// 遷移(ラスト)
		const float SCALE = 1.0f;		// 拡大率
	}

	namespace BG
	{
		const float SPAWN = 0.3f;	// 出現
		const float ROTATE = 2.0f;	// 出現
	}

	namespace SandSmoke
	{
		const float SPAWN = 3.5f;	// 生成
		const int RANDRANGE = 8;	// ランダム範囲(10分の1秒)
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle_ControllWait::STATE_FUNC CTitle_ControllWait::m_StateFunc[] =	// 状態関数
{
	&CTitle_ControllWait::StateWait,					// 待機
	&CTitle_ControllWait::StateControll,				// 操作
	&CTitle_ControllWait::StateTransitionMoreFirst,		// 遷移
	&CTitle_ControllWait::StateTransitionWait,			// 遷移待機
	&CTitle_ControllWait::StateTransitionMoreSecond,	// 遷移
	&CTitle_ControllWait::StateTransitionMoreLast,		// 遷移(ラスト)
	&CTitle_ControllWait::StateTansitionBack,			// 戻る
};

CTitle_ControllWait::STATEBG_FUNC CTitle_ControllWait::m_StateBGFunc[] =	// 状態関数
{
	&CTitle_ControllWait::StateBGSpawn,					// 待機
	&CTitle_ControllWait::StateBGLoop,					// 待機
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle_ControllWait::CTitle_ControllWait() : CTitleScene()
{
	// 状態
	m_state = EState::STATE_WAIT;		// 状態
	m_fStateTime = 0.0f;				// 状態時間

	// 背景状態
	m_stateBG = EStateBG::STATEBG_SPAWN;		// 状態
	m_fStateTimeBG = 0.0f;	// 状態時間

	// 選択肢
	m_select = ESelect::SELECT_BATTLE;

	// その他
	m_pLogo = nullptr;			// ロゴ
	m_pOptionBBS = nullptr;		// 掲示板
	m_fTimeMarker = 0.0f;		// マーカーのタイマー
	m_fTimeSandSmoke = 0.0f;		// 砂煙のタイマー
	m_fIntervalSandSmoke = 0.0f;	// 砂煙のインターバル
	m_vecTitleStudent.clear();	// タイトルの生徒

}

//==========================================================================
// デストラクタ
//==========================================================================
CTitle_ControllWait::~CTitle_ControllWait()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitle_ControllWait::Init()
{
	// 初期化処理
	if (FAILED(CTitleScene::Init())) return E_FAIL;

	// ロゴの生成
	m_pLogo = CTitleLogo::Create();
	if (m_pLogo == nullptr) return E_FAIL;

	// プレイヤー生成
	int num = UtilFunc::Transformation::Random(PLAYER_MIN, PLAYER_MAX);
	for (int i = 0; i < num; i++)
	{
		// タイトルの生徒生成
		m_vecTitleStudent.push_back(CTitleStudent::Create());
		if (m_vecTitleStudent.back() == nullptr) return E_FAIL;
	}

	// 背景生成
	if (FAILED(CreateBG()))
	{
		return E_FAIL;
	}

	// 選択肢生成
	if (FAILED(CreateSelect()))
	{
		return E_FAIL;
	}

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::ELabel::LABEL_BGM_TITLE);

	// 掲示板生成
	m_pOptionBBS = COption_BBS::Create();

	// 砂煙
	m_fTimeSandSmoke = 0.0f;		// 砂煙のタイマー
	m_fIntervalSandSmoke = Timer::SandSmoke::SPAWN;	// 砂煙のインターバル
	return S_OK;
}

//==========================================================================
// 選択肢生成
//==========================================================================
HRESULT CTitle_ControllWait::CreateSelect()
{
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// 生成処理
		m_pSelectUI[i] = CObject2D::Create(1);
		if (m_pSelectUI[i] == nullptr) return E_FAIL;

		// オブジェクトの種類設定
		m_pSelectUI[i]->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

		// テクスチャ設定
		CTexture* pTexture = CTexture::GetInstance();
		int texID = CTexture::GetInstance()->Regist(TEXTURE_SELECT[i]);
		m_pSelectUI[i]->BindTexture(texID);


		// 画像サイズ取得
		MyLib::Vector2 size = pTexture->GetImageSize(texID);

		// 縦幅を元にサイズ計算
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[i]);
		m_pSelectUI[i]->SetSize(MyLib::Vector2(size.x, size.y));
		m_pSelectUI[i]->SetSizeOrigin(m_pSelectUI[i]->GetSize());

		// 位置設定
		m_pSelectUI[i]->SetPosition(POSITION_SELECT[i]);
	}

	return S_OK;
}

//==========================================================================
// 背景生成
//==========================================================================
HRESULT CTitle_ControllWait::CreateBG()
{
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// 生成処理
		m_pBG[i] = CObject2D::Create(0);
		if (m_pBG[i] == nullptr) return E_FAIL;

		// オブジェクトの種類設定
		m_pBG[i]->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

		// テクスチャ設定
		CTexture* pTexture = CTexture::GetInstance();
		int texID = CTexture::GetInstance()->Regist(TEXTURE_BG[i]);
		m_pBG[i]->BindTexture(texID);


		// 画像サイズ取得
		MyLib::Vector2 size = pTexture->GetImageSize(texID);

		// 縦幅を元にサイズ計算
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_BG);
		m_pBG[i]->SetSize(MyLib::Vector2(size.x, size.y));
		m_pBG[i]->SetSizeOrigin(m_pBG[i]->GetSize());

		// 位置設定
		m_pBG[i]->SetPosition(POSITION_SELECT[i]);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle_ControllWait::Uninit()
{
	delete this;
}

//==========================================================================
// 削除処理
//==========================================================================
void CTitle_ControllWait::Kill()
{
	// ロゴ
	SAFE_KILL(m_pLogo);

	// 生徒
	for (int i = 0; i < static_cast<int>(m_vecTitleStudent.size()); i++)
	{
		SAFE_KILL(m_vecTitleStudent[i]);
	}

	// 掲示板削除
	SAFE_KILL(m_pOptionBBS);

	// 背景削除
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pBG[i]);
	}

	// 選択肢UI削除
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pSelectUI[i]);
	}

	// 終了処理
	Uninit();

	// エフェクト全削除
	CMyEffekseer::GetInstance()->StopAll();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle_ControllWait::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新
	CTitleScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// 色更新
	UpdateColor(fDeltaTime, fDeltaRate, fSlowRate);

	// 砂煙更新
	UpdateSandSmoke(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG
	ImGui::Text("%s", magic_enum::enum_name(m_select));
#endif // _DEBUG


}

//==========================================================================
// 砂煙更新
//==========================================================================
void CTitle_ControllWait::UpdateSandSmoke(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 砂煙のタイマー加算
	m_fTimeSandSmoke += fDeltaTime * fSlowRate;;
	if (m_fTimeSandSmoke < m_fIntervalSandSmoke) return;

	// インターバル再抽選
	m_fTimeSandSmoke = 0.0f;
	m_fIntervalSandSmoke = 
		Timer::SandSmoke::SPAWN + 
		UtilFunc::Transformation::Random(-Timer::SandSmoke::RANDRANGE, Timer::SandSmoke::RANDRANGE) * 0.1f;

	// 生成位置算出
	MyLib::Vector3 pos;
	pos.x = UtilFunc::Transformation::Random(-6, 6) * 100.0f;
	pos.z = 100.0f + UtilFunc::Transformation::Random(-6, 2) * 100.0f;

	// ニブイチで逆転
	float rotY = 0.0f;
	if (rand() % 2 == 0)
	{
		rotY += D3DX_PI;
	}

	// 砂煙生成
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_SANDSMOKE,
		pos,
		MyLib::Vector3(0.0f, rotY, 0.0f),	// 向き
		MyLib::Vector3(),
		10.0f);
}

//==========================================================================
// 選択肢更新
//==========================================================================
void CTitle_ControllWait::UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() == CInstantFade::EState::STATE_FADEIN)
	{// フェード中は抜ける
		return;
	}

	// インプット情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{// 左(逆ループ)

		int nSelect = (m_select + (ESelect::SELECT_MAX - 1)) % ESelect::SELECT_MAX;
		m_select = (ESelect)nSelect;

		// タイマーリセット
		m_fStateTime = 0.0f;

		// マーカータイマーリセット
		m_fTimeMarker = 0.0f;
		SetStateBG(EStateBG::STATEBG_SPAWN);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{// 右(ループ)

		int nSelect = (m_select + 1) % ESelect::SELECT_MAX;
		m_select = (ESelect)nSelect;

		// タイマーリセット
		m_fStateTime = 0.0f;

		// マーカータイマーリセット
		m_fTimeMarker = 0.0f;
		SetStateBG(EStateBG::STATEBG_SPAWN);
	}

	// 遷移
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// 決定時処理
		Decide();
	}
}

//==========================================================================
// 決定
//==========================================================================
void CTitle_ControllWait::Decide()
{
	switch (m_select)
	{
	case CTitle_ControllWait::SELECT_BATTLE:

		// 遷移
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
		break;

	case CTitle_ControllWait::SELECT_MORE:
	{
		// カメラの初期位置を現在の値に設定
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// 向きも現在のにする
		pCamera->SetOriginRotation(pCamera->GetRotation());

		// ロゴを待機させる
		m_pLogo->SetState(CTitleLogo::EState::STATE_WAIT);

		// オプションメニューの状態NONE
		m_pOptionBBS->GetOptionMenu()->SetState(COptionMenu::EState::STATE_NONE);

		// 状態遷移
		SetState(EState::STATE_TRANSITION_MORE_1st);

		// その他へ遷移
		GET_MANAGER->GetInstantFade()->SetFade(MyLib::color::White(), Timer::State::TRANSITION);
	}
		break;

	default:
		break;
	}
}

//==========================================================================
// 色更新
//==========================================================================
void CTitle_ControllWait::UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// マーカーのタイマー加算
	m_fTimeMarker += fDeltaTime * fSlowRate;

	// 透明度更新
	float color = UtilFunc::Correction::EasingQuintOut(COLOR_NOTSELECT.r, 1.0f, 0.0f, MARKERTIME, m_fTimeMarker);
	D3DXCOLOR selectColor = D3DXCOLOR(color, color, color, 1.0f);

	// 選択肢のUI
	for (const auto& select : m_pSelectUI)
	{
		select->SetColor(MyLib::color::White() - (selectColor - COLOR_NOTSELECT));
		select->SetAlpha(1.0f);
	}
	m_pSelectUI[m_select]->SetColor(selectColor);

	// 拡大率
	float scale = UtilFunc::Correction::EasingQuintOut(0.5f, 1.0f, 0.0f, MARKERTIME, m_fTimeMarker);
	float scaleRate = (SCALE_SELECT + (1.0f - SCALE_SELECT) * scale);

	for (const auto& select : m_pSelectUI)
	{
		select->SetSize(select->GetSizeOrigin() * (1.0f - (scaleRate - SCALE_SELECT)));
	}
	m_pSelectUI[m_select]->SetSize(m_pSelectUI[m_select]->GetSizeOrigin() * scaleRate);

	// 選択肢のサイズ更新
	UpdateSelectSize(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 選択肢のサイズ更新
//==========================================================================
void CTitle_ControllWait::UpdateSelectSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 拡縮
	float scale = sinf(D3DX_PI * (m_fTimeMarker / Timer::State::SCALE * 0.5f)) * 0.025f;

	MyLib::Vector2 size = m_pSelectUI[m_select]->GetSize();

	// 今のサイズに加算
	size += m_pSelectUI[m_select]->GetSizeOrigin() * scale;
	m_pSelectUI[m_select]->SetSize(size);
}

//==========================================================================
// 状態更新
//==========================================================================
void CTitle_ControllWait::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;
	m_fStateTimeBG += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	(this->*(m_StateBGFunc[m_stateBG]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 待機
//==========================================================================
void CTitle_ControllWait::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 選択肢更新
	UpdateSelect(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_fStateTime >= Timer::State::WAIT)
	{
		// シーン切り替え
		CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_SUSURU);
	}
}

//==========================================================================
// 操作
//==========================================================================
void CTitle_ControllWait::StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;

	// オプションメニューで戻ってたら遷移する
	if (m_pOptionBBS->GetOptionMenu()->IsBack())
	{
		SetState(EState::STATE_TRANSITION_BACK);
	}
}

//==========================================================================
// 色々に遷移(前半)
//==========================================================================
void CTitle_ControllWait::StateTransitionMoreFirst(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// カメラ取得
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// 奥へ高速移動
	MyLib::Vector3 posR = UtilFunc::Correction::EaseInExpo(pCamera->GetPositionROrigin(), MyLib::Vector3(0.0f, 0.0f, 1500.0f), 0.0f, Timer::State::TRANSITION, m_fStateTime);
	pCamera->SetPositionR(posR);

	if (m_fStateTime >= Timer::State::TRANSITION)
	{// 後半へ遷移

		// カメラの初期位置を現在の値に設定
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// 向きリセット
		pCamera->SetRotation(MyLib::Vector3(0.0f, D3DX_PI * 0.2f, pCamera->GetOriginRotation().z));
		pCamera->SetOriginRotation(pCamera->GetRotation());

		// 背景消す
		for (const auto& bg : m_pBG)
		{
			bg->SetEnableDisp(false);
		}

		// 選択肢のUI消す
		for (const auto& select : m_pSelectUI)
		{
			select->SetEnableDisp(false);
		}

		// 遷移待機
		SetState(EState::STATE_TRANSITION_WAIT);
	}

}

//==========================================================================
// 遷移待機
//==========================================================================
void CTitle_ControllWait::StateTransitionWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= Timer::State::TRANSITION_WAIT)
	{// 後半へ遷移
		SetState(EState::STATE_TRANSITION_MORE_2nd);
	}
}

//==========================================================================
// 色々に遷移(後半)
//==========================================================================
void CTitle_ControllWait::StateTransitionMoreSecond(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// カメラ取得
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// 掲示板の位置へ移動
	MyLib::Vector3 posR = UtilFunc::Correction::EasingQuintInOut(pCamera->GetPositionROrigin(), m_pOptionBBS->GetPosition(), 0.0f, Timer::State::TRANSITION_SECOND, m_fStateTime);
	pCamera->SetPositionR(posR);

	// 向き
	MyLib::Vector3 rot = UtilFunc::Correction::EasingQuintInOut(pCamera->GetOriginRotation(), MyLib::Vector3(0.0f, 0.0f, pCamera->GetOriginRotation().z), 0.0f, Timer::State::TRANSITION_SECOND, m_fStateTime);
	pCamera->SetRotation(rot);

	if (m_fStateTime >= Timer::State::TRANSITION_SECOND)
	{// ラストへ遷移

		// カメラの初期位置を現在の値に設定
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// 向きリセット
		pCamera->SetOriginRotation(pCamera->GetRotation());

		SetState(EState::STATE_TRANSITION_MORE_LAST);
	}
}

//==========================================================================
// 色々に遷移(ラスト)
//==========================================================================
void CTitle_ControllWait::StateTransitionMoreLast(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// カメラ取得
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// 向き
	MyLib::Vector3 rot = UtilFunc::Correction::EasingQuintInOut(pCamera->GetOriginRotation(), MyLib::Vector3(), 0.0f, Timer::State::TRANSITION_SECOND, m_fStateTime);
	pCamera->SetRotation(rot);

	// 距離
	float distance = UtilFunc::Correction::EasingQuintInOut(pCamera->GetDistanceOrigin(), 150.0f, 0.0f, Timer::State::TRANSITION_LAST, m_fStateTime);
	pCamera->SetDistance(distance);

	if (m_fStateTime >= Timer::State::TRANSITION_SECOND)
	{// 操作へ遷移

		// カメラの初期位置を現在の値に設定
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// オプションメニュー選択できるようにする
		m_pOptionBBS->GetOptionMenu()->SetState(COptionMenu::EState::STATE_SELECT);

		SetState(EState::STATE_CONTROLL);
	}
}

//==========================================================================
// 戻る
//==========================================================================
void CTitle_ControllWait::StateTansitionBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// カメラ取得
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// 奥へ高速移動
	MyLib::Vector3 posR = UtilFunc::Correction::EaseInExpo(pCamera->GetPositionROrigin(), MyLib::Vector3(0.0f, 0.0f, -1500.0f), 0.0f, Timer::State::TRANSITION, m_fStateTime);
	pCamera->SetPositionR(posR);

	if (m_fStateTime >= Timer::State::TRANSITION)
	{// 待機へ遷移
		SetState(EState::STATE_WAIT);

		// ロゴ復活
		m_pLogo->SetState(CTitleLogo::EState::STATE_LOOP);

		// 背景復活
		for (const auto& bg : m_pBG)
		{
			bg->SetEnableDisp(true);
		}

		// 選択肢のUI復活
		for (const auto& select : m_pSelectUI)
		{
			select->SetEnableDisp(true);
		}

		// リセット
		pCamera->ResetByMode(CScene::MODE::MODE_TITLE);

		// オプションメニュー選択肢リセット
		m_pOptionBBS->GetOptionMenu()->SetSelect(COptionMenu::ESelect::SELECT_MASTERVOLUME);
	}
}

//==========================================================================
// 出現
//==========================================================================
void CTitle_ControllWait::StateBGSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 透明度更新
	float alpha = UtilFunc::Correction::EasingQuintOut(0.0f, 1.0f, 0.0f, Timer::BG::SPAWN, m_fStateTimeBG);
	D3DXCOLOR selectColor = MyLib::color::White(alpha);

	// 拡縮
	float scale = UtilFunc::Correction::EasingQuintInOut(0.0f, 1.0f, 0.0f, Timer::BG::SPAWN, m_fStateTimeBG);

	// 回転して登場
	float rotZ = UtilFunc::Correction::EasingQuintInOut(D3DX_PI * 4.0f, 0.0f, 0.0f, Timer::BG::SPAWN, m_fStateTimeBG);

	for (const auto& select : m_pBG)
	{
		// 色
		select->SetColor(MyLib::color::White(1.0f - UtilFunc::Correction::EasingQuintOut(0.0f, 0.4f, 0.0f, 1.0f, alpha)));

		// 拡縮
		select->SetSize(select->GetSizeOrigin() * UtilFunc::Correction::EasingQuintOut(1.0f, SCALE_SELECT, 0.0f, 1.0f, scale));

		// 回転
		select->SetRotation(MyLib::Vector3());
	}
	m_pBG[m_select]->SetColor(selectColor);
	m_pBG[m_select]->SetSize(m_pBG[m_select]->GetSizeOrigin() * scale);
	m_pBG[m_select]->SetRotation(MyLib::Vector3(0.0f, 0.0f, rotZ));

	// 状態遷移
	if (m_fStateTimeBG >= Timer::BG::SPAWN)
	{
		m_stateBG = EStateBG::STATEBG_LOOP;
		m_fStateTimeBG = 0.0f;
	}
}

//==========================================================================
// ループ
//==========================================================================
void CTitle_ControllWait::StateBGLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 回転
	float rotZ = sinf(D3DX_PI * (m_fStateTimeBG / Timer::BG::ROTATE)) * (D3DX_PI * 0.01f);
	m_pBG[m_select]->SetRotation(MyLib::Vector3(0.0f, 0.0f, rotZ));

	// 拡縮
	float scale = 1.0f + sinf(D3DX_PI * (m_fStateTimeBG / Timer::BG::ROTATE * 0.5f)) * 0.05f;
	m_pBG[m_select]->SetSize(m_pBG[m_select]->GetSizeOrigin() * scale);

	if (m_fStateTimeBG >= Timer::BG::ROTATE * 2.0f)
	{
		m_fStateTimeBG = 0.0f;
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitle_ControllWait::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitle_ControllWait::SetStateBG(EStateBG state)
{
	m_stateBG = state;
	m_fStateTimeBG = 0.0f;
}
//=============================================================================
// 
//  サンプル_キャラクター処理 [sample_character.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "sample_character.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "calculation.h"
#include "shadow.h"
#include "game.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\tyuuni\\setup_player.txt";	// キャラクターファイル
	
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CSample_Character::STATE_FUNC CSample_Character::m_StateFunc[] =
{
	&CSample_Character::StateNone,		// なし
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CSample_Character> CSample_Character::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CSample_Character::CSample_Character(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;	// 状態
	m_mMatcol = mylib_const::DEFAULT_COLOR;		// マテリアルの色
	
	// ポインタ
	m_pShadow = nullptr;	// 影のポインタ
}

//==========================================================================
// デストラクタ
//==========================================================================
CSample_Character::~CSample_Character()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSample_Character* CSample_Character::Create()
{
	// メモリ確保
	CSample_Character* pObj = DEBUG_NEW CSample_Character;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSample_Character::Init()
{
	// 種類の設定
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// 状態

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// 割り当て
	m_List.Regist(this);

	
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSample_Character::Uninit()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// 終了処理
	CObjectChara::Uninit();

	// 削除
	m_List.Delete(this);
}

//==========================================================================
// 終了処理
//==========================================================================
void CSample_Character::Kill()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// 終了処理
	CObjectChara::Uninit();

	// 削除
	m_List.Delete(this);
}

//==========================================================================
// 更新処理
//==========================================================================
void CSample_Character::Update()
{
	
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	if (pInputKeyboard->GetTrigger(DIK_F5))
	{// F5でリセット
		SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		SetMove(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	}

	// エディット中は抜ける
	if (CGame::GetInstance()->GetEditType() != CGame::GetInstance()->EDITTYPE_OFF)
	{
		return;
	}

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// 親の更新処理
	CObjectChara::Update();

	// 状態更新
	UpdateState();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// 影の位置更新
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CSample_Character::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	switch (nType)
	{
	case MOTION::MOTION_DEFAULT:
		// 攻撃時の処理をする
		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CSample_Character::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	if (pATKInfo->fRangeSize == 0.0f) return;

	if (pATKInfo->bEndAtk) return;

	// モーション取得
	CMotion* pMotion = GetMotion();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	CEffect3D* pEffect = nullptr;
}

//==========================================================================
// 状態更新
//==========================================================================
void CSample_Character::UpdateState()
{
	// 他の状態設定

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// なし
//==========================================================================
void CSample_Character::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==========================================================================
// 描画処理
//==========================================================================
void CSample_Character::Draw()
{

	// 描画処理
	// 状態に分けて使い分ける
	/*
	CObjectChara::Draw(m_mMatcol);

	CObjectChara::Draw(m_mMatcol.a);

	CObjectChara::Draw();
	*/
}

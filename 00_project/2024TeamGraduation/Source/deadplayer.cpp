//=============================================================================
// 
//  死亡プレイヤー処理 [deadplayer.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "deadplayer.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"
#include "limitarea.h"
#include "debugproc.h"


//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float MOVE_VELOCITY = 15.0f;	// 移動速度
	const int TURN_RIGHT = 100;	// 回れ右間隔
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\tyuuni\\setup_raou.txt";	// キャラクターファイル
}

namespace STATE_TIME
{
	const float UP = 5.0f;	// 上昇時間
	const float FADEOUT = 1.5f;	// 死亡時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CDeadPlayer::STATE_FUNC CDeadPlayer::m_StateFunc[] =
{
	&CDeadPlayer::StateNone,	// なし
	&CDeadPlayer::StateUP,		// 上昇
	&CDeadPlayer::StateFadeOut,	// フェードアウト
};

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CDeadPlayer> CDeadPlayer::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CDeadPlayer::CDeadPlayer(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_Oldstate = m_state;	// 前回の状態
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// マテリアルの色

	m_fStateTime = 0.0f;		// 状態遷移カウンター
	m_pShadow = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CDeadPlayer::~CDeadPlayer()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDeadPlayer* CDeadPlayer::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CDeadPlayer* pPeople = DEBUG_NEW CDeadPlayer;

	if (pPeople != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pPeople->SetPosition(pos);
		pPeople->CObject::SetOriginPosition(pos);

		// 初期化処理
		pPeople->Init();
	}

	return pPeople;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDeadPlayer::Init()
{

	// テキスト読み込み
	HRESULT hr = LoadText(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// 各種変数の初期化
	m_state = STATE::STATE_UP;	// 状態
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// 状態遷移カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_PLAYER);

	// リストに追加
	m_List.Regist(this);

	// 昇天設定
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION::MOTION_DEAD);
	}

	// 昇天エフェクト
	m_pEfkEffect = CEffekseerObj::Create(
		CMyEffekseer::EFKLABEL::EFKLABEL_PHOTON,
		GetPosition(), 0.0f, 0.0f, 100.0f, false);

	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
HRESULT CDeadPlayer::LoadText(const std::string& filename)
{
	// キャラ作成
	HRESULT hr = SetCharacter(filename);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDeadPlayer::Uninit()
{
	
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// エフェクト削除
	if (m_pEfkEffect != nullptr)
	{
		m_pEfkEffect->Uninit();
		m_pEfkEffect = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectChara::Uninit();
}

//==========================================================================
// 死亡処理
//==========================================================================
void CDeadPlayer::Kill()
{
	
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// エフェクト削除
	if (m_pEfkEffect != nullptr)
	{
		m_pEfkEffect->Uninit();
		m_pEfkEffect = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CDeadPlayer::Update()
{
	// 死亡の判定
	if (IsDeath() == true)
	{// 死亡フラグが立っていたら
		return;
	}

	// 過去の位置設定
	SetOldPosition(GetPosition());

	// 親の処理
	CObjectChara::Update();

	// 死亡の判定
	if (IsDeath())
	{// 死亡フラグが立っていたら
		return;
	}

	// 状態更新
	UpdateState();

	// 死亡の判定
	if (IsDeath())
	{// 死亡フラグが立っていたら
		return;
	}

	
	// 影の位置更新
	if (m_pShadow != nullptr)
	{
		// 位置取得
		MyLib::Vector3 pos = GetPosition();
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	if (GetPosition().y <= mylib_const::KILL_Y)
	{
		// 敵の終了処理
		Kill();
		Uninit();
		return;
	}

	// 大人の壁
	LimitArea();
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CDeadPlayer::UpdateState()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a);

	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// 何もない状態
//==========================================================================
void CDeadPlayer::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// 上昇
//==========================================================================
void CDeadPlayer::StateUP()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	if (m_fStateTime >= STATE_TIME::UP)
	{
		m_state = STATE::STATE_FADEOUT;
		m_fStateTime = 0.0f;
		return;
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CDeadPlayer::StateFadeOut()
{
	// 色設定
	m_mMatcol.a = (1.0f - m_fStateTime / STATE_TIME::FADEOUT) * 0.6f;

	if (m_fStateTime >= STATE_TIME::FADEOUT)
	{
		// 終了処理
		Kill();
		Uninit();
		return;
	}
}


//==========================================================================
// 大人の壁
//==========================================================================
void CDeadPlayer::LimitArea()
{
	return;

	// 自身の値を取得
	MyLib::Vector3 pos = GetPosition();

	// 大人の壁取得
	CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
	CLimitArea* pLimitArea = nullptr;

	while (limitareaList.ListLoop(&pLimitArea))
	{
		CLimitArea::sLimitEreaInfo info = pLimitArea->GetLimitEreaInfo();

		// 大人の壁を適用
		if (pos.x + GetRadius() >= info.fMaxX) { pos.x = info.fMaxX - GetRadius(); }
		if (pos.x - GetRadius() <= info.fMinX) { pos.x = info.fMinX + GetRadius(); }
		if (pos.z + GetRadius() >= info.fMaxZ) { pos.z = info.fMaxZ - GetRadius(); }
		if (pos.z - GetRadius() <= info.fMinZ) { pos.z = info.fMinZ + GetRadius(); }
	}

	// 値を適用
	SetPosition(pos);

}

//==========================================================================
// モーションの設定
//==========================================================================
void CDeadPlayer::SetMotion(int motionIdx)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx);
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CDeadPlayer::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	return;
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CDeadPlayer::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	//return;
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

	if (pATKInfo->bEndAtk)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(weponpos, MyLib::Vector3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pATKInfo->fRangeSize, 10, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	

}

//==========================================================================
// 描画処理
//==========================================================================
void CDeadPlayer::Draw()
{
	// オブジェクトキャラの描画
	CObjectChara::Draw(m_mMatcol);
}

//==========================================================================
// 状態設定
//==========================================================================
void CDeadPlayer::SetState(STATE state)
{
	m_state = state;
}

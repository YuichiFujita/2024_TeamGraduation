//=============================================================================
// 
//  依頼人処理 [request_people.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "request_people.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "camera.h"
#include "3D_Effect.h"
#include "shadow.h"
#include "sound.h"
#include "objectX.h"
#include "limitarea.h"
#include "debugproc.h"
#include "baggage.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\character\\mob\\person_01\\setup_human.txt";
}

namespace STATE_TIME
{
	const float FADEOUT = 0.6f;	// 死亡時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CRequestPeople::STATE_FUNC CRequestPeople::m_StateFunc[] =
{
	&CRequestPeople::StateNone,	// なし
	&CRequestPeople::StateFadeIn,	// フェードイン
	&CRequestPeople::StateFadeOut,	// フェードアウト
	&CRequestPeople::StateWait,		// 待機
	&CRequestPeople::StatePass,		// パス
	&CRequestPeople::StateByeBye,	// バイバイ
};


//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CRequestPeople> CRequestPeople::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CRequestPeople::CRequestPeople(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_Oldstate = m_state;	// 前回の状態
	m_fStateTime = 0.0f;		// 状態遷移カウンター
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	m_pShadow = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CRequestPeople::~CRequestPeople()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRequestPeople* CRequestPeople::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CRequestPeople* pPeople = DEBUG_NEW CRequestPeople;

	if (pPeople != nullptr)
	{
		// 位置設定
		pPeople->SetPosition(pos);
		pPeople->CObject::SetOriginPosition(pos);

		// テキスト読み込み
		HRESULT hr = pPeople->LoadText(FILENAME);
		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}

		// 初期化処理
		pPeople->Init();
	}

	return pPeople;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRequestPeople::Init()
{
	// 各種変数の初期化
	m_state = STATE::STATE_PASS;	// 状態
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// 状態遷移カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_ENEMY);

	// 向き設定
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// リストに追加
	m_List.Regist(this);

	// デフォルト設定
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION::MOTION_PASS);
	}

	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
HRESULT CRequestPeople::LoadText(const std::string& pFileName)
{
	// キャラ作成
	HRESULT hr = SetCharacter(pFileName);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRequestPeople::Uninit()
{
	
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectChara::Uninit();
}

//==========================================================================
// 死亡処理
//==========================================================================
void CRequestPeople::Kill()
{
	
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CRequestPeople::Update()
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

	// 当たり判定
	Collision();

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
}

//==========================================================================
// 当たり判定
//==========================================================================
void CRequestPeople::Collision()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// 重力処理
	move.y -= mylib_const::GRAVITY;

	// 位置更新
	pos += move;

	// 慣性補正
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	if (300.0f > pos.y)
	{// 地面の方が自分より高かったら

		// 地面の高さに補正
		pos.y = 300.0f;
		
		// ジャンプ使用可能にする
		move.y = 0.0f;
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CRequestPeople::UpdateState()
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
void CRequestPeople::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードイン
//==========================================================================
void CRequestPeople::StateFadeIn()
{
	// 色設定
	m_mMatcol.a = m_fStateTime / STATE_TIME::FADEOUT;

	if (m_fStateTime >= STATE_TIME::FADEOUT)
	{
		m_state = STATE::STATE_NONE;
		return;
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CRequestPeople::StateFadeOut()
{
	// 色設定
	m_mMatcol.a = 1.0f - m_fStateTime / STATE_TIME::FADEOUT;

	if (m_fStateTime >= STATE_TIME::FADEOUT)
	{
		// 終了処理
		Kill();
		Uninit();
		return;
	}
}

//==========================================================================
// 待機
//==========================================================================
void CRequestPeople::StateWait()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_BYEBYE)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_BYEBYE);
	}
}

//==========================================================================
// パス
//==========================================================================
void CRequestPeople::StatePass()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType != MOTION::MOTION_PASS)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_PASS);
	}

	nType = pMotion->GetType();
	if ((nType == MOTION::MOTION_PASS && pMotion->IsFinish()) ||
		nType != MOTION::MOTION_PASS)
	{// パス終了 or パス以外

		// モーション設定
		pMotion->Set(MOTION::MOTION_BYEBYE);
		m_state = STATE::STATE_BYEBYE;
	}
}

//==========================================================================
// バイバイ
//==========================================================================
void CRequestPeople::StateByeBye()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();

	if (nType != MOTION::MOTION_BYEBYE)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_BYEBYE);
	}

}

//==========================================================================
// モーションの設定
//==========================================================================
void CRequestPeople::SetMotion(int motionIdx)
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
void CRequestPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int motionType = pMotion->GetType();

	switch (motionType)
	{
	case MOTION::MOTION_PASS:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		if (nCntATK == 0)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_APPEARANCE);
			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_TORIDASU);
		}
		else if (nCntATK == 1)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_PASS);
			pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));

			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_NAGERU);
		}
	}
	break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CRequestPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int motionType = pMotion->GetType();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);


	switch (motionType)
	{
	case MOTION::MOTION_PASS:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		if (pBaggage->GetState() != CBaggage::STATE::STATE_PASS)
		{
			pBaggage->SetPosition(weponpos);
		}
	}
		break;

	default:
		break;
	}


	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

	if (pATKInfo->bEndAtk)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		pATKInfo->fRangeSize,
		10,
		CEffect3D::MOVEEFFECT_NONE,
		CEffect3D::TYPE_NORMAL);
#endif

	

}

//==========================================================================
// 描画処理
//==========================================================================
void CRequestPeople::Draw()
{
	if (m_state == STATE_FADEOUT || m_state == STATE::STATE_FADEIN)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else if (m_mMatcol != D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a))
	{
		// オブジェクトキャラの描画
		CObjectChara::Draw(m_mMatcol);
	}
	else
	{
		// オブジェクトキャラの描画
		CObjectChara::Draw();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CRequestPeople::SetState(STATE state)
{
	m_state = state;
}

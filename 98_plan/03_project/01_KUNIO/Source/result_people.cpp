//=============================================================================
// 
//  依頼人処理 [result_people.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "result_people.h"
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
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\character\\mob\\person_01\\setup_human.txt";
}

namespace STATE_TIME
{
	const float THROW = 0.5f;	// 投げ
	const float FADEOUT = 0.6f;	// 死亡時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CResultPeople::STATE_FUNC CResultPeople::m_StateFunc[] =
{
	&CResultPeople::StateNone,	// なし
	&CResultPeople::StateFadeIn,	// フェードイン
	&CResultPeople::StateFadeOut,	// フェードアウト
	&CResultPeople::StateWait,		// 待機
	&CResultPeople::StateResult,	// リザルト
	&CResultPeople::StateByeBye,	// バイバイ
};


//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CResultPeople> CResultPeople::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CResultPeople::CResultPeople(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_Oldstate = m_state;			// 前回の状態
	m_fStateTime = 0.0f;			// 状態遷移カウンター
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	m_pShadow = nullptr;
	m_pScroll = nullptr;		// 巻き物
	m_fScrollThrowTimer = 0.0f;	// 巻き物タイマー
}

//==========================================================================
// デストラクタ
//==========================================================================
CResultPeople::~CResultPeople()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CResultPeople* CResultPeople::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CResultPeople* pPeople = DEBUG_NEW CResultPeople;

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
HRESULT CResultPeople::Init()
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
		pMotion->ResetPose(MOTION::MOTION_RESULT);
		pMotion->Set(MOTION::MOTION_RESULT);
	}

	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
HRESULT CResultPeople::LoadText(const std::string& pFileName)
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
void CResultPeople::Uninit()
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
void CResultPeople::Kill()
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
void CResultPeople::Update()
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

	// 巻き物の更新処理
	UpdateScroll();

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
void CResultPeople::Collision()
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
// 巻き物の更新処理
//==========================================================================
void CResultPeople::UpdateScroll()
{
	if (m_pScroll == nullptr) return;

	// 目標の位置がまだ設定されてない
	if (m_posDestScroll.IsNearlyZero(0.1f)) return;

	// 投げのタイマー加算
	m_fScrollThrowTimer += CManager::GetInstance()->GetDeltaTime();

	if (m_fScrollThrowTimer < STATE_TIME::THROW + 0.2f)
	{
		// 線形補間
		MyLib::Vector3 pos, posOrigin = m_pScroll->GetOriginPosition();
		pos.x = UtilFunc::Correction::EaseInExpo(posOrigin.x, m_posDestScroll.x, 0.0f, 0.5f, m_fScrollThrowTimer);
		pos.y = UtilFunc::Correction::EaseInExpo(posOrigin.y, m_posDestScroll.y, 0.0f, 0.5f, m_fScrollThrowTimer);
		pos.z = UtilFunc::Correction::EaseInExpo(posOrigin.z, m_posDestScroll.z, 0.0f, 0.5f, m_fScrollThrowTimer);

		// 位置設定
		m_pScroll->SetPosition(pos);

		// 移動量設定
		m_pScroll->SetMove(MyLib::Vector3(0.0f, 4.0f, 0.0f));

	}
	else
	{
		// 線形補間
		MyLib::Vector3 pos = m_pScroll->GetPosition();
		MyLib::Vector3 move = m_pScroll->GetMove();
		MyLib::Vector3 rot = m_pScroll->GetRotation();

		// 落下
		pos.y += move.y;
		move.y -= 0.6f;

		pos.z -= 5.0f;

		// ランダム回転
		rot.y += D3DX_PI * UtilFunc::Transformation::Random(10, 40) * 0.001f;
		rot.x += D3DX_PI * UtilFunc::Transformation::Random(10, 40) * 0.001f;

		// 位置設定
		m_pScroll->SetPosition(pos);
		m_pScroll->SetMove(move);
		m_pScroll->SetRotation(rot);
	}
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CResultPeople::UpdateState()
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
void CResultPeople::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードイン
//==========================================================================
void CResultPeople::StateFadeIn()
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
void CResultPeople::StateFadeOut()
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
void CResultPeople::StateWait()
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
// リザルト
//==========================================================================
void CResultPeople::StateResult()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType != MOTION::MOTION_RESULT)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_RESULT);
	}

	nType = pMotion->GetType();
	if ((nType == MOTION::MOTION_RESULT && pMotion->IsFinish()) ||
		nType != MOTION::MOTION_RESULT)
	{// リザルト終了 or リザルト以外

		// モーション設定
		pMotion->Set(MOTION::MOTION_BYEBYE);
		m_state = STATE::STATE_BYEBYE;
	}
}

//==========================================================================
// バイバイ
//==========================================================================
void CResultPeople::StateByeBye()
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
void CResultPeople::SetMotion(int motionIdx)
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
void CResultPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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
	case MOTION::MOTION_RESULT:
	{
		if (nCntATK == 0)
		{
			// 巻き物生成
			CreateScroll();
		}
		else if (nCntATK == 1)
		{
			// 元の位置設定
			m_pScroll->SetOriginPosition(m_pScroll->GetPosition());

			// 目標の位置設定
			m_posDestScroll = CManager::GetInstance()->GetCamera()->GetPositionV() + MyLib::Vector3(0.0f, 0.0f, 100.0f);

			// 巻き物の投げタイマー
			m_fScrollThrowTimer = 0.0f;

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
void CResultPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
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
	case MOTION::MOTION_RESULT:
	{
		if (m_pScroll != nullptr)
		{
			// 巻き物位置設定
			m_pScroll->SetPosition(weponpos);
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
// 巻き物生成
//==========================================================================
void CResultPeople::CreateScroll()
{
	// 巻き物生成
	m_pScroll = CObjectX::Create("data\\MODEL\\scroll.x");
}

//==========================================================================
// 描画処理
//==========================================================================
void CResultPeople::Draw()
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
void CResultPeople::SetState(STATE state)
{
	m_state = state;
}

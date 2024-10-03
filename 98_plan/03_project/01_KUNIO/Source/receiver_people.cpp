//=============================================================================
// 
//  届け先処理 [receiver_people.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "receiver_people.h"
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
#include "receiver_people.h"
#include "catchresult.h"
#include "player.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\character\\mob\\person_01\\setup_human.txt";
	const float HEAD_POSITION = 300.0f;
	const float CATCH_RANGE = 500.0f;
	const float NEAR_RANGE = 1000.0f;
	const float WALK_TIMER = (1.0f / 120.0f);
	const MyLib::Vector3 START_ROT = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const float LOOKBACK_TIMER = (1.0f / 30.0f);
}

namespace STATE_TIME
{
	const float FADEOUT = 0.6f;	// 死亡時間
	const float DIVE = 1.0f;	// ダイブ時間
	const float CHASE = 0.5f;	// 追いかけ
	const float CATCH = 0.3f;	// キャッチ
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CReceiverPeople::STATE_FUNC CReceiverPeople::m_StateFunc[] =
{
	&CReceiverPeople::StateNone,	// なし
	&CReceiverPeople::StateFadeIn,	// フェードイン
	&CReceiverPeople::StateFadeOut,	// フェードアウト
	&CReceiverPeople::StateWait,	// 待機
	&CReceiverPeople::StateGet,		// キャッチ
	&CReceiverPeople::StateReturn,	// レシーブ
	&CReceiverPeople::StateYabai,	// やばい
	&CReceiverPeople::StateDive,	// ダイブ
	&CReceiverPeople::StateDrown,	// 溺れる
	&CReceiverPeople::StateByeBye,	// バイバイ
	&CReceiverPeople::StateWalk,	// 歩く
};


//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CReceiverPeople> CReceiverPeople::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CReceiverPeople::CReceiverPeople(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_Oldstate = m_state;	// 前回の状態
	m_fStateTime = 0.0f;		// 状態遷移カウンター
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_StartPos = 0.0f;
	m_fMoveTimer = 0.0f;
	m_pShadow = nullptr;
	m_bEnd = false;
}

//==========================================================================
// デストラクタ
//==========================================================================
CReceiverPeople::~CReceiverPeople()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CReceiverPeople* CReceiverPeople::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CReceiverPeople* pPeople = DEBUG_NEW CReceiverPeople;

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
HRESULT CReceiverPeople::Init()
{
	// 各種変数の初期化
	m_state = STATE::STATE_GET;	// 状態
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// 状態遷移カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_ENEMY);

	// 向き設定
	SetRotation(START_ROT);

	// リストに追加
	m_List.Regist(this);

	// デフォルト設定
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION::MOTION_DROWN);
	}

	// 桟橋
	MyLib::Vector3 setpos(GetPosition());

	int max = 5;
	float distance = 400.0f;
	setpos.z -= distance * (max / 2) + distance;
	for (int i = 0; i < max; i++)
	{
		setpos.z += distance;

		// 生成
		CObjectX* pObj = CObjectX::Create("data\\MODEL\\map_object\\sanbashi.x", setpos, 0.0f);
		pObj->SetType(CObject::TYPE::TYPE_OBJECTX);
		pObj->SetScale(3.0f);
	}
	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
HRESULT CReceiverPeople::LoadText(const std::string& pFileName)
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
void CReceiverPeople::Uninit()
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
void CReceiverPeople::Kill()
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
void CReceiverPeople::Update()
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
void CReceiverPeople::Collision()
{
	return;
	if (m_state == STATE::STATE_DIVE ||
		m_state == STATE::STATE_DROWN)
	{
		return;
	}

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
void CReceiverPeople::UpdateState()
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
void CReceiverPeople::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードイン
//==========================================================================
void CReceiverPeople::StateFadeIn()
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
void CReceiverPeople::StateFadeOut()
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
void CReceiverPeople::StateWait()
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

	// ゴール中のみ情報設定
	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
	{
		return;
	}

	// 荷物の現在位置を比較する
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

	// 距離を取得
	MyLib::Vector3 bagpos = pBaggage->GetPosition();
	MyLib::Vector3 mypos = GetPosition();
	float distance = bagpos.DistanceXZ(mypos);

	if (!(distance <= NEAR_RANGE) && GetPosition().x <= bagpos.x)
	{// 近い

		m_Distance = DISTANCE::DISTANCE_FAR;

		SetState(STATE::STATE_WALK);

		// 状態設定
		pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
		pBaggage->SetState(CBaggage::STATE::STATE_FALL);
		pBaggage->SetIsFall(true);
		pBaggage->SetMove(0.0f);
	}

	// 届け中のみ
	if (pBaggage->GetState() != CBaggage::STATE::STATE_SEND)
	{
		return;
	}


	// キャッチ判定を取る
	if (bagpos.y <= 0.0f)
	{// 既に落下している
		m_StartPos = GetPosition();
		m_StartRot = GetRotation();

		// 移動時間リセット
		m_fMoveTimer = 0.0f;

		// 距離を設定
		if (distance <= NEAR_RANGE)
		{// 近い

			if (bagpos.y <= 0.0f)
			{
				m_Distance = DISTANCE::DISTANCE_NEAR;
				SetState(STATE::STATE_YABAI);

				// 荷物の現在位置を比較する
				CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

				// 状態設定
				pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
				pBaggage->SetState(CBaggage::STATE::STATE_FALL);
			}
		}
		else
		{// 遠い

			m_Distance = DISTANCE::DISTANCE_FAR;

			if (m_StartPos.x >= bagpos.x)
			{// 荷物が届いていない
				SetState(STATE::STATE_YABAI);

				// ドロップ設定
				pBaggage->SetState(CBaggage::STATE::STATE_FALL);
			}
			else
			{// 通り過ぎた
				SetState(STATE::STATE_WALK);

				// 状態設定
				pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
				pBaggage->SetState(CBaggage::STATE::STATE_FALL);
				pBaggage->SetIsFall(true);
				pBaggage->SetMove(0.0f);
			}
		}
	}
	else if (bagpos.y <= mypos.y + HEAD_POSITION && distance <= CATCH_RANGE)
	{// キャッチできる
		m_StartPos = GetPosition();
		SetState(STATE::STATE_GET);
	}
}

//==========================================================================
// パス
//==========================================================================
void CReceiverPeople::StatePass()
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
// キャッチ
//==========================================================================
void CReceiverPeople::StateGet()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();

	if (nType != MOTION::MOTION_GET)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_GET);

		// 注視点を自分に
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));
		pCamera->GetCameraMotion()->SetEnablePause(true);
		pCamera->SetPositionR(GetPosition());
		pCamera->SetDistance(1200.0f);

		// 向きも変える
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * -0.25f;
		pCamera->SetRotation(setrot);


		// 潰れるエフェクト
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_SMASH,
			GetPosition(), MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
	}
	nType = pMotion->GetType();

	// 荷物情報取得
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
	pBaggage->SetMove(0.0f);

	if ((nType == MOTION::MOTION_GET && pMotion->IsFinish()) ||
		nType != MOTION::MOTION_GET)
	{// パス終了 or パス以外

		// モーション設定
		if (!m_bEnd)
		{
			CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_OK);
			m_bEnd = true;

			// 荷物情報
			pBaggage->SetState(CBaggage::STATE::STATE_FALL);
			pBaggage->SetIsFall(true);
		}
	}


}

//==========================================================================
// レシーブ
//==========================================================================
void CReceiverPeople::StateReturn()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_RETURN)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_RETURN);

	}

	// 攻撃情報取得
	CMotion::Info aInfo = pMotion->GetInfo(pMotion->GetType());
	CMotion::AttackInfo* AttackInfo = aInfo.AttackInfo[0];
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *AttackInfo);

	int nType = pMotion->GetType();
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();
	if (nType == MOTION::MOTION_RETURN && (int)pMotion->GetAllCount() >= AttackInfo->nMinCnt && (int)pMotion->GetAllCount() <= AttackInfo->nMaxCnt)
	{// リターンの攻撃判定中

		// 荷物の現在位置を比較する
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		MyLib::Vector3 bagpos = pBaggage->GetPosition(), bagOriginPos = pBaggage->GetOriginPosition();

		// 攻撃情報取得
		CMotion::Info aInfo = pMotion->GetInfo(pMotion->GetType());
		CMotion::AttackInfo* AttackInfo = aInfo.AttackInfo[0];
		MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *AttackInfo);

		// 手の位置まで線形補間
		bagpos = UtilFunc::Correction::EasingLinear(m_StartPos, weponpos, 0.0f, STATE_TIME::CATCH, m_fMoveTimer);
		pBaggage->SetPosition(bagpos);

		// レシーブエフェクト
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_HIT,
			weponpos, MyLib::Vector3(0.0f,D3DX_PI * 0.5f, 0.0f), 0.0f, 20.0f, true);
	}

	// 終了確認
	if (((pMotion->GetType() == MOTION::MOTION_RETURN && pMotion->IsFinish()) ||
		pMotion->GetType() != MOTION::MOTION_RETURN) && !m_bEnd)
	{// パス終了 or パス以外
		m_bEnd = true;
		CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_FAIL);
	}

	//// 向きを調整
	//{
	//	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();
	//	MyLib::Vector3 rot = UtilFunc::Correction::EasingEaseIn(m_StartRot, START_ROT, 0.0f, 1.0f, m_fMoveTimer);
	//	SetRotation(rot);
	//}
}

//==========================================================================
// やばい
//==========================================================================
void CReceiverPeople::StateYabai()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int type = pMotion->GetType();
	if (type != MOTION::MOTION_YABAI)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_YABAI);

		// 注視点を自分に
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));
		pCamera->GetCameraMotion()->SetEnablePause(true);
		pCamera->SetPositionR(GetPosition());

		// 向きも変える
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * -0.05f;
		pCamera->SetRotation(setrot);
	}

	// ダイブ設定
	if (type == MOTION::MOTION_YABAI &&
		pMotion->IsFinish())
	{
		m_state = STATE::STATE_DIVE;
		m_fStateTime = 0.0f;
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DIVE);


		// 荷物の現在位置を比較する
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		// 注視点を荷物に
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(pBaggage->GetPosition());
		pCamera->SetPositionR(pBaggage->GetPosition() + MyLib::Vector3(0.0f, 50.0f, 0.0f));

		// 向きも変える
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * -0.1f;
		pCamera->SetRotation(setrot);
	}
}

//==========================================================================
// ダイブ
//==========================================================================
void CReceiverPeople::StateDive()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_DIVE)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_DIVE);

	}

	// 移動時間加算
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();

	// 荷物取得
	CBaggage* pBag = CBaggage::GetListObj().GetData(0);
	MyLib::Vector3 bagpos = pBag->GetPosition();
	bagpos.y = 0.0f;

	// 位置設定
	MyLib::Vector3 pos = UtilFunc::Correction::EasingLinear(m_StartPos, bagpos, 0.0f, STATE_TIME::DIVE, m_fMoveTimer);
	SetPosition(pos);

	// 向きを調整
	MyLib::Vector3 rot = GetRotation();
	rot.y = m_StartPos.AngleXZ(bagpos);
	SetRotation(rot);


	// 状態遷移
	if (m_fMoveTimer >= STATE_TIME::DIVE)
	{
		// 状態遷移
		SetState(STATE::STATE_DROWN);
		m_fMoveTimer = 0.0f;

		// 向きを調整
		SetRotation(START_ROT);

		// 水エフェクト
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_SPRAYWATER_MINI,
			pos, MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
	}

}

//==========================================================================
// 溺れる
//==========================================================================
void CReceiverPeople::StateDrown()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetType() != MOTION::MOTION_DROWN)
	{
		// モーション設定
		pMotion->Set(MOTION::MOTION_DROWN);
	}


	// 移動時間加算
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();

	if (m_fMoveTimer >= 2.0f &&
		(
		((pMotion->GetType() == MOTION::MOTION_DROWN && pMotion->IsFinish()) ||
		pMotion->GetType() != MOTION::MOTION_DROWN) && !m_bEnd
			))
	{// パス終了 or パス以外
		m_bEnd = true;
		CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_FAIL);
	}

	// 荷物の位置にそろえる
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
	MyLib::Vector3 bagpos = pBaggage->GetPosition();
	bagpos.y = 0.0f;
	SetPosition(bagpos);
}

//==========================================================================
// バイバイ
//==========================================================================
void CReceiverPeople::StateByeBye()
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

	if (((pMotion->GetType() == MOTION::MOTION_BYEBYE && pMotion->IsFinish()) ||
		pMotion->GetType() != MOTION::MOTION_BYEBYE) && !m_bEnd)
	{// パス終了 or パス以外
		m_bEnd = true;
		CCatchResult::Create(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CCatchResult::TYPE::TYPE_OK);
	}

	// 向きを調整
	{
		m_fMoveTimer += LOOKBACK_TIMER;
		MyLib::Vector3 rot = UtilFunc::Correction::EasingEaseIn(m_StartRot, START_ROT, 0.0f, 1.0f, m_fMoveTimer);
		SetRotation(rot);
	}
}

//==========================================================================
// 歩行
//==========================================================================
void CReceiverPeople::StateWalk()
{
	// 移動時間加算
	m_fMoveTimer += CManager::GetInstance()->GetDeltaTime();

	// 荷物取得
	CBaggage* pbag = CBaggage::GetListObj().GetData(0);
	MyLib::Vector3 bagpos = pbag->GetPosition();
	MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseIn(m_StartPos, bagpos, 0.0f, STATE_TIME::CHASE, m_fMoveTimer);
	SetPosition(pos);

	if (m_Distance == DISTANCE::DISTANCE_FAR && m_fMoveTimer >= STATE_TIME::CHASE)
	{
		SetState(STATE::STATE_RETURN);
		m_fMoveTimer = 0.0f;
		m_StartPos = bagpos;

		// 注視点を自分に
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->GetCameraMotion()->SetPosition(CPlayer::GetListObj().GetData(0)->GetPosition());
		pCamera->GetCameraMotion()->SetEnablePause(true);
		pCamera->SetPositionR(GetPosition());
		pCamera->SetDistance(2500.0f);

		// 向きも変える
		MyLib::Vector3 setrot = pCamera->GetRotation();
		setrot.z = D3DX_PI * 0.05f;
		pCamera->SetRotation(setrot);

		// 瞬間移動エフェクト生成
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_TP,
			pos, MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f), 0.0f, 30.0f, true);

		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_TELEPORTATION);
		return;
	}

	// 向きを調整
	if(m_Distance == DISTANCE::DISTANCE_NEAR)
	{
		MyLib::Vector3 targetrot = 0.0f;
		float movetimer = UtilFunc::Transformation::Clamp(m_fMoveTimer, 0.0f, 0.5f);
		targetrot.y = m_StartPos.AngleXZ(bagpos);
		
		MyLib::Vector3 rot = UtilFunc::Correction::EasingEaseIn(m_StartRot, targetrot, 0.0f, 0.5f, movetimer);
		SetRotation(rot);

		// モーション設定
		SetMotion(MOTION::MOTION_WALK);
	}
	else
	{
		SetRotation(MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f));

		// モーション設定
		SetMotion(MOTION::MOTION_RETURN);
	}

	// 終了確認
	if (m_fMoveTimer < 1.0f) { return; }

	m_fMoveTimer = 0.0f;
	m_StartRot = GetRotation();

	// 状態を距離によって変更
	switch (m_Distance)
	{
	case DISTANCE::DISTANCE_NEAR:
	{
		SetState(STATE::STATE_YABAI);

		// 荷物の現在位置を比較する
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		// 状態設定
		pBaggage->SetState(CBaggage::STATE::STATE_RECEIVE);
		pBaggage->SetState(CBaggage::STATE::STATE_FALL);
		pBaggage->SetIsFall(true);
		pBaggage->SetMove(0.0f);
	}
		break;

	case DISTANCE::DISTANCE_FAR:
	{
		if (m_StartPos.x >= bagpos.x)
		{// 荷物が届いていない
			SetState(STATE::STATE_DROWN);
		}
		else 
		{// 通り過ぎた
			SetState(STATE::STATE_RETURN);
		}
	}
		break;

	default:
	{
		SetState(STATE::STATE_BYEBYE);
	}
		break;
	}
}

//==========================================================================
// モーションの設定
//==========================================================================
void CReceiverPeople::SetMotion(int motionIdx)
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
void CReceiverPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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
		}
		else if (nCntATK == 1)
		{
			pBaggage->SetState(CBaggage::STATE::STATE_PASS);
			pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));
		}
	}
	break;

	case MOTION::MOTION_GET:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));
	}
	break;

	case MOTION::MOTION_RETURN:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
		pBaggage->SetAwayStartPosition(pMotion->GetAttackPosition(GetModel(), ATKInfo));
		pBaggage->SetForce(0.0f);
		pBaggage->SetState(CBaggage::STATE::STATE_RETURN);
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetPositionRDest(pCamera->GetPositionR());
		pCamera->SetPositionVDest(pCamera->GetPositionV());
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_SMASHATTACK);
	}
	break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CReceiverPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// ゴール中のみ情報設定
	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
	{
		return;
	}

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
	case MOTION::MOTION_BYEBYE:
	{
		// ばいばい状態だけ
		if (m_state != STATE::STATE_BYEBYE) {
			return;
		}

		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		if (pBaggage->GetState() != CBaggage::STATE::STATE_PASS)
		{
			pBaggage->SetPosition(weponpos);
		}
	}
	break;

	case MOTION::MOTION_GET:
	{
		CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);

		if (pBaggage->GetState() != CBaggage::STATE::STATE_PASS)
		{
			pBaggage->SetPosition(weponpos);
		}
	}
	break;

	case MOTION::MOTION_RETURN:
	{
		
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
void CReceiverPeople::Draw()
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
void CReceiverPeople::SetState(STATE state)
{
	m_state = state;
}

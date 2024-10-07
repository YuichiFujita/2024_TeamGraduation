//=============================================================================
// 
//  プレイヤー処理 [people.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "people.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "camera.h"
#include "3D_Effect.h"
#include "shadow.h"
#include "sound.h"
#include "stage.h"
#include "objectX.h"
#include "limitarea.h"
#include "debugproc.h"
#include "motion.h"

// 派生クラス
#include "kite.h"


//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float MOVE_VELOCITY = 15.0f;	// 移動速度
	const int TURN_RIGHT = 100;	// 回れ右間隔
}

namespace STATE_TIME
{
	const float FADEOUT = 0.6f;	// 死亡時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CPeople::STATE_FUNC CPeople::m_StateFunc[] =
{
	&CPeople::StateNone,	// なし
	&CPeople::StateFadeIn,	// フェードイン
	&CPeople::StateFadeOut,	// フェードアウト
};

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CPeople> CPeople::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CPeople::CPeople(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_Oldstate = m_state;	// 前回の状態
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// マテリアルの色
	m_TargetPosition = mylib_const::DEFAULT_VECTOR3;	// 目標の位置

	
	m_fStateTime = 0.0f;		// 状態遷移カウンター
	m_sMotionFrag = SMotionFrag();		// モーションのフラグ
	m_pShadow = nullptr;
	m_flame = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CPeople::~CPeople()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPeople* CPeople::Create(const std::string& filename, const MyLib::Vector3& pos, const TYPE& type)
{
	// メモリの確保
	CPeople* pPeople = nullptr;

	switch (type)
	{
	case TYPE::TYPE_KITE:
	case TYPE::TYPE_KITE2:
	case TYPE::TYPE_KITE3:
		pPeople = DEBUG_NEW CKite;
		break;

	default:
		pPeople = DEBUG_NEW CPeople;
		break;
	}

	if (pPeople != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pPeople->SetPosition(pos);
		pPeople->CObject::SetOriginPosition(pos);

		// テキスト読み込み
		HRESULT hr = pPeople->LoadText(filename.c_str());
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
HRESULT CPeople::Init()
{
	// 各種変数の初期化
	m_state = STATE::STATE_FADEIN;	// 状態
	m_Oldstate = m_state;
	m_fStateTime = 0.0f;			// 状態遷移カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_ENEMY);

	// リストに追加
	m_List.Regist(this);

	// デフォルト設定
	CMotion* pMotion = GetMotion();
	int motionmove = UtilFunc::Transformation::Random(0, pMotion->GetNumMotion() - 1);

	if (pMotion != nullptr)
	{
		pMotion->Set(motionmove);
	}

	float rotY = D3DX_PI * 0.5f;
	if (pMotion->IsGetMove(pMotion->GetType()) == 1)
	{// 移動モーション

		// 向き設定
		if (rand() % 2 == 0)
		{
			rotY += D3DX_PI;
		}
	}
	else
	{
		rotY = UtilFunc::Transformation::Random(-31, 31) * 0.1f;
	}
	UtilFunc::Transformation::RotNormalize(rotY);
	SetRotation(MyLib::Vector3(0.0f, rotY, 0.0f));
	SetRotDest(rotY);

	// 移動速度
	m_fMoveVelocity = MOVE_VELOCITY + UtilFunc::Transformation::Random(-20, 50) * 0.1f;

	// 移動量
	MyLib::Vector3 move, rot = GetRotation();
	move.x = sinf(D3DX_PI + rot.y) * m_fMoveVelocity;
	move.z = cosf(D3DX_PI + rot.y) * m_fMoveVelocity;
	SetMove(move);

	// 初期フレーム
	m_flame = UtilFunc::Transformation::Random(0, TURN_RIGHT / 2);

	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
HRESULT CPeople::LoadText(const char *pFileName)
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
void CPeople::Uninit()
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
void CPeople::Kill()
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
void CPeople::Update()
{
	// 死亡の判定
	if (IsDeath() == true)
	{// 死亡フラグが立っていたら
		return;
	}

	CMotion* pMotion = GetMotion();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	if (pMotion->IsGetMove(pMotion->GetType()) == 1)
	{// 歩きモーションの時

		// 向き補正
		float rotDiff = GetRotDest() - rot.y;
		UtilFunc::Transformation::RotNormalize(rotDiff);

		rot.y += rotDiff * 0.2f;
		UtilFunc::Transformation::RotNormalize(rot.y);

		move.x += sinf(D3DX_PI + rot.y) * (m_fMoveVelocity * deltaTime);
		move.z += cosf(D3DX_PI + rot.y) * (m_fMoveVelocity * deltaTime);

		pos += move;
		m_flame++;

		if (m_flame >= TURN_RIGHT)
		{// フレーム数超過

			if (m_flame == TURN_RIGHT)
			{// フレーム数超過
				
				// 右回転
				float dest = rot.y + D3DX_PI * 0.5f;
				UtilFunc::Transformation::RotNormalize(dest);
				SetRotDest(dest);
			}

			if (m_flame >= TURN_RIGHT + 20)
			{
				// 右回転
				float dest = rot.y + D3DX_PI * 0.5f;
				UtilFunc::Transformation::RotNormalize(dest);
				SetRotDest(dest);

				// フレームリセット
				m_flame = UtilFunc::Transformation::Random(0, TURN_RIGHT / 2);
			}
		}
	}

	SetMove(move);
	SetPosition(pos);
	SetRotation(rot);

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

	// 大人の壁
	LimitArea();
}

//==========================================================================
// 当たり判定
//==========================================================================
void CPeople::Collision()
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

	if (move.x >= 0.1f || move.x <= -0.1f ||
		move.z >= 0.1f || move.z <= -0.1f)
	{// 移動中
		m_sMotionFrag.bMove = true;
	}
	else
	{
		m_sMotionFrag.bMove = false;
	}

	if (300.0f > pos.y)
	{// 地面の方が自分より高かったら

		// 地面の高さに補正
		pos.y = 300.0f;
		
		// ジャンプ使用可能にする
		move.y = 0.0f;
		m_sMotionFrag.bJump = false;
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// 着地時の処理
//==========================================================================
void CPeople::ProcessLanding()
{
	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// ジャンプ使用可能にする
	move.y = 0.0f;
	m_sMotionFrag.bJump = false;

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CPeople::UpdateState()
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
void CPeople::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードイン
//==========================================================================
void CPeople::StateFadeIn()
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
void CPeople::StateFadeOut()
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
// 大人の壁
//==========================================================================
void CPeople::LimitArea()
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
void CPeople::SetMotion(int motionIdx)
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
void CPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	return;
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
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
void CPeople::Draw()
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
void CPeople::SetState(STATE state)
{
	m_state = state;
}

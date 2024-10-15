//=============================================================================
// 
//  プレイヤー処理 [player.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "player.h"
#include "game.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "calculation.h"
#include "Xload.h"
#include "model.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"
#include "sound.h"
#include "fade.h"
#include "listmanager.h"
#include "MyEffekseer.h"
#include "map.h"
#include "edit_map.h"

// 使用クラス
#include "playercontrol.h"
#include "playercontrol_action.h"
#include "playerAction.h"
#include "playerStatus.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\sample\\setup_player.txt";	// キャラクターファイル
	const float JUMP = 20.0f * 1.5f;			// ジャンプ力初期値
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayer::STATE_FUNC CPlayer::m_StateFunc[] =	// 状態関数
{
	&CPlayer::StateNone,		// なし
	&CPlayer::StateInvincible,	// 無敵
	&CPlayer::StateDamage,		// ダメージ
	&CPlayer::StateDead,		// 死亡
	&CPlayer::StateDeadWait,	// 死亡待機
};


//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CPlayer> CPlayer::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayer::CPlayer(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;			// 状態
	m_Oldstate = STATE_NONE;		// 前回の状態
	m_fStateTime = 0.0f;			// 状態時間

	// オブジェクトのパラメータ
	m_mMatcol = MyLib::Color();			// マテリアルの色
	m_posKnokBack = MyLib::Vector3();	// ノックバックの位置

	// 行動フラグ
	m_bJump = false;				// ジャンプ中かどうか
	m_bDash = false;				// ダッシュ判定
	m_sMotionFrag = SMotionFrag();	// モーションのフラグ

	// パターン用インスタンス
	m_pControlMove = nullptr;	// 移動操作
	m_pControlAction = nullptr;	// アクション操作
	m_pActionPattern = nullptr;	// アクションパターン
	m_pStatus = nullptr;		// ステータス

	// その他
	m_nMyPlayerIdx = 0;				// プレイヤーインデックス番号
	m_pShadow = nullptr;			// 影の情報
	m_sDamageInfo = sDamageInfo();	// ダメージ情報
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayer::~CPlayer()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPlayer* CPlayer::Create(int nIdx)
{
	// メモリ確保
	CPlayer* pPlayer = DEBUG_NEW CPlayer;

	if (pPlayer != nullptr)
	{
		// プレイヤーインデックス番号
		pPlayer->m_nMyPlayerIdx = nIdx;

		// 初期化処理
		pPlayer->Init();
	}

	return pPlayer;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayer::Init()
{
	// 種類の設定
	SetType(TYPE_PLAYER);

	// ダメージ受け付け判定
	m_sDamageInfo.bReceived = true;
	m_sDamageInfo.reciveTime = 0.0f;

	m_state = STATE_NONE;	// 状態
	m_Oldstate = m_state;
	m_sMotionFrag.bMove = true;

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// 割り当て
	m_List.Regist(this);

	// 操作関連
	ChangeMoveControl(DEBUG_NEW CPlayerControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerControlAction());

	// アクションパターン
	if (m_pActionPattern == nullptr)
	{
		m_pActionPattern = DEBUG_NEW CPlayerAction(this);
	}

	// ステータス
	if (m_pStatus == nullptr)
	{
		m_pStatus = DEBUG_NEW CPlayerStatus(this);
	}

	return S_OK;
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayer::ChangeMoveControl(CPlayerControlMove* control)
{
	delete m_pControlMove;
	m_pControlMove = control;
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayer::ChangeActionControl(CPlayerControlAction* control)
{
	delete m_pControlAction;
	m_pControlAction = control;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Uninit()
{
	// 影
	m_pShadow = nullptr;

	// 操作系
	DeleteControl();

	// 終了処理
	CObjectChara::Uninit();

	// 削除
	m_List.Delete(this);
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Release()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// 操作系
	DeleteControl();

	// アクションパターン
	if (m_pActionPattern != nullptr)
	{
		delete m_pActionPattern;
		m_pActionPattern = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsDeath()) return;

	// エディット中は抜ける
	if (CGame::GetInstance()->GetEditType() != CGame::GetInstance()->EDITTYPE_OFF)
	{
		return;
	}

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// フラグリセット
	ResetFrag();

	// 親の更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 操作
	Controll(fDeltaTime, fDeltaRate, fSlowRate);

	// モーションの設定処理
	if (CGame::GetInstance()->GetGameManager()->IsControll())
	{
		MotionSet();
	}

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// アクション更新
	if (m_pActionPattern != nullptr)
	{
		m_pActionPattern->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetTargetPosition(pos);
	}

	// 影の位置更新
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	// 位置の制限
	LimitPos();

#if 1

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[プレイヤーの操作]------------------\n"
		"位置：【X：%f, Y：%f, Z：%f】 【W / A / S / D】\n"
		"向き：【X：%f, Y：%f, Z：%f】 【Z / C】\n"
		"移動量：【X：%f, Y：%f, Z：%f】\n"
		"体力：【%d】\n"
		"状態：【%d】\n"
		, pos.x, pos.y, pos.z, rot.x, rot.y, rot.y, move.x, move.y, move.z, GetLife(), m_state);

#endif

}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayer::Controll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// キーボード情報取得
	CInputKeyboard *pKey = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pPad = CInputGamepad::GetInstance();

	if (CGame::GetInstance()->GetGameManager()->IsControll())
	{// 行動できるとき

		// 移動操作
		m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
		m_pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();

	// 移動量加算
	pos += move * fDeltaRate;

	// 慣性補正
	move.x += (0.0f - move.x) * (0.1f * fDeltaRate);
	move.z += (0.0f - move.z) * (0.1f * fDeltaRate);

	// 重力処理
	if (m_state != STATE_DEAD && m_state != STATE_DEADWAIT)
	{
		move.y -= mylib_const::GRAVITY * fDeltaRate;
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);


	// 現在と目標の差分を求める
	float fRotDiff = GetRotDest() - rot.y;
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	// 角度の補正をする
	rot.y += fRotDiff * (0.25f * fDeltaRate);
	UtilFunc::Transformation::RotNormalize(rot.y);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// 操作関連削除
//==========================================================================
void CPlayer::DeleteControl()
{
	if (m_pControlMove != nullptr) 
	{// 移動操作
		delete m_pControlMove;
		m_pControlMove = nullptr;
	}

	if (m_pControlAction != nullptr)
	{// アクション操作
		delete m_pControlAction;
		m_pControlAction = nullptr;
	}
	
	if (m_pActionPattern != nullptr)
	{// アクションパターン
		delete m_pActionPattern;
		m_pActionPattern = nullptr;
	}
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::SetMotion(int motionIdx)
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
// モーションの設定
//==========================================================================
void CPlayer::MotionSet()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// 移動できないと通さない
	if (!m_bPossibleMove) return;

	// 再生中
	if (!pMotion->IsFinish()) return;

	// 現在の種類取得
	int nType = pMotion->GetType();
	int nOldType = pMotion->GetOldType();

	if (m_sMotionFrag.bMove)
	{// 移動していたら

		m_sMotionFrag.bMove = false;	// 移動判定OFF

		// 移動モーション
		if (m_bDash)
		{// ダッシュモーション

		}
		else
		{// 歩行モーション
			pMotion->Set(MOTION_WALK);
		}
	}
	else if (m_sMotionFrag.bJump)
	{// ジャンプ中

		// ジャンプのフラグOFF
		m_sMotionFrag.bJump = false;

		// ジャンプモーション
	}
	else if (m_bJump && !m_sMotionFrag.bJump)
	{// ジャンプ中&&ジャンプモーション再生が終わってる時

		// 落下モーション
	}
	else
	{
		// ニュートラルモーション
		pMotion->Set(MOTION_DEF);
	}
}

//==========================================================================
// フラグリセット
//==========================================================================
void CPlayer::ResetFrag()
{

}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CPlayer::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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
	case MOTION::MOTION_WALK:
		break;

	case MOTION::MOTION_START:

		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CPlayer::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		pATKInfo->fRangeSize, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	if (pATKInfo->bEndAtk)
	{
		return;
	}

	
}

//==========================================================================
// 位置制限
//==========================================================================
void CPlayer::LimitPos()
{
	MyLib::Vector3 pos = GetPosition();
	if (pos.y <= 0.0f)
	{
		pos.y = 0.0f;

		// 重力リセット
		m_bJump = false;
		MyLib::Vector3 move = GetMove();
		move.y = 0.0f;
		SetMove(move);
	}
	SetPosition(pos);
	return;
}

//==========================================================================
// ヒット処理
//==========================================================================
#if 0
MyLib::HitResult_Character CPlayer::Hit(const int nValue)
{
	MyLib::HitResult_Character hitresult = {};

	if (m_state == STATE::STATE_DEAD ||
		m_state == STATE::STATE_DEADWAIT)
	{
		hitresult.isdeath = true;
		return hitresult;
	}

	// 体力取得
	int nLife = GetLife();
	nLife -= nValue;
	nLife = UtilFunc::Transformation::Clamp(nLife, 0, GetLifeOrigin());

	// 体力設定
	SetLife(nLife);

	// 振動
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (nLife <= 0)
	{// 体力がなくなったら

		// 死亡時の設定
		DeadSetting(&hitresult);
	}

	// 当たった判定を返す
	return hitresult;
}
#else
bool Hit(const CBall* pBall)
{
	return false;
}
#endif

//==========================================================================
// 死亡時の設定
//==========================================================================
void CPlayer::DeadSetting(MyLib::HitResult_Character* result)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 死状態
	m_state = STATE_DEAD;

	// ノックバックの位置更新
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	m_posKnokBack = pos;

	// 死んだ
	result->isdeath = true;
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayer::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ダメージ受付時間更新
	UpdateDamageReciveTimer(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ダメージ受付時間更新
//==========================================================================
void CPlayer::UpdateDamageReciveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ダメージ受け付け時間減算
	m_sDamageInfo.reciveTime -= fDeltaTime;
	if (m_sDamageInfo.reciveTime <= 0.0f)
	{
		// スーパーアーマーがない時はダメージモーション終了
		if (!m_sDamageInfo.bActiveSuperArmor &&
			!m_sDamageInfo.bReceived)
		{
			// なにもない状態にする
			m_state = STATE_NONE;

			// モーション取得
			CMotion* pMotion = GetMotion();
			if (pMotion == nullptr)
			{
				return;
			}
			pMotion->ToggleFinish(true);
		}

		if (!m_sDamageInfo.bReceived)
		{// 受け付け無い時

		}

		// ダメージ受け付け判定
		m_sDamageInfo.bReceived = true;
		m_sDamageInfo.reciveTime = 0.0f;
	}
}

//==========================================================================
// なし
//==========================================================================
void CPlayer::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==========================================================================
// 無敵状態
//==========================================================================
void CPlayer::StateInvincible()
{

}

//==========================================================================
// ダメージ状態
//==========================================================================
void CPlayer::StateDamage()
{
	
}

//==========================================================================
// 死亡状態
//==========================================================================
void CPlayer::StateDead()
{
	
}

//==========================================================================
// 死亡待機
//==========================================================================
void CPlayer::StateDeadWait()
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// バッファへの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	// テスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z+ステンシル成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z+ステンシル失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテストのみ失敗

	// ステンシル描画
	CObjectChara::Draw();

	// ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// 普通の描画
	if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else
	{
		CObjectChara::Draw();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CPlayer::SetState(STATE state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

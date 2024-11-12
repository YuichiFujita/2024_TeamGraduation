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
#include "gamemanager.h"
#include "MyEffekseer.h"
#include "map.h"
#include "edit_map.h"
#include "charmManager.h"
#include "teamStatus.h"

// 派生先
#include "playerAI.h"
#include "playerUser.h"

// デバッグ
#include "ObjectLine.h"

// 使用クラス
#include "playerAction.h"
#include "playerStatus.h"
#include "playercontrol_action.h"
#include "playercontrol_move.h"
#include "dressup_hair.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE[] =		// キャラクターファイル
	{
		"data\\TEXT\\character\\player\\righthand\\setup_player.txt",
		"data\\TEXT\\character\\player\\lefthand\\setup_player.txt",
	};

	const float DODGE_RADIUS = 300.0f;			// 回避範囲
	const float JUST_VIEW = 90.0f;	//ジャストキャッチ時の方向ゆとり(左右1/8π)
}

namespace Knockback
{
	const float HEIGHT = 50.0f;					// 最大高度
	const float HEIGHT_OUTCOURT = 50.0f;		// 最大高度(コート越え)
	const float DAMAGE = 50.0f;		// ダメージ
	const float DEAD = 100.0f;		// 死亡
}

// TODO：ボールステータスに移行
namespace Catch
{
	const float Impact[CBall::EAttack::ATK_MAX] =	// 衝撃
	{
		0.0f,	// なし
		0.5f,	// 通常
		1.0f,	// ジャンプ
		5.0,	// スペシャル
	};

	const float ANGLE = 160;	// キャッチ判定角度(度数法)
}

namespace StateTime
{
	const float DAMAGE = 0.5f;		// ダメージ
	const float DEAD = 2.0f;		// 死亡
	const float INVINCIBLE = 0.8f;	// 無敵
	const float CATCH = 0.5f;		// キャッチ
	const float COURT_RETURN = 2.0f;		// コートに戻ってくる
	const float INVADE_TOSS = 0.3f;		// 侵入後トス
}

namespace Align	// 足揃え
{
	const float MULTIPLY_MOVE = 0.25f;	// 移動倍率
	const float MULTIPLY_MOTION = 1.5f;	// モーション倍率
}

namespace Court	// 移動制限
{
	const float COMEBACK_LINE = 100.0f;	// 戻ってくるライン
	const float VELOCITY_INVADE = 2.0f;	// 戻る速度
}

namespace Crab	// カニ歩き
{
	const float RANGE = D3DX_PI * 0.25f;	// 判定角度の半分

	const float RANGE_MIN_MAX[] =	// 判定角度列挙
	{
		D3DX_PI * 1.0f + RANGE,		// 下Max
		D3DX_PI * 1.0f - RANGE,		// 下Min
		D3DX_PI * 0.0f + RANGE,		// 上Max
		D3DX_PI * 0.0f - RANGE,		// 上Min
		D3DX_PI * -0.5f + RANGE,	// 左Max
		D3DX_PI * -0.5f - RANGE,	// 左Min
		D3DX_PI * 0.5f + RANGE,		// 右Max
		D3DX_PI * 0.5f - RANGE,		// 右Min
	};

	// カニ歩きモーション設定列挙
	const CPlayer::EMotion MOTION_WALK[CPlayer::CRAB_DIRECTION::CRAB_MAX][CPlayer::CRAB_DIRECTION::CRAB_MAX] =
	{
		// [ プレイヤー向き ][ インプット向き ]

		{// UP(プレイヤーが上向き)
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// UP 
			CPlayer::EMotion::MOTION_CRAB_BACK,		// DOWN
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// LEFT
			CPlayer::EMotion::MOTION_CRAB_RIGHT		// RIGHT
		},
		{// DOWN(プレイヤーが下向き)
			CPlayer::EMotion::MOTION_CRAB_BACK,		// UP 
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// DOWN
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// LEFT
			CPlayer::EMotion::MOTION_CRAB_LEFT 		// RIGHT
		},
		{// LEFT(プレイヤーが左向き)
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// UP 
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// DOWN
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// LEFT
			CPlayer::EMotion::MOTION_CRAB_BACK 		// RIGHT
		},
		{// RIGHT(プレイヤーが右向き)
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// UP 
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// DOWN
			CPlayer::EMotion::MOTION_CRAB_BACK,		// LEFT
			CPlayer::EMotion::MOTION_CRAB_FRONT 	// RIGHT
		},
	};
}

namespace Motion
{
	// デバッグ表示用
	const std::string NAME_MAP[CPlayer::EMotion::MOTION_MAX] =
	{
		"MOTION_DEF",				// ニュートラルモーション
		"MOTION_DEF_BALL",			// ニュートラルモーション(ボール所持)
		"MOTION_WALK",				// 移動
		"MOTION_WALK_BALL",			// 移動(ボール所持)
		"MOTION_CRAB_FRONT",		// カニ歩き(前)
		"MOTION_CRAB_BACK",			// カニ歩き(後)
		"MOTION_CRAB_LEFT",			// カニ歩き(左)
		"MOTION_CRAB_RIGHT",		// カニ歩き(右)
		"MOTION_RUN",				// 走り
		"MOTION_RUN_BALL",			// 走り(ボール所持)
		"MOTION_BLINK",				// ブリンク
		"MOTION_DODGE",				// 回避成功時
		"MOTION_JUMP",				// ジャンプ
		"MOTION_JUMP_BALL",			// ジャンプ(ボール所持)
		"MOTION_LAND",				// 着地
		"MOTION_CATCH_STANCE",		// キャッチの構え
		"MOTION_CATCH_STANCE_JUMP",	// キャッチの構え(ジャンプ)
		"MOTION_CATCH_NORMAL",		// キャッチ(通常)
		"MOTION_CATCH_JUMP",		// キャッチ(ジャンプ)
		"MOTION_JUSTCATCH_NORMAL",	// ジャストキャッチ(通常)
		"MOTION_JUSTCATCH_JUMP",	// ジャストキャッチ(ジャンプ)
		"MOTION_DROPCATCH_WALK",	// 落ちてるのキャッチ(歩き)
		"MOTION_THROW",				// 投げ
		"MOTION_THROW_RUN",			// 投げ(走り)
		"MOTION_THROW_JUMP",		// 投げ(ジャンプ)
		"MOTION_TOSS",				// トス
		"MOTION_HYPE",				// 盛り上げ
		"MOTION_SPECIAL",			// スペシャル
		"MOTION_WIN",				// 勝利
		"MOTION_DAMAGE",			// ダメージ
		"MOTION_DEAD",				// 死亡
		"MOTION_DEAD_AFTER",		// 死亡後
		"MOTION_GRIP_DEF",			// デフォグリップ
		"MOTION_GRIP_FRONT",		// 前グリップ
	};
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayer::STATE_FUNC CPlayer::m_StateFunc[] =	// 状態関数
{
	&CPlayer::StateNone,				// なし
	&CPlayer::StateInvincible,			// 無敵
	&CPlayer::StateDamage,				// ダメージ
	&CPlayer::StateDead,				// 死亡
	&CPlayer::StateDeadAfter,			// 死亡後
	&CPlayer::StateDodge,				// 回避
	&CPlayer::StateCatch_Normal,		// 通常キャッチ
	&CPlayer::StateCatch_Just,			// ジャストキャッチ
	&CPlayer::StateSpecial,				// スペシャル
	&CPlayer::StateOutCourt,			// コート越え
	&CPlayer::StateOutCourt_Return,		// コートに戻る
	&CPlayer::StateInvade_Toss,			// 相手コートに侵入トス
	&CPlayer::StateInvade_Return,		// 相手コート侵入から戻る
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
	m_sKnockback = SKnockbackInfo();	// ノックバック情報

	// 行動フラグ
	m_bPossibleMove = false;		// 移動可能フラグ
	m_bAutoMotionSet = false;		// オートモーション設定
	m_bJump = false;				// ジャンプ中かどうか
	m_bDash = false;				// ダッシュ判定
	m_bFootLR = false;				// 足左右判定
	m_bAlign = false;	// 揃え
	m_sMotionFrag = SMotionFrag();	// モーションのフラグ

	// パターン用インスタンス
	m_pActionPattern = nullptr;	// アクションパターン
	m_pStatus = nullptr;		// ステータス
	m_pBase = nullptr;			// ベース

	// 着せ替え
	m_pDressup_Hair = nullptr;		// 髪着せ替え
	m_pDressup_Accessory = nullptr;	// アクセ更新

	// その他
	m_nMyPlayerIdx = 0;				// プレイヤーインデックス番号
	m_pShadow = nullptr;			// 影の情報
	m_pBall = nullptr;				// ボールの情報
	m_sDamageInfo = SDamageInfo();	// ダメージ情報
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
CPlayer* CPlayer::Create(EBaseType type, const CGameManager::TeamSide team, const MyLib::Vector3& rPos, EHandedness handtype)
{
	// メモリの確保
	CPlayer* pPlayer = DEBUG_NEW CPlayer;
	if (pPlayer != nullptr)
	{
		// 利き手
		pPlayer->m_Handress = handtype;

		// クラスの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pPlayer);
			return nullptr;
		}

		// ベースを設定
		pPlayer->ChangeBase(type);

		// チームサイドを設定
		pPlayer->GetStatus()->SetTeam(team);

		// 位置を設定
		pPlayer->SetPosition(rPos);
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
	m_sDamageInfo.fReceiveTime = 0.0f;

	m_state = STATE_NONE;	// 状態
	m_Oldstate = m_state;
	m_sMotionFrag.bMove = true;
	m_bPossibleMove = true;

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE[m_Handress]);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// プレイヤーインデックス番号を設定
	m_nMyPlayerIdx = m_List.GetNumAll();

	// 割り当て
	m_List.Regist(this);

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

	// 着せ替え
	if (m_pDressup_Hair == nullptr)
	{
		m_pDressup_Hair = CDressup::Create(CDressup::EType::TYPE_HAIR, this, 15);	// 髪着せ替え
	}

	// アクセ
	if (m_pDressup_Accessory == nullptr)
	{
		m_pDressup_Accessory = CDressup::Create(CDressup::EType::TYPE_ACCESSORY, this, 16);	// 髪着せ替え
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Uninit()
{
	// 影
	m_pShadow = nullptr;
	
	// ボール
	m_pBall = nullptr;

	// アクションパターン
	SAFE_DELETE(m_pActionPattern);

	// ベース
	SAFE_DELETE(m_pBase);

	// ステータス
	SAFE_DELETE(m_pStatus);

	// 着せ替え
	SAFE_UNINIT(m_pDressup_Hair);
	SAFE_UNINIT(m_pDressup_Accessory);

	// 終了処理
	CObjectChara::Uninit();

	// 削除
	m_List.Delete(this);
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CPlayer::Kill()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// アクションパターン
	if (m_pActionPattern != nullptr)
	{
		delete m_pActionPattern;
		m_pActionPattern = nullptr;
	}

	// ステータス
	SAFE_DELETE(m_pStatus);

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsDeath()) return;

	// ドレスアップの更新
	UpdateDressUP(fDeltaTime, fDeltaRate, fSlowRate);

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
	float nowSlowRate = fSlowRate;
	if (m_bAlign) nowSlowRate *= Align::MULTIPLY_MOTION;
	CObjectChara::Update(fDeltaTime, fDeltaRate, nowSlowRate);

	// 操作
	Controll(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// モーションの設定処理
	if (CGameManager::GetInstance()->IsControll())
	{
		MotionSet(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// アクション更新
	if (m_pActionPattern != nullptr)
	{
		m_pActionPattern->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 位置の制限
	LimitPos();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 影の位置更新
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

#if _DEBUG	// デバッグ処理

	std::string treename = "Player" + std::to_string(m_nMyPlayerIdx);	// ツリー名
	if (ImGui::TreeNode(treename.c_str()))
	{
		Debug();
		ImGui::TreePop();
	}
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

	if (CGameManager::GetInstance()->IsControll() && m_bPossibleMove)
	{ // 行動できるとき

		// ベースの更新
		m_pBase->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();

	// 移動量加算
	pos += move * fDeltaRate * fSlowRate;

	// 慣性補正
	EAction act = m_pActionPattern->GetAction();
	float ratio = 0.25f;
	if (act == EAction::ACTION_BLINK)
	{
		ratio = 0.1f;
	}
	move.x += (0.0f - move.x) * (ratio * fDeltaRate * fSlowRate);
	move.z += (0.0f - move.z) * (ratio * fDeltaRate * fSlowRate);

	// 重力処理
	move.y -= mylib_const::GRAVITY * fDeltaRate * fSlowRate;

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 現在と目標の差分を求める
	float fRotDiff = GetRotDest() - rot.y;
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	// 角度の補正をする
	rot.y += fRotDiff * (0.25f * fDeltaRate * fSlowRate);
	UtilFunc::Transformation::RotNormalize(rot.y);

	// 向き設定
	SetRotation(rot);
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::SetMotion(int motionIdx, int startKey, bool bBlend) const
{
	//TAKADA: はじく条件(死んだら)
	//if (m_sMotionFrag.bDead) return;

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx, startKey, bBlend);
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::MotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_bAlign = false;	// 揃え

	// 足左右の更新
	UpdateFootLR();

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 現在の種類取得
	int nType = pMotion->GetType();
	int nOldType = pMotion->GetOldType();

	// オートモーション設定
	if (!m_bAutoMotionSet) return;

	// 移動できないと通さない
	if (!m_bPossibleMove) return;

	// 死亡時通さない
	if (m_sMotionFrag.bDead) return;

	// 再生中
	if (!pMotion->IsFinish()) return;

	if (m_sMotionFrag.bMove)
	{// 移動していたら

		m_sMotionFrag.bMove = false;	// 移動判定OFF

		// 移動モーション設定
		SetMoveMotion(false);

		if (nOldType != pMotion->GetType())
		{
			//m_bFootLR = true;
		}
	}
	else if (m_sMotionFrag.bJump)
	{// ジャンプ中

		// ジャンプのフラグOFF
		m_sMotionFrag.bJump = false;

		// ジャンプモーション
	}
	else
	{
		// デフォルトモーションの設定
		DefaultMotionSet(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// 移動モーション設定
//==========================================================================
void CPlayer::SetMoveMotion(bool bNowDrop)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 再生中モーション
	int nType = pMotion->GetType();

	// モーションの種類
	EMotion motionType;
	if (m_pBall == nullptr) // ボール所持によって分ける
	{
		motionType = m_bDash ? MOTION_RUN : MOTION_WALK;
	}
	else
	{
		motionType = m_bDash ? MOTION_RUN_BALL : MOTION_WALK_BALL;
	}

	// 歩行の情報取得
	CMotion::Info info = pMotion->GetInfo(motionType);

	// 開始キー
	int nStartKey = 0;
	if (m_bFootLR)
	{
		nStartKey = info.nNumKey / 2;
	}

	// モーション設定
	if (!m_bDash && IsCrab() && (motionType == MOTION_WALK || motionType == MOTION_WALK_BALL))
	{// カニ歩き
		MotionCrab(nStartKey);
	}
	else if(bNowDrop || nType != EMotion::MOTION_DROPCATCH_WALK)
	{
		SetMotion(motionType, nStartKey);
	}

	// ダッシュリセット
	m_bDash = false;
}

//==========================================================================
// デフォルトモーションの設定
//==========================================================================
void CPlayer::DefaultMotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 情報取得
	CMotion::Info info = pMotion->GetInfo();
	int nType = pMotion->GetType();

	// 状況別設定
	if (nType != EMotion::MOTION_WALK)
	{
		// ニュートラルモーション設定
		if (m_pBall != nullptr)	{ SetMotion(MOTION_DEF_BALL); }
		else					{ SetMotion(MOTION_DEF); }
	}
	else if (nType == EMotion::MOTION_WALK)
	{// ニュートラルモーション設定
		
		if (!pMotion->IsAlignFrame(info))
		{// 足が揃ってない

			// 移動量取得
			MyLib::Vector3 move = GetMove();

			// 移動する
			float fMove = GetParameter().fVelocityNormal;
			switch (nType)
			{
			case EMotion::MOTION_WALK:
				fMove = GetParameter().fVelocityNormal;
				break;

			case EMotion::MOTION_RUN:
				fMove = GetParameter().fVelocityDash;
				break;

			default:
				break;
			}

			// 補正倍率
			fMove *= fDeltaRate;
			fMove *= fSlowRate;
			fMove *= Align::MULTIPLY_MOVE;		// 揃えるからゆっくり

			// 移動量更新
			MyLib::Vector3 rot = GetRotation();
			move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * fMove;
			move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * fMove;

			// 移動量設定
			SetMove(move);
			m_bAlign = true;	// 揃え
		 }
		else
		{
			// ニュートラルモーション設定
			if (m_pBall != nullptr) { SetMotion(MOTION_DEF_BALL); }
			else					{ SetMotion(MOTION_DEF); }
		}
	}
}

//==========================================================================
// 足左右の更新
//==========================================================================
void CPlayer::UpdateFootLR()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)	return;

	// 情報取得
	CMotion::Info info = pMotion->GetInfo();
	int nType = pMotion->GetType();

	// 歩き以外は抜ける
	if (nType != EMotion::MOTION_DROPCATCH_WALK) return;

	if (pMotion->IsImpactFrame(info))
	{// 衝撃のフレーム

		if (nType == EMotion::MOTION_DROPCATCH_WALK)
		{// 落ちてるのを拾うとき

			// 移動モーション設定
			SetMoveMotion(true);
		}

		// 足左右切り替え : 落ちてるの拾うためにいるかも
		//m_bFootLR = !m_bFootLR;
	}

}

//==========================================================================
// フラグリセット
//==========================================================================
void CPlayer::ResetFrag()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	//キャッチできない状態
	m_sMotionFrag.bCatch = false;
	m_sMotionFrag.bCatchJust = false;
	m_bDash = false;

	// オートモーション設定
	m_bAutoMotionSet = true;

	
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
	case EMotion::MOTION_THROW:
	case EMotion::MOTION_THROW_RUN:

		if (m_pBall != nullptr)
		{// 通常投げ
			m_pBall->ThrowNormal(this);
		}
		break;

	case EMotion::MOTION_THROW_JUMP:

		if (m_pBall != nullptr)
		{// ジャンプ投げ
			m_pBall->ThrowJump(this);
		}
		break;

	case EMotion::MOTION_TOSS:

		if (m_pBall != nullptr)
		{// トス
			m_pBall->Toss(this);
		}
		break;

	case EMotion::MOTION_WALK:
	case EMotion::MOTION_WALK_BALL:
		PLAY_SOUND(CSound::ELabel::LABEL_SE_WALK);
		break;

	case EMotion::MOTION_RUN:
	case EMotion::MOTION_RUN_BALL:
	{
		// 設定するラベル
		CSound::ELabel label = static_cast<CSound::ELabel>(static_cast<int>(CSound::ELabel::LABEL_SE_RUN01) + nCntATK);
		PLAY_SOUND(label);
	}
		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CPlayer::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;
	int nType = pMotion->GetType();

	switch (nType)
	{
	case EMotion::MOTION_CATCH_STANCE:

		// キャッチフラグON
		m_sMotionFrag.bCatch = true;

		if (nCntATK == 1)
		{//2個目(1)がジャスト

			//ジャストフラグON
			m_sMotionFrag.bCatchJust = true;
		
			CEffect3D::Create(
				GetPosition(),
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				80.0f, 1.0f/60.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}

		break;

	default:
		break;
	}

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);

	if (ATKInfo.fRangeSize == 0.0f)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		ATKInfo.fRangeSize, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	if (ATKInfo.bEndAtk)
	{// 終了してたら抜ける
		return;
	}
}

//==========================================================================
// キャッチ時処理(地上・通常)
//==========================================================================
void CPlayer::CatchSettingLandNormal(CBall::EAttack atkBall)
{
	switch (atkBall)
	{
	case CBall::ATK_NORMAL:
		SetMotion(EMotion::MOTION_CATCH_NORMAL);
		break;

	case CBall::ATK_JUMP:
		SetMotion(EMotion::MOTION_CATCH_JUMP);
		break;

	case CBall::ATK_SPECIAL:	//TODO: スペシャルに変更
		SetMotion(EMotion::MOTION_CATCH_JUMP);
		break;

	default:
		break;
	}

	// キャッチ状態
	SetState(EState::STATE_CATCH_NORMAL);
}

//==========================================================================
// キャッチ時処理(地上・ジャスト)
//==========================================================================
void CPlayer::CatchSettingLandJust(CBall::EAttack atkBall)
{
	MyLib::Vector3 pos = GetPosition();
	pos.y += 130.0f;

	//演出
	CEffect3D::Create(
		pos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		80.0f, 4.0f / 60.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	switch (atkBall)
	{
	case CBall::ATK_NORMAL:
		SetMotion(EMotion::MOTION_JUSTCATCH_NORMAL);
		break;

	case CBall::ATK_JUMP:
		SetMotion(EMotion::MOTION_JUSTCATCH_JUMP);
		break;

	case CBall::ATK_SPECIAL:	//TODO: スペシャルに変更
		SetMotion(EMotion::MOTION_JUSTCATCH_JUMP);
		break;

	default:
		break;
	}

	// ジャストキャッチ状態
	SetState(EState::STATE_CATCH_JUST);

	// モテ加算
	CGameManager* pGameMgr = CGameManager::GetInstance();
	pGameMgr->AddCharmValue(GetStatus()->GetTeam(), CCharmManager::EType::TYPE_JUSTCATCH);
}

//==========================================================================
// カニ歩きモーション設定
//==========================================================================
void CPlayer::MotionCrab(int nStartKey)
{
	CRAB_DIRECTION playerDir = CRAB_DIRECTION::CRAB_NONE;
	CRAB_DIRECTION inputDir = CRAB_DIRECTION::CRAB_NONE;

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 向き
	MyLib::Vector3 rot = GetRotation();

	// ラムダ(bool)
	auto CollisionRangeAngle = [](float angle, float maxAngle, float minAngle)
	{
		// 正規化解除
		UtilFunc::Transformation::RotUnNormalize(angle);
		UtilFunc::Transformation::RotUnNormalize(maxAngle);
		UtilFunc::Transformation::RotUnNormalize(minAngle);

		// 度数法に変換
		int nAngle =	static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(angle));
		int nMaxAngle =	static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(maxAngle));
		int nMinAngle =	static_cast<int>(UtilFunc::Transformation::RadianChangeToDegree(minAngle));

		if (nMaxAngle <= nMinAngle)
		{// 範囲が360°を跨ぐ場合

			// nAngleがMin以上Max以下。
			bool bRange = (nMaxAngle <= nAngle && nAngle <= nMinAngle);
			return bRange;
		}
		else
		{// 範囲が通常の順序で指定されている場合

			// nAngleがMin以上Max以下。
			bool bRange = (nMaxAngle >= nAngle && nAngle >= nMinAngle);
			return bRange;
		}

		return false;
	};

	//--------------------------------
	// プレイヤー方向
	//--------------------------------
	bool bRot = false;
	D3DXCOLOR col = D3DXCOLOR();
	float fRotY = D3DX_PI * 1.0f + rot.y;
	UtilFunc::Transformation::RotNormalize(fRotY);

	float fRangeZero = Crab::RANGE_MIN_MAX[0];
	UtilFunc::Transformation::RotNormalize(fRangeZero);
	if (!CollisionRangeAngle(fRotY, fRangeZero, Crab::RANGE_MIN_MAX[1]))
	{// 下向き
		playerDir = CRAB_DIRECTION::CRAB_DOWN;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[2], Crab::RANGE_MIN_MAX[3]))
	{// 上向き
		playerDir = CRAB_DIRECTION::CRAB_UP;
		bRot = true;
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[4], Crab::RANGE_MIN_MAX[5]))
	{// 左向き
		playerDir = CRAB_DIRECTION::CRAB_LEFT;
		bRot = true;
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (CollisionRangeAngle(fRotY, Crab::RANGE_MIN_MAX[6], Crab::RANGE_MIN_MAX[7]))
	{// 右向き
		playerDir = CRAB_DIRECTION::CRAB_RIGHT;
		bRot = true;
		col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{// 抜けちゃった
		MyAssert::CustomAssert(false, "カニ歩き：どこ見てんねん");
	}

	//--------------------------------
	// 入力方向
	//--------------------------------
	EDashAngle* angle = m_pBase->GetPlayerControlMove()->GetInputAngle();
	if (angle == nullptr) return;

	switch (*angle)
	{
	case EDashAngle::ANGLE_UP:
		inputDir = CRAB_DIRECTION::CRAB_UP;
		break;

	case EDashAngle::ANGLE_DOWN:
		inputDir = CRAB_DIRECTION::CRAB_DOWN;
		break;
	
	case EDashAngle::ANGLE_RIGHT:
	case EDashAngle::ANGLE_RIGHTUP:
	case EDashAngle::ANGLE_RIGHTDW:
		inputDir = CRAB_DIRECTION::CRAB_RIGHT;
		break;
	
	case EDashAngle::ANGLE_LEFT:
	case EDashAngle::ANGLE_LEFTUP:
	case EDashAngle::ANGLE_LEFTDW:
		inputDir = CRAB_DIRECTION::CRAB_LEFT;
		break;

	default:
		break;
	}

	if (playerDir == CRAB_DIRECTION::CRAB_NONE ||
		inputDir == CRAB_DIRECTION::CRAB_NONE)
	{// 判定に引っかかっていない
		return;
	}

	// モーション設定
	SetMotion(Crab::MOTION_WALK[playerDir][inputDir], nStartKey);
}

//==========================================================================
// 位置制限
//==========================================================================
void CPlayer::LimitPos()
{
	MyLib::Vector3 pos = GetPosition();
	
	if (m_state != EState::STATE_OUTCOURT &&
		m_state != EState::STATE_OUTCOURT_RETURN)
	{//コート越え状態以外はコート内補正
		CGameManager::GetInstance()->SetPosLimit(pos);
	}

	if (!m_bJump && !m_sMotionFrag.bDead)
	{// 相手コートに侵入したとき用コート内に補正
		TeamCourt_Return(pos);
	}

	if (pos.y <= 0.0f)
	{
		pos.y = 0.0f;

		if (m_bJump && !m_sMotionFrag.bDead)
		{// ジャンプ中着地
			SetMotion(EMotion::MOTION_LAND);
		}

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
CPlayer::SHitInfo CPlayer::Hit(CBall* pBall)
{
	// ベースのヒット処理
	return m_pBase->Hit(pBall);
}

//==========================================================================
// スペシャル攻撃の設定
//==========================================================================
void CPlayer::SetSpecialAttack()
{
	bool bInverse = (m_pStatus->GetTeam() == CGameManager::TeamSide::SIDE_LEFT) ? false : true;	// カメラモーションの反転フラグ
	CCamera* pCamera = GET_MANAGER->GetCamera();				// カメラ情報
	CCameraMotion* pCameraMotion = pCamera->GetCameraMotion();	// カメラモーション情報

	// かめはめ波モーションを設定
	pCameraMotion->SetMotion(CCameraMotion::MOTION_KAMEHAMEHA, bInverse);	// TODO：スペシャルごとに変更

	// スペシャル状態にする
	SetState(STATE_SPECIAL);

	// スペシャルモーションにする
	SetMotion(CPlayer::MOTION_SPECIAL);	// TODO：スペシャルごとに変更
}

//==========================================================================
// 死亡時の設定
//==========================================================================
void CPlayer::DeadSetting(MyLib::HitResult_Character* result, CBall* pBall)
{
	// 死亡状態にする
	SetState(STATE_DEAD);
	SetMotion(MOTION_DEAD);
	m_sMotionFrag.bDead = true;

	// ノックバックの位置設定
	MyLib::Vector3 vecBall = pBall->GetMove().Normal();
	MyLib::Vector3 posS = GetPosition();
	MyLib::Vector3 posE = posS;
	posE.x += vecBall.x * Knockback::DEAD;
	posE.z += vecBall.z * Knockback::DEAD;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	// 死んだ
	result->isdeath = true;
}

//==========================================================================
// ダメージ発生時設定
//==========================================================================
void CPlayer::DamageSetting(CBall* pBall)
{
	// ダメージ状態にする
	SetState(EState::STATE_DMG);
	SetMotion(EMotion::MOTION_DAMAGE);

	// ノックバックの位置設定
	MyLib::Vector3 vecBall = pBall->GetMove().Normal();
	MyLib::Vector3 posS = GetPosition();	//始点
	MyLib::Vector3 posE = posS;				//終点
	posE.x += vecBall.x * Knockback::DAMAGE;
	posE.z += vecBall.z * Knockback::DAMAGE;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	// ダメージ受付時間を設定
	m_sDamageInfo.fReceiveTime = StateTime::DAMAGE;
}

//==========================================================================
// キャッチ時処理
//==========================================================================
void CPlayer::CatchSetting(CBall* pBall)
{
	// ボールをキャッチ
	pBall->CatchAttack(this);

	CBall::EAttack atkBall = pBall->GetTypeAtk();	// ボール攻撃種類
	MyLib::Vector3 posB = pBall->GetPosition();		// ボール位置
	MyLib::Vector3 pos = GetPosition();				// プレイヤー位置
	
	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 入力判定
	bool bInput = false;
	EDashAngle* angle = m_pBase->GetPlayerControlMove()->GetInputAngle();
	if (angle != nullptr)
	{
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		float fRot = division * (*angle) + D3DX_PI + Camerarot.y;
		UtilFunc::Transformation::RotNormalize(fRot);
		bInput = UtilFunc::Collision::CollisionViewRange3D(pos, posB, fRot, JUST_VIEW);
	}

	// モーション設定
	if (m_sMotionFrag.bCatchJust
		&& bInput)
	{// ジャストキャッチ

		CatchSettingLandJust(atkBall);		// キャッチ時処理(地上・ジャスト)
	}
	else
	{// 通常キャッチ

		CatchSettingLandNormal(atkBall);	// キャッチ時処理(地上・通常)
	}

	// 受けた種類
	m_sDamageInfo.eReiceiveType = atkBall;

	// 今切り替えられました
}

//==========================================================================
// カバーキャッチ時処理
//==========================================================================
void CPlayer::CoverCatchSetting(CBall* pBall)
{
	// カバー対象を回復
	CPlayer* pCoverPlayer = pBall->GetCover();
	if (pCoverPlayer == nullptr) return;

	// 死んでいない味方回復
	if (!pCoverPlayer->GetMotionFrag().bDead &&
		m_pStatus->GetTeam() == pCoverPlayer->GetStatus()->GetTeam())
	{
		pCoverPlayer->GetStatus()->LifeHeal(10);
	}

	// 演出
	CEffect3D::Create(
		GetPosition(),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f),
		80.0f, 4.0f / 60.0f, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	// 攻撃キャッチ処理
	pBall->CatchAttack(this);

	// ゲームマネージャ取得
	CGameManager* pGameMgr = CGameManager::GetInstance();
	if (pGameMgr == nullptr) return;

	// モテ加算
	pGameMgr->AddCharmValue(GetStatus()->GetTeam(), CCharmManager::EType::TYPE_COVERCATCH);
}

//==========================================================================
// コート越え処理
//==========================================================================
void CPlayer::OutCourtSetting()
{
	//ダメージ
	m_pStatus->LifeDamage(10);
	SetMotion(EMotion::MOTION_DAMAGE);

	// ノックバックの位置設定
	MyLib::Vector3 rot = GetRotation();
	UtilFunc::Transformation::RotNormalize(rot.y);
	MyLib::Vector3 posS = GetPosition();	//始点
	MyLib::Vector3 posE = posS;				//終点
	posE.x += sinf(rot.y) * Knockback::DEAD;
	posE.z += cosf(rot.y) * Knockback::DEAD;
	m_sKnockback.posStart = posS;
	m_sKnockback.posEnd = posE;

	SetState(EState::STATE_OUTCOURT);
}

//==========================================================================
// チームコート内に戻る
//==========================================================================
void CPlayer::TeamCourt_Return(MyLib::Vector3& pos)
{
	// 自陣サイズ取得
	CGameManager::TeamSide team = GetStatus()->GetTeam();
	MyLib::Vector3 posCourt = MyLib::Vector3();
	MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize(team, posCourt);

	// もう戻ってる場合は抜ける
	if (m_state == EState::STATE_INVADE_RETURN ||
		m_state == EState::STATE_INVADE_TOSS) return;

	// 場外判定
	bool bOut = false;
	switch (team)
	{
	case CGameManager::SIDE_LEFT:	// 左チーム

		if (pos.x > Court::COMEBACK_LINE)
		{// ライン越え

			bOut = true;
		}
		break;

	case CGameManager::SIDE_RIGHT:	// 右チーム

		if (pos.x < -Court::COMEBACK_LINE)
		{// ライン越え

			bOut = true;
		}
		break;

	default:
		return;
		break;
	}

	if (bOut)
	{
		// ノックバックの開始位置
		m_sKnockback.posStart = GetPosition();

		// ボールの有無で遷移別
		if (m_pBall == nullptr)
		{
			// 侵入から戻る状態へ遷移
			SetState(EState::STATE_INVADE_RETURN);
		}
		else
		{
			// トスモーション設定
			SetMotion(EMotion::MOTION_TOSS);

			// トスへ遷移
			SetState(EState::STATE_INVADE_TOSS);
		}
	}
}

//==========================================================================
// 回避範囲取得
//==========================================================================
float CPlayer::GetDodgeDistance()
{
	return 0.0f;
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayer::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ダメージ受付時間更新
	UpdateDamageReciveTimer(fDeltaTime, fDeltaRate, fSlowRate);

	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ダメージ受付時間更新
//==========================================================================
void CPlayer::UpdateDamageReciveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ダメージ受付判定リセット
	m_sDamageInfo.bReceived = false;

	// ダメージ受け付け時間減算
	m_sDamageInfo.fReceiveTime -= fDeltaTime;
	if (m_sDamageInfo.fReceiveTime <= 0.0f)
	{
		// ダメージ受け付け判定
		m_sDamageInfo.bReceived = true;
		m_sDamageInfo.fReceiveTime = 0.0f;
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
	if (m_fStateTime >= StateTime::INVINCIBLE)
	{
		m_sDamageInfo.fReceiveTime = 0.0f;
		m_sDamageInfo.bReceived = true;

		SetState(STATE_NONE);
	}
}

//==========================================================================
// ダメージ状態
//==========================================================================
void CPlayer::StateDamage()
{
	MyLib::Vector3 pos = GetPosition();
	CMotion* pMotion = GetMotion();
	
	// ノックバック
	float time = m_fStateTime / StateTime::DAMAGE;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);
	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT,time);
	SetPosition(pos);

	if (m_fStateTime >= StateTime::DAMAGE)
	{
		// 移動可能
		SetEnableMove(true);

		pMotion->ToggleFinish(true);
		SetState(STATE_INVINCIBLE);
	}
}

//==========================================================================
// 死亡状態
//==========================================================================
void CPlayer::StateDead()
{
	MyLib::Vector3 pos = GetPosition();

	float time = m_fStateTime / StateTime::DEAD;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT, time);

	SetPosition(pos);

	// 死亡状態をキャンセル不能にする
	SetEnableMove(false);
	//m_sMotionFrag.bDead = true;

	if (m_fStateTime >= StateTime::DEAD)
	{
		SetState(STATE_DEAD_AFTER);
		SetMotion(MOTION_DEAD_AFTER);
	}
}

//==========================================================================
// 死亡状態
//==========================================================================
void CPlayer::StateDeadAfter()
{
	//MyLib::Vector3 pos = GetPosition();

	//float time = m_fStateTime / StateTime::DEAD;
	//time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	//pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT, time);

	//SetPosition(pos);

	//死亡状態をキャンセル不能にする
	SetEnableMove(false);
	//m_sMotionFrag.bDead = true;
}

//==========================================================================
// 回避
//==========================================================================
void CPlayer::StateDodge()
{

}

//==========================================================================
// 通常キャッチ
//==========================================================================
void CPlayer::StateCatch_Normal()
{
	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// キャンセル可能フレーム取得
	CMotion::Info motionInfo = pMotion->GetInfo();
	float fCancelableTime = static_cast<float>(motionInfo.nCancelableFrame);

	// TODO : ずざざーとする
	MyLib::Vector3 rot = GetRotation();

	// 割合
	float ratio = m_fStateTime / StateTime::CATCH;
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.4f, 1.0f);
	ratio -= 1.0f;

	// 後退する
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	// 移動量更新
	move.x += sinf(D3DX_PI + rot.y) * (Catch::Impact[m_sDamageInfo.eReiceiveType] * ratio);
	move.z += cosf(D3DX_PI + rot.y) * (Catch::Impact[m_sDamageInfo.eReiceiveType] * ratio);

	// 位置更新
	pos.x += move.x;
	pos.z += move.z;
	SetPosition(pos);
	SetMove(move);

	//TODO: 砂埃的な何か

	//スペシャル時ライン越え判定
	if (m_sDamageInfo.eReiceiveType == CBall::EAttack::ATK_SPECIAL &&
		CGameManager::GetInstance()->SetPosLimit(pos) &&
		m_state != EState::STATE_OUTCOURT)
	{
		OutCourtSetting();
	}
		
	if (pMotion->IsGetCancelable())
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ジャストキャッチ
//==========================================================================
void CPlayer::StateCatch_Just()
{
	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->IsGetCancelable())
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayer::StateSpecial()
{
	CMotion* pMotion = GetMotion();	// プレイヤーモーション情報

	if (!pMotion->IsSpecial())
	{ // モーションがスペシャルじゃなくなった場合

		// 何もしない状態にする
		SetState(STATE_NONE);
	}
}

//==========================================================================
// コート越え
//==========================================================================
void CPlayer::StateOutCourt()
{
	MyLib::Vector3 pos = GetPosition();

	//ノックバック
	float time = m_fStateTime / StateTime::DAMAGE;
	time = UtilFunc::Transformation::Clamp(time, 0.0f, 1.0f);

	pos = UtilFunc::Calculation::GetParabola3D(m_sKnockback.posStart, m_sKnockback.posEnd, Knockback::HEIGHT_OUTCOURT, time);

	SetPosition(pos);

	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->IsGetCancelable())
	{// キャンセル可能
		SetState(EState::STATE_OUTCOURT_RETURN);
	}
}

//==========================================================================
// コート越えから戻る
//==========================================================================
void CPlayer::StateOutCourt_Return()
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posStart = CGameManager::GetInstance()->GetCourtSize();
	posStart.y = 0.0f;
	posStart.z = 0.0f;
	posStart.x *= 0.5f;

	//コート内に戻る
	pos = UtilFunc::Correction::EasingLinear(m_sKnockback.posEnd, posStart, 0.0f, StateTime::COURT_RETURN, m_fStateTime);
	SetPosition(pos);

	// モーションのキャンセルで管理
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	if (m_fStateTime >= StateTime::COURT_RETURN)
	{// キャンセル可能
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// 相手コートに侵入トス
//==========================================================================
void CPlayer::StateInvade_Toss()
{
	// オートモーション設定解除
	m_bAutoMotionSet = false;

	// 移動不可
	m_bPossibleMove = false;

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;

	// トスするまで以下return
	if (m_pBall != nullptr)
	{
		m_fStateTime = 0.0f;
		return;
	}

	if (pMotion->GetType() == EMotion::MOTION_TOSS && !pMotion->IsGetCancelable())
	{
		m_fStateTime = 0.0f;
		return;
	}

	//-----------------------------
	// ボールのトス終了後処理
	//-----------------------------
	
	// 走って戻す
	SetMotion(CPlayer::EMotion::MOTION_WALK);

	// チーム別向き設定
	float fRotDest = 0.0f;
	switch (GetStatus()->GetTeam())
	{
	case CGameManager::SIDE_LEFT:	// 左チーム
		fRotDest = HALF_PI;
		break;

	case CGameManager::SIDE_RIGHT:	// 右チーム
		fRotDest = -HALF_PI;
		break;

	default:
		return;
		break;
	}

	// 向き設定
	SetRotDest(fRotDest);

	// 遷移タイマー
	if (StateTime::INVADE_TOSS <= m_fStateTime)
	{
		// 相手コート侵入から戻る
		SetState(EState::STATE_INVADE_RETURN);
	}

}

//==========================================================================
// 相手コート侵入から戻る
//==========================================================================
void CPlayer::StateInvade_Return()
{
	// 自陣サイズ取得
	CGameManager::TeamSide team = GetStatus()->GetTeam();
	MyLib::Vector3 posCourt = MyLib::Vector3();
	MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize(team, posCourt);
	MyLib::Vector3 pos = GetPosition();

	// チーム別でラインの位置まで戻す
	MyLib::Vector3 posDest = m_sKnockback.posStart;
	switch (team)
	{
	case CGameManager::SIDE_LEFT:	// 左チーム

		posDest.x = -Court::COMEBACK_LINE;
		break;

	case CGameManager::SIDE_RIGHT:	// 右チーム

		posDest.x = Court::COMEBACK_LINE;
		break;

	default:
		return;
		break;
	}

	// オートモーション設定解除
	m_bAutoMotionSet = false;

	// 移動不可
	m_bPossibleMove = false;

	// 走って戻す
	SetMotion(CPlayer::EMotion::MOTION_WALK);

	// 移動量更新
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();
#if 1
	move.x += sinf(D3DX_PI + rot.y) * Court::VELOCITY_INVADE;
	move.z += cosf(D3DX_PI + rot.y) * Court::VELOCITY_INVADE;
#else
	move.x += sinf(D3DX_PI + rot.y) * GetParameter().fVelocityDash;
	move.z += cosf(D3DX_PI + rot.y) * GetParameter().fVelocityDash;
#endif
	SetMove(move);


	// 戻る方向向く
	float rotDest = m_sKnockback.posStart.AngleXZ(posDest);
	UtilFunc::Transformation::RotNormalize(rotDest);
	SetRotDest(rotDest);

#if 0	// 無敵にするならこれ有効に
	// ダメージ受付判定リセット
	m_sDamageInfo.bReceived = false;
#endif

	// X軸の位置で割合
	float ratio = GetPosition().x / posDest.x;
	if (ratio >= 1.0f)
	{// 完了
		m_bPossibleMove = true;
		SetState(EState::STATE_NONE);
	}
}

//==========================================================================
// ドレスアップの更新
//==========================================================================
void CPlayer::UpdateDressUP(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 髪更新
	if (m_pDressup_Hair != nullptr)
	{
		m_pDressup_Hair->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// アクセ更新
	if (m_pDressup_Accessory != nullptr)
	{
		m_pDressup_Accessory->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

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
void CPlayer::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// ベースの変更処理
//==========================================================================
void CPlayer::ChangeBase(EBaseType type)
{
	// ベースクラスの破棄
	SAFE_DELETE(m_pBase);

	// ベースクラスの変更
	switch (type)
	{ // ユーザー種類ごとの処理
	case TYPE_USER:
		m_pBase = DEBUG_NEW CPlayerUser(this);
		break;

	case TYPE_AI:
		m_pBase = DEBUG_NEW CPlayerAI(this);
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// ベースの取得処理
//==========================================================================
CPlayer::EBaseType CPlayer::GetBaseType() const
{
	// クラス型からベースを判定
	if		(typeid(*m_pBase) == typeid(CPlayerUser))	{ return TYPE_USER; }
	else if	(typeid(*m_pBase) == typeid(CPlayerAI))		{ return TYPE_AI; }

	// ベース指定なし
	assert(false);
	return (EBaseType)-1;
}

//==========================================================================
// カニ歩き判定
//==========================================================================
bool CPlayer::IsCrab()
{
	if (m_pBall != nullptr) return false;

	CPlayer::EAction action = GetActionPattern()->GetAction();
	int motionType = GetMotion()->GetType();
	
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return false;

	// ボールの状態：敵側であるか
	if (pBall->GetTypeTeam() == GetStatus()->GetTeam()) return false;
	if (pBall->GetTypeTeam() == CGameManager::TeamSide::SIDE_NONE) return false;
	//if (pBall->GetState() != CBall::EState::STATE_CATCH) return false;

	// 自身の状態：ブリンク＆走りでない
	if (action == CPlayer::EAction::ACTION_BLINK) return false;
	if (action == CPlayer::EAction::ACTION_JUMP && m_bDash) return false;
	if (m_bDash) return false;

	return true;
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CPlayer::Debug()
{
#if _DEBUG

	//-----------------------------
	// 位置
	//-----------------------------
	if (ImGui::TreeNode("SetPosition"))
	{
		// 位置取得
		MyLib::Vector3 pos = GetPosition();

		if (ImGui::Button("Reset"))
		{// リセット
			pos = MyLib::Vector3();
		}
		ImGui::DragFloat3("pos", (float*)&pos, 1.0f, 0.0f, 0.0f, "%.2f");

		// 位置設定
		SetPosition(pos);
		ImGui::TreePop();
	}

	//-----------------------------
	// パラメーター
	//-----------------------------
	if (ImGui::TreeNode("Parameter"))
	{
		// 取得
		CCharacterStatus* pStatus = GetCharStatus();
		CCharacterStatus::CharParameter parameter = pStatus->GetParameter();

		ImGui::DragFloat("fVelocityNormal", (float*)&parameter.fVelocityNormal, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fVelocityDash", (float*)&parameter.fVelocityDash, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fVelocityBlink", (float*)&parameter.fVelocityBlink, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fVelocityJump", (float*)&parameter.fVelocityJump, 0.01f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fRadius", (float*)&parameter.fRadius, 0.5f, 0.0f, 100.0f, "%.2f");
		ImGui::DragFloat("fJumpStartMove", &parameter.fJumpStartMove, 0.001f, 0.0f, 100.0f, "%.3f");
		ImGui::DragFloat("fJumpUpdateMove", &parameter.fJumpUpdateMove, 0.0001f, 0.0f, 100.0f, "%.3f");
		ImGui::DragFloat3("ballOffset", (float*)&parameter.ballOffset, 0.1f, -2000.0f, 2000.0f, "%.3f");

		// 設定
		SetRadius(parameter.fRadius);

		pStatus->SetParameter(parameter);
		
		ImGui::TreePop();
	}

	//-----------------------------
	// 情報表示
	//-----------------------------
	if (ImGui::TreeNode("Transform Info"))
	{
		MyLib::Vector3 pos = GetPosition();
		MyLib::Vector3 rot = GetRotation();
		MyLib::Vector3 move = GetMove();
		CMotion* motion = GetMotion();

		ImGui::Text("pos : [X : %.2f, Y : %.2f, Z : %.2f]", pos.x, pos.y, pos.z);
		ImGui::Text("rot : [X : %.2f, Y : %.2f, Z : %.2f]", rot.x, rot.y, rot.z);
		ImGui::Text("rotDest : [Y : %.2f]", GetRotDest());
		ImGui::Text("move : [X : %.2f, Y : %.2f, Z : %.2f]", move.x, move.y, move.z);
		ImGui::Text("Life : [%d]", GetLife());
		ImGui::Text("State : [%d]", m_state);
		ImGui::Text("Action : [%d]", m_pActionPattern->GetAction());
		ImGui::Text("Motion : [%s]", Motion::NAME_MAP[static_cast<CPlayer::EMotion>(motion->GetType())].c_str());
		ImGui::Text("bPossibleMove: [%s]", m_bPossibleMove ? "true" : "false");

		//現在の入力方向を取る(向き)
		bool bInput = false;
		EDashAngle* angle = m_pBase->GetPlayerControlMove()->GetInputAngle();
		if (angle != nullptr)
		{
			float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
			float fRot = division * *angle;
			ImGui::Text("fRot : [%.2f]", fRot);
		}

		ImGui::TreePop();
	}

	//-----------------------------
	// 体力
	//-----------------------------
	if (ImGui::TreeNode("Life"))
	{
		int nLife = GetLife();

		ImGui::DragInt("nLife", &nLife, 1, 0, 50, "%d");

		SetLife(nLife);

		ImGui::TreePop();
	}

	// 髪更新
	if (m_pDressup_Hair != nullptr)
	{
		m_pDressup_Hair->Debug();
	}

	// アクセ更新
	if (m_pDressup_Accessory != nullptr)
	{
		m_pDressup_Accessory->Debug();
	}

	if (ImGui::Button("Dead"))
	{// リセット
		MyLib::HitResult_Character* result = DEBUG_NEW MyLib::HitResult_Character;
		CBall* pBall = CBall::Create(GetPosition());
		DeadSetting(result,pBall);
		delete result;
		pBall->Kill();
	}

	//-----------------------------
	// コート外検証
	//-----------------------------
	if (ImGui::Button("OutCourt"))
	{// リセット
		OutCourtSetting();
	}

	//-----------------------------
	// ベースのデバッグ表示
	//-----------------------------
	m_pBase->Debug();

#endif
}

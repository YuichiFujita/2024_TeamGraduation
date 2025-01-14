//=============================================================================
// 
//  タイトルの生徒処理 [titlestudent.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "titlestudent.h"
#include "manager.h"
#include "calculation.h"
#include "shadow.h"
#include "sound.h"
#include "player.h"
#include "dressup.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE[CPlayer::EBody::BODY_MAX] = 	// キャラクターファイル
	{
		"data\\TEXT\\character\\title\\setup_player.txt",		// ノーマル
		"data\\TEXT\\character\\title\\setup_player_fat.txt",	// デブ
		"data\\TEXT\\character\\title\\setup_player_gari.txt",	// ガリ
	};

	const float RANGE = D3DX_PI * 0.8f;	// 生成範囲
	const float DISTANCE = 600.0f;	// 間隔
}

namespace StateTime
{
	const float WAIT = 1.0f;	// 待機
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitleStudent::STATE_FUNC CTitleStudent::m_StateFunc[] =	// 状態関数
{
	&CTitleStudent::StateNone,		// なし
	&CTitleStudent::StateWait,		// 待機
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CTitleStudent> CTitleStudent::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CTitleStudent::CTitleStudent(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	// 状態
	m_state = EState::STATE_NONE;		// 状態
	m_fStateTime = 0.0f;				// 状態時間

	// ドレスアップ
	m_pDressUp_Hair = nullptr;		// ドレスアップ(髪)
	m_pDressUp_Accessory = nullptr;	// ドレスアップ(アクセ)
	m_pDressUp_Face = nullptr;		// ドレスアップ(顔)

	// その他変数
	m_pShadow = nullptr;			// 影
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitleStudent::~CTitleStudent()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitleStudent* CTitleStudent::Create()
{
	// メモリの確保
	CTitleStudent* pObj = DEBUG_NEW CTitleStudent();
	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitleStudent::Init()
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_PLAYER);

	// 状態
	SetState(EState::STATE_NONE);

	// キャラ作成
	int body = UtilFunc::Transformation::Random(0, CPlayer::EBody::BODY_MAX - 1);
	HRESULT hr = SetCharacter(CHARAFILE[body]);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// リストに割当
	m_List.Regist(this);

	// 影生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(this, 50.0f);
	}

	// ドレスアップ(髪)
	m_pDressUp_Hair = CDressup::Create(
		CDressup::EType::TYPE_HAIR,		// 着せ替えの種類
		this,							// 変更するプレイヤー
		CPlayer::ID_HAIR);				// 変更箇所のインデックス

	// ドレスアップ(アクセ)
	m_pDressUp_Accessory = CDressup::Create(
		CDressup::EType::TYPE_ACCESSORY,	// 着せ替えの種類
		this,								// 変更するプレイヤー
		CPlayer::ID_ACCESSORY);				// 変更箇所のインデックス

	// ドレスアップ(顔)
	m_pDressUp_Face = CDressup::Create(
		CDressup::EType::TYPE_FACE,	// 着せ替えの種類
		this,						// 変更するプレイヤー
		CPlayer::ID_FACE);			// 変更箇所のインデックス

	// ランダム設定
	m_pDressUp_Hair->RandSet();
	m_pDressUp_Accessory->RandSet();
	m_pDressUp_Face->RandSet();



	// プレイヤー数
	int size = m_List.GetNumAll();
	float division = RANGE / size;	// 1分割あたりの向き
	float fRot = RANGE / (float)(size + 1);

	MyLib::Vector3 setpos, setrot;

	// カメラの位置
	MyLib::Vector3 posV = GET_MANAGER->GetCamera()->GetPositionV();

	for (int i = 0; i < size; i++)
	{
		// 位置計算
		setpos.x = sinf(fRot * (i + 1) - RANGE * 0.5f) * DISTANCE;
		setpos.z = DISTANCE * 0.25f + cosf(fRot * (i + 1) - RANGE * 0.5f) * DISTANCE;

		// カーブを緩やかにする
		setpos.z -= cosf(fRot * (i + 1) - RANGE * 0.5f) * DISTANCE * 0.75f;

		// 位置設定
		m_List.GetData(i)->SetPosition(setpos);

		// 向き設定
		setrot.y = setpos.AngleXZ(posV);
		m_List.GetData(i)->SetRotation(setrot);
	}

	// 状態
	m_state = EState::STATE_WAIT;		// 状態
	float ratio = (float)UtilFunc::Transformation::Random(0, 10) / (float)10;
	m_fStateTime = ratio * StateTime::WAIT;	// 状態時間

	// 待機
	GetMotion()->Set(EMotion::MOTION_WAIT);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitleStudent::Uninit()
{
	// 影
	m_pShadow = nullptr;

	// ドレスアップ
	SAFE_UNINIT(m_pDressUp_Hair);
	SAFE_UNINIT(m_pDressUp_Accessory);
	SAFE_UNINIT(m_pDressUp_Face);

	// 終了処理
	CObjectChara::Uninit();

	// プレイヤーリストから削除
	m_List.Delete(this);
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CTitleStudent::Kill()
{
	// 影を消す
	SAFE_UNINIT(m_pShadow);

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitleStudent::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsDeath()) return;

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// 親の更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CTitleStudent::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
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
	case EMotion::MOTION_DEF:
		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CTitleStudent::AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr) return;
	int nType = pMotion->GetType();

	switch (nType)
	{
	case EMotion::MOTION_DEF:
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
// 状態更新
//==========================================================================
void CTitleStudent::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態タイマー加算
	m_fStateTime += fDeltaTime * fSlowRate;

	// 状態更新
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CTitleStudent::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション取得
	CMotion* pMotion = GetMotion();

	if (pMotion->IsFinish())
	{
		// 待機する
		m_state = EState::STATE_WAIT;
		m_fStateTime = 0.0f;

		// 待機
		GetMotion()->Set(EMotion::MOTION_WAIT);
	}
}

//==========================================================================
// 待機
//==========================================================================
void CTitleStudent::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= StateTime::WAIT)
	{// 待機終了

		// 準備体操する
		m_state = EState::STATE_NONE;
		m_fStateTime = 0.0f;

		// ランダムの準備体操設定
		SetRandWarmUp();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitleStudent::Draw()
{
	// 描画
	CObjectChara::Draw();
}

//==========================================================================
// ランダムの準備体操設定
//==========================================================================
void CTitleStudent::SetRandWarmUp()
{
	// モーション設定
	GetMotion()->Set(UtilFunc::Transformation::Random(MOTION_WARMUP_01, MOTION_WARMUP_01 + (MOTION_MAX - MOTION_WARMUP_01)));
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitleStudent::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

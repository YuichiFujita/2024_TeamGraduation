//=============================================================================
// 
//  ドレスアップ用プレイヤー処理 [playerDressup.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerDressup.h"
#include "playerManager.h"
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"
#include "sound.h"
#include "fade.h"
#include "dressup_uniform.h"	// ユニフォーム

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE[CPlayer::EBody::BODY_MAX][CPlayer::EHandedness::HAND_MAX] =		// キャラクターファイル
	{
		{
			"data\\TEXT\\character\\dressup\\setup_playerR.txt",
			"data\\TEXT\\character\\dressup\\setup_playerL.txt",
		},
		{
			"data\\TEXT\\character\\dressup\\setup_player_fatR.txt",
			"data\\TEXT\\character\\dressup\\setup_player_fatL.txt",
		},
		{
			"data\\TEXT\\character\\dressup\\setup_player_gariR.txt",
			"data\\TEXT\\character\\dressup\\setup_player_gariL.txt",
		}
	};
}

//==========================================================================
// 関数ポインタ
//==========================================================================

//==========================================================================
// 静的メンバ変数
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerDressUP::CPlayerDressUP(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) :
	CPlayer(typeTeam, typeArea, typeBase, nPriority)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerDressUP::~CPlayerDressUP()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerDressUP::Init()
{
	// 種類の設定
	CObject::SetType(CObject::TYPE_PLAYER);

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE[GetBodyType()][GetHandedness()]);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// カメラの方向向きにする
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// 待機にしておく
	SetState(EState::STATE_NONE);

	// ドレスアップ生成
	CreateDressUp();

	// チームごとのユニフォームにする
	CDressup* pDressUP = GetDressUp_Uniform();
	if (pDressUP != nullptr)
	{
		assert(GetTeam() != CGameManager::SIDE_NONE);
		pDressUP->SetNowIdx((int)GetTeam());
		pDressUP->ReRegist();
	}

	// モーション設定
	int motionIdx = UtilFunc::Transformation::Random(MOTION_WAIT01, MOTION_WAIT01 + (MOTION_MAX - MOTION_WAIT01));
	GetMotion()->ResetPose(motionIdx);
	GetMotion()->Set(motionIdx);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerDressUP::Uninit()
{
	// 終了処理
	CPlayer::Uninit();
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CPlayerDressUP::Kill()
{
	// 動的削除処理
	CPlayer::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerDressUP::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親の更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 位置の補正
	SetPosition(VEC3_ZERO);
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerDressUP::Draw()
{
	// 描画処理
	CPlayer::Draw();
}

//==========================================================================
// 
//  顔アイコンプレイヤー処理 [playerFaceIcon.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerFaceIcon.h"
#include "playerManager.h"
#include "model.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerFaceIcon::CPlayerFaceIcon(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority) :
	CPlayer(typeTeam, typeArea, typeBase, nPriority)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerFaceIcon::~CPlayerFaceIcon()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerFaceIcon::Init()
{
	// キャラ作成
	HRESULT hr = SetCharacter(GetCharaPath(GetBodyType(), GetHandedness()));
	if (FAILED(hr))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// ドレスアップ生成
	CreateDressUp();

#if 0
	// チームごとのユニフォームにする
	CDressup* m_pDressUp_Uniform = GetDressUp_Uniform();
	if (m_pDressUp_Uniform != nullptr)
	{
		CGameManager::ETeamSide typeTeam = GetTeam();
		assert(typeTeam != CGameManager::SIDE_NONE);
		m_pDressUp_Uniform->SetNowIdx((int)typeTeam);
		m_pDressUp_Uniform->ReRegist();
	}
#endif

	// 顔と髪以外の描画をOFFにする
	int nNumModel = GetNumModel() - 1;
	for (int i = 0; i < nNumModel; i++)
	{
		// 顔か髪の場合次へ
		if (i == ID_FACE || i == ID_HAIR) { continue; }

		// パーツの描画をOFFにする
		GetModel(i)->SetEnableDisp(false);
	}

	// モーション再生を停止
	GetMotion()->SetEnableMotion(false);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerFaceIcon::Uninit()
{
	// 終了処理
	CPlayer::Uninit();
}

//==========================================================================
// 削除処理
//==========================================================================
void CPlayerFaceIcon::Kill()
{
	// 削除処理
	CPlayer::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerFaceIcon::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 過去の位置保存
	SetOldPosition(GetPosition());

	// 親の更新処理
	CObjectChara::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 身長オフセットを与える
	switch (GetBodyType())
	{
	case BODY_DEBU:
		SetPosition(MyLib::Vector3(0.0f, 7.0f, 0.0f));
		break;

	case BODY_GARI:
		SetPosition(MyLib::Vector3(0.0f, -15.5f, 0.0f));
		break;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerFaceIcon::Draw()
{
	// 描画処理
	CPlayer::Draw();
}

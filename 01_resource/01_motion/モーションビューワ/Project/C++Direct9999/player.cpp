//=============================================================================
// 
//  プレイヤー処理 [player.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "calculation.h"
#include "texture.h"
#include "Xload.h"
#include "model.h"
#include "motion.h"
#include "objectChara.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define JUMP			(20.0f * 1.5f)	// ジャンプ力初期値
#define MAX_LIFE		(5)				// 体力
#define OFFSET_MOVE		(1.0f)
#define SETUP_MOVE		(0.5f)
#define SCALE_MOVE		(0.01f)
#define SPHERE_LIFE		(6)

namespace
{
	const float MIN_ROT = -D3DX_PI * 10.0f;
	const float MAX_ROT = D3DX_PI * 10.0f;
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayer::CPlayer(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;	// 状態
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// マテリアルの色
	m_bJump = false;		// ジャンプ中かどうか
	m_bMove = false;		// 移動中かどうか
	m_bATK = false;			// 攻撃中かどうか
	m_bMWait = false;		// 魔法待機中かどうか
	m_nCntState = 0;		// 状態遷移カウンター
	m_nLife = 0;			// 体力
	m_nTexIdx = 0;			// テクスチャのインデックス番号
	m_nIdxXFile = 0;		// Xファイルのインデックス番号
	m_pMotion = NULL;		// モーションの情報

	m_nNowParts = 0;		// 現在のパーツ
	m_nNowMotion = 0;		// 現在のモーション
	m_nNowKey = 0;			// 現在のキー
	m_nNowAttackIdx = 0;	// 現在の攻撃インデックス番号
	m_fSetUpBuff = 0.0f;	// セットアップバフ
	m_nNowColliderIdx = 0;	// 現在のコライダーインデックス番号
	memset(&m_CopyData, 0, sizeof(CMotion::Key));	// コピー用パーツデータ
	m_bStop = false;		// ストップするか

#if ACTION_MODE
	m_Shotmode = SHOTMODE_TRIGGER;	// 射撃の種類

#else
	m_Shotmode = SHOTMODE_NONE;	// 射撃の種類
#endif
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
CPlayer *CPlayer::Create(const std::string pTextFile)
{
	// 生成用のオブジェクト
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULLだったら

		// メモリの確保
		pPlayer = DEBUG_NEW CPlayer;

		if (pPlayer->GetID() < 0)
		{// メモリ確保に失敗していたら

			delete pPlayer;
			return NULL;
		}

		if (pPlayer != NULL)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pPlayer->Init(pTextFile);
		}

		return pPlayer;
	}

	return NULL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayer::Init(void)
{
	return E_FAIL;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayer::Init(const std::string pTextFile)
{
	// 種類の設定
	SetType(TYPE_PLAYER);

	m_state = STATE_NONE;	// 状態
	m_nCntState = 0;		// 状態遷移カウンター
	m_nLife = MAX_LIFE;		// 体力
	m_fSetUpBuff = 1.0f;	// セットアップバフ

	// キャラ作成
	HRESULT hr = SetCharacter(pTextFile);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// モーションの生成処理
	m_pMotion = CMotion::Create();

	// モーションのファイル読み込み
	m_pMotion->ReadText(pTextFile.c_str());

	// オブジェクトキャラクターの情報取得
	CObjectChara *pObjChar = GetObjectChara();

	// モーションの設定
	m_pMotion->SetModel(pObjChar->GetModel(), pObjChar->GetNumModel(), CObjectChara::GetObjectChara());

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 影の生成
	m_pShadow = CShadow::Create(pos, 50.0f);

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// パーツ数取得
	int nPartsNum = GetObjectChara()->GetNumModel();

	// 情報上書き
	aInfo = m_pMotion->GetInfo(m_nNowMotion);

	for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
	{
		// 今回のモデル取得
		CModel *pModel = GetObjectChara()->GetModel()[nCntParts];
		if (pModel == NULL)
		{
			continue;
		}

		// 保存されている向きに設定
		pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Uninit(void)
{
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// 影を消す
	if (m_pShadow != NULL)
	{
		//m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// 終了処理
	CObjectChara::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayer::Update(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{// F5でリセット
		SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		CManager::GetCamera()->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -0.2f));	// 向き
	}

	CObjectChara::Update();

	// テキスト
	ImGui::Text("Change Edit Mode");
	if (ImGui::Button("Edit"))
	{
		CManager::SetEditType(CManager::EDITTYPE_EDIT);
	}
	ImGui::SameLine();
	if (ImGui::Button("Play"))
	{
		CManager::SetEditType(CManager::EDITTYPE_PLAY);
	}
	ImGui::SameLine();
	if (ImGui::Button("SetUp"))
	{
		CManager::SetEditType(CManager::EDITTYPE_SETUP);
		// リセット
		for (int nCntParts = 0; nCntParts < GetNumModel(); nCntParts++)
		{
			// 今回のモデル取得
			CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
			if (pModel == nullptr)
			{
				continue;
			}
			pModel->SetRotation(mylib_const::DEFAULT_VECTOR3);
			pModel->SetPosition(pModel->GetOriginPotision());
			pModel->SetScale({ 1.0f, 1.0f, 1.0f });
		}
	}
	ImGui::Separator();

#if 0
	if (ImGui::BeginMenu("Change Edit Type"))
	{
		const char* names[] = { "Edit", "Play", "SetUp" };
		int selectedIndex = 0;

		for (int i = 0; i < IM_ARRAYSIZE(names); i++)
		{
			if (ImGui::MenuItem(names[i]))
			{
				selectedIndex = i;
				CManager::SetEditType(static_cast<CManager::EDITTYPE>(selectedIndex));
			}
		}
		ImGui::EndMenu();
	}
#endif

	if (CManager::GetEditType() != CManager::EDITTYPE_PLAY)
	{// 操作状態なら
		return;
	}


	if (pInputKeyboard->GetTrigger(DIK_P) == true)
	{// F3でワイヤーフレーム切り替え
		m_bStop = m_bStop ? false : true;
	}

	SetOldPosition(GetPosition());

	// 操作
	Controll();

	// 攻撃情報設定
	SetAttackInfo();

	// モーションの設定処理
	MotionSet();

	// モーションの更新処理
	if (m_bStop == false)
	{
		m_pMotion->Update();
	}
	else
	{

		// モーションの情報取得
		CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

		// 攻撃情報の総数取得
		int nNumAttackInfo = aInfo.nNumAttackInfo;

		for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
		{
			if (aInfo.AttackInfo[nCntAttack] == NULL)
			{// NULLだったら
				continue;
			}

			// モデル情報取得
			CModel **pModel = GetObjectChara()->GetModel();
			D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

			// 武器のマトリックス取得
			D3DXMATRIX mtxWepon;
			D3DXMatrixIdentity(&mtxWepon);

			if (pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum] == NULL)
			{// NULLだったら
				return;
			}

			// 判定するパーツのマトリックス取得
			mtxWepon = pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum]->GetWorldMtx();

			// 位置を反映する
			D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z);
			D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

			// 武器の位置
			D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

			D3DXCOLOR col;
			if (nCntAttack == m_nNowAttackIdx)
			{
				col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			CEffect3D::Create(weponpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), col, aInfo.AttackInfo[nCntAttack]->fRangeSize, SPHERE_LIFE, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		}
	}

	// 影の位置更新
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPosition(GetPosition());
	}

	// 頂点情報設定
	SetVtx();

	// 攻撃処理
	Atack();

	// 状態更新
	UpdateState();

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayer::Controll(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CManager::GetInputGamepad();

	// カメラの情報取得
	CCamera *pCamera = CManager::GetCamera();

	// カメラの向き取得
	D3DXVECTOR3 Camerarot = pCamera->GetRotation();

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 目標の向き取得
	float fRotDest = GetRotDest();

	// 現在の種類取得
	int nType = m_pMotion->GetType();

	if (m_pMotion->IsGetMove(nType) == 1 ||
		(m_pMotion->IsGetMove(nType) == 0 && m_pMotion->IsGetCancelable()))
	{// 移動可能モーションの時

		// 移動量取得
		float fMove = GetVelocity();

		if (nType == MOTION_SPEED_RUSH_MOVE)
		{// 移動ラッシュ中
			fMove *= 0.6f;
		}

		if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(0).x < 0)
		{//←キーが押された,左移動

			// 移動中にする
			m_bMove = true;

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//A+W,左上移動

				move.x += sinf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//A+S,左下移動

				move.x += sinf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//A,左移動

				move.x += sinf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(0).x > 0)
		{//Dキーが押された,右移動

			// 移動中にする
			m_bMove = true;

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//D+W,右上移動

				move.x += sinf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.25f + rot.z + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//D+S,右下移動

				move.x += sinf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.75f + rot.z + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//D,右移動

				move.x += sinf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.5f + rot.z + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
		{//Wが押された、上移動

			// 移動中にする
			m_bMove = true;

			move.x += sinf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.0f + rot.z + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 1.0f + rot.z + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
		{//Sが押された、下移動

			// 移動中にする
			m_bMove = true;

			move.x += sinf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 1.0f + rot.z + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 0.0f + rot.z + Camerarot.y;
		}
		else
		{
			// 移動中かどうか
			m_bMove = false;
		}

		if (m_bMove &&
			(m_pMotion->IsGetMove(nType) == 0 && m_pMotion->IsGetCancelable()))
		{// キャンセル
			m_pMotion->SetFinish(true);
		}

		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
		{//SPACEが押された,ジャンプ

			if (m_bJump == false)
			{//ジャンプ中じゃないとき

				m_bJump = true;
				move.y += JUMP;
			}
		}
	}
	else
	{
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputGamepad->GetStickMoveL(0).x < 0)
		{//←キーが押された,左移動

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//A+W,左上移動
				fRotDest = D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//A+S,左下移動
				fRotDest = D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//A,左移動
				fRotDest = D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputGamepad->GetStickMoveL(0).x > 0)
		{//Dキーが押された,右移動

			if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
			{//D+W,右上移動
				fRotDest = -D3DX_PI * 0.75f + rot.z + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
			{//D+S,右下移動
				fRotDest = -D3DX_PI * 0.25f + rot.z + Camerarot.y;
			}
			else
			{//D,右移動
				fRotDest = -D3DX_PI * 0.5f + rot.z + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true || pInputGamepad->GetStickMoveL(0).y > 0)
		{//Wが押された、上移動
			fRotDest = D3DX_PI * 1.0f + rot.z + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputGamepad->GetStickMoveL(0).y < 0)
		{//Sが押された、下移動
			fRotDest = D3DX_PI * 0.0f + rot.z + Camerarot.y;
		}
	}



	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_bATK == false)
	{// 攻撃

		// 攻撃判定ON
		m_bATK = true;
	}

	//現在と目標の差分を求める
	float fRotDiff = fRotDest - rot.y;

	//角度の正規化
	RotNormalize(fRotDiff);

	//角度の補正をする
	rot.y += fRotDiff * 0.1f;

	//角度の正規化
	RotNormalize(rot.y);

	// 重力処理
	move.y -= mylib_const::GRAVITY;

	// 位置更新
	pos += move;

	// 慣性補正
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);

	// 目標の向き設定
	SetRotDest(fRotDest);

	//**********************************
	// 当たり判定
	//**********************************
	Collision();
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::MotionSet(void)
{
	if (m_pMotion->IsFinish() == true)
	{// 終了していたら

		// 現在の種類取得
		int nType = m_pMotion->GetType();

		if (m_pMotion->GetInfo(nType).nLoop == 1 &&
			nType != MOTION_DEF &&
			nType != MOTION_WALK)
		{
			return;
		}

		if (m_bMove == true && m_bATK == false)
		{// 移動していたら

			m_bMove = false;	// 移動判定OFF

			// 移動モーション
			m_pMotion->Set(MOTION_WALK);
		}
		else if (m_bMWait == true)
		{// 待機中だったら

			// 待機モーション
			m_pMotion->Set(MOTION_MAGIC_WAIT);
		}
		else if (m_bATK == true)
		{// 攻撃していたら

			m_bATK = false;		// 攻撃判定OFF

			m_pMotion->Set(m_nNowMotion);
		}
		else
		{
			// ニュートラルモーション
			m_pMotion->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// 攻撃
//==========================================================================
void CPlayer::Atack(void)
{

	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion->GetInfo(m_pMotion->GetType());

	// 攻撃情報の総数取得
	int nNumAttackInfo = aInfo.nNumAttackInfo;

	for (int nCntAttack = 0; nCntAttack < nNumAttackInfo; nCntAttack++)
	{
		if (aInfo.AttackInfo[nCntAttack] == NULL)
		{// NULLだったら
			continue;
		}

		if (m_pMotion->GetAllCount() == aInfo.AttackInfo[nCntAttack]->nInpactCnt)
		{// 衝撃のカウントと同じとき

			D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

			if (GetObjectChara()->GetModel()[aInfo.AttackInfo[nCntAttack]->nCollisionNum] == NULL)
			{// NULLだったら
				return;
			}

			// 判定するパーツのマトリックス取得
			D3DXMATRIX mtxWepon = GetObjectChara()->GetModel()[aInfo.AttackInfo[nCntAttack]->nCollisionNum]->GetWorldMtx();

			// 位置を反映する
			D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z);
			D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

			// 武器の位置
			D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

			// パーティクル生成
			CManager::GetParticle()->Create(weponpos, CParticle::TYPE_OFFSETTING);
		}

		// モーションカウンター取得
		if (m_pMotion->GetAllCount() > aInfo.AttackInfo[nCntAttack]->nMinCnt && m_pMotion->GetAllCount() < aInfo.AttackInfo[nCntAttack]->nMaxCnt)
		{// 攻撃判定中

			// モデル情報取得
			CModel **pModel = GetObjectChara()->GetModel();
			D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

			// 武器のマトリックス取得
			D3DXMATRIX mtxWepon;
			D3DXMatrixIdentity(&mtxWepon);

			if (pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum] == NULL)
			{// NULLだったら
				return;
			}

			// 判定するパーツのマトリックス取得
			mtxWepon = pModel[aInfo.AttackInfo[nCntAttack]->nCollisionNum]->GetWorldMtx();

			// 位置を反映する
			D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z);
			D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

			// 武器の位置
			D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

			CEffect3D::Create(weponpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), aInfo.AttackInfo[nCntAttack]->fRangeSize, 4, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}
	}
}

//==========================================================================
// 当たり判定
//==========================================================================
void CPlayer::Collision(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 補正
	/*ValueNormalize(pos.x, SCREEN_WIDTH - size.x, 0.0f + size.x);
	ValueNormalize(pos.y, (float)SCREEN_HEIGHT, 0.0f + size.y);*/

	// 着地したかどうか
	bool bLand = false;

	// 位置取得
	D3DXVECTOR3 posOld = GetPosition();

	//// 高さを取得
	//CElevation *pMeshF = CManager::GetObject3DMesh();
	//pos.y = pMeshF->GetHeight(GetPosition(), bLand);

	//if (posOld.y != pos.y)
	//{
	//	// ジャンプ使用可能にする
	//	m_bJump = false;
	//	bLand = true;	// 着地状態
	//	move.y = 0.0f;
	//}

	if (pos.y <= 0.0f)
	{// 地面に付いた

		bLand = true;	// 着地状態
		pos.y = 0.0f;
		move.y = 0.0f;

		// 位置設定
		SetPosition(pos);
	}

	if (bLand == true)
	{// 着地してたら

		// ジャンプ使用可能にする
		m_bJump = false;
		move.y = 0.0f;
	}
	
	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);
}


//==========================================================================
// ヒット処理
//==========================================================================
bool CPlayer::Hit(const int nValue)
{
	if (m_state != STATE_DEAD && m_state != STATE_DMG)
	{// ダメージ受付状態の時

		// 体力減らす
		m_nLife -= nValue;

		if (m_nLife <= 0)
		{// 体力がなくなったら

			// 死状態
			m_state = STATE_DEAD;

			// 終了処理
			Uninit();

			// 死んだ
			return true;
		}

		// 補正
		ValueNormalize(m_nLife, MAX_LIFE, 0);

		// ダメージ状態にする
		m_state = STATE_DMG;

		// 遷移カウンター設定
		m_nCntState = 10;
	}

	// 死んでない
	return false;
}


//==========================================================================
// 状態更新処理
//==========================================================================
void CPlayer::UpdateState(void)
{
	// 位置取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量取得
	D3DXVECTOR3 move = GetMove();

	// 向き取得
	D3DXVECTOR3 rot = GetRotation();

	// 目標の向き取得
	float fRotDest = GetRotDest();

	// 移動量取得
	float fMove = GetVelocity();

	// 現在と目標の差分
	float fRotDiff = 0.0f;

	// 現在の種類取得
	int nType = m_pMotion->GetType();

	switch (m_state)
	{
	case STATE_NONE:
		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case STATE_DMG:

		// 状態遷移カウンター減算
		m_nCntState--;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		if (m_nCntState <= 0)
		{// 遷移カウンターが0になったら

			// 親追い掛け状態にする
			m_state = STATE_NONE;
			m_nCntState = 0;
		}
		break;

	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);

	// 目標の向き設定
	SetRotDest(fRotDest);
}

//==========================================================================
// 向き設定
//==========================================================================
void CPlayer::SetPartsRot(void)
{
#define REPEAT		(2)	// リピートのカウント
#define ROT_MOVE	(D3DX_PI * 0.01f)

	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// エディットタイプ取得
	CManager::EDITTYPE editType = CManager::GetEditType();

	// パーツ数取得
	int nPartsNum = GetObjectChara()->GetNumModel();

	// モーション総数
	int nMotionNum = m_pMotion->GetNumMotion();

	// 今回のモデル取得
	CModel* pModel = GetObjectChara()->GetModel()[m_nNowParts];

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);


	// 向き取得
	D3DXVECTOR3 scale = pModel->GetScale();
	if (editType == CManager::EDITTYPE_EDIT && pInputKeyboard->GetRepeat(DIK_R, REPEAT) == true)
	{// スケール拡大
		scale.x += SCALE_MOVE;
		scale.y += SCALE_MOVE;
		scale.z += SCALE_MOVE;
	}
	else if (editType == CManager::EDITTYPE_EDIT && pInputKeyboard->GetRepeat(DIK_F, REPEAT) == true)
	{// スケール縮小
		scale.x -= SCALE_MOVE;
		scale.y -= SCALE_MOVE;
		scale.z -= SCALE_MOVE;
	}
	if (pInputKeyboard->GetTrigger(DIK_V) == true)
	{// スケールリセット
		scale = mylib_const::DEFAULT_SCALE;
	}
	pModel->SetScale(scale);

	// デバッグ情報
	CManager::GetDebugProc()->Print("[F3]：ポーズ登録\n");
	CManager::GetDebugProc()->Print("[F5]：プレイヤー配置リセット\n");
	CManager::GetDebugProc()->Print("[F6]：ポーズコピー\n");
	CManager::GetDebugProc()->Print("[F7]：ポーズペースト\n");

	if (editType == CManager::EDITTYPE_EDIT)
	{
		CManager::GetDebugProc()->Print("[F9]：モーション情報セーブ[data/TEXT/key_info.txt]\n");
	}
	else if (editType == CManager::EDITTYPE_SETUP)
	{
		CManager::GetDebugProc()->Print("[F9]：セットアップ情報セーブ[data/TEXT/setup_info.txt]\n");
	}
	CManager::GetDebugProc()->Print("\n");

	CManager::GetDebugProc()->Print("モーション切り替え：[ ← ] [ → ] 【%d / %d】\n", m_nNowMotion, nMotionNum);
	CManager::GetDebugProc()->Print("現在のキー切り替え：[ １ ] [ ２ ] 【%d / %d】\n", m_nNowKey, aInfo.nNumKey);


	//***********************
	// セーブ
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// テキスト
	ImGui::Text("OutPut TextFile");
	
	if (ImGui::Button("Save Motion") ||
		pInputKeyboard->GetTrigger(DIK_F9))
	{
		SaveNowMotion();
	}
	// 反転出力
	ImGui::SameLine();
	ImGui::Checkbox("Save Inverse", &m_bSaveInverse);

	ImGui::Separator();

#if 1
	// 書き出し
	if (ImGui::Button(u8"セーブ"))
	{
		OPENFILENAMEA filename = {};
		char sFilePass[1024] = {};
		// ファイル選択ダイアログの設定
		filename.lStructSize = sizeof(OPENFILENAMEA);
		filename.hwndOwner = NULL;
		filename.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
		filename.lpstrFile = sFilePass;
		filename.nMaxFile = MAX_PATH;
		filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;







		// カレントディレクトリを取得する
		char szCurrentDir[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

		// "data"フォルダの絶対パスを求める
		std::string strDataDir = szCurrentDir;
		strDataDir += "\\data";

		// 存在する場合は、lpstrInitialDirに指定する
		if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
		{
			filename.lpstrInitialDir = strDataDir.c_str();
		}
		else
		{
			// "data"フォルダが存在しない場合は、エラー処理を行うなど
		}





		//// 相対パスを絶対パスに変換する
		//char szInitialDir[MAX_PATH] = {};
		//GetCurrentDirectoryA(MAX_PATH, szInitialDir);
		//PathCombineA(szInitialDir, szInitialDir, ".\\data");
		//PathCanonicalizeA(szInitialDir, szInitialDir);

		//filename.lpstrInitialDir = szInitialDir;

		// ファイル選択ダイアログを表示
		if (GetOpenFileNameA(&filename))
		{
			// 選択されたファイルのパスを表示
			printf("Selected file: %s\n", sFilePass);
		}
		//セーブ
		if (strcmp(&sFilePass[0], "") != 0)
		{
			// セーブする処理
		}
		else
		{
			//MessageBox(hWnd, "Failed to save", "Failed to save", MB_OK);
		}
	}
#endif

	if (editType != CManager::EDITTYPE_SETUP)
	{
		//***********************
		// ポーズ登録
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// テキスト
		ImGui::Text("Controll Pause");
		if (ImGui::Button("Register Pause") ||
			pInputKeyboard->GetTrigger(DIK_F3))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// モーション情報の登録
				m_pMotion->SetInfoSave(m_nNowMotion, m_nNowKey, nCntParts, pModel->GetRotation());
				m_pMotion->SetInfoSavePos(m_nNowMotion, m_nNowKey, nCntParts, pModel->GetPosition() - pModel->GetOriginPosition());
				m_pMotion->SetInfoSaveScale(m_nNowMotion, m_nNowKey, nCntParts, pModel->GetScale());
			}
			m_pMotion->ResetMoveData(m_nNowMotion, m_nNowKey);
		}
		ImGui::SameLine();

		//***********************
		// ポーズコピー
		//***********************
		if (ImGui::Button("Copy Pause"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// コピー
				m_CopyData.aParts[nCntParts].rot = pModel->GetRotation();
				m_CopyData.aParts[nCntParts].pos = pModel->GetPosition();
			}
		}
		ImGui::SameLine();

		//***********************
		// ポーズペースト
		//***********************
		if (ImGui::Button("Paste Pause"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// 保存されている向きに設定
				pModel->SetRotation(m_CopyData.aParts[nCntParts].rot);
				pModel->SetPosition(m_CopyData.aParts[nCntParts].pos);
			}
		}
		ImGui::SameLine();


		//***********************
		// 反転
		//***********************
		if (ImGui::Button("Inverse Pause"))
		{
			// 読み込み情報取得
			Load loadData = GetLoadData(GetIdxFile());


			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == nullptr) continue;


				MyLib::Vector3 rot = aInfo.aKey[m_nNowKey].aParts[nCntParts].rot;
				MyLib::Vector3 pos = aInfo.aKey[m_nNowKey].aParts[nCntParts].pos;

				// 反転出力
				if (loadData.LoadData[nCntParts].side == EPartSide::SIDE_CENTER)
				{
					rot.y *= -1;
					rot.z *= -1;

					pos.x *= -1;
				}
				else
				{
					// 反転するインデックス
					int nIdx = loadData.LoadData[nCntParts].nInverseIdx;

					if (nIdx != -1)
					{
						rot.x = aInfo.aKey[m_nNowKey].aParts[nIdx].rot.x;
						rot.y = -aInfo.aKey[m_nNowKey].aParts[nIdx].rot.y;
						rot.z = -aInfo.aKey[m_nNowKey].aParts[nIdx].rot.z;

						pos.x = -aInfo.aKey[m_nNowKey].aParts[nIdx].pos.x;
						pos.y = aInfo.aKey[m_nNowKey].aParts[nIdx].pos.y;
						pos.z = aInfo.aKey[m_nNowKey].aParts[nIdx].pos.z;
					}
					else
					{
						rot.y *= -1;
						rot.z *= -1;

						pos.x *= -1;
					}
				}

				// 保存されている向きに設定
				pModel->SetRotation(rot);
				pModel->SetPosition(pModel->GetOriginPotision() + pos);
			}
		}
		ImGui::Separator();


		// テキスト
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Text("Change Motion / Key");

		//***********************
		// モーション切り替え
		//***********************
		// [グループ]モーション切り替え
		if (ImGui::CollapsingHeader("ChangeMotion"))
		{
			// [ラジオボタン]モーション切り替え
			for (int i = 0; i < m_pMotion->GetNumMotion(); i++)
			{
				if (ImGui::RadioButton(m_pMotion->GetInfo(i).filename.c_str(), &m_nNowMotion, i))
				{
					// 情報上書き
					aInfo = m_pMotion->GetInfo(m_nNowMotion);

					for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
					{
						// 今回のモデル取得
						CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
						if (pModel == NULL)
						{
							continue;
						}

						// 保存されている向きに設定
						pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
						pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
						pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
					}
				}
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{// パーツ切り替え
			m_nNowMotion = (m_nNowMotion + (nMotionNum - 1)) % nMotionNum;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// 保存されている向きに設定
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{// パーツ切り替え
			m_nNowMotion = (m_nNowMotion + 1) % nMotionNum;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// 保存されている向きに設定
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}


		//***********************
		// 現在のキー変更
		//***********************
		ImGui::SetNextItemWidth(140.0f);
		// [スライダー]現在のキー設定
		if (ImGui::SliderInt("NowKey", &m_nNowKey, 0, aInfo.nNumKey - 1))
		{
			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// 保存されている向きに設定
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_1) == true)
		{// キー切り替え
			m_nNowKey = (m_nNowKey + (aInfo.nNumKey - 1)) % aInfo.nNumKey;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// 保存されている向きに設定
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_2) == true)
		{// キー切り替え
			m_nNowKey = (m_nNowKey + 1) % aInfo.nNumKey;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// 保存されている向きに設定
				pModel->SetRotation(aInfo.aKey[m_nNowKey].aParts[nCntParts].rot);
				pModel->SetPosition(pModel->GetOriginPotision() + aInfo.aKey[m_nNowKey].aParts[nCntParts].pos);
				pModel->SetScale(aInfo.aKey[m_nNowKey].aParts[nCntParts].scale);
			}
		}


		//***********************
		// キー総数変更
		//***********************
		ImGui::SameLine();
		ImGui::SetCursorPosX(250.0f); // ボタンの横位置を設定
		ImGui::SetNextItemWidth(140.0f);

		// [テキストボックス]キーの総数
		ImGui::InputInt("NumKey", &aInfo.nNumKey, 1);
		ValueNormalize(aInfo.nNumKey, MAX_KEY, 1);
		m_pMotion->ChangeKeyNum(m_nNowMotion, aInfo.nNumKey);
		ImGui::Separator();
	}


	if (editType == CManager::EDITTYPE_SETUP)
	{
		//***********************
		// モーション切り替え
		//***********************
		if (pInputKeyboard->GetTrigger(DIK_4) == true)
		{// キーフレーム増加

			// 加算
			m_fSetUpBuff += 0.1f;
		}
		else if (pInputKeyboard->GetTrigger(DIK_3) == true)
		{// キーフレーム減少

			// 減算
			m_fSetUpBuff -= 0.1f;
		}

		// もしくはテキストボックスを使った変数の調整
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputFloat("Multipy SetUp", &m_fSetUpBuff, 0.1f, 1.0f);
		ImGui::Separator();

		// 値の正規化
		if (m_fSetUpBuff < 0.0f)
		{
			m_fSetUpBuff = 0.1f;
		}

		CManager::GetDebugProc()->Print("セットアップ倍率：【%f】\n", m_fSetUpBuff);
	}


	//***********************
	// 向き設定
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// [グループ]向き設定
	float windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Rotation"))
	{
		// リセット
		if (ImGui::Button("ALL RESET"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == nullptr)
				{
					continue;
				}
				pModel->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}

		// [スライダー]向き設定
		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
		{
			// 今回のモデル取得
			CModel* pModel = GetModel()[nCntParts];
			if (pModel == nullptr)
			{
				continue;
			}

			// 向き取得
			D3DXVECTOR3 rot = pModel->GetRotation();

			ImGui::Text(pModel->GetModelName().c_str());
			ImGui::SameLine();

			// Xの向き
			ImGui::PushID(nCntParts); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ValueNormalize(rot.x, MAX_ROT, MIN_ROT);
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
				ValueNormalize(rot.y, MAX_ROT, MIN_ROT);
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ValueNormalize(rot.z, MAX_ROT, MIN_ROT);
			}
			ImGui::PopID();

			// 向き設定
			pModel->SetRotation(rot);
		}
	}


	//***********************
	// 位置設定
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// [グループ]位置設定
	windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Position"))
	{
		// リセット
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		if (ImGui::Button("ALL RESET"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == nullptr)
				{
					continue;
				}
				pModel->SetPosition(pModel->GetOriginPotision());
			}
		}
		ImGui::PopID();

		// [スライダー]位置設定
		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
		{
			// 今回のモデル取得
			CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
			if (pModel == NULL)
			{
				continue;
			}

			// 位置取得
			D3DXVECTOR3 pos = pModel->GetPosition() - pModel->GetOriginPosition();

			ImGui::Text(pModel->GetModelName().c_str());
			ImGui::SameLine();

			// Xの向き
			ImGui::PushID(nCntParts + nPartsNum); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &pos.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts + nPartsNum); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &pos.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			ImGui::PushID(nCntParts + nPartsNum); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &pos.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// 位置設定
			pModel->SetPosition(pos + pModel->GetOriginPosition());
		}
	}


	//***********************
	// スケール設定
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// [グループ]スケール設定
	windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Scale"))
	{
		// リセット
		if (ImGui::Button("ALL RESET"))
		{
			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == nullptr)
				{
					continue;
				}
				pModel->SetScale({1.0f, 1.0f, 1.0f});
			}
		}

		// [スライダー]スケール設定
		for (int nCntParts = 0, nID = nPartsNum + nPartsNum; nCntParts < nPartsNum; nCntParts++, nID++)
		{
			// 今回のモデル取得
			CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
			if (pModel == NULL)
			{
				continue;
			}

			// 位置取得
			D3DXVECTOR3 scale = pModel->GetScale();

			ImGui::Text(pModel->GetModelName().c_str());
			ImGui::SameLine();

			// Xの向き
			ImGui::PushID(nID); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat3("scale", (float*)&scale, 0.01f, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// 位置設定
			pModel->SetScale(scale);
		}
	}

#if 0
	//***********************
	// デバッグ表示
	//***********************
	CManager::GetDebugProc()->Print("\n------------------[ エディット情報 ]------------------\n");

	D3DXVECTOR3 posdebug = pModel->GetPosition() - pModel->GetOriginPosition();
	D3DXVECTOR3 posscale = pModel->GetScale();

	CManager::GetDebugProc()->Print(
		"≫位置     [%d]：（%f, %f, %f）≪\n"
		"≫スケール [%d]：（%f, %f, %f）≪\n", m_nNowParts, posdebug.x, posdebug.y, posdebug.z, m_nNowParts, posscale.x, posscale.y, posscale.z);


	/*if (editType == CManager::EDITTYPE_SETUP)
	{
		D3DXVECTOR3 pos = GetObjectChara()->GetModel()[nNow]->GetPosition();

		CManager::GetDebugProc()->Print("\n-----------[ 位置情報 ]-----------\n");
		CManager::GetDebugProc()->Print(
			"≫ [%d]：（%f, %f, %f）≪\n", m_nNowParts, pos.x, pos.y, pos.z);
		nPartsNum = 3;
	}*/

	if (editType == CManager::EDITTYPE_SETUP)
	{
		for (int nCnt = 0; nCnt < nPartsNum; nCnt++)
		{
			// 今回のモデル取得
			CModel* pModel = GetObjectChara()->GetModel()[nCnt];
			if (pModel == NULL)
			{
				continue;
			}

			// 向き取得
			D3DXVECTOR3 pos;

			if (pModel != NULL)
			{// 今回のモデルがNULLじゃなかったら

				// 向き取得
				pos = pModel->GetPosition();
			}
			else
			{
				pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			// デバッグ情報
			CManager::GetDebugProc()->Print(
				" [%d]：(%f, %f, %f)\n", nCnt, pos.x, pos.y, pos.z);
		}
	}

	CManager::GetDebugProc()->Print("\n-----------[ 向き情報 ]-----------\n");
	for (int nCnt = 0; nCnt < nPartsNum; nCnt++)
	{
		// 今回のモデル取得
		CModel* pModel = GetObjectChara()->GetModel()[nCnt];
		if (pModel == NULL)
		{
			continue;
		}

		// 向き取得
		D3DXVECTOR3 rotParts;

		if (pModel != NULL)
		{// 今回のモデルがNULLじゃなかったら

			// 向き取得
			rotParts = pModel->GetRotation();
		}
		else
		{
			rotParts = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		// デバッグ情報
		CManager::GetDebugProc()->Print(
			" [%d]：(%f, %f, %f)\n", nCnt, rotParts.x, rotParts.y, rotParts.z);
	}
#endif
}

//==========================================================================
// 攻撃情報設定
//==========================================================================
void CPlayer::SetAttackInfo(void)
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// パーツ数取得
	int nPartsNum = GetObjectChara()->GetNumModel();

	// モーション総数
	int nMotionNum = m_pMotion->GetNumMotion();

	// 現在の種類取得
	int nType = m_pMotion->GetType();

	CManager::GetDebugProc()->Print("\n++----------------------------------------------++\n");

	if (nType == 0)
	{
		CManager::GetDebugProc()->Print(" MOTION：[%d] DEFAULT\n", nType);
	}
	else if (nType == 1)
	{
		CManager::GetDebugProc()->Print(" MOTION：[%d] WALK\n", nType);
	}
	else if (nType == 2)
	{
		CManager::GetDebugProc()->Print(" MOTION：[%d] ATTACK\n", nType);
	}

	CManager::GetDebugProc()->Print(
		" KEY   ：[%d / %d]\n"
		" FRAME ：[%d / %d] [%d / %d]\n"
		"++----------------------------------------------++\n",
		m_pMotion->GetNowPattern(), aInfo.nNumKey, 
		m_pMotion->GetFrameCount(), aInfo.aKey[m_pMotion->GetNowPattern()].nFrame, 
		m_pMotion->GetAllCount(), aInfo.nMaxAllFrame);

	CManager::GetDebugProc()->Print("モーション切り替え【%d / %d】\n", m_nNowMotion, nMotionNum);
	CManager::GetDebugProc()->Print("現在のキー切り替え【%d / %d】\n", m_nNowKey, aInfo.nNumKey);
	CManager::GetDebugProc()->Print("現在のキーフレーム変更【FRAME：%d】\n", aInfo.aKey[m_nNowKey].nFrame);
	CManager::GetDebugProc()->Print("キャンセル可能フレーム変更【FRAME：%d】\n", aInfo.nCancelableFrame);
	CManager::GetDebugProc()->Print("コンボ可能フレーム変更【FRAME：%d】\n", aInfo.nCombolableFrame);
	CManager::GetDebugProc()->Print("[P]：ポーズ状態切り替え\n");
	CManager::GetDebugProc()->Print("[F7]：カメラ追従切り替え\n");


	// ウィンドウ生成
	//ImGui::CollapsingHeader("AttackInfo");
	{
		//***********************
		// モーション切り替え
		//***********************
		// [グループ]モーション切り替え
		if (ImGui::CollapsingHeader("ChangeMotion"))
		{
			// [ラジオボタン]モーション切り替え
			for (int i = 0; i < m_pMotion->GetNumMotion(); i++)
			{
				if (ImGui::RadioButton(m_pMotion->GetInfo(i).filename.c_str(), &m_nNowMotion, i))
				{
					// 情報上書き
					aInfo = m_pMotion->GetInfo(m_nNowMotion);
					m_pMotion->Set(m_nNowMotion);
					m_nNowAttackIdx = 0;	// 攻撃のインデックスリセット
				}
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{// パーツ切り替え
			m_nNowMotion = (m_nNowMotion + (nMotionNum - 1)) % nMotionNum;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
			m_pMotion->Set(m_nNowMotion);
			m_nNowAttackIdx = 0;	// 攻撃のインデックスリセット
		}
		else if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{// パーツ切り替え
			m_nNowMotion = (m_nNowMotion + 1) % nMotionNum;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
			m_pMotion->Set(m_nNowMotion);
			m_nNowAttackIdx = 0;	// 攻撃のインデックスリセット
		}

		if (m_nNowMotion == 1)
		{
			m_bMove = true;
		}


		//***********************
		// 現在のキー変更
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(140.0f);
		// [スライダー]現在のキー設定
		if (ImGui::SliderInt("NowKey", &m_nNowKey, 0, aInfo.nNumKey - 1))
		{
			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);

			for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
			{
				// 今回のモデル取得
				CModel* pModel = GetObjectChara()->GetModel()[nCntParts];
				if (pModel == NULL)
				{
					continue;
				}

				// 情報上書き
				aInfo = m_pMotion->GetInfo(m_nNowMotion);
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_1) == true)
		{// キー切り替え
			m_nNowKey = (m_nNowKey + (aInfo.nNumKey - 1)) % aInfo.nNumKey;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
		}
		else if (pInputKeyboard->GetTrigger(DIK_2) == true)
		{// キー切り替え
			m_nNowKey = (m_nNowKey + 1) % aInfo.nNumKey;

			// 情報上書き
			aInfo = m_pMotion->GetInfo(m_nNowMotion);
		}
		ImGui::SameLine();


		//***********************
		// キーフレーム変更
		//***********************
		ImGui::SetCursorPosX(250.0f); // ボタンの横位置を設定
		ImGui::SetNextItemWidth(140.0f);
		// [テキストボックス]キー再生フレームの総数
		ImGui::InputInt("Key Frame", &aInfo.aKey[m_nNowKey].nFrame, 1, 10);
		ValueNormalize(aInfo.aKey[m_nNowKey].nFrame, 99999, 1);

		// キー再生フレーム変更
		m_pMotion->ChangeKeyFrame(m_nNowMotion, m_nNowKey, aInfo.aKey[m_nNowKey].nFrame);

		if (pInputKeyboard->GetRepeat(DIK_UP, REPEAT) == true)
		{// キーフレーム増加

			// 加算
			aInfo.aKey[m_nNowKey].nFrame++;

			// 値の正規化
			ValueNormalize(aInfo.aKey[m_nNowKey].nFrame, 99999, 1);

			// キー数変更
			m_pMotion->ChangeKeyFrame(m_nNowMotion, m_nNowKey, aInfo.aKey[m_nNowKey].nFrame);
		}
		else if (pInputKeyboard->GetRepeat(DIK_DOWN, REPEAT) == true)
		{// キーフレーム減少

			// 減算
			aInfo.aKey[m_nNowKey].nFrame--;

			// 値の正規化
			ValueNormalize(aInfo.aKey[m_nNowKey].nFrame, 99999, 1);

			// キー数変更
			m_pMotion->ChangeKeyFrame(m_nNowMotion, m_nNowKey, aInfo.aKey[m_nNowKey].nFrame);
		}


		//***********************
		// キャンセル可能フレーム変更
		//***********************
		// [スライダー]キャンセル可能フレーム設定
		if (ImGui::SliderInt("Cancelable Frame", &aInfo.nCancelableFrame, -1, aInfo.nMaxAllFrame))
		{
			// キャンセル可能フレーム変更
			m_pMotion->ChangeCancelableFrame(m_nNowMotion, aInfo.nCancelableFrame);
		}


		//***********************
		// コンボ可能フレーム変更
		//***********************
		// [スライダー]コンボ可能フレーム設定
		if (ImGui::SliderInt("Combolable Frame", &aInfo.nCombolableFrame, -1, aInfo.nMaxAllFrame))
		{
			// コンボ可能フレーム変更
			m_pMotion->ChangeCombolableFrame(m_nNowMotion, aInfo.nCombolableFrame);
		}
		ImGui::Separator();


		//***********************
		// 攻撃情報の総数
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		// テキスト
		ImGui::Text("AttackInfo Num: %d", aInfo.nNumAttackInfo);
		ImGui::SameLine();

		// [ボタン]攻撃情報の総数
		if (ImGui::Button("Sub"))
		{
			// 総数減算
			m_pMotion->SubNumAttackInfo(m_nNowMotion);
		}
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			// 総数加算
			m_pMotion->AddNumAttackInfo(m_nNowMotion);
		}


		//***********************
		// 現在の攻撃情報切り替え
		//***********************
		if (ImGui::SliderInt("Now AttackInfo", &m_nNowAttackIdx, 0, aInfo.nNumAttackInfo - 1))
		{

		}
		ImGui::Separator();


		//***********************
		// 攻撃判定設定
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		// テキスト
		ImGui::Text("Collision FrameRange");

		bool bSlide = false;

		// [スライダー]攻撃判定の最小値
		ImGui::SetNextItemWidth(130.0f);
		if (ImGui::SliderInt("Min Frame", &aInfo.AttackInfo[m_nNowAttackIdx]->nMinCnt, 0, aInfo.nMaxAllFrame))
		{
			bSlide = true;
		}
		ImGui::SameLine();

		ImGui::SetNextItemWidth(130.0f);
		if (ImGui::SliderInt("MAX Frame", &aInfo.AttackInfo[m_nNowAttackIdx]->nMaxCnt, 0, aInfo.nMaxAllFrame))
		{
			bSlide = true;
		}

		if (bSlide)
		{
			// モデル情報取得
			CModel** pModel = GetObjectChara()->GetModel();

			if (pModel[aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum] != NULL)
			{
				D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

				// 武器のマトリックス取得
				D3DXMATRIX mtxWepon;
				D3DXMatrixIdentity(&mtxWepon);

				// 判定するパーツのマトリックス取得
				mtxWepon = pModel[aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum]->GetWorldMtx();

				// 位置を反映する
				D3DXMatrixTranslation(&mtxTrans, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z);
				D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

				// 武器の位置
				D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

				CEffect3D::Create(
					weponpos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize,
					SPHERE_LIFE,
					CEffect3D::MOVEEFFECT_NONE,
					CEffect3D::TYPE_NORMAL);
			}
		}


		//***********************
		// 衝撃のカウント
		//***********************
		// [スライダー]衝撃のカウント
		if (ImGui::SliderInt("Impact Frame", &aInfo.AttackInfo[m_nNowAttackIdx]->nInpactCnt, -1, aInfo.nMaxAllFrame))
		{

		}


		//***********************
		// 判定のサイズ
		//***********************
		if (ImGui::DragFloat("Collision Radius", &aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize, 1.0f, 0.0f, 0.0f, "%.2f"))
		{
			// 値の正規化
			ValueNormalize(aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize, 9999.0f, 0.0f);
		}

		//***********************
		// 判定を取るパーツ切り替え
		//***********************
		if (ImGui::SliderInt("CollisionDetector", &aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum, 0, nPartsNum))
		{

		}


		//***********************
		// オフセット位置設定
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// [グループ]位置設定
		float windowWidth = 130.0f;
		if (ImGui::CollapsingHeader("Position"))
		{
			// リセット
			if (ImGui::Button("ALL RESET"))
			{
				aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x = 0.0f;
				aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y = 0.0f;
				aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z = 0.0f;
			}

			// [スライダー]位置設定
			// X
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			// Y
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();

			// Z
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		}
	}
	//ImGui::End();





	


	// オフセット設定
	CManager::GetDebugProc()->Print("\n---------------------------------[攻撃情報]---------------------------------\n");
	CManager::GetDebugProc()->Print(" 当たり判定の総数【 NUM：%d 】\n", aInfo.nNumAttackInfo);
	CManager::GetDebugProc()->Print(" 現在の当たり判定【 %d / %d 】\n", m_nNowAttackIdx, aInfo.nNumAttackInfo);
	CManager::GetDebugProc()->Print(" 原点からのオフセット【 %f, %f, %f 】\n", aInfo.AttackInfo[m_nNowAttackIdx]->Offset.x, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.y, aInfo.AttackInfo[m_nNowAttackIdx]->Offset.z);
	CManager::GetDebugProc()->Print(" 判定を取るカウント【 %d ～ %d 】\n", aInfo.AttackInfo[m_nNowAttackIdx]->nMinCnt, aInfo.AttackInfo[m_nNowAttackIdx]->nMaxCnt);
	CManager::GetDebugProc()->Print(" 衝撃のカウント【 %d 】\n", aInfo.AttackInfo[m_nNowAttackIdx]->nInpactCnt);
	CManager::GetDebugProc()->Print(" 判定のサイズ【 %f 】\n", aInfo.AttackInfo[m_nNowAttackIdx]->fRangeSize);
	CManager::GetDebugProc()->Print(" 当たり判定を取るパーツ【 %d 】\n", aInfo.AttackInfo[m_nNowAttackIdx]->nCollisionNum);

	// 攻撃情報設定
	if (aInfo.AttackInfo[m_nNowAttackIdx] != NULL)
	{
		m_pMotion->ChangeAttackInfo(m_nNowMotion, m_nNowAttackIdx, *aInfo.AttackInfo[m_nNowAttackIdx]);
	}
}

//==========================================================================
// 球の当たり判定設定
//==========================================================================
void CPlayer::SetSphereCollider(void)
{

	// パーツ数取得
	int nPartsNum = GetNumModel();

	SphereCollider collider = GetNowSphereCollider(m_nNowColliderIdx);

	//***********************
	// 攻撃情報の総数
	//***********************
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// テキスト
	ImGui::Text("Collider Num: %d", GetNumCollider());
	ImGui::SameLine();

	// [ボタン]攻撃情報の総数
	if (ImGui::Button("Sub"))
	{
		// 総数減算
		SubCollider();
	}
	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		// 総数加算
		AddCollider(SphereCollider());
	}

	//***********************
	// 現在のコライダー切り替え
	//***********************
	if (GetNumCollider() <= 0)
	{
		// テキスト
		ImGui::Text("NO Collider");
	}
	else
	{
		if (ImGui::SliderInt("Now Collider", &m_nNowColliderIdx, 0, GetNumCollider() - 1))
		{
			collider = GetNowSphereCollider(m_nNowColliderIdx);
		}
	}
	ImGui::Separator();



	//***********************
	// 判定のサイズ
	//***********************
	if (ImGui::DragFloat("Collision Radius", &collider.radius, 1.0f, 0.0f, 0.0f, "%.2f"))
	{
		if (collider.radius <= 1.0f)
		{
			collider.radius = 1.0f;
		}
	}

	//***********************
	// 判定を取るパーツ切り替え
	//***********************
	if (ImGui::SliderInt("Collision Detector", &collider.nParentPartsIdx, 0, nPartsNum - 1))
	{

	}

	//***********************
	// オフセット位置設定
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	// [グループ]位置設定
	float windowWidth = 130.0f;
	if (ImGui::CollapsingHeader("Collider Position"))
	{
		// リセット
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		if (ImGui::Button("ALL RESET"))
		{
			collider.offset.x = 0.0f;
			collider.offset.y = 0.0f;
			collider.offset.z = 0.0f;
		}
		ImGui::PopID();

		// [スライダー]位置設定
		// X
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("x", &collider.offset.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Y
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("y", &collider.offset.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Z
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("z", &collider.offset.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
	}



	// 今回のモデル取得
	CModel* pModel = GetObjectChara()->GetModel()[collider.nParentPartsIdx];

	D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

	// 武器のマトリックス取得
	D3DXMATRIX mtxWepon;
	D3DXMatrixIdentity(&mtxWepon);

	// 判定するパーツのマトリックス取得
	mtxWepon = pModel->GetWorldMtx();

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, collider.offset.x, collider.offset.y, collider.offset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	// 武器の位置
	D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

	collider.center = weponpos;


	SetNowSphereCollider(m_nNowColliderIdx, collider);

	for (int i = 0; i < GetNumCollider(); i++)
	{
		SphereCollider collider = GetNowSphereCollider(i);

		// 今回のモデル取得
		CModel* pModel = GetObjectChara()->GetModel()[collider.nParentPartsIdx];

		D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

		// 武器のマトリックス取得
		D3DXMATRIX mtxWepon;
		D3DXMatrixIdentity(&mtxWepon);

		// 判定するパーツのマトリックス取得
		mtxWepon = pModel->GetWorldMtx();

		// 位置を反映する
		D3DXMatrixTranslation(&mtxTrans, collider.offset.x, collider.offset.y, collider.offset.z);
		D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

		// 武器の位置
		D3DXVECTOR3 weponpos = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

		collider.center = weponpos;

		SetNowSphereCollider(i, collider);

		D3DXCOLOR col = mylib_const::DEFAULT_COLOR;
		if (i == m_nNowColliderIdx)
		{
			col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		GetMeshSphere(i)->SetColor(col);
	}
	ImGui::Separator();










	//***********************
	// 中心位置設定
	//***********************
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Text("Center Position");

	int centerID = GetCenterPartsIdx();
	//***********************
	// 中心のパーツ切り替え
	//***********************
	if (ImGui::SliderInt("CenterPartsID", &centerID, 0, nPartsNum - 1))
	{

	}
	SetCenterPartsIdx(centerID);

	D3DXVECTOR3 centerOffset = GetCenterOffset();

	// [グループ]位置設定
	if (ImGui::CollapsingHeader("Center Position"))
	{
		// リセット
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		if (ImGui::Button("ALL RESET"))
		{
			centerOffset.x = 0.0f;
			centerOffset.y = 0.0f;
			centerOffset.z = 0.0f;
		}

		// [スライダー]位置設定
		// X
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("x", &centerOffset.x, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Y
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("y", &centerOffset.y, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		// Z
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::DragFloat("z", &centerOffset.z, SETUP_MOVE, 0.0f, 0.0f, "%.2f");
		ImGui::PopID();
	}
	SetCenterOffset(centerOffset);
}

//==========================================================================
// 現在のモーションセーブ
//==========================================================================
void CPlayer::SaveNowMotion(void)
{

	// エディットタイプ取得
	CManager::EDITTYPE editType = CManager::GetEditType();

	if (editType == CManager::EDITTYPE_EDIT)
	{
		// 現在のモーションセーブ
		SaveMotionInfo();
	}
	else if (editType == CManager::EDITTYPE_SETUP)
	{
		// 現在のセットアップ情報セーブ
		SaveSetUpInfo();
		SaveSphereColliders();
	}
}

//==========================================================================
// 現在のモーションセーブ
//==========================================================================
void CPlayer::SaveMotionInfo(void)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen("data\\TEXT\\key_info.txt", "w");

	if (pFile == NULL)
	{// ファイルが開けなかった場合
		return;
	}

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// パーツ数取得
	int nPartsNum = GetObjectChara()->GetNumModel();

	fprintf(pFile,
		"#==============================================================================\n"
		"#\n"
		"# モーション情報スクリプトファイル[%s]\n"
		"# Author : 相馬 靜雅\n"
		"#\n"
		"#==============================================================================\n", m_pMotion->GetMotionFilename(m_nNowMotion).c_str());

	fprintf(pFile,
		"MOTIONSET\n"
		"\tLOOP = %d\t\t# ループするかどうか[0:ループしない / 1:ループする]\n"
		"\tNUM_KEY = %d\t\t# キー数\n"
		"\tMOVE = %d\t\t# 移動できるかどうか[0:移動できない / 1:移動できる]\n"
		"\tCANCELABLE = %d\t# キャンセル可能フレーム[-1：キャンセル不可]\n"
		"\tCOMBOLABLE = %d\t# コンボ可能フレーム[-1：キャンセル不可]\n\n"
		, aInfo.nLoop, aInfo.nNumKey, aInfo.nMove, aInfo.nCancelableFrame, aInfo.nCombolableFrame);

	fprintf(pFile,
		"\t#===================\n"
		"\t# 攻撃情報\n"
		"\t#===================\n");
	for (int nCntAttack = 0; nCntAttack < aInfo.nNumAttackInfo; nCntAttack++)
	{
		fprintf(pFile,
			"\tATTACKINFO\n"
			"\t\tCOLLISIONPARTS = %d\n"
			"\t\tATTACKRANGE = %.2f\n"
			"\t\tOFFSET = %.2f %.2f %.2f\n"
			"\t\tATTACKCOUNT = %d %d\n"
			"\t\tINPACTCOUNT = %d\n"
			"\t\tDAMAGE = %d\n"
			"\tEND_ATTACKINFO\n\n",
			aInfo.AttackInfo[nCntAttack]->nCollisionNum, aInfo.AttackInfo[nCntAttack]->fRangeSize, aInfo.AttackInfo[nCntAttack]->Offset.x, aInfo.AttackInfo[nCntAttack]->Offset.y, aInfo.AttackInfo[nCntAttack]->Offset.z,
			aInfo.AttackInfo[nCntAttack]->nMinCnt, aInfo.AttackInfo[nCntAttack]->nMaxCnt, aInfo.AttackInfo[nCntAttack]->nInpactCnt, aInfo.AttackInfo[nCntAttack]->nDamage);
	}



	// 読み込み情報取得
	Load loadData = GetLoadData(GetIdxFile());

	for (int nCntKey = 0; nCntKey < aInfo.nNumKey; nCntKey++)
	{
		fprintf(pFile, "\tKEYSET\t\t\t# --- << KEY : %d / %d >> ---\n", nCntKey, aInfo.nNumKey);
		fprintf(pFile, "\t\tFRAME = %d\n", aInfo.aKey[nCntKey].nFrame);

		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++)
		{// パーツ数分繰り返し

			MyLib::Vector3 rot = aInfo.aKey[nCntKey].aParts[nCntParts].rot;
			MyLib::Vector3 pos = aInfo.aKey[nCntKey].aParts[nCntParts].pos;

			// 反転出力
			if (m_bSaveInverse)
			{
				if (loadData.LoadData[nCntParts].side == EPartSide::SIDE_CENTER)
				{
					rot.y *= -1;
					rot.z *= -1;

					pos.x *= -1;
				}
				else
				{
					// 反転するインデックス
					int nIdx = loadData.LoadData[nCntParts].nInverseIdx;

					if (nIdx != -1)
					{
						rot.x = aInfo.aKey[nCntKey].aParts[nIdx].rot.x;
						rot.y = -aInfo.aKey[nCntKey].aParts[nIdx].rot.y;
						rot.z = -aInfo.aKey[nCntKey].aParts[nIdx].rot.z;

						pos.x = -aInfo.aKey[nCntKey].aParts[nIdx].pos.x;
						pos.y = aInfo.aKey[nCntKey].aParts[nIdx].pos.y;
						pos.z = aInfo.aKey[nCntKey].aParts[nIdx].pos.z;
					}
					else
					{
						rot.y *= -1;
						rot.z *= -1;

						pos.x *= -1;
					}
				}
			}

			fprintf(pFile,
				"\t\tPARTS\t# ----- [ %d ] -----\n"
				"\t\t\tROT = %.2f %.2f %.2f\n"
				"\t\t\tPOS = %.2f %.2f %.2f\n"
				"\t\t\tSCALE = %.2f %.2f %.2f\n"
				"\t\tEND_PARTS\n\n", nCntParts,
				rot.x, rot.y, rot.z,
				aInfo.aKey[nCntKey].aParts[nCntParts].pos.x, aInfo.aKey[nCntKey].aParts[nCntParts].pos.y, aInfo.aKey[nCntKey].aParts[nCntParts].pos.z,
				aInfo.aKey[nCntKey].aParts[nCntParts].scale.x, aInfo.aKey[nCntKey].aParts[nCntParts].scale.y, aInfo.aKey[nCntKey].aParts[nCntParts].scale.z);
		}
		fprintf(pFile, "\tEND_KEYSET\n\n");
	}
	fprintf(pFile, "END_MOTIONSET\n\n");

	//ファイルを閉じる
	fclose(pFile);
}

//==========================================================================
// 現在のセットアップ情報セーブ
//==========================================================================
void CPlayer::SaveSetUpInfo(void)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen("data\\TEXT\\setup_info.txt", "w");

	if (pFile == NULL)
	{// ファイルが開けなかった場合
		return;
	}

	// モーションの情報取得
	CMotion::Info aInfo = m_pMotion->GetInfo(m_nNowMotion);

	// パーツ数取得
	int nPartsNum = GetObjectChara()->GetNumModel();

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# キャラクター情報\n"
		"#------------------------------------------------------------------------------\n");

	fprintf(pFile,
		"CHARACTERSET\n\n"
		"\tCENTERSET\n"
		"\t\tPARTS = %d\t\t# 中心にするパーツ\n"
		"\t\tOFFSET = %.2f %.2f %.2f\t\t# オフセット位置\n"
		"\tEND_CENTERSET\n\n"
		"\tVELOCITY = %.2f\t\t# 移動量\n"
		"\tJUMP = %.2f\t\t# ジャンプ量\n"
		"\tRADIUS = %.2f\t\t# 半径\n"
		"\tHEIGHT = %.2f\t\t# 高さ\n"
		"\tLIFE = %d\t\t# 体力\n", 
		GetCenterPartsIdx(),
		GetCenterOffset().x,
		GetCenterOffset().y,
		GetCenterOffset().z,
		GetVelocity(), 0.00f, GetRadius(), 0.00f, 5);



	// 読み込み情報取得
	Load loadData = GetLoadData(GetIdxFile());

	for (int nCntParts = 0; nCntParts < GetObjectChara()->GetNumModel(); nCntParts++)
	{
		// 今回のモデル取得
		CModel *pModel = GetObjectChara()->GetModel()[nCntParts];
		if (pModel == NULL)
		{// NULLで抜ける
			continue;
		}

		// 位置取得
		D3DXVECTOR3 posParts = pModel->GetPosition();
		D3DXVECTOR3 rotParts = pModel->GetRotation();
		posParts *= m_fSetUpBuff;

		loadData.LoadData[nCntParts].nInverseIdx;

		int side = loadData.LoadData[nCntParts].side;
		fprintf(pFile,
			"\n"
			"\tPARTSSET\n"
			"\t\tINDEX = %d\n"
			"\t\tPARENT = %d\n"
			"\t\tSIDE = %d\t\t\t\t\t# 反転[-1:左 / 0 : 中央 / 1 : 右]\n"
			"\t\tINVERSE = %d\t\t\t\t\t# 反転先\n"
			"\t\tPOS = %.2f %.2f %.2f\n"
			"\t\tROT = %.2f %.2f %.2f\n"
			"\t\tSTART = %d\n"
			"\tEND_PARTSSET\n", nCntParts, pModel->GetParentIdx(),
			side,
			loadData.LoadData[nCntParts].nInverseIdx,
			posParts.x, posParts.y, posParts.z,
			rotParts.x, rotParts.y, rotParts.z,
			1);
	}

	fprintf(pFile, "END_CHARACTERSET\n");

	//ファイルを閉じる
	fclose(pFile);
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayer::Draw(void)
{
	int nNowParts = m_nNowParts;
	if (CManager::GetEditType() == CManager::EDITTYPE_PLAY)
	{// プレイ状態なら
		nNowParts = -1;
	}

	CObjectChara::Draw(nNowParts, 1.0f);
}

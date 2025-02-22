//=============================================================================
// 
//  オブジェクトキャラクター処理 [objectChara.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectChara.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"
#include "characterStatus.h"

namespace
{
	const float MUL_LIFE[CEntryRuleManager::ELife::LIFE_MAX] = { 0.5f, 1.0f, 1.5f };	// 体力倍率
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
std::vector<CObjectChara::ColliderData> CObjectChara::m_LoadColliderData = {};	// コライダー情報

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectChara::CObjectChara(int nPriority) : CObjectHierarchy(nPriority)
{
	// 値のクリア
	m_fRotDest = 0.0f;			// 目標の向き
	m_nLife = 0;				// 体力
	m_nLifeOrigin = 0;			// 元の体力
	m_pStatus = nullptr;		// ステータス
	m_pStatusBall = nullptr;	// ステータス(ボール)
	m_bInDicision = false;		// 攻撃判定中フラグ
	m_pMotion = nullptr;		// モーション
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectChara::~CObjectChara()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectChara* CObjectChara::Create(const std::string& pTextFile)
{
	// メモリの確保
	CObjectChara* pObjChara = DEBUG_NEW CObjectChara;

	if (pObjChara != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		HRESULT hr = pObjChara->SetCharacter(pTextFile);
		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// キャラ作成
//==========================================================================
HRESULT CObjectChara::SetCharacter(const std::string& file)
{
	// キャラ作成
	CObjectHierarchy::SetCharacter(file);

	// モーションの生成処理
	m_pMotion = CMotion::Create(file, this);

	// モーションの設定
	if (m_pMotion != nullptr)
	{
		m_pMotion->SetModel(GetModel(), GetNumModel());

		// ポーズのリセット
		m_pMotion->ResetPose(0);
	}

	// スフィアコライダーデータ読み込み
	LoadSphereColliders(file);

	return S_OK;
}

//==========================================================================
// オブジェクト毎のデータ割り当て
//==========================================================================
void CObjectChara::BindObjectData(int nCntData)
{
	// オブジェクト毎のデータ割り当て
	CObjectHierarchy::BindObjectData(nCntData);

	// ステータス生成
	CreateStatus(m_aLoadData[m_nIdxFile].parameter);

	// ステータス(ボール)生成
	CreateStatusBall(m_aLoadData[m_nIdxFile].parameterBall);

	CEntryRuleManager::SRule rule;		// ルール
	CEntryRuleManager::LoadRule(&rule);	// ルール読込
	float fMul = MUL_LIFE[rule.life];	// 体力倍率

	// 体力の設定
	m_nLife = m_aLoadData[m_nIdxFile].parameter.nLife * fMul;
	m_nLifeOrigin = m_nLife;
}

//==========================================================================
// ステータス生成
//==========================================================================
void CObjectChara::CreateStatus(const CCharacterStatus::CharParameter& parameter)
{
	SAFE_DELETE(m_pStatus);

	// 生成
	m_pStatus = DEBUG_NEW CCharacterStatus(parameter);
}

//==========================================================================
// ステータス(ボール)生成
//==========================================================================
void CObjectChara::CreateStatusBall(const CBallStatus::SBallParameter& parameter)
{
	SAFE_DELETE(m_pStatusBall);

	// 生成
	m_pStatusBall = DEBUG_NEW CBallStatus(parameter);
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectChara::Init()
{
	// 初期化処理
	CObjectHierarchy::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectChara::Uninit()
{
	// モーション削除
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// ステータス削除
	SAFE_DELETE(m_pStatus);

	// ステータス(ボール)削除
	SAFE_DELETE(m_pStatusBall);

	// 終了処理
	CObjectHierarchy::Uninit();
}

//==========================================================================
// 動的削除処理
//==========================================================================
void CObjectChara::Kill()
{
	// 動的削除処理
	CObjectHierarchy::Kill();

	// 終了処理
	CObjectChara::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectChara::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// オブジェクトヒエラルキーの更新
	CObjectHierarchy::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// モーション更新
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// コライダーの位置調整
	for (auto& collider : m_SphereColliders)
	{
		// 総数制御
		if (collider.nParentPartsIdx >= GetNumModel()) continue;

		// 判定するパーツ取得
		CModel* pModel = GetModel()[collider.nParentPartsIdx];

		// 判定するパーツのマトリックス取得
		MyLib::Matrix mtxTrans;
		MyLib::Matrix mtxWepon = pModel->GetWorldMtx();

		// 位置を反映する
		mtxTrans.Translation(collider.offset);
		mtxWepon.Multiply(mtxTrans, mtxWepon);
		collider.center = mtxWepon.GetWorldPosition();
	}

	// モーション中の行動処理
	MotionInProgressAction();
}

//==========================================================================
// モーション中の行動処理
//==========================================================================
void CObjectChara::MotionInProgressAction()
{
	// 攻撃判定フラグリセット
	m_bInDicision = false;

	if (m_pMotion == nullptr) return;

	// 情報取得
	std::vector<CMotionManager::AttackInfo> attackInfo = m_pMotion->GetAttackInfo();
	if (attackInfo.empty()) return;

	// 攻撃時処理
	int i = 0;
	for (const auto& attack : attackInfo)
	{
		if (attack.bAtkking)
		{// 攻撃判定中

			// 攻撃判定フラグ
			m_bInDicision = true;

			// 攻撃判定中処理
			AttackInDicision(attack, i);
		}

		if (attack.bInpactAct)
		{// 衝撃のカウントと同じとき

			// 攻撃時処理
			AttackAction(attack, i);
		}

		i++;
	}
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CObjectChara::AttackAction(CMotionManager::AttackInfo ATKInfo, int nCntATK)
{
	// 武器の位置
	MyLib::Vector3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CObjectChara::AttackInDicision(CMotionManager::AttackInfo ATKInfo, int nCntATK)
{
	// 武器の位置
	MyLib::Vector3 weponpos = m_pMotion->GetAttackPosition(GetModel(), ATKInfo);
}

//==========================================================================
// モーションファイル変更
//==========================================================================
void CObjectChara::ChangeMotion(const char* pMotionFile)
{
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// モーションの生成処理
	m_pMotion = CMotion::Create(pMotionFile, this);

	// モーションの設定
	m_pMotion->SetModel(GetModel(), GetNumModel());

	// ポーズのリセット
	m_pMotion->ResetPose(0);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw()
{
	// 描画処理
	CObjectHierarchy::Draw();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(D3DXCOLOR col)
{
	// 描画処理
	CObjectHierarchy::Draw(col);
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(float fAlpha)
{
	// 描画処理
	CObjectHierarchy::Draw(fAlpha);
}

//==========================================================================
// オブジェクト毎のデータ読み込み
//==========================================================================
void CObjectChara::LoadObjectData(FILE* pFile, const std::string& file)
{
	// オブジェクト毎のデータ読み込み
	CObjectHierarchy::LoadObjectData(pFile, file);

	char hoge[MAX_COMMENT];	// コメント

	if (file.find("VELOCITY_NORMAL") != std::string::npos)
	{// 通常移動量

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityNormal);	// 通常移動量
	}

	if (file.find("VELOCITY_RUN") != std::string::npos)
	{// ダッシュ移動量

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityDash);	// ダッシュ移動量
	}

	if (file.find("VELOCITY_BLINK") != std::string::npos)
	{// ブリンク移動量

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityBlink);	// ブリンク移動量
	}

	if (file.find("THROWJUMP_STARTMOVE") != std::string::npos)
	{// ジャンプ投げ時の開始上移動量

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fJumpStartMove);	// ジャンプ投げ時の開始上移動量
	}

	if (file.find("THROWJUMP_UPDATEMOVE") != std::string::npos)
	{// ジャンプ投げ時の常時上移動量

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fJumpUpdateMove);	// ジャンプ投げ時の常時上移動量
	}

	if (file.find("JUMP") != std::string::npos)
	{// ジャンプ量

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fVelocityJump);	// ジャンプ量
	}

	if (file.find("HEIGHT") != std::string::npos)
	{// 身長

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.fHeight);	// 身長
	}

	if (file.find("LIFE") != std::string::npos)
	{// LIFEで体力

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].parameter.nLife);	// 体力
	}

	// ボールのステータス読み込み
	if (file.find("BALLSTATUSSET") != std::string::npos)
	{// BALLSTATUSSETで読み込み

		LoadBallData(pFile, file);
	}

	// ボールのオフセット読み込み
	if (file.find("BALLSET") != std::string::npos)
	{// BALLSETで中心位置読み込み

		while (strcmp(hoge, "END_BALLSET") != 0)
		{// END_BALLSETが来るまで繰り返す

			fscanf(pFile, "%s", &hoge[0]);	//確認する

			if (strcmp(hoge, "PARTS") == 0)
			{// PARTSが来たらパーツ番号読み込み

				fscanf(pFile, "%s", &hoge[0]);	// =の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].parameter.nBallPartsIdx);	// モデル種類の列挙
			}

			if (strcmp(hoge, "OFFSET") == 0)
			{// OFFSETが来たら位置読み込み

				fscanf(pFile, "%s", &hoge[0]);	// =の分
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.ballOffset.x);	// X座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.ballOffset.y);	// Y座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].parameter.ballOffset.z);	// Z座標
			}
		}
	}
}

//==========================================================================
// ボールのステータス読み込み
//==========================================================================
void CObjectChara::LoadBallData(FILE* pFile, const std::string& file)
{
	char hoge[MAX_COMMENT];	// コメント
	CBallStatus::SBallParameter param = CBallStatus::SBallParameter();	// 受け取り用パラメータ
	int nCatchRange = 0;	// キャッチ範囲受け取り

	while (strcmp(hoge, "END_BALLSTATUSSET") != 0)
	{
		fscanf(pFile, "%s", &hoge[0]);	//確認する

		if (strcmp(hoge, "THROW_NORMAL") == 0)
		{// 投げ速度(通常)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%f", &param.fThrowMoveNormal);	// 投げ速度(通常)
		}

		if (strcmp(hoge, "THROW_JUMP") == 0)
			{// 投げ速度(ジャンプ)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%f", &param.fThrowMoveJump);		// 投げ速度(ジャンプ)
		}

		if (strcmp(hoge, "THROW_SPECIAL") == 0)
		{// 投げ速度(スペシャル)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%f", &param.fThrowMoveSpecial);	// 投げ速度(スペシャル)
		}

		if (strcmp(hoge, "CATCH_RANGE") == 0)
		{// キャッチ範囲

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%d", &nCatchRange);
			param.fCatchRange = UtilFunc::Transformation::DegreeChangeToRadian(static_cast<float>(nCatchRange));		// キャッチ範囲
		}

		if (strcmp(hoge, "DAMAGE_NORMAL") == 0)
			{// ダメージ(通常)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%d", &param.nDamageNormal);		// ダメージ(通常)
		}

		if (strcmp(hoge, "DAMAGE_JUMP") == 0)
		{// ダメージ(ジャンプ)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%d", &param.nDamageJump);		// ダメージ(ジャンプ)
		}

		if (strcmp(hoge, "DAMAGE_SPECIAL") == 0)
			{// ダメージ(スペシャル)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%d", &param.nDamageSpecial);		// ダメージ(スペシャル)
		}
	
		//ノックバック
		if (strcmp(hoge, "KNOCKBACK_NORMAL") == 0)
		{// (通常)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%f", &param.fKnockbackNormal);		// ノックバック(通常)
		}

		if (strcmp(hoge, "KNOCKBACK_JUMP") == 0)
		{// (ジャンプ)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%f", &param.fKnockbackJump);			// ノックバック(ジャンプ)
		}

		if (strcmp(hoge, "KNOCKBACK_SPECIAL") == 0)
		{// (スペシャル)

			fscanf(pFile, "%s", &hoge[0]);	// =の分
			fscanf(pFile, "%f", &param.fKnockbackSpecial);		// ノックバック(スペシャル)
		}
	}
	
	m_aLoadData[m_nNumLoad].parameterBall = param;
}

//==========================================================================
// パーツ毎のデータ読み込み
//==========================================================================
void CObjectChara::LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts)
{
	// パーツ毎のデータ読み込み
	CObjectHierarchy::LoadPartsData(pFile, file, pCntParts);
}

//==========================================================================
// スフィアコライダー読み込み
//==========================================================================
void CObjectChara::LoadSphereColliders(const std::string& file)
{
	// 構造体の中の文字列を探す
	auto itr = std::find_if(m_LoadColliderData.begin(), m_LoadColliderData.end(),
		[&file](const CObjectChara::ColliderData& data) {return data.filename == file; });

	// 見つかった場合の処理
	if (itr != m_LoadColliderData.end())
	{
		m_SphereColliders = (*itr).colliders;
		return;
	}


	// ファイルを開く
	FILE* pFile = fopen(file.c_str(), "r");
	if (pFile == nullptr)
	{//ファイルが開けなかった場合
		return;
	}

	std::string filename;

	// 読み込み用変数
	char aComment[MAX_COMMENT];	// コメント用


	// 要素追加
	m_LoadColliderData.emplace_back();

	while (1)
	{
		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// コライダーファイルの読み込み
		if (strcmp(aComment, "COLLIDER_FILENAME") == 0)
		{// COLLIDER_FILENAMEがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%s", &aComment[0]);	// ファイル名

			// ファイル名保存
			filename = aComment;

			m_LoadColliderData.back().filename = filename;
			break;
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);


	// ファイルからJSONを読み込む
	std::ifstream filejson(filename);
	if (!filejson.is_open())
	{
		return;
	}

	nlohmann::json jsonData;
	filejson >> jsonData;	// jsonデータを与える

	// jsonデータから読み込む
	from_json(jsonData);
}

//==========================================================================
// スフィアコライダー書き込み
//==========================================================================
void CObjectChara::SaveSphereColliders()
{
	// ファイルにキャラクターのデータを書き込む
	std::ofstream outFile("character_data.json");
	if (!outFile.is_open())
	{
		return;
	}

	nlohmann::json outputData;
	to_json(outputData);
}

//==========================================================================
// コライダー取得
//==========================================================================
CObjectChara::SphereCollider CObjectChara::GetNowSphereCollider(int nIdx)
{
	if (nIdx >= 0 && nIdx < static_cast<int>(m_SphereColliders.size()))
	{// 範囲内
		return m_SphereColliders[nIdx];
	}
	return SphereCollider();
}


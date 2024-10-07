//=============================================================================
// 
//  階層オブジェクト処理 [objectHierarchy.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectHierarchy.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
std::vector<CObjectHierarchy::Load> CObjectHierarchy::m_aLoadData = {};	// 読み込みデータ
int CObjectHierarchy::m_nNumLoad = 0;	// 読み込んだ数

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectHierarchy::CObjectHierarchy(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	//D3DXMatrixIdentity(&m_mtxWorld);			// ワールドマトリックス
	m_posOrigin = 0.0f;			// 最初の位置
	m_posCenter = 0.0f;			// 中心位置
	m_fRadius = 0.0f;			// 半径
	m_nNumModel = 0;			// モデルの数
	m_nIdxFile = 0;				// ファイルのインデックス番号
	m_nCenterPartsIdx = 0;		// 中心にするパーツのインデックス
	m_CenterOffset = 0.0f;		// 中心のオフセット

	m_apModel.clear();			// モデル(パーツ)のポインタ
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectHierarchy::~CObjectHierarchy()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectHierarchy* CObjectHierarchy::Create(const std::string& pTextFile)
{
	// メモリの確保
	CObjectHierarchy* pObjChara = DEBUG_NEW CObjectHierarchy;

	if (pObjChara != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		HRESULT hr = pObjChara->SetCharacter(pTextFile);
		if (FAILED(hr)){
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// キャラ作成
//==========================================================================
HRESULT CObjectHierarchy::SetCharacter(const std::string& file)
{
	// 初期化処理
	if (FAILED(CObjectHierarchy::Init()))
	{// 失敗していたら
		return E_FAIL;
	}

	// 構造体の中の文字列を探す
	auto itr = std::find_if(m_aLoadData.begin(), m_aLoadData.end(), [&file](const CObjectHierarchy::Load& string) {return string.sTextFile == file;});
	
	// 見つかった場合の処理
	if (itr != m_aLoadData.end())
	{
		// オブジェクト毎のデータ割り当て
		BindObjectData(itr - m_aLoadData.begin());
		return S_OK;
	}

	// ない場合の処理
	if (FAILED(CObjectHierarchy::ReadText(file)))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// オブジェクト毎のデータ割り当て
//==========================================================================
void CObjectHierarchy::BindObjectData(int nCntData)
{
	Load loadData = m_aLoadData[nCntData];

	// 中心にするパーツのインデックス
	m_nCenterPartsIdx = loadData.nCenterIdx;

	// 中心のオフセット
	m_CenterOffset = loadData.centerOffSet;

	// モデル数設定
	m_nNumModel = loadData.nNumModel;

	// 最初の位置設定
	m_posOrigin = loadData.posOrigin;

	// 半径
	m_fRadius = loadData.fRadius;

	// ファイルのインデックス番号
	m_nIdxFile = nCntData;

	for (int nCntParts = 0; nCntParts < loadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		LoadData modelInfo = loadData.LoadData[nCntParts];

		// モデルの生成
		m_apModel.emplace_back();

		m_apModel[nCntParts] = CModel::Create(
			loadData.LoadData[modelInfo.nType].pModelFile.c_str(),
			modelInfo.pos,
			modelInfo.rot);

		// 親モデルの設定
		if (modelInfo.nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[modelInfo.nParent]);

			int idx = modelInfo.nParent;
			m_apModel[nCntParts]->SetMtxParent(m_apModel[idx]->GetPtrWorldMtx());
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(nullptr);
			m_apModel[nCntParts]->SetMtxParent(&m_mtxWorld);
		}

		// 初期配置の判定
		if (modelInfo.nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectHierarchy::Init()
{
	
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectHierarchy::Uninit()
{
	// モデルの終了処理
	for (auto& model : m_apModel)
	{
		model->Uninit();
		delete model;
		model = nullptr;
	}
	m_apModel.clear();

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectHierarchy::Update()
{
	// 判定するパーツ取得
	CModel* pModel = m_apModel[m_nCenterPartsIdx];
	if (pModel == nullptr)
	{
		return;
	}

	// 判定するパーツのマトリックス取得
	MyLib::Matrix mtxTrans;
	MyLib::Matrix mtxWepon = pModel->GetWorldMtx();

	// 位置を反映する
	mtxTrans.Translation(m_CenterOffset);
	mtxWepon.Multiply(mtxTrans, mtxWepon);
	m_posCenter = mtxWepon.GetWorldPosition();
}

//==========================================================================
// モデル変更
//==========================================================================
void CObjectHierarchy::ChangeObject(int nDeleteParts, int nNewParts)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

	// オブジェクトキャラクターの情報取得
	CObjectHierarchy *pObjChar = GetObjectHierarchy();

	int nNumAll = pObjChar->GetNumModel();

	if (m_apModel[nDeleteParts] != nullptr)
	{
		// モデルの終了処理
		m_apModel[nDeleteParts]->Uninit();
		delete m_apModel[nDeleteParts];
		m_apModel[nDeleteParts] = nullptr;
	}

	// 新しいパーツを読み込む
	if (nNewParts >= 0 && m_apModel[nNewParts] == nullptr)
	{
		if (m_apModel[nNewParts] == nullptr)
		{
			// モデル作成
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}
	}

	// 親モデルの設定
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] == nullptr)
		{// nullptrだったら
			continue;
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(nullptr);
		}
	}
}

//==========================================================================
// 切り替えの種類
//==========================================================================
void CObjectHierarchy::ChangeObject(int nSwitchType)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

#if _DEBUG
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] != nullptr)
		{
			// モデルの終了処理
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = nullptr;
		}

		// モデル作成
		if (m_apModel[nCntParts] == nullptr)
		{
			m_apModel[nCntParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(nullptr);
		}

		if (LoadData.LoadData[nCntParts].nStart != 1)
		{
			ChangeObject(nCntParts, -1);
		}
	}
#endif

	// モデルの切り替え
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// 切り替えの種類が違うとき
			continue;
		}

		// 削除するインデックス番号
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != nullptr)
		{
			// モデルの終了処理
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = nullptr;
		}

		// 生成するインデックス番号
		int nNewIdx = nDeleteIdx;

		if (nNewIdx < 0)
		{
			nNewIdx = nCntParts;
		}

		// モデル作成
		if (m_apModel[nNewIdx] == nullptr)
		{
			m_apModel[nNewIdx] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nCntParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nCntParts].pos,
				LoadData.LoadData[nCntParts].rot);
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nNewIdx]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nNewIdx]->SetParent(nullptr);
		}
	}

}

//==========================================================================
// モデル設定
//==========================================================================
void CObjectHierarchy::SetObject(int nNewParts)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

	// 新しいパーツを読み込む
	if (nNewParts >= 0 && m_apModel[nNewParts] == nullptr)
	{
		if (m_apModel[nNewParts] == nullptr)
		{
			// モデル作成
			m_apModel[nNewParts] = CModel::Create(
				LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
				LoadData.LoadData[nNewParts].pos,
				LoadData.LoadData[nNewParts].rot);
		}

		if (m_apModel[nNewParts] == nullptr)
		{
			// モデルの終了処理
			m_apModel[nNewParts]->Uninit();
			delete m_apModel[nNewParts];
			m_apModel[nNewParts] = nullptr;
		}
	}

	// 親モデルの設定
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] == nullptr)
		{
			continue;
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent]);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(nullptr);
		}
	}
}

//==========================================================================
// モデル削除
//==========================================================================
void CObjectHierarchy::DeleteObject(int nSwitchType)
{
	// ファイル情報取得
	Load LoadData = GetLoadData(m_nIdxFile);

	// モデルの切り替え
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (LoadData.LoadData[nCntParts].nSwitchType != nSwitchType)
		{// 切り替えの種類が違うとき
			continue;
		}

		// 削除するインデックス番号
		int nDeleteIdx = LoadData.LoadData[nCntParts].nIDSwitchModel;

		if (nDeleteIdx >= 0 && m_apModel[nDeleteIdx] != nullptr)
		{
			// モデルの終了処理
			m_apModel[nDeleteIdx]->Uninit();
			delete m_apModel[nDeleteIdx];
			m_apModel[nDeleteIdx] = nullptr;
		}
	}
}

//==========================================================================
// ワールドマトリックスの計算処理
//==========================================================================
void CObjectHierarchy::CalWorldMtx()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxRot, mtxTrans;	// 計算用マトリックス宣言

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// 向きを反映する
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// 位置を反映する
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);

}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectHierarchy::Draw()
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// モデルの描画
	for (const auto& model : m_apModel)
	{
		model->Draw();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectHierarchy::Draw(D3DXCOLOR col)
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// モデルの描画
	for (const auto& model : m_apModel)
	{
		model->Draw(col);
	}
}


//==========================================================================
// 描画処理
//==========================================================================
void CObjectHierarchy::Draw(float fAlpha)
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// モデルの描画
	for (const auto& model : m_apModel)
	{
		model->Draw(fAlpha);
	}
}

//==========================================================================
// テキスト読み込み処理
//==========================================================================
HRESULT CObjectHierarchy::ReadText(const std::string& file)
{
	FILE *pFile = nullptr;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(file.c_str(), "r");

	if (pFile == nullptr)
	{// ファイルが開けた場合
		return E_FAIL;
	}

	// 要素追加
	m_aLoadData.emplace_back();

	// ファイル名保存
	m_aLoadData[m_nNumLoad].sTextFile = file;

	// ファイルのインデックス番号
	m_nIdxFile = m_nNumLoad;

	char aComment[MAX_COMMENT];	// コメント
	int nCntFileName = 0;
	int nCntSetParts = 0;

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル数の設定
		if (strcmp(aComment, "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &m_nNumModel);	// モデル数
			m_aLoadData[m_nNumLoad].nNumModel = m_nNumModel;
		}

		while (nCntFileName != m_nNumModel)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(aComment, "MODEL_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 要素追加
				m_aLoadData[m_nNumLoad].LoadData.emplace_back();

				// ファイル名保存
				m_aLoadData[m_nNumLoad].LoadData[nCntFileName].pModelFile = aComment;

				nCntFileName++;	// ファイル数加算
			}
		}

		// 各パーツの設定
		if (strcmp(aComment, "CHARACTERSET") == 0)
		{// キャラクター情報の読み込みを開始

			while (strcmp(aComment, "END_CHARACTERSET") != 0)
			{// END_CHARACTERSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	//確認する

				// オブジェクト毎のデータ読み込み
				LoadObjectData(pFile, aComment);

				// パーツ毎のデータ読み込み
				LoadPartsData(pFile, aComment, &nCntSetParts);
			}
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	// 読み込んだ数加算
	m_nNumLoad++;

	return S_OK;
}

//==========================================================================
// オブジェクト毎のデータ読み込み
//==========================================================================
void CObjectHierarchy::LoadObjectData(FILE *pFile, const std::string& file)
{
	char hoge[MAX_COMMENT] = {};	// コメント
	if (file.find("RADIUS") != std::string::npos)
	{// RADIUSで半径

		fscanf(pFile, "%s", &hoge[0]);	// =の分
		fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fRadius);	// 半径
		m_fRadius = m_aLoadData[m_nNumLoad].fRadius;
	}

	if (file.find("CENTERSET") != std::string::npos)
	{// CENTERSETで中心位置読み込み

		while (strcmp(hoge, "END_CENTERSET") != 0)
		{// END_CENTERSETが来るまで繰り返す

			fscanf(pFile, "%s", &hoge[0]);	//確認する

			if (strcmp(hoge, "PARTS") == 0)
			{// PARTSが来たらパーツ番号読み込み

				fscanf(pFile, "%s", &hoge[0]);	// =の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nCenterIdx);		// モデル種類の列挙

				m_nCenterPartsIdx = m_aLoadData[m_nNumLoad].nCenterIdx;		// 中心にするパーツのインデックス
			}

			if (strcmp(hoge, "OFFSET") == 0)
			{// OFFSETが来たら位置読み込み

				fscanf(pFile, "%s", &hoge[0]);		// =の分
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.x);	// X座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.y);	// Y座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.z);	// Z座標
				m_CenterOffset = m_aLoadData[m_nNumLoad].centerOffSet;			// 中心のオフセット
			}
		}
	}
}

//==========================================================================
// パーツ毎のデータ読み込み
//==========================================================================
void CObjectHierarchy::LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts)
{
	char CheckText[MAX_COMMENT] = {};	// チェック用
	char hoge[MAX_COMMENT] = {};	// コメント

	int nCntSetParts = *pCntParts;

	if (file.find("PARTSSET") != std::string::npos)
	{// PARTSSETでパーツ情報の読み込み開始

		while (strcmp(CheckText, "END_PARTSSET") != 0)
		{// END_PARTSSETが来るまで繰り返す

			fscanf(pFile, "%s", &CheckText[0]);	// 確認する

			if (strcmp(CheckText, "INDEX") == 0)
			{// INDEXが来たらモデル番号読み込み

				fscanf(pFile, "%s", &hoge[0]);	// =の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType);		// モデル種類の列挙
			}

			if (strcmp(CheckText, "PARENT") == 0)
			{// PARENTが来たら親の番号読み込み

				fscanf(pFile, "%s", &hoge[0]);	// =の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);		// 親の番号
			}

			if (strcmp(CheckText, "POS") == 0)
			{// POSが来たら位置読み込み

				fscanf(pFile, "%s", &hoge[0]);		// =の分
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.x);	// X座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.y);	// Y座標
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.z);	// Z座標

				if (nCntSetParts == 0)
				{
					m_posOrigin = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos;	// 最初の位置
					m_aLoadData[m_nNumLoad].posOrigin = m_posOrigin;	// 最初の位置
				}
			}

			if (strcmp(CheckText, "ROT") == 0)
			{// ROTが来たら向き読み込み

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.x);	// Xの角度
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.y);	// Yの角度
				fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.z);	// Zの角度
			}

			if (strcmp(CheckText, "START") == 0)
			{// STARTが来たら初期状態

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart);	// 最初からあるか
			}

			if (strcmp(CheckText, "SWITCHTYPE") == 0)
			{// SWITCHTYPEが来たら切り替えの種類

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nSwitchType);	// 切り替えの種類
			}

			if (strcmp(CheckText, "ID_SWITCHMODEL") == 0)
			{// ID_SWITCHMODELが来たら切り替えるモデルID

				fscanf(pFile, "%s", &hoge[0]);		//=の分
				fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nIDSwitchModel);	// 切り替えるモデルID
			}

		}// END_PARTSSETのかっこ

		// モデルの生成
		m_apModel.emplace_back();

		if (m_apModel[nCntSetParts] == nullptr)
		{
			m_apModel[nCntSetParts] = CModel::Create(
				m_aLoadData[m_nNumLoad].LoadData[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType].pModelFile.c_str(),
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos,
				m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot);
		}

		// 親モデルの設定
		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent >= 0)
		{
			m_apModel[nCntSetParts]->SetParent(m_apModel[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent]);

			int idx = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent;
			m_apModel[nCntSetParts]->SetMtxParent(m_apModel[idx]->GetPtrWorldMtx());
		}
		else
		{
			m_apModel[nCntSetParts]->SetParent(nullptr);
			m_apModel[nCntSetParts]->SetMtxParent(&m_mtxWorld);
		}

		if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart != 1)
		{
			ChangeObject(nCntSetParts, -1);
		}

		(*pCntParts)++;	// パーツのカウントを加算
	}
}

//==========================================================================
// マトリックス設定
//==========================================================================
void CObjectHierarchy::SetmtxWorld(const MyLib::Matrix mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// マトリックス取得
//==========================================================================
MyLib::Matrix CObjectHierarchy::GetmtxWorld() const
{
	return m_mtxWorld;
}

//==========================================================================
// 中心の位置取得
//==========================================================================
MyLib::Vector3 CObjectHierarchy::GetCenterPosition() const
{
	return m_posCenter;
}

//==========================================================================
// 位置設定
//==========================================================================
void CObjectHierarchy::SetOriginPosition(const MyLib::Vector3& pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CObjectHierarchy::GetOriginPosition() const
{
	return m_posOrigin;
}

//==========================================================================
// 半径設定
//==========================================================================
void CObjectHierarchy::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//==========================================================================
// 半径取得
//==========================================================================
float CObjectHierarchy::GetRadius() const
{
	return m_fRadius;
}

//==========================================================================
// オブジェクトキャラクターオブジェクトの取得
//==========================================================================
CObjectHierarchy *CObjectHierarchy::GetObjectHierarchy()
{
	return this;
}

//==========================================================================
// モデル数取得
//==========================================================================
int CObjectHierarchy::GetNumModel() 
{
	return m_nNumModel;
}

//==========================================================================
// モデル数設定
//==========================================================================
void CObjectHierarchy::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//==========================================================================
// モデル情報の取得
//==========================================================================
CModel **CObjectHierarchy::GetModel()
{
	return &m_apModel[0];
}

//==========================================================================
// モデル取得
//==========================================================================
CModel* CObjectHierarchy::GetModel(int idx)
{
	if (m_nNumModel <= idx) return nullptr;

	return m_apModel[idx];
}

//==========================================================================
// 読み込み情報取得
//==========================================================================
CObjectHierarchy::Load CObjectHierarchy::GetLoadData(int nIdx)
{
	return m_aLoadData[nIdx];
}

//==========================================================================
// ファイルのインデックス番号取得
//==========================================================================
int CObjectHierarchy::GetIdxFile()
{
	return m_nIdxFile;
}

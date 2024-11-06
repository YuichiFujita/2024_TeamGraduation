//=============================================================================
// 
//  オブジェクト3D処理 [objectChara.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectChara.h"
#include "calculation.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "3D_effect.h"

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CObjectChara::Load CObjectChara::m_aLoadData[mylib_const::MAX_MODEL] = {};	// 読み込みデータ
int CObjectChara::m_nNumLoad = 0;	// 読み込んだ数

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectChara::CObjectChara(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	D3DXMatrixIdentity(&m_mtxWorld);			// ワールドマトリックス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最初の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_fVelocity = 0.0f;			// 移動速度
	m_fRadius = 0.0f;			// 半径
	m_fRotDest = 0.0f;							// 目標の向き
	m_nNumModel = 0;		// モデルの数
	m_nIdxFile = 0;			// ファイルのインデックス番号

	m_nCenterPartsIdx = 0;		// 中心にするパーツのインデックス
	m_CenterOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心のオフセット


	memset(&m_apModel[0], NULL, sizeof(m_apModel));	// モデル(パーツ)のポインタ
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectChara::~CObjectChara()
{

}

//==========================================================================
// キャラ作成
//==========================================================================
HRESULT CObjectChara::SetCharacter(const std::string pTextFile)
{
	// 初期化処理
	if (FAILED(CObjectChara::Init()))
	{// 失敗していたら
		return E_FAIL;
	}


	// スフィアコライダーデータ読み込み
	LoadSphereColliders(pTextFile);

	for (const auto& colliderData : m_SphereColliders)
	{
		m_MeshSphere.push_back(nullptr);

		// 生成
		m_MeshSphere[m_MeshSphere.size() - 1] = CMeshSphere::Create(mylib_const::DEFAULT_VECTOR3, 10.0f, 0, 4, 8);
	}

	// 読み込み確認
	for (int nCntData = 0; nCntData < m_nNumLoad; nCntData++)
	{
		if (m_aLoadData[nCntData].sTextFile == pTextFile)
		{// ファイル名が一致している

			for (int nCntParts = 0; nCntParts < m_aLoadData[nCntData].nNumModel; nCntParts++)
			{// パーツ分繰り返し

				// モデル数設定
				m_nNumModel = m_aLoadData[nCntData].nNumModel;

				// 最初の位置設定
				m_posOrigin = m_aLoadData[nCntData].posOrigin;

				// 移動速度
				m_fVelocity = m_aLoadData[nCntData].fVelocity;

				// 半径
				m_fRadius = m_aLoadData[nCntData].fRadius;

				// ファイルのインデックス番号
				m_nIdxFile = nCntData;

				// モデルの生成
				m_apModel[nCntParts] = CModel::Create(
					m_aLoadData[nCntData].LoadData[m_aLoadData[nCntData].LoadData[nCntParts].nType].pModelFile.c_str(),
					m_aLoadData[nCntData].LoadData[nCntParts].pos,
					m_aLoadData[nCntData].LoadData[nCntParts].rot);

				// 親モデルの設定
				if (m_aLoadData[nCntData].LoadData[nCntParts].nParent >= 0)
				{
					// 親のモデルオブジェクト設定
					m_apModel[nCntParts]->SetParent(m_apModel[m_aLoadData[nCntData].LoadData[nCntParts].nParent], m_aLoadData[nCntData].LoadData[nCntParts].nParent);
				}
				else
				{// 自分が親の時
					m_apModel[nCntParts]->SetParent(NULL, -1);
				}
			}

			return S_OK;
		}
	}

	if (FAILED(CObjectChara::ReadText(pTextFile)))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectChara::Init(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < mylib_const::MAX_MODEL; nCntParts++)
		{
			m_aLoadData[nCntModel].LoadData[nCntParts].nStart = 1;	// 読み込みのデータ
			m_aLoadData[nCntModel].LoadData[nCntParts].side = EPartSide::SIDE_CENTER;	// 読み込みのデータ
		}
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectChara::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			// モデルの終了処理
			m_apModel[nCntModel]->Uninit();
			m_apModel[nCntModel] = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

// コライダー追加
void CObjectChara::AddCollider(const SphereCollider& collider)
{
	m_SphereColliders.push_back(collider);
	m_MeshSphere.push_back(nullptr);

	// 生成
	m_MeshSphere[GetNumCollider() - 1] = CMeshSphere::Create(mylib_const::DEFAULT_VECTOR3, 10.0f, 0, 4, 8);
}

void CObjectChara::SubCollider(void)	// コライダー削除
{
	m_MeshSphere[GetNumCollider() - 1]->Uninit();

	m_MeshSphere.pop_back();			// メッシュスフィア
	m_SphereColliders.pop_back();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectChara::Update(void)
{
	// モデル情報取得
	CModel** pModel = GetModel();
	D3DXMATRIX mtxTrans;	// 計算用マトリックス宣言

	// 武器のマトリックス取得
	D3DXMATRIX mtxWepon;
	D3DXMatrixIdentity(&mtxWepon);

	if (pModel[m_nCenterPartsIdx] == NULL)
	{
		return;
	}

	// 判定するパーツのマトリックス取得
	mtxWepon = pModel[m_nCenterPartsIdx]->GetWorldMtx();

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, m_CenterOffset.x, m_CenterOffset.y, m_CenterOffset.z);
	D3DXMatrixMultiply(&mtxWepon, &mtxTrans, &mtxWepon);

	// 武器の位置
	m_posCenter = D3DXVECTOR3(mtxWepon._41, mtxWepon._42, mtxWepon._43);

	for (int i = 0; i < 5; i++)
	{
		CEffect3D::Create(m_posCenter,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.4f, 1.0f, 0.6f, 1.0f), 5.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
}

//==========================================================================
// モデル変更
//==========================================================================
void CObjectChara::ChangeObject(int nDeleteParts, int nNewParts)
{
	int nIdx = GetIdxFile();
	Load LoadData = GetLoadData(nIdx);

	// オブジェクトキャラクターの情報取得
	CObjectChara *pObjChar = GetObjectChara();

	int nNumAll = pObjChar->GetNumModel();

	if (m_apModel[nDeleteParts] != NULL)
	{// NULLじゃなかったら

		// モデルの終了処理
		m_apModel[nDeleteParts]->Uninit();
		m_apModel[nDeleteParts] = NULL;
	}

	// NULLにする
	//m_apModel[nDeleteParts] = NULL;

	// 新しいパーツを読み込む
	if (nNewParts >= 0)
	{
		m_apModel[nNewParts] = CModel::Create(
			LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
			LoadData.LoadData[nNewParts].pos,
			LoadData.LoadData[nNewParts].rot);
	}

	// 親モデルの設定
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] == NULL)
		{// NULLだったら
			continue;
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent], LoadData.LoadData[nCntParts].nParent);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(NULL, -1);
		}
	}
}

//==========================================================================
// モデル設定
//==========================================================================
void CObjectChara::SetObject(int nNewParts)
{
	int nIdx = GetIdxFile();
	Load LoadData = GetLoadData(nIdx);

	// 新しいパーツを読み込む
	if (nNewParts >= 0)
	{
		m_apModel[nNewParts] = CModel::Create(
			LoadData.LoadData[LoadData.LoadData[nNewParts].nType].pModelFile.c_str(),
			LoadData.LoadData[nNewParts].pos,
			LoadData.LoadData[nNewParts].rot);
	}

	// 親モデルの設定
	for (int nCntParts = 0; nCntParts < LoadData.nNumModel; nCntParts++)
	{// パーツ分繰り返し

		if (m_apModel[nCntParts] == NULL)
		{// NULLだったら
			continue;
		}

		// 親モデルの設定
		if (LoadData.LoadData[nCntParts].nParent >= 0)
		{
			// 親のモデルオブジェクト設定
			m_apModel[nCntParts]->SetParent(m_apModel[LoadData.LoadData[nCntParts].nParent], LoadData.LoadData[nCntParts].nParent);
		}
		else
		{// 自分が親の時
			m_apModel[nCntParts]->SetParent(NULL, -1);
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			// パーツごとの描画
			m_apModel[nCntModel]->Draw();
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			// パーツごとの描画
			m_apModel[nCntModel]->Draw(col);
		}
	}
}


//==========================================================================
// 描画処理
//==========================================================================
void CObjectChara::Draw(int nIdx, float fAlpha)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言

									// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (int nCntModel = 0; nCntModel < mylib_const::MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLじゃなかったら

			if (nIdx == nCntModel)
			{
				// パーツごとの描画
				m_apModel[nCntModel]->Draw(fAlpha);
			}
			else
			{
				// パーツごとの描画
				m_apModel[nCntModel]->Draw();
			}
		}
	}
}

//==========================================================================
// テキスト読み込み処理
//==========================================================================
HRESULT CObjectChara::ReadText(const std::string pTextFile)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen(pTextFile.c_str(), "r");

	if (pFile == NULL)
	{//ファイルが開けた場合
		return E_FAIL;
	}

	// ファイル名保存
	m_aLoadData[m_nNumLoad].sTextFile = pTextFile;

	// ファイルのインデックス番号
	m_nIdxFile = m_nNumLoad;

	char aComment[MAX_COMMENT];	// コメント
	int nCntSetParts = 0;
	int nCntFileName = 0;

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


				if (strcmp(aComment, "CENTERSET") == 0)
				{// CENTERSETで中心位置読み込み

					while (strcmp(aComment, "END_CENTERSET") != 0)
					{// END_CENTERSETが来るまで繰り返す

						fscanf(pFile, "%s", &aComment[0]);	//確認する

						if (strcmp(aComment, "PARTS") == 0)
						{// PARTSが来たらパーツ番号読み込み

							fscanf(pFile, "%s", &aComment[0]);	// =の分
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].nCenterIdx);		// モデル種類の列挙

							m_nCenterPartsIdx = m_aLoadData[m_nNumLoad].nCenterIdx;		// 中心にするパーツのインデックス
						}

						if (strcmp(aComment, "OFFSET") == 0)
						{// OFFSETが来たら位置読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.x);	// X座標
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.y);	// Y座標
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].centerOffSet.z);	// Z座標
							m_CenterOffset = m_aLoadData[m_nNumLoad].centerOffSet;			// 中心のオフセット
						}
					}
				}

				if (strcmp(aComment, "VELOCITY") == 0)
				{// VELOCITYで移動速度

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fVelocity);	// 移動速度
					m_fVelocity = m_aLoadData[m_nNumLoad].fVelocity;
				}

				if (strcmp(aComment, "RADIUS") == 0)
				{// RADIUSで半径

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].fRadius);	// 半径
					m_fRadius = m_aLoadData[m_nNumLoad].fRadius;
				}

				if (strcmp(aComment, "PARTSSET") == 0)
				{// PARTSSETでパーツ情報の読み込み開始

					while (strcmp(aComment, "END_PARTSSET") != 0)
					{// END_PARTSSETが来るまで繰り返す

						fscanf(pFile, "%s", &aComment[0]);	// 確認する

						if (strcmp(aComment, "INDEX") == 0)
						{// INDEXが来たらモデル番号読み込み

							fscanf(pFile, "%s", &aComment[0]);	// =の分
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType);		// モデル種類の列挙
						}

						if (strcmp(aComment, "PARENT") == 0)
						{// PARENTが来たら親の番号読み込み

							fscanf(pFile, "%s", &aComment[0]);	// =の分
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);		// 親の番号
						}

						if (strcmp(aComment, "SIDE") == 0)
						{// SIDEが来たら配置情報

							fscanf(pFile, "%s", &aComment[0]);	// =の分

							int nSide = 0;
							fscanf(pFile, "%d", &nSide);	// 配置情報

							if (nSide < -1) nSide = -1;
							if (nSide > 1) nSide = 1;

							// SIDE読み込み
							m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].side = static_cast<EPartSide>(nSide);

							const auto& loadData = m_aLoadData[m_nNumLoad];
							// 左右
							if (loadData.LoadData[nCntSetParts].side != EPartSide::SIDE_CENTER)
							{// 中央以外

								// 前回のインデックス
								int beforeIdx = nCntSetParts - 1;
								if (beforeIdx < 0) beforeIdx = 0;

								int nStart = 0;
								int nCnt = 0;

								if (loadData.LoadData[beforeIdx].side != loadData.LoadData[nCntSetParts].side)
								{// 前回と今回のサイドが違う
									
									// 開始
									nStart = nCntSetParts;
									nCnt++;	// パーツ数加算
								}
								else if (loadData.LoadData[beforeIdx].side == loadData.LoadData[nCntSetParts].side)
								{// 前回と今回のサイドが違う

									// 開始
									nCnt++;	// パーツ数加算
								}

							}

							m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].side;

						}

						if (strcmp(aComment, "POS") == 0)
						{// POSが来たら位置読み込み

							fscanf(pFile, "%s", &aComment[0]);		// =の分
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.x);	// X座標
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.y);	// Y座標
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos.z);	// Z座標

							if (nCntSetParts == 0)
							{
								m_posOrigin = m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos;	// 最初の位置
								m_aLoadData[m_nNumLoad].posOrigin = m_posOrigin;	// 最初の位置
							}
						}

						if (strcmp(aComment, "ROT") == 0)
						{// ROTが来たら向き読み込み

							fscanf(pFile, "%s", &aComment[0]);		//=の分
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.x);	// Xの角度
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.y);	// Yの角度
							fscanf(pFile, "%f", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot.z);	// Zの角度
						}

						if (strcmp(aComment, "START") == 0)
						{// STARTが来たら初期状態

							fscanf(pFile, "%s", &aComment[0]);		//=の分
							fscanf(pFile, "%d", &m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart);	// 最初からあるか
						}

					}// END_PARTSSETのかっこ

					// モデルの生成
					m_apModel[nCntSetParts] = CModel::Create(
						m_aLoadData[m_nNumLoad].LoadData[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nType].pModelFile.c_str(),
						m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].pos,
						m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].rot);

					// 親モデルの設定
					if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent >= 0)
					{
						m_apModel[nCntSetParts]->SetParent(m_apModel[m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent], m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nParent);
					}
					else
					{
						m_apModel[nCntSetParts]->SetParent(NULL, -1);
					}

					if (m_aLoadData[m_nNumLoad].LoadData[nCntSetParts].nStart != 1)
					{
						ChangeObject(nCntSetParts, -1);
					}

					nCntSetParts++;	// パーツのカウントを加算
				}
			}// END_CHARACTERSETのかっこ
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
// スフィアコライダー読み込み
//==========================================================================
void CObjectChara::LoadSphereColliders(const std::string textfile)
{
	m_SetUpFilename = "";

	// ファイルを開く
	FILE* pFile = fopen(textfile.c_str(), "r");
	if (pFile == NULL)
	{//ファイルが開けなかった場合
		return;
	}

	// 読み込み用変数
	char aComment[MAX_COMMENT];	// コメント用

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
			m_SetUpFilename = aComment;
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
	std::ifstream file(m_SetUpFilename);
	if (!file.is_open())
	{
		return;
	}

	nlohmann::json jsonData;
	file >> jsonData;	// jsonデータを与える

	// jsonデータから読み込む
	from_json(jsonData);
}

//==========================================================================
// スフィアコライダー書き込み
//==========================================================================
void CObjectChara::SaveSphereColliders()
{
	if (m_SetUpFilename == "")
	{
		return;
	}

	// ファイルにキャラクターのデータを書き込む
	std::ofstream outFile(m_SetUpFilename);
	if (!outFile.is_open())
	{
		return;
	}

	nlohmann::json outputData;
	to_json(outputData);

	// ファイルにデータを書き込む
	outFile << std::setw(4) << outputData << std::endl;
}

//==========================================================================
// マトリックス設定
//==========================================================================
void CObjectChara::SetmtxWorld(const D3DXMATRIX mtx)
{
	m_mtxWorld = mtx;
}

//==========================================================================
// マトリックス取得
//==========================================================================
D3DXMATRIX CObjectChara::GetmtxWorld(void) const
{
	return m_mtxWorld;
}

//==========================================================================
// 位置設定
//==========================================================================
void CObjectChara::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObjectChara::GetPosition(void) const
{
	return m_pos;
}


//==========================================================================
// 位置設定
//==========================================================================
void CObjectChara::SetOriginPosition(const D3DXVECTOR3 pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObjectChara::GetOriginPosition(void) const
{
	return m_posOrigin;
}

//==========================================================================
// 位置設定
//==========================================================================
void CObjectChara::SetOldPosition(const D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR3 CObjectChara::GetOldPosition(void) const
{
	return m_posOld;
}


//==========================================================================
// 移動量設定
//==========================================================================
void CObjectChara::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//==========================================================================
// 移動量取得
//==========================================================================
D3DXVECTOR3 CObjectChara::GetMove(void) const
{
	return m_move;
}

//==========================================================================
// 移動速度設定
//==========================================================================
void CObjectChara::SetVelocity(const float fVelocity)
{
	m_fVelocity = fVelocity;
}

//==========================================================================
// 移動速度設定
//==========================================================================
float CObjectChara::GetVelocity(void) const
{
	return m_fVelocity;
}

//==========================================================================
// 向き設定
//==========================================================================
void CObjectChara::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==========================================================================
// 向き取得
//==========================================================================
D3DXVECTOR3 CObjectChara::GetRotation(void) const
{
	return m_rot;
}

//==========================================================================
// 目標の向き設定
//==========================================================================
void CObjectChara::SetRotDest(const float fRotDest)
{
	m_fRotDest = fRotDest;
}

//==========================================================================
// 目標の向き設定
//==========================================================================
float CObjectChara::GetRotDest(void) const
{
	return m_fRotDest;
}

//==========================================================================
// 半径設定
//==========================================================================
void CObjectChara::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//==========================================================================
// 半径取得
//==========================================================================
float CObjectChara::GetRadius(void) const
{
	return m_fRadius;
}

//==========================================================================
// オブジェクトキャラクターオブジェクトの取得
//==========================================================================
CObjectChara *CObjectChara::GetObjectChara(void)
{
	return this;
}

//==========================================================================
// モデル数取得
//==========================================================================
int CObjectChara::GetNumModel(void) 
{
	return m_nNumModel;
}

//==========================================================================
// モデル数設定
//==========================================================================
void CObjectChara::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//==========================================================================
// モデル情報の取得
//==========================================================================
CModel **CObjectChara::GetModel(void)
{
	return &m_apModel[0];
}

//==========================================================================
// 読み込み情報取得
//==========================================================================
CObjectChara::Load CObjectChara::GetLoadData(int nIdx)
{
	return m_aLoadData[nIdx];
}

//==========================================================================
// ファイルのインデックス番号取得
//==========================================================================
int CObjectChara::GetIdxFile(void)
{
	return m_nIdxFile;
}

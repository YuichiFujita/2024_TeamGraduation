//=============================================================================
// 
//  ランキングスコア処理 [rankingscore.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "rankingscore.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "object2D.h"
#include "multinumber.h"
#include "calculation.h"
#include "ranking.h"
#include "rankingmanager.h"
#include "sound.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define RANKINGFILE			"data\\BIN\\ranking.bin"
#define NUMBERTEXTURE		"data\\TEXTURE\\number\\number_blackclover_01.png"	// テクスチャのファイル
#define TEX_U				(0.1f)				// Uの分割
#define BASE_WIDTH			(40.0f)		// 横幅
#define BASE_HEIGHT			(50.0f)		// 縦幅
#define BASE_DIS_X			(50.0f)		// 間隔
#define DIS_X				(50.0f)				// 間隔
#define POSY_BASE			(330.0f)			// Y位置
#define BASETEXT_POSX		(180.0f)			// X位置
#define TEXT_MOVETIME		(20)				// 移動時間
#define INIT_POSX			(1600.0f)			// 初期位置

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CRankingScore::m_apTextureFile[VTX_MAX] =		// テクスチャのファイル
{
	"data\\TEXTURE\\rankingnum_01.png",
	"data\\TEXTURE\\rankinglogo_01.png",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CRankingScore::CRankingScore(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_nNumRanking = 0;				// ランキング数
	memset(&m_nScore[0], 0, sizeof(m_nScore));	// スコア
	m_nNowScore = 0;				// 今回のスコア
	memset(&m_pObj2D[0], 0, sizeof(m_pObj2D));	// オブジェクト2Dのオブジェクト
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));	// テクスチャのインデックス番号
	m_nTexIdxNumber = 0;			// 数字テクスチャのインデックス番号
	m_nIdxNewRecord = 0;			// ニューレコードのインデックス番号
	m_nCntNewRecord = 0;			// ニューレコードのカウンター
	memset(&m_fPosDestX[0], 0, sizeof(m_fPosDestX));	// 目標の位置
	m_bNewRecord = false;			// ニューレコードのフラグ
	memset(&m_bArrival[0], 0, sizeof(m_bArrival));	// 到着判定
	memset(&m_pScore[0], 0, sizeof(m_pScore));	// 数字のオブジェクト
}

//==========================================================================
// デストラクタ
//==========================================================================
CRankingScore::~CRankingScore()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRankingScore *CRankingScore::Create()
{
	// 生成用のオブジェクト
	CRankingScore *pScore = nullptr;

	if (pScore == nullptr)
	{// nullptrだったら

		// メモリの確保
		pScore = DEBUG_NEW CRankingScore;

		if (pScore != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pScore->Init();
		}

		return pScore;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRankingScore::Init()
{
	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		// 生成処理
		m_pObj2D[nCntVtx] = CObject2D::Create(7);

		// テクスチャの割り当て
		m_nTexIdx[nCntVtx] = CTexture::GetInstance()->Regist(m_apTextureFile[nCntVtx]);

		// テクスチャの割り当て
		m_pObj2D[nCntVtx]->GetObject2D()->BindTexture(m_nTexIdx[nCntVtx]);

		// 各種変数の初期化
		switch (nCntVtx)
		{
		case VTX_LOGO:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CTexture::GetInstance()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.3f);	// サイズ
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(MyLib::Vector3(280.0f, 60.0f, 0.0f));	// 位置
			break;

		case VTX_NUM:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CTexture::GetInstance()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.5f);	// サイズ
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(MyLib::Vector3(m_pObj2D[nCntVtx]->GetSize().x, 360.0f, 0.0f));	// 位置
			break;

		default:
			break;
		}

		// 種類の設定
		m_pObj2D[nCntVtx]->GetObject2D()->SetType(CObject::TYPE::TYPE_OBJECT2D);
	}

	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		// 目標の位置
		m_fPosDestX[nCntRanking] = m_pObj2D[VTX_LOGO]->GetSize().x;


		// 生成処理
		m_pScore[nCntRanking] = CMultiNumber::Create(
			MyLib::Vector3(INIT_POSX, nCntRanking * 120.0f + 170.0f, 0.0f),
			D3DXVECTOR2(BASE_WIDTH, BASE_WIDTH), 6,
			CNumber::OBJECTTYPE_2D,
			NUMBERTEXTURE, true, GetPriority());
	}

	// ランキング読み込み
	Load();

	
		// 今回のスコア取得
		m_nNowScore = CManager::GetInstance()->GetRankingManager()->GetNowScore();

		// ソート処理
		Sort();

		// セーブ処理
		Save();
	

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRankingScore::Uninit()
{
	// 終了処理
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_pObj2D[nCntVtx] != nullptr)
		{// メモリの確保がされていたら

			// 終了処理
			m_pObj2D[nCntVtx] = nullptr;
		}
	}

	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		// 数字のオブジェクトの終了処理
		if (m_pScore[nCntRanking] != nullptr)
		{
			m_pScore[nCntRanking]->Uninit();
			m_pScore[nCntRanking] = nullptr;
		}
	}

	// 情報削除
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CRankingScore::Update()
{
	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		if (m_bArrival[nCntRanking] == false &&
			(nCntRanking == 0 || m_bArrival[nCntRanking - 1]))
		{
			// 移動処理
			Moving(nCntRanking);
		}

		m_pScore[nCntRanking]->Update();
		m_pScore[nCntRanking]->SetValue(m_nScore[nCntRanking]);
	}

	if (m_bNewRecord)
	{// 記録更新していたら

		// ニューレコードの更新処理
		UpdateNewRecord();
	}

}

//==========================================================================
// 移動処理
//==========================================================================
void CRankingScore::Moving(int nCntRanking)
{

	// 位置取得
	MyLib::Vector3 pos = m_pScore[nCntRanking]->GetPosition();

	if (pos.x == INIT_POSX)
	{
		// サウンド再生
		//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_NUMBERMOVE);
	}

	// 色取得
	D3DXCOLOR col = m_pScore[nCntRanking]->GetColor();

	float fDest = m_fPosDestX[nCntRanking];

	// 目標の位置へ補正
	if (fDest < pos.x)
	{
		pos.x += (fDest - INIT_POSX) / (float)TEXT_MOVETIME;
		col.a += 1.0f / (float)TEXT_MOVETIME;
	}

	if (fDest >= pos.x)
	{// 目標で固定
		pos.x = fDest;
		col.a = 1.0f;
		m_bArrival[nCntRanking] = true;

		if (nCntRanking == RANKINGNUM - 1)
		{
			CRanking::SetEnableArrival();
		}
	}

	// 位置設定
	m_pScore[nCntRanking]->SetPosition(pos);

	// 色設定
	m_pScore[nCntRanking]->SetColor(col);
}

//==========================================================================
// ニューレコードの更新処理
//==========================================================================
void CRankingScore::UpdateNewRecord()
{

	// 位置取得
	MyLib::Vector3 pos = m_pScore[m_nIdxNewRecord]->GetPosition();

	// サイズ取得
	D3DXVECTOR2 size = m_pScore[m_nIdxNewRecord]->GetSize();

	// 色取得
	D3DXCOLOR col = m_pScore[m_nIdxNewRecord]->GetColor();

	// 曲線補正
	UtilFunc::Correction::CuadricCurveComp(col.a, 60, 0.3f, 1.0f, m_nCntNewRecord);

	// 位置設定
	m_pScore[m_nIdxNewRecord]->SetPosition(pos);

	// サイズ設定
	m_pScore[m_nIdxNewRecord]->SetSize(size);

	// 色設定
	m_pScore[m_nIdxNewRecord]->SetColor(col);
}

//==========================================================================
// 全ての到着設定
//==========================================================================
void CRankingScore::SetAllArrival()
{

	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
	{
		m_bArrival[nCntScore] = true;	// 到着判定
	}

	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		
			// 位置取得
			MyLib::Vector3 pos = m_pScore[nCntRanking]->GetPosition();

			if (pos.x == INIT_POSX)
			{
				// サウンド再生
				//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_NUMBERMOVE);
			}

			// 色取得
			D3DXCOLOR col = m_pScore[nCntRanking]->GetColor();

			// 移動
			float fDest = m_fPosDestX[nCntRanking];
			pos.x = fDest;

			// 不透明度設定
			col.a = 1.0f;

			// 位置設定
			m_pScore[nCntRanking]->SetPosition(pos);

			// 色設定
			m_pScore[nCntRanking]->SetColor(col);
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CRankingScore::Draw()
{

}

//==============================================================
// ソート処理
//==============================================================
void CRankingScore::Sort()
{
	// 降順処理
	for (int nCntData = 0; nCntData < m_nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking; nCntSort++)
		{
			if (m_nScore[nCntData] < m_nScore[nCntSort])
			{// 要素1より要素2が大きかったら

				int nTemp = m_nScore[nCntData];
				m_nScore[nCntData] = m_nScore[nCntSort];
				m_nScore[nCntSort] = nTemp;
			}
		}
	}

	// ランキング最大値よりも今回のが小さかったら
	if (m_nScore[m_nNumRanking - 1] < m_nNowScore)
	{// 入れ替え

		m_nScore[m_nNumRanking - 1] = m_nNowScore;	// 最小値と今回のデータ入れ替え
		m_bNewRecord = true;	// フラグON
	}

	// 降順処理
	for (int nCntData = 0; nCntData < m_nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking; nCntSort++)
		{
			if (m_nScore[nCntData] < m_nScore[nCntSort])
			{// 要素1より要素2が大きかったら

				int nTemp = m_nScore[nCntData];
				m_nScore[nCntData] = m_nScore[nCntSort];
				m_nScore[nCntSort] = nTemp;
			}
		}
	}

	if (m_bNewRecord)
	{// ニューレコードの時

		for (int nCount = RANKINGNUM; nCount > 0; nCount--)
		{// 要素番号記憶して抜ける

			if (m_nScore[nCount - 1] == m_nNowScore)
			{
				m_nIdxNewRecord = nCount - 1;
				break;
			}
		}
	}
}

//==============================================================
// セーブ処理
//==============================================================
void CRankingScore::Save()
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(RANKINGFILE, "wb");

	if (pFile != nullptr)
	{// ファイルが開けた場合

		// ファイルに数値を書き出す
		fwrite(&m_nNumRanking, sizeof(int), 1, pFile);
		fwrite(&m_nScore[0], sizeof(int), m_nNumRanking, pFile);

		// ファイルを閉じる
		fclose(pFile);
	}
}

//==============================================================
// ロード処理
//==============================================================
void CRankingScore::Load()
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(RANKINGFILE, "rb");

	if (pFile != nullptr)
	{// ファイルが開けた場合

		// ファイルに数値を読み込む
		fread(&m_nNumRanking, sizeof(int), 1, pFile);
		fread(&m_nScore[0], sizeof(int), m_nNumRanking, pFile);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けなかった場合

		m_nNumRanking = RANKINGNUM;
		for (int nCount = 0; nCount < m_nNumRanking; nCount++)
		{// 全部ゼロ
			m_nScore[nCount] = 500;
			//m_nScore[nCount] = 150000 - nCount * 25000;
		}

		// セーブ
		Save();
	}
}
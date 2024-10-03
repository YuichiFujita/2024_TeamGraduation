//=============================================================================
// 
// ランキング2D処理 [rankingItem.cpp]
//  Author : 澗口将太郎
// 
//=============================================================================
#include "rankingItem_top3.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "gamemanager.h"
#include "game.h"
#include "game_pressenter.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	//ロゴ情報
	//const std::string TEXTURE_RESULT = "data\\TEXTURE\\result\\ninja_resultlogo.png";
	//const MyLib::Vector3 LOGO_POS = MyLib::Vector3(220.0f, 60.0f, 0.0f);
	//const D3DXVECTOR2 LOGO_SIZE = D3DXVECTOR2(200.0f, 50.0f);
	
	const MyLib::Vector3 ITEM_POS = MyLib::Vector3(250.0f,250.0f, 0.0f);
	const D3DXVECTOR2 ITEM_SIZE = D3DXVECTOR2(100.0f, 50.0f);
	const std::string TEXTURE_ITEM[CRankingItem_top3::ITEM_MAX] = { "data\\TEXTURE\\result\\ranking_num00.png",
										"data\\TEXTURE\\result\\toatalrank.png",
										"data\\TEXTURE\\result\\num00.png",
										"data\\TEXTURE\\result\\num00_slash.png",
										"data\\TEXTURE\\result\\rank00.png",
										"data\\TEXTURE\\result\\new_records.png" }; 

	const std::string TEXTURE_COLON = "data\\TEXTURE\\result\\num00_colon.png";
	const D3DXVECTOR2 SPRITE_SIZE = D3DXVECTOR2(50.0f, 50.0f);
	const int MAX_SCORE_DEGIT = 8;
	const int MAX_DATE_DEGIT = 10;

	const float TIME_SET = 0.2f;
}
//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CRankingItem_top3::m_nObjNum = NULL;	//当オブジェクトの生成数

//==========================================================================
// コンストラクタ
//==========================================================================
CRankingItem_top3::CRankingItem_top3(int nPriority) : CRankingItem()
{
	m_fStateTime = 0.0f;
	
	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < MAX_SCORE_DEGIT; nCnt++)
	{
		m_pScoreItem[nCnt] = nullptr;
	}

	//当オブジェクトの生成数の初期化と加算処理
	if (m_nObjNum == NULL)
	{
		m_nObjNum = 1;
	}
	else
	{
		m_nObjNum++;
	}
	
}
//==========================================================================
// デストラクタ
//==========================================================================
CRankingItem_top3::~CRankingItem_top3()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRankingItem_top3* CRankingItem_top3::Create(int nNumRank, int nMin_Time, int nSec_Time, int nMilliSec_Time, int nAllRank, int nDate[3], MyLib::Vector3 posAll, float fSize, bool bNewRecord)
{
	// メモリの確保
	CRankingItem_top3 *pMarker = DEBUG_NEW CRankingItem_top3;

	if (pMarker != nullptr)
	{
		//順位を取得
		pMarker->m_nNumRank = nNumRank;

		//タイムを取得
		pMarker->m_nTime.nMinutes = nMin_Time;
		pMarker->m_nTime.nSeconds = nSec_Time;
		pMarker->m_nTime.nMilliSeconds = nMilliSec_Time;

		//時間を取得
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			pMarker->m_nDate[nCnt] = nDate[nCnt];
		}

		//総評を取得
		pMarker->m_nAllRank = nAllRank;

		//新記録かを取得
		pMarker->m_bUpdate = bNewRecord;

		//位置・サイズ設定
		pMarker->m_posAll = posAll;
		pMarker->m_fSize = fSize;

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRankingItem_top3::Init()
{
	//項目の初期化処理
	MyLib::Vector3 difItemPos = MyLib::Vector3(-610.0f * m_fSize,30.0f * m_fSize,0.0f);
	MyLib::Vector3 difRankPos = MyLib::Vector3(0.0f, 100.0f * (m_nObjNum - 1), 0.0f);
	D3DXVECTOR2 persize = D3DXVECTOR2(0.5f, 0.5f);

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (nCntItem < ITEM_TIME)
		{//横長文字OBJの初期化

			//Obj2D生成
			m_pItem[nCntItem] = CObject2D::Create(GetPriority());

			//各項目の距離加算
			difItemPos += MyLib::Vector3(610.0f*m_fSize, -30.0f * m_fSize, 0.0f);

			m_posItem[nCntItem] = m_posAll + difItemPos;
			m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

			//テクスチャ割り当て
			int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
			m_pItem[nCntItem]->BindTexture(nIdxTex);

			//サイズ設定
			D3DXVECTOR2 size = ITEM_SIZE*m_fSize;

			if (nCntItem == 1)
			{//縮小処理
				size.x *= persize.x;
				size.y *= persize.y;
			}
			m_pItem[nCntItem]->SetSize(size);
			m_pItem[nCntItem]->SetSizeOrigin(size);

			//タイプ設定
			CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

			//スプライト調整
			if (nCntItem == 0)
			{//順位の調整
				D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(0.0f,m_nNumRank * 0.1f - 0.1f), D3DXVECTOR2(1.0f,m_nNumRank * 0.1f - 0.1f),  D3DXVECTOR2(0.0f,m_nNumRank * 0.1f),  D3DXVECTOR2(1.0f,m_nNumRank * 0.1f) };
				m_pItem[nCntItem]->SetTex(uvpos);
			}
		}
		else if (nCntItem == ITEM_TIME)
		{//スコアの初期化
			difItemPos = MyLib::Vector3(150.0f * m_fSize, 0.0f, 0.0f);
			int nDegitNum[MAX_SCORE_DEGIT + 1];

			m_posItem[nCntItem] = m_posAll + difItemPos;

			//スコアのみ別管理のためnullptrを代入
			m_pItem[nCntItem] = nullptr;

			//スコアを桁ごとに分解
			int nMinutes =m_nTime.nMinutes;
			for (int nCnt = 0, nDegitCnt = 8; nDegitCnt > 6; nDegitCnt--, nCnt++)
			{
				nDegitNum[nDegitCnt] = nMinutes / pow(10, 2- nCnt - 1);
				nMinutes = nMinutes - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			nDegitNum[6] = 11;

			int nSeconds = m_nTime.nSeconds;
			for (int nCnt = 0, nDegitCnt = 5; nDegitCnt > 3; nDegitCnt--, nCnt++)
			{
				nDegitNum[nDegitCnt] = nSeconds / pow(10, 2 - nCnt - 1);
				nSeconds = nSeconds - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			nDegitNum[3] = 11;

			int nMilliSeconds = m_nTime.nMilliSeconds;
			for (int nCnt = 0, nDegitCnt = 2; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				nDegitNum[nDegitCnt] = nMilliSeconds / pow(10, 2 - nCnt - 1);
				nMilliSeconds = nMilliSeconds - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			//各桁の初期化
			for (int nCnt = 0, nDegitCnt = MAX_SCORE_DEGIT; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				MyLib::Vector3 difDegitPos = MyLib::Vector3(50.0f * nCnt * m_fSize, 0.0f, 0.0f);
				m_pScoreItem[nDegitCnt] = CObject2D::Create(GetPriority());

				m_pScoreItem[nDegitCnt]->SetPosition(m_posItem[nCntItem] + difDegitPos);

				if (nDegitNum[nDegitCnt] == 11)
				{
					int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_COLON);
					m_pScoreItem[nDegitCnt]->BindTexture(nIdxTex);
				}
				else
				{
					int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
					m_pScoreItem[nDegitCnt]->BindTexture(nIdxTex);
				}

				D3DXVECTOR2 size = SPRITE_SIZE * m_fSize;

				m_pScoreItem[nDegitCnt]->SetSize(size);
				m_pScoreItem[nDegitCnt]->SetSizeOrigin(size);

				CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

				if (nDegitNum[nDegitCnt] != 11)
				{
					D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f,0.0f), D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,0.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f ,1.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,1.0f) };
					m_pScoreItem[nDegitCnt]->SetTex(uvpos);
				}
			}
		}
		else if (nCntItem == ITEM_DATE)
		{//日付データの初期化
			difItemPos = MyLib::Vector3(140.0f * m_fSize, 65.0f * m_fSize, 0.0f);
			int nDegitNum[MAX_DATE_DEGIT + 1];

			m_posItem[nCntItem] = m_posAll + difItemPos;

			//日付データのみ別管理のためnullptrを代入
			m_pItem[nCntItem] = nullptr;

			//日付データを桁ごとに分解
			int nYear = m_nDate[0];
			for (int nCnt=0,nDegitCnt = 10; nDegitCnt > 6; nDegitCnt--,nCnt++)
			{
				nDegitNum[nDegitCnt] = nYear / pow(10,4- nCnt-1);
				nYear = nYear - pow(10, 4 - nCnt-1) * nDegitNum[nDegitCnt];
			}

			nDegitNum[6] = 11;

			int nMonth = m_nDate[1];
			for (int nCnt=0,nDegitCnt = 5; nDegitCnt > 3; nDegitCnt--,nCnt++)
			{
				nDegitNum[nDegitCnt] = nMonth / pow(10, 2-nCnt - 1);
				nMonth = nMonth - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			nDegitNum[3] = 11;
			
			int nDay = m_nDate[2];
			for (int nCnt=0,nDegitCnt = 2; nDegitCnt > 0; nDegitCnt--,nCnt++)
			{
				nDegitNum[nDegitCnt] = nDay / pow(10, 2 - nCnt - 1);
				nDay = nDay - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			//各桁の初期化
			for (int nCnt = 0, nDegitCnt = MAX_DATE_DEGIT; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				MyLib::Vector3 difDegitPos = MyLib::Vector3(50.0f * nCnt *0.5f* m_fSize, 0.0f, 0.0f);
				m_pDateItem[nDegitCnt] = CObject2D::Create(GetPriority());

				m_pDateItem[nDegitCnt]->SetPosition(m_posItem[nCntItem] + difDegitPos);

				if (nDegitNum[nDegitCnt] == 11)
				{
					int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
					m_pDateItem[nDegitCnt]->BindTexture(nIdxTex);
				}
				else
				{
					int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem - 1]);
					m_pDateItem[nDegitCnt]->BindTexture(nIdxTex);
				}
				
				D3DXVECTOR2 size = SPRITE_SIZE *0.5f* m_fSize;

				m_pDateItem[nDegitCnt]->SetSize(size);
				m_pDateItem[nDegitCnt]->SetSizeOrigin(size);

				CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

				if (nDegitNum[nDegitCnt] != 11)
				{
					D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f,0.0f), D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,0.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f ,1.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,1.0f) };
					m_pDateItem[nDegitCnt]->SetTex(uvpos);
				}
			}
		}

		else if (nCntItem == ITEM_ALLRANK_DATA)
		{//総評ランクアイコンの初期化
			m_pItem[nCntItem] = CObject2D::Create(GetPriority());

			difItemPos = MyLib::Vector3(700.0f * m_fSize, 0.0f, 0.0f);

			m_posItem[nCntItem] = m_posAll + difItemPos;

			m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

			int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
			m_pItem[nCntItem]->BindTexture(nIdxTex);

			D3DXVECTOR2 size = SPRITE_SIZE*m_fSize;

			m_pItem[nCntItem]->SetSize(size);
			m_pItem[nCntItem]->SetSizeOrigin(size);

			CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);
			
			D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(0.0f,m_nAllRank * 0.25f), D3DXVECTOR2(1.0f,m_nAllRank * 0.25f),  D3DXVECTOR2(0.0f,m_nAllRank * 0.25f+0.25f),  D3DXVECTOR2(1.0f,m_nAllRank * 0.25f+0.25f) };
			m_pItem[nCntItem]->SetTex(uvpos);
		}
		else if (nCntItem == ITEM_YOU)
		{//YOUアイコンの初期化
			if (m_bUpdate == true)
			{
				m_pItem[nCntItem] = CObject2D::Create(GetPriority());

				difItemPos = MyLib::Vector3(600.0f * m_fSize, 55.0f * m_fSize, 0.0f);

				m_posItem[nCntItem] = m_posAll + difItemPos;

				m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

				int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
				m_pItem[nCntItem]->BindTexture(nIdxTex);

				D3DXVECTOR2 size = ITEM_SIZE * m_fSize;

				size.x = size.x * 2;
				size.y = size.y*0.8f;

				m_pItem[nCntItem]->SetSize(size);
				m_pItem[nCntItem]->SetSizeOrigin(size);

				CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);
			}
			else
			{
				m_pItem[nCntItem] = nullptr;
			}
		}
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRankingItem_top3::Uninit()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem]!=nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}

	for (int nCntScoreDegit = 0; nCntScoreDegit <= MAX_SCORE_DEGIT; nCntScoreDegit++)
	{
		if (m_pScoreItem[nCntScoreDegit] != nullptr)
		{
			m_pScoreItem[nCntScoreDegit]->Uninit();
			m_pScoreItem[nCntScoreDegit] = nullptr;
		}
	}

	for (int nCntDateDegit = 0; nCntDateDegit <= MAX_SCORE_DEGIT; nCntDateDegit++)
	{
		if (m_pDateItem[nCntDateDegit] != nullptr)
		{
			m_pDateItem[nCntDateDegit]->Uninit();
			m_pDateItem[nCntDateDegit] = nullptr;
		}
	}

	Release();
}

//==========================================================================
// 削除
//==========================================================================
void CRankingItem_top3::Kill()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}

	for (int nCntScoreDegit = 0; nCntScoreDegit <= MAX_SCORE_DEGIT; nCntScoreDegit++)
	{
		if (m_pScoreItem[nCntScoreDegit] != nullptr)
		{
			m_pScoreItem[nCntScoreDegit]->Uninit();
			m_pScoreItem[nCntScoreDegit] = nullptr;
		}
	}

	for (int nCntDateDegit = 0; nCntDateDegit <= MAX_SCORE_DEGIT; nCntDateDegit++)
	{
		if (m_pDateItem[nCntDateDegit] != nullptr)
		{
			m_pDateItem[nCntDateDegit]->Uninit();
			m_pDateItem[nCntDateDegit] = nullptr;
		}
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CRankingItem_top3::Update()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//項目の更新
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			D3DXVECTOR2 size = m_pItem[nCntItem]->GetSize();
			size.x = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.x * 10.0f, ITEM_SIZE.x, 0.0f, TIME_SET, m_fStateTime);
			size.y = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.y * 10.0f, ITEM_SIZE.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(size);

			D3DXVECTOR2 sizetext = m_pItem[nCntItem]->GetSize();
			D3DXVECTOR2 sizetextOR = m_pItem[nCntItem]->GetSizeOrigin();
			sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
			sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(sizetext);

			//位置の更新
			m_posItem[nCntItem] += m_moveAll;
			m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

			m_pItem[nCntItem]->Update();
		}
		if (nCntItem == ITEM_TIME)
		{
			//位置の更新
			m_posItem[nCntItem] += m_moveAll;

			//各桁の更新
			for (int nCnt = 0, nDegitCnt = MAX_SCORE_DEGIT; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				MyLib::Vector3 difDegitPos = MyLib::Vector3(50.0f * nCnt, 0.0f, 0.0f);

				m_pScoreItem[nDegitCnt]->SetPosition(m_posItem[nCntItem] + difDegitPos);
				m_pScoreItem[nDegitCnt]->Update();
			}
		}
		if (nCntItem == ITEM_DATE)
		{
			//位置の更新
			m_posItem[nCntItem] += m_moveAll;

			//各桁の更新
			for (int nCnt = 0, nDegitCnt = MAX_DATE_DEGIT; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				MyLib::Vector3 difDegitPos = MyLib::Vector3(50.0f * nCnt*0.5f, 0.0f, 0.0f);

				m_pDateItem[nDegitCnt]->SetPosition(m_posItem[nCntItem] + difDegitPos);
				m_pDateItem[nDegitCnt]->Update();
			}
		}
	}
	m_posAll += m_moveAll;

	
	
}
//==========================================================================
// 描画処理
//==========================================================================
void CRankingItem_top3::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// オブジェクト2Dの描画

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Draw();
		}
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
//==========================================================================
// 移動値セット処理
//==========================================================================
void CRankingItem_top3::SetMove(MyLib::Vector3 move)
{
	m_moveAll = move;
}
//==========================================================================
// 移動値セット処理
//==========================================================================
MyLib::Vector3 CRankingItem_top3::GetPos()
{
	return m_posAll;
}
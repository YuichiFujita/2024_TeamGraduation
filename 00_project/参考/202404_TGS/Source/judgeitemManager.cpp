//=============================================================================
// 
//  ジャッジアイテムマネージャ処理 [judgeitemManager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "judgeitemManager.h"
#include "judgeitem.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "gamemanager.h"
#include "judgeobj.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float CHANGE_POINT[] =	// ポイント変更する基準
	{
		0.0f,	// 0割でDDD
		0.3f,	// 3割でCCC
		0.5f,	// 5割でBBB
		0.8f,	// 8割でAAA
	};

	const int ADDPOINT[] =
	{
		15,	// AAA
		5,	// BBB
		-5,	// CCC
		-10,// DDD
	};
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CJudgeItemManager> CJudgeItemManager::m_List = {};	// リスト
std::map<int, CListManager<CJudgeItemManager>> CJudgeItemManager::m_ListBlock = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeItemManager::CJudgeItemManager() : CObject()
{
	// 値のクリア
	m_fStartLength = 0.0f;	// 初期地点長さ
	m_Top = MyLib::Vector3();		// 先頭
	m_Cur = MyLib::Vector3();		// 終端
	m_vecJudgeInfo.clear();	// ジャッジ情報
	m_vecJudgeItem.clear();		// ジャッジアイテム
	m_bCheckCollision = false;	// コリジョンチェック判定
	m_bBindPoint = false;		// ポイント反映
	m_nMyBlockIdx = 0;			// ブロックインデックス

}

//==========================================================================
// 生成
//==========================================================================
CJudgeItemManager* CJudgeItemManager::Create(const std::vector<CMapBlockInfo::SJudgeInfo>& vecJudge, float startLen, int mapBlock)
{
	// インスタンス生成
	CJudgeItemManager* pJudge = DEBUG_NEW CJudgeItemManager;

	if (pJudge != nullptr)
	{
		// 引数情報設定
		pJudge->m_vecJudgeInfo = vecJudge;
		pJudge->m_fStartLength = startLen;	// 初期地点長さ
		pJudge->m_nMyBlockIdx = mapBlock;	// マップブロックのインデックス

		// 初期化処理
		pJudge->Init();
	}

	return pJudge;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudgeItemManager::Init()
{
	// リストに追加
	m_List.Regist(this);
	m_ListBlock[m_nMyBlockIdx].Regist(this);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ジャッジ生成
	int minIdx = 0, maxIdx = 0;
	float minX = 99990.0f, maxX = -99990.0f;

	int i = 0;
	for (const auto& vecInfo : m_vecJudgeInfo)
	{
		CJudgeItem* pItem = CJudgeItem::Create(this, m_nMyBlockIdx, vecInfo.length + m_fStartLength, vecInfo.height);
		
		// 追加
		m_vecJudgeItem.push_back(pItem);

		MyLib::Vector3 pos = pItem->GetPosition();

		if (pos.x < minX)
		{// 最小値更新
			m_Top = pos;
			minX = pos.x;
		}

		if (pos.x > maxX)
		{// 最大値更新
			m_Cur = pos;
			maxX = pos.x;
		}

		// インデックス更新
		i++;
	}

	return S_OK;
}

//==========================================================================
// リセット
//==========================================================================
void CJudgeItemManager::Reset()
{
	// アイテムのリストクリア
	m_vecJudgeItem.clear();

	// ジャッジ生成
	int minIdx = 0, maxIdx = 0;
	float minX = 99990.0f, maxX = -99990.0f;

	int i = 0;
	for (const auto& vecInfo : m_vecJudgeInfo)
	{
		CJudgeItem* pItem = CJudgeItem::Create(this, m_nMyBlockIdx, vecInfo.length + m_fStartLength, vecInfo.height);

		// 追加
		m_vecJudgeItem.push_back(pItem);

		MyLib::Vector3 pos = pItem->GetPosition();

		if (pos.x < minX)
		{// 最小値更新
			m_Top = pos;
			minX = pos.x;
		}

		if (pos.x > maxX)
		{// 最大値更新
			m_Cur = pos;
			maxX = pos.x;
		}

		// インデックス更新
		i++;
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeItemManager::Uninit()
{
	// リストから削除
	m_List.Delete(this);
	m_ListBlock[m_nMyBlockIdx].Delete(this);
}


//==========================================================================
// ジャッジアイテムの削除
//==========================================================================
void CJudgeItemManager::DeleteJudgeItem(CJudgeItem* pItem)
{
	std::vector<CJudgeItem*>::iterator itr = std::find(m_vecJudgeItem.begin(), m_vecJudgeItem.end(), pItem);
	(*itr) = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CJudgeItemManager::Update()
{
	if (IsDeath()) return;

	// コリジョンチェック判定
	m_bCheckCollision = CheckCollisionRange();

	// ポイント反映
	BindPoint();
}

//==========================================================================
// コリジョン範囲確認
//==========================================================================
bool CJudgeItemManager::CheckCollisionRange()
{
	// プレイヤーの位置取得
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return false;
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();

	return (m_Top.x <= posPlayer.x && m_Cur.x >= posPlayer.x);
}

//==========================================================================
// ポイント反映
//==========================================================================
void CJudgeItemManager::BindPoint()
{
	if (m_bBindPoint) return;

	// プレイヤーの位置取得
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return;
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();

	// 最後をまだ超えていない
	if (posPlayer.x < m_Cur.x + 200.0f) return;

	// 取得数計算
	int getNum = 0, allNum = static_cast<int>(m_vecJudgeItem.size());
	for (const auto& item : m_vecJudgeItem)
	{
		if (item == nullptr)
		{// 消えてる
			getNum++;
		}
	}

	// 取得割合
	float ratio = static_cast<float>(getNum) / static_cast<float>(allNum);

	int rank = 4;
	for (int i = 0; i < 4; i++)
	{
		if (CHANGE_POINT[i] <= ratio)
		{
			rank--;
		}
	}

	if (rank == 4)
	{
		rank = 3;
	}


	// 評価生成
	CJudge::JUDGE judgeResult = static_cast<CJudge::JUDGE>(rank);

	// 評価ごとのUI生成
	CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), judgeResult);
	pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
	pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

	// ポイント加算
	CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[judgeResult]);

	// 割り当てたよ
	m_bBindPoint = true;
}

//==========================================================================
// 描画処理
//==========================================================================
void CJudgeItemManager::Draw()
{

}

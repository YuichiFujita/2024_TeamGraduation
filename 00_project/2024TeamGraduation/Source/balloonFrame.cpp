//==========================================================================
// 
//  吹き出しの枠処理 [balloonFrame.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "balloonFrame.h"
#include "object2D.h"
#include "2D_effect.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE[CGameManager::ETeamSide::SIDE_MAX] =	// テクスチャ
	{
		"data\\TEXTURE\\speech\\balloon_left.png",
		"data\\TEXTURE\\speech\\balloon_right.png",
	};
	const int DIVISION_WIDTH = 4;					// 横分割
	const int BLOCK_WIDTH = DIVISION_WIDTH + 1;		// 横のブロック
	const float FRAME_WIDTH = 8.0f;				// 枠の幅

	// インデックスバッファ
	const WORD IDXBUFF[] =
	{
	0, 1, 4,   // 上部枠
	1, 5, 4,
	2, 6, 3,   // 下部枠
	3, 6, 7,
	0, 4, 2,   // 左側枠
	2, 4, 6,
	1, 3, 5,   // 右側枠
	3, 7, 5
	};

	const int NUM_INDICES = 24; // インデックスの個数
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBalloonFrame::CBalloonFrame(int nPriority) : CObject2D(nPriority)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CBalloonFrame::~CBalloonFrame()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CBalloonFrame* CBalloonFrame::Create(const MyLib::Vector2& size, CGameManager::ETeamSide side)
{
	// メモリの確保
	CBalloonFrame* pObj = DEBUG_NEW CBalloonFrame;

	if (pObj != nullptr)
	{
		// 引数情報設定
		pObj->m_TextSize = size;
		pObj->m_TeamSide = side;		// チームサイド

		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBalloonFrame::Init()
{
	// 中央に設定
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// 埋めるポリゴン生成
	CreateFillPoly();

	// 尾生成
	CreateTail();

	// 初期化処理
	CObject2D::Init(8);

	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_NONE);

	// 色設定
	SetColor(MyLib::color::Black());

	// インデックス生成
	CreateIndex();

	// サイズに応じて枠
	SetVtx();

	return S_OK;
}

//==========================================================================
// 埋めるポリゴン生成
//==========================================================================
void CBalloonFrame::CreateFillPoly()
{
	// 生成処理
	m_pFillPoly = CObject2D::Create(UtilFunc::Transformation::Clamp(GetPriority() - 1, 0, mylib_const::PRIORITY_NUM));
	m_pFillPoly->SetType(CObject::TYPE::TYPE_NONE);

	// 色設定
	m_pFillPoly->SetColor(MyLib::color::White());

	// 位置設定
	m_pFillPoly->SetPosition(GetPosition());

	// サイズ設定
	m_pFillPoly->SetSize(m_TextSize * 0.5f);
}

//==========================================================================
// 尾生成
//==========================================================================
void CBalloonFrame::CreateTail()
{
	// 生成処理
	m_pTail = CObject2D::Create(UtilFunc::Transformation::Clamp(GetPriority() + 1, 0, mylib_const::PRIORITY_NUM));
	m_pTail->SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_TeamSide]);
	m_pTail->BindTexture(texID);

	// 色設定
	m_pTail->SetColor(MyLib::color::White());

	// 位置設定
	m_pTail->SetPosition(GetPosition() + MyLib::Vector3(0.0f, m_pFillPoly->GetSize().y - FRAME_WIDTH * 0.5f, 0.0f));

	// サイズ設定
	m_pTail->SetSize(MyLib::Vector2(150.0f));
}

//==========================================================================
// インデックス生成
//==========================================================================
HRESULT CBalloonFrame::CreateIndex()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumIndex = NUM_INDICES;	// インデックス数

	// インデックスバッファの生成
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * NUM_INDICES,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr
	);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// インデックスバッファにコピー
	void* pIndices = nullptr;
	m_pIdxBuff->Lock(0, 0, (void**)&pIndices, 0);
	memcpy(pIndices, IDXBUFF, sizeof(IDXBUFF));
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBalloonFrame::Uninit()
{
	// インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CBalloonFrame::Kill()
{
	// 削除
	SAFE_KILL(m_pFillPoly);
	SAFE_KILL(m_pTail);

	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBalloonFrame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 頂点座標
	SetVtx();

	// 他のオブジェクトの更新
	m_pFillPoly->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pTail->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CBalloonFrame::Draw()
{
	// 埋めるポリゴンの描画
	m_pFillPoly->Draw();

	// インデックスバッファ描画
	DrawIdxBuff();

	// 尾の描画
	m_pTail->Draw();
}

//==========================================================================
// インデックスバッファ描画
//==========================================================================
void CBalloonFrame::DrawIdxBuff()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, GetVtxBuff(), 0, sizeof(VERTEX_2D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(GetIdxTexture()));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,	// プリミティブタイプ
		0,					// BaseVertexIndex
		0,					// MinIndex
		8,					// NumVertices
		0,					// StartIndex
		8					// PrimitiveCount
	);
}

//==========================================================================
// 頂点座標
//==========================================================================
void CBalloonFrame::SetVtx()
{
	// 情報取得
	MyLib::Vector3 pos = GetPosition();		// 位置
	D3DXCOLOR col = GetColor();				// 色

	// テキストの幅の対角線を取る
	float distance = sqrtf(m_TextSize.x * m_TextSize.x + m_TextSize.y * m_TextSize.y);
	float angle = atan2f(m_TextSize.x, m_TextSize.y);	// 対角線の向き

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < 8; i++)
	{// 横の頂点数分繰り返す

		pVtx[i].rhw = 1.0f;
		pVtx[i].col = col;
	}

	//-----------------------------
	// 四角形
	//-----------------------------
	for (int nCntHeight = 0; nCntHeight < 2; nCntHeight++)
	{// 縦の頂点数分繰り返す

		// 外側と内側の長さ
		float fOuterScale = (nCntHeight == 0) ? 1.0f : 1.0f - (FRAME_WIDTH / m_TextSize.x);
		float fInnerScale = (nCntHeight == 0) ? 1.0f : 1.0f - (FRAME_WIDTH / m_TextSize.y);

		// 頂点を計算
		pVtx[0].pos = pos + MyLib::Vector3   // 左上
		(
			-m_TextSize.x * 0.5f * fOuterScale,
			-m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);
		pVtx[1].pos = pos + MyLib::Vector3   // 右上
		(
			m_TextSize.x * 0.5f * fOuterScale,
			-m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);
		pVtx[2].pos = pos + MyLib::Vector3   // 左下
		(
			-m_TextSize.x * 0.5f * fOuterScale,
			m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);
		pVtx[3].pos = pos + MyLib::Vector3   // 右下
		(
			m_TextSize.x * 0.5f * fOuterScale,
			m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);

		// 次の頂点バッファ位置に進む
		pVtx += 4;
	}

	// 頂点バッファをアンロックロック
	GetVtxBuff()->Unlock();
}

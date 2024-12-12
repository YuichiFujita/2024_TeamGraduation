//==========================================================================
// 
//  吹き出しの枠処理 [balloonFrame.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "balloonFrame.h"
#include "object2D.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\title\\line.png";	// テクスチャ
	const int DIVISION_WIDTH = 4;					// 横分割
	const int BLOCK_WIDTH = DIVISION_WIDTH + 1;		// 横のブロック
	const float FRAME_WIDTH = 10.0f;				// 枠の幅

	const MyLib::Vector2 UVLOOP = MyLib::Vector2(40.0f, 0.0f);	// UVループ
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
CBalloonFrame* CBalloonFrame::Create(const MyLib::Vector2& size)
{
	// メモリの確保
	CBalloonFrame* pObj = DEBUG_NEW CBalloonFrame;

	if (pObj != nullptr)
	{
		// 引数情報設定
		pObj->m_TextSize = size;

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
	// 初期化処理
	CObject2D::Init();

	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// サイズに応じて枠
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBalloonFrame::Uninit()
{
	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CBalloonFrame::Kill()
{
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
}

//==========================================================================
// 頂点座標
//==========================================================================
void CBalloonFrame::SetVtx()
{
	// 情報取得
	MyLib::Vector3 pos = GetPosition();		// 位置
	D3DXCOLOR col = GetColor();				// 色
	D3DXVECTOR2* pVtxTex = GetTex();		// テクスチャ座標取得

	// 頂点情報の設定
	int i = 0;
	float u = 0.0f;
	float fRotWidth = (D3DX_PI * 2) / (float)(DIVISION_WIDTH);		// 1分割数あたりの角度割合

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < 2; nCntHeight++)
	{// 縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < BLOCK_WIDTH; nCntWidth++)
		{// 横の頂点数分繰り返す

			// 頂点座標の設定
			float fLength = (FRAME_WIDTH * 0.5f - nCntHeight * (FRAME_WIDTH * 0.5f));
			pVtx[i].pos = pos + m_TextSize + MyLib::Vector3
			(
				sinf(nCntWidth % BLOCK_WIDTH * fRotWidth) * fLength,
				cosf(nCntWidth % BLOCK_WIDTH * fRotWidth) * fLength,
				0.0f
			);

			// rhwの設定
			pVtx[i].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[i].col = col;

			// UV座標
			pVtx[i].tex = D3DXVECTOR2
			(
				u,
				(float)nCntHeight
			);

			u += fLength;	// スクロール
		}
	}

	// 頂点バッファをアンロックロック
	GetVtxBuff()->Unlock();
}

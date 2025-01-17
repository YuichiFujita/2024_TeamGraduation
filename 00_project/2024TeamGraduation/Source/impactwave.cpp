//=============================================================================
// 
//  メッシュシリンダー処理 [impactwave.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "impactwave.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define TEXTURE		"data\\TEXTURE\\field002.png"
#define POS_IMPACTWAVE	(5000.0f)
#define POS_IMPACTWAVE_Y	(3000.0f)
#define WIDTH	(32)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CImpactWave::m_apFilename[] =	//ファイル読み込み
{
	"data\\TEXTURE\\GRADATION\\black_01.jpg",
	"data\\TEXTURE\\GRADATION\\black_02.jpg",
	"data\\TEXTURE\\GRADATION\\orange_01.jpg",
	"data\\TEXTURE\\GRADATION\\orange_02.jpg",
	"data\\TEXTURE\\GRADATION\\blue_01.jpg",
	"data\\TEXTURE\\GRADATION\\blue_02.jpg",
	"data\\TEXTURE\\GRADATION\\green_01.jpg",
	"data\\TEXTURE\\GRADATION\\green_02.jpg",
	"data\\TEXTURE\\GRADATION\\purple_01.jpg",
	"data\\TEXTURE\\GRADATION\\purple_02.jpg",
	"data\\TEXTURE\\GRADATION\\purple_03.jpg",
	"data\\TEXTURE\\GRADATION\\purple_04.jpg",
	"data\\TEXTURE\\GRADATION\\pink_01.jpg",
	"data\\TEXTURE\\effect\\effect003.tga",
	"data\\TEXTURE\\GRADATION\\giza_01.png",
	"data\\TEXTURE\\GRADATION\\giza_02.png",
	"data\\TEXTURE\\GRADATION\\giza_03.png",
	"data\\TEXTURE\\GRADATION\\giza_06.png",
	"data\\TEXTURE\\GRADATION\\giza_05.png",
	"data\\TEXTURE\\GRADATION\\gizagradation.png",
	"data\\TEXTURE\\GRADATION\\wave_01.png",
};

//==========================================================================
// コンストラクタ
//==========================================================================
CImpactWave::CImpactWave(int nPriority) : CObject3DMesh(nPriority)
{
	m_nTexIdx = 0;			// テクスチャのインデックス番号
	m_fLife = 0.0f;			// 寿命
	m_fMaxLife = 0.0f;		// 最大寿命
	m_nTexDivision = 0;		// テクスチャ分割
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 元の色
	m_fOutWidth = 0.0f;		// 横幅(外)
	m_fInWidth = 0.0f;		// 幅(内)
	m_fRotWidth = 0.0f;		// 1分割数あたりの角度割合
	m_fHeight = 0.0f;		// 高さ
	m_fMove = 0.0f;			// 広がる速度
	m_bAddBlend = false;	// 加算合成の判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CImpactWave::~CImpactWave()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CImpactWave *CImpactWave::Create(MyLib::Vector3 pos, MyLib::Vector3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fCenterDistance, float fLife, float fMove, int nTexType, bool bAddBlend)
{
	// 生成用のオブジェクト
	CImpactWave *pObjMeshCylinder = nullptr;

	if (pObjMeshCylinder == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjMeshCylinder = DEBUG_NEW CImpactWave;

		if (pObjMeshCylinder != nullptr)
		{// メモリの確保が出来ていたら

			// 位置・向き
			pObjMeshCylinder->SetPosition(pos);
			pObjMeshCylinder->SetRotation(rot);
			pObjMeshCylinder->SetColor(col);
			pObjMeshCylinder->m_colOrigin = col;
			pObjMeshCylinder->SetWidthBlock(WIDTH);
			pObjMeshCylinder->SetHeightBlock(1);
			pObjMeshCylinder->SetWidthLen(POS_IMPACTWAVE);
			pObjMeshCylinder->SetHeightLen(POS_IMPACTWAVE_Y);

			pObjMeshCylinder->m_fOutWidth = fWidth + fCenterDistance;		// 横幅(外)
			pObjMeshCylinder->m_fInWidth = fWidth;		// 幅(内)
			pObjMeshCylinder->m_fHeight = fHeight;		// 高さ
			pObjMeshCylinder->m_fLife = fLife;			// 寿命
			pObjMeshCylinder->m_fMove = fMove;			// 広がる速度
			pObjMeshCylinder->m_bAddBlend = bAddBlend;	// 加算合成の判定

			// テクスチャの割り当て
			pObjMeshCylinder->m_nTexIdx = CTexture::GetInstance()->Regist(m_apFilename[nTexType]);

			// テクスチャの割り当て
			pObjMeshCylinder->BindTexture(pObjMeshCylinder->m_nTexIdx);

			// 初期化処理
			pObjMeshCylinder->Init();
		}

		return pObjMeshCylinder;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CImpactWave::Init()
{
	HRESULT hr;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	m_fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1分割数あたりの角度割合
	m_fMaxLife = m_fLife;	// 最大寿命
	m_nTexDivision = 4;		// テクスチャ分割

	// オブジェクト3Dメッシュの初期化処理
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_DONUTS);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	MyLib::Vector3 *pVtxPos = GetVtxPos();	// 頂点座標取得
	D3DXVECTOR2 *pVtxTex = GetVtxTex();	// テクスチャ座標取得

	// 分割数取得
	int nHeightBlock = GetHeightBlock();
	int nWidthBlock = GetWidthBlock();

	// 頂点情報の設定
	for (int nCntHeight = 0; nCntHeight < nHeightBlock + 1; nCntHeight++)
	{// 縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < nWidthBlock + 1; nCntWidth++)
		{// 横の頂点数分繰り返す

			float fLength = (m_fOutWidth - nCntHeight * m_fInWidth);
			if (fLength <= 0.0f)
			{
				fLength = 0.0f;
			}

			// 頂点座標の設定
			pVtxPos[nCntWidth + (nCntHeight * (nWidthBlock + 1))] = MyLib::Vector3
			(
				sinf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength,
				nCntHeight * m_fHeight,
				cosf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength
			);

			pVtxTex[nCntWidth + (nCntHeight * (nWidthBlock + 1))] = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(nWidthBlock / m_nTexDivision)),
				nCntHeight * (1.0f / (float)(nHeightBlock))
			);
		}
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CImpactWave::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CImpactWave::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 色取得
	D3DXCOLOR col = GetColor();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	// 位置更新
	pos += move * fDeltaRate * fSlowRate;

	SetPosition(pos);
	SetMove(move);

	// 広げていく
	m_fOutWidth += m_fMove * fDeltaRate * fSlowRate;

	// 寿命更新
	m_fLife -= fDeltaTime * fSlowRate;

	// 不透明度更新
	col.a = m_colOrigin.a * m_fLife / m_fMaxLife;

	// 色設定
	SetColor(col);

	// 頂点情報設定
	SetVtx();
	
	if (m_fLife <= 0.0f)
	{// 寿命が尽きた

		// 終了処理
		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CImpactWave::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();


	// 背面のカリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αブレンディングを加算合成に設定
	if (m_bAddBlend)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject3DMesh::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングのデフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CImpactWave::SetVtx()
{
	MyLib::Vector3 *pVtxPos = GetVtxPos();	// 頂点座標取得
	D3DXVECTOR2 *pVtxTex = GetVtxTex();	// テクスチャ座標取得

	// 分割数取得
	int nHeightBlock = GetHeightBlock();
	int nWidthBlock = GetWidthBlock();

	// 頂点情報の設定
	for (int nCntHeight = 0; nCntHeight < nHeightBlock + 1; nCntHeight++)
	{// 縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < nWidthBlock + 1; nCntWidth++)
		{// 横の頂点数分繰り返す

			float fLength = (m_fOutWidth - nCntHeight * m_fInWidth);
			if (fLength <= 0.0f)
			{
				fLength = 0.0f;
			}

			// 頂点座標の設定
			pVtxPos[nCntWidth + (nCntHeight * (nWidthBlock + 1))] = MyLib::Vector3
			(
				sinf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength,
				nCntHeight * m_fHeight,
				cosf(nCntWidth % nWidthBlock * m_fRotWidth) * fLength
			);
		}
	}

	// 頂点座標設定
	SetVtxPos(pVtxPos);

	// 頂点情報更新
	CObject3DMesh::SetVtx();
}

//==========================================================================
// 寿命設定
//==========================================================================
void CImpactWave::SetLife(const float fLife)
{
	m_fLife = fLife;
}

//==========================================================================
// 寿命取得
//==========================================================================
float CImpactWave::GetLife() const
{
	return m_fLife;
}

//==========================================================================
// 最大寿命設定
//==========================================================================
void CImpactWave::SetMaxLife(const float fLife)
{
	m_fMaxLife = fLife;
}

//==========================================================================
// 最大寿命取得
//==========================================================================
float CImpactWave::GetMaxLife() const
{
	return m_fMaxLife;
}

//==========================================================================
// 元の色設定
//==========================================================================
void CImpactWave::SetColOrigin(const D3DXCOLOR col)
{
	m_colOrigin = col;
}

//==========================================================================
// 元の色取得
//==========================================================================
D3DXCOLOR CImpactWave::GetColOrigin() const
{
	return m_colOrigin;
}

//==========================================================================
// 外周設定
//==========================================================================
void CImpactWave::SetOutWidth(const float fOutWidth)
{
	m_fOutWidth = fOutWidth;
}

//==========================================================================
// 外周取得
//==========================================================================
float CImpactWave::GetOutWidth() const
{
	return m_fOutWidth;
}

//==========================================================================
// 内周設定
//==========================================================================
void CImpactWave::SetInWidth(const float fInWidth)
{
	m_fInWidth = fInWidth;
}

//==========================================================================
// 内周取得
//==========================================================================
float CImpactWave::GetInWidth() const
{
	return m_fInWidth;
}

//==========================================================================
// 一分割あたりの角度設定
//==========================================================================
void CImpactWave::SetRotWidth(const float fRot)
{
	m_fRotWidth = fRot;
}

//==========================================================================
// 一分割あたりの角度取得
//==========================================================================
float CImpactWave::GetRotWidth() const
{
	return m_fRotWidth;
}

//==========================================================================
// サイズの移動量設定
//==========================================================================
void CImpactWave::SetMoveSize(const float fMoveSize)
{
	m_fMove = fMoveSize;
}

//==========================================================================
// サイズの移動量取得
//==========================================================================
float CImpactWave::GetMoveSize() const
{
	return m_fMove;
}

//==========================================================================
// テクスチャ分割数設定
//==========================================================================
void CImpactWave::SetTexDivision(const int nDivision)
{
	m_nTexDivision = nDivision;
}

//==========================================================================
// テクスチャ分割数取得
//==========================================================================
int CImpactWave::GetTexDivision() const
{
	return m_nTexDivision;
}

//==========================================================================
// 加算合成の設定
//==========================================================================
void CImpactWave::SetIsAddBlend(bool bAddBlend)
{
	m_bAddBlend = bAddBlend;
}

//==========================================================================
// ファイル名取得
//==========================================================================
const char *CImpactWave::GetFileName(int nTexType)
{
	return m_apFilename[nTexType];
}

//==========================================================================
// メッシュシリンダーオブジェクトの取得
//==========================================================================
CImpactWave *CImpactWave::GetMyObject()
{
	return this;
}

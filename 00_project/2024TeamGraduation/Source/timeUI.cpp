//============================================================
//
//	タイムUI処理 [timeUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "timeUI.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "timer.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_NUM = "data\\TEXTURE\\number\\school03.png";	// 数字テクスチャ
	const char *TEXTURE_PART = "data\\TEXTURE\\timepart000.png";		// 区切りテクスチャ
	const int PRIORITY = 6;	// タイムUIの優先順位
}

//************************************************************
//	子クラス [CTimeUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTimeUI::CTimeUI() : CObject(PRIORITY, CObject::LAYER::LAYER_2D),
	m_sizeValue		(VEC3_ZERO),				// 数字の大きさ
	m_sizePart		(VEC3_ZERO),				// 区切りの大きさ
	m_spaceValue	(VEC3_ZERO),				// 数字の空白
	m_spacePart		(VEC3_ZERO),				// 区切りの空白
	m_col			(MyLib::color::White()),	// 色
	m_alignX		(XALIGN_CENTER),			// 横配置
	m_alignY		(YALIGN_CENTER),			// 縦配置
	m_fTime			(0.0f)						// 表示時間
{
	// メンバ変数をクリア
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// 区切りの情報
}

//============================================================
//	デストラクタ
//============================================================
CTimeUI::~CTimeUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTimeUI::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// 区切りの情報
	m_sizeValue		= VEC2_ZERO;					// 数字の大きさ
	m_sizePart		= VEC2_ZERO;					// 区切りの大きさ
	m_spaceValue	= VEC2_ZERO;					// 数字の空白
	m_spacePart		= VEC2_ZERO;					// 区切りの空白
	m_col			= MyLib::color::White();		// 色
	m_alignX		= XALIGN_CENTER;				// 横配置
	m_alignY		= YALIGN_CENTER;				// 縦配置
	m_fTime			= 0.0f;							// 表示時間

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の生成
		m_apValue[nCntValue] = CNumber::Create(CMultiNumber::EObjType::OBJTYPE_2D, PRIORITY);
		if (m_apValue[nCntValue] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 数字のテクスチャを割当
		m_apValue[nCntValue]->BindTexture(CTexture::GetInstance()->Regist(TEXTURE_NUM));

		// 数字の設定
		m_apValue[nCntValue]->SetNum(0);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの生成
		m_apPart[nCntPart] = CObject2D::Create(PRIORITY);
		if (m_apPart[nCntPart] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 区切りのテクスチャを割当
		m_apPart[nCntPart]->BindTexture(CTexture::GetInstance()->Regist(TEXTURE_PART));
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTimeUI::Uninit(void)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の終了
		SAFE_UNINIT(m_apValue[nCntValue]);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの終了
		SAFE_UNINIT(m_apPart[nCntPart]);
	}

	// オブジェクトの破棄
	CObject::Release();
}

//============================================================
// 動的削除処理
//============================================================
void CTimeUI::Kill()
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の終了
		SAFE_UNINIT(m_apValue[nCntValue]);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの終了
		SAFE_UNINIT(m_apPart[nCntPart]);
	}

	// 自身の終了
	CTimeUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTimeUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 相対位置の設定
	SetPositionRelative();

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	描画処理
//============================================================
void CTimeUI::Draw(void)
{

}

//============================================================
//	描画状況の設定処理
//============================================================
void CTimeUI::SetEnableDisp(const bool bDisp)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDisp);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の描画状況を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableDisp(bDisp);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの描画状況を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableDisp(bDisp);
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CTimeUI::SetPosition(const MyLib::Vector3& pos)
{
	// 自身の位置を設定
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	向きの設定処理
//============================================================
void CTimeUI::SetRotation(const MyLib::Vector3& rot)
{
	// 自身の向きを設定
	CObject::SetRotation(rot);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の向きを設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetRotation(rot);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの向きを設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetRotation(rot);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	生成処理
//============================================================
CTimeUI *CTimeUI::Create
(
	const float fTime,				// 表示時間
	const D3DXVECTOR3& rPos,		// 位置
	const float fHeightValue,		// 数字の大きさ
	const float fHeightPart,		// 区切りの大きさ
	const D3DXVECTOR2& rSpaceValue,	// 数字の空白
	const D3DXVECTOR2& rSpacePart,	// 区切りの空白
	const EAlignX alignX,			// 横配置
	const EAlignY alignY,			// 縦配置
	const D3DXVECTOR3& rRot,		// 向き
	const D3DXCOLOR& rCol			// 色
)
{
	// タイムUIの生成
	CTimeUI *pTimeUI = DEBUG_NEW CTimeUI;
	if (pTimeUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイムUIの初期化
		if (FAILED(pTimeUI->Init()))
		{ // 初期化に失敗した場合

			// タイムUIの破棄
			SAFE_DELETE(pTimeUI);
			return nullptr;
		}

		// 表示時間を設定
		pTimeUI->SetTime(fTime);

		// 原点位置を設定
		pTimeUI->SetPosition(rPos);

		// 原点向きを設定
		pTimeUI->SetRotation(rRot);

		// 数字の大きさを設定
		pTimeUI->SetSizeValue(fHeightValue);

		// 区切りの大きさを設定
		pTimeUI->SetSizePart(fHeightPart);

		// 数字の空白を設定
		pTimeUI->SetSpaceValue(rSpaceValue);

		// 区切りの空白を設定
		pTimeUI->SetSpacePart(rSpacePart);

		// 色を設定
		pTimeUI->SetColor(rCol);

		// 横配置を設定
		pTimeUI->SetAlignX(alignX);

		// 縦配置を設定
		pTimeUI->SetAlignY(alignY);

		// 確保したアドレスを返す
		return pTimeUI;
	}
}

//============================================================
//	表示時間の設定処理
//============================================================
void CTimeUI::SetTime(const float fTime)
{
	// 引数の表示時間を設定
	m_fTime = fTime;

	// 時間を補正
	UtilFunc::Transformation::ValueNormalize(m_fTime, timer::TIME_MAX, timer::TIME_MIN);
}

//============================================================
//	数字のテクスチャインデックスの設定処理
//============================================================
void CTimeUI::BindTextureValue(const int nTexIdx)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字のテクスチャインデックスを設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->BindTexture(nTexIdx);
	}
}

//============================================================
//	区切りのテクスチャインデックスの設定処理
//============================================================
void CTimeUI::BindTexturePart(const int nTexIdx)
{
	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りのテクスチャインデックスを設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->BindTexture(nTexIdx);
	}
}

//============================================================
//	数字の大きさの設定処理
//============================================================
void CTimeUI::SetSizeValue(const float fHeight)
{
	// 縦幅を元にサイズを設定
	const int nTexID = m_apValue[0]->GetIdxTexture();
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, fHeight);
	size.y *= 10.0f;

	// 設定された数字の大きさを保存
	m_sizeValue = size;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の大きさを設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetSize(size);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	区切りの大きさの設定処理
//============================================================
void CTimeUI::SetSizePart(const float fHeight)
{
	// 縦幅を元にサイズを設定
	const int nTexID = m_apPart[0]->GetIdxTexture();
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, fHeight);

	// 設定された区切りの大きさを保存
	m_sizePart = size;

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの大きさを設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetSize(size);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	数字の大きさの設定処理
//============================================================
void CTimeUI::SetSizeValue(const D3DXVECTOR2& rSize)
{
	// 設定された数字の大きさを保存
	m_sizeValue = rSize;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の大きさを設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetSize(rSize);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	区切りの大きさの設定処理
//============================================================
void CTimeUI::SetSizePart(const D3DXVECTOR2& rSize)
{
	// 設定された区切りの大きさを保存
	m_sizePart = rSize;

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの大きさを設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetSize(rSize);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	数字の空白の設定処理
//============================================================
void CTimeUI::SetSpaceValue(const D3DXVECTOR2& rSpace)
{
	// 引数の数字の空白を設定
	m_spaceValue = rSpace;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	区切りの空白の設定処理
//============================================================
void CTimeUI::SetSpacePart(const D3DXVECTOR2& rSpace)
{
	// 引数の区切りの空白を設定
	m_spacePart = rSpace;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	色の設定処理
//============================================================
void CTimeUI::SetColor(const D3DXCOLOR& rCol)
{
	// 設定された色を保存
	m_col = rCol;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の色を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetColor(rCol);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの色を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetColor(rCol);
	}
}

//============================================================
//	透明度の設定処理
//============================================================
void CTimeUI::SetAlpha(const float fAlpha)
{
	// 透明度を設定
	D3DXCOLOR col = GetColor();	// 色
	col.a = fAlpha;

	// 色の設定
	SetColor(col);
}

//============================================================
//	横配置の設定処理
//============================================================
void CTimeUI::SetAlignX(const EAlignX align)
{
	// 引数の横配置を設定
	m_alignX = align;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	縦配置の設定処理
//============================================================
void CTimeUI::SetAlignY(const EAlignY align)
{
	// 引数の縦配置を設定
	m_alignY = align;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	タイム全体の横幅取得処理
//============================================================
float CTimeUI::GetTimeWidth(void) const
{
	float fTimeWidth = 0.0f;				// タイム全体の横幅
	int nEndNumID = timeUI::MAX_DIGIT - 1;	// 終端数字のインデックス

	// 全ての空白を加算 (最後の文字は含まない)
	fTimeWidth += m_spaceValue.x * nEndNumID;
	fTimeWidth += m_spacePart.x * timeUI::MAX_PART;

	// 先頭数字の横幅を加算
	assert(m_apValue[0] != nullptr);
	fTimeWidth += m_apValue[0]->GetSize().x * 0.5f;

	// 終端数字の横幅を加算
	assert(m_apValue[nEndNumID] != nullptr);
	fTimeWidth += m_apValue[nEndNumID]->GetSize().x * 0.5f;

	// タイム全体の横幅を返す
	return fTimeWidth;
}

//============================================================
//	タイム全体の縦幅取得処理
//============================================================
float CTimeUI::GetTimeHeight(void) const
{
	float fTimeHeight = 0.0f;				// タイム全体の縦幅
	int nEndNumID = timeUI::MAX_DIGIT - 1;	// 終端数字のインデックス

	// 全ての空白を加算 (最後の文字は含まない)
	fTimeHeight += m_spaceValue.y * nEndNumID;
	fTimeHeight += m_spacePart.y * timeUI::MAX_PART;

	// 先頭数字の縦幅を加算
	assert(m_apValue[0] != nullptr);
	fTimeHeight += m_apValue[0]->GetSize().y * 0.5f;

	// 終端数字の縦幅を加算
	assert(m_apValue[nEndNumID] != nullptr);
	fTimeHeight += m_apValue[nEndNumID]->GetSize().y * 0.5f;

	// タイム全体の縦幅を返す
	return fTimeHeight;
}

//============================================================
//	数字全体の大きさ取得処理
//============================================================
D3DXVECTOR2 CTimeUI::GetTimeSize(void) const
{
	// 数字全体の大きさを返す
	return D3DXVECTOR2(GetTimeWidth(), GetTimeHeight());
}

//============================================================
//	相対位置の設定処理
//============================================================
void CTimeUI::SetPositionRelative(void)
{
	int nValueID = 0;	// 数字の生成数
	int nPartID = 0;	// 区切りの生成数
	D3DXVECTOR3 rotThis = GetRotation();	// 自身の向き
	D3DXVECTOR2 spaceValue = m_spaceValue * 0.5f;	// 数字の空白
	D3DXVECTOR2 spacePart = m_spacePart * 0.5f;		// 区切りの空白
	D3DXVECTOR2 sizeTime = GetTimeSize() * 0.5f;	// タイム全体の大きさ
	D3DXVECTOR2 sizeHead = m_apValue[0]->GetSize() * 0.5f;	// 先頭数字の大きさ
	D3DXVECTOR3 rotStart = D3DXVECTOR3(rotThis.z + HALF_PI, rotThis.z, 0.0f);	// 文字の開始向き

	D3DXVECTOR3 posOffset = VEC3_ZERO;	// 文字の開始オフセット
	posOffset.x = -sizeTime.x - spaceValue.x + sizeHead.x - (sizeTime.x * (m_alignX - 1));	// 開始オフセットX
	posOffset.y = -sizeTime.y - spaceValue.y + sizeHead.y - (sizeTime.y * (m_alignY - 1));	// 開始オフセットY

	D3DXVECTOR3 posStart = GetPosition();	// 文字の開始位置
	posStart.x += sinf(rotStart.x) * posOffset.x + sinf(rotStart.y) * posOffset.y;	// 開始位置X
	posStart.y += cosf(rotStart.x) * posOffset.x + cosf(rotStart.y) * posOffset.y;	// 開始位置Y

	for (int nCntTimer = 0; nCntTimer < timeUI::MAX_DIGIT + timeUI::MAX_PART; nCntTimer++)
	{ // 数字の数 + 区切りの数分繰り返す

		if (nCntTimer == timeUI::MAX_MIN || nCntTimer == timeUI::MAX_MIN + timeUI::MAX_SEC + 1)
		{ // 区切りタイミングの場合

			assert(m_apPart[nValueID] != nullptr);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spacePart.x + sinf(rotStart.y) * spacePart.y;
			posStart.y += cosf(rotStart.x) * spacePart.x + cosf(rotStart.y) * spacePart.y;

			// 区切りの位置を設定
			m_apPart[nValueID]->SetPosition(posStart);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spacePart.x + sinf(rotStart.y) * spacePart.y;
			posStart.y += cosf(rotStart.x) * spacePart.x + cosf(rotStart.y) * spacePart.y;

			// 区切りの生成数を加算
			nValueID++;
		}
		else
		{ // 数字タイミングの場合

			assert(m_apValue[nPartID] != nullptr);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spaceValue.x + sinf(rotStart.y) * spaceValue.y;
			posStart.y += cosf(rotStart.x) * spaceValue.x + cosf(rotStart.y) * spaceValue.y;

			// 数字の位置を設定
			m_apValue[nPartID]->SetPosition(posStart);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spaceValue.x + sinf(rotStart.y) * spaceValue.y;
			posStart.y += cosf(rotStart.x) * spaceValue.x + cosf(rotStart.y) * spaceValue.y;

			// 数字の生成数を加算
			nPartID++;
		}
	}
}

//============================================================
//	数字のテクスチャ座標の設定処理
//============================================================
void CTimeUI::SetTexNum(void)
{
	int aNumDivide[timeUI::MAX_DIGIT];	// 数値分解用

	// 分を桁数ごとに分解
	UtilFunc::Transformation::DivideDigitNum(&aNumDivide[0], GetMin(), timeUI::MAX_MIN);

	// 秒を桁数ごとに分解
	UtilFunc::Transformation::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN], GetSec(), timeUI::MAX_SEC);

	// ミリ秒を桁数ごとに分解
	UtilFunc::Transformation::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN + timeUI::MAX_SEC], GetMSec(), timeUI::MAX_MSEC);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の設定
		m_apValue[nCntValue]->SetNum(aNumDivide[nCntValue]);
	}
}

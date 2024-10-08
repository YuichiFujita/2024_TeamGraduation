//============================================================
//
//	文字列2D処理 [string2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "string2D.h"
#include "manager.h"
#include "font.h"
#include "char2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// 文字列2Dの優先順位
}

//************************************************************
//	子クラス [CString2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CString2D::CString2D() : CObject(PRIORITY, LAYER::LAYER_2D),
	m_ppChar		(nullptr),					// 文字ポリゴンの情報
	m_pFontChar		(nullptr),					// フォント文字
	m_col			(MyLib::color::White()),	// 色
	m_alignX		(XALIGN_CENTER),			// 横配置
	m_fCharHeight	(0.0f)						// 文字の縦幅
{

}

//============================================================
//	デストラクタ
//============================================================
CString2D::~CString2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CString2D::Init(void)
{
	// メンバ変数を初期化
	m_ppChar		= nullptr;					// 文字ポリゴンの情報
	m_pFontChar		= nullptr;					// フォント文字
	m_col			= MyLib::color::White();	// 色
	m_alignX		= XALIGN_CENTER;			// 横配置
	m_fCharHeight	= 0.0f;						// 文字の縦幅

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CString2D::Uninit(void)
{
	if (m_ppChar != nullptr)
	{ // 文字列が破棄されていない場合

		for (int i = 0; i < (int)m_wsStr.size(); i++)
		{ // 文字数分繰り返す

			// 文字の破棄
			SAFE_UNINIT(m_ppChar[i]);
		}
	}

	// 文字列の破棄
	SAFE_DEL_ARRAY(m_ppChar);

	// オブジェクトの破棄
	CObject::Release();
}

//============================================================
//	破棄処理
//============================================================
void CString2D::Release(void)
{
	// 自身の終了
	CString2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CString2D::Update(const float fDeltaTime)
{
	// 相対位置の設定
	SetPositionRelative();

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字の更新
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->Update(fDeltaTime);
	}
}

//============================================================
//	描画処理
//============================================================
void CString2D::Draw(void)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字の描画
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->Draw();
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CString2D::SetEnableDisp(const bool bDraw)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字の描画状況を設定
		m_ppChar[i]->SetEnableDisp(bDraw);
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CString2D::SetPosition(const MyLib::Vector3& pos)
{
	// 設定する位置を保存
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	向きの設定処理
//============================================================
void CString2D::SetRotation(const MyLib::Vector3& rot)
{
	// 設定する向きを保存
	CObject::SetRotation(rot);

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字向きの設定
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetRotation(rot);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	生成処理
//============================================================
CString2D *CString2D::Create
(
	const std::string &rFilePass,	// フォントパス
	const bool bItalic,				// イタリック
	const std::wstring &rStr,		// 指定文字列
	const D3DXVECTOR3 &rPos,		// 原点位置
	const float fHeight,			// 文字縦幅
	const EAlignX alignX,			// 横配置
	const D3DXVECTOR3& rRot,		// 原点向き
	const D3DXCOLOR& rCol			// 色
)
{
	// 文字列2Dの生成
	CString2D *pString2D = new CString2D;
	if (pString2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 文字列2Dの初期化
		if (FAILED(pString2D->Init()))
		{ // 初期化に失敗した場合

			// 文字列2Dの破棄
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// フォントを設定
		pString2D->SetFont(rFilePass, bItalic);

		// 文字列を設定
		if (FAILED(pString2D->SetString(rStr)))
		{ // 設定に失敗した場合

			// 文字列2Dの破棄
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// 原点位置を設定
		pString2D->SetPosition(rPos);

		// 原点向きを設定
		pString2D->SetRotation(rRot);

		// 色を設定
		pString2D->SetColor(rCol);

		// 文字縦幅を設定
		pString2D->SetCharHeight(fHeight);

		// 横配置を設定
		pString2D->SetAlignX(alignX);

		// 確保したアドレスを返す
		return pString2D;
	}
}

//============================================================
//	文字列の設定処理
//============================================================
HRESULT CString2D::SetString(const std::wstring& rStr)
{
	// 文字数を保存する
	int nOldStrLen = (int)m_wsStr.size();	// 破棄する文字数
	int nCurStrLen = (int)rStr.size();		// 生成する文字数

	// 指定文字列を変更
	m_wsStr = rStr;

	//--------------------------------------------------------
	//	既に使用している文字を破棄
	//--------------------------------------------------------
	for (int i = 0; i < nOldStrLen; i++)
	{ // 破棄する文字数分繰り返す

		// 文字を破棄
		SAFE_UNINIT(m_ppChar[i]);
	}

	// 文字列を破棄
	SAFE_DEL_ARRAY(m_ppChar);

	//--------------------------------------------------------
	//	新しい文字を生成
	//--------------------------------------------------------
	// 文字を格納する配列を生成
	m_ppChar = new CChar2D*[nCurStrLen];
	for (int i = 0; i < nCurStrLen; i++)
	{ // 生成する文字数分繰り返す

		// 文字を生成
		m_ppChar[i] = CChar2D::Create
		( // 引数
			m_pFontChar->GetFilePass(),	// フォントパス
			m_pFontChar->GetItalic(),	// イタリック
			m_wsStr[i],		// 指定文字
			GetPosition(),	// 原点位置
			m_fCharHeight,	// 文字縦幅
			GetRotation(),	// 原点向き
			m_col			// 色
		);
		if (m_ppChar[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// 文字の種類を指定なしにする
		m_ppChar[i]->SetType(CObject::TYPE::TYPE_NONE);
	}

	// 相対位置の設定
	SetPositionRelative();

	// 成功を返す
	return S_OK;
}

//============================================================
//	フォントの設定処理
//============================================================
void CString2D::SetFont
(
	const std::string &rFilePass,	// フォントパス
	const bool bItalic				// イタリック
)
{
	// フォント文字情報を設定
	CFont *pFont = GET_MANAGER->GetFont();	// フォント情報
	m_pFontChar = pFont->Regist(rFilePass, bItalic).pFontChar;

	// 文字列の再設定
	SetString(m_wsStr);
}

//============================================================
//	色の設定処理
//============================================================
void CString2D::SetColor(const D3DXCOLOR& rCol)
{
	// 設定する色を保存
	m_col = rCol;

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字色の設定
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetColor(rCol);
	}
}

//============================================================
//	透明度の設定処理
//============================================================
void CString2D::SetAlpha(const float fAlpha)
{
	// 引数の透明度を代入
	m_col.a = fAlpha;

	// 色の設定
	SetColor(m_col);
}

//============================================================
//	文字縦幅の設定処理
//============================================================
void CString2D::SetCharHeight(const float fHeight)
{
	// 設定する文字の縦幅を保存
	m_fCharHeight = fHeight;

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		// 文字縦幅の設定
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetCharHeight(fHeight);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	横配置の設定処理
//============================================================
void CString2D::SetAlignX(const CString2D::EAlignX alignX)
{
	// 引数の横配置を設定
	m_alignX = alignX;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	文字列の横幅取得処理
//============================================================
float CString2D::GetStrWidth(void) const
{
	// 文字列がない場合抜ける
	if ((int)m_wsStr.size() <= 0) { assert(false); return 0.0f; }

	float fStrWidth = 0.0f;	// 文字列の横幅
	int nEndCharID = (int)m_wsStr.size() - 1;	// 終端文字のインデックス

	for (int i = 0; i < nEndCharID; i++)
	{ // 終端文字を抜いた文字数分繰り返す

		// 次の文字までの距離を加算
		assert(m_ppChar[i] != nullptr);
		fStrWidth += m_ppChar[i]->GetNext();
	}

	// 先頭文字のサイズをブラックボックス左からの大きさに補正
	assert(m_ppChar[0] != nullptr);
	float fHeadWidth = m_ppChar[0]->GetSize().x * 0.5f;				// 先頭文字の横幅
	fStrWidth += fHeadWidth - m_ppChar[0]->GetOffsetOrigin();		// 原点より前の空間を加算
	fStrWidth -= fHeadWidth + m_ppChar[0]->GetOffsetBlackBoxLU().x;	// ブラックボックス開始より前の空間を減算

	// 終端文字のサイズをブラックボックス右までの大きさに補正
	assert(m_ppChar[nEndCharID] != nullptr);
	float fTailWidth = m_ppChar[nEndCharID]->GetSize().x * 0.5f;				// 終端文字の横幅
	fStrWidth -= fTailWidth - m_ppChar[nEndCharID]->GetOffsetOrigin();			// 原点より前の空間を減算
	fStrWidth += fTailWidth + m_ppChar[nEndCharID]->GetOffsetBlackBoxRD().x;	// ブラックボックス終了より前の空間を加算

	// 文字列の横幅を返す
	return fStrWidth;
}

//============================================================
//	文字の取得処理
//============================================================
CChar2D *CString2D::GetChar2D(const int nCharID) const
{
	// 文字列がない場合抜ける
	int nStrLen = (int)m_wsStr.size();
	if (nStrLen <= 0) { assert(false); return nullptr; }

	// インデックスが範囲外の場合抜ける
	if (nCharID <= -1 || nCharID >= nStrLen) { assert(false); return nullptr; }

	// 引数インデックスの文字を返す
	return m_ppChar[nCharID];
}

//============================================================
//	相対位置の設定処理
//============================================================
void CString2D::SetPositionRelative(void)
{
	// 文字列がない場合抜ける
	if ((int)m_wsStr.size() <= 0) { return; }

	assert(m_ppChar[0] != nullptr);
	float fHeadOffsetOrigin	= m_ppChar[0]->GetOffsetOrigin();		// 先頭文字の原点オフセット
	float fHeadOffsetLU	= m_ppChar[0]->GetOffsetBlackBoxLU().x;		// 先頭文字のブラックボックスオフセット
	float fHeadRot		= m_ppChar[0]->GetRotation().z - HALF_PI;	// 先頭文字の向き
	float fHeadWidth	= m_ppChar[0]->GetSize().x * 0.5f;			// 先頭文字の横幅
	float fStrWidth		= GetStrWidth() * 0.5f * 2.0f;				// 文字列全体の横幅

	float fStartOffset = fStrWidth;	// 文字の開始位置オフセット
	fStartOffset -= fHeadWidth - fHeadOffsetOrigin;	// 先頭文字の原点分ずらす
	fStartOffset += fHeadWidth + fHeadOffsetLU;		// 先頭文字のブラックボックス分ずらす
	fStartOffset += (fStrWidth * (m_alignX - 1));	// 指定された横配置にする

	D3DXVECTOR3 posStart = GetPosition();	// 文字の開始位置
	posStart.x += sinf(fHeadRot) * fStartOffset;	// 開始位置X
	posStart.y += cosf(fHeadRot) * fStartOffset;	// 開始位置Y

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // 文字数分繰り返す

		assert(m_ppChar[i] != nullptr);
		float fOffset = m_ppChar[i]->GetOffsetOrigin() * 2.0f;	// 原点オフセット
		float fNext = m_ppChar[i]->GetNext() * 2.0f;			// 次文字までの距離

		// 設定座標に原点オフセットを与える
		posStart.x -= sinf(fHeadRot) * fOffset;
		posStart.y -= cosf(fHeadRot) * fOffset;

		// 位置を反映
		m_ppChar[i]->SetPosition(posStart);

		// 次の設定座標の開始点を保存
		float fNextStart = fOffset - fNext;	// 次の開始点
		posStart.x += sinf(fHeadRot) * fNextStart;
		posStart.y += cosf(fHeadRot) * fNextStart;
	}
}

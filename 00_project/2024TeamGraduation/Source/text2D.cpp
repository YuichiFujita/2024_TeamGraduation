//============================================================
//
//	テキスト2D処理 [text2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "text2D.h"
#include "manager.h"
#include "font.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// テキスト2Dの優先順位
}

//************************************************************
//	子クラス [CText2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CText2D::CText2D() : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pFontChar		(nullptr),					// フォント文字
	m_col			(MyLib::color::White()),	// 色
	m_alignX		(CString2D::XALIGN_CENTER),	// 横配置
	m_alignY		(YALIGN_CENTER),			// 縦配置
	m_fCharHeight	(0.0f),						// 文字の縦幅
	m_fLineHeight	(0.0f)						// 文字の行間
{
	// 文字列リストをクリア
	m_listString.clear();
}

//============================================================
//	デストラクタ
//============================================================
CText2D::~CText2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CText2D::Init(void)
{
	// メンバ変数を初期化
	m_pFontChar		= nullptr;					// フォント文字
	m_col			= MyLib::color::White();	// 色
	m_alignX		= CString2D::XALIGN_CENTER;	// 横配置
	m_alignY		= YALIGN_CENTER;			// 縦配置
	m_fCharHeight	= 0.0f;						// 文字の縦幅
	m_fLineHeight	= 0.0f;						// 文字の行間

	// 文字列リストを初期化
	m_listString.clear();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CText2D::Uninit(void)
{
	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列の終了
		SAFE_UNINIT(rList);
	}

	// 文字列リストをクリア
	m_listString.clear();

	// オブジェクトの破棄
	CObject::Release();
}

//============================================================
//	破棄処理
//============================================================
void CText2D::Release(void)
{
	// 自身の終了
	CText2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CText2D::Update(const float fDeltaTime)
{
	// 相対位置の設定
	SetPositionRelative();

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列の更新
		assert(rList != nullptr);
		rList->Update(fDeltaTime);
	}
}

//============================================================
//	描画処理
//============================================================
void CText2D::Draw(void)
{
	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列の描画
		assert(rList != nullptr);
		rList->Draw();
	}
}

//============================================================
//	種類の設定処理
//============================================================
void CText2D::SetType(const TYPE type)
{
	// 自身の種類を設定
	CObject::SetType(type);

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列の種類を設定
		rList->SetType(type);
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CText2D::SetEnableDisp(const bool bDisp)
{
	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列の描画状況を設定
		rList->SetEnableDisp(bDisp);
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CText2D::SetPosition(const MyLib::Vector3& pos)
{
	// 設定する位置を保存
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	向きの設定処理
//============================================================
void CText2D::SetRotation(const MyLib::Vector3& rot)
{
	// 設定する向きを保存
	CObject::SetRotation(rot);

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列向きの設定
		assert(rList != nullptr);
		rList->SetRotation(rot);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	生成処理
//============================================================
CText2D *CText2D::Create
(
	const std::string &rFilePass,		// フォントパス
	const bool bItalic,					// イタリック
	const D3DXVECTOR3& rPos,			// 原点位置
	const float fCharHeight,			// 文字縦幅
	const float fLineHeight,			// 行間縦幅
	const CString2D::EAlignX alignX,	// 横配置
	const EAlignY alignY,				// 縦配置
	const D3DXVECTOR3& rRot,			// 原点向き
	const D3DXCOLOR& rCol				// 色
)
{
	// テキスト2Dの生成
	CText2D *pText2D = new CText2D;
	if (pText2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// テキスト2Dの初期化
		if (FAILED(pText2D->Init()))
		{ // 初期化に失敗した場合

			// テキスト2Dの破棄
			SAFE_DELETE(pText2D);
			return nullptr;
		}

		// フォントを設定
		pText2D->SetFont(rFilePass, bItalic);

		// 原点位置を設定
		pText2D->SetPosition(rPos);

		// 原点向きを設定
		pText2D->SetRotation(rRot);

		// 色を設定
		pText2D->SetColor(rCol);

		// 文字縦幅を設定
		pText2D->SetCharHeight(fCharHeight);

		// 行間縦幅を設定
		pText2D->SetLineHeight(fLineHeight);

		// 横配置を設定
		pText2D->SetAlignX(alignX);

		// 縦配置を設定
		pText2D->SetAlignY(alignY);

		// 確保したアドレスを返す
		return pText2D;
	}
}

//============================================================
//	文字列の追加処理
//============================================================
HRESULT CText2D::AddString(const std::wstring& rStr)
{
	// 文字列オブジェクトを生成
	CString2D *pStr = CString2D::Create
	( // 引数
		m_pFontChar->GetFilePass(),	// フォントパス
		m_pFontChar->GetItalic(),	// イタリック
		rStr,			// 指定文字列
		GetPosition(),	// 原点位置
		m_fCharHeight,	// 文字縦幅
		m_alignX,		// 横配置
		GetRotation(),	// 原点向き
		m_col			// 色
	);
	if (pStr == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 文字列のラベルを指定なしにする
	pStr->SetType(CObject::TYPE::TYPE_NONE);

	// 最後尾に生成した文字列を追加
	m_listString.push_back(pStr);

	// 相対位置の設定
	SetPositionRelative();

	// 成功を返す
	return S_OK;
}

//============================================================
//	文字列の削除処理
//============================================================
void CText2D::DeleteString(const int nStrID)
{
	// 文字列がない場合抜ける
	int nStrSize = (int)m_listString.size();
	if (nStrSize <= 0) { assert(false); return; }

	// インデックスが範囲外の場合抜ける
	if (nStrID <= -1 || nStrID >= nStrSize) { assert(false); return; }

	// イテレーターをインデックス分ずらす
	auto itr = m_listString.begin();
	std::advance(itr, nStrID);

	// イテレーター内の文字列2Dを終了
	SAFE_UNINIT(*itr);

	// イテレーターをリストから削除
	m_listString.erase(itr);
}

//============================================================
//	文字列の全削除処理
//============================================================
void CText2D::DeleteStringAll(void)
{
	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列の終了
		assert(rList != nullptr);
		SAFE_UNINIT(rList);
	}

	// 文字列リストをクリア
	m_listString.clear();
}

//============================================================
//	フォントの設定処理
//============================================================
void CText2D::SetFont
(
	const std::string &rFilePass,	// フォントパス
	const bool bItalic				// イタリック
)
{
	// フォント文字情報を設定
	CFont *pFont = GET_MANAGER->GetFont();	// フォント情報
	m_pFontChar = pFont->Regist(rFilePass, bItalic).pFontChar;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列フォントの設定
		assert(rList != nullptr);
		rList->SetFont(rFilePass, bItalic);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	色の設定処理
//============================================================
void CText2D::SetColor(const D3DXCOLOR& rCol)
{
	// 設定する色を保存
	m_col = rCol;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列色の設定
		assert(rList != nullptr);
		rList->SetColor(rCol);
	}
}

//============================================================
//	透明度の設定処理
//============================================================
void CText2D::SetAlpha(const float fAlpha)
{
	// 引数の透明度を代入
	m_col.a = fAlpha;

	// 色の設定
	SetColor(m_col);
}

//============================================================
//	文字縦幅の設定処理
//============================================================
void CText2D::SetCharHeight(const float fHeight)
{
	// 設定する文字の縦幅を保存
	m_fCharHeight = fHeight;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列縦幅の設定
		assert(rList != nullptr);
		rList->SetCharHeight(fHeight);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	行間縦幅の設定処理
//============================================================
void CText2D::SetLineHeight(const float fHeight)
{
	// 引数の行間を設定
	m_fLineHeight = fHeight;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	横配置の設定処理
//============================================================
void CText2D::SetAlignX(const CString2D::EAlignX align)
{
	// 引数の横配置を設定
	m_alignX = align;

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 文字列横幅の設定
		assert(rList != nullptr);
		rList->SetAlignX(align);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	縦配置の設定処理
//============================================================
void CText2D::SetAlignY(const EAlignY align)
{
	// 引数の縦配置を設定
	m_alignY = align;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	文字列の取得処理
//============================================================
CString2D *CText2D::GetString2D(const int nStrID) const
{
	// 文字列がない場合抜ける
	int nStrSize = (int)m_listString.size();
	if (nStrSize <= 0) { assert(false); return nullptr; }

	// インデックスが範囲外の場合抜ける
	if (nStrID <= -1 || nStrID >= nStrSize) { assert(false); return nullptr; }

	// 引数インデックスの文字列を返す
	auto itr = m_listString.begin();	// 先頭イテレーター
	std::advance(itr, nStrID);			// イテレーターを進める
	return *itr;						// イテレーターの中身を返す
}

//============================================================
//	テキストの縦幅取得処理
//============================================================
float CText2D::GetTextHeight(void) const
{
	// 文字列がない場合抜ける
	if ((int)m_listString.size() <= 0) { assert(false); return 0.0f; }

	float fTextHeight = 0.0f;	// テキストの縦幅
	int nEndStrID = (int)m_listString.size() - 1;	// 終端文字列のインデックス
	for (int i = 0; i < nEndStrID; i++)
	{ // 終端文字列を抜いた文字列数分繰り返す

		// 次の文字列までの行間を加算
		fTextHeight += m_fLineHeight;
	}

	// 先頭と終端の文字列の無視されたサイズを加算
	fTextHeight += m_listString.front()->GetCharHeight() * 0.5f;	// 先頭文字列の原点上サイズ
	fTextHeight += m_listString.back()->GetCharHeight() * 0.5f;		// 終端文字列の原点下サイズ

	// テキストの縦幅を返す
	return fTextHeight;
}

//============================================================
//	相対位置の設定処理
//============================================================
void CText2D::SetPositionRelative(void)
{
	// 文字列がない場合抜ける
	if ((int)m_listString.size() <= 0) { return; }

	float fTextHeight	= GetTextHeight() * 0.5f;	// テキスト全体の縦幅
	float fFrontHeight	= m_listString.front()->GetCharHeight() * 0.5f;	// 先頭文字列の縦幅
	float fStartOffset	= -fTextHeight + fFrontHeight - (fTextHeight * (m_alignY - 1));	// 文字列の開始位置オフセット

	D3DXVECTOR3 posThis = GetPosition();	// 自身の位置
	D3DXVECTOR3 rotThis = GetRotation();	// 自身の向き
	D3DXVECTOR3 posStart = VEC3_ZERO;		// 文字の開始位置
	posStart.x = posThis.x + sinf(rotThis.z) * fStartOffset;	// 開始位置X
	posStart.y = posThis.y + cosf(rotThis.z) * fStartOffset;	// 開始位置Y

	for (auto& rList : m_listString)
	{ // 文字列の格納数分繰り返す

		// 位置を反映
		assert(rList != nullptr);
		rList->SetPosition(posStart);

		// 次の設定座標の開始点を保存
		posStart.x += sinf(rotThis.z) * m_fLineHeight;
		posStart.y += cosf(rotThis.z) * m_fLineHeight;
	}
}

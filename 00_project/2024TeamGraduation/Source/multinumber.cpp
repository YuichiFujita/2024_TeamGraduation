//=============================================================================
// 
//  数字処理 [multinumber.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "multinumber.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "objectBillboard.h"
#include "texture.h"
#include "calculation.h"
#include "camera.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const std::string DEFAULT_TEXTURE = "data\\TEXTURE\\number_blackclover_01.png";	// テクスチャのファイル
	const float TEX_U = 0.1f;	// 横分割
}

//==========================================================================
// コンストラクタ
//==========================================================================
CMultiNumber::CMultiNumber(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_nNum = 0;				// 数字
	m_AlignmentType = EAlignmentType::ALIGNMENT_CENTER;
}

//==========================================================================
// デストラクタ
//==========================================================================
CMultiNumber::~CMultiNumber()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CMultiNumber* CMultiNumber::Create(const int nNum, const int texIdx, const MyLib::Vector3& pos, const MyLib::Vector2& size, int nMaxDigit, EObjType objtype, bool bDigitDraw, int nPriority)
{
	// メモリの確保
	CMultiNumber* pNumber = DEBUG_NEW CMultiNumber(nPriority);

	if (pNumber != nullptr)
	{// メモリの確保が出来ていたら

		// 引数情報
		pNumber->m_nNum = nNum;				// 数字
		pNumber->m_nIdxTexture = texIdx;	// テクスチャのインデックス
		pNumber->m_objType = objtype;		// オブジェクトの種類
		pNumber->m_size = size;				// サイズ
		pNumber->m_nMaxDigit = nMaxDigit;	// 桁数
		pNumber->m_bDigitDraw = bDigitDraw;	// 桁数描画

		// 初期化処理
		pNumber->Init();

		pNumber->SetPosition(pos);			// 位置
	}

	return pNumber;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMultiNumber::Init()
{
	// メモリクリア
	m_vecNumber.clear();

	// 情報取得
	MyLib::Vector3 pos = GetPosition();	// 位置
	MyLib::Vector3 rot = GetRotation();	// 向き
	int nPriority = GetPriority();		// 優先順位

	// 数字生成
	CNumber* pNumber = nullptr;
	for (int i = 0; i < m_nMaxDigit; i++)
	{
		// 生成処理
		pNumber = CNumber::Create(m_objType, nPriority);

		// 生成失敗
		if (pNumber == nullptr) return S_OK;

		// 情報設定
		pNumber->SetPosition(pos);
		pNumber->SetOriginPosition(pos);
		pNumber->SetRotation(rot);
		pNumber->SetOriginRotation(pos);
		pNumber->SetSize(m_size);
		pNumber->SetSizeOrigin(m_size);

		// テクスチャ割り当て
		pNumber->BindTexture(m_nIdxTexture);

		// コンテナへ追加
		m_vecNumber.push_back(pNumber);
	}

	// 値設定
	SetNum(m_nNum);

	// 桁数
	m_nDigit = UtilFunc::Calculation::GetDigit(m_nNum);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMultiNumber::Uninit()
{
	// 終了
	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr) continue;
		number->Uninit();
	}
	m_vecNumber.clear();

	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CMultiNumber::Kill()
{
	// 削除
	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr) continue;
		number->Kill();
	}
	m_vecNumber.clear();

	// 終了処理
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMultiNumber::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 桁数
	m_nDigit = UtilFunc::Calculation::GetDigit(m_nNum);

	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr ||
			number->GetType() != CObject::TYPE::TYPE_NONE)
		{
			continue;
		}

		// 更新
		number->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CMultiNumber::Draw()
{
	int i = 1;
	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr ||
			number->GetType() != CObject::TYPE::TYPE_NONE)
		{
			continue;
		}

		if (m_bDigitDraw && m_nMaxDigit - m_nDigit >= i)
		{// 桁数描画 && 表示桁数より大きい
			i++;
			continue;
		}

		// 描画処理
		number->Draw();

		i++;
	}
}

//==========================================================================
// 値設定
//==========================================================================
void CMultiNumber::SetNum(int nNum)
{
	// 値設定
	m_nNum = nNum;

	for (int i = 0; i < m_nMaxDigit; i++)
	{
		if (m_vecNumber[i] == nullptr) continue;

		// 今回の桁のパターン割り出し
		int nTexU = m_nNum % (int)std::pow(10, m_nMaxDigit + 1 - i) / ((int)std::pow(10, m_nMaxDigit - i) / 10);
		UtilFunc::Calculation::GetDigit(m_nNum);
		m_vecNumber[i]->SetNum(nTexU);
	}
}

//==========================================================================
// 位置揃え
//==========================================================================
void CMultiNumber::AdjustPosition()
{
	switch (m_AlignmentType)
	{
	case CMultiNumber::ALIGNMENT_LEFT:
		AdjustLeft();
		break;

	case CMultiNumber::ALIGNMENT_RIGHT:
		AdjustRight();
		break;

	case CMultiNumber::ALIGNMENT_CENTER:
		AdjustCenter();
		break;

	default:
		MyAssert::CustomAssert(false, "位置揃えできないよ");
		break;
	}
}

//==========================================================================
// 左揃え
//==========================================================================
void CMultiNumber::AdjustLeft()
{
	// 桁数分左へずらす
	MyLib::Vector3 setpos = GetPosition();
	setpos.x -= m_size.x * 2.0f * m_nMaxDigit;

	for (const auto& number : m_vecNumber)
	{
		// 位置設定
		number->SetPosition(setpos);

		// 移動していく
		setpos.x += m_size.x * 2.0f;
	}
}

//==========================================================================
// 右揃え
//==========================================================================
void CMultiNumber::AdjustRight()
{
	// 桁数分右へずらす
	MyLib::Vector3 setpos = GetPosition();
	setpos.x += m_size.x * 2.0f * m_nMaxDigit;

	for (const auto& number : m_vecNumber)
	{
		// 位置設定
		number->SetPosition(setpos);

		// 移動していく
		setpos.x += m_size.x * 2.0f;
	}
}

//==========================================================================
// 中央揃え
//==========================================================================
void CMultiNumber::AdjustCenter()
{
	// 半分の位置へ移動
	MyLib::Vector3 setpos = GetPosition();
	setpos.x -= (m_size.x * 2.0f * m_nMaxDigit) * 0.5f;

	for (const auto& number : m_vecNumber)
	{
		// 位置設定
		number->SetPosition(setpos);

		// 移動していく
		setpos.x += m_size.x * 2.0f;
	}
}

//==========================================================================
// 位置設定
//==========================================================================
void CMultiNumber::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);

	for (const auto& number : m_vecNumber)
	{
		number->SetPosition(pos);
	}

	// 位置揃え
	AdjustPosition();
}

//==========================================================================
// 過去の位置設定
//==========================================================================
void CMultiNumber::SetOldPosition(const MyLib::Vector3& pos)
{
	CObject::SetOldPosition(pos);

	for (const auto& number : m_vecNumber)
	{
		number->SetOldPosition(pos);
	}
}

//==========================================================================
// 元の位置設定
//==========================================================================
void CMultiNumber::SetOriginPosition(const MyLib::Vector3& pos)
{
	CObject::SetOriginPosition(pos);

	for (const auto& number : m_vecNumber)
	{
		number->SetOriginPosition(pos);
	}
}

//==========================================================================
// 向き設定
//==========================================================================
void CMultiNumber::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);

	for (const auto& number : m_vecNumber)
	{
		number->SetRotation(rot);
	}
}

//==========================================================================
// 前回の向き設定
//==========================================================================
void CMultiNumber::SetOldRotation(const MyLib::Vector3& rot)
{
	CObject::SetOldRotation(rot);

	for (const auto& number : m_vecNumber)
	{
		number->SetOldRotation(rot);
	}
}

//==========================================================================
// 元の向き設定
//==========================================================================
void CMultiNumber::SetOriginRotation(const MyLib::Vector3& rot)
{
	CObject::SetOriginRotation(rot);

	for (const auto& number : m_vecNumber)
	{
		number->SetOriginRotation(rot);
	}
}

//==========================================================================
// サイズ設定
//==========================================================================
void CMultiNumber::SetSize(const MyLib::Vector2& size)
{
	m_size = size;

	for (const auto& number : m_vecNumber)
	{
		number->SetSize(size);
	}
}

//==========================================================================
// 元のサイズ設定
//==========================================================================
void CMultiNumber::SetSizeOrigin(const MyLib::Vector2& size)
{
	m_sizeOrigin = size;

	for (const auto& number : m_vecNumber)
	{
		number->SetSizeOrigin(size);
	}
}

//==========================================================================
// 色設定
//==========================================================================
void CMultiNumber::SetColor(const D3DXCOLOR& col)
{
	m_col = col;

	for (const auto& number : m_vecNumber)
	{
		number->SetColor(col);
	}
}

//==========================================================================
// 元の色設定
//==========================================================================
void CMultiNumber::SetOriginColor(const D3DXCOLOR& col)
{
	m_colOrigin = col;

	for (const auto& number : m_vecNumber)
	{
		number->SetOriginColor(col);
	}
}

//==========================================================================
// 不透明度設定
//==========================================================================
void CMultiNumber::SetAlpha(const float alpha)
{
	m_col.a = alpha;

	for (const auto& number : m_vecNumber)
	{
		number->SetAlpha(alpha);
	}
}

//==========================================================================
// テクスチャ割り当て
//==========================================================================
void CMultiNumber::BindTexture(int nIdxTexture)
{
	// テクスチャ割り当て
	m_nIdxTexture = nIdxTexture;

	for (const auto& number : m_vecNumber)
	{
		// 位置設定
		number->BindTexture(nIdxTexture);
	}
}

//==========================================================================
// 寄せ種類設定
//==========================================================================
void CMultiNumber::SetAlignmentType(EAlignmentType type)
{
	m_AlignmentType = type;
	SetPosition(GetPosition());
}

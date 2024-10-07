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
	m_nNumNumber = 0;		// 数字の数
	m_nTexIdx = 0;			// テクスチャのインデックス番号
	m_nPriority = 0;		// 優先順位
	m_ppMultiNumber = nullptr;	// 数字のオブジェクト
	m_col = mylib_const::DEFAULT_COLOR;		// 色
	m_size = D3DXVECTOR2(0.0f, 0.0f);	// 数字のサイズ
	m_objType = CNumber::OBJECTTYPE_2D;	// オブジェクトの種類
	m_bDigitDraw = false;				// 桁数描画
	m_bAutoUVSetting = false;			// 自動UV座標設定
	m_fKerning = 0.0f;				// 文字間隔
	m_Alignment = AlignmentType::ALIGNMENT_LEFT;		// 揃え
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
CMultiNumber* CMultiNumber::Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, bool bDigitDraw, int nPriority)
{
	// メモリの確保
	CMultiNumber* pNumber = DEBUG_NEW CMultiNumber;

	if (pNumber != nullptr)
	{// メモリの確保が出来ていたら

		// オブジェクトの種類
		pNumber->m_objType = objtype;

		// サイズ
		pNumber->m_size = size;

		// 位置
		pNumber->SetPosition(pos);

		// 数字の数
		pNumber->m_nNumNumber = nNum;

		// 桁数描画
		pNumber->m_bDigitDraw = bDigitDraw;

		// 優先順位
		pNumber->m_nPriority = nPriority;

		// 初期化処理
		pNumber->Init();
	}

	return pNumber;
}


//==========================================================================
// 生成処理(オーバーロード)
//==========================================================================
CMultiNumber* CMultiNumber::Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, const std::string& file, bool bDigitDraw, int nPriority)
{
	// メモリの確保
	CMultiNumber* pNumber = DEBUG_NEW CMultiNumber;

	if (pNumber != nullptr)
	{// メモリの確保が出来ていたら

		// オブジェクトの種類
		pNumber->m_objType = objtype;

		// サイズ
		pNumber->m_size = size;
		pNumber->m_sizeOrigin = size;

		// 位置
		pNumber->SetPosition(pos);
		pNumber->SetOriginPosition(pos);

		// 数字の数
		pNumber->m_nNumNumber = nNum;

		// 桁数描画
		pNumber->m_bDigitDraw = bDigitDraw;

		// 優先順位
		pNumber->m_nPriority = nPriority;

		// テクスチャ読み込み
		pNumber->m_nTexIdx = CTexture::GetInstance()->Regist(file);

		// 初期化処理
		pNumber->Init();
	}

	return pNumber;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMultiNumber::Init()
{
	// 数分メモリ確保
	m_ppMultiNumber = DEBUG_NEW(CNumber*[m_nNumNumber]);

	// 生成処理
	if (m_nTexIdx == 0)
	{
		m_nTexIdx = CTexture::GetInstance()->Regist(DEFAULT_TEXTURE);
	}

	MyLib::Vector3 m_pos = GetPosition();
	MyLib::Vector3 m_rot = GetRotation();

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		// 生成処理
		m_ppMultiNumber[nCntNum] = CNumber::Create(m_objType, m_nPriority);

		// 各種変数の初期化
		m_ppMultiNumber[nCntNum]->SetSize(m_size);	// サイズ
		m_ppMultiNumber[nCntNum]->SetSizeOrigin(m_size);	// サイズ
		m_ppMultiNumber[nCntNum]->SetPosition(MyLib::Vector3(m_pos.x + m_size.y * nCntNum, m_pos.y, m_pos.z));	// 位置
		m_ppMultiNumber[nCntNum]->SetRotation(m_rot);
		m_ppMultiNumber[nCntNum]->SetType(CObject::TYPE::TYPE_NUMBER);

		// テクスチャの割り当て
		m_ppMultiNumber[nCntNum]->BindTexture(m_nTexIdx);
	}

	if (m_bDigitDraw)
	{
		// 描画設定
		SettingDisp();
	}

	if (m_objType == CNumber::OBJECTTYPE_BILLBOARD)
	{
		// カメラの向き取得
		MyLib::Vector3 camerarot = CManager::GetInstance()->GetCamera()->GetRotation();

		SetRotation(MyLib::Vector3(0.0f, camerarot.y, 0.0f));
	}

	// 文字間隔
	m_fKerning = m_size.y;

	// 数字設定
	SetValue();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMultiNumber::Uninit()
{
	// 終了処理
	if (m_ppMultiNumber != nullptr)
	{
		// 各数字オブジェクトの破棄
		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != nullptr)
			{
				m_ppMultiNumber[nCntNum]->Uninit();
				m_ppMultiNumber[nCntNum] = nullptr;
			}
		}
		delete[] m_ppMultiNumber;
		m_ppMultiNumber = nullptr;
	}

	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CMultiNumber::Kill()
{
	// 終了処理
	if (m_ppMultiNumber != nullptr)
	{
		// 各数字オブジェクトの破棄
		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != nullptr)
			{
				m_ppMultiNumber[nCntNum]->Kill();
				m_ppMultiNumber[nCntNum] = nullptr;
			}
		}
		delete[] m_ppMultiNumber;
		m_ppMultiNumber = nullptr;
	}

	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMultiNumber::Update()
{
	if (m_bDigitDraw == false)
	{// 桁数描画じゃない場合
		return;
	}

	if (m_objType == CNumber::OBJECTTYPE_BILLBOARD)
	{
		// カメラの向き取得
		MyLib::Vector3 camerarot = CManager::GetInstance()->GetCamera()->GetRotation();

		SetRotation(MyLib::Vector3(0.0f, camerarot.y, 0.0f));
	}

	SettingDisp();
}

//==========================================================================
// 描画設定
//==========================================================================
void CMultiNumber::SettingDisp()
{
	int nNumberDigit = UtilFunc::Calculation::GetDigit(m_nNum);

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == nullptr)
		{
			continue;
		}

		switch (m_objType)
		{
		case CNumber::OBJECTTYPE_2D:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// 桁数
				m_ppMultiNumber[nCntNum]->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->SetEnableDisp(false);
			}
			break;

		case CNumber::OBJECTTYPE_3D:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// 桁数
				m_ppMultiNumber[nCntNum]->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->SetEnableDisp(false);
			}
			break;

		case CNumber::OBJECTTYPE_BILLBOARD:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// 桁数
				m_ppMultiNumber[nCntNum]->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->SetEnableDisp(false);
			}
			break;
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CMultiNumber::Draw()
{
	int nNumNumber = m_nNumNumber;
	if (m_bDigitDraw)
	{// 桁数描画だったら
		nNumNumber = UtilFunc::Calculation::GetDigit(m_nNum);
	}
}

//==========================================================================
// 値の設定処理
//==========================================================================
void CMultiNumber::AddNumber(int nValue)
{
	m_nNum += nValue;

	// 値の設定処理
	SetValue();
}

//==========================================================================
// 値の設定処理
//==========================================================================
void CMultiNumber::SetValue(int nValue)
{
	m_nNum = nValue;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == nullptr)
		{
			continue;
		}

		int aTexU = m_nNum % (int)std::pow(10, m_nNumNumber + 1 - nCntNum) / ((int)std::pow(10, m_nNumNumber - nCntNum) / 10);

		if (aTexU < 0)
		{
			aTexU = 0;
		}
		m_ppMultiNumber[nCntNum]->SetNum(aTexU);
	}

	// 値の設定処理
	SetValue();
}

//==========================================================================
// 値の取得処理
//==========================================================================
int CMultiNumber::GetValue()
{
	return m_nNum;
}

//==========================================================================
// 値の設定処理
//==========================================================================
void CMultiNumber::SetValue()
{
	if (m_bAutoUVSetting) return;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] == nullptr)
		{
			continue;
		}

		int aTexU = m_nNum % (int)std::pow(10, m_nNumNumber + 1 - nCntNum) / ((int)std::pow(10, m_nNumNumber - nCntNum) / 10);

		if (aTexU < 0)
		{
			aTexU = 0;
		}
		m_ppMultiNumber[nCntNum]->SetNum(aTexU);

		// テクスチャポインタ取得
		D3DXVECTOR2 *pTex = m_ppMultiNumber[nCntNum]->GetTex();

		// テクスチャ座標の設定
		pTex[0] = D3DXVECTOR2(aTexU * TEX_U, 0.0f);
		pTex[1] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 0.0f);
		pTex[2] = D3DXVECTOR2(aTexU * TEX_U, 1.0f);
		pTex[3] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 1.0f);

		// 頂点設定
		m_ppMultiNumber[nCntNum]->SetVtx();
	}
}

//==========================================================================
// 位置設定
//==========================================================================
void CMultiNumber::SetPosition(const MyLib::Vector3& pos)
{
	// 位置設定
	CObject::SetPosition(pos);
	MyLib::Vector3 m_pos = pos;
	MyLib::Vector3 m_rot = GetRotation();

	int nNumberDigit = UtilFunc::Calculation::GetDigit(m_nNum);
	if (m_Alignment == AlignmentType::ALIGNMENT_LEFT)
	{
		if (!m_bDigitDraw)
		{
			nNumberDigit = m_nNumNumber;
		}

		MyLib::Vector3 setpos = m_pos;
		if (m_bDigitDraw)
		{
			for (int i = 0; i < m_nNumNumber - nNumberDigit; i++)
			{
				setpos.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * m_fKerning;
				setpos.z -= cosf(D3DX_PI * 0.5f + m_rot.y) * m_fKerning;
			}
		}

		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != nullptr)
			{
				m_ppMultiNumber[nCntNum]->SetPosition(MyLib::Vector3
				(
					setpos.x + sinf(D3DX_PI * 0.5f + m_rot.y) * (m_fKerning * nCntNum),
					setpos.y,
					setpos.z + cosf(D3DX_PI * 0.5f + m_rot.y) * (m_fKerning * nCntNum))
				);	// 位置
			}
		}
	}
	else if (m_Alignment == AlignmentType::ALIGNMENT_RIGHT)
	{
		MyLib::Vector3 setpos = m_pos;
		for (int i = 0; i < m_nNumNumber; i++)
		{
			setpos.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * m_fKerning;
			setpos.z -= cosf(D3DX_PI * 0.5f + m_rot.y) * m_fKerning;
		}

		for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
		{
			if (m_ppMultiNumber[nCntNum] != nullptr)
			{
				setpos.x += sinf(D3DX_PI * 0.5f + m_rot.y) * m_fKerning;
				setpos.z += cosf(D3DX_PI * 0.5f + m_rot.y) * m_fKerning;

				m_ppMultiNumber[nCntNum]->SetPosition(setpos);	// 位置
			}
		}
	}
}

//==========================================================================
// 向き設定
//==========================================================================
void CMultiNumber::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);
	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != nullptr)
		{
			m_ppMultiNumber[nCntNum]->SetRotation(rot);	// 位置
		}
	}
}

//==========================================================================
// 色設定
//==========================================================================
void CMultiNumber::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != nullptr)
		{
			m_ppMultiNumber[nCntNum]->SetColor(m_col);	// 色
		}
	}
}

//==========================================================================
// 色取得
//==========================================================================
D3DXCOLOR CMultiNumber::GetColor() const
{
	return m_col;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CMultiNumber::SetSize(const D3DXVECTOR2 size)
{
	m_size = size;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != nullptr)
		{
			m_ppMultiNumber[nCntNum]->SetSize(m_size);
		}
	}
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CMultiNumber::GetSize() const
{
	return m_size;
}

//==========================================================================
// サイズ設定
//==========================================================================
void CMultiNumber::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_sizeOrigin = size;
}

//==========================================================================
// サイズ取得
//==========================================================================
D3DXVECTOR2 CMultiNumber::GetSizeOrigin() const
{
	return m_sizeOrigin;
}

//==========================================================================
// 文字間隔設定
//==========================================================================
void CMultiNumber::SetKerning(float kerning)
{
	m_fKerning = kerning;
}

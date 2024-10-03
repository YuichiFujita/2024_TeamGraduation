//=============================================================================
// 
//  �������� [multinumber.cpp]
//  Author : ���n�Ή�
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
// �}�N����`
//==========================================================================
namespace
{
	const std::string DEFAULT_TEXTURE = "data\\TEXTURE\\number_blackclover_01.png";	// �e�N�X�`���̃t�@�C��
	const float TEX_U = 0.1f;	// ������
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMultiNumber::CMultiNumber(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_nNum = 0;				// ����
	m_nNumNumber = 0;		// �����̐�
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nPriority = 0;		// �D�揇��
	m_ppMultiNumber = nullptr;	// �����̃I�u�W�F�N�g
	m_col = mylib_const::DEFAULT_COLOR;		// �F
	m_size = D3DXVECTOR2(0.0f, 0.0f);	// �����̃T�C�Y
	m_objType = CNumber::OBJECTTYPE_2D;	// �I�u�W�F�N�g�̎��
	m_bDigitDraw = false;				// �����`��
	m_bAutoUVSetting = false;			// ����UV���W�ݒ�
	m_fKerning = 0.0f;				// �����Ԋu
	m_Alignment = AlignmentType::ALIGNMENT_LEFT;		// ����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMultiNumber::~CMultiNumber()
{

}

//==========================================================================
// ��������
//==========================================================================
CMultiNumber* CMultiNumber::Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, bool bDigitDraw, int nPriority)
{
	// �������̊m��
	CMultiNumber* pNumber = DEBUG_NEW CMultiNumber;

	if (pNumber != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I�u�W�F�N�g�̎��
		pNumber->m_objType = objtype;

		// �T�C�Y
		pNumber->m_size = size;

		// �ʒu
		pNumber->SetPosition(pos);

		// �����̐�
		pNumber->m_nNumNumber = nNum;

		// �����`��
		pNumber->m_bDigitDraw = bDigitDraw;

		// �D�揇��
		pNumber->m_nPriority = nPriority;

		// ����������
		pNumber->Init();
	}

	return pNumber;
}


//==========================================================================
// ��������(�I�[�o�[���[�h)
//==========================================================================
CMultiNumber* CMultiNumber::Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, const std::string& file, bool bDigitDraw, int nPriority)
{
	// �������̊m��
	CMultiNumber* pNumber = DEBUG_NEW CMultiNumber;

	if (pNumber != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �I�u�W�F�N�g�̎��
		pNumber->m_objType = objtype;

		// �T�C�Y
		pNumber->m_size = size;
		pNumber->m_sizeOrigin = size;

		// �ʒu
		pNumber->SetPosition(pos);
		pNumber->SetOriginPosition(pos);

		// �����̐�
		pNumber->m_nNumNumber = nNum;

		// �����`��
		pNumber->m_bDigitDraw = bDigitDraw;

		// �D�揇��
		pNumber->m_nPriority = nPriority;

		// �e�N�X�`���ǂݍ���
		pNumber->m_nTexIdx = CTexture::GetInstance()->Regist(file);

		// ����������
		pNumber->Init();
	}

	return pNumber;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMultiNumber::Init()
{
	// �����������m��
	m_ppMultiNumber = DEBUG_NEW(CNumber*[m_nNumNumber]);

	// ��������
	if (m_nTexIdx == 0)
	{
		m_nTexIdx = CTexture::GetInstance()->Regist(DEFAULT_TEXTURE);
	}

	MyLib::Vector3 m_pos = GetPosition();
	MyLib::Vector3 m_rot = GetRotation();

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		// ��������
		m_ppMultiNumber[nCntNum] = CNumber::Create(m_objType, m_nPriority);

		// �e��ϐ��̏�����
		m_ppMultiNumber[nCntNum]->SetSize(m_size);	// �T�C�Y
		m_ppMultiNumber[nCntNum]->SetSizeOrigin(m_size);	// �T�C�Y
		m_ppMultiNumber[nCntNum]->SetPosition(MyLib::Vector3(m_pos.x + m_size.y * nCntNum, m_pos.y, m_pos.z));	// �ʒu
		m_ppMultiNumber[nCntNum]->SetRotation(m_rot);
		m_ppMultiNumber[nCntNum]->SetType(CObject::TYPE::TYPE_NUMBER);

		// �e�N�X�`���̊��蓖��
		m_ppMultiNumber[nCntNum]->BindTexture(m_nTexIdx);
	}

	if (m_bDigitDraw)
	{
		// �`��ݒ�
		SettingDisp();
	}

	if (m_objType == CNumber::OBJECTTYPE_BILLBOARD)
	{
		// �J�����̌����擾
		MyLib::Vector3 camerarot = CManager::GetInstance()->GetCamera()->GetRotation();

		SetRotation(MyLib::Vector3(0.0f, camerarot.y, 0.0f));
	}

	// �����Ԋu
	m_fKerning = m_size.y;

	// �����ݒ�
	SetValue();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMultiNumber::Uninit()
{
	// �I������
	if (m_ppMultiNumber != nullptr)
	{
		// �e�����I�u�W�F�N�g�̔j��
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
// �폜����
//==========================================================================
void CMultiNumber::Kill()
{
	// �I������
	if (m_ppMultiNumber != nullptr)
	{
		// �e�����I�u�W�F�N�g�̔j��
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
// �X�V����
//==========================================================================
void CMultiNumber::Update()
{
	if (m_bDigitDraw == false)
	{// �����`�悶��Ȃ��ꍇ
		return;
	}

	if (m_objType == CNumber::OBJECTTYPE_BILLBOARD)
	{
		// �J�����̌����擾
		MyLib::Vector3 camerarot = CManager::GetInstance()->GetCamera()->GetRotation();

		SetRotation(MyLib::Vector3(0.0f, camerarot.y, 0.0f));
	}

	SettingDisp();
}

//==========================================================================
// �`��ݒ�
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
			{// ����
				m_ppMultiNumber[nCntNum]->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->SetEnableDisp(false);
			}
			break;

		case CNumber::OBJECTTYPE_3D:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// ����
				m_ppMultiNumber[nCntNum]->SetEnableDisp(true);
			}
			else
			{
				m_ppMultiNumber[nCntNum]->SetEnableDisp(false);
			}
			break;

		case CNumber::OBJECTTYPE_BILLBOARD:
			if (m_nNumNumber - nNumberDigit <= nCntNum)
			{// ����
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
// �`�揈��
//==========================================================================
void CMultiNumber::Draw()
{
	int nNumNumber = m_nNumNumber;
	if (m_bDigitDraw)
	{// �����`�悾������
		nNumNumber = UtilFunc::Calculation::GetDigit(m_nNum);
	}
}

//==========================================================================
// �l�̐ݒ菈��
//==========================================================================
void CMultiNumber::AddNumber(int nValue)
{
	m_nNum += nValue;

	// �l�̐ݒ菈��
	SetValue();
}

//==========================================================================
// �l�̐ݒ菈��
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

	// �l�̐ݒ菈��
	SetValue();
}

//==========================================================================
// �l�̎擾����
//==========================================================================
int CMultiNumber::GetValue()
{
	return m_nNum;
}

//==========================================================================
// �l�̐ݒ菈��
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

		// �e�N�X�`���|�C���^�擾
		D3DXVECTOR2 *pTex = m_ppMultiNumber[nCntNum]->GetTex();

		// �e�N�X�`�����W�̐ݒ�
		pTex[0] = D3DXVECTOR2(aTexU * TEX_U, 0.0f);
		pTex[1] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 0.0f);
		pTex[2] = D3DXVECTOR2(aTexU * TEX_U, 1.0f);
		pTex[3] = D3DXVECTOR2(aTexU * TEX_U + TEX_U, 1.0f);

		// ���_�ݒ�
		m_ppMultiNumber[nCntNum]->SetVtx();
	}
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CMultiNumber::SetPosition(const MyLib::Vector3& pos)
{
	// �ʒu�ݒ�
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
				);	// �ʒu
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

				m_ppMultiNumber[nCntNum]->SetPosition(setpos);	// �ʒu
			}
		}
	}
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CMultiNumber::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);
	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != nullptr)
		{
			m_ppMultiNumber[nCntNum]->SetRotation(rot);	// �ʒu
		}
	}
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CMultiNumber::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	for (int nCntNum = 0; nCntNum < m_nNumNumber; nCntNum++)
	{
		if (m_ppMultiNumber[nCntNum] != nullptr)
		{
			m_ppMultiNumber[nCntNum]->SetColor(m_col);	// �F
		}
	}
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CMultiNumber::GetColor() const
{
	return m_col;
}

//==========================================================================
// �T�C�Y�ݒ�
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
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CMultiNumber::GetSize() const
{
	return m_size;
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CMultiNumber::SetSizeOrigin(const D3DXVECTOR2 size)
{
	m_sizeOrigin = size;
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CMultiNumber::GetSizeOrigin() const
{
	return m_sizeOrigin;
}

//==========================================================================
// �����Ԋu�ݒ�
//==========================================================================
void CMultiNumber::SetKerning(float kerning)
{
	m_fKerning = kerning;
}

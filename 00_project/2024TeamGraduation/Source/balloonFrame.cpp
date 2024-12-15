//==========================================================================
// 
//  �����o���̘g���� [balloonFrame.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "balloonFrame.h"
#include "object2D.h"
#include "2D_effect.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE[CGameManager::ETeamSide::SIDE_MAX] =	// �e�N�X�`��
	{
		"data\\TEXTURE\\speech\\balloon_left.png",
		"data\\TEXTURE\\speech\\balloon_right.png",
	};
	const int DIVISION_WIDTH = 4;					// ������
	const int BLOCK_WIDTH = DIVISION_WIDTH + 1;		// ���̃u���b�N
	const float FRAME_WIDTH = 8.0f;				// �g�̕�

	// �C���f�b�N�X�o�b�t�@
	const WORD IDXBUFF[] =
	{
	0, 1, 4,   // �㕔�g
	1, 5, 4,
	2, 6, 3,   // �����g
	3, 6, 7,
	0, 4, 2,   // �����g
	2, 4, 6,
	1, 3, 5,   // �E���g
	3, 7, 5
	};

	const int NUM_INDICES = 24; // �C���f�b�N�X�̌�
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBalloonFrame::CBalloonFrame(int nPriority) : CObject2D(nPriority)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBalloonFrame::~CBalloonFrame()
{

}

//==========================================================================
// ��������
//==========================================================================
CBalloonFrame* CBalloonFrame::Create(const MyLib::Vector2& size, CGameManager::ETeamSide side)
{
	// �������̊m��
	CBalloonFrame* pObj = DEBUG_NEW CBalloonFrame;

	if (pObj != nullptr)
	{
		// �������ݒ�
		pObj->m_TextSize = size;
		pObj->m_TeamSide = side;		// �`�[���T�C�h

		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBalloonFrame::Init()
{
	// �����ɐݒ�
	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));

	// ���߂�|���S������
	CreateFillPoly();

	// ������
	CreateTail();

	// ����������
	CObject2D::Init(8);

	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �F�ݒ�
	SetColor(MyLib::color::Black());

	// �C���f�b�N�X����
	CreateIndex();

	// �T�C�Y�ɉ����Ęg
	SetVtx();

	return S_OK;
}

//==========================================================================
// ���߂�|���S������
//==========================================================================
void CBalloonFrame::CreateFillPoly()
{
	// ��������
	m_pFillPoly = CObject2D::Create(UtilFunc::Transformation::Clamp(GetPriority() - 1, 0, mylib_const::PRIORITY_NUM));
	m_pFillPoly->SetType(CObject::TYPE::TYPE_NONE);

	// �F�ݒ�
	m_pFillPoly->SetColor(MyLib::color::White());

	// �ʒu�ݒ�
	m_pFillPoly->SetPosition(GetPosition());

	// �T�C�Y�ݒ�
	m_pFillPoly->SetSize(m_TextSize * 0.5f);
}

//==========================================================================
// ������
//==========================================================================
void CBalloonFrame::CreateTail()
{
	// ��������
	m_pTail = CObject2D::Create(UtilFunc::Transformation::Clamp(GetPriority() + 1, 0, mylib_const::PRIORITY_NUM));
	m_pTail->SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_TeamSide]);
	m_pTail->BindTexture(texID);

	// �F�ݒ�
	m_pTail->SetColor(MyLib::color::White());

	// �ʒu�ݒ�
	m_pTail->SetPosition(GetPosition() + MyLib::Vector3(0.0f, m_pFillPoly->GetSize().y - FRAME_WIDTH * 0.5f, 0.0f));

	// �T�C�Y�ݒ�
	m_pTail->SetSize(MyLib::Vector2(150.0f));
}

//==========================================================================
// �C���f�b�N�X����
//==========================================================================
HRESULT CBalloonFrame::CreateIndex()
{
	HRESULT hr;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumIndex = NUM_INDICES;	// �C���f�b�N�X��

	// �C���f�b�N�X�o�b�t�@�̐���
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * NUM_INDICES,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr
	);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�ɃR�s�[
	void* pIndices = nullptr;
	m_pIdxBuff->Lock(0, 0, (void**)&pIndices, 0);
	memcpy(pIndices, IDXBUFF, sizeof(IDXBUFF));
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBalloonFrame::Uninit()
{
	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CBalloonFrame::Kill()
{
	// �폜
	SAFE_KILL(m_pFillPoly);
	SAFE_KILL(m_pTail);

	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBalloonFrame::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���_���W
	SetVtx();

	// ���̃I�u�W�F�N�g�̍X�V
	m_pFillPoly->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pTail->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBalloonFrame::Draw()
{
	// ���߂�|���S���̕`��
	m_pFillPoly->Draw();

	// �C���f�b�N�X�o�b�t�@�`��
	DrawIdxBuff();

	// ���̕`��
	m_pTail->Draw();
}

//==========================================================================
// �C���f�b�N�X�o�b�t�@�`��
//==========================================================================
void CBalloonFrame::DrawIdxBuff()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, GetVtxBuff(), 0, sizeof(VERTEX_2D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAdress(GetIdxTexture()));

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,	// �v���~�e�B�u�^�C�v
		0,					// BaseVertexIndex
		0,					// MinIndex
		8,					// NumVertices
		0,					// StartIndex
		8					// PrimitiveCount
	);
}

//==========================================================================
// ���_���W
//==========================================================================
void CBalloonFrame::SetVtx()
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();		// �ʒu
	D3DXCOLOR col = GetColor();				// �F

	// �e�L�X�g�̕��̑Ίp�������
	float distance = sqrtf(m_TextSize.x * m_TextSize.x + m_TextSize.y * m_TextSize.y);
	float angle = atan2f(m_TextSize.x, m_TextSize.y);	// �Ίp���̌���

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < 8; i++)
	{// ���̒��_�����J��Ԃ�

		pVtx[i].rhw = 1.0f;
		pVtx[i].col = col;
	}

	//-----------------------------
	// �l�p�`
	//-----------------------------
	for (int nCntHeight = 0; nCntHeight < 2; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		// �O���Ɠ����̒���
		float fOuterScale = (nCntHeight == 0) ? 1.0f : 1.0f - (FRAME_WIDTH / m_TextSize.x);
		float fInnerScale = (nCntHeight == 0) ? 1.0f : 1.0f - (FRAME_WIDTH / m_TextSize.y);

		// ���_���v�Z
		pVtx[0].pos = pos + MyLib::Vector3   // ����
		(
			-m_TextSize.x * 0.5f * fOuterScale,
			-m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);
		pVtx[1].pos = pos + MyLib::Vector3   // �E��
		(
			m_TextSize.x * 0.5f * fOuterScale,
			-m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);
		pVtx[2].pos = pos + MyLib::Vector3   // ����
		(
			-m_TextSize.x * 0.5f * fOuterScale,
			m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);
		pVtx[3].pos = pos + MyLib::Vector3   // �E��
		(
			m_TextSize.x * 0.5f * fOuterScale,
			m_TextSize.y * 0.5f * fInnerScale,
			0.0f
		);

		// ���̒��_�o�b�t�@�ʒu�ɐi��
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N���b�N
	GetVtxBuff()->Unlock();
}

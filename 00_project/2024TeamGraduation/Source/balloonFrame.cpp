//==========================================================================
// 
//  �����o���̘g���� [balloonFrame.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "balloonFrame.h"
#include "object2D.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\title\\line.png";	// �e�N�X�`��
	const int DIVISION_WIDTH = 4;					// ������
	const int BLOCK_WIDTH = DIVISION_WIDTH + 1;		// ���̃u���b�N
	const float FRAME_WIDTH = 10.0f;				// �g�̕�

	const MyLib::Vector2 UVLOOP = MyLib::Vector2(40.0f, 0.0f);	// UV���[�v
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
CBalloonFrame* CBalloonFrame::Create(const MyLib::Vector2& size)
{
	// �������̊m��
	CBalloonFrame* pObj = DEBUG_NEW CBalloonFrame;

	if (pObj != nullptr)
	{
		// �������ݒ�
		pObj->m_TextSize = size;

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
	// ����������
	CObject2D::Init();

	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// �T�C�Y�ɉ����Ęg
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBalloonFrame::Uninit()
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CBalloonFrame::Kill()
{
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
}

//==========================================================================
// ���_���W
//==========================================================================
void CBalloonFrame::SetVtx()
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();		// �ʒu
	D3DXCOLOR col = GetColor();				// �F
	D3DXVECTOR2* pVtxTex = GetTex();		// �e�N�X�`�����W�擾

	// ���_���̐ݒ�
	int i = 0;
	float u = 0.0f;
	float fRotWidth = (D3DX_PI * 2) / (float)(DIVISION_WIDTH);		// 1������������̊p�x����

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < 2; nCntHeight++)
	{// �c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < BLOCK_WIDTH; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// ���_���W�̐ݒ�
			float fLength = (FRAME_WIDTH * 0.5f - nCntHeight * (FRAME_WIDTH * 0.5f));
			pVtx[i].pos = pos + m_TextSize + MyLib::Vector3
			(
				sinf(nCntWidth % BLOCK_WIDTH * fRotWidth) * fLength,
				cosf(nCntWidth % BLOCK_WIDTH * fRotWidth) * fLength,
				0.0f
			);

			// rhw�̐ݒ�
			pVtx[i].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[i].col = col;

			// UV���W
			pVtx[i].tex = D3DXVECTOR2
			(
				u,
				(float)nCntHeight
			);

			u += fLength;	// �X�N���[��
		}
	}

	// ���_�o�b�t�@���A�����b�N���b�N
	GetVtxBuff()->Unlock();
}

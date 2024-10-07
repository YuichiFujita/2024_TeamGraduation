//=============================================================================
// 
//  �X�e���V���o�b�t�@�g�p�e[stencilshadow.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "stencilshadow.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStencilShadow::CStencilShadow(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStencilShadow::~CStencilShadow()
{

}

//==========================================================================
// ��������
//==========================================================================
CStencilShadow* CStencilShadow::Create()
{
	// �������̊m��
	CStencilShadow* pObj = DEBUG_NEW CStencilShadow;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();

		// ���ݒ�
		pObj->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		pObj->SetSize(D3DXVECTOR2(640.0f, 360.0f));
		pObj->SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));
		pObj->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f));
		pObj->SetVtx();

	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStencilShadow::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStencilShadow::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CStencilShadow::Kill()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStencilShadow::Update()
{
	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStencilShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@�L���i�X�e���V���g�p�`��j
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �Q�ƒl�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// �o�b�t�@�ւ̒l�ɑ΂��Ẵ}�X�N�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	//�e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	// Z+�X�e���V������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	// Z+�X�e���V�����s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	// Z�e�X�g�̂ݎ��s

	// �`��
	CObject2D::Draw();

	// �X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

